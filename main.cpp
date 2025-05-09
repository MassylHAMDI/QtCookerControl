#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "ui/temperaturecontrol.h"
#include "ui/speed.h"
#include "ui/timercontrol.h"
#include "ui/weightcontrol.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Interface Robot Cuiseur");

    // Widgets principaux
    auto *temp = new TemperatureControl;
    auto *gauge = new SpeedGauge;
    auto *timer = new TimerControl;
    auto *weight = new WeightControl;

    temp->setMinimumSize(150, 150);
    gauge->setMinimumSize(150, 150);
    timer->setMinimumSize(150, 150);
    weight->setMinimumSize(150, 150);
    weight->hide(); // caché par défaut

    // Boutons
    auto *plusBtn = new QPushButton("+");
    auto *minusBtn = new QPushButton("–");
    auto *startBtn = new QPushButton("Démarrer");
    auto *weightBtn = new QPushButton("Pesée");

    plusBtn->setFixedSize(60, 60);
    minusBtn->setFixedSize(60, 60);
    startBtn->setFixedSize(160, 60);
    weightBtn->setFixedSize(160, 60);

    weightBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #2196F3;
            color: white;
            font-size: 20px;
            font-weight: bold;
            padding: 10px 20px;
            border: none;
            border-radius: 12px;
        }
        QPushButton:hover {
            background-color: #1976D2;
        }
        QPushButton:pressed {
            background-color: #0D47A1;
        }
    )");

    plusBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #f0f0f0;
            border: 2px solid #ccc;
            border-radius: 20px;
            font-size: 24px;
            font-weight: bold;
            color: #333;
        }
        QPushButton:hover {
            background-color: #e0e0e0;
        }
    )");
    minusBtn->setStyleSheet(plusBtn->styleSheet());

    enum class Mode { Temperature, Vitesse, Timer, Poids };
    Mode currentMode = Mode::Temperature;

    temp->setSelected(false);
    gauge->setSelected(false);
    timer->setSelected(false);
    weight->setSelected(false);

    auto *topLayout = new QHBoxLayout;
    topLayout->addWidget(temp, 1);
    topLayout->addWidget(gauge, 1);
    topLayout->addWidget(timer, 1);
    topLayout->addWidget(weight, 1);

    auto *controlLayout = new QHBoxLayout;
    controlLayout->addStretch();
    controlLayout->addWidget(minusBtn);
    controlLayout->addWidget(startBtn);
    controlLayout->addWidget(plusBtn);
    controlLayout->addWidget(weightBtn);
    controlLayout->addStretch();

    auto *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout, 1);
    mainLayout->addLayout(controlLayout);
    window.setLayout(mainLayout);
    window.resize(1000, 420);
    window.show();

    bool cooking = false;

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
                QPushButton:hover { background-color: #d32f2f; }
                QPushButton:pressed { background-color: #b71c1c; }
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
                QPushButton:hover { background-color: #45a049; }
                QPushButton:pressed { background-color: #3e8e41; }
            )");
        }
    };

    updateStartButton();

    // Température
    QObject::connect(temp, &TemperatureControl::clicked, [&]() {
        currentMode = Mode::Temperature;
        temp->show();
        gauge->show();
        timer->show();
        weight->hide();

        temp->setSelected(true);
        gauge->setSelected(false);
        timer->setSelected(false);
        weight->setSelected(false);

        plusBtn->setEnabled(true);
        minusBtn->setEnabled(true);
    });

    // Vitesse
    QObject::connect(gauge, &SpeedGauge::clicked, [&]() {
        currentMode = Mode::Vitesse;
        temp->show();
        gauge->show();
        timer->show();
        weight->hide();

        temp->setSelected(false);
        gauge->setSelected(true);
        timer->setSelected(false);
        weight->setSelected(false);

        plusBtn->setEnabled(true);
        minusBtn->setEnabled(true);
    });

    // Timer
    QObject::connect(timer, &TimerControl::clicked, [&]() {
        if (cooking) return;
        currentMode = Mode::Timer;
        temp->show();
        gauge->show();
        timer->show();
        weight->hide();

        temp->setSelected(false);
        gauge->setSelected(false);
        timer->setSelected(true);
        weight->setSelected(false);

        plusBtn->setEnabled(true);
        minusBtn->setEnabled(true);
    });

    // Pesée (via bouton)
    QObject::connect(weightBtn, &QPushButton::clicked, [&]() {
        currentMode = Mode::Poids;
        temp->hide();
        gauge->hide();
        timer->hide();
        weight->show();

        temp->setSelected(false);
        gauge->setSelected(false);
        timer->setSelected(false);
        weight->setSelected(true);

        plusBtn->setEnabled(false);
        minusBtn->setEnabled(false);
    });

    // Clic direct sur le cadran de pesée (optionnel)
    QObject::connect(weight, &WeightControl::clicked, [&]() {
        currentMode = Mode::Poids;
        temp->hide();
        gauge->hide();
        timer->hide();
        weight->show();

        temp->setSelected(false);
        gauge->setSelected(false);
        timer->setSelected(false);
        weight->setSelected(true);

        plusBtn->setEnabled(false);
        minusBtn->setEnabled(false);
    });

    // Boutons +
    QObject::connect(plusBtn, &QPushButton::clicked, [&]() {
        if (currentMode == Mode::Temperature)
            temp->setTemperature(temp->temperature() + 5);
        else if (currentMode == Mode::Vitesse)
            gauge->setSpeed(gauge->speed() + 1);
        else if (currentMode == Mode::Timer)
            timer->setSeconds(timer->seconds() + 30);
    });

    // Boutons –
    QObject::connect(minusBtn, &QPushButton::clicked, [&]() {
        if (currentMode == Mode::Temperature)
            temp->setTemperature(temp->temperature() - 5);
        else if (currentMode == Mode::Vitesse)
            gauge->setSpeed(gauge->speed() - 1);
        else if (currentMode == Mode::Timer)
            timer->setSeconds(timer->seconds() - 30);
    });

    // Start
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
