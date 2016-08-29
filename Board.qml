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
                model: stateModel

                delegate: Item {
                    id: field
                    width: parent.width / parent.columns
                    height: parent.height / parent.rows

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        enabled: board.inMoveState && stateModel.isMovablePlayer(stone) && stone === stateModel.currentPlayer
                        drag.target: stoneEntity
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
                        anchors.fill: parent

                        property bool dragActive: mouseArea.drag.active
                        Drag.mimeData: { "ninemensmorris/stone": stone}
                        Drag.active: dragActive
                        Drag.hotSpot.y: 0
                        Drag.hotSpot.x: 0
                        Drag.dragType: Drag.Automatic

                        Drag.onDragStarted: {
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
