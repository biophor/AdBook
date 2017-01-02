/*
Copyright (C) 2015-2017 Goncharov Andrei.

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
#include "AboutDlg.h"
#include "QtAdBook.h"
#include "WaitCursor.h"
#include "SettingsDlg.h"
#include "FilterTypeItem.h"
#include "FilterListModel.h"
#include "ChangeSvAttrDlg.h"
#include "PropertiesModel.h"
#include "TableViewTooltip.h"
#include "ContactListModel.h"
#include "FilterNamesModel.h"
#include "ConnectionSettings.h"
#include "ConnectionSettings.h"
#include "AutoToolTipDelegate.h"
#include "FilterConditionItem.h"
#include "FilterConditionModel.h"


QtAdBook::QtAdBook(QWidget * parent)
    : QDialog(parent), _adSearcher{ ConnectionSettings::Instance() }
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);    
    InitFilterListNames();
    InitFilterList();
    InitConditionList();    
    InitAdSearcher();    
    InitPropertyList();
    InitSearchResultList();
    InitSignalConnections();
    RestoreState();        
}

void QtAdBook::InitSignalConnections()
{
    bool b = connect(ui.exit, SIGNAL(clicked(bool)), SLOT(OnClose()));
    Q_ASSERT(b);
    b = connect(ui.findContacts, SIGNAL(clicked(bool)), SLOT(OnFindContacts()));
    Q_ASSERT(b);
    b = connect(ui.addFilter, SIGNAL(clicked(bool)), SLOT(OnAddFilter()));
    Q_ASSERT(b);
    b = connect(ui.removeFilter, SIGNAL(clicked(bool)), SLOT(OnRemoveFilter()));
    Q_ASSERT(b);
    b = connect(ui.copyProperty, SIGNAL(clicked(bool)), SLOT(OnCopyAttribute()));
    Q_ASSERT(b);
    b = connect(ui.changeProperty, SIGNAL(clicked(bool)), SLOT(OnChangeAttribute()));
    Q_ASSERT(b);
    b = connect(ui.selectPhoto, SIGNAL(clicked(bool)), SLOT(OnSelectPhoto()));
    Q_ASSERT(b);
    b = connect(ui.clearPhoto, SIGNAL(clicked(bool)), SLOT(OnClearPhoto()));
    Q_ASSERT(b);
    b = connect(ui.showSettings, SIGNAL(clicked(bool)), SLOT(OnShowSettings()));
    Q_ASSERT(b);
    b = connect(ui.filters->selectionModel(),
        SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), 
        SLOT(OnFilterSelectionChanged())
    );
    Q_ASSERT(b);
    b = connect(ui.filterValue, SIGNAL(editTextChanged(const QString&)),
        SLOT(OnFilterValueTextChanged(const QString&))
    );
    Q_ASSERT(b);
    b =connect(this, SIGNAL(SearchStartedSignal()), SLOT(OnSearchStarted()));
    Q_ASSERT(b);
    b = connect(this, SIGNAL(SearchStoppedSignal()), SLOT(OnSearchStopped()));
    Q_ASSERT(b);
    b = connect(this, SIGNAL(OneOrMoreContactsFoundSignal()), SLOT(OnOneOrMoreContactsFound()));
    Q_ASSERT(b);
    b = connect(ui.contacts->horizontalHeader(), SIGNAL(sectionPressed(int)), 
        SLOT(OnHeaderSectionPressed(int))
    );
    Q_ASSERT(b);
    b = connect(ui.contacts->selectionModel(),
        SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
        SLOT(OnAnotherContactSelected(const QModelIndex &, const QModelIndex &))
    );
    Q_ASSERT(b);
    b = connect(ui.properties->selectionModel(),
        SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
        SLOT(OnAnotherAttributeSelected(const QModelIndex &, const QModelIndex &))
    );
    Q_ASSERT(b);
    b = connect(ui.mainSplitter, SIGNAL(splitterMoved(int, int)), SLOT(OnSplitterMoved(int,int)));
    Q_ASSERT(b);
    b = connect(ui.contactSplitter, SIGNAL(splitterMoved(int, int)), SLOT(OnSplitterMoved(int, int)));
    Q_ASSERT(b);
    b = connect(ui.propertySplitter, SIGNAL(splitterMoved(int, int)), SLOT(OnSplitterMoved(int, int)));
    Q_ASSERT(b);
    b = connect(ui.about, SIGNAL(clicked()), SLOT(OnAbout()));
    Q_ASSERT(b);
}

void QtAdBook::InitAdSearcher()
{
    _adSearcher.SetCallbacks(
        [this](adbook::AdPersonDesc && arg) { ContactFoundCallback(std::move(arg)); },
        [this]() { SearchStartedCallback(); },
        [this]() { SearchStoppedCallback(); }
    );
}

QtAdBook::~QtAdBook()
{

}

void QtAdBook::OnClose()
{
    close();
}

void QtAdBook::OnFindContacts()
{
    try
    {
        WaitCursor wc;
        if (_adSearcher.IsStarted()) {
            _adSearcher.Stop();
        }
        else {
            _adSearcher.Start(ConstructLdapRequest());
            ui.findContacts->setEnabled(false);
            ui.addFilter->setEnabled(false);
            ui.removeFilter->setEnabled(false);
            ui.changeProperty->setEnabled(false);
            ui.copyProperty->setEnabled(false);
            ui.filterValue->setEnabled(false);
            ui.clearPhoto->setEnabled(false);
            ui.selectPhoto->setEnabled(false);
        }
    }
    catch (const adbook::Error & e)
    {
        QMessageBox::critical(this, QApplication::applicationName(), QString::fromStdWString(e.What()), 
            QMessageBox::Warning, QMessageBox::Ok);
    }
    catch (const std::exception & e)
    {
        QMessageBox::critical(this, QApplication::applicationName(), e.what(),
            QMessageBox::Warning, QMessageBox::Ok);
    }
}

void QtAdBook::OnCopyAttribute()
{
    auto currentIndex = ui.properties->currentIndex();
    if (!currentIndex.isValid()) {
        return;
    }
    QString value = _propertiesModel->item(currentIndex.row(), PropertiesModel::AttrValueColId)->text();
    QApplication::clipboard()->setText(value);
}

void QtAdBook::OnChangeAttribute()
{
    auto currentAttrIndex = ui.properties->currentIndex();    
    if (!currentAttrIndex.isValid()) {
        return;
    }
    auto currentContactIndex = ui.contacts->currentIndex();
    if (!currentContactIndex.isValid()) {
        return;
    }
    auto attrId = _propertiesModel->GetAttr(currentAttrIndex.row());
    auto contact = _contactListModel->GetContact(currentContactIndex.row());
    auto dn = QString::fromStdWString(contact.GetDn());
    auto currentValue = QString::fromStdWString(contact.GetStringAttr(attrId));
    ChangeSvAttrDlg dlg(dn, currentValue, attrId, this);
    int res = dlg.exec();
    if (res != QDialog::Accepted) {
        return;
    }
    QString newValue = dlg.GetNewValue();
    if (newValue == currentValue) {
        return;
    }
    const auto attrName =
        adbook::Attributes::GetInstance().GetLdapAttrName(attrId);
    try {
        adbook::AdConnector ac{ ConnectionSettings::Instance(), contact.GetDn() };
        WaitCursor wc;
        ac.Connect();
        ac.UploadStringAttr(attrName, newValue.toStdWString());
        contact.SetStringAttr(attrName, newValue.toStdWString());
        _contactListModel->SetContact(currentContactIndex.row(), contact);
        OnAnotherContactSelected(currentContactIndex, currentContactIndex);
    }
    catch (const adbook::Error & e) {
        QMessageBox::warning(this, QApplication::applicationName(), QString::fromStdWString(e.What()));
    }
    catch (const std::exception & e) {
        QMessageBox::warning(this, QApplication::applicationName(), QString::fromStdString(e.what()));
    }
}

void QtAdBook::OnSelectPhoto()
{
    QFileDialog dlg(this);
    dlg.setWindowTitle(tr("Select a photo(size <= 100kb)"));
    dlg.setFileMode(QFileDialog::ExistingFile);
    dlg.setNameFilter(tr("JPEG Images (*.jpeg *.jpg)"));
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    if (QDialog::Accepted != dlg.exec()) {
        return;
    }
    QString photoFileName = dlg.selectedFiles().first();
    const auto photoMaxSizeInBytes =
        adbook::Attributes::GetInstance().GetAttrMaxLength(adbook::Attributes::thumbnailPhoto);
    QFile photoFile(photoFileName);    
    if (!photoFile.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, QApplication::applicationName(),
            tr("Failed to open a file."));
        return;
    }
    auto photoFileSize = QFileInfo(photoFileName).size();
    if (0 == photoFileSize) {
        QMessageBox::warning(this, QApplication::applicationName(),
            tr("A file is empty."));
        return;
    }
    if (boost::numeric_cast<decltype(photoMaxSizeInBytes)>(photoFileSize) > photoMaxSizeInBytes) {
        QMessageBox::warning(this, QApplication::applicationName(),
            tr("File size should be less than 100kb"));
        return;
    }
    QByteArray ba = photoFile.readAll();    
    adbook::BinaryAttrVal photoFileData(photoFileSize);
    std::transform(ba.begin(), ba.end(), 
        photoFileData.begin(), [](char ch) { return static_cast<BYTE>(ch); });
    
    QModelIndex currentIndex = ui.contacts->currentIndex();
    if (!currentIndex.isValid()) {
        return;
    }
    auto contact = _contactListModel->GetContact(currentIndex.row());    
    const auto dn = contact.GetDn();
    const auto attrName = 
        adbook::Attributes::GetInstance().GetLdapAttrName(adbook::Attributes::thumbnailPhoto);
    adbook::AdConnector ac{ ConnectionSettings::Instance(), dn };
    try {
        WaitCursor wc;
        ac.Connect();
        ac.UploadBinaryAttr(attrName, photoFileData);
        contact.SetBinaryAttr(attrName, photoFileData);
        _contactListModel->SetContact(currentIndex.row(), contact);
        OnAnotherContactSelected(currentIndex, currentIndex);
    }
    catch (const adbook::Error & e) {        
        QMessageBox::warning(this, QApplication::applicationName(), QString::fromStdWString(e.What()));
    }
    catch (const std::exception & e) {
        QMessageBox::warning(this, QApplication::applicationName(), QString::fromStdString(e.what()));
    }
}

void QtAdBook::OnClearPhoto()
{
    QModelIndex currentIndex = ui.contacts->currentIndex();
    if (!currentIndex.isValid()) {
        return;
    }
    auto contact = _contactListModel->GetContact(currentIndex.row());
    const auto dn = contact.GetDn();
    const auto attrName =
        adbook::Attributes::GetInstance().GetLdapAttrName(adbook::Attributes::thumbnailPhoto);
    adbook::AdConnector ac{ ConnectionSettings::Instance(), dn };
    try {
        WaitCursor wc;
        ac.Connect();
        ac.UploadBinaryAttr(attrName, adbook::BinaryAttrVal());
        contact.SetBinaryAttr(attrName, adbook::BinaryAttrVal());
        _contactListModel->SetContact(currentIndex.row(), contact);
        OnAnotherContactSelected(currentIndex, currentIndex);
    }
    catch (const adbook::Error & e) {
        QMessageBox::warning(this, QApplication::applicationName(), QString::fromStdWString(e.What()));
    }
    catch (const std::exception & e) {
        QMessageBox::warning(this, QApplication::applicationName(), QString::fromStdString(e.what()));
    }
}

void QtAdBook::OnShowSettings()
{
    SettingsDlg dlg(this);
    dlg.exec();    
}

void QtAdBook::InitFilterListNames()
{
    _filterNamesModel = new FilterNamesModel(ui.filterNames);    
    ui.filterNames->setModel(_filterNamesModel);
}

void QtAdBook::InitFilterList()
{
    _filterListModel = new FilterListModel(ui.filterNames);        
    ui.filters->setModel(_filterListModel);    
    ui.filters->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.filters->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.filters->setItemDelegate(new AutoToolTipDelegate(ui.filters));
}

void QtAdBook::OnAddFilter()
{    
    int row = _filterListModel->AddFilter (
        _filterNamesModel->item(ui.filterNames->currentIndex())->clone(),
        _filterConditionModel->item(ui.conditions->currentIndex())->clone(),
        ui.filterValue->currentText().trimmed()
    );        
    ui.filters->resizeRowToContents(row);         
}

void QtAdBook::OnRemoveFilter()
{
    auto selModel = ui.filters->selectionModel();
    if (selModel->hasSelection()) {
        _filterListModel->removeRow(selModel->selectedRows().at(0).row());
    }
}

void QtAdBook::InitConditionList()
{
    _filterConditionModel = new FilterConditionModel(ui.conditions);
    ui.conditions->setModel(_filterConditionModel);
    ui.conditions->setCurrentIndex(0);
}

void QtAdBook::closeEvent(QCloseEvent * /*event*/ )
{
    /*
    int answer = QMessageBox::information(this, QApplication::applicationName(), 
        tr("Do you realy want to exit?"), QMessageBox::Ok | QMessageBox::Cancel);
    if (answer == QMessageBox::Cancel) {
        event->ignore();
    }
    else {
        event->accept();
    }
    */
    WaitCursor wc;

    if (_adSearcher.IsStarted()) {
        _adSearcher.Stop();
        try {
            _adSearcher.Wait();
        }
        catch (const std::exception&) { //-V565
          // user wants to exit. An error message should not be displayed.
        }
    }    
    SaveState();    
}

void QtAdBook::OnFilterSelectionChanged() {
    ui.removeFilter->setEnabled(ui.filters->selectionModel()->hasSelection());
}

void QtAdBook::OnFilterValueTextChanged(const QString & newText) {
    ui.addFilter->setEnabled(!newText.trimmed().isEmpty());
}

void QtAdBook::OnHeaderSectionPressed(int /*logicalIndex*/)
{
    ui.contacts->horizontalHeader()->setSortIndicatorShown(true);
}

void QtAdBook::ContactFoundCallback(adbook::AdPersonDesc && item) 
{
    QMutexLocker locker(&_contactsMutex);
    _contacts.push_back(std::move(item));

    QTime currentTime = QTime::currentTime();
    if (_prevProcessingTime.secsTo(currentTime) > 1) {  // handle next portion each second
        emit OneOrMoreContactsFoundSignal();
        _prevProcessingTime = currentTime;
    }    
}

void QtAdBook::SearchStartedCallback() {    
    emit SearchStartedSignal();
}

void QtAdBook::SearchStoppedCallback() {
    emit SearchStoppedSignal();
}

void QtAdBook::OnSearchStarted()
{
    _contactListModel->Clear();    
    _propertiesModel->Clear();
    ui.contacts->horizontalHeader()->setSortIndicatorShown(false);
    ui.groupBoxContacts->setTitle("Contacts:");
    _prevProcessingTime = QTime::currentTime();
    ui.findContacts->setEnabled(true);
    ui.findContacts->setText(tr("Stop"));        
}

void QtAdBook::OnSearchStopped()
{
    ui.findContacts->setText(tr("Find"));
    ui.addFilter->setEnabled(true);
    ui.removeFilter->setEnabled(ui.filters->selectionModel()->hasSelection());
    ui.filterValue->setEnabled(true);
    
    if (!_contacts.empty()) {
        emit OneOrMoreContactsFoundSignal();
    }
    int numContacts = _contactListModel->rowCount();
    if (numContacts > 0) {
        SaveSuccessfulFilterValue();
    }    
}

void QtAdBook::SaveSuccessfulFilterValue()
{
    QStringList filterValues = _filterListModel->GetFilterValues();
    for (QString filterValue : filterValues) {
        if (-1 == ui.filterValue->findText(filterValue)) {
            ui.filterValue->addItem(filterValue);
        }
    }
}

void QtAdBook::OnOneOrMoreContactsFound()
{
    {
        QMutexLocker locker(&_contactsMutex);
        _contactListModel->AddContacts(std::move(_contacts));
        _contacts.clear();
    }
    RefreshNumFoundContacts();
}

void QtAdBook::RefreshNumFoundContacts()
{
    int numContacts = _contactListModel->rowCount();
    ui.groupBoxContacts->setTitle(QString("Contacts(%1):").arg(numContacts));
}

