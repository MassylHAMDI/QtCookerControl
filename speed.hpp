#pragma once

#include <QWidget>

class SpeedGauge : public QWidget {
    Q_OBJECT

public:
    explicit SpeedGauge(QWidget *parent = nullptr);
    void setSpeed(int value);
    int speed() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_speed;
    
};


