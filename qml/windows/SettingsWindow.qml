import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtPositioning

Window
    {

    width: 400; height: 100
    title: "Ввод координат"
    property var mapCenterLatitude: ""
    property var mapCenterLongitude: ""


    Column
    {
        spacing: 5
        anchors.fill: parent
        anchors.topMargin: 10
        anchors.leftMargin: 10

        Button
        {
            id: b1

            width: 30
            height: 30

            onClicked:
            {
                // map.center = QtPositioning.coordinate(54.2,16.2)
                // map.zoomLevel = 10
                let lat = parseFloat(mapCenterLatitude);
                                let lon = parseFloat(mapCenterLongitude);

                                if (!isNaN(lat) && !isNaN(lon)) {
                                    map.center = QtPositioning.coordinate(lat, lon);
                                } else {
                                    console.log("Неверный формат координат");
                                }


            }



        }

        TextField {
                    id: latitudeField
                    placeholderText: "Введите широту"
                    onTextChanged: mapCenterLatitude = text
                }

                TextField {
                    id: longitudeField
                    placeholderText: "Введите долготу"
                    onTextChanged: mapCenterLongitude = text
                }
    }
}
