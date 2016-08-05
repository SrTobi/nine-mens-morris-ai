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
                model: myModel

                delegate: Item {
                    width: parent.width / parent.columns
                    height: parent.height / parent.rows

                    Rectangle {
                        id: painter
                        color: stone
                        visible: isOccupied
                        anchors.fill: parent
                        Text {
                            text: "" + position
                        }

                        /*MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            drag.target: grid
                        }

                        states: [
                            State {
                                name: "hover"
                                when: mouseArea.containsMouse
                                PropertyChanges {
                                    target: painter
                                    color: "grey"
                                }
                            }
                        ]*/
                    }
                }
            }
        }
    }
}
