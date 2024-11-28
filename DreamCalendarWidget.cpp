#include "DreamCalendarWidget.h"
#include "DreamDay.h"
#include "DataManager.h"

#include <QLabel>
#include <QDebug>
#include <QTimer>
#include <QDateTime>

const int CALENDAR_WIDTH = 621;
const int CALENDAR_HIEGHT = 589;

const int DAY_GRID_LEN = 58;
const int DAY_START_X = 38;
const int DAY_START_Y = 159;
const int DAY_PADDING_X = 23;
const int DAY_PADDING_Y = 15;

DreamCalendarWidget::DreamCalendarWidget(QWidget *parent)
	: QWidget(parent)
{
	lbBG_ = new QLabel(this);
	lbBG_->setPixmap(QPixmap(":/images/images/calendar_bg.png"));

	lbDescription_ = new QLabel(this);
	lbDescription_->setStyleSheet(R"(
		font-family: "Microsoft YaHei";
		font-size: 18px;
		color: #000000;
	)");
	lbDescription_->setText("Tips: 每天专注一小时即可点亮~");

	for (int i = 0; i < 42; ++i) {
		DreamDay* dayGrid = new DreamDay(this);
		dayGrid->setDayPerperty(DayProperty::IDLE);
		dayGrid->setDay(i);
		dayGrid->setHidden(true);
		dayGrids_.append(dayGrid);
	}

	today_ = QDate::currentDate();
	
	QTimer::singleShot(msecsToMidnight(), this, &DreamCalendarWidget::updateCalendar);
}

DreamCalendarWidget::~DreamCalendarWidget()
{
	if (lbBG_) {
		delete lbBG_;
		lbBG_ = nullptr;
	}
}


void DreamCalendarWidget::resizeEvent(QResizeEvent* event)
{
	int widthCenterX = width() / 2;
	int widthCenterY = height() / 2;

	calendarStartX_ = widthCenterX - CALENDAR_WIDTH / 2;
	calendarStartY_ = widthCenterY - CALENDAR_HIEGHT / 2;

	lbBG_->move(calendarStartX_, calendarStartY_);
	lbDescription_->move(0, widthCenterY);

	initDayGrid();
	updateDayGrid();
}

void DreamCalendarWidget::initDayGrid()
{
	int startX = calendarStartX_ + DAY_START_X;
	int startY = calendarStartY_ + DAY_START_Y;
	for (int i = 0; i < dayGrids_.count(); ++i) {
		int row = i / 7;
		int col = i % 7;
		int posX = startX + (DAY_GRID_LEN + DAY_PADDING_X) * col;
		int posY = startY + (DAY_GRID_LEN + DAY_PADDING_Y) * row;

		dayGrids_[i]->move(posX, posY);
	}
}

void DreamCalendarWidget::updateDayGrid()
{
	QDate firstDayofMonth { today_.year(), today_.month(), 1 };
	QDate lastDayofMonth = firstDayofMonth.addDays(firstDayofMonth.daysInMonth() - 1);
	int index = firstDayofMonth.dayOfWeek();

	QDate DayofLastMonth = firstDayofMonth.addDays(-1);
	for (int lastMonthIndex = index; lastMonthIndex >= 0; --lastMonthIndex, DayofLastMonth = DayofLastMonth.addDays(-1)) {
		DreamDay* dayGrid = dayGrids_[lastMonthIndex];
		dayGrid->setHidden(false);
		dayGrid->setDayPerperty(DayProperty::NOT_IN_THIS_MONTH);
		dayGrid->setDay(DayofLastMonth.day());
	}

	for (QDate day = firstDayofMonth; day <= lastDayofMonth; day = day.addDays(1), ++index) {
		DreamDay* dayGrid = dayGrids_[index];
		dayGrid->setHidden(false);
		DayProperty property = DayProperty::IDLE;
		if (DataManager::getInstance()->isConcentrate(day)) {
			property = DayProperty::FININSH;
		}
		dayGrid->setDayPerperty(property);
		dayGrid->setDay(day.day());

		if (day == today_) {
			dayGrid->setStyleSheet("DreamDay { border: 2px solid #00308F; }");
		}
		else {
			dayGrid->setStyleSheet("");
		}
	}

	QDate DayofNextMonth = lastDayofMonth.addDays(1);
	for (int nextMonthIndex = index; nextMonthIndex < dayGrids_.count(); ++nextMonthIndex, DayofNextMonth = DayofNextMonth.addDays(1)) {
		DreamDay* dayGrid = dayGrids_[nextMonthIndex];
		dayGrid->setHidden(false);
		dayGrid->setDayPerperty(DayProperty::NOT_IN_THIS_MONTH);
		dayGrid->setDay(DayofNextMonth.day());
	}
}

void DreamCalendarWidget::updateCalendar()
{
 	today_ = QDate::currentDate();
	updateDayGrid();

	QTimer::singleShot(msecsToMidnight(), this, &DreamCalendarWidget::updateCalendar);
}

int DreamCalendarWidget::msecsToMidnight()
{
	QDateTime currentTime = QDateTime::currentDateTime();
	QDateTime tomorrow = currentTime.addDays(1);
	tomorrow.setTime(QTime(0, 0, 0, 0));

	return currentTime.msecsTo(tomorrow);
}
