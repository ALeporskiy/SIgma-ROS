import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtLocation
import QtPositioning
import SpaceSim 1.0

Map
{
    id: map
    plugin: mapPlugin
    anchors.fill: parent
    copyrightsVisible: false



    zoomLevel: 2
    minimumZoomLevel: 2
    maximumZoomLevel: 10
    center: QtPositioning.coordinate(33.2,46.2)

    property geoCoordinate startCentroid
    property alias center: map.center
    property alias zoomLevel: map.zoomLevel
    property bool followStation: false
    property bool showShadow: false
    property bool showSunMoon: false
    property bool showTwilight: false
    property bool showVisZone: false
    property real earthRadius: 6371000

    onZoomLevelChanged: {
            if (zoomLevel > maximumZoomLevel)
                zoomLevel = maximumZoomLevel;
            if (zoomLevel < minimumZoomLevel)
                zoomLevel = minimumZoomLevel;
        }

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


        PluginParameter
        {
            name: "osm.mapping.providersrepository.disabled"
            value: true
        }

        PluginParameter {
                name: "osm.mapping.providersrepository.address"
                value: ""
            }
        PluginParameter {
                name: "osm.mapping.host"
                value: "https://tile.openstreetmap.org"
            }

    }


    MouseArea                                                                                               // Всплывающее меню при нажатии ПКМ
        {
        id: mousearea

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
            width: 250

            MenuItem {

                onTriggered: {
                    map.followStation = true
                    map.center = trajectoryManager.currentPosition
                    map.zoomLevel = 5
                    notificationTextItem.showNotification("Станция в центре экрана")
                }

                contentItem: RowLayout {

                        Text {
                            text: "Станция в центре экрана"
                            color: "white"
                            elide: Text.ElideNone

                        }
                        Text {
                            text: "Ctrl+1"
                            Layout.alignment: Qt.AlignRight
                            color: "gray"


                        }
                    }

            }

            MenuItem {

                onTriggered: {
                    map.followStation = false
                    notificationTextItem.showNotification("Свободное перемещение")
                }

                contentItem: RowLayout {

                    Text {
                        text: "Свободное перемещение"
                        color: "white"
                        elide: Text.ElideNone

                    }

                    Text {
                        text: "Ctrl+2"
                        Layout.alignment: Qt.AlignRight
                        color: "gray"

                    }

                }

            }

            MenuSeparator {
                contentItem: Rectangle {
                            implicitWidth: mousearea.width
                            implicitHeight: 1
                            color: "gray"
                        }
            }

            MenuItem {
                id: toggleShadowItem

                    onTriggered: {
                        map.showShadow = !map.showShadow
                        notificationTextItem.showNotification(
                            map.showShadow ? "Светотеневая обстановка включена" : "Светотеневая обстановка выключена"
                        )
                    }

                    contentItem: RowLayout {
                        Text {
                            text: map.showShadow ? "Выключить тень" : "Включить тень "
                            color: "white"
                            elide: Text.ElideNone

                        }
                        Text {
                            text: "Ctrl+3"
                            Layout.alignment: Qt.AlignRight
                            color: "gray"
                        }
                    }
            }



            MenuItem {
                id: toggleSunMoonItem

                    onTriggered: {
                        map.showSunMoon = !map.showSunMoon
                        notificationTextItem.showNotification(
                            map.showSunMoon ? "Отображение Солнца и Луны включено" : "Отображение Солнца и Луны выключено"
                        )
                    }

                    contentItem: RowLayout {
                        Text {
                            text: map.showSunMoon ? "Выключить Солнце/Луну" : "Включить Солнце/Луну"
                            color: "white"
                            elide: Text.ElideNone

                        }
                        Text {
                            text: "Ctrl+4"
                            Layout.alignment: Qt.AlignRight
                            color: "gray"
                        }
                    }
            }

            MenuItem {
                id: toggleTwilight

                    onTriggered: {
                        map.showTwilight = !map.showTwilight
                        notificationTextItem.showNotification(
                            map.showTwilight ? "Сумерки включены" : "Сумерки выключены"
                        )
                    }

                    contentItem: RowLayout {
                        Text {
                            text: map.showTwilight ? "Выключить сумерки" : "Включить сумерки"
                            color: "white"
                            elide: Text.ElideNone

                        }
                        Text {
                            text: "Ctrl+5"
                            Layout.alignment: Qt.AlignRight
                            color: "gray"
                        }
                    }
            }

            MenuItem {
                id: toggleVisZone

                    onTriggered: {
                        map.showVisZone = !map.showVisZone
                        notificationTextItem.showNotification(
                            map.showVisZone ? "Зона видимости включена" : "Зона видимости выключена"
                        )
                    }

                    contentItem: RowLayout {
                        Text {
                            text: map.showVisZone ? "Выключить зону видимости" : "Включить зону видимости"
                            color: "white"
                            elide: Text.ElideNone

                        }
                        Text {
                            text: "Ctrl+6"
                            Layout.alignment: Qt.AlignRight
                            color: "gray"
                        }
                    }
            }
        }
    }

    ZoomButtons
    {

    }

    MapItemView {
        model: sharedSideMenuModel

        delegate: MapCircle {
                center: QtPositioning.coordinate(model.latitude, model.longitude)
                //radius: 50000  // в метрах
                visible: model.checked
                radius:{
                    // Примерная формула: чем больше zoom, тем меньше радиус в метрах
                    // Можно подстроить масштаб под свою карту
                    if (!map) return 10000
                    return 500000 / Math.pow(2, map.zoomLevel)
                }

                color: "red"
                border.color: "black"
                border.width: 1.5
            }

    }

    TrajectoryManager
        {
            id: trajectoryManager
            Component.onCompleted: {
                    trajectoryManager.loadTLE("TLE3.TLE")
                }
            onTrajectoryChanged: {
                flightPath.path = trajectoryManager.trajectory
            }
            onCurrentPositionChanged: {
                satellite.coordinate = trajectoryManager.currentPosition
            }
        }

        MapPolyline {
            id: flightPath
            line.width: 2.5
            line.color: "indigo"
            path: []
            z: 99
        }

        MapQuickItem {
            id: satellite
            coordinate: QtPositioning.coordinate(0, 0)
            sourceItem: Rectangle {
                width: 12
                height: 12
                radius: 6
                color: "red"
            }
            anchorPoint.x: 6
            anchorPoint.y: 6
            z: 99
        }


        Connections {
            target: trajectoryManager
            onCurrentPositionChanged: {
                if (map.followStation) {
                    map.center = trajectoryManager.currentPosition
                    map.zoomLevel = 5 // или другой фиксированный зум
                }
            }
        }


        Item {
            id: notificationTextItem
            width: 400
            height: 300
            anchors.horizontalCenter: map.horizontalCenter
            anchors.bottom: map.bottom
            z: 300

            Rectangle {
                id: notificationRectangle
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                width: content.implicitWidth + 20
                height: content.implicitHeight + 10
                radius: 5
                color: "#333"
                opacity: 0
                visible: false

                Text {
                    id: content
                    text: ""
                    color: "white"
                    font.pixelSize: 14
                    anchors.centerIn: parent
                }

                // Плавное затухание
                OpacityAnimator {
                    id: fadeOut
                    target: notificationRectangle
                    from: 1.0
                    to: 0.0
                    duration: 1000
                    onFinished: notificationRectangle.visible = false
                }
            }

            Timer {
                id: holdTimer
                interval: 3000
                onTriggered: fadeOut.start()
            }

            function showNotification(message) {
                content.text = message
                notificationRectangle.visible = true
                notificationRectangle.opacity = 1.0
                holdTimer.restart()
            }


        }

    Shortcut {
        sequence: "Ctrl+1"
        onActivated: {
            map.followStation = true
            map.center = trajectoryManager.currentPosition
            map.zoomLevel = 5
            notificationTextItem.showNotification("Станция в центре экрана")
        }
    }

    Shortcut {
        sequence: "Ctrl+2"
        onActivated: {
            map.followStation = false

            notificationTextItem.showNotification("Свободное перемещение")
        }
    }

    Shortcut {
        sequence: "Ctrl+3"
        onActivated: {
            map.showShadow = !map.showShadow

            notificationTextItem.showNotification(
                map.showShadow ? "Светотеневая обстановка включена" : "Светотеневая обстановка выключена"
            )
        }
    }

    Shortcut {
        sequence: "Ctrl+4"
        onActivated: {
            map.showSunMoon = !map.showSunMoon

            notificationTextItem.showNotification(
                map.showSunMoon ? "Отображение Солнца и Луны включено" : "Отображение Солнца и Луны выключено"
            )
        }
    }

    Shortcut {
        sequence: "Ctrl+5"
        onActivated: {
            map.showTwilight = !map.showTwilight

            notificationTextItem.showNotification(
                map.showTwilight ? "Сумерки включены" : "Сумерки выключены"
            )
        }
    }

    Shortcut {
        sequence: "Ctrl+6"
        onActivated: {
            map.showVisZone = !map.showVisZone

            notificationTextItem.showNotification(
                map.showVisZone ? "Зона видимости включена" : "Зона видимости выключена"
            )
        }
    }

    MapQuickItem {
        id: sunMarker
        anchorPoint.x: 8
        anchorPoint.y: 8
        visible: map.showSunMoon
        coordinate: trajectoryManager.calculateSubsolarPoint()
        sourceItem: /*Rectangle {
            width: 16
            height: 16
            radius: 8
            color: "yellow"
            border.color: "orange"
            border.width: 2
        }*/
        Image {
            id: sunImage
            source: "qrc:/qt/qml/Sigma_ROS/Icons/icons8-sun.svg"
        }
    }

    MapQuickItem {
        id: moonMarker
        anchorPoint.x: 8
        anchorPoint.y: 8
        visible: map.showSunMoon
        coordinate: trajectoryManager.calculateSublunarPoint()
        sourceItem: /*Rectangle {
            width: 16
            height: 16
            radius: 8
            color: "grey"
            border.color: "black"
            border.width: 2
        }*/
        Image {
            id: moonImage
            source: "qrc:/qt/qml/Sigma_ROS/Icons/Waxing_Gibbous.svg"
            width: 36
            height: 36

        }
    }

    Timer {
        interval: 10 * 1000  // каждые 10 секунд
        running: true
        repeat: true
        onTriggered: {
            sunMarker.coordinate = trajectoryManager.calculateSubsolarPoint()
        }
    }

    MapPolyline {
        id: terminatorLine
        line.width: 0.5
        line.color: "#888888"
        autoFadeIn: true
        opacity: 0.4
        visible: map.showShadow
        path: trajectoryManager.terminatorPath
        z: 98
    }

    Timer {
        interval: 10 * 1000
        running: true
        repeat: true
        triggeredOnStart: true
        onTriggered: trajectoryManager.updateTerminator()
    }



    MapPolygon {
        id: shadowPolygon
        color: "#2f4f4f"     // полупрозрачная чёрная тень
        border.width: 0        // без обводки
        opacity: 0.4
        visible: map.showShadow
        path: trajectoryManager.terminatorShadowPolygon()
        z: 100                  // чуть ниже линии терминатора
    }

    Timer {
        interval: 10 * 1000
        running: true
        repeat: true
        triggeredOnStart: true
        onTriggered: {
            trajectoryManager.updateTerminator()
            shadowPolygon.path = trajectoryManager.terminatorShadowPolygon()
        }
    }

    MapPolygon {
        id: civilTwilight
        path: trajectoryManager.civilTwilight
        color: "#2f4f4f"      // самая светлая
        border.width: 0
        opacity: 0.3
        visible: map.showTwilight
        z: 95
    }

    MapPolygon {
        id: nauticalTwilight
        path: trajectoryManager.nauticalTwilight
        color: "#2f4f4f"
        border.width: 0
        opacity: 0.4
        visible: map.showTwilight
        z: 94
    }

    MapPolygon {
        id: astroTwilight
        path: trajectoryManager.astroTwilight
        color: "#2f4f4f"      // самая тёмная
        border.width: 0
        opacity: 0.8
        visible: map.showTwilight
        z: 93
    }

    MapCircle {
        id: circle
        //radius: windowView.visibilityRadiusKm * 1000 // в метрах
        radius: Math.sqrt(2 * map.earthRadius * (trajectoryManager.currentAltitudeKm.toFixed(2) * 1000) + Math.pow((trajectoryManager.currentAltitudeKm * 1000), 2))
        //color: "#5533AAFF"
        color: "green"
        visible: map.showVisZone
        border.color: "transparent"
        opacity: 0.2
        center: trajectoryManager.currentPosition
    }









}
