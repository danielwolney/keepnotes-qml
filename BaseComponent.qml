import QtQuick 2.7
import QtGraphicalEffects 1.0

Item  {
    id: item
    property color backgroundColor: elevation > 0 ? "white" : "transparent"
    property color tintColor: "transparent"

    property real minimumSize: Math.min(height, width)
    property real radius: 0
    property alias border: rect.border
    property alias color: rect.color
    property int elevation: 0
    property bool fullWidth
    property bool fullHeight
    property bool elevationInverted: false

    property var topShadow: [
        {
            "opacity": 0,
            "offset": 0,
            "blur": 0
        },

        {
            "opacity": 0.12,
            "offset": 1/* * dp*/,
            "blur": 1.5/* * dp*/
        },

        {
            "opacity": 0.16,
            "offset": 3/* * dp*/,
            "blur": 3/* * dp*/
        },

        {
            "opacity": 0.19,
            "offset": 10/* * dp*/,
            "blur": 10/* * dp*/
        },

        {
            "opacity": 0.25,
            "offset": 14/* * dp*/,
            "blur": 14/* * dp*/
        },

        {
            "opacity": 0.30,
            "offset": 19/* * dp*/,
            "blur": 19/* * dp*/
        }
    ]

    property var bottomShadow: [
        {
            "opacity": 0,
            "offset": 0/* * dp*/,
            "blur": 0/* * dp*/
        },

        {
            "opacity": 0.24,
            "offset": 1/* * dp*/,
            "blur": 1/* * dp*/
        },

        {
            "opacity": 0.23,
            "offset": 3/* * dp*/,
            "blur": 3/* * dp*/
        },

        {
            "opacity": 0.23,
            "offset": 6/* * dp*/,
            "blur": 3/* * dp*/
        },

        {
            "opacity": 0.22,
            "offset": 10/* * dp*/,
            "blur": 5/* * dp*/
        },

        {
            "opacity": 0.22,
            "offset": 15/* * dp*/,
            "blur": 6/* * dp*/
        }
    ]

    RectangularGlow {
        property var elevationInfo: bottomShadow[Math.min(elevation, 5)]
        property real horizontalShadowOffset: elevationInfo.offset * Math.sin((2 * Math.PI) * (parent.rotation / 360.0))
        property real verticalShadowOffset: elevationInfo.offset * Math.cos((2 * Math.PI) * (parent.rotation / 360.0))

        anchors.centerIn: parent
        width: parent.width + (fullWidth ? 10/* * dp*/ : 0)
        height: parent.height + (fullHeight ? 20/* * dp*/ : 0)
        anchors.horizontalCenterOffset: horizontalShadowOffset * (elevationInverted ? -1 : 1)
        anchors.verticalCenterOffset: verticalShadowOffset * (elevationInverted ? -1 : 1)
        glowRadius: elevationInfo.blur
        opacity: elevationInfo.opacity
        spread: 0.05
        color: "black"
        cornerRadius: radius + glowRadius * 2.5
        visible: parent.opacity == 1
    }

    RectangularGlow {
        property var elevationInfo: topShadow[Math.min(elevation, 5)]
        property real horizontalShadowOffset: elevationInfo.offset * Math.sin((2 * Math.PI) * (parent.rotation / 360.0))
        property real verticalShadowOffset: elevationInfo.offset * Math.cos((2 * Math.PI) * (parent.rotation / 360.0))

        anchors.centerIn: parent
        width: parent.width + (fullWidth ? 10/* * dp*/ : 0)
        height: parent.height + (fullHeight ? 20/* * dp*/ : 0)
        anchors.horizontalCenterOffset: horizontalShadowOffset * (elevationInverted ? -1 : 1)
        anchors.verticalCenterOffset: verticalShadowOffset * (elevationInverted ? -1 : 1)
        glowRadius: elevationInfo.blur
        opacity: elevationInfo.opacity
        spread: 0.05
        color: "black"
        cornerRadius: radius + glowRadius * 2.5
        visible: parent.opacity == 1
    }
    Rectangle {
        id: rect
        anchors.fill: parent
        color: Qt.tint(backgroundColor, tintColor)
        radius: item.radius
        antialiasing: parent.rotation || radius > 0 ? true : false
        clip: true
        Behavior on color {
            ColorAnimation { duration: 200 }
        }
    }
}
