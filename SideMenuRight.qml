// В этом файле происходит инициализация правого бокового меню и анимация его появления
// Само меню представлено в виде Rectangle
// Меню появляется на экране по нажатию кнопки "Меню"
// Кнопка "Меню" находится в отдельном Rectangle

import QtQuick
import QtQuick.Controls


Rectangle                                                                                                   //Инициализация Rectangle для бокового меню
{
        id: sMenuRight
        width: parent.width/10
        height: parent.height
        x: parent.width
        border.width: 1
        opacity: 0.9
        color: "white"
        property bool bMenuShown: false

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
                icon.source: "Icons/More_Grid_Big.svg"

                onClicked: onMenuRight();

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

}

