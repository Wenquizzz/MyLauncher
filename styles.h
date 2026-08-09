#ifndef STYLES_H
#define STYLES_H

#include <QString>

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
    background: transparent;
    font-size: 16px;
    border: none;
    margin: 8px;
}
)";

const QString MAIN_WINDOW_STYLE = R"(
QMainWindow {
    background-color: #FFFAFA; /* Основной темный фон */
    font-family: "Segoe UI", "Inter", sans-serif;
}
)";

const QString MENU_CARD_STYLE = R"(

QPushButton {
    font-size: 14px;
    background-color: #D3D3D3;
    border: 1px solid #808080;
    border-radius: 8px;
}

QPushButton:hover {
    background-color: #A9A9A9;
    border: 1px solid #2F4F4F;
}
)";

#endif // STYLES_H