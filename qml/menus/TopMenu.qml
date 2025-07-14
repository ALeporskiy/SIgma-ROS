import QtQuick
// import QtQuick.Controls
// import QtLocation
// import QtPositioning
import SpaceSim

Rectangle
{
    id: topMenu
    anchors.topMargin: 5
    anchors.top: parent.top
    anchors.horizontalCenter: parent.horizontalCenter

    border.width: 1
    border.color: "black"
    opacity: 0.8
    radius: 5


    property string placeName: ""
    property string currentTime: Qt.formatDateTime(new Date(), "hh:mm:ss")



    width: 300
    height: 150
    color: "dimgrey"



    TrajectoryManager {
        id: trajectoryManager
        Component.onCompleted: {
                trajectoryManager.loadTLE("TLE3.TLE")
            }
        onCurrentPositionChanged: {
            positionDisplayLatitude.text = trajectoryManager.currentPosition.latitude.toFixed(4).toString()
            positionDisplayLongitude.text = trajectoryManager.currentPosition.longitude.toFixed(4).toString()

            if (!lookupTimer.running && trajectoryManager.currentPosition.isValid) {
                    lookupTimer.start() // запуск таймера один раз
                    lookupPlace(
                        trajectoryManager.currentPosition.latitude,
                        trajectoryManager.currentPosition.longitude
                    )
                }




        }

    }

    Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 4

        Row {


            Text {
                id: positionDisplayText
                font.pixelSize: 14
                color: "white"
                text: "Подспутниковая точка:"
                rightPadding: 5
            }

            Text {
                id: positionDisplayLatitude
                font.pixelSize: 14
                color: "white"
                text: ""
                rightPadding: 5
            }

            Text {
                id: positionDisplayLongitude
                font.pixelSize: 14
                color: "white"
                text: ""
                rightPadding: 5
            }
        }

        Row {

            Text {
                id: locationDisplay
                color: "white"
                font.pixelSize: 14
                text: "Летим над: " + topMenu.placeName
                rightPadding: 5
                wrapMode: Text.Wrap
                maximumLineCount: 2

                width: topMenu.width
            }
        }

        Row {

            Text {
                id: locationDisplayDate
                color: "white"
                font.pixelSize: 14
                text: "Дата: " + Qt.formatDate(new Date(), "dd.MM.yyyy")
                rightPadding: 5
                //topPadding: 15
            }
        }

        Row {

            Text {
                id: locationDisplayTime
                color: "white"
                font.pixelSize: 14
                text: "Время: " + topMenu.currentTime
                rightPadding: 5
            }
        }

        Row {

            Text {
                id: locationDisplayHeight
                color: "white"
                font.pixelSize: 14
                text: "Высота: " + trajectoryManager.currentAltitudeKm.toFixed(2) + " км"
                rightPadding: 5
            }

            Text {
                id: locationDisplayRev
                color: "white"
                font.pixelSize: 14
                text: "№ Витка: " + trajectoryManager.currentRevNumber
                rightPadding: 5
            }
        }
    }

    Timer {
            interval: 1000
            running: true
            repeat: true
            onTriggered: {
                topMenu.currentTime = Qt.formatDateTime(new Date(), "hh:mm:ss")
            }
        }

    Timer {
        id: lookupTimer
        interval: 60000   // 60 000 мс = 60 секунд
        running: false
        repeat: true

        onTriggered: {
            if (trajectoryManager.currentPosition.isValid) {
                lookupPlace(
                    trajectoryManager.currentPosition.latitude,
                    trajectoryManager.currentPosition.longitude
                )
            }
        }
    }

    function lookupPlace(lat, lon) {
        console.log("lookupPlace: координаты", lat, lon)

        var xhr = new XMLHttpRequest()
        var key = "a495e112d43542998d75f5973f53767d"
        var url = "https://api.opencagedata.com/geocode/v1/json?q=" +
                  lat + "+" + lon + "&key=" + key + "&language=ru"

        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                console.log("HTTP статус:", xhr.status)

                if (xhr.status === 200) {
                    var result = JSON.parse(xhr.responseText)
                    console.log("API ответ:", JSON.stringify(result, null, 2))

                    if (result.results.length > 0) {
                        var comp = result.results[0].components

                        if (comp.country || comp.city || comp.state) {
                            topMenu.placeName = (comp.country || "") +
                                        (comp.city ? ", " + comp.city :
                                         comp.state ? ", " + comp.state : "")
                        } else if (comp.body_of_water) {
                            topMenu.placeName = comp.body_of_water
                        } else {
                            topMenu.placeName = "Над неизвестной областью"
                        }
                    } else {
                        topMenu.placeName = "Нет данных"
                    }
                } else {
                    topMenu.placeName = "Ошибка HTTP " + xhr.status
                }
            }
        }

        xhr.open("GET", url)
        xhr.send()
    }





}
