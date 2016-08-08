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

        Grid {
            id: grid
            anchors.fill: parent
            rows: 7
            columns: 7

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
                        drag.target: painter
                    }

                    DropArea {
                        id: dropArea
                        keys: ["text/plain"]
                        anchors.fill: parent

                        onDropped: {
                        }
                    }

                    Rectangle {
                        id: painter
                        color: stone
                        visible: isOccupied
                        anchors.fill: parent

                        property bool dragActive: mouseArea.drag.active
                        Drag.mimeData: { "text/plain": "bla" }
                        Drag.active: mouseArea.drag.active
                        Drag.hotSpot.y: 0
                        Drag.hotSpot.x: 0
                        Drag.dragType: Drag.Automatic

                        Drag.onDragStarted: {
                            console.debug("Start drag from " + position);
                            stateModel.startMove(position);
                        }

                        Drag.onDragFinished: {
                            console.debug("Finish drag on " + position);
                        }


                        states: [

                            State {
                                name: "isDragged"
                                when: painter.dragActive
                                ParentChange { target: painter; parent: playRect }
                                //AnchorChanges { target: painter; undefined; }
                            },
                            State {
                                name: "mouseHover"
                                when: mouseArea.containsMouse || mouseArea.drag.active
                                PropertyChanges {
                                    target: painter
                                    color: mouseArea.drag.active? "green": (dropArea.containsDrag? "red" : "grey")
                                }
                            },
                            State {
                                name: "draggedOver"
                                when: dropArea.containsDrag && !isOccupied && stateModel.isMoving && stateModel.isValidMoveEnd(position)
                                PropertyChanges {
                                    target: painter
                                    color: "yellow"
                                    visible: true
                                }
                            },
                            State {
                                name: "possibleTarget"
                                when: !isOccupied && stateModel.isMoving && stateModel.isValidMoveEnd(position)
                                PropertyChanges {
                                    target: painter
                                    color: "blue"
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
