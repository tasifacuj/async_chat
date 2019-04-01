import QtQuick 2.0

Item {
    id: menuPanel
    width: parent.width
    height: parent.height
    property bool startedFromRemote: false

    onStartedFromRemoteChanged: {
        if( startedFromRemote ){
            console.log( "startedFromRemote: ", startedFromRemote );
            startGame( true );
        }
    }

    function startGame( remote ){
        if( gameData.users.length > 0 ){
            gameData.resetGame( remote )
            gameView.centerBoard()
            menuPanel.state = "hide"
            gameData.pauseGame(false)
        }else{
            console.warn( "Can't start without any participant" );
        }
    }

    Item {
        id: minimizeButton
        width: 80
        height: 80
        anchors.top: parent.top
        anchors.left: parent.left
        Image {
            source: "qrc:/images/minimize.png"
            anchors.centerIn: parent
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                mainWidget.minimizeWindow();
            }
        }
    }

    Item {
        id: exitButton
        width: 80
        height: 80
        anchors.top: parent.top
        anchors.right: parent.right

        Image {
            source: "qrc:/images/close.png"
            anchors.centerIn: parent
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                mainWidget.exitApplication()
            }
        }
    }

    Frame {
        id: menuPanelFrame
        width: 540
        height: 370
        anchors.centerIn: parent

        Image {
            id: gameLogo
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            source: "qrc:/images/tile_button1.png"
            Text {
                anchors.centerIn: parent
                font.family: "Helvetica"
                font.pointSize: 28
                smooth: true
                color: "white"
                style: Text.Outline
                styleColor: "black"
                text: "5-in-a-row"
            }
        }
        Row {
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.top: gameLogo.bottom
            anchors.margins: 10
            spacing: 10

            Frame {
                id: aboutArea
                height: 270
                width: 150
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#ffffff" }
                    GradientStop { position: 1.0; color: "#eeeeee" }
                }

                Flickable {
                    id: aboutAreaFlickable
                    anchors.fill: parent
                    contentWidth: aboutArea.width
                    contentHeight: users.height  + 20
                    flickableDirection: Flickable.VerticalFlick
                    clip: true

                   Users{
                       anchors.horizontalCenter: parent.horizontalCenter
                       anchors.verticalCenter: parent.verticalCenter
                       id: users
                       width:  parent.width - 20
                   }
                }
            }

            Column {
                width: parent.width - aboutArea.width - 10
                spacing: 10

                Button {
                    id: button2PlayerGame
                    text: "2 Players Game"
                    onClicked: startGame( false )
                }

                Button {
                    id: buttonContinue
                    text: "Continue Game"
                    // "Continue" button is enabled only when game is on
                    disabled: {
                        if (!gameData.gameOn)
                            return true
                        else
                            return false
                    }
                    onClicked: {
                        menuPanel.state = "hide"
                        gameData.pauseGame(false)
                    }
                }
            }
        }
    }

    states: [
    State {
        name: "hide"
        PropertyChanges {
            target: menuPanel
            opacity: 0.0
            y: -menuPanelFrame.height
        }
    }
    ]

    transitions: Transition {
        NumberAnimation {
            properties: "opacity, y"
            duration: 400
        }
    }

}