adbook::LdapRequest QtAdBook::ConstructLdapRequest() {
    return _filterListModel->ConstructLdapRequest(ui.allFilters->isChecked());
}

void QtAdBook::InitSearchResultList()
{
    _contactListModel = new ContactListModel(ui.contacts);    
    ui.contacts->setModel(_contactListModel);    
    ui.contacts->setItemDelegate(new AutoToolTipDelegate(ui.contacts));
    ui.contacts->horizontalHeader()->setSectionsMovable(true);    
}

namespace {
char origUserPhotoPixmapPropertyName[] = "origPixmap";
}

void QtAdBook::OnAnotherContactSelected(const QModelIndex &current, const QModelIndex &/*previous*/)
{
    if (current.isValid()) {
        auto contact = _contactListModel->GetContact(current.row());
        bool canChangePhoto = contact.IsAttributeWritable(adbook::Attributes::thumbnailPhoto);
        ui.selectPhoto->setEnabled(canChangePhoto);
        ui.clearPhoto->setEnabled(canChangePhoto);
        _propertiesModel->SetContact(std::move(contact));
        adbook::BinaryAttrVal photoBytes = contact.GetBinaryAttr(adbook::Attributes::thumbnailPhoto);        
        ui.photo->clear();
        ui.photo->setProperty(origUserPhotoPixmapPropertyName, QVariant());
        if (!photoBytes.empty()) {
            QPixmap photoPixmap;
            if (photoPixmap.loadFromData(&photoBytes[0], boost::numeric_cast<uint>(photoBytes.size()))) {
                ui.photo->setProperty(origUserPhotoPixmapPropertyName, photoPixmap);
                SetPhoto(photoPixmap);
            }
        }
    }
}

