import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.5

Window {
    id: mainWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    property bool showMenu: true
    property string captionText: "Nine men's morris"

    Board {
        anchors.centerIn: parent
        playable: !showMenu;

        onInEndStateChanged: {
            if(!inEndState)
                return
            var winner = stateModel.currentPlayer
            showMenu = true
            console.debug("Winner: " + winner)
            if(winner === "black" || winner === "white")
                mainWindow.captionText = winner + " wins!";
            else
                mainWindow.captionText = "Draw!"
        }
    }

    Rectangle {
        function startGame(whiteAi, blackAi)
        {
            stateModel.initGame(whiteAi, blackAi)
            mainWindow.showMenu = false;
        }

        id: menu
        width: 300
        height: 200
        color: "#dda4a4a4"
        radius: 20
        visible: showMenu
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter


        Button {
            id: playerVsPlayerButton
            x: 95
            y: 0
            width: 200
            text: qsTr("Player vs Player")
            anchors.bottomMargin: 10
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter

            onClicked: {
                menu.startGame("none", "none")
            }
        }

        Button {
            id: aiVsPlayerButton
            x: 109
            y: 52
            width: 200
            text: qsTr("Player vs AI")
            anchors.bottom: playerVsPlayerButton.top
            anchors.bottomMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter

            onClicked: {
                menu.startGame("none", "minmax")
            }
        }

        Text {
            id: caption
            x: 138
            text: mainWindow.captionText
            anchors.horizontalCenterOffset: 0
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 30
        }

        Text {
            id: descr
            x: 138
            text: qsTr("Choose a new game")
            anchors.top: caption.bottom
            anchors.topMargin: 14
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 15
        }
    }
}
