import QtQuick
import QtQuick.Window
import QtQuick.Controls 6.3
Window {
    width: 640
    height: 480
    visible: true
    visibility: Window.Windowed
    title: qsTr("NeuralNetwork")
    id: rWindow


    AppMenu
    {
        id: frameChanger
        width: rWindow.width
        height: rWindow.height/10
        anchors.top: rWindow.top
        anchors.left: rWindow.left
        anchors.right: rWindow.right

        onWhichFrameChanged: {
            if(frameChanger.whichFrame === AppMenu.ListOfFrames.App){
                settingsFrame.z = 0
                appFrame.z = 1
                //neuron.howManyColor = 10.0
            }
            else{
                appFrame.z = 0
                settingsFrame.z = 1
                //neuron.howManyColor = 0.0
            }
        }
    }

    AppFrame
    {
        id: appFrame
        width: rWindow.width
        height: rWindow.height - frameChanger.height
        anchors.top: frameChanger.bottom
        anchors.left: rWindow.left
        anchors.right: rWindow.right
        anchors.bottom: rWindow.bottom
    }

    SettingsFrame
    {
        id: settingsFrame
        width: rWindow.width
        height: rWindow.height - frameChanger.height
        anchors.top: frameChanger.bottom
        anchors.left: rWindow.left
        anchors.right: rWindow.right
        anchors.bottom: rWindow.bottom
    }

//    Item {
//        id: neuron
//        width: 300
//        height: 300
//        anchors.centerIn: rWindow
//        property double howManyColor: 0.0

//        Behavior on howManyColor {SmoothedAnimation{velocity:3}}

//        Rectangle
//        {
//            id: sphere
//            anchors.fill: neuron
//            radius: 150
//            gradient: Gradient


//            {
//                GradientStop{position: 1.0-neuron.howManyColor; color:"#00ffffff";}
//                GradientStop{position: 1.0; color:"red"}
//            }
//        }
//    }


//    MouseArea {
//        id:area
//        anchors.fill: parent
//        hoverEnabled:true
//        preventStealing:true
//        property double factor: 1.2

//        property bool a: false

//       onClicked: {

//           var btn = Qt.createQmlObject('import QtQuick; import QtQuick.Controls 6.3; Rectangle{id:_'+mouseX+''+mouseY+' ;width:25;height:25;color:"red";x:'+mouseX+';y:'+mouseY+'}',rWindow,"DynamicallyLoaded")
//       }

//    }
}
