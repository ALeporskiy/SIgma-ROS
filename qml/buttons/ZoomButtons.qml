import QtQuick
import QtQuick.Controls.Fusion


Column
{
    id: columnB
    anchors.left: parent.left
    anchors.leftMargin: 50
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 100
    spacing: 10
    z: 100

    property Item rowButtons: parent

    Button //Кнопка зума карты ПЛЮС
    {
        id: zoomButtonPlus

        width: 50
        height: 50

        ToolTip.visible: hovered
        ToolTip.text: qsTr("Приблизить")
        //padding: 0

        icon.width: 50
        icon.height: 50
        icon.source: "qrc:/qt/qml/Sigma_ROS/Icons/Add_Plus_Square.svg"
        icon.color: pressed ? "red" : "white"


        onClicked: map.zoomLevel = map.zoomLevel + 0.01;
        autoRepeat: true
        autoRepeatInterval: 1
        autoRepeatDelay: 1

        // background: Rectangle
        // {
        //     radius: 15
        //     color: zoomButtonPlus.hovered ? "lemonchiffon" : "white"
        // }

    }
    Button  //Кнопка зума карты МИНУС
    {
        id: zoomButtonMinus

        width: 50
        height: 50

        ToolTip.visible: hovered
        ToolTip.text: qsTr("Отдалить")
        //padding: 0

        icon.width: 50
        icon.height: 50
        icon.source: "qrc:/qt/qml/Sigma_ROS/Icons/Add_Minus_Square.svg"
        icon.color: pressed ? "red" : "white"


        onClicked: map.zoomLevel = map.zoomLevel - 0.01;
        autoRepeat: true
        autoRepeatInterval: 1
        autoRepeatDelay: 1



        // background: Rectangle
        // {
        //     radius: 15
        //     color: zoomButtonMinus.hovered ? "lemonchiffon" : "white"

        // }
    }

}


