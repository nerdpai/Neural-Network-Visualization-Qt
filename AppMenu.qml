import QtQuick

Item {
    id: frameChanger
//    width: rWindow.width
//    height: rWindow.height/10
//    anchors.top: rWindow
//    anchors.left: rWindow

    property color colorFrameSelected: "#00e200"
    property color colorFrameUnSelected: "green"
    property color colorIndicatorSelected: "#00be00"
    property color colorIndicatorUnSelected: "#dbdbdb"

    enum ListOfFrames{
        App,
        Settings
    }

    property int whichFrame: AppMenu.ListOfFrames.App


    Text {
        id: appFrameButton
        text: qsTr("App")
        anchors.centerIn: frameChanger
        transform: Translate{x:-frameChanger.width*0.1 - appMetrics.boundingRect.right/2}
        font.pixelSize: frameChanger.height/2
        color: frameChanger.colorFrameSelected
        property bool isChecked: true

        Behavior on color{
            ColorAnimation {
                duration: 200
            }
        }

        onIsCheckedChanged: {
            if(isChecked)
            {
                settingsFrameButton.isChecked = false
                frameChanger.whichFrame = AppMenu.ListOfFrames.App


                appFrameButton.color = frameChanger.colorFrameSelected
                appIndicator.color = frameChanger.colorIndicatorSelected
                appIndicator.width = appIndicator.maxWidth
            }
            else
            {
                appFrameButton.color = frameChanger.colorFrameUnSelected
                appIndicator.color = frameChanger.colorIndicatorUnSelected
                appIndicator.width = appIndicator.height
            }
        }

        TextMetrics {
            id:     appMetrics
            font:   appFrameButton.font
            text:   appFrameButton.text
        }

        Rectangle{
            id: appIndicator
            anchors.centerIn: appFrameButton
            transform: Translate{y: + appMetrics.boundingRect.bottom*4}
            width: appMetrics.boundingRect.right*0.8
            height: appMetrics.boundingRect.bottom*0.5
            color: frameChanger.colorIndicatorSelected
            radius: appIndicator.height/2
            property double maxWidth: appMetrics.boundingRect.right*0.8

            Behavior on width{SmoothedAnimation{duration: 200; velocity: -1;}}
        }

        MouseArea{
            id: appButton
            anchors.centerIn: appFrameButton
            width: appMetrics.boundingRect.right*1.3
            height: frameChanger.height*1.1

            onClicked: {
                appFrameButton.isChecked = true
            }

            hoverEnabled: true
            onExited: {
                if(!appFrameButton.isChecked)
                    appIndicator.width = appIndicator.height
            }
            onEntered: {
                appIndicator.width = appIndicator.maxWidth
            }
        }

    }

    Text {
        id: settingsFrameButton
        text: qsTr("Settings")
        anchors.centerIn: frameChanger
        transform: Translate{x:+frameChanger.width*0.1 + settingsMetrics.boundingRect.right/2}
        font.pixelSize: frameChanger.height/2
        color: frameChanger.colorFrameUnSelected
        property bool isChecked: false

        Behavior on color{
            ColorAnimation {
                duration: 200
            }
        }

        onIsCheckedChanged: {
            if(isChecked)
            {
                appFrameButton.isChecked = false
                frameChanger.whichFrame = AppMenu.ListOfFrames.Settings


                settingsFrameButton.color = frameChanger.colorFrameSelected
                settingsIndicator.color = frameChanger.colorIndicatorSelected
                settingsIndicator.width = settingsIndicator.maxWidth
            }
            else
            {
                settingsFrameButton.color = frameChanger.colorFrameUnSelected
                settingsIndicator.color = frameChanger.colorIndicatorUnSelected
                settingsIndicator.width = settingsIndicator.height
            }
        }


        TextMetrics {
            id:     settingsMetrics
            font:   settingsFrameButton.font
            text:   settingsFrameButton.text
        }

        Rectangle{
            id: settingsIndicator
            color: frameChanger.colorIndicatorUnSelected
            anchors.centerIn: settingsFrameButton
            transform: Translate{y: + settingsMetrics.boundingRect.bottom*4}
            width: settingsMetrics.boundingRect.bottom*0.5
            height: settingsMetrics.boundingRect.bottom*0.5
            radius: appIndicator.height/2
            property double maxWidth: settingsMetrics.boundingRect.right*0.8
            Behavior on width{SmoothedAnimation{duration: 200; velocity: -1}}

            Behavior on color{
                ColorAnimation {
                    duration: 300
                }
            }

        }

        MouseArea{
            id: settingsButton
            anchors.centerIn: settingsFrameButton
            width: settingsMetrics.boundingRect.right*1.3
            height: frameChanger.height*1.1

            onClicked: {
                settingsFrameButton.isChecked = true
            }

            hoverEnabled: true
            onExited: {
                if(!settingsFrameButton.isChecked)
                    settingsIndicator.width = settingsIndicator.height
            }
            onEntered: {
                settingsIndicator.width = settingsIndicator.maxWidth
            }
        }
    }

}
