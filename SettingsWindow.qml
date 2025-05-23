import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtLocation
import QtPositioning

Window
    {

    width: 400; height: 100
    title: "Настройки"

    Text {
        anchors.centerIn: parent
        text: qsTr("Настройки")
    }

    Column
    {
        spacing: 5
        anchors.fill: parent
        anchors.topMargin: 10
        anchors.leftMargin: 10

        Button
        {
            id: b1

            width: 30
            height: 30

            onClicked:
            {
                Connections
                {
                    target: map
                    map.center = QtPositioning.coordinate(55.75,37.61)
                }


            }



        }

        Button
        {
            id: b2
            width: 30
            height: 30

        }
    }
}
