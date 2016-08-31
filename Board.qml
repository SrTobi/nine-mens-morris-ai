import QtQuick 2.5
import QtQml.Models 2.2

/*
 * The Board where the game is played on.
 * boardRect is surrounding the actual board to ensure correct aspect ratio
 */
Rectangle {
    id: boardRect
    color: "#c18a34"

    signal gameEnded (string winner)
    property bool playable: true
    property bool inEndState: stateModel.phase === "end"
    onInEndStateChanged: {
        boardRect.gameEnded(stateModel.currentPlayer)
    }

    anchors.fill: parent

    Rectangle {
        function stoneNameToImage(name) {
            switch(name) {
            case "white":
                return "media/white-stone.svg";
            case "black":
                return "media/black-stone.svg";
            default:
            }
            return "media/neutral-stone.svg";
        }

        function size() {
            return Math.min(parent.width, parent.height);
        }

        property var movedStoneOriginField: null
        property bool playable: boardRect.playable && !inEndState
        property bool inMoveState: stateModel.phase === "move";
        property bool inPutState: stateModel.phase === "put";
        property bool inRemoveState: stateModel.phase === "remove";
        property bool inEndState: boardRect.inEndState
        property bool movablePlayerActive: stateModel.isMovablePlayer(stateModel.currentPlayer)


        id: board
        color: "#c18a34"
        border.color: "#000000"
        anchors.centerIn: parent
        width: size()
        height: size()

        Image {
            id: fieldImage
            source: "media/simple-field.svg"
            anchors.fill: parent
        }

        GridView {
            id: grid
            interactive: false
            anchors.fill: parent
            cellWidth: (width-1) / 7
            cellHeight: (height-1) / 7

            move: Transition {
                NumberAnimation { properties: "x,y"; duration: 300; easing.type: Easing.InCubic }
            }

            model: stateModel
            delegate: Item {
                property var pos: position
                id: field
                width: board.width / 7
                height: board.height / 7

                MouseArea {
                    id: fieldMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    enabled: isField && board.playable;

                    onClicked: {
                        if(board.inPutState && !isOccupied && board.movablePlayerActive)
                            stateModel.put(position);
                        else if(board.inRemoveState && stateModel.canRemove(position) && board.movablePlayerActive)
                            stateModel.remove(position);
                    }
                }

                DropArea {
                    id: dropArea
                    keys: ["ninemensmorris/stone"]
                    anchors.fill: parent
                    enabled: board.playable && board.inMoveState && stateModel.isMoving && stateModel.isValidMoveEnd(position)

                    onDropped: {
                        console.debug("dropped(" + position + ")")
                        stateModel.endMove(position)
                    }
                }

                ListView {
                    id: stoneView
                    interactive: false

                    add: Transition {
                        NumberAnimation { properties: "opacity"; duration: 700; from: 0; to: 1; easing.type: Easing.OutCubic }
                        onRunningChanged: {
                            console.debug("add running[" + board.movedStoneOriginField + "]: " + running)
                            if(!running)
                                board.movedStoneOriginField = null
                        }

                    }
                    remove: Transition {
                        NumberAnimation { properties: "opacity"; duration: 700; from: 1; to: 0; easing.type: Easing.OutCubic }
                        onRunningChanged: {
                            if(!running)
                                board.movedStoneOriginField = null
                        }
                    }


                    model: VisualDataModel {
                        id: visualModel
                        model: stateModel
                        rootIndex: modelIndex(index)


                        delegate: Item {
                            id: stoneEntityContainer
                            width: field.width
                            height: field.height

                            MouseArea {
                                property bool canBeDragged: board.inMoveState && stateModel.currentPlayer === stone && stateModel.isMovablePlayer(stone)
                                id: stoneMouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                enabled: canBeDragged && board.playable;
                                drag.target: stoneEntity
                            }

                            Image {
                                width: field.width
                                height: field.height
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                id: stoneEntity
                                source: board.stoneNameToImage(stone)

                                Drag.keys: ["ninemensmorris/stone"]
                                property bool dragActive: stoneMouseArea.drag.active
                                Drag.hotSpot.y: height / 2
                                Drag.hotSpot.x: width / 2
                                Drag.dragType: Drag.Automatic

                                onDragActiveChanged: {
                                    if(dragActive) {
                                        board.movedStoneOriginField = field
                                        console.debug("drag started(" + field.pos + ")")
                                        Drag.start();
                                        stateModel.startMove(field.pos);
                                    }else{
                                        console.debug("drag ended(" + field.pos + ")")
                                        Drag.drop();
                                        stateModel.abortMove();
                                    }
                                }

                                states: [
                                    State {
                                        name: "isDragged"
                                        when: stoneEntity.dragActive
                                        ParentChange {
                                            target: stoneEntity;
                                            parent: board
                                        }
                                        AnchorChanges {
                                            target: stoneEntity;
                                            anchors.horizontalCenter: undefined
                                            anchors.verticalCenter: undefined
                                        }
                                    },
                                    State {
                                        name: "canBeRemoved"
                                        when: board.inRemoveState && !stateModel.canRemove(field.pos)
                                        PropertyChanges {
                                            target: stoneEntity
                                            opacity: 0.6
                                        }
                                    },
                                    State {
                                        name: "canBeRemovedHovered"
                                        when: board.inRemoveState && stateModel.canRemove(field.pos) && stoneMouseArea.containsMouse
                                        PropertyChanges {
                                            target: stoneEntity
                                            source: board.stoneNamToImage("remove")
                                        }
                                    }

                                ]
                            }
                        }
                    }
                }

                Image {
                    id: fieldEntity
                    source: ""
                    visible: isField
                    anchors.fill: parent

                    states: [
                        State {
                            name: "hoverPossiblePut"
                            when: board.inPutState && stateModel.isMovablePlayer(stateModel.currentPlayer) && !isOccupied && fieldMouseArea.containsMouse
                            PropertyChanges {
                                target: fieldEntity
                                source: board.stoneNameToImage(stateModel.currentPlayer)
                                opacity: 0.6
                            }
                        },

                        State {
                            name: "draggedOverPossibleMove"
                            when: board.inMoveState && dropArea.containsDrag && stateModel.isMoving && stateModel.isValidMoveEnd(position)
                            PropertyChanges {
                                target: fieldEntity
                                source: board.stoneNameToImage(stateModel.currentPlayer)
                                opacity: 0.6
                            }
                        },
                        State {
                            name: "possibleTargetForMove"
                            when: board.inMoveState && stateModel.isMoving && stateModel.isValidMoveEnd(position)
                            PropertyChanges {
                                target: fieldEntity
                                source: board.stoneNameToImage(stateModel.currentPlayer)
                                opacity: 0.4
                            }
                        }
                    ]
                }
            }
        }
    }
}
