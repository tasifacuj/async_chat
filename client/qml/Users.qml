import QtQuick 2.0

Item {
    property alias curIdx: usersId.currentIndex

    onCurIdxChanged:  {
        gameData.selectUser( curIdx )
    }


    ListView{
        id: usersId
        width: 100
        height: 100

        Component{
            id: userDelegate
            Item{
                height: 25
                width: 100
                Text{
                    text: modelData
                }

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        usersId.currentIndex = index
                    }
                }
            }
        }

        highlight:  Rectangle{
            color: 'lightgrey'

        }


        model: gameData.users
        delegate: userDelegate


    }
}
