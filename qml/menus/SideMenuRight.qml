// В этом файле происходит инициализация правого бокового меню и анимация его появления
// Само меню представлено в виде Rectangle
// Меню появляется на экране по нажатию кнопки "Меню"
// Кнопка "Меню" находится в отдельном Rectangle

import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts


Rectangle                                                                                                   //Инициализация Rectangle для бокового меню
{
        id: sMenuRight

        width: maxItemWidth + menuPadding
        height: parent.height/2
        radius: 5
        x: parent.width
        border.width: 1
        opacity: 0.9
        color: "dimgrey"
        property bool bMenuShown: false
        property var sideMenuModel
        property int maxItemWidth: 0
        property int menuPadding: 24
        property bool allChecked: false



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
            var max = 150
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

        ColumnLayout {


            anchors.fill: parent
            anchors.margins: 2
            spacing: 4




            ListView {
                id: objectListView

                anchors.margins: 2
                boundsBehavior: Flickable.StopAtBounds
                Layout.fillWidth: true
                Layout.fillHeight: true
                width: objectListView.contentWidth + 30
                model: sideMenuModel

                clip: true

                delegate: ItemDelegate {
                    width: objectListView.width
                    height: 40
                    onClicked: model.checked = !model.checked
                    background: Rectangle {
                        color: hovered ? "#444444" : "#222222" // постоянный фон
                    }

                    Row {
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 8

                        CheckBox {
                            id: checkBox
                            checked: model.checked === true
                            onClicked: {
                                    model.checked = checkBox.checked

                                }

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

                Text {
                        visible: sideMenuModel.count === 0
                        anchors.centerIn: parent
                        text: "Список объектов пуст"
                        opacity: 0.8
                        color: "#aaaaaa"
                        font.pixelSize: 14
                        z: 1
                    }

                ScrollBar.vertical: ScrollBar {
                    policy: ScrollBar.AsNeeded
                    visible: sideMenuModel.count > 13

                    contentItem: Rectangle {
                        implicitWidth: 3
                        radius: 3
                        color: "#cccccc" // Цвет ползунка

                    }
                    background: Rectangle {
                        color: "#444444" // Цвет фона полосы
                        radius: 3
                        anchors.fill: parent
                    }
                }


                // WheelHandler {
                //     id: wheelGuard
                //     acceptedDevices: PointerDevice.Mouse

                //     onWheel: (wheel) => {
                //         if (objectListView.count > 0) {
                //             wheel.accepted = true;  // блокируем событие, не доходит до карты
                //         } else {
                //             wheel.accepted = false; // пропускаем дальше, до карты
                //         }
                //     }
                // }
            }

            Button {
                text: sMenuRight.allChecked ? "Снять выделение" : "Выделить все"
                Layout.fillWidth: true

                onClicked: {
                        for (let i = 0; i < sideMenuModel.count; ++i) {
                            let obj = sideMenuModel.get(i)
                            obj.checked = !sMenuRight.allChecked
                        }
                        sMenuRight.allChecked = !sMenuRight.allChecked
                    }
            }

            Button {
                text: "Очистить список"
                Layout.fillWidth: true
                onClicked: {
                    sideMenuModel.clear()
                    onMenuRight()
                    sMenuRight.allChecked = false

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
                ToolTip.text: "Меню объектов"

                icon.width: 38
                icon.height: 38
                icon.source: "qrc:/qt/qml/Sigma_ROS/Icons/More_Grid_Big.svg"
                icon.color: hovered ? "red" : "white"

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



    Connections {
        target: sideMenuModel
        onCountChanged: {
            console.log("Модель в SideMenuRight изменилась, новый размер:", sideMenuModel.count)
        }
    }





}

