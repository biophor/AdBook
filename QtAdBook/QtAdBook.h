/*
Copyright (C) 2015-2020 Andrei Goncharov.

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

#ifndef QTADBOOK_H
#define QTADBOOK_H

#include "ui_QtAdBook.h"
#include "AppSettings.h"

class FilterNamesModel;
class FilterListModel;
class FilterConditionModel;
class ContactListModel;
class PropertiesModel;

class QtAdBook : public QDialog
{
    Q_OBJECT

public:
    QtAdBook(
        std::shared_ptr<adbook::AbstractAdAccessFactory> adFactory,
        AppSettings & appSettings,
        QWidget *parent = 0
    );
    ~QtAdBook();
protected:
    void closeEvent(QCloseEvent *event);
private:
    void InitSignalConnections();
    void InitSearchResultList();
    void InitFilterListNames();
    void InitConditionList();
    void InitFilterList();
    void InitAdSearcher();
    void InitPropertyList();
    void ContactFoundCallback(adbook::AdPersonDesc && item);
    void SearchStartedCallback();
    void SearchStoppedCallback();
    void SaveState();
    void RestoreState();
    std::wstring ConstructLdapRequest();
    void SaveSuccessfulFilterValue();
    void RefreshNumFoundContacts();
    void UpdatePhotoSizeAfterResizing();
    void SetPhoto(const QPixmap & photo);
signals:
    void SearchStartedSignal();
    void SearchStoppedSignal();
    void OneOrMoreContactsFoundSignal();
private slots:
    void OnAbout();
    void OnSearchStarted();
    void OnSearchStopped();
    void OnOneOrMoreContactsFound();
    void OnClose();
    void OnFindContacts();
    void OnAddFilter();
    void OnRemoveFilter();
    void OnCopyAttribute();
    void OnChangeAttribute();
    void OnSelectPhoto();
    void OnClearPhoto();
    void OnShowSettings();
    void OnFilterSelectionChanged();
    void OnFilterValueTextChanged(const QString & newText);
    void OnHeaderSectionPressed(int logicalIndex);
    void OnAnotherContactSelected(const QModelIndex &current, const QModelIndex &previous);
    void OnAnotherAttributeSelected(const QModelIndex &current, const QModelIndex &previous);
    void OnSplitterMoved(int pos, int index);
private:
    Ui::QtAdBookClass ui;

    std::shared_ptr<adbook::AbstractAdAccessFactory> _adFactory;
    AppSettings & _appSettings;
    std::unique_ptr<adbook::AbstractAdSearcher> _adSearcher;

    FilterConditionModel * _filterConditionModel;
    FilterNamesModel * _filterNamesModel;
    FilterListModel * _filterListModel;
    ContactListModel * _contactListModel;
    PropertiesModel * _propertiesModel;
    QMutex _contactsMutex;
    std::list<adbook::AdPersonDesc> _contacts;
    QTime _prevProcessingTime;
};

#endif // QTADBOOK_H
