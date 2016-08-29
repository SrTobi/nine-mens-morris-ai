import QtQuick 2.0

Rectangle {
    id: boardRect
    color: "#c18a34"

    anchors.fill: parent

    Rectangle {
        function size() {
            return Math.min(parent.width, parent.height);
        }

        property bool inMoveState: stateModel.phase === "move";
        property bool inPutState: stateModel.phase === "put";
        property bool inRemoveState: stateModel.phase === "remove";

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

        Grid {
            id: grid
            anchors.fill: parent
            rows: 7
            columns: 7

            move: Transition {
                NumberAnimation { properties: "x,y"; duration: 300; easing.type: Easing.InCubic }
            }

            Repeater {
                id: rep
                model: stateModel

                delegate: Item {
                    id: field
                    width: parent.width / parent.columns
                    height: parent.height / parent.rows

                    MouseArea {
                        property bool canBeDragged: isOccupied && stateModel.isMovablePlayer(stone) && stone === stateModel.currentPlayer
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        drag.threshold: canBeDragged? 1 : 1000000
                        enabled: isField
                        drag.target: stoneEntity

                        onClicked: {
                            if(board.inPutState && !isOccupied)
                                stateModel.put(position);
                            else if(board.inRemoveState && isOccupied)
                                stateModel.remove(position);
                        }
                    }

                    DropArea {
                        id: dropArea
                        keys: ["ninemensmorris/stone"]
                        anchors.fill: parent

                        onDropped: {
                            stateModel.endMove(position)
                        }
                    }

                    Image {
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

                        id: stoneEntity
                        source: ""
                        visible: isField
                        anchors.fill: parent


                        //property bool dragActive:

                        Drag.mimeData: { "ninemensmorris/stone": stone}
                        Drag.active: mouseArea.drag.active
                        Drag.hotSpot.y: 0
                        Drag.hotSpot.x: 0
                        Drag.dragType: Drag.Automatic

                        Drag.onDragStarted: {
                            console.debug("drag started")
                            if(!board.inMoveState) {
                                console.debug("cancel!");
                                Drag.cancel();
                            }else
                                stateModel.startMove(position);
                        }

                        states: [

                            /*State {
                                name: "isDragged"
                                when: painter.dragActive
                                AnchorChanges {
                                    target: painter;
                                    anchors.right: undefined;
                                    anchors.top: undefined;
                                    anchors.left: undefined;
                                    anchors.bottom: undefined
                                }
                                ParentChange {
                                    target: painter;
                                    parent: playRect
                                }
                            },*/
                            /*State {
                                name: "mouseOver"
                                when: mouseArea.containsMouse || mouseArea.drag.active
                                PropertyChanges {
                                    target: stoneEntity
                                    //color: mouseArea.drag.active? "green": (dropArea.containsDrag? "red" : "grey")
                                }
                            },*/
                            State {
                                name: "hoverPossiblePut"
                                when: board.inPutState && !isOccupied && mouseArea.containsMouse
                                PropertyChanges {
                                    target: stoneEntity
                                    source: stoneNameToImage("neutral")
                                }
                            },

                            State {
                                name: "draggedOverPossibleMove"
                                when: board.inMoveState && dropArea.containsDrag && stateModel.isMoving && stateModel.isValidMoveEnd(position)
                                PropertyChanges {
                                    target: stoneEntity
                                    source: stoneNameToImage("neutral")
                                }
                            },
                            State {
                                name: "possibleTargetForMove"
                                when: board.inMoveState && stateModel.isMoving && stateModel.isValidMoveEnd(position)
                                PropertyChanges {
                                    target: stoneEntity
                                    source: stoneNameToImage("neutral")
                                    opacity: 0.5
                                }
                            },
                            State {
                                name: "placedStone"
                                when: isOccupied
                                PropertyChanges {
                                    target: stoneEntity
                                    source: stoneNameToImage(stone)
                                }
                            }
                        ]
                    }
                }
            }
        }
    }
}
