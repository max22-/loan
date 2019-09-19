#include "timeslider.h"
#include <QTime>

TimeSlider::TimeSlider(QWidget *parent) : QWidget(parent)
{
    slider.setMinimum(0);
    slider.setMaximum(100);
    slider.setSingleStep(1);

    updateLabel(0);

    layout.addWidget(&slider);
    layout.addWidget(&label);
    setLayout(&layout);
}

void TimeSlider::setMaxTime(int ms) {
    maxTime = ms;
}

void TimeSlider::setTime(int ms) {
    int percent = ms * 100 / maxTime;
    slider.setValue(percent);
    updateLabel(ms);
}

void TimeSlider::updateLabel(int ms) {
    QTime time = QTime(0, 0, 0, 0).addMSecs(ms);
    label.setText(time.toString("mm:ss.zzz"));
}
