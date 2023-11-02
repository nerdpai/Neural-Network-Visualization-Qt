import QtQuick

Item {
    id: settingsFrame
//    width: rWindow.width
//    height: rWindow.height - frameChanger.height
//    anchors.top: frameChanger
//    anchors.left: rWindow
//    anchors.right: rWindow
//    anchors.bottom: rWindow
    opacity: 0.0

    z: 0

    property double appearingOffset: rWindow.width*0.05
    property double appearing: rWindow.width*0.05
    property bool isAppearingEnabled: false

    Behavior on appearing {
        SmoothedAnimation{duration: 500; velocity: -1}
        enabled: settingsFrame.isAppearingEnabled
    }

    transform: Translate{x: -settingsFrame.appearing}

    Behavior on opacity {
        SmoothedAnimation{duration: 500; velocity: -1}
        enabled: settingsFrame.isAppearingEnabled
    }

    onZChanged: {
        if(z === 1.0)
        {
            settingsFrame.isAppearingEnabled = true
            settingsFrame.appearing = 0.0
            settingsFrame.opacity = 1.0
        }

        else
        {
            settingsFrame.isAppearingEnabled = false
            settingsFrame.appearing = settingsFrame.appearingOffset
            settingsFrame.opacity = 0.0
        }
    }

    Column{
        id: column
        anchors.centerIn: settingsFrame

        property double buttonsOffset: height/10
        property double buttonHeight: (height - buttonsOffset) *0.75/5
        property double buttonWidth: buttonHeight*2.5
        spacing: buttonHeight/0.75*0.1

        height: settingsFrame.height
        width: settingsFrame.width/5


        Rectangle {
            height: column.buttonsOffset
            color:"#00000000"
            width: 20
        }


        ButtonSet{
            width: column.width
            height: column.buttonHeight
            color: "red"
            text: "PerfectSet"
            setIndex: 0
        }

        ButtonSet{
            width: column.width
            height: column.buttonHeight
            color: "yellow"
            text: "GoodSet"
            setIndex: 1
        }

        ButtonSet{
            width: column.width
            height: column.buttonHeight
            color: "green"
            text: "NormSet"
            setIndex: 2
        }

        ButtonSet{
            width: column.width
            height: column.buttonHeight
            color: "purple"
            text: "BadSet"
            setIndex: 3
        }

        ButtonSet{
            width: column.width
            height: column.buttonHeight
            color: "orange"
            text: "WithoutSet"
            setIndex: 4
        }
    }
}
