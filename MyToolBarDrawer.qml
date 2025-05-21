import QtQuick

Rectangle {

    id: toolBar

    //ВНЕШНИЕ СВОЙСТВА

    //Свойство открыта либо закрыта панель
    property bool open: false

    //Высота панели
    property int heightPanel: 48

    //Ссылка на родительский элемент
    property Item visualParent: parent


    //ВНЕШНИЕ ФУНКЦИИ
    function show() { open = true; }
    function hide() { open = false; }


    //ГЕОМЕТРИЯ

    //Значение Y в закрытом состоянии
    readonly property int _collapsedY: -heightPanel
    //Значение Y в открытом состоянии
    readonly property int _expandedY: 0
    //Ширина и высота панели
    width: visualParent.width
    height: heightPanel
    //Привязка по X
    x: 0
    y: _collapsedY
    //Цвет фона
    color: "#4cd964"

    //АНИМАЦИЯ
    //Время анимации
    readonly property int _slideDuration: 260


    Behavior on y{

        id: slideAnimation

        NumberAnimation{

            duration: _slideDuration
            easing.type: Easing.OutCubic
        }
    }

    //Зона мыши для перехвата указателя с целью открытия панели
    MouseArea{

        id: mouse
        parent: visualParent

        //Зона мыши по горизонтали начинается от X=0 родительского элемента и продолжается на его ширину
        x: visualParent.x
        width: visualParent.width
        //Зона мыши по вертикали начинается от Y=0 родительского элемента и продолжается на высоту панели
        y: visualParent.y
        height: toolBar.height
        //Для перехвата событий мыши свойство должно быть true
        hoverEnabled: true

        onExited: {
            toolBar.open = false;
            toolBar.y = _collapsedY;
        }

        onEntered: {
            toolBar.open = true;
            toolBar.y = _expandedY;
        }
    }
}
