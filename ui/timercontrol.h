#ifndef TIMERCONTROL_H
#define TIMERCONTROL_H

#include <QWidget>
#include <QTimer>

class TimerControl : public QWidget {
    Q_OBJECT

    public:
    explicit TimerControl(QWidget *parent = nullptr);
    int seconds() const;
    void setSeconds(int value);  // fixe le temps restant
    bool isRunning() const;

    void startCountdown();
    void stopCountdown();

    void setSelected(bool selected);
    bool isSelected() const;

signals:
    void clicked();
    void finished();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void onTimeout();

private:
    int m_seconds;
    bool m_selected;
    bool m_running = false;
    int m_initial = 0;
    QTimer m_timer;
};

#endif // TIMERCONTROL_H
