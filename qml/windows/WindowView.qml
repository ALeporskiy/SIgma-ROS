import QtQuick 2.15
import QtQuick.Window 2.15
import QtLocation 5.15
import QtPositioning 5.15
import SpaceSim


Window {
    id: windowView
    width: 800
    height: 600
    visible: true
    title: "Вид из иллюминатора в надир"

    property var trajectoryManager
    property real visibilityRadiusKm: 2301
    property real earthRadius: 6371000


    minimumWidth: width
    maximumWidth: width
    minimumHeight: height
    maximumHeight: height

    flags: Qt.Dialog |  Qt.WindowTitleHint | Qt.WindowCloseButtonHint


    TrajectoryManager {
        id: trajectoryManager
        Component.onCompleted: {
                trajectoryManager.loadTLE("TLE3.TLE")
            }
        onCurrentPositionChanged: {
            map.center = trajectoryManager.currentPosition
            circle.center = trajectoryManager.currentPosition
        }

    }

    Map {
        id: map
        anchors.fill: parent
        plugin: Plugin {
            name: "osm"
            PluginParameter
            {
                name: "osm.mapping.providersrepository.disabled"
                value: "true"
            }

        }
        zoomLevel: 4.2
        center: QtPositioning.coordinate(0, 0)
        copyrightsVisible: false

        MapCircle {
            id: circle
            //radius: windowView.visibilityRadiusKm * 1000 // в метрах
            radius: Math.sqrt(2 * windowView.earthRadius * (trajectoryManager.currentAltitudeKm.toFixed(2) * 1000) + Math.pow((trajectoryManager.currentAltitudeKm * 1000), 2))
            color: "#5533AAFF"
            border.color: "blue"
            center: QtPositioning.coordinate(0, 0)
        }
    }

    // Text {
    //     text: "Зона видимости   " + circle.radius.toFixed(2)/1000
    // }

    onClosing: {
        Qt.callLater(() => destroy())
    }

    Component.onDestruction: {
        console.log("WindowView destroyed")
    }

    onVisibleChanged: {
        console.log("WindowView visible =", visible)
    }




}
