#include "speed.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>

SpeedGauge::SpeedGauge(QWidget *parent)
    : QWidget(parent), m_speed(5), m_selected(false) {}

void SpeedGauge::setSpeed(int value) {
    m_speed = qBound(0, value, 8);
    update();
}

int SpeedGauge::speed() const {
    return m_speed;
}

void SpeedGauge::setSelected(bool selected) {
    m_selected = selected;
    update();
}

bool SpeedGauge::isSelected() const {
    return m_selected;
}

void SpeedGauge::mousePressEvent(QMouseEvent *) {
    emit clicked();
}

void SpeedGauge::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QRect r = rect();

    // Définir la zone circulaire centrale
    int diameter = qMin(width(), height()) - 40;
    QPoint center = r.center();
    QRectF circleRect(center.x() - diameter / 2, center.y() - diameter / 2, diameter, diameter);

    // ✅ Ombre portée discrète
    QColor shadowColor(0, 0, 0, 40);
    p.setBrush(shadowColor);
    p.setPen(Qt::NoPen);
    p.drawEllipse(circleRect.adjusted(3, 3, 3, 3));

    // ✅ Fond blanc semi-transparent (effet "carte flottante")
    QColor foreground(255, 255, 255, 210);
    p.setBrush(foreground);
    p.setPen(Qt::NoPen);
    p.drawEllipse(circleRect);

    // ✅ Dessin des 8 cadrans (segments ronds)
    int segmentCount = 8;
    double startAngle = 180.0;
    double angleStep = 180.0 / (segmentCount - 1);
    int radius = diameter / 2 - 20;
    int circleRadius = 10;

    for (int i = 0; i < segmentCount; ++i) {
        double angleDeg = startAngle - i * angleStep;
        double angleRad = qDegreesToRadians(angleDeg);

        int x = center.x() + radius * qCos(angleRad);
        int y = center.y() - radius * qSin(angleRad);

        QRectF dot(x - circleRadius, y - circleRadius, 2 * circleRadius, 2 * circleRadius);
        p.setBrush(i < m_speed ? QColor("#00FF00") : QColor("#cccccc"));
        p.setPen(Qt::NoPen);
        p.drawEllipse(dot);
    }

    // ✅ Texte central - style LCD
    p.setPen(Qt::black);
    QFont lcdFont("DS-Digital", 48, QFont::Normal);
    if (!lcdFont.exactMatch())
        lcdFont = QFont("Courier", 48, QFont::Bold);
    p.setFont(lcdFont);
    p.drawText(circleRect, Qt::AlignCenter, QString("%1").arg(m_speed));

    // ✅ Halo vert si sélectionné
    if (m_selected) {
        QColor greenGlow(0, 200, 0, 120);
        QPen pen(greenGlow, 6);
        p.setBrush(Qt::NoBrush);
        p.setPen(pen);
        p.drawEllipse(circleRect.adjusted(-6, -6, 6, 6));
    }
}
