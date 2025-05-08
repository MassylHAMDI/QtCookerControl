#include "speed.hpp"
#include <QPainter>
#include <QtMath>

SpeedGauge::SpeedGauge(QWidget *parent) : QWidget(parent), m_speed(0) {}

void SpeedGauge::setSpeed(int value) {
    m_speed = qBound(0, value, 8);
    update();
}
int SpeedGauge::speed() const {
    return m_speed;
}

void SpeedGauge::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();
    QPoint center(w / 2, h / 2 + 20);  // Légèrement plus haut
    int outerRadius = qMin(w, h) / 2 - 20;
    int innerRadius = outerRadius - 40;

    int segmentCount = 8;
    double startAngle = 180.0;
    double angleStep = 180.0 / segmentCount;
    int angleDetail = 2;

    for (int i = 0; i < segmentCount; ++i) {
        double angleStart = startAngle - i * angleStep;
        double angleEnd = angleStart - angleStep;

        QPolygonF polygon;

        for (double a = angleStart; a >= angleEnd; a -= angleDetail) {
            double rad = qDegreesToRadians(a);
            QPointF pt(center.x() + outerRadius * qCos(rad),
                       center.y() - outerRadius * qSin(rad));
            polygon << pt;
        }

        for (double a = angleEnd; a <= angleStart; a += angleDetail) {
            double rad = qDegreesToRadians(a);
            QPointF pt(center.x() + innerRadius * qCos(rad),
                       center.y() - innerRadius * qSin(rad));
            polygon << pt;
        }

        QColor color = (i < m_speed) ? QColor("#00FF00") : QColor("#e0e0e0");
        p.setBrush(color);
        p.setPen(Qt::NoPen);
        p.drawPolygon(polygon);
    }

    // Texte central - DEBUG visible
    p.setPen(Qt::black);
    QFont font("Digital-7", 80);
    p.setFont(font);

    int textWidth = 100;
    int textHeight = 140;
    QRect textRect(center.x() - textWidth / 2, center.y() - textHeight / 2 -45, textWidth, textHeight);

    // DEbuginig
    // p.setPen(Qt::red);
    // p.drawRect(textRect);  

    // Affiche le texte
    p.setPen(Qt::black);
    p.drawText(textRect, Qt::AlignCenter, QString("%1").arg(m_speed));
}
