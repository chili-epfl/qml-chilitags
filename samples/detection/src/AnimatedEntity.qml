import Qt3D 2.0
import Qt3D.Render 2.0

import QtQuick 2.3 as QQ2


Entity {
    id: sceneRoot

    Camera {
        id: camera
        projectionType: CameraLens.PerspectiveProjection
        fieldOfView: 45
        aspectRatio: 16/9
        nearPlane : 0.1
        farPlane : 1000.0
        position: Qt.vector3d( 0.0, 0.0, -40.0 )
        upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
        viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
    }

    Configuration  {
        controlledCamera: camera
    }

    components: [
        FrameGraph {
            activeFrameGraph: Viewport {
                id: viewport
                rect: Qt.rect(0.0, 0.0, 1.0, 1.0) // From Top Left
                clearColor: Qt.rgba(0, 0, 0, 0)

                CameraSelector {
                    id : cameraSelector
                    camera: camera

                    ClearBuffer {
                        buffers : ClearBuffer.ColorDepthBuffer
                    }
                }
            }
        }
    ]

    PhongMaterial {
        id: material
    }


    SphereMesh {
        id: sphereMesh
        radius: 3
    }

    Transform {
        id: sphereTransform
        MatrixTransform{
            matrix: tag.transform
        }
    }

    Entity {
        id: sphereEntity
        components: [ sphereMesh, material, sphereTransform ]
    }
}

