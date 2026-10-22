#ifndef STYLES_H
#define STYLES_H

#include <QString>

const QString MAIN_WINDOW_STYLE = R"(
    QMainWindow {
        background-color: #FFFFFF;
        font-family: "Segoe UI", "Inter", sans-serif;
    }
)";

const QString GAME_CARD_STYLE = R"(

.Gamecard {
    background-color: #D3D3D3;
    border: 1px solid transparent;
    border-radius: 8px;
}

.Gamecard:hover {
    background-color: #A9A9A9;
    border: 1px solid #2F4F4F;
}

QLabel {
    color: #000000;
    background: transparent;
    font-size: 16px;
    border: none;
    margin: 8px;
}
)";

const QString MENU_CARD_STYLE = R"(

QPushButton {
    font-size: 14px;
    color: #000000;
    background-color: #D3D3D3;
    border: 1px solid #808080;
    border-radius: 8px;
}

QPushButton:hover {
    background-color: #A9A9A9;
    border: 1px solid #2F4F4F;
}
)";
const QString ADD_CARD_STYLE = R"(

QPushButton {
    font-size: 40px;
    background-color: #D3D3D3;
    border: 1px solid #808080;
    border-radius: 24px;
}

QPushButton:hover {
    background-color: #A9A9A9;
    border: 1px solid #2F4F4F;
}
)";

const QString CHECKBOX_STYLE = R"(
QCheckBox {
    font-size: 20px;
    color: #2F4F4F;
    spacing: 8px;
}

QCheckBox::indicator {
    width: 28px;
    height: 28px;
    background-color: #D3D3D3;
    border: 1px solid #808080;
    border-radius: 15px;
}

QCheckBox::indicator:hover {
    background-color: #A9A9A9;
    border: 1px solid #2F4F4F;
}

QCheckBox::indicator:checked {
    background-color: #2F4F4F;
    border: 1px solid #2F4F4F;
}

QCheckBox::indicator:checked:hover {
    background-color: #1F3333;
}
)";
const QString QTAB_STYLE = R"(
QTabBar::tab {
    border-top: none;
    background-color: #FFFFFF;
    color: #000000;
    border: 1px solid black;
    padding: 12px 40px;
    font-size: 16px;
    font-weight: bold;
}

QTabBar::tab:selected {
    background-color: #FFFFFF;
    border-bottom: 1px solid #FFFFFF;
}

QTabWidget::pane {
    border: 1px solid black;
    top: -1px;
    background-color: #FFFFFF;
}
)";
#endif // STYLES_H