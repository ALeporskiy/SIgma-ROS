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
                                                                                                     // из области за экраном по оси X (-150 задана начальная точка)
            Behavior on x
            {
                NumberAnimation
                {
                    duration: 1000                                                                          // Продолжительность анимации
                    easing.type: Easing.OutQuad                                                             // Сглаживание
                }
            }
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
                text: qsTr("МЕНЮ")
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
