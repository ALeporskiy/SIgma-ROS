import QtQuick
import QtQuick.Controls


Rectangle
{
        id: sMenuRight
        width: parent.width/10
        height: parent.height
        x: parent.width
        color: "red"
        property bool bMenuShown: false

        transform: Translate                                                                                // Перемещение основного прямоугольника
        {
            id: sMenuRightTrans
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
                text: qsTr("МЕНЮ")
                onClicked: onMenuRight();

            }

            transform: Translate                                                                            // Инициализация движения по оси Х
            {                                                                                               // Начальное значение по оси Х
                id: rightButtonTrans                                                                           // Продолжительность анимации
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
                onClicked: onMenuRight()
            }
        }

    function onMenuRight()
    {
        bMenuShown = !bMenuShown
        sMenuRightTrans.x = bMenuShown ? -sMenuRight.width : 0;


    }

}

