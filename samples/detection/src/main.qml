
import QtMultimedia 5.5
import Chilitags 1.0
import QtQuick 2.0
import QtQuick.Scene3D 2.0
import QtQuick.Window 2.2

Item {
    Camera{
        id:camDevice
        imageCapture.resolution: "640x480" //Android sets the viewfinder resolution as the capture one
        viewfinder.resolution:"640x480"
    }


    VideoOutput{
        //width: 640
        //height: 480
        //scale: Screen.width/width
        anchors.centerIn: parent
        //anchors.fill: parent
        source:camDevice
        filters:[chilitags]
        Scene3D {
        anchors.fill: parent
        focus: true
        aspects: "input"
        AnimatedEntity {}
        }
    }


    Chilitags{
        id:chilitags
        chiliobjects: [tag]

    }

    ChilitagsObject{
        id: tag
        name: "tag_1018"
    }







}


