import QtQuick
import QtQuick.Controls


Column
{
    id: drawerContentCol
    anchors.fill: parent
    anchors.leftMargin: 20
    anchors.topMargin: 10
    spacing: 5

    property var objectsWindow



    Button
    {
        id: drawerContentButton1
        width: 40
        height: 40
        onClicked: {
            if (!objectsWindow || objectsWindow === null) {
                var component = Qt.createComponent("../windows/ObjectsList.qml");
                if (component.status === Component.Ready) {
                    objectsWindow = component.createObject();
                    if (objectsWindow) {
                        objectsWindow.show();
                    }
                } else {
                    console.log("Ошибка загрузки:", component.errorString());
                }
            }
        }
    }

    Button
    {
        id: drawerContentButton2
        width: 40
        height: 40
    }

    Button
    {
        id: drawerContentButton3
        width: 40
        height: 40
    }
}
