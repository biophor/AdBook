/*
Copyright (C) 2015-2021 Andrei Goncharov.

This file is part of the 'Active Directory Contact Book'.
'Active Directory Contact Book' is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

'Active Directory Contact Book' is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along with
'Active Directory Contact Book'. If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdafx.h"
#include "ADependencyResolver.h"
#include "MainWindowViewModel.h"
#include "LdapRequestCreator.h"
#include "SearchFiltersSettings.h"

namespace qmladbook
{
    bool RunningInTestEnvironment() {
        return (QGuiApplication::eventDispatcher() == nullptr);
    }

    MainWindowViewModel::MainWindowViewModel(QObject * parent) : QObject(parent)
    {
        LoadContactsFromLocalCache();
        LoadSearchSettings();
    }

    MainWindowViewModel::~MainWindowViewModel()
    {
        WaitUntilSearchIsFinished();
    }

    void MainWindowViewModel::WaitUntilSearchIsFinished()
    {
        if (_contactFinder && _contactFinder->IsActive()) {
            _contactFinder->Wait();
        }
    }

    QString MainWindowViewModel::lastError() const {
        return _lastError;
    }

    void MainWindowViewModel::setLastError(QString error) {
        _lastError = error;
        emit lastErrorChanged();
    }

    void MainWindowViewModel::InitializeContactFinder()
    {
        Qt::ConnectionType connectionType = Qt::ConnectionType::QueuedConnection;
        if (RunningInTestEnvironment()) {
            // DirectConnection is the only connection type that can be used in my test environment
            // see the AdBookBL_IntegrationTestsPrepareEnvironment project
            connectionType = Qt::ConnectionType::DirectConnection;
        }

        _contactFinder = GetDependencyResolver()->GetContactFinder();
        bool contactFoundConnected = connect(_contactFinder.get(), SIGNAL(contactFound()),
            this, SLOT(onContactFound()), connectionType);

        bool searchStartedConnected = connect(_contactFinder.get(), SIGNAL(searchStarted()),
            this, SLOT(onSearchStarted()), connectionType);

        bool searchFinishedConnected = connect(_contactFinder.get(), SIGNAL(searchFinished()),
            this, SLOT(onSearchFinished()), connectionType);

        bool errorOccurredConnected = connect(_contactFinder.get(), SIGNAL(errorOccurred(QString)),
            this, SLOT(onSearchError(QString)), connectionType);

        if (!contactFoundConnected || !searchStartedConnected
            || !searchFinishedConnected || !errorOccurredConnected)
        {
            throw adbook::HrError(E_UNEXPECTED, L"InitializeContactFinder() failed", __FUNCTIONW__);
        }
    }

    void MainWindowViewModel::onContactFound()
    {
        std::vector<qmladbook::Contact> contacts = _contactFinder->Peek();
        for (qmladbook::Contact & contact : contacts) {
            contactsModel()->AddContact(std::move(contact));
        }
        emit contactFound();
    }

    void MainWindowViewModel::onSearchStarted()
    {
        setLastError("");
        ClearSelectedContactPhoto();
        _contactsModel.Clear();
        _contactDetailsModel.Clear();

        GetDependencyResolver()->GetContactPhotoProvider()->Clear();

        emit searchStarted();
    }

    void MainWindowViewModel::onSearchFinished()
    {
        if (lastError().isEmpty()) {
            emit searchFinished(false);
        }
        else {
            emit searchFinished(true);
        }
    }

    void MainWindowViewModel::onSearchError(QString errorDesc)
    {
        setLastError(errorDesc);
    }

    bool MainWindowViewModel::startSearching()
    {
        try {
            if (!_contactFinder) {
                InitializeContactFinder();
            }
            auto settingsKeeper = GetDependencyResolver()->GetSettingsKeeper();
            qmladbook::ConnectionSettings settings = settingsKeeper->GetConnectionSettings();
            QString ldapRequest = ConstructLdapRequest();
            _contactFinder->Start(ldapRequest, settings);
        }
        catch (const adbook::Error & e)
        {
            MY_TRACE_ERROR(e);
            setLastError(QString::fromWCharArray(e.What()));
            return false;
        }
        catch (const std::exception & e)
        {
            MY_TRACE_STDEXCEPTION(e);
            setLastError(QString::fromLatin1(e.what()));
            return false;
        }
        return true;
    }

    void MainWindowViewModel::stopSearching()
    {
        try
        {
            if (!_contactFinder) {
                throw adbook::HrError(E_UNEXPECTED, L"_contactFinder is nullptr", __FUNCTIONW__);
            }
            if (_contactFinder->IsActive()) {
                _contactFinder->Stop();
            }
        }
        catch (const std::exception & e)
        {
            MY_TRACE_STDEXCEPTION(e);
            setLastError(QString::fromLatin1(e.what()));
        }
    }

    QString MainWindowViewModel::ConstructLdapRequest()
    {
        qmladbook::LdapRequestCreator ldapRequestCreator(
            _searchFiltersModel.GetFilters(),
            _allFilteringRulesMustBeMet
        );
        return ldapRequestCreator.Get();
    }

    qmladbook::ContactsModel * MainWindowViewModel::contactsModel()
    {
        return &_contactsModel;
    }

    void MainWindowViewModel::LoadContactsFromLocalCache()
    {
        auto contactsKeeper = GetDependencyResolver()->GetContactsKeeper();
        contactsKeeper->Load(&_contactsModel);
    }

    void MainWindowViewModel::DisplaySelectedContactPhoto(const qmladbook::Contact & contact)
    {
        qmladbook::AttributeValue photoAttrVal = contact.GetAttr(adbook::Attributes::ThumbnailPhoto);
        if (!std::holds_alternative<adbook::BinaryAttrVal>(photoAttrVal)) {
            setContactPhotoUrn("");
            return;
        }
        adbook::BinaryAttrVal contactPhotoBlob = std::get<adbook::BinaryAttrVal>(photoAttrVal);
        if (contactPhotoBlob.empty()) {
            setContactPhotoUrn("");
            return;
        }
        auto photoProvider = GetDependencyResolver()->GetContactPhotoProvider();
        qmladbook::AttributeValue dnAttrVal = contact.GetAttr(adbook::Attributes::Dn);
        if (!std::holds_alternative<QString>(dnAttrVal)) {
            setContactPhotoUrn("");
            return;
        }
        QString dn = std::get<QString>(dnAttrVal);
        QString photoUrn = photoProvider->SetContactPhoto(dn, contactPhotoBlob);

        setContactPhotoUrn(photoUrn);
    }

    void MainWindowViewModel::handleContactSelected(int row)
    {
        _currentContactRow = row;
        const auto & contact = _contactsModel.GetContact(row);
        _contactDetailsModel.SetContact(contact);
        DisplaySelectedContactPhoto(contact);
    }

    void MainWindowViewModel::handleAttributeSelected(int row)
    {
        _currentAttributeRow = row;
    }

    void MainWindowViewModel::EnsureContactLoadingStopped()
    {
        if (_contactFinder && _contactFinder->IsActive()) {
            _contactFinder->EnsureStopped();
        }
    }

    void MainWindowViewModel::SaveSearchResult()
    {
        auto contactsKeeper = GetDependencyResolver()->GetContactsKeeper();
        contactsKeeper->Save(&_contactsModel);
    }

    void MainWindowViewModel::SaveSearchSettings()
    {
        SearchSettings settings;
        settings.AllFilteringRulesMustBeMet(_allFilteringRulesMustBeMet);
        settings.SetSerializedFilters(searchFiltersModel()->GetSerializedFilters());
        settings.EnableSearchFilters(_searchFiltersEnabled);

        GetDependencyResolver()->GetSettingsKeeper()->SetSearchSettings(settings);
    }

    void MainWindowViewModel::handleWindowClosing()
    {
        EnsureContactLoadingStopped();
        SaveSearchResult();
        SaveSearchSettings();
    }

    void MainWindowViewModel::LoadSearchSettings()
    {
        SearchSettings settings = GetDependencyResolver()->GetSettingsKeeper()->GetSearchSettings();

        setAllFilteringRulesMustBeMet(settings.AllFilteringRulesMustBeMet());
        enableSearchFilters(settings.SearchFiltersEnabled());
        searchFiltersModel()->SetFilters(settings.GetSerializedFilters());
    }

    qmladbook::ContactDetailsModel * MainWindowViewModel::contactDetailsModel()
    {
        return &_contactDetailsModel;
    }

    qmladbook::SearchFilterTypesModel * MainWindowViewModel::searchFilterTypesModel()
    {
        return &_searchFilterTypesModel;
    }

    qmladbook::SearchFilterRulesModel * MainWindowViewModel::searchFilterRulesModel()
    {
        return &_searchFilterRulesModel;
    }

    qmladbook::SearchFiltersModel * MainWindowViewModel::searchFiltersModel()
    {
        return &_searchFiltersModel;
    }

    bool MainWindowViewModel::addNewFilter(int filterTypeIndex, int filterRuleIndex, QString filterValue)
    {
        if (filterValue.isEmpty()) {
            setLastError(tr("filter value could not be empty"));
            return false;
        }
        try {
            qmladbook::SearchFilterType filterType = _searchFilterTypesModel.Get(filterTypeIndex);
            qmladbook::SearchFilterRule filterRule = _searchFilterRulesModel.Get(filterRuleIndex);
            qmladbook::SearchFilter filter(filterType, filterRule, filterValue);
            if (_searchFiltersModel.Exists(filter)) {
                return true;
            }
            _searchFiltersModel.Add(filter);
        }
        catch (const adbook::HrError & e) {
            MY_TRACE_HRERROR(e);
            setLastError(QString::fromWCharArray(e.What()));
            return false;
        }
        return true;
    }

    bool MainWindowViewModel::removeSelectedFilter(int selectedFilterIndex)
    {
        try {
            _searchFiltersModel.Remove(selectedFilterIndex);
        }
        catch (const adbook::HrError & e) {
            MY_TRACE_HRERROR(e);
            setLastError(QString::fromWCharArray(e.What()));
            return false;
        }
        return true;
    }

    bool MainWindowViewModel::searchFiltersEnabled() const
    {
        return _searchFiltersEnabled;
    }

    void MainWindowViewModel::enableSearchFilters(bool newValue)
    {
        _searchFiltersEnabled = newValue;
        emit searchFiltersEnabledChanged();
    }

    bool MainWindowViewModel::allFilteringRulesMustBeMet() const
    {
        return _allFilteringRulesMustBeMet;
    }

    void MainWindowViewModel::setAllFilteringRulesMustBeMet(bool newValue)
    {
        _allFilteringRulesMustBeMet = newValue;
        emit allFilteringRulesMustBeMetChanged();
    }

    QString MainWindowViewModel::contactPhotoUrn()
    {
        if (_selectedContactPhotoUrn.isEmpty()) {
            return "";
        }
        return _selectedContactPhotoUrn;
    }

    void MainWindowViewModel::setContactPhotoUrn(QString newValue)
    {
        _selectedContactPhotoUrn = newValue;
        emit contactPhotoUrnChanged();
    }

    void MainWindowViewModel::ClearSelectedContactDetails()
    {
        _contactDetailsModel.Clear();
    }

    void MainWindowViewModel::ClearSelectedContactPhoto()
    {
        setContactPhotoUrn("");
    }

    void MainWindowViewModel::sort(QString roleName, int sortOrder)
    {
        ClearSelectedContactDetails();
        ClearSelectedContactPhoto();
        _contactsModel.sort(roleName, sortOrder);
    }

    bool MainWindowViewModel::uploadPhotoForSelectedContact(QString filePath)
    {
        try {
            auto contact = _contactDetailsModel.GetContact();
            bool retValue = true;
            try {
                if (filePath.startsWith("file:///", Qt::CaseInsensitive)) {
                    filePath = QUrl(filePath).toLocalFile();
                }
                GetDependencyResolver()->GetContactUpdater()->UpdatePhoto(contact, filePath);
            }
            catch (const ObsoleteAttributeValue & )
            {
                SetObsoleteAttributeError();
                retValue = false;
            }
            catch (const TooBigAttributeValue &)
            {
                setLastError(tr("The file is too big."));
                retValue = false;
            }
            catch (const InvalidFormatAttributeValue &)
            {
                setLastError(tr("The file has an invalid format."));
                retValue = false;
            }
            _contactDetailsModel.SetContact(contact);
            _contactsModel.SetContact(_currentContactRow, contact);
            DisplaySelectedContactPhoto(contact);
            return retValue;
        }
        catch (const adbook::HrError & e) {
            MY_TRACE_HRERROR(e);
            setLastError(tr("Failed to upload the photo. ") + QString::fromWCharArray(e.What()));
            return false;
        }
    }

    bool MainWindowViewModel::clearPhotoForSelectedContact()
    {
        try {
            auto contact = _contactDetailsModel.GetContact();
            bool retValue = true;
            try {
                GetDependencyResolver()->GetContactUpdater()->ClearPhoto(contact);
            }
            catch (const ObsoleteAttributeValue & )
            {
                SetObsoleteAttributeError();
                retValue = false;
            }
            _contactDetailsModel.SetContact(contact);
            _contactsModel.SetContact(_currentContactRow, contact);
            DisplaySelectedContactPhoto(contact);
            return retValue;
        }
        catch (const adbook::HrError & e) {
            MY_TRACE_HRERROR(e);
            setLastError(tr("Failed to clear the photo. ") + QString::fromWCharArray(e.What()));
            return false;
        }
    }

    void MainWindowViewModel::copySelectedContactAttr(int row)
    {
        auto model = contactDetailsModel();
        QVariant vValue = model->data(model->index(row), qmladbook::ContactDetailsModel::AttrValueRole);
        if (vValue.type() == QVariant::String) {
            QString sValue = vValue.toString();

            GetDependencyResolver()->GetClipboardService()->PutText(sValue);
        }
    }

    bool MainWindowViewModel::applyAttrModificationForSelectedContact()
    {
        if (_currentAttributeRow < 0) {
            setLastError(tr("Please, select the attribute you want to change."));
            return false;
        }
        if (_currentContactRow < 0) {
            setLastError(tr("Please, select the contact you want to change."));
            return false;
        }
        try {
            auto dependencyResolver = GetDependencyResolver();
            auto dataExchanger = dependencyResolver->GetDataExchanger();
            QString newValue;
            if (!dataExchanger->Get(ADataExchanger::SenderId::AttrEditor, newValue)) {
                setLastError(tr("Error. A new value was not set."));
                return false;
            }
            auto attrId = _contactDetailsModel.GetAttrId(_currentAttributeRow);
            auto contact = _contactsModel.GetContact(_currentContactRow);

            bool retValue = true;
            try {
                dependencyResolver->GetContactUpdater()->Update(contact, attrId, newValue);
            }
            catch (const ObsoleteAttributeValue & )
            {
                SetObsoleteAttributeError();
                retValue = false;
            }
            catch (const EmptyCommonName &)
            {
                setLastError(tr("'Common Name' cannot be empty or consists of spaces."));
                retValue = false;
            }
            catch (const TooBigAttributeValue &)
            {
                setLastError(tr("The value is too big."));
                retValue = false;
            }
            catch (const InvalidFormatAttributeValue &)
            {
                setLastError(tr("The value has an invalid format."));
                retValue = false;
            }

            _contactsModel.SetContact(_currentContactRow, contact);
            _contactDetailsModel.SetContact(contact);
            return retValue;
        }
        catch (const adbook::Error & e)
        {
            MY_TRACE_ERROR(e);
            setLastError(tr("Failed to change the contact. ") + QString::fromWCharArray(e.What()));
            return false;
        }
    }

    std::shared_ptr<ADependencyResolver> MainWindowViewModel::GetDependencyResolver()
    {
        return qmladbook::GetDependencyResolver();
    }

    void MainWindowViewModel::SetObsoleteAttributeError() {
        setLastError(
            tr("Error. The attribute's value is obsolete. Please, review the actual value and try again.")
        );
    }
}

