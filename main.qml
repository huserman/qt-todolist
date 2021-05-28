import QtQuick 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls.Universal 2.12

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Todo List")

    Universal.theme: Universal.Light
    Universal.accent: Universal.Violet

    TodoList {
        anchors.centerIn: parent
    }
}
