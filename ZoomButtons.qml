import QtQuick
import QtQuick.Controls


Row
{
    id: rowB
    anchors.left: parent.left
    anchors.leftMargin: 200
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 100
    spacing: 10
    property Item rowButtons: parent

    Button //Кнопка зума карты ПЛЮС
    {
        id: zoomButtonPlus
        // x: rowB.rowButtons.width/2 - 60
        // y: rowB.rowButtons.height - 100
        width: 50
        height: 50

        ToolTip.visible: hovered
        ToolTip.text: qsTr("Приблизить")

        icon.width: 50
        icon.height: 50
        icon.source: "Icons/Add_Plus_Square.svg"
        //opacity: 0.5

        onClicked: map.zoomLevel = map.zoomLevel + 0.01;
        autoRepeat: true
        autoRepeatInterval: 1
        autoRepeatDelay: 1

    }
    Button  //Кнопка зума карты МИНУС
    {
        id: zoomButtonMinus
        // x: rowB.rowButtons.width/2 + 60
        // y: rowB.rowButtons.height - 100
        width: 50
        height: 50

        ToolTip.visible: hovered
        ToolTip.text: qsTr("Отдалить")

        icon.width: 50
        icon.height: 50
        icon.source: "Icons/Add_Minus_Square.svg"

        onClicked: map.zoomLevel = map.zoomLevel - 0.01;
        autoRepeat: true
        autoRepeatInterval: 1
        autoRepeatDelay: 1
    }

}


