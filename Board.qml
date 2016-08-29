import QtQuick 2.0

Rectangle {
    id: boardRect
    color: "#c18a34"

    anchors.fill: parent

    Rectangle {
        function size() {
            return Math.min(parent.width, parent.height);
        }

        id: playRect
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
                        drag.target: (boardModel.isField(position) && isOccupied ? painter : undefined)
                    }

                    DropArea {
                        id: dropArea
                        keys: ["text/plain"]
                        anchors.fill: parent

                        onDropped: {
                            console.debug("dropped on " + position)
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

                        id: painter
                        source: stoneNameToImage(stone)
                        visible: isOccupied
                        anchors.fill: parent

                        property bool dragActive: mouseArea.drag.active
                        Drag.mimeData: { "text/plain": "bla" }
                        Drag.active: dragActive
                        Drag.hotSpot.y: 0
                        Drag.hotSpot.x: 0
                        Drag.dragType: Drag.Automatic

                        Drag.onDragStarted: {
                            console.debug("Start drag from " + position);

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
                            State {
                                name: "mouseHover"
                                when: mouseArea.containsMouse || mouseArea.drag.active
                                PropertyChanges {
                                    target: painter
                                    //color: mouseArea.drag.active? "green": (dropArea.containsDrag? "red" : "grey")
                                }
                            },
                            State {
                                name: "draggedOver"
                                when: dropArea.containsDrag && !isOccupied && stateModel.isMoving && stateModel.isValidMoveEnd(position)
                                PropertyChanges {
                                    target: painter
                                    //color: "yellow"
                                    visible: true
                                }
                            },
                            State {
                                name: "possibleTarget"
                                when: !isOccupied && stateModel.isMoving && stateModel.isValidMoveEnd(position)
                                PropertyChanges {
                                    target: painter
                                    source: stoneNameToImage("neutral")
                                    opacity: 0.5
                                    visible: true
                                }
                            }
                        ]
                    }
                }
            }
        }
    }
}
