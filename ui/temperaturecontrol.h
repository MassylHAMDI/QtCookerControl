#ifndef TEMPERATURECONTROL_H
#define TEMPERATURECONTROL_H

#include <QWidget>
#include <QLCDNumber>

class TemperatureControl : public QWidget {
    Q_OBJECT

public:
    explicit TemperatureControl(QWidget *parent = nullptr);
    int temperature() const;
    void setTemperature(int value);

    void setSelected(bool selected);
    bool isSelected() const;

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    int m_temperature;
    bool m_selected;
    QLCDNumber *m_lcdDisplay; // Ajout d'un affichage numérique
};

#endif // TEMPERATURECONTROL_H
