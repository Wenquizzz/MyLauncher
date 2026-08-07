#ifndef STYLES_H
#define STYLES_H

#include <QString>

const QString GAME_CARD_STYLE = R"(
    QToolButton {
        padding-top: 25px;
        border: none;
        background: #DCDCDC;
        font-size: 11px;
    }
    QToolButton:hover {
        background: #A9A9A9;
    }
)";

#endif // STYLES_H