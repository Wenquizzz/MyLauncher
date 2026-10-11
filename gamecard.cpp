#include "gamecard.h"
#include "styles.h"
#include <QProcess>

Gamecard::Gamecard(int id, const QString &name, const QString &exePath, const QIcon &icon, QWidget *parent)
    : QWidget(parent)
{
    m_exePath = exePath;
    m_id = id;
    setFixedSize(150, 210);
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet(GAME_CARD_STYLE);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *iconLabel = new QLabel(this);
    iconLabel->setPixmap(icon.pixmap(100, 100));
    iconLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(iconLabel);

    QLabel *nameLabel = new QLabel(name, this);
    nameLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(nameLabel);

    QPushButton *delete_button = new QPushButton("X", this);
    delete_button -> setGeometry(120, 10, 20, 20);
    connect(delete_button, &QPushButton::clicked, this, &Gamecard::onDeleteClicked);

    setCursor(Qt::PointingHandCursor);
}

void Gamecard::mouseDoubleClickEvent(QMouseEvent *event) {
    QProcess::startDetached (m_exePath);
}


void Gamecard::onDeleteClicked() {
    emit removeRequested(m_id);
}



