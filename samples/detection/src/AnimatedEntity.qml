import Qt3D 2.0
import Qt3D.Render 2.0



Entity {
    id: sceneRoot

    Camera {
        id: camera
        projectionType: CameraLens.FrustumProjection
        nearPlane : 0.1
        farPlane : 1000

        top: 0.1*(chilitags.projectionMatrix.m23/chilitags.projectionMatrix.m11)
        bottom: -0.1*(chilitags.projectionMatrix.m23/chilitags.projectionMatrix.m11)
        left: -0.1*(chilitags.projectionMatrix.m13/chilitags.projectionMatrix.m22)
        right: 0.1*(chilitags.projectionMatrix.m13/chilitags.projectionMatrix.m22)

        position: Qt.vector3d( 0.0, 0.0, -1 )
        upVector: Qt.vector3d( 0.0, -1.0, 0.0 )
        viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
    }

    Configuration  {
        controlledCamera: camera
        onControlledCameraChanged: console.log(sphereTransform.matrix)

    }

    components: [

        FrameGraph {
            activeFrameGraph:
                ForwardRenderer {
                id: viewport
                clearColor: "transparent"
                camera:camera
            }
        }
    ]

    PhongMaterial {
        id: material
    }


    CuboidMesh {
        id: cuboidMesh
        xExtent: 20
        yExtent: 20
        zExtent: 0.5
    }

    Transform {
        id: sphereTransform

        Translate{dx:10
                  dy:10}
        MatrixTransform{matrix: tag.transform}

    }

    Entity {
        id: sphereEntity
        components: [ cuboidMesh, material, sphereTransform ]

    }

}

