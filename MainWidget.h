#pragma once

#include <QtWidgets/QWidget>

class DreamConcentrateWidget;
class DreamCalendarWidget;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:

    DreamConcentrateWidget* concentrateWidget_;
    DreamCalendarWidget* calendarWidget_;
};
