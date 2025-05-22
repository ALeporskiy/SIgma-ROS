import QtQuick
import QtQuick.Window
import QtLocation
import QtPositioning
import QtQuick.Controls


Window
{
    id: root
    visible: true
    visibility: Window.Maximized
    width: Screen.width/1.5
    height: Screen.height/1.5
    title: qsTr("Sigma V2")
    property bool bMenuShown: false



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
    Map
    {
        id: map
        plugin: mapPlugin
        anchors.fill: parent
        activeMapType: supportedMapTypes[supportedMapTypes.length]
        zoomLevel: 2
        minimumZoomLevel: 5
        maximumZoomLevel: 17
        center: QtPositioning.coordinate(54.2,36.2)
        property geoCoordinate startCentroid

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

// Обращение к файлу SideMenuLeft.qml, где происходит инициализация левого бокового меню

    // SideMenuLeft
    // {

    // }



// Обращение к файлу SideMenuRight.qml, где происходит инициализация правого бокового меню

    SideMenuRight
    {

    }

// Обращение к файлу ZoomButtons.qml, где происходит инициализация кнопок для зума карты

    ZoomButtons
    {

    }

    MyDrawer
    {

    }

    // MyToolBarDrawer
    // {

    // }

}
