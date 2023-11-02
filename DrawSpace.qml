import QtQuick

Item {
    id: drawSpace
//    height: appFrame.height>(appFrame.width/3)?appFrame.width/3:appFrame.height
//    width: appFrame.height>(appFrame.width/3)?appFrame.width/3:appFrame.height
//    anchors.left: appFrame.left
//    anchors.top: appFrame.top
//    transform: Translate{x:(appFrame.width/3-drawSpace.width)/2;y:(appFrame.height-drawSpace.height)/2}

    property var selectList: []

    MouseArea
    {
        hoverEnabled: true
        onExited: {
            drawCanvas.isLeftPressed = false
            drawCanvas.isRightPressed = false
        }
        anchors.fill: drawSpace
    }

    Item{
        id: drawCanvas
        property int rows: 28
        property int columns: 28
        width:drawSpace.height*0.8
        height:drawSpace.height*0.8
        objectName: "drawCanvas"
        anchors.horizontalCenter: drawSpace.horizontalCenter
        anchors.top:  drawSpace.top

        transform: Translate{y:(drawSpace.height-drawCanvas.height-cleanButton.height)/3}

        property bool isRightPressed: false
        property bool isLeftPressed: false
        property var rectList: []


        Component.onCompleted: {
            for(var i = 0; i< drawCanvas.rows; i++){
                for(var j = 0; j< drawCanvas.columns; j++){
                    var index = i*drawCanvas.rows+j
                    var rec = Qt.createQmlObject('import QtQuick
                                                    Rectangle{
                                                        property int index: '+index+'
                                                        color:"#767778"
                                                        width:drawCanvas.width/drawCanvas.columns
                                                        height:drawCanvas.height/drawCanvas.rows
                                                        anchors.left: drawCanvas.left
                                                        anchors.top: drawCanvas.top
                                                        transform: Translate{x:'+j+'*(drawCanvas.width/drawCanvas.columns);y:'+i+'*(drawCanvas.height/drawCanvas.rows)}
                                                    }',

                                                        drawCanvas,"DynamicallyLoaded")

                    Qt.createQmlObject('import QtQuick
                                        import QtQuick.Controls 6.3
                                        MouseArea{
                                            acceptedButtons: Qt.LeftButton | Qt.RightButton
                                            hoverEnabled: true;
                                            onClicked: (mouse)=> {
                                                if(mouse.button === Qt.RightButton){
                                                    if(!drawCanvas.isRightPressed){
                                                        drawCanvas.isRightPressed = true
                                                        drawCanvas.isLeftPressed = false
                                                        parent.color = "#767778"
                                                        drawSpace.selectList[parent.index] = false
                                                    }
                                                    else drawCanvas.isRightPressed = false
                                                }


                                                else if(mouse.button === Qt.LeftButton){
                                                    if(!drawCanvas.isLeftPressed){
                                                        drawCanvas.isLeftPressed = true
                                                        drawCanvas.isRightPressed = false
                                                        parent.color = "#e2e3e4"
                                                        drawSpace.selectList[parent.index] = true
                                                    }
                                                    else drawCanvas.isLeftPressed = false
                                                }
                                            }
                                            onEntered: {
                                                if(drawCanvas.isRightPressed){
                                                    parent.color = "#767778"
                                                    drawSpace.selectList[parent.index] = false
                                                }
                                                else if(drawCanvas.isLeftPressed){
                                                    drawSpace.selectList[parent.index] = true
                                                    parent.color = "#e2e3e4"
                                                }
                                            }
                                            anchors.fill:parent
                                        }',

                                       rec, "DynamicallyLoaded")

                    rectList.push(rec)
                    drawSpace.selectList.push(false)
                }
            }
        }

    }


    Rectangle{
        id: cleanButton
        anchors.horizontalCenter: drawSpace.horizontalCenter
        anchors.top: drawCanvas.bottom
        width: 0.6*drawCanvas.width
        height: 0.2*drawCanvas.height
        radius:height*0.2
        color:"#00b900"

        Behavior on color{
            ColorAnimation {
                duration: 200
            }
        }

        transform: Translate{y:(drawSpace.height-drawCanvas.height-cleanButton.height)}

        Behavior on radius{SmoothedAnimation{velocity: -1; duration: 200}}

        onHeightChanged:  cleanButton.radius = cleanButton.height*0.2

        Text{
            id: cleanButtonText
            anchors.centerIn: cleanButton
            text: qsTr("Clean")
            font.pixelSize: cleanButton.height/2
            color: "black"
        }

        MouseArea{
            hoverEnabled: true

            onClicked: {
                for(var i = 0; i< drawCanvas.rectList.length;i++)
                {
                    drawSpace.selectList[i] = false
                    drawCanvas.rectList[i].color = "#767778"
                }
                answerImg.source = "qrc:/images/nothing.png"

                var index = 0
                for(var i = 0; i < neuronArea.layerIndex;i++)
                {
                    for(var j = 0; j < neuronImg.layersCount[i];j++)
                    {
                        neuronArea.neuronsList[index].cOpacity = 0.0
                        index++
                    }
                }

                neuronArea.layerIndex = 0
            }

            onPressed: {
                cleanButtonText.color = "yellow"
                cleanButton.radius = cleanButton.height*0.3
            }

            onReleased: {
                cleanButtonText.color = "black"
                cleanButton.radius = cleanButton.height*0.2
            }

            onEntered: {
                cleanButton.color = "#00e200"
            }

            onExited: {
                cleanButton.color = "#00b900"
                cleanButtonText.color = "black"
                cleanButton.radius = cleanButton.height*0.2
            }

            anchors.fill: cleanButton
        }
    }
}
