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
    property bool isListModified: false
    property bool allowClose: false
    property var sideMenuModel
    property string dialogContext: ""
    property int contextIndex: -1
    property int pendingDeleteIndex: -1     // свойство для реализации подтверждения удаления объекта из списка



    Component.onCompleted: {
        window.closing.connect(handleWindowClosing)
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        anchors.margins: 10

        RowLayout {
            width: parent.width
            //spacing: 5



            Button {
                text: "Добавить объект"


                Layout.alignment: Qt.AlignVCenter
                onClicked: addDialog.open()
            }

            Item { Layout.fillWidth: true }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter


                text: "Выберите объекты для отображения на карте"
                font.pixelSize: 16
                //Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            }

            Item { Layout.fillWidth: true }

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
                                isListModified = true
                            }

                }

                property int modelIndex: index

                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.RightButton
                        onPressed: (mouse) => {
                            if (mouse.button === Qt.RightButton) {
                                contextIndex = modelIndex
                                contextMenu.popup()
                            }
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
                    isListModified = true
                }
            }
            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
                visible: userSelectionModel.count > 10
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
                onClicked: loadDialog.open()
            }

            Item { Layout.fillWidth: true }

            Button {
                text: "Взять объекты в работу"
                Layout.alignment: Qt.AlignHCenter
                onClicked: {
                    // sideMenuModel.clear();
                    // for (let i = 0; i < userSelectionModel.count; ++i) {
                    //     let item = userSelectionModel.get(i);
                    //     sideMenuModel.append({
                    //         name: item.name,
                    //         category: item.category,
                    //         latitude: item.latitude,
                    //         longitude: item.longitude,
                    //         checked: false
                    //     });
                    // }
                    // console.log("Передаю", userSelectionModel.count, "в sharedSideMenuModel");
                    // window.destroy(); // закрыть окно после отправки

                        if (isListModified) {
                            dialogContext = "applyList"
                            confirmDialog.open()
                        } else {
                            applyObjectsToSideMenu()
                            window.destroy()
                        }

                }
            }

            Item { Layout.fillWidth: true }

            Button {
                text: "Сохранить мой список"
                Layout.alignment: Qt.AlignRight

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

    Dialog {            // диалог с ошибками json файла
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



    Dialog {                            // диалог после нажатия "взять в работу"
        id: confirmDialog
        title: "Несохранённые изменения"
        modal: true
        focus: true
        standardButtons: Dialog.NoButton
        anchors.centerIn: parent


        Column {
            spacing: 10
            padding: 10

            Text {
                text: "Вы хотите сохранить список объектов перед продолжением?"
                color: "white"
                wrapMode: Text.WordWrap
            }

            RowLayout {
                spacing: 10
                Layout.fillWidth: true

                Button {
                    text: "Сохранить"
                    onClicked: {
                        saveDialog.open()
                        confirmDialog.close()
                    }
                }

                Button {
                    text: "Продолжить без сохранения"
                    onClicked: {
                        if (dialogContext === "exitWindow") {
                            allowClose = true
                            window.destroy()
                        } else if (dialogContext === "applyList") {
                            applyObjectsToSideMenu()
                            window.destroy()
                        }
                        confirmDialog.close()
                    }
                }

                Button {
                    text: "Отмена"
                    onClicked: {
                        confirmDialog.close()
                    }
                }
            }
        }
    }

    Dialog {            // диалог после нажатия "добавить объект"
        id: addDialog
        modal: true
        title: "Новый объект"
        standardButtons: Dialog.Ok | Dialog.Cancel
        width: 300
        height: 200
        anchors.centerIn: parent
        visible: false

        property alias nameField: inputName
        property alias latField: inputLat
        property alias lonField: inputLon

        contentItem: Column {
            spacing: 10
            padding: 10

            Row {
                spacing: 10
            TextField {
                id: inputName
                placeholderText: "Название"
            }
                Text {

                    text: "Название"
                    font.pixelSize: 14
                    color: "white"
                }
            }

            TextField {
                id: inputLat
                placeholderText: "Широта"
                inputMethodHints: Qt.ImhFormattedNumbersOnly
            }

            TextField {
                id: inputLon
                placeholderText: "Долгота"
                inputMethodHints: Qt.ImhFormattedNumbersOnly
            }
        }

        onAccepted: {
            const lat = parseFloat(inputLat.text)
            const lon = parseFloat(inputLon.text)

            if (inputName.text === "" || isNaN(lat) || isNaN(lon)) {
                console.log("Некорректные данные")
                return
            }

            objectListProvider.addObject({
                name: inputName.text,
                latitude: lat,
                longitude: lon,
                category: "custom"
            })

            objectListProvider.saveToJson("predefined_objects.json")

            inputName.text = ""
            inputLat.text = ""
            inputLon.text = ""
        }

        onRejected: {
            inputName.text = ""
            inputLat.text = ""
            inputLon.text = ""
        }
    }

    Dialog {
        id: confirmDeleteDialog
        title: "Подтверждение"
        modal: true
        anchors.centerIn: parent
        standardButtons: Dialog.Yes | Dialog.No
        width: 300

        contentItem: Item {
            width: parent.width
            height: textBlock.paintedHeight + 20

            Text {
                id: textBlock
                text: "Вы уверены, что хотите удалить этот объект?"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                wrapMode: Text.WordWrap
                color: "white"
                width: 260
                font.pixelSize: 14
            }
        }

        onAccepted: {
            if (pendingDeleteIndex >= 0) {
                objectListProvider.removeAt(pendingDeleteIndex)
                pendingDeleteIndex = -1
            }
        }

        onRejected: pendingDeleteIndex = -1
    }

    function applyObjectsToSideMenu() {
        if (typeof sharedSideMenuModel === "undefined")
            console.warn("sharedSideMenuModel не определён")
        else {
            sharedSideMenuModel.clear()
            for (let i = 0; i < userSelectionModel.count; ++i) {
                sharedSideMenuModel.append({
                    name: userSelectionModel.get(i).name,
                    category: userSelectionModel.get(i).category,
                    latitude: userSelectionModel.get(i).latitude,
                    longitude: userSelectionModel.get(i).longitude,
                    checked: false
                })
            }
        }
    }

    function handleWindowClosing(close) {
        if (!allowClose && isListModified) {
            close.accepted = false
            dialogContext = "exitWindow"
            confirmDialog.open()
        }
    }

    Menu {
        id: contextMenu

        MenuItem {
            text: "Удалить"
            onTriggered: {
                pendingDeleteIndex = contextIndex
                confirmDeleteDialog.open()
            }
        }
    }
}
