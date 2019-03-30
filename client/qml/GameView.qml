import QtQuick 2.0
import "."

Item {
    id: gameView
    anchors.fill: parent

    // Function to center the board when starting new game
    function centerBoard() {
//        centeringAnimation.start()
    }

    // *** Background image ***
    Image {
        id: backgroundImage
        source: "qrc:/images/background.jpg"
        // Make the background "parallax scrolling"
        x: 0
        y: 0
    }

    // *** Board ***
//    Flickable {
//        id: boardFlickable
//        anchors.fill: parent
//        contentWidth: boardGrid.width + controlPanel.width
//        contentHeight: boardGrid.height
//        maximumFlickVelocity: 500
//        flickDeceleration: 1000

//        // This implements "Fluid" 3D tilting
//        transform: [
//        Rotation {
//            origin.x: gameView.width / 2
//            origin.y: gameView.height / 2
//            axis { x: 1; y: 0; z: 0 }
//            angle: 0
//            Behavior on angle { SpringAnimation { spring: 1.5; damping: 0.75 } }
//        },
//        Rotation {
//            origin.x: gameView.width / 2
//            origin.y: gameView.height / 2
//            axis { x: 0; y: 1; z: 0 }
//            angle: 0
//            Behavior on angle { SpringAnimation { spring: 1.5; damping: 0.75 } }
//        }
//        ]

//        Grid {
//            // Board is 19x19 tiles
//            id: boardGrid
//            columns: 19
//            spacing: 2
//            x: 20
//            y: 20

//            Repeater {
//                model: gameData.tiles
//                Tile { }
//            }
//        }
//    }
    Grid {
        // Board is 19x19 tiles
        id: boardGrid
        columns: 19
        spacing: 2
        width: parent.width - 50

        x: 20
        y: 20

        Repeater {
            model: gameData.tiles
            Tile { }
        }


    }

//    // *** Animation to center the board ***
//    ParallelAnimation {
//        id: centeringAnimation
//        NumberAnimation {
//            target: boardFlickable
//            property: "contentX"
//            to: boardFlickable.contentWidth / 2 - boardFlickable.width / 2
//            duration: 1000
//            easing.type: Easing.InOutQuad
//        }
//        NumberAnimation {
//            target: boardFlickable
//            property: "contentY"
//            to: boardFlickable.contentHeight / 2 - boardFlickable.height / 2
//            duration: 1000
//            easing.type: Easing.InOutQuad
//        }
//    }

    // *** Control panel ***
    ControlPanel {
        id: controlPanel
        anchors.right: boardGrid.right
        anchors.rightMargin: 10
    }
}
