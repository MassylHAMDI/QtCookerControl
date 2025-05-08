#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "speed.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Fenêtre principale
    QWidget window;
    window.setWindowTitle("Jauge de vitesse");
    window.resize(400, 700);

    // Jauge personnalisée
    SpeedGauge* gauge = new SpeedGauge;

    // Boutons
    QPushButton* plusBtn = new QPushButton("+");
    QPushButton* minusBtn = new QPushButton("–");

    // Style
    plusBtn->setFixedHeight(40);
    minusBtn->setFixedHeight(40);

    // Layout vertical
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(gauge, 1);       // jauge
    layout->addWidget(plusBtn);        // bouton +
    layout->addWidget(minusBtn);       // bouton -
    layout->setSpacing(10);

    window.setLayout(layout);

    // Connexions
    QObject::connect(plusBtn, &QPushButton::clicked, [=]() {
        int speed = gauge->speed();    // fonction à ajouter (voir ci-dessous)
        if (speed < 8)
            gauge->setSpeed(speed + 1);
    });

    QObject::connect(minusBtn, &QPushButton::clicked, [=]() {
        int speed = gauge->speed();
        if (speed > 0)
            gauge->setSpeed(speed - 1);
    });

    window.show();
    return app.exec();
}