void QtAdBook::OnAnotherAttributeSelected(const QModelIndex &current, const QModelIndex &/*previous*/)
{
    if (current.isValid()) {
        ui.changeProperty->setEnabled(_propertiesModel->IsAttrWritable(current.row()));        
        
        ui.copyProperty->setEnabled(!_propertiesModel->item(
            current.row(), PropertiesModel::AttrValueColId)->text().isEmpty());
    }
}

void QtAdBook::OnSplitterMoved(int /*pos*/, int /*index*/)
{
    UpdatePhotoSizeAfterResizing();
}

void QtAdBook::UpdatePhotoSizeAfterResizing()
{
    QVariant varOrigPixmap = ui.photo->property(origUserPhotoPixmapPropertyName);
    if (!varOrigPixmap.isValid()) {
        return;
    }
    QPixmap origPixmap = qvariant_cast<QPixmap>(varOrigPixmap);
    SetPhoto(origPixmap);    
}

void QtAdBook::SetPhoto(const QPixmap & photo)
{
    QSize sz = ui.photo->size();
    int h = static_cast<int>(sz.height()*0.95);
    int w = static_cast<int>(sz.width()*0.95);
    QPixmap scaledPhoto = photo.scaled(w, h, Qt::KeepAspectRatio);
    ui.photo->setPixmap(scaledPhoto);
}

void QtAdBook::InitPropertyList()
{
    _propertiesModel = new PropertiesModel(ui.properties);    
    ui.properties->setItemDelegate(new AutoToolTipDelegate(ui.properties));
    ui.properties->setModel(_propertiesModel);
    ui.properties->resizeRowsToContents();
}

namespace MainWndSettings
{
const QString sectionName = "MainWindow";
const QString wndPosParam = "MainWindowPosition";
const QString contactSplitterParam = "ContactSplitter";
const QString mainSplitterParam = "MainSplitter";
const QString prevSearchAttrValuesParam = "PrevSearchAttrValues";
const QString wndMaximizedParam = "maximixed";
const QString filterTopLevelRuleParam = "AllTheConditionsShouldBeMet";
const QString contactsHeaderStateParam = "ContactsHeaderState";
const QString filtersHeaderStateParam = "FiltersHeaderState";
const QString propertiesHeaderStateParam = "PropertiesHeaderState";
}

