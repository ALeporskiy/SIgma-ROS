// В этом файле происходит инициализация левого бокового меню и анимация его появления
// Само меню представлено в виде Rectangle
// Меню появляется на экране по нажатию кнопки "Меню"
// Кнопка "Меню" находится в отдельном Rectangle

import QtQuick
import QtQuick.Controls

Rectangle
{
        id: sMenuLeft
        width: parent.width/10
        height: parent.height
        x: -sMenuLeft.width
        color: "grey"
        property bool bMenuShown: false

        transform: Translate                                                                                // Перемещение основного прямоугольника
        {
            id: sMenuLeftTrans

            Behavior on x
            {
                NumberAnimation
                {
                    duration: 1000                                                                          // Продолжительность анимации
                    easing.type: Easing.OutQuad                                                             // Сглаживание
                }
            }
        }

        Button
        {
            id: lMenuButton1
            x: sMenuLeft.width/5

            width: 28
            height: 28



        }

        Rectangle                                                                                           // Вспомогательный прямоугольник с кнопкой
        {
            x: sMenuLeft.width
            id: leftButtonRect
            width: 48
            height: 48
            color: "transparent";


            Button
            {
                id: leftButton
                width: 48
                height: 48

                ToolTip.visible: hovered
                ToolTip.text: qsTr("Меню")

                icon.width: 38
                icon.height: 38
                icon.source: "Icons/Hamburger_MD.svg"

                onClicked: onMenuLeft();

            }

            transform: Translate                                                                            // Инициализация движения по оси Х
            {                                                                                               // Начальное значение по оси Х
                id: leftButtonTrans                                                                         // Продолжительность анимации
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
                onClicked: onMenuLeft()
            }
        }

    function onMenuLeft()
    {

        sMenuLeftTrans.x = bMenuShown ?  0 : sMenuLeft.width
        bMenuShown = !bMenuShown


    }

}
