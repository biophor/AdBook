import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3 as Dialogs
import QtQuick.Controls.Styles 1.4 as Styles
import QtQuick.Controls 1.4 as Controls
import biophor.local 1.0
import Qt.labs.settings 1.0

Window{
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Active Directory Contact Book (BETA)")
    property bool appExitConfirmed: false

    Settings {
        category: "MainWindowPosition"
        property alias x: root.x
        property alias y: root.y
        property alias width: root.width
        property alias height: root.height
    }
	AboutDlg {
        id: aboutDlg
    }
	SettingsDlg {
        id: settingsDlg
    }
    MainWindowViewModel {
        id: mainWindowViewModel
        onContactFound: {
        }
        onSearchFinished: {
            startSearching.enabled = true
            stopSearching.enabled = false
            busyIndicator.running = false;

            if (errorOccurred == true) {
                messageDialog.text = qsTr("Error occurred during loading")
                messageDialog.informativeText = mainWindowViewModel.lastError
                messageDialog.icon = Dialogs.StandardIcon.Warning
                messageDialog.open()
            }
        }
    }
    Dialogs.MessageDialog {
        id: exitConfirmationDialog
        title: qsTr("Active Directory Contact Book")
        icon: Dialogs.StandardIcon.Question
        text: qsTr("Do you really want to exit?");
        modality: Qt.WindowModal
        standardButtons: Dialogs.StandardButton.Ok | Dialogs.StandardButton.Cancel
        onAccepted: {
            appExitConfirmed = true
            root.close()
        }
    }
    onClosing: {
        if (appExitConfirmed) {
            mainWindowViewModel.handleWindowClosing()
        }
        else {
            exitConfirmationDialog.open()
            close.accepted = false
        }
    }
	ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        Controls.Label {
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            font.bold: true
            font.pointSize: 10
            text: qsTr("Click the 'Load' button to load contacts.")
        }
        SearchResultFragment {
            id: searchResultFragment
            Layout.fillHeight: true
            Layout.fillWidth: true
            mainWindowViewModel: mainWindowViewModel
            searchResultModel: mainWindowViewModel.contactsModel
            contactDetailsModel: mainWindowViewModel.contactDetailsModel
            contactPhotoUrn: mainWindowViewModel.contactPhotoUrn
            onSelectionChanged: {
                mainWindowViewModel.handleContactSelected(rowIndex)
            }
        }
        Item {
            Layout.alignment: Qt.AlignTop
            height: 10
        }

        Controls.CheckBox {
            id: useSearchFilters
            Layout.alignment: Qt.AlignTop
            checked: mainWindowViewModel.searchFiltersEnabled

            Binding {
                target: mainWindowViewModel;
                property: "searchFiltersEnabled";
                value: useSearchFilters.checked
                delayed: true
            }
            style: Styles.CheckBoxStyle {
                label: Controls.Label {
                    font.bold: true
                    font.pointSize: 10
                    text: qsTr("Load contacts matching these conditions:")
                }
            }
        }

        FiltersFragment {
            id: filtersFragment
            enabled: useSearchFilters.checked
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            filterTypesModel: mainWindowViewModel.searchFilterTypesModel
            filterRulesModel: mainWindowViewModel.searchFilterRulesModel
            filtersModel: mainWindowViewModel.searchFiltersModel
            allRules: mainWindowViewModel.allFilteringRulesMustBeMet
            Binding {
                target: mainWindowViewModel;
                property: "allFilteringRulesMustBeMet";
                value: filtersFragment.allRules
                delayed: true
            }
            onAddingFilterRequested: {
                mainWindowViewModel.addNewFilter(filterTypeIndex, filterRuleIndex, filterValue)
            }
            onRemovingFilterRequested: {
                mainWindowViewModel.removeSelectedFilter(selectedFilterIndex)
            }
        }
        RowLayout {
            Layout.margins: 10
            Layout.alignment: Qt.AlignBottom

            Controls.Button {
                id: displayAboutBox
                Layout.minimumWidth: 100
                text: qsTr("About...")
                onClicked: {
                    aboutDlg.open()
                }
            }
            Controls.Button {
                id: displaySettingsDialog
                Layout.minimumWidth: 100
                text: qsTr("Settings...")
                onClicked: {
                    settingsDlg.open()
                }
            }

            Item {
                // empty space between left and right groups of buttons
                Layout.fillWidth: true
            }

            Controls.BusyIndicator {
                id: busyIndicator
                running: false
                Layout.alignment: Qt.AlignRight
            }

            Controls.Button {
                id: startSearching
                Layout.alignment: Qt.AlignRight
                Layout.minimumWidth: 100
                objectName: "startSearching"
                text: qsTr("Load")
                onClicked: {
                    busyIndicator.running = true;
                    if (mainWindowViewModel.startSearching()){
                        enabled = false
                        stopSearching.enabled = true
                        searchResultFragment.clearSelection()
                    }
                    else {
                        busyIndicator.running = false;
                        messageDialog.text = qsTr("Could't load contacts")
                        messageDialog.informativeText = mainWindowViewModel.lastError
                        messageDialog.icon = Dialogs.StandardIcon.Warning
                        messageDialog.open()
                    }
                }
            }

            Controls.Button {
                objectName: "stopSearching"
                Layout.alignment: Qt.AlignRight
                Layout.minimumWidth: 100
                id: stopSearching
                text: qsTr("Stop")
                enabled: false
                onClicked: {
                    mainWindowViewModel.stopSearching()
                    busyIndicator.running = false;
                    enabled = false;
                }
            }

            Controls.Button {
                Layout.alignment: Qt.AlignRight
                Layout.minimumWidth: 100
                id: exitApp
                text: qsTr("Exit...")
                onClicked: {
                    root.close()
                }
            }
        }
	}

    Dialogs.MessageDialog {
        id: messageDialog
        title: qsTr("Active Directory Contact Book")
        modality: Qt.WindowModal
        standardButtons: Dialogs.StandardButton.Ok
    }
}
