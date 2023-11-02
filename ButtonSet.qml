import QtQuick

Item {
    id: buttonSet
    property string text: "text"
    width: 300
    height: 300
    property color color: "green"
    property int setIndex: -1

    Rectangle
    {
        id: realyButton
        color: "#d4d4d4"
        anchors.fill: buttonSet
        property color smallRectangleColor: buttonSet.color
        property string text: buttonSet.text
        property double smallestSide: width<height?width:height
        property int setIndex: buttonSet.setIndex

        Behavior on radius{SmoothedAnimation{velocity: -1; duration: 200}}

        Text {
            id:realyButtonText
            text: qsTr(realyButton.text)
            anchors.centerIn: realyButton
            font.pixelSize: realyButton.smallestSide/3
        }

        Rectangle
        {
            id:colorModel
            color: realyButton.smallRectangleColor
            height: realyButton.smallestSide/5
            width: height
            anchors.top: realyButton.top
            anchors.right: realyButton.right
            transform: Translate{x: -realyButton.smallestSide/10 ;y:realyButton.smallestSide/10}
        }

        MouseArea
        {
            id: areaRealyButton
            hoverEnabled: true
            anchors.fill: parent

            onEntered: {
                realyButton.color = "#939393"
            }

            onExited: {
                realyButton.color = "#d4d4d4"
            }

            onPressed: {
                realyButton.radius = realyButton.height*0.2
            }

            onReleased: {
                 realyButton.radius = 0
            }

            onClicked: {
                neural.setSet(realyButton.setIndex)
            }
        }


    }
}
