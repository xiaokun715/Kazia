import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    width: 1200
    height: 36

    // 关键：透明背景
    Rectangle {
        anchors.fill: parent
        color: "transparent"
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 6
        spacing: 8

        // ========= 左侧 =========
        ToolButton { text: "☰" }
        ToolButton { text: "📦" }

        ComboBox {
            model: ["Perspective", "Top", "Front", "Side"]
            currentIndex: 0
        }

        ComboBox {
            model: ["Lit", "Unlit", "Wireframe"]
            currentIndex: 0
        }

        ComboBox {
            model: ["Show", "Hide"]
            currentIndex: 0
        }

        // 中间弹性空白
        Item {
            Layout.fillWidth: true
        }

        // ========= 右侧 =========
        Repeater {
            model: [
                { text: "⭮", active: true },
                { text: "✥", active: true },
                { text: "⌖", active: false },
                { text: "🌐", active: false }
            ]

            delegate: ToolButton {
                text: modelData.text
                background: Rectangle {
                    radius: height / 2
                    color: modelData.active ? "#2f80ff" : "#3a3a3a"
                }
            }
        }

        // 数值按钮
        ValueButton { label: "10" }
        ValueButton { label: "10°" }
        ValueButton { label: "0.25" }
        ValueButton { label: "1" }
    }

    // ========= 自定义控件 =========

    component ToolButton: Button {
        implicitHeight: 24
        implicitWidth: 32

        background: Rectangle {
            radius: 6
            color: "#3a3a3a"
        }

        contentItem: Text {
            text: parent.text
            color: "white"
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    component ValueButton: Button {
        property string label

        text: label
        implicitHeight: 24
        implicitWidth: 40

        background: Rectangle {
            radius: 6
            color: "#2f80ff"
        }

        contentItem: Text {
            text: parent.text
            color: "white"
            font.pixelSize: 13
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
