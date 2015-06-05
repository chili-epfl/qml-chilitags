
import QtMultimedia 5.5
import Chilitags 1.0

import QtQuick 2.3
import QtQuick.Scene3D 2.0

Item {
    Text {
        text: "Click me!"
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Camera{
     id:camDevice
     //CameraCapture.resolution: "640x480"
     viewfinder.resolution:"640x480"
    }
    VideoOutput{
        source:camDevice
        filters:[chilitags]
    }
    Chilitags{
        id:chilitags
    ChilitagsObject{
        id: tag
        name: "tag_1023"
        }
    }

    Item {
        id: scene
        width: Math.min(parent.width, parent.height) - 100
        height: width
        anchors.centerIn: parent
        //color: "darkRed"

        Scene3D {
            anchors.fill: parent
            anchors.margins: 10
            focus: true
            aspects: "input"

            AnimatedEntity {}
        }
    }


}

