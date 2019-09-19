#ifndef TIMESLIDER_H
#define TIMESLIDER_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>

class TimeSlider : public QWidget
{
    Q_OBJECT
public:
    explicit TimeSlider(QWidget *parent = nullptr);

public slots:
    void setMaxTime(int);
    void setTime(int);
    void setOrientation(Qt::Orientation);

private:
    void updateLabel(int);
    QHBoxLayout layout;
    QSlider slider;
    QLabel label;
    int maxTime;
};

#endif // TIMESLIDER_H
