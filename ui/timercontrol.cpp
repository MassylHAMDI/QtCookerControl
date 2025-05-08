#include "timercontrol.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>

TimerControl::TimerControl(QWidget *parent)
    : QWidget(parent), m_seconds(180), m_selected(false), m_running(false), m_initial(0) {
    m_timer.setInterval(1000);
    connect(&m_timer, &QTimer::timeout, this, &TimerControl::onTimeout);
}

int TimerControl::seconds() const {
    return m_seconds;
}

bool TimerControl::isRunning() const {
    return m_running;
}

void TimerControl::setSeconds(int value) {
    m_seconds = qBound(0, value, 3600);
    if (!m_running)
        m_initial = m_seconds;  // mémorise la durée initiale
    update();
}

void TimerControl::setSelected(bool selected) {
    m_selected = selected;
    update();
}

void TimerControl::startCountdown() {
    if (m_seconds <= 0) return;
    m_running = true;
    m_timer.start();
}

void TimerControl::stopCountdown() {
    m_running = false;
    m_timer.stop();
}

void TimerControl::onTimeout() {
    if (m_seconds > 0) {
        m_seconds--;
        update();
        if (m_seconds == 0) {
            stopCountdown();
            emit finished();  // signal quand terminé
        }
    }
}



bool TimerControl::isSelected() const {
    return m_selected;
}

void TimerControl::mousePressEvent(QMouseEvent *) {
    if (!isEnabled()) return;
    emit clicked();
}

void TimerControl::paintEvent(QPaintEvent *) {
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

    // Format mm:ss
    int mins = m_seconds / 60;
    int secs = m_seconds % 60;
    QString timeStr = QString("%1:%2")
                          .arg(mins)
                          .arg(secs, 2, 10, QChar('0'));

    QFont font("DS-Digital", 36, QFont::Normal);
    if (!font.exactMatch())
        font = QFont("Courier", 36, QFont::Bold);

    p.setPen(Qt::black);
    p.setFont(font);
    p.drawText(circleRect, Qt::AlignCenter, timeStr);

    if (m_selected) {
        p.setPen(QPen(QColor(0, 255, 0, 120), 6));
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(circleRect.adjusted(-6, -6, 6, 6));
    }
}

