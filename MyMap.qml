import QtQuick
import QtQuick.Controls
import QtLocation
import QtPositioning
import SpaceSim 1.0

Map
{
    id: map
    plugin: mapPlugin
    anchors.fill: parent
    copyrightsVisible: false
    //activeMapType: supportedMapTypes[supportedMapTypes.length]
    //activeMapType: Map.street

    zoomLevel: 2
    minimumZoomLevel: 5
    maximumZoomLevel: 17
    center: QtPositioning.coordinate(33.2,46.2)

    property geoCoordinate startCentroid
    property alias center: map.center
    property alias zoomLevel: map.zoomLevel

    PinchHandler                                                                                       // Пермещение карты с помощью тачпада или сенсорного экрана
        {
        id: pinch
        target: null
        onActiveChanged: if (active)
        {
            map.startCentroid = map.toCoordinate(pinch.centroid.position, false)
        }
        onScaleChanged: (delta) =>
        {
            map.zoomLevel += Math.log2(delta)
            map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
        }
        onRotationChanged: (delta) =>
        {
            map.bearing -= delta
            map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
        }
            grabPermissions: PointerHandler.TakeOverForbidden
        }
    DragHandler                                                                                         // Перемещение карты с зажатой ЛКМ
        {
            id: drag
            target: null
            onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
        }

    WheelHandler                                                                                        // Приближение карты с помощью мыши
        {
        id: wheel                                                                                       // Баг с мышью от Apple
                                                                                                        // workaround for QTBUG-87646 / QTBUG-112394 / QTBUG-112432:
                                                                                                        // Magic Mouse pretends to be a trackpad but doesn't work with PinchHandler
                                                                                                        // and we don't yet distinguish mice and trackpads on Wayland either
        acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland"
                        ? PointerDevice.Mouse | PointerDevice.TouchPad
                        : PointerDevice.Mouse
        rotationScale: 1/150
        property: "zoomLevel"
        }


    Plugin
    {
    id: mapPlugin
    name: "osm"

    // PluginParameter
    // {
    // name: "osm.mapping.custom.host"
    // value: "https://basemap.nationalmap.gov/arcgis/rest/services/USGSImageryOnly/MapServer/tile/8/136/74"
    // }
        PluginParameter
        {
            name: "osm.mapping.providersrepository.disabled"
            value: "true"
        }
            PluginParameter
            {
                name: "osm.mapping.providersrepository.address"
                value: "http://maps-redirect.qt.io/osm/5.6/"
            }
    }


    MouseArea                                                                                               // Всплывающее меню при нажатии ПКМ
        {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: (mouse) => {                                                                             // Проверка нажатия клавиши мыши
            if (mouse.button === Qt.RightButton)                                                            // если это ПКМ, то отобразить всплывающее меню
                contextMenu.popup()
        }
        onPressAndHold: (mouse) => {
            if (mouse.source === Qt.MouseEventNotSynthesized)
                contextMenu.popup()
        }

        Menu {
            id: contextMenu
            MenuItem { text: "Cut" }
            MenuItem { text: "Copy" }
            MenuItem { text: "Paste" }
        }
    }

    ZoomButtons
    {

    }

    TrajectoryManager
        {
            id: trajectoryManager
            onTrajectoryChanged: {
                flightPath.path = trajectoryManager.trajectory
            }
            onCurrentPositionChanged: {
                satellite.coordinate = trajectoryManager.currentPosition
            }
        }

        MapPolyline {
            id: flightPath
            line.width: 2
            line.color: "red"
            path: []
        }

        MapQuickItem {
            id: satellite
            coordinate: QtPositioning.coordinate(0, 0)
            sourceItem: Rectangle {
                width: 12
                height: 12
                radius: 6
                color: "blue"
            }
            anchorPoint.x: 6
            anchorPoint.y: 6
        }

        Button {
            text: "Загрузить орбиту"
            anchors.bottom: parent.bottom
            onClicked: trajectoryManager.loadTLE("TLE.TLE")
        }

        Column {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 4


            Text {
                text: "🚀 Спутник: " + trajectoryManager.currentPosition.latitude.toFixed(4) +
                      ", " + trajectoryManager.currentPosition.longitude.toFixed(4) +
                      "\n📍 Траектория[0]: " +
                      (trajectoryManager.trajectory.length > 0
                          ? trajectoryManager.trajectory[0].latitude.toFixed(4)
                          : "—") +
                      ", " +
                      (trajectoryManager.trajectory.length > 0
                          ? trajectoryManager.trajectory[0].longitude.toFixed(4)
                          : "—")
            }
        }


}
