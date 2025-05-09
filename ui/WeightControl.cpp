#include "weightcontrol.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>

WeightControl::WeightControl(QWidget *parent)
    : QWidget(parent), m_weight(0.0f), m_selected(false) {}

void WeightControl::setWeight(float grams) {
    m_weight = qBound(0.0f, grams, 5000.0f);
    update();
}

float WeightControl::weight() const {
    return m_weight;
}

void WeightControl::setSelected(bool selected) {
    m_selected = selected;
    update();
}

bool WeightControl::isSelected() const {
    return m_selected;
}

void WeightControl::mousePressEvent(QMouseEvent *) {
    emit clicked();
}

void WeightControl::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QRect r = rect();

    int diameter = qMin(width(), height()) - 40;
    QPoint center = r.center();
    QRectF circleRect(center.x() - diameter / 2, center.y() - diameter / 2, diameter, diameter);

    // Ombre
    p.setBrush(QColor(0, 0, 0, 40));
    p.setPen(Qt::NoPen);
    p.drawEllipse(circleRect.adjusted(3, 3, 3, 3));

    // Fond
    p.setBrush(QColor(255, 255, 255, 210));
    p.setPen(Qt::NoPen);
    p.drawEllipse(circleRect);

    // Affichage poids
    QFont font("DS-Digital", 36, QFont::Normal);
    if (!font.exactMatch())
        font = QFont("Courier", 36, QFont::Bold);
    p.setPen(Qt::black);
    p.setFont(font);
    p.drawText(circleRect, Qt::AlignCenter, QString("%1 g").arg(m_weight, 0, 'f', 0));

    if (m_selected) {
        p.setPen(QPen(QColor(0, 255, 0, 120), 6));
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(circleRect.adjusted(-6, -6, 6, 6));
    }
}
