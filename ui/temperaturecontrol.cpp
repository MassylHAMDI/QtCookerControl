#include "temperaturecontrol.h"
#include <QPainter>
#include <QMouseEvent>
#include <QLCDNumber>

TemperatureControl::TemperatureControl(QWidget *parent)
    : QWidget(parent), m_temperature(30), m_selected(false) {

    }

int TemperatureControl::temperature() const {
    return m_temperature;
}

void TemperatureControl::setTemperature(int value) {
    m_temperature = qBound(30, value, 120);
    update();
}

void TemperatureControl::setSelected(bool selected) {
    m_selected = selected;
    update();
}

bool TemperatureControl::isSelected() const {
    return m_selected;
}

void TemperatureControl::mousePressEvent(QMouseEvent *) {
    emit clicked();
}

void TemperatureControl::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QRect r = rect();

    // Définir le cercle central
    int diameter = qMin(width(), height()) - 40;
    QPoint center = r.center();
    QRectF circleRect(center.x() - diameter / 2, center.y() - diameter / 2, diameter, diameter);

    // ✅ Ombre portée douce (simulée avec un halo gris clair)
    QColor shadowColor(0, 0, 0, 40);  // noir très transparent
    p.setBrush(shadowColor);
    p.setPen(Qt::NoPen);
    p.drawEllipse(circleRect.adjusted(3, 3, 3, 3));

    // ✅ Fond du cadran (blanc semi-transparent, type "frosted glass")
    QColor foreground(255, 255, 255, 210);
    p.setBrush(foreground);
    p.setPen(Qt::NoPen);
    p.drawEllipse(circleRect);

    // ✅ Texte LCD stylisé
    p.setPen(Qt::black);
    QFont lcdFont("DS-Digital", 48, QFont::Normal);
    if (!lcdFont.exactMatch())
        lcdFont = QFont("Courier", 48, QFont::Bold);
    p.setFont(lcdFont);
    p.drawText(circleRect, Qt::AlignCenter, QString("%1°C").arg(m_temperature));

    // ✅ Halo de sélection discret et moderne
    if (m_selected) {
        QColor greenGlow(0, 200, 0, 120);  // vert doux
        QPen pen(greenGlow, 6);
        p.setBrush(Qt::NoBrush);
        p.setPen(pen);
        p.drawEllipse(circleRect.adjusted(-6, -6, 6, 6));
    }
}

