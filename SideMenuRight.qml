// В этом файле происходит инициализация правого бокового меню и анимация его появления
// Само меню представлено в виде Rectangle
// Меню появляется на экране по нажатию кнопки "Меню"
// Кнопка "Меню" находится в отдельном Rectangle

import QtQuick
import QtQuick.Controls.Fusion


Rectangle                                                                                                   //Инициализация Rectangle для бокового меню
{
        id: sMenuRight
        //width: 300
        width: maxItemWidth + menuPadding
        height: parent.height

        x: parent.width
        border.width: 1
        opacity: 0.9
        color: "dimgrey"
        property bool bMenuShown: false
        property var sideMenuModel
        property int maxItemWidth: 0
        property int menuPadding: 24

        TextMetrics {
            id: textMetrics
            font.pixelSize: 14
        }

        Component.onCompleted: {

            calculateMenuWidth()
        }
        Connections {
            target: sideMenuModel
            onCountChanged: calculateMenuWidth()
        }

        function calculateMenuWidth() {
            var max = 0
            for (var i = 0; i < sideMenuModel.count; ++i) {
                var entry = sideMenuModel.get(i)
                textMetrics.text = entry.name + " (" + entry.category + ")"
                var width = textMetrics.advanceWidth + 40  // учёт чекбокса, отступов
                if (width > max)
                    max = width
            }
            maxItemWidth = max
        }

        transform: Translate                                                                                // Перемещение основного прямоугольника
        {
            id: sMenuRightTrans

            Behavior on x
            {
                NumberAnimation
                {
                    duration: 1000                                                                          // Продолжительность анимации
                    easing.type: Easing.OutQuad                                                             // Сглаживание
                }
            }
        }

        Item {
            width: parent.width

            height: parent.height

            ListView {
                id: objectListView
                anchors.fill: parent
                anchors.margins: 8
                width: objectListView.contentWidth + 20
                model: sideMenuModel
                clip: true

                delegate: ItemDelegate {
                    width: objectListView.width
                    height: 40
                    background: Rectangle {
                        color: hovered ? "#444444" : "#222222" // постоянный фон
                    }

                    Row {
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 8

                        CheckBox {
                            checked: model.checked === true
                            onCheckedChanged: model.checked = checked
                        }

                        Text {
                            text: model.name + " (" + model.category + ")"
                            font.pixelSize: 14
                            color: "white" // текст всегда белый
                            elide: Text.ElideRight
                            wrapMode: Text.NoWrap
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AsNeeded
                }
            }
        }

        Rectangle                                                                                           // Инициализация Rectangle для кнопки "Меню"
        {

            x: -rightButton.width
            id: rightButtonRect
            width: 48
            height: 48
            color: "transparent";


            Button
            {
                id: rightButton
                width: 48
                height: 48

                ToolTip.visible: hovered
                ToolTip.text: "Параметры"

                icon.width: 38
                icon.height: 38
                icon.source: "qrc:/qt/qml/Sigma_ROS/Icons/More_Grid_Big.svg"
                icon.color: hovered ? "red" : "white"

                onClicked: onMenuRight();

                // background: Rectangle
                // {
                //     radius: 15
                //     color: "white"

                // }

            }

            transform: Translate                                                                            // Инициализация движения по оси Х
            {                                                                                               // Начальное значение по оси Х
                id: rightButtonTrans                                                                        // Продолжительность анимации
                x: 0                                                                                        // Определение типа сглаживания
                Behavior on x
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
                onClicked: onMenuRight()
            }
        }

    function onMenuRight()                                                                                  // Функция для кнопки, по нажатию которой происходит появление меню
    {
        bMenuShown = !bMenuShown
        sMenuRightTrans.x = bMenuShown ? -sMenuRight.width : 0;
    }

    //Component.onCompleted: {
    //    console.log("SideMenuRight получил sharedSideMenuModel, элементов:", sideMenuModel.count);
    //}

    Connections {
        target: sideMenuModel
        onCountChanged: {
            console.log("Модель в SideMenuRight изменилась, новый размер:", sideMenuModel.count)
        }
    }





}

