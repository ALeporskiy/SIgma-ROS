import QtQuick
import QtQuick.Window



Window
{
    id: root
    visible: true
    visibility: Window.Maximized
    width: Screen.width/1.5
    height: Screen.height/1.5
    title: qsTr("Sigma V2")
    property bool bMenuShown: false



// Обращение к файлу MyMap.qml, где происходит инициализация карты

    MyMap
    {

    }

// Обращение к файлу SideMenuRight.qml, где происходит инициализация правого бокового меню


    SideMenuRight
    {

    }

// Обращение к файлу MyDrawer.qml, где происходит инициализация левого бокового меню

    MyDrawer
    {

    }

    // MyToolBarDrawer
    // {

    // }

}
