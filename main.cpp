#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "ui/temperaturecontrol.h"
#include "ui/speed.h"
#include "ui/timercontrol.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Interface Robot Cuiseur");

    // Widgets principaux
    auto *temp = new TemperatureControl;
    auto *gauge = new SpeedGauge;
    auto *timer = new TimerControl;

    temp->setMinimumSize(150, 150);
    gauge->setMinimumSize(150, 150);
    timer->setMinimumSize(150, 150);

    auto *plusBtn = new QPushButton("+");
    auto *minusBtn = new QPushButton("–");
    auto *startBtn = new QPushButton("Démarrer");

    plusBtn->setFixedSize(60, 60);
    minusBtn->setFixedSize(60, 60);
    startBtn->setFixedSize(160, 60);

    // Style boutons +/–
    plusBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #f0f0f0;
            border: 2px solid #ccc;
            border-radius: 20px;
            font-size: 24px;
            font-weight: bold;
            color: #333;
            width: 60px;
            height: 60px;
        }
        QPushButton:hover {
            background-color: #e0e0e0;
        }
    )");
    
    minusBtn->setStyleSheet(plusBtn->styleSheet());

    // Enum pour la sélection du cadran actif
    enum class Mode { Temperature, Vitesse, Timer };
    Mode currentMode = Mode::Temperature;

    // Sélection initiale
    temp->setSelected(false);
    gauge->setSelected(false);
    timer->setSelected(false);

    // Layout des cadrans
    auto *topLayout = new QHBoxLayout;
    topLayout->addWidget(temp, 1);
    topLayout->addWidget(gauge, 1);
    topLayout->addWidget(timer, 1);

    // Layout des contrôles
    auto *controlLayout = new QHBoxLayout;
    controlLayout->addStretch();
    controlLayout->addWidget(minusBtn);
    controlLayout->addWidget(startBtn);
    controlLayout->addWidget(plusBtn);
    controlLayout->addStretch();

    // Layout principal
    auto *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout, 1);
    mainLayout->addLayout(controlLayout);
    window.setLayout(mainLayout);
    window.resize(900, 400);
    window.show();

    // 🔁 État cuisson
    bool cooking = false;

    // 🔁 Fonction pour mettre à jour le bouton Démarrer
    auto updateStartButton = [&]() {
        if (cooking) {
            startBtn->setText("Arrêter");
            startBtn->setStyleSheet(R"(
                QPushButton {
                    background-color: #e53935;
                    color: white;
                    font-size: 20px;
                    font-weight: bold;
                    padding: 10px 20px;
                    border: none;
                    border-radius: 12px;
                }
                QPushButton:hover {
                    background-color: #d32f2f;
                }
                QPushButton:pressed {
                    background-color: #b71c1c;
                }
            )");
        } else {
            startBtn->setText("Démarrer");
            startBtn->setStyleSheet(R"(
                QPushButton {
                    background-color: #4CAF50;
                    color: white;
                    font-size: 20px;
                    font-weight: bold;
                    padding: 10px 20px;
                    border: none;
                    border-radius: 12px;
                }
                QPushButton:hover {
                    background-color: #45a049;
                }
                QPushButton:pressed {
                    background-color: #3e8e41;
                }
            )");
        }
    };

    updateStartButton();

    // 🔘 Connexions : clics sur les cadrans
    QObject::connect(temp, &TemperatureControl::clicked, [&]() {
        currentMode = Mode::Temperature;
        temp->setSelected(true);
        gauge->setSelected(false);
        timer->setSelected(false);
    
        // Réactiver boutons si Timer était sélectionné et verrouillé
        plusBtn->setEnabled(true);
        minusBtn->setEnabled(true);
    });

    QObject::connect(gauge, &SpeedGauge::clicked, [&]() {
        currentMode = Mode::Vitesse;
        temp->setSelected(false);
        gauge->setSelected(true);
        timer->setSelected(false);
    
        plusBtn->setEnabled(true);
        minusBtn->setEnabled(true);
    });

    QObject::connect(timer, &TimerControl::clicked, [&]() {
        if (cooking) return;
        currentMode = Mode::Timer;
        temp->setSelected(false);
        gauge->setSelected(false);
        timer->setSelected(true);
    });

    // 🔘 Boutons +/–
    QObject::connect(plusBtn, &QPushButton::clicked, [&]() {
        if (currentMode == Mode::Temperature)
            temp->setTemperature(temp->temperature() + 5);
        else if (currentMode == Mode::Vitesse)
            gauge->setSpeed(gauge->speed() + 1);
        else
            timer->setSeconds(timer->seconds() + 30);
    });

    QObject::connect(minusBtn, &QPushButton::clicked, [&]() {
        if (currentMode == Mode::Temperature)
            temp->setTemperature(temp->temperature() - 5);
        else if (currentMode == Mode::Vitesse)
            gauge->setSpeed(gauge->speed() - 1);
        else
            timer->setSeconds(timer->seconds() - 30);
    });

    // 🔘 Bouton Démarrer
    QObject::connect(startBtn, &QPushButton::clicked, [&]() {
        cooking = !cooking;
        updateStartButton();
    
       
        temp->setEnabled(true);
        gauge->setEnabled(true);

        
        timer->setEnabled(!cooking);

        
        bool disableButtons = (cooking && currentMode == Mode::Timer);
        plusBtn->setEnabled(!disableButtons);
        minusBtn->setEnabled(!disableButtons);
    
        if (cooking) {
            timer->startCountdown();
        } else {
            timer->stopCountdown();
        }
    });
    
    return app.exec();
}
