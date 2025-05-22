import QtQuick
import QtQuick.Controls

Rectangle {

    id: panel
    border.width: 1
    opacity: 0.9
    //ВНЕШНИЕ СВОЙСТВА И ФУНКЦИИ ЭЛЕМЕНТА MYDRAWER

    //Состояние элемента MyDrawer: true - ящик выдвинут, false - ящик закрыт.
    property bool open: false

    //Определяет сторону окна на которой будет расположен элемент MyDrawer.
    //property int position: Qt.LeftEdge
    property int position: Qt.LeftEdge

    //Элемент который должен открывать или закрывать элемент MyDrawer. По умолчанию является родительским элементом.
    property Item visualParent: parent

    //Определяет процент открытия элемента MyDrawer (является дробью меньше нуля).
    readonly property real panelProgress:  (panel.x - _minimumX) / (_maximumX - _minimumX)


    //ВНЕШНИЕ ФУНКЦИИ ЭЛЕМЕНТА MYDRAWER

    //Функция для открытия MyDrawer.
    function show() { open = true; }

    //Функция для эакрытия MyDrawer.
    function hide() { open = false; }

    //Функция для переопределения состояния открыт/закрыт MyDrawer
    function toggle() { open = open ? false : true; }


    //ВНУТРЕННИЕ СВОЙСТВА И ФУНКЦИИ ЭЛЕМЕНТА MYDRAWER

    //Предоставляет доступ к списку дочерних элементов контента MyDrawer.
    default property alias data: contentItem.data

    //Определяет процент перекрытия экрана MyDrawer
    //!!! Попробовать переопределить через внешние свойства.!!!
    readonly property real expandedFraction: 0.25

    //Масштабный коэффициент.
    readonly property real _scaleFactor: _rootItem.width / 320

    //Порог вытягивания.
    readonly property int _pullThreshold: panel.width/2

    //Длительность анимации при изменении состояния MyDrawer.
    readonly property int _slideDuration: 260

    /*Определяет значение X MyDrawer, когда он в закрытом положении.
      Если MyDrawer выровнен по левому краю - X равно минус ширине MyDrawer.
      Если MyDrawer выровнен по правому краю - X равно ширина корневого элемента.*/
    readonly property int _collapsedX: _rightEdge ? _rootItem.width :  - panel.width

    //Определяет ширину MyDrawer.
    readonly property int _expandedWidth: expandedFraction * _rootItem.width

    /*Определяет значение X MyDrawer, когда он в открытом положении.
      Если MyDrawer выровнен по левому краю - X равно 0.
      Если MyDrawer выровнен по правому краю - X равно ширина корневого элемента минус ширина MyDrawer.*/
    readonly property int _expandedX: _rightEdge ? _rootItem.width - panel.width : 0

    //Определяет размещение элемента MyDrawer по стороне окна: true - правая сторона, false - левая сторона.
    readonly property bool _rightEdge: position === Qt.RightEdge

    /*Определяет минимально возможное значение X MyDrawer.
      Если MyDrawer выровнен по левому краю минимальное значение X равно минус ширине MyDrawer (MyDrawer в закрытом состоянии).
      Если MyDrawer выровнен по правому краю минимальное значение X равно ширина корневого элемента минус ширина MyDrawer
    (MyDrawer в открытом состоянии).*/
    readonly property int _minimumX:  _rightEdge ?  _rootItem.width - panel.width : -panel.width

    /*Определяет максимально возможное значение X MyDrawer.
      Если MyDrawer выровнен по левому краю максимальное значение X равно 0 (MyDrawer в открытом состоянии).
      Если MyDrawer выровнен по правому краю максимальное значение X равно ширине корневого элемента (MyDrawer в закрытом состоянии).*/
    readonly property int _maximumX: _rightEdge ? _rootItem.width : 0

    //Размер отступа в открытом состоянии MyDrawer.
    readonly property int _openMarginSize: 20 * _scaleFactor

    //Скорость перемещения мыши (разница между свойством X положения мыши до и после возникновения события перемещения мыши).
    property real _velocity: 0

    //Значение свойства X положения мыши до возникновения события перемещения мыши.
    property real _oldMouseX: -1

    /*Функция для поиска корневого элемента в программе.
      Функция перебирает в цикле родительские элементы Drawer пока не дойдет до корневого.*/
    // function _findRootItem(){
    //     var p = panel;
    //     while(p != null){
    //         p = p.parent;
    //     }
    //     return p;
    // }

    //Определяет корневой элемент программы.
    //property Item _rootItem: _findRootItem()
    property Item _rootItem: parent

    //Определяет высоту MyDrawer. Высота равна высоте родительского элемента.
    height: parent.height/1.3

    //Обработка события изменения правого края MyDrawer.
    on_RightEdgeChanged: _setupAnchors()

    //Обработка события изменения состояния MyDrawer.
    onOpenChanged: completeSlideDirection()

    //Определяет ширину MyDrawer.
    width: _expandedWidth

    //Определяет свойство X MyDrawer устанавливая его в положение соотвествующему закрытому состоянию.
    x: _collapsedX

    //Устанавливает Z-порядок. Будет 10 элементом снизу.
    z: 10

    //Функция для привязки MyDrawer к окну.
    function _setupAnchors(){
        //Переопределение корневого элемента.
        _rootItem = parent;

        //Отвязывание элемента выделения пункта меню от якорей.
        shadow.anchors.right = undefined;
        shadow.anchors.left = undefined;

        //Отвязывание области мыши элемента меню от якорей.
        mouse.anchors.left = undefined;
        mouse.anchors.right = undefined;

        /*Связывание элемента выделения пункта меню  и области мыши.
          Если MyDrawer размещен по правому краю окна - область мыши привязывается к правому краю родительского элемента,
          а элемент выделения пункта меню к левому краю MyDrawer.
          Если MyDrawer размещен по левому краю окна - область мыши привязывается к левому краю родительского элемента,
          а элемент выделения пункта меню к правому краю MyDrawer.*/
        if (_rightEdge){
            mouse.anchors.right = mouse.parent.right;
            shadow.anchors.right = panel.left;
        } else{
            mouse.anchors.left = mouse.parent.left;
            shadow.anchors.left = panel.right;
        }

        /*Изменение свойства X MyDrawer.
          Для корректного изменения свойства отключается анимация. После изменения свойства анимация включается.
          Если MyDrawer установлен по левому краю окна X равен минус ширине MyDrawer.
          Ксли MyDrawer установлен по правому краю окна X равен ширине корневого элемента
          В обоих случаях MyDrawer в состоянии закрыт.*/
        slideAnimation.enabled = false;
        panel.x =_rightEdge ? _rootItem.width :  - panel.width;
        slideAnimation.enabled = true;
    }

    /*Функция для обработки события изменения состояния MyDarwer.
      Определяет свойство X MyDrawer в соответсвии с его состоянием.
      Если MyDrawer находится в закрытом положении вызывает функцию закрытия.*/
    function completeSlideDirection(){
        if (open){
            panel.x = _expandedX;
        } else{
            panel.x = _collapsedX;
            Qt.inputMethod.hide();
        }
    }

    /*Функция для определения состояния MyDrawer.*/
    function handleRelease(){
        //Определение скорости в соотвествии с масштабом (контрольный параметр для определения перемещения мыши с целью изменения состояния MyDrawer).
        var velocityThreshold = 5 * _scaleFactor;

        /*Определение состояния MyDrawer.
          Первая проверка:
          - при установке MyDrawer по лекому краю - если движение мыши было влево и модуль изменения X мыши больше контрольного параметра,
          то состояние MyDrawer переводится в "закрыт" и вызывается функция для обработки события изменения состояния MyDrawer.
          - при установке MyDrawer по правому краю - если движение мыши было вправо и модуль изменения X мыши больше контрольного параметра,
          то состояние MyDrawer переводится в "закрыт" и вызывается функция для обработки события изменения состояния MyDrawer.
          Вторая проверка:
          - при установке MyDrawer по левому краю - если движение мыши было вправо и модуль изменения X мыши больше контрольного параметра,
          то состояние MyDrawer переводится в "открыт" и вызывается функция для обработки события изменения состояния MyDrawer.
          - при установке MyDrawer по правому краю - если движение мыши было влево и модуль изменения X мыши больше контрольного параметра,
          то состояние MyDrawer переводится в "открыт" и вызывается функция для обработки события изменения состояния MyDrawer.
          Третья проверка:
          - при установке MyDrawer по левому краю - если X MyDrawer больше разности X MyDriwer в открытом положении минус половина ширины
          корневого элемента, то состояние MyDrawer переводится в "открыт" и вызывается функция для обработки события изменения состояния MyDrawer.
           - при установке MyDrawer по правому краю - если X MyDrawer меньше суммы X MyDriwer в открытом положении плюс половина ширины
          корневого элемента, то состояние MyDrawer переводится в "открыт" и вызывается функция для обработки события изменения состояния MyDrawer.
          В противном случае:
          - состояние MyDrawer переводится в "закрыт" и вызывается функция для обработки события изменения состояния MyDrawer.*/
        if((_rightEdge && _velocity > velocityThreshold) || (!_rightEdge && _velocity < -velocityThreshold)){
            panel.open = false;
            completeSlideDirection();
        } else if((_rightEdge && _velocity < -velocityThreshold) || (!_rightEdge && _velocity > velocityThreshold)){
            panel.open = true;
            completeSlideDirection();
        } else if((_rightEdge && panel.x < _expandedX + _pullThreshold) || (!_rightEdge && panel.x > _expandedX - _pullThreshold)){
            panel.open = true;
            panel.x = _expandedX;
        } else{
            panel.open = false;
            panel.x = _collapsedX;
        }
    }

    //Функция для закрытия MyDrawer по щелчку мыши вне MyDrawer.
    function handleClick(mouse){
        if((_rightEdge && mouse.x < panel.x ) || mouse.x > panel.width){
            open = false;
        }
    }

    //Обработка события изменения совйства position MyDriwer (перехват ошибки если будет определено неизветсное положение MyDrawer).
    onPositionChanged:{
        if(! (position === Qt.RightEdge || position === Qt.LeftEdge )){
            console.warn("SlidePanel: Unsupported position.")
        }
    }

    //Элемент определяющий поведение при открытии и закрытии.
    Behavior on x{
        id: slideAnimation
        //Если активен режим передаскивания элемента мышью поведение отключается.
        enabled: !mouse.drag.active
        //Определение анимации при изменении парамктра X MyDarwer.
        NumberAnimation{
            duration: _slideDuration
            //Замедление до нуля по кубической функции.
            easing.type: Easing.OutCubic
        }
    }

    //Подключение корневого элемента для передачи сигнала об изменении его ширины объекту MyDriwer
    Connections{
        target: parent
        /*Обработка события изменения ширины корневого элемнта.
          1. Отключение анимации.
          2. Переопределение положения X MyDriwer.
          3. Включение анимации.*/
        onWidthChanged:{
            slideAnimation.enabled = false
            panel.completeSlideDirection()
            slideAnimation.enabled = true
        }
    }

    //Элемент определяющий анимацию при перетаскивании MyDrawer.
    NumberAnimation on x{
        id: holdAnimation
        //Перетаскивается на 20 масштабных коэффициентов влево или вправо в зависимости от расположения.
        to: _collapsedX + (_openMarginSize * (_rightEdge ? -1 : 1))
        //Анимация запсукается только вызовом функции.
        running : false
        easing.type: Easing.OutCubic
        duration: 200
    }

    //Элемент для перехвата сообщений от мыши
    MouseArea{
        id: mouse
        parent: _rootItem

        y: visualParent.y
        /*Ширина области мыши:
          - если MyDrawer открыт, то весь корневой элемент.
          - если MyDrawer закрыт, то величина отступа.*/
        width: open ? _rootItem.width : _openMarginSize
        height: visualParent.height/1.3

        //Обработка события зажатия кнопки мыши. Запускает анимацию для перетаскивания
        onPressed:  if (!open) holdAnimation.restart();
        //Обработка события клика мыши. Запускает функцию для закрытия MyDrawer по щелчку мыши вне MyDrawer.
        onClicked: handleClick(mouse)

        //Свойства для перетаскивания мышью.
        drag.target: panel
        //Как далеко можно перетащить MyDrawer
        drag.minimumX: _minimumX
        drag.maximumX: _maximumX
        drag.axis: Qt.Horizontal
        //Обработка события изменения активности при перетаскивании
        drag.onActiveChanged: if (active) holdAnimation.stop()
        //Обработка события отпуска кнопки мыши. Вызов функции для определения состояния MyDrawer (открытие/закрытие MyDrawer при помощи мыши).
        onReleased: handleRelease()
        //Определение Z-порядка области мыши. Если MyDrawer открыт, то область мыши над корневым элементом. Если закрыт, то под ним.
        z: open ? 1 : 0
        //Обработка события изменения координат мыши по оси X.
        onMouseXChanged:{
            _velocity = (mouse.x - _oldMouseX);
            _oldMouseX = mouse.x;
        }
    }

    //Обеспечивает затемнение области корневого элемента не закрытого MyDrawer/
    // Rectangle{
    //     id: backgroundDimmer
    //     parent: visualParent
    //     anchors.fill: parent
    //     opacity: 0.5 * Math.min(1, Math.abs(panel.x - _collapsedX) / _rootItem.width/2)
    //     color: "black"
    // }

    //Обеспечивает связь контента с MyDrawer
    Item{
        id: contentItem
        parent: _rootItem
        width: panel.width
        height: panel.height
        x: panel.x
        y: panel.y
        z: open ? 5 : 0
        clip: true
    }

    //Элемент для создания тени от MyDrawer.
    // Item{
    //     id: shadow
    //     anchors.left: panel.right
    //     anchors.leftMargin: _rightEdge ? 0 : 4 * _scaleFactor
    //     height: parent.height

    //     Rectangle{
    //         height: 4 * _scaleFactor
    //         width: panel.height
    //         rotation: 90
    //         opacity: Math.min(1, Math.abs(panel.x - _collapsedX)/_openMarginSize)
    //         //Точка вокруг которой осуществляется поворот на 90 градусов.
    //         transformOrigin: Item.TopLeft
    //         gradient: Gradient{
    //             GradientStop { position: _rightEdge ? 1 : 0 ; color: "#00000000"}
    //             GradientStop { position: _rightEdge ? 0 : 1 ; color: "#2c000000"}
    //         }
    //     }
    //     Rectangle{

    //         height: 2 * _scaleFactor
    //         width: panel.width
    //         x: -panel.width - 4 * _scaleFactor
    //         y: panel.height
    //         //rotation: 90
    //         opacity: Math.min(1, Math.abs(panel.x - _collapsedX)/_openMarginSize)
    //         //Точка вокруг которой осуществляется поворот на 90 градусов.
    //         //transformOrigin: Item.TopLeft
    //         gradient: Gradient{
    //             GradientStop { position: _rightEdge ? 1 : 0 ; color: "#00000000"}
    //             GradientStop { position: _rightEdge ? 0 : 1 ; color: "#2c000000"}
    //         }
    //     }
    // }
}
