import QtQuick
import QtQuick.Controls


Column
{
    id: drawerContentCol
    anchors.fill: parent
    anchors.leftMargin: 20
    anchors.topMargin: 10
    spacing: 5


    Button
    {
        id: drawerContentButton1
        width: 40
        height: 40
        onClicked: {
            var component = Qt.createComponent("SettingsWindow.qml")
            var window    = component.createObject(root)
            window.show()
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
