import QtQuick 2.2
import QtQuick.Window 2.1
import QtMultimedia 5.0
import CVCamera 1.0
import Chilitags 1.0

Window {
    visible: true
    width: camera.size.width
    height: camera.size.height
    maximumWidth: camera.size.width
    maximumHeight: camera.size.height

    //Set up physical camera
    CVCamera{
        id: camera
        device: 0
        size: "640x480"
    }

    //Set up detection
    Chilitags{
        id: chilitags
        sourceImage: camera.cvImage

        //We declare the detection of tag #0
        ChilitagsObject{
            id: tag
            name: "tag_0"
        }
    }

    //Set up visual output
    VideoOutput{
        source: camera

        //This item describes the camera frame, hence its transform is the camera matrix, i.e the projection matrix
        //Its children will have transform relative to this frame, so we can give them transforms coming directly from Chilitags
        //We could have put this outside the VideoOutput but this is semantically more elegant
        Item{
            id: cameraFrame
            transform: MatrixTransform{ matrix: chilitags.projectionMatrix }

            //We draw a blue rectangle on top of the tag, its default size is 20 mm
            Rectangle{
                color: "blue"
                width: 20
                height: 20
                transform: MatrixTransform{ matrix: tag.transform }
                visible: tag.visible
            }
        }
    }
}

