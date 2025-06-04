import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs 6.2

Window {
    id: window
    width: 800
    height: 800
    visible: true

    title: "Список объектов"

    ListModel {
            id: userSelectionModel
        }

    Component.onDestruction: console.log("Окно уничтожено")

    property var userSelection: []

    onClosing: {
        Qt.callLater(function() {
            window.destroy(); // безопасно удалить вручную
        });
    }


    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        anchors.margins: 10

        Text {
            text: "Выберите объекты для отображения на карте"
            font.pixelSize: 18
            Layout.alignment: Qt.AlignHCenter
        }



        ListView {
            id: objectList
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: objectListProvider.objects
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            delegate: ItemDelegate {
                width: parent ? parent.width : 400
                text: modelData.name + " (" + modelData.category + ")"
                onClicked: {
                    console.log("Нажатие:", modelData.name);
                        var exists = false;
                            for (var i = 0; i < userSelectionModel.count; ++i) {
                                if (userSelectionModel.get(i).name === modelData.name) {
                                    exists = true;
                                    break;
                                }
                            }
                            if (!exists) {
                                userSelectionModel.append(modelData)
                            }

                }
            }

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AlwaysOn
                clip: true
                contentItem: Rectangle {
                    implicitWidth: 6
                    radius: 3
                    color: "#cccccc" // Цвет ползунка

                }
                background: Rectangle {
                    color: "#444444" // Цвет фона полосы
                    radius: 3
                    anchors.fill: parent
                }
            }
        }


        Rectangle {
            height: 4
            color: "#ccc"
            Layout.fillWidth: true
        }

        Text {
            text: "Ваш список объектов"
            font.pixelSize: 16
            Layout.alignment: Qt.AlignHCenter
        }

        ListView {
            id: selectedList
            Layout.fillWidth: true
            Layout.preferredHeight: 300
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            model: userSelectionModel
            delegate: ItemDelegate {
                width: parent ? parent.width : 200
                text: model.name + " (" + model.category + ")"
                onClicked: {
                    userSelectionModel.remove(index);
                }
            }
            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AlwaysOn
                clip: true
                contentItem: Rectangle {
                    implicitWidth: 6
                    radius: 3
                    color: "#cccccc" // Цвет ползунка

                }
                background: Rectangle {
                    color: "#444444" // Цвет фона полосы
                    radius: 3
                    anchors.fill: parent
                }
            }
        }
        RowLayout
        {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom
            spacing: 10

            Button {
                text: "Загрузить мой список"
                Layout.alignment: Qt.AlignLeft
                // anchors.bottom: parent.bottom
                // anchors.left: parent.left
                // anchors.margins: 10
                onClicked: loadDialog.open()
            }

            Item { Layout.fillWidth: true }

            Button {
                text: "Сохранить мой список"
                Layout.alignment: Qt.AlignRight
            // anchors.bottom: parent.bottom
            // anchors.right: parent.right
            // anchors.margins: 10
                onClicked: {
                    var fileDialog = saveDialog;
                    fileDialog.open();
                }
            }
        }

    }

    FileDialog {
        id: saveDialog
        title: "Сохранить ваш список объектов"
        fileMode: FileDialog.SaveFile
        nameFilters: ["JSON files (*.json)"]
        onAccepted: {
            var json = { objects: [] };
                for (var i = 0; i < userSelectionModel.count; ++i) {
                    json.objects.push(userSelectionModel.get(i));
                }

                var selected = saveDialog.selectedFile;
                if (selected) {
                    saveProvider.saveJsonToFile(selected, JSON.stringify(json, null, 2));
                } else {
                    console.log("Путь не выбран");
                }
            }
    }




    FileDialog {
            id: loadDialog
            title: "Загрузить ваш список объектов"
            fileMode: FileDialog.OpenFile
            nameFilters: ["JSON files (*.json)"]
            onAccepted: {
                var filePath = loadDialog.selectedFile;
                if (filePath) {
                    var jsonString = saveProvider.loadJsonFromFile(filePath);
                    if (jsonString) {
                        try {
                            var parsed = JSON.parse(jsonString);
                            if (parsed && parsed.objects && parsed.objects.length > 0) {
                                userSelectionModel.clear();
                                for (var i = 0; i < parsed.objects.length; ++i) {
                                    userSelectionModel.append(parsed.objects[i]);
                                }
                            } else {
                                errorDialog.message = "Файл не содержит допустимых объектов.";
                                errorDialog.open();
                            }
                        } catch (e) {
                            errorDialog.message = "Ошибка при разборе JSON-файла.";
                            errorDialog.open();
                        }
                    } else {
                        errorDialog.messsage = "Не удалось загрузить файл.";
                        errorDialog.open();
                    }
                }
            }
        }

    Dialog {
        id: errorDialog
        width: 320
        title: "Ошибка"
        modal: true
        anchors.centerIn: parent
        property string message: ""

        contentItem: ColumnLayout {
            spacing: 10
            Label {
                text: errorDialog.message
                wrapMode: Text.WordWrap
                Layout.preferredWidth: 300
            }
            Button {
                text: "OK"
                Layout.alignment: Qt.AlignRight
                onClicked: errorDialog.close()
            }
        }
    }
}
