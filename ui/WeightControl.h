#ifndef WEIGHTCONTROL_H
#define WEIGHTCONTROL_H

#include <QWidget>

class WeightControl : public QWidget {
    Q_OBJECT

public:
    explicit WeightControl(QWidget *parent = nullptr);
    void setWeight(float grams);
    float weight() const;
    void setSelected(bool selected);
    bool isSelected() const;

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    float m_weight;
    bool m_selected;
};

#endif // WEIGHTCONTROL_H
