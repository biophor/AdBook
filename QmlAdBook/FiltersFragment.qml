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

import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 1.4 as QC1

RowLayout {
    property alias filterTypesModel: filterTypesComboBox.model
    property alias filterRulesModel: filterRulesComboBox.model
    property alias filtersModel: filtersTableView.model
    property alias allRules: allRulesRadioButton.checked

    signal addingFilterRequested(int filterTypeIndex, int filterRuleIndex, string filterValue)
    signal removingFilterRequested(int selectedFilterIndex)

    Connections {
        target: addFilterButton
        onClicked: {
            var filterTypeIndex = filterTypesComboBox.currentIndex
            var filterRuleIndex = filterRulesComboBox.currentIndex
            var value = filterValueEditBox.text
            addingFilterRequested(filterTypeIndex, filterRuleIndex, value)
        }
    }

    Connections {
        target: removeSelectedFilterButton
        onClicked: {
            removingFilterRequested(filtersTableView.currentRow)
            removeSelectedFilterButton.enabled = false
        }
    }

    Layout.fillWidth: true
    ColumnLayout {
        Layout.fillWidth: true
        RowLayout {
            spacing: 10
            QC1.ComboBox {
                id: filterTypesComboBox
                Layout.minimumWidth: 100
                model: filterTypesModel
            }
            QC1.ComboBox {
                id: filterRulesComboBox
                Layout.minimumWidth: 100
                model: filterRulesModel
            }
            QC1.TextField {
                id: filterValueEditBox
                focus: true
                Layout.fillWidth: true
                Layout.minimumWidth: 100
                height: parent.childrenRect.height
                onTextChanged: {
                    addFilterButton.enabled = text.trim().length > 0
                }
            }
        }
        RowLayout {
            QC1.TableView {
                id: filtersTableView
                implicitHeight: 100
                model: filtersModel
                Layout.fillWidth: true
                selectionMode: QC1.SelectionMode.SingleSelection
                rowDelegate: TableViewRowDelegate{}

                Connections {
                    target: filtersTableView.selection
                    onSelectionChanged: {
                        if (filtersTableView.rowCount == 0){
                            return;
                        }
                        filtersTableView.selection.forEach(
                            function(rowIndex) {
                                removeSelectedFilterButton.enabled = filtersTableView.selection.count > 0
                            }
                        )
                    }
                }
                QC1.TableViewColumn {
                    id: filterTypeColumn
                    title: qsTr("Filter")
                    role: "FilterType"
                    movable: false
                    resizable: true
                    width: 100
                    delegate: TableViewItemDelegate{
                        width: parent.width
                    }
                }
                QC1.TableViewColumn {
                    id: filterRuleColumn
                    title: qsTr("Matching rule")
                    role: "FilterRule"
                    movable: false
                    resizable: true
                    width: 100
                    delegate: TableViewItemDelegate{
                        width: parent.width
                    }
                }
                QC1.TableViewColumn {
                    id: filterValueColumn
                    title: qsTr("Value")
                    role: "FilterValue"
                    movable: false
                    resizable: false
                    width: filtersTableView.viewport.width - filterRuleColumn.width - filterTypeColumn.width
                    delegate: TableViewItemDelegate{
                        width: parent.width
                    }
                }
            }
        }
        QC1.ExclusiveGroup {
            id: rulesRadioGroup
        }
        RowLayout {
            QC1.RadioButton {
                id: allRulesRadioButton
                exclusiveGroup: rulesRadioGroup
                text: qsTr("All the matching rules should be met")
            }
            QC1.RadioButton {
                id: oneRuleRadioButton
                checked: !allRules
                exclusiveGroup: rulesRadioGroup
                text: qsTr("At least one matching rule should be met")
            }
        }
    }

    ColumnLayout {
        Layout.alignment: Qt.AlignTop
        spacing: 5
        QC1.Button {
            id: addFilterButton
            Layout.minimumWidth: 100
            text: qsTr("Add")
            enabled: false
        }
        QC1.Button {
            id: removeSelectedFilterButton
            Layout.minimumWidth: 100
            text: qsTr("Remove")
            enabled: false
        }
    }
}

