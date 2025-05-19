import QtQuick
import QtQuick.Window
import QtLocation
import QtPositioning
import QtQuick.Controls


Window{
    id: root
    visible: true
    visibility: Window.Maximized
    width: Screen.width/1.5
    height: Screen.height/1.5
    title: qsTr("Sigma V2")                                                                                 // Новое раздел с property
    property bool bMenuShown: false



        Plugin
        {
        id: mapPlugin
        name: "osm"

            PluginParameter
            {
            name: "osm.mapping.custom.host"
            value: "https://basemap.nationalmap.gov/arcgis/rest/services/USGSImageryOnly/MapServer/tile/8/136/74"
            }
        }
    Map
    {
        id: map
        plugin: mapPlugin
        anchors.fill: parent
        activeMapType: supportedMapTypes[supportedMapTypes.length]
        zoomLevel: 1
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
            rotationScale: 1/60
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

//-----------------------------------------------------Создание и анимация левого бокового меню------------------------------------------//

    Rectangle                                                                                               // Основной прямоугольник
    {
        id: mainRec
        width: 150
        height: root.height
        color: "grey";
        opacity: bMenuShown ? 1 : 0.85                                                                       // Изменение значения opacity
        Behavior on opacity                                                                                 // для закрытия меню
        {                                                                                                   //
            NumberAnimation                                                                                 // продолжительность анимации
            {
                duration: 100
            }
        }

        transform: Translate                                                                                // Перемещение основного прямоугольника
        {
            id: menuTranslate1
            x: -150                                                                                         // из области за экраном по оси X (-150 задана начальная точка)
            Behavior on x
            {
                NumberAnimation
                {
                    duration: 1000                                                                          // Продолжительность анимации
                    easing.type: Easing.OutQuad                                                             // Сглаживание
                }
            }
        }

    }

        Rectangle                                                                                           // Вспомогательный прямоугольник с кнопкой
        {

            width: 48
            height: 48
            color: "transparent";


            Button
            {

                width: 48
                height: 48
                text: qsTr("МЕНЮ")
                onClicked: onMenu();

            }

            transform: Translate                                                                            // Инициализация движения по оси Х
            {                                                                                               // Начальное значение по оси Х
                id: menuTranslate                                                                           // Продолжительность анимации
                x: 0                                                                                        // Определение типа сглаживания
                Behavior on x                                                                               //
                {
                    NumberAnimation
                    {
                        duration: 1000;
                        easing.type: Easing.OutQuad
                    }
                }
            }

            MouseArea
            {
                anchors.fill: parent
                enabled: bMenuShown
                onClicked: onMenu()
            }
        }

    function onMenu()
    {
        menuTranslate.x = bMenuShown ? 1 : mainRec.width //root.width * 0.0769
        menuTranslate1.x = bMenuShown ? -150 : 0
        bMenuShown = !bMenuShown;
    }

//-----------------------------------------------------Конец Создание и анимация левого бокового меню------------------------------------------//



}
