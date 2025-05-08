#ifndef SPEED_H
#define SPEED_H

#include <QWidget>

class SpeedGauge : public QWidget {
    Q_OBJECT

public:
    explicit SpeedGauge(QWidget *parent = nullptr);
    void setSpeed(int value);
    int speed() const;

    void setSelected(bool selected);
    bool isSelected() const;

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    int m_speed;
    bool m_selected;
};

#endif // SPEED_H
