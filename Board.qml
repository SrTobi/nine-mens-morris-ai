import QtQuick 2.5
import QtQml.Models 2.2

Rectangle {
    id: boardRect
    color: "#c18a34"

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
                    enabled: isField

                    onClicked: {
                        if(board.inPutState && !isOccupied)
                            stateModel.put(position);
                        else if(board.inRemoveState && stateModel.canRemove(position))
                            stateModel.remove(position);
                    }
                }

                DropArea {
                    id: dropArea
                    keys: ["ninemensmorris/stone"]
                    anchors.fill: parent
                    enabled: board.inMoveState && stateModel.isMoving && stateModel.isValidMoveEnd(position)

                    onDropped: {
                        console.debug("dropped(" + position + ")")
                        stateModel.endMove(position)
                    }
                }

                ListView {
                    id: stoneView
                    interactive: false

                    add: Transition {
                        //id: addTrans
                        //NumberAnimation { properties: "opacity"; duration: 200; from: 0; to: 1; easing.type: Easing.OutCubic }
                        /*enabled: board.movedStoneOriginField != null
                        property var origin: board.movedStoneOriginField == null? Qt.point(0, 0) : stoneView.mapFromItem(board.movedStoneOriginField, 0, 0)
                        NumberAnimation { properties: "y"; duration: 2000; from: addTrans.origin.y; easing.type: Easing.InCubic }
                        NumberAnimation { properties: "x"; duration: 2000; from: addTrans.origin.x; easing.type: Easing.InCubic }
                        onRunningChanged: {
                            console.debug("running[" + origin + "]: " + running)
                            if(!running)
                                board.movedStoneOriginField = null
                        }*/

                    }
                    remove: Transition {
                        //NumberAnimation { properties: "opacity"; duration: 200; from: 1; to: 0; easing.type: Easing.OutCubic }
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
                                property bool canBeDragged: board.inMoveState && stateModel.currentPlayer == stone && stateModel.isMovablePlayer(stone)
                                id: stoneMouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                enabled: canBeDragged
                                drag.target: stoneEntity
                            }

                            Image {
                                width: field.width
                                height: field.height
                                //anchors.centerIn: parent
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                id: stoneEntity
                                source: board.stoneNameToImage(stone)

                                Drag.keys: ["ninemensmorris/stone"]
                                property bool dragActive: stoneMouseArea.drag.active
                                //Drag.active: stoneMouseArea.drag.active
                                //Drag.source: stoneView
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

                                Drag.onDragStarted: {
                                    /*board.movedStoneOriginField = field
                                    //console.debug("item from " + board.movedStoneOrigin)
                                    if(!board.inMoveState) {
                                        console.debug("cancel!");
                                        Drag.cancel();
                                    }else
                                        stateModel.startMove(field.pos);*/
                                }
                                //Drop.onDrag

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
                            when: board.inPutState && !isOccupied && fieldMouseArea.containsMouse
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

                        /*,
                        State {
                            name: "placedStone"
                            when: isOccupied
                            PropertyChanges {
                                target: stoneEntity
                                source: stoneNameToImage(stone)
                            }
                        }*/
                    ]
                }
            }
        }
    }
}
