#include "steamselectorwindow.h"
#include "styles.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QCheckBox>
#include <QPushButton>

SteamSelectorWindow::SteamSelectorWindow(const QVector<AppItem> &games, QWidget *parent)
    : QDialog(parent), m_allGames(games)
{
    setWindowTitle("Steam Library Scanner");
    resize(500, 600);
    this->setStyleSheet("QDialog { background-color: white; }");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; background: #FFFFFF; }");

    QWidget *container = new QWidget();
    container->setStyleSheet("background: transparent;");
    QVBoxLayout *containerLayout = new QVBoxLayout(container);
    containerLayout->setSpacing(15);
    containerLayout->setContentsMargins(10, 10, 10, 10);

    QVector<QCheckBox*> checkBoxes;

    for (int i = 0; i < m_allGames.size(); ++i) {
        QCheckBox *checkBox = new QCheckBox(m_allGames[i].name, container);
        checkBox->setStyleSheet(CHECKBOX_STYLE);
        containerLayout->addWidget(checkBox);
        checkBoxes.append(checkBox);
    }

    containerLayout->addStretch();
    scrollArea->setWidget(container);
    mainLayout->addWidget(scrollArea);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *cancelButton = new QPushButton("Cancel", this);
    QPushButton *addButton = new QPushButton("Add Selected", this);

    cancelButton->setStyleSheet(MENU_CARD_STYLE);
    addButton->setStyleSheet(MENU_CARD_STYLE);

    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    connect(addButton, &QPushButton::clicked, this, [this, checkBoxes]() {
        for (int i = 0; i < checkBoxes.size(); ++i) {
            if (checkBoxes[i]->isChecked()) {
                m_selectedGames.append(m_allGames[i]);
            }
        }
        accept();
    });

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(addButton);
    mainLayout->addLayout(buttonLayout);
}

QVector<AppItem> SteamSelectorWindow::getSelectedGames() const {
    return m_selectedGames;
}
