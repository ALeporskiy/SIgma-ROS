import QtQuick
import QtQuick.Controls

Rectangle
{
    id: topMenu
    anchors.topMargin: 5
    anchors.top: parent.top
    anchors.horizontalCenter: parent.horizontalCenter

    border.width: 1
    border.color: "black"
    opacity: 0.8
    radius: 5


    width: 300
    height: 100
    color: "dimgrey"

    Connections {
        target: trajectoryManager

        function onCurrentPositionChanged() {
            positionDisplay.text = generateText();
        }
    }

    Column {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 4


        Text {
            id: positionDisplay
            font.pixelSize: 14
            color: "white"
            text: generateText()
        }
    }

    function safe(value) {
        return (value !== undefined && !isNaN(value)) ? value.toFixed(4) : "—";
    }

    function generateText() {
        if (!trajectoryManager)
            return "Данные не загружены";

        const pos = trajectoryManager.currentPosition;
        const traj = trajectoryManager.trajectory;

        return "🚀 МКС: " + safe(pos?.latitude) + ", " + safe(pos?.longitude) +
               "\n📍 Траектория[0]: " +
               (traj?.length > 0 ? safe(traj[0].latitude) : "—") + ", " +
               (traj?.length > 0 ? safe(traj[0].longitude) : "—");
    }

}