void QtAdBook::SaveState()
{
    using namespace MainWndSettings;
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.beginGroup(sectionName);
        settings.setValue(filterTopLevelRuleParam, ui.allFilters->isChecked());
        settings.setValue(wndMaximizedParam, isMaximized());
        settings.setValue(wndPosParam, geometry());
        auto IntListToVarList = [](QList<int> il) {
            QVariantList r; for (int sz : il) { r.append(sz); } return r; 
        };
        settings.setValue(contactSplitterParam, IntListToVarList(ui.contactSplitter->sizes()));
        settings.setValue(mainSplitterParam, IntListToVarList(ui.mainSplitter->sizes()));
        QStringList filterValueItems;
        for (int i = 0, m = ui.filterValue->count(); i < m; ++i) {
            filterValueItems.append(ui.filterValue->itemText(i));
        }
        settings.setValue(prevSearchAttrValuesParam, filterValueItems);
        settings.setValue(contactsHeaderStateParam, ui.contacts->horizontalHeader()->saveState());
        settings.setValue(filtersHeaderStateParam, ui.filters->horizontalHeader()->saveState());
        settings.setValue(propertiesHeaderStateParam, ui.properties->horizontalHeader()->saveState());        
    settings.endGroup();
    _filterListModel->Save();
    _contactListModel->Save();
}

void QtAdBook::RestoreState()
{
    using namespace MainWndSettings;
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());    
    settings.beginGroup(sectionName);
        if (settings.contains(filterTopLevelRuleParam)) {
            if (settings.value(filterTopLevelRuleParam).toBool() == true) {
                ui.allFilters->setChecked(true);
            }
            else {
                ui.oneFilter->setChecked(true);
            }
        }
        if (settings.value(wndMaximizedParam).toBool() == true) {
            showMaximized();
        }
        else if (settings.contains(wndPosParam)) {
            setGeometry(settings.value(wndPosParam, geometry()).toRect());
        }
        auto VarListToIntList = [](QVariantList vl) {
            QList<int> r; for (auto v : vl) r.push_back(v.toInt()); return r;
        };
        if (settings.contains(contactSplitterParam)) {
            QVariantList vl = settings.value(contactSplitterParam).toList();
            ui.contactSplitter->setSizes(VarListToIntList(vl));
        }
        if (settings.contains(mainSplitterParam)) {
            QVariantList vl = settings.value(mainSplitterParam).toList();
            ui.mainSplitter->setSizes(VarListToIntList(vl));
        }
        if (settings.contains(prevSearchAttrValuesParam)) {
            ui.filterValue->clear();
            ui.filterValue->addItems(settings.value(prevSearchAttrValuesParam).toStringList());
            ui.filterValue->setCurrentText("");
        }
        if (settings.contains(contactsHeaderStateParam)) {
            ui.contacts->horizontalHeader()->restoreState(settings.value(contactsHeaderStateParam).toByteArray());
        }
        if (settings.contains(filtersHeaderStateParam)) {
            ui.filters->horizontalHeader()->restoreState(settings.value(filtersHeaderStateParam).toByteArray());
        }
        if (settings.contains(propertiesHeaderStateParam)) {
            ui.properties->horizontalHeader()->restoreState(settings.value(propertiesHeaderStateParam).toByteArray());
        }        
    settings.endGroup();
    try {
        _contactListModel->Load();
        _contactListModel->rowCount();
        RefreshNumFoundContacts();
    }
    catch (const std::exception &) {
        QMessageBox::warning(this, QApplication::applicationName(), tr("Failed to load contacts."));
    }    
    _filterListModel->Load();
    ui.filters->resizeRowsToContents();    
}

void QtAdBook::OnAbout()
{    
    AboutDlg dlg(this);
    dlg.exec();
}

