#pragma once

#include <QWidget>
#include <QDate>

class DreamDay;
class QLabel;

class DreamCalendarWidget  : public QWidget
{
	Q_OBJECT

public:
	DreamCalendarWidget(QWidget *parent);
	~DreamCalendarWidget();

	// 每天更新使用
	void updateCalendar();
	void updateDayGrid();

protected:
	void resizeEvent(QResizeEvent*event) override;

private:
	void initDayGrid();
	int msecsToMidnight();

private:
	QLabel* lbBG_;
	QLabel* lbDescription_;
	QList<DreamDay*> dayGrids_;

	QDate today_;

	int calendarStartX_;
	int calendarStartY_;
};
