import QtQuick
import QtQuick.Controls.Fusion





Column
{
    id: drawerContentCol
    anchors.fill: parent
    anchors.leftMargin: 20
    anchors.topMargin: 10
    spacing: 10

    property var objectsWindow
    property var sideMenuModel
    property var windowView
    property var settingsWindow





    Button
    {

        id: drawerContentButton1

        anchors.left: parent.left
        anchors.right: parent.right

        anchors.rightMargin: 20
        height: 40
        icon.width: 20
        icon.height: 20
        icon.source: "qrc:/qt/qml/Sigma_ROS/Icons/File_Document.svg"
        font.pixelSize: 16

        icon.color: hovered ? "red" : "white"
        text: "Список объектов"

        display: Button.TextBesideIcon

        onClicked: {
            if (!objectsWindow || objectsWindow === null) {
                var component = Qt.createComponent("../windows/ObjectsList.qml").createObject(root);        // создаем компонент как дочерний от root в main.qml
                if (component.status === Component.Ready) {
                    objectsWindow = component.createObject(null, {
                            sideMenuModel: sharedSideMenuModel
                    });

                    if (objectsWindow) {
                        objectsWindow.show();

                        // 💡 Обработчик на скрытие окна
                        objectsWindow.visibleChanged.connect(function() {
                            if (!objectsWindow.visible) {
                                console.log("Окно закрыто — сбрасываю ссылку");
                                objectsWindow = null;
                            }
                        });

                        // 💡 (Дополнительно) если окно будет уничтожено явно
                        // objectsWindow.destroyed.connect(function() {
                        //     console.log("Окно уничтожено — сбрасываю ссылку");
                        //     objectsWindow = null;
                        // });
                    }
                }
            }

        }



    }




    Button
    {
        id: drawerContentButton2
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 20
        height: 40

        icon.width: 20
        icon.height: 20
        icon.source: "qrc:/qt/qml/Sigma_ROS/Icons/Settings.svg"
        font.pixelSize: 16

        icon.color: hovered ? "red" : "white"
        text: "Настройки"

        onClicked: {
            if (!settingsWindow || settingsWindow === null) {
                var component = Qt.createComponent("../windows/SettingsWindow.qml").createObject(root);
                if (component.status === Component.Ready) {
                    settingsWindow = component.createObject(null,)
                    if (settingsWindow) {
                        settingsWindow.show()
                        settingsWindow.visibleChanged.connect(function() {
                            if (!settingsWindow.visible) {
                                settingsWindow = null
                            }
                        })
                    }
                }
            }
        }





    }

    Button
    {
        id: drawerContentButton3
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 20
        height: 40
    }

    Button
    {
        id: drawerContentButton4
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 20
        height: 40
    }

    Button
    {
        id: drawerContentButton5
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 20
        height: 40
    }

    Button {
        text: "Вид из иллюминатора"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 20

        height: 40
        font.pixelSize: 14
        onClicked: {
            if (!windowView || windowView === null) {
                var component = Qt.createComponent("../windows/WindowView.qml").createObject(root);
                if (component.status === Component.Ready) {
                    windowView = component.createObject(null, { "trajectoryManager": trajectoryManager })
                    if (windowView) {
                        windowView.show()
                        windowView.visibleChanged.connect(function() {
                            if (!windowView.visible) {
                                windowView = null
                            }
                        })
                    }
                }
            }
        }
    }

}
