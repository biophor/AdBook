import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3 as Dialogs
import QtQuick.Controls 1.4 as Controls
import QtQuick.Controls.Styles 1.4 as Styles
import Qt.labs.settings 1.0

RowLayout {
    id: root
    Layout.fillHeight: true
    Layout.fillWidth: true
    property var mainWindowViewModel
    property alias searchResultModel: searchResultTableView.model
    property alias contactDetailsModel: contactDetailsTableView.model
    property alias contactPhotoUrn: photoImage.source
    property var contactDetailsSelectedRow: -1

    signal selectionChanged(int rowIndex)

    // SplitView with two sections: a list of contacts on the left; the contact's details on the right
    Controls.SplitView {
        Layout.fillWidth: true
        Layout.fillHeight: true
        orientation: Qt.Horizontal

        Controls.TableView {
            id: searchResultTableView
            sortIndicatorVisible: true
            sortIndicatorColumn: -1
            Layout.fillWidth: true
            Layout.minimumWidth: 150
            Layout.minimumHeight: 100
            selectionMode: Controls.SelectionMode.SingleSelection
            property string rolesOrder: ""
            rowDelegate: TableViewRowDelegate{}

            onSortIndicatorColumnChanged: {
                var column = getColumn(searchResultTableView.sortIndicatorColumn)
                mainWindowViewModel.sort(column.role,
                    searchResultTableView.sortIndicatorOrder)
                clearSelection()
            }
            onSortIndicatorOrderChanged: {
                var column = getColumn(searchResultTableView.sortIndicatorColumn)
                mainWindowViewModel.sort(column.role,
                    searchResultTableView.sortIndicatorOrder)
                clearSelection()
            }

            Controls.TableViewColumn {
                id: cnColumn
                title: qsTr("Common name")
                role: "CommonName"
                movable: false
                resizable: true
                width: 100
                delegate: TableViewItemDelegate{
                    width: parent.width
                }
            }
            Controls.TableViewColumn {
                id: emailColumn
                title: qsTr("E-mail")
                role: "Email"
                movable: true
                resizable: true
                width: 100
                delegate: TableViewItemDelegate{
                    width: parent.width
                }
            }

            Controls.TableViewColumn {
                id: companyColumn
                title: qsTr("Company")
                role: "Company"
                movable: true
                resizable: true
                width: 100
                delegate: TableViewItemDelegate{
                    width: parent.width
                }
            }
            Controls.TableViewColumn {
                id: titleColumn
                title: qsTr("Title")
                role: "Title"
                movable: true
                resizable: true
                width: 100
                delegate: TableViewItemDelegate{
                    width: parent.width
                }
            }
            Controls.TableViewColumn {
                id: departmentColumn
                title: qsTr("Department")
                role: "Department"
                movable: true
                resizable: true
                width: 100
                delegate: TableViewItemDelegate{
                    width: parent.width
                }
            }
            Controls.TableViewColumn {
                id: stateOrProvinceColumn
                title: qsTr("State(province)")
                role: "StateOrProvince"
                movable: true
                resizable: true
                width: 100
                delegate: TableViewItemDelegate{
                    width: parent.width
                }
            }
            Controls.TableViewColumn {
                id: employeeIdColumn
                title: qsTr("Employee ID")
                role: "EmployeeID"
                movable: true
                resizable: true
                width: 100
                delegate: TableViewItemDelegate{
                    width: parent.width
                }
            }
            Controls.TableViewColumn {
                id: surnameColumn
                title: qsTr("Surname(last name)")
                role: "SurName"
                movable: true
                resizable: true
                width: 100
                delegate: TableViewItemDelegate{
                    width: parent.width
                }
            }
            Controls.TableViewColumn {
                id: givenNameColumn
                title: qsTr("Given name(first name)")
                role: "GivenName"
                movable: true
                resizable: true
                width: 100
                delegate: TableViewItemDelegate{
                    width: parent.width
                }
            }
            Controls.TableViewColumn {
                id: displayNameColumn
                title: qsTr("Display name")
                role: "DisplayName"
                movable: true
                resizable: true
                width: 100
                delegate: TableViewItemDelegate{
                    width: parent.width
                }
            }
            Controls.TableViewColumn {
                id: workPhoneColumn
                title: qsTr("Work phone")
                role: "WorkPhone"
                movable: true
                resizable: true
                width: 100
                delegate: TableViewItemDelegate{
                    width: parent.width
                }
            }
            Controls.TableViewColumn {
                id: homePhoneColumn
                title: qsTr("Home phone")
                role: "HomePhone"
                movable: true
                resizable: true
                width: 100
                delegate: TableViewItemDelegate{
                    width: parent.width
                }
            }
            Controls.TableViewColumn {
                id: mobilePhoneColumn
                title: qsTr("Mobile phone")
                role: "MobilePhone"
                movable: true
                resizable: true
                width: 100
                delegate: TableViewItemDelegate{
                    width: parent.width
                }
            }
            Controls.TableViewColumn {
                id: localityOrCityColumn
                title: qsTr("Locality(city)")
                role: "Locality"
                movable: true
                resizable: true
                width: 100
                delegate: TableViewItemDelegate{
                    width: parent.width
                }
            }
            Settings {
                category: "SearchResultTableViewColumns"
                property alias cnColumnWidth: cnColumn.width
                property alias emailColumnWidth: emailColumn.width
                property alias companyColumnWidth: companyColumn.width
                property alias titleColumnWidth: titleColumn.width
                property alias departmentColumnWidth: departmentColumn.width
                property alias stateOrProvinceColumnWidth: stateOrProvinceColumn.width
                property alias employeeIdColumnWidth: employeeIdColumn.width
                property alias surnameColumnWidth: surnameColumn.width
                property alias givenNameColumnWidth: givenNameColumn.width
                property alias displayNameColumnWidth: displayNameColumn.width
                property alias workPhoneColumnWidth: workPhoneColumn.width
                property alias homePhoneColumnWidth: homePhoneColumn.width
                property alias mobilePhoneColumnWidth: mobilePhoneColumn.width
                property alias localityOrCityColumnWidth: localityOrCityColumn.width
                property alias rolesOrder: searchResultTableView.rolesOrder
            }
            Component.onCompleted: {
                restoreColumnOrder()
            }
            Component.onDestruction: {
                saveColumnOrder()
            }
            Connections {
                target: searchResultTableView.selection
                onSelectionChanged: {
                    if (searchResultTableView.rowCount == 0){
                        return;
                    }
                    searchResultTableView.selection.forEach(
                        function(rowIndex) {
                            root.selectionChanged(rowIndex)
                        }
                    )
                }
            }
        }

        // Contact's details and his/her photo
        Controls.SplitView {
            id: contactDetailsSplitView
            orientation: Qt.Vertical
            Layout.minimumWidth: 150

            // Contact's details and two buttons: 'Copy' and 'Edit'
            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumHeight: 100

                // Contact's details
                Controls.TableView {
                    id: contactDetailsTableView
                    sortIndicatorVisible: false
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    rowDelegate: TableViewRowDelegate{}

                    Controls.TableViewColumn {
                        id: attrIdColumn
                        title: qsTr("Attribute")
                        role: "AttrName"
                        movable: false
                        resizable: true
                        width: 150
                        delegate: TableViewItemDelegate{
                            width: parent.width
                        }
                    }
                    Controls.TableViewColumn {
                        id: attrValueColumn
                        title: qsTr("Value")
                        role: "AttrValue"
                        movable: false
                        resizable: true
                        width: 150
                        delegate: TableViewItemDelegate{
                            width: parent.width
                        }
                    }
                    Settings {
                        category: "ContactDetailsTableViewColumns"
                        property alias attrIdColumnWidth: attrIdColumn.width
                        property alias attrValueColumnWidth: attrValueColumn.width
                    }
                    Connections {
                        target: contactDetailsTableView.selection
                        onSelectionChanged: {
                            evaluateContactDetailsSelectedRow()
                            mainWindowViewModel.handleAttributeSelected(contactDetailsSelectedRow)
                        }
                    }
                }
                // Buttons 'Copy' and 'Edit'
                RowLayout {
                    Item {
                        Layout.fillWidth: true
                    }
                    Controls.Button {
                        id: copySelectedAttribute
                        Layout.alignment: Qt.AlignRight
                        Layout.minimumWidth: 100
                        text: qsTr("Copy")
                        enabled: {
                            return (searchResultTableView.selection.count > 0) &&
                                   (contactDetailsTableView.selection.count > 0)
                        }
                        onClicked: {
                            contactDetailsTableView.selection.forEach (
                                function(rowIndex) {
                                    mainWindowViewModel.copySelectedContactAttr(rowIndex);
                                }
                            )
                        }
                    }
                    Controls.Button {
                        id: changeSelectedAttribute
                        Layout.alignment: Qt.AlignRight
                        Layout.minimumWidth: 100
                        text: qsTr("Change")
                        enabled: {
                            return (searchResultTableView.selection.count > 0) &&
                                   (contactDetailsTableView.selection.count > 0) &&
                                   (contactDetailsSelectedRow >= 0) &&
                                   contactDetailsModel.isAttributeWriteable(contactDetailsSelectedRow)
                        }
                        onClicked: {
                            attrEditDlg.contactDetailsModel = contactDetailsModel
                            attrEditDlg.contactDetailsModelRow = contactDetailsSelectedRow
                            attrEditDlg.load()
                            attrEditDlg.open()
                        }
                    }
                    AttrEditorDlg {
                        id: attrEditDlg
                        onAccepted: {
                            if (!mainWindowViewModel.applyAttrModificationForSelectedContact()) {
                                messageDialog.text = qsTr("Failed to change the attribute.")
                                messageDialog.informativeText = mainWindowViewModel.lastError
                                messageDialog.icon = Dialogs.StandardIcon.Warning
                                messageDialog.open()
                            }
                        }
                    }
                }
            }
            ColumnLayout {
                id: photo
                Layout.fillWidth: true
                Rectangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    border.color: "grey"
                    Layout.minimumHeight: 50
                    Image {
                        id: photoImage
                        fillMode: Image.PreserveAspectFit
                        anchors.fill: parent
                        onSourceChanged: {
                            noPhotoLabel.visible = (photoImage.source == "")
                        }
                        Controls.Label {
                            id: noPhotoLabel
                            text: qsTr("There is no photo")
                            anchors.centerIn: parent
                        }
                    }
                }
                RowLayout {
                    Layout.fillWidth: true
                    Item {
                        Layout.fillWidth: true
                    }

                    Controls.Button {
                        text: qsTr("Clear the photo")
                        Layout.minimumWidth: 100
                        enabled: searchResultTableView.selection.count > 0 && noPhotoLabel.visible == false
                        onClicked: {
                            photoRemovingConfirmationDialog.open()
                        }
                    }
                    Dialogs.MessageDialog {
                        id: photoRemovingConfirmationDialog
                        text: qsTr("The contact's photo is about to be removed. Continue?")
                        title: qsTr("Active Directory Contact Book")
                        icon: Dialogs.StandardIcon.Question
                        modality: Qt.WindowModal
                        // 'Yes/No' is used instead of 'Ok/Cancel' to prevent double call the onAccepted() handler.
                        //standardButtons: Dialogs.StandardButton.Ok | Dialogs.StandardButton.Cancel
                        standardButtons: Dialogs.StandardButton.Yes | Dialogs.StandardButton.No
                        onAccepted: {
                            if (!mainWindowViewModel.clearPhotoForSelectedContact()){
                                messageDialog.text = qsTr("Failed to remove the photo.")
                                messageDialog.informativeText = mainWindowViewModel.lastError
                                messageDialog.icon = Dialogs.StandardIcon.Warning
                                messageDialog.open()
                            }
                        }
                    }
                    Controls.Button {
                        text: qsTr("Select a photo...")
                        Layout.minimumWidth: 100
                        enabled: searchResultTableView.selection.count > 0
                        onClicked: {
                            openContactPhotoDialog.open()
                        }
                    }
                    Dialogs.FileDialog {
                        id: openContactPhotoDialog
                        selectExisting: true
                        selectMultiple: false
                        nameFilters: [ qsTr("Jpeg files (*.jpg *.jpeg)") ]
                        title: qsTr("Select a jpg-file. The size must be less than 100 kilobytes.")
                        folder: shortcuts.home
                        onAccepted: {
                            photoUploadingConfirmationDialog.open()
                        }
                    }
                    Dialogs.MessageDialog {
                        id: photoUploadingConfirmationDialog
                        text: qsTr("The contact's photo is about to be replaced. Continue?")
                        title: qsTr("Active Directory Contact Book")
                        icon: Dialogs.StandardIcon.Question
                        modality: Qt.WindowModal
                        // The following line causes the onAccepted() handler to be called twice.
                        //standardButtons: Dialogs.StandardButton.Ok | Dialogs.StandardButton.Cancel
                        standardButtons: Dialogs.StandardButton.Yes | Dialogs.StandardButton.No
                        onAccepted: {
                            if (!mainWindowViewModel.uploadPhotoForSelectedContact(openContactPhotoDialog.fileUrl)){
                                messageDialog.text = qsTr("Failed to upload the photo.")
                                messageDialog.informativeText = mainWindowViewModel.lastError
                                messageDialog.icon = Dialogs.StandardIcon.Warning
                                messageDialog.open()
                            }
                        }
                    }
                }
            }
        }
    }

    Settings {
        category: "SeachResultFragment"
        property alias contactDetailsSplitViewWidth: contactDetailsSplitView.width
        property alias photoHeight: photo.height
    }

    Dialogs.MessageDialog {
        id: messageDialog
        title: qsTr("Active Directory Contact Book")
        modality: Qt.WindowModal
        standardButtons: Dialogs.StandardButton.Ok
    }

    function restoreColumnOrder()
    {
        if (searchResultTableView.rolesOrder.length == 0){
            return;
        }
        var rolesArray = searchResultTableView.rolesOrder.split(',')

        for (var i = 0; i < searchResultTableView.columnCount; i++) {
            var column = searchResultTableView.getColumn(i);
            if (column.movable === false) {
                continue
            }
            if (column.role === rolesArray[i]) {
                continue
            }
            for (var j = i; j < searchResultTableView.columnCount; j++) {
                var tmpColumn = searchResultTableView.getColumn(j);
                if (tmpColumn.role === rolesArray[i]) {
                    searchResultTableView.moveColumn(j, i)
                    break
                }
            }
        }
    }

    function saveColumnOrder()
    {
        searchResultTableView.rolesOrder = ""
        for (var i = 0; i < searchResultTableView.columnCount; i++){
            var column = searchResultTableView.getColumn(i);

            if (searchResultTableView.rolesOrder.length > 0) {
                searchResultTableView.rolesOrder += ","
            }
            searchResultTableView.rolesOrder += column.role
        }
    }

    function getSelectedContactRow() {
        if (searchResultTableView.rowCount == 0) {
            return -1;
        }
        var selectedContactRow = -1
        searchResultTableView.selection.forEach(
            function(rowIndex) {
                selectedContactRow = rowIndex
            }
        )
        return selectedContactRow;
    }

    function evaluateContactDetailsSelectedRow() {
        contactDetailsSelectedRow = -1
        if (contactDetailsTableView.rowCount == 0){
            return;
        }
        contactDetailsTableView.selection.forEach(
            function(rowIndex) {
                contactDetailsSelectedRow = rowIndex
            }
        )
    }

    function clearSelection() {
        searchResultTableView.selection.clear()
        contactDetailsTableView.selection.clear()
    }
}
