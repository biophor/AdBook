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

#pragma once
#include "qmladbookbl_global.h"
#include "ContactsModel.h"
#include "ContactDetailsModel.h"
#include "SearchFiltersModel.h"
#include "SearchFilterRulesModel.h"
#include "SearchFilterTypesModel.h"
#include "AContactFinder.h"
#include "ADependencyResolver.h"

namespace qmladbook
{
    class QMLADBOOKBL_EXPORT MainWindowViewModel : public QObject
    {
        Q_OBJECT

    public:
        MainWindowViewModel(QObject * parent = nullptr);
        virtual ~MainWindowViewModel();

        void WaitUntilSearchIsFinished();

    public: // Qt/QML specific
        Q_PROPERTY(qmladbook::ContactsModel * contactsModel READ contactsModel CONSTANT);
        qmladbook::ContactsModel * contactsModel();

        Q_PROPERTY(qmladbook::ContactDetailsModel * contactDetailsModel READ contactDetailsModel CONSTANT);
        qmladbook::ContactDetailsModel * contactDetailsModel();

        Q_PROPERTY(qmladbook::SearchFilterTypesModel * searchFilterTypesModel READ searchFilterTypesModel CONSTANT);
        qmladbook::SearchFilterTypesModel * searchFilterTypesModel();

        Q_PROPERTY(qmladbook::SearchFilterRulesModel * searchFilterRulesModel READ searchFilterRulesModel CONSTANT);
        qmladbook::SearchFilterRulesModel * searchFilterRulesModel();

        Q_PROPERTY(qmladbook::SearchFiltersModel * searchFiltersModel READ searchFiltersModel CONSTANT);
        qmladbook::SearchFiltersModel * searchFiltersModel();

        Q_PROPERTY(QString lastError READ lastError WRITE setLastError NOTIFY lastErrorChanged);
        Q_INVOKABLE QString lastError() const;
        Q_INVOKABLE void setLastError(QString error);

        Q_PROPERTY(bool searchFiltersEnabled READ searchFiltersEnabled WRITE enableSearchFilters NOTIFY searchFiltersEnabledChanged);
        Q_INVOKABLE bool searchFiltersEnabled() const;
        Q_INVOKABLE void enableSearchFilters(bool newValue);

        Q_PROPERTY(bool allFilteringRulesMustBeMet READ allFilteringRulesMustBeMet WRITE setAllFilteringRulesMustBeMet NOTIFY allFilteringRulesMustBeMetChanged);
        Q_INVOKABLE bool allFilteringRulesMustBeMet() const;
        Q_INVOKABLE void setAllFilteringRulesMustBeMet(bool newValue);

        Q_PROPERTY(QString contactPhotoUrn READ contactPhotoUrn WRITE setContactPhotoUrn NOTIFY contactPhotoUrnChanged);
        Q_INVOKABLE QString contactPhotoUrn();
        Q_INVOKABLE void setContactPhotoUrn(QString newValue);


        Q_INVOKABLE void copySelectedContactAttr(int row);
        Q_INVOKABLE bool startSearching();
        Q_INVOKABLE void stopSearching();
        Q_INVOKABLE void handleWindowClosing();
        Q_INVOKABLE void handleContactSelected(int row);
        Q_INVOKABLE void handleAttributeSelected(int row);
        Q_INVOKABLE bool addNewFilter(int filterTypeIndex, int filterRuleIndex, QString filterValue);
        Q_INVOKABLE bool removeSelectedFilter(int selectedFilterIndex);
        Q_INVOKABLE void sort(QString roleName, int sortOrder);
        Q_INVOKABLE bool uploadPhotoForSelectedContact(QString filePath);
        Q_INVOKABLE bool clearPhotoForSelectedContact();
        Q_INVOKABLE bool applyAttrModificationForSelectedContact();

    signals:
        void contactPhotoUrnChanged();
        void contactFound();
        void searchFinished(bool errorOccurred);
        void searchStarted();
        void lastErrorChanged();
        void allFilteringRulesMustBeMetChanged();
        void searchFiltersEnabledChanged();
    private slots:
        void onContactFound();
        void onSearchStarted();
        void onSearchFinished();
        void onSearchError(QString errorDesc);
    protected:
        // for testing purposes
        virtual std::shared_ptr<ADependencyResolver> GetDependencyResolver();
    private:
        void SetObsoleteAttributeError();
        void InitializeContactFinder();
        QString ConstructLdapRequest();
        void EnsureContactLoadingStopped();
        void SaveSearchResult();
        void SaveSearchSettings();
        void DisplaySelectedContactPhoto(const qmladbook::Contact & contact);
        void ClearSelectedContactDetails();
        void ClearSelectedContactPhoto();
        void LoadContactsFromLocalCache();
        void LoadSearchSettings();
    private:
        int _currentContactRow{ -1 };
        int _currentAttributeRow{ -1 };
        QString _lastError;
        QString _selectedContactPhotoUrn;
        bool _searchFiltersEnabled{ false };
        bool _allFilteringRulesMustBeMet{ false };

        ContactsModel _contactsModel;
        ContactDetailsModel _contactDetailsModel;
        SearchFilterTypesModel _searchFilterTypesModel;
        SearchFilterRulesModel _searchFilterRulesModel;
        SearchFiltersModel _searchFiltersModel;

        std::shared_ptr<qmladbook::AContactFinder> _contactFinder;
    };
}
