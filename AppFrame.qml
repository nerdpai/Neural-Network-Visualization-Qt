import QtQuick

Item {
    id: appFrame
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
        enabled: appFrame.isAppearingEnabled
    }

    transform: Translate{x: -appFrame.appearing}

    Behavior on opacity {
        SmoothedAnimation{duration: 500; velocity: -1}
        enabled: appFrame.isAppearingEnabled
    }

    Component.onCompleted: {
        appFrame.z = 1
    }

    onZChanged: {
        if(z === 1.0)
        {
            appFrame.isAppearingEnabled = true
            appFrame.appearing = 0.0
            appFrame.opacity = 1.0
        }

        else
        {
            appFrame.isAppearingEnabled = false
            appFrame.appearing = appFrame.appearingOffset
            appFrame.opacity = 0.0
        }
    }



    Connections{
        target: neural
        onUpdateAnswer:{
            var path = "qrc:/images/%1.png"
            answerImg.source = path.arg(answer)

        }

        onUpdateNeurons:{
            var index = 0
            for(var i = 0; i < neuronArea.layerIndex;i++)
            {
                for(var j = 0; j < neuronImg.layersCount[i];j++)
                {
                    neuronArea.neuronsList[index].cOpacity = weights[i][j]
                    index++
                }
            }
        }

        onUpdateNeuronsColor:
        {
            var color
            switch(colorIndex)
            {
                case 0:
                    color = "red"
                    break
                case 1:
                    color = "yellow"
                    break
                case 2:
                    color = "green"
                    break
                case 3:
                    color = "purple"
                    break
                case 4:
                    color = "orange"
                    break
            }
            for(var i = 0;i<neuronArea.neuronsList.length;i++)
            {
                neuronArea.neuronsList[i].cColor = color
                neuronArea.neuronsList[i].cOpacity = 0.0
                answerImg.source = "qrc:/images/nothing.png"
                neuronArea.layerIndex = 0
            }
        }
    }



    DrawSpace{
        id: drawSpace
        height: appFrame.height*0.9>(appFrame.width/3)?appFrame.width/3:appFrame.height*0.9
        width: appFrame.height*0.9>(appFrame.width/3)?appFrame.width/3:appFrame.height*0.9
        anchors.left: appFrame.left
        anchors.top: appFrame.top
        transform: Translate{x:(appFrame.width/3-drawSpace.width)/2;y:(appFrame.height-drawSpace.height)/2}
    }


    Item{
        id:imageArea
        height: appFrame.height*0.9>(appFrame.width/3)?appFrame.width/3:appFrame.height*0.9
        width: appFrame.height*0.9>(appFrame.width/3)?appFrame.width/3:appFrame.height*0.9
        anchors.right: appFrame.right
        anchors.top: appFrame.top
        transform: Translate{x:-(appFrame.width/3-imageArea.width)/2;y:(appFrame.height-imageArea.height)/2}

        Image {
            id: answerImg
            source: "qrc:/images/nothing.png"

            width:imageArea.height*0.8
            height:imageArea.height*0.8
            anchors.horizontalCenter: imageArea.horizontalCenter
            anchors.top:  imageArea.top

            transform: Translate{y:(imageArea.height-imageArea.height*0.8-imageArea.height*0.16)/3}
            smooth: false

        }
    }


    Item{
        id:neuronArea
        width: appFrame.width/3
        anchors.horizontalCenter: appFrame.horizontalCenter
        anchors.top: appFrame.top
        height: appFrame.height*0.9
        transform: Translate{y:appFrame.height*0.05}

        property var neuronsList: []
        property int layerIndex: 0

        Image {
            id: neuronImg
            source: "qrc:/images/Network2.svg"
            anchors.centerIn: neuronArea

            height: (neuronArea.height>neuronArea.width?neuronArea.width:neuronArea.height) *0.9
            width: (neuronArea.height>neuronArea.width?neuronArea.width:neuronArea.height) *0.9
            transform: Translate{y: -neuronImg.height*0.07}

            property var layersCount: [16,16,10]


            Component.onCompleted: {
                var offset = neuronImg.width*0.03
                var layerDistance = neuronImg.width*0.3
                var neuronsDistance = neuronImg.height*0.05

                for(var i = 0; i<layersCount.length;i++)
                {
                    for(var j = 0;j<layersCount[i];j++)
                    {

                        var y = (16-layersCount[i])/2
                        var circle = Qt.createQmlObject('import QtQuick
                                                        Rectangle
                                                        {
                                                            width: parent.width*0.05
                                                            height: width
                                                            color:  "#8b8f92"
                                                            border.color: "white"
                                                            border.width: 1+height*0.1
                                                            radius: height/2
                                                            property double xOffset: parent.width*35/900-width/2 + parent.width*415/900*'+i+'
                                                            property double yOffset: parent.width*35/900-width/2+parent.height*55.2/900*('+j+'+'+y+')
                                                            property double cOpacity: 0.0
                                                            property color cColor: "red"

                                                            anchors.top: parent.top
                                                            anchors.left: parent.left
                                                            transform: Translate{x:xOffset;y:yOffset;}}'
                                                        ,neuronImg,"DynamicallyLoaded")

                        var neuron = Qt.createQmlObject('import QtQuick
                                                        Rectangle
                                                        {
                                                        width: parent.width - parent.border.width*2.0
                                                        height: width
                                                        color:  parent.cColor
                                                        radius: height/2
                                                        opacity: parent.cOpacity

                                                        Behavior on opacity{
                                                            SmoothedAnimation {
                                                                velocity: -1
                                                                duration: 500
                                                            }
                                                        }

                                                        anchors.centerIn: parent

                                                        }'
                                                       ,circle,"DynamicallyLoaded")

                        var text = Qt.createQmlObject('import QtQuick
                                                        Text {
                                                            text: Math.round(parent.cOpacity*100)/100.0
                                                            anchors.centerIn: parent
                                                            opacity: 0.0

                                                            Behavior on opacity{
                                                                SmoothedAnimation {
                                                                    velocity: -1
                                                                    duration: 200
                                                                }
                                                            }

                                                            font.pixelSize: parent.height*0.7
                                                            color: "white"
                                                            style: Text.Outline
                                                            styleColor: "black"

                                                            MouseArea{
                                                                hoverEnabled: true
                                                                anchors.fill: parent
                                                                onEntered: parent.opacity = 1.0
                                                                onExited: parent.opacity = 0.0
                                                            }
                                                        }'
                                                       ,circle,"DynamicallyLoaded")


                        neuronArea.neuronsList.push(circle)
                    }
                }
            }
        }

        Rectangle{
            id: stepButton
            anchors.left: neuronImg.left
            anchors.top: neuronImg.bottom
            width: 0.25*neuronImg.width
            height: 0.13*neuronImg.height
            radius:height*0.2
            color:"#00b900"

            Behavior on color{
                ColorAnimation {
                    duration: 200
                }
            }
            onHeightChanged: {
                stepButton.radius = stepButton.height*0.2
            }

            transform: Translate{y: -neuronImg.height*0.03;  x: (neuronImg.width-stepButton.width*3)/4}

            Behavior on radius{SmoothedAnimation{velocity: -1; duration: 200}}

            Text{
                id: stepButtonText
                anchors.centerIn: stepButton
                text: qsTr("Step")
                font.pixelSize: stepButton.height/2
                color: "black"
            }

            MouseArea{
                hoverEnabled: true

                onClicked: {
                    if(neuronArea.layerIndex<3){
                        neuronArea.layerIndex += 1
                        neural.answer(drawSpace.selectList, neuronArea.layerIndex)
                    }
                }

                onPressed: {
                    stepButtonText.color = "yellow"
                    stepButton.radius = stepButton.height*0.3
                }

                onReleased: {
                    stepButtonText.color = "black"
                    stepButton.radius = stepButton.height*0.2
                }

                onEntered: {
                    stepButton.color = "#00e200"
                }

                onExited: {
                    stepButton.color = "#00b900"
                    stepButtonText.color = "black"
                    stepButton.radius = stepButton.height*0.2
                }

                anchors.fill: stepButton
            }
        }

        Rectangle{
            id: answerButton
            anchors.right: neuronImg.right
            anchors.top: neuronImg.bottom
            width: 0.25*neuronImg.width
            height: 0.13*neuronImg.height
            radius:height*0.2
            color:"#00b900"

            transform: Translate{y: -neuronImg.height*0.03; x: -(neuronImg.width-answerButton.width*3)/4}

            Behavior on color{
                ColorAnimation {
                    duration: 200
                }
            }
            onHeightChanged: {
                answerButton.radius = answerButton.height*0.2
            }

            //transform: Translate{y: -neuronImg.height*0.03}

            Behavior on radius{SmoothedAnimation{velocity: -1; duration: 200}}

            Text{
                id: answerButtonText
                anchors.centerIn: answerButton
                text: qsTr("Answer")
                font.pixelSize: answerButton.height/2
                color: "black"
            }

            MouseArea{
                hoverEnabled: true

                onClicked: {
                    neuronArea.layerIndex = 3
                    neural.answer(drawSpace.selectList, neuronArea.layerIndex)//
                }

                onPressed: {
                    answerButtonText.color = "yellow"
                    answerButton.radius = answerButton.height*0.3
                }

                onReleased: {
                    answerButtonText.color = "black"
                    answerButton.radius = answerButton.height*0.2
                }

                onEntered: {
                    answerButton.color = "#00e200"
                }

                onExited: {
                    answerButton.color = "#00b900"
                    answerButtonText.color = "black"
                    answerButton.radius = answerButton.height*0.2
                }

                anchors.fill: answerButton
            }
        }


//        Rectangle{
//            id: logButton
//            anchors.right: neuronImg.right
//            anchors.top: neuronImg.bottom
//            width: 0.25*neuronImg.width
//            height: 0.13*neuronImg.height
//            radius:height*0.2
//            color:"#00b900"

//            Behavior on color{
//                ColorAnimation {
//                    duration: 200
//                }
//            }
//            onHeightChanged: {
//                logButton.radius = logButton.height*0.2
//            }

//            transform: Translate{y: -neuronImg.height*0.03; x: -(neuronImg.width-logButton.width*3)/4}

//            Behavior on radius{SmoothedAnimation{velocity: -1; duration: 200}}

//            Text{
//                id: logButtonText
//                anchors.centerIn: logButton
//                text: qsTr("Log")
//                font.pixelSize: logButton.height/2
//                color: "black"
//            }

//            MouseArea{
//                hoverEnabled: true

//                onClicked: {

//                }

//                onPressed: {
//                    logButtonText.color = "yellow"
//                    logButton.radius = logButton.height*0.3
//                }

//                onReleased: {
//                    logButtonText.color = "black"
//                    logButton.radius = logButton.height*0.2
//                }

//                onEntered: {
//                    logButton.color = "#00e200"
//                }

//                onExited: {
//                    logButton.color = "#00b900"
//                    logButtonText.color = "black"
//                    logButton.radius = logButton.height*0.2
//                }

//                anchors.fill: logButton
//            }
//        }

    }


}
