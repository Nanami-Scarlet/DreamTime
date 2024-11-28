#pragma once

#include <QLabel>

enum class DayProperty {
	NOT_IN_THIS_MONTH,
	FININSH,
	IDLE
};

class DreamDay  : public QLabel
{
	Q_OBJECT

public:
	DreamDay(QWidget *parent);
	~DreamDay();

public:
	void setDay(int day);
	void setDayPerperty(DayProperty perperty);

private:
	QLabel* laDay_;
};
