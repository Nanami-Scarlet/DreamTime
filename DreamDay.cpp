#include "DreamDay.h"

const int DAY_GRID_LEN = 58;

DreamDay::DreamDay(QWidget *parent)
	: QLabel(parent)
{
	laDay_ = new QLabel(this);

	setAlignment(Qt::AlignCenter);
	laDay_->setStyleSheet(R"(
		font-family: "Microsoft YaHei";
		font-size: 30px;
		color: #FFFFFF;
	)");
}

DreamDay::~DreamDay()
{
	if (laDay_) {
		delete laDay_;
		laDay_ = nullptr;
	}
}

void DreamDay::setDay(int day)
{
	laDay_->setText(QString::number(day));
	QFontMetrics fontMeterics{ laDay_->font() };
	int textWidth = fontMeterics.width(laDay_ ->text());
	int textHeight = fontMeterics.height();
	
	laDay_->move(DAY_GRID_LEN / 2 - textWidth / 2, DAY_GRID_LEN / 2 - textHeight / 2);
}

void DreamDay::setDayPerperty(DayProperty perperty)
{
	QString img;

	switch (perperty)
	{
	case DayProperty::NOT_IN_THIS_MONTH:
		img = ":/images/images/grey_day.png";
		break;
	case DayProperty::FININSH:
		img = ":/images/images/deepblue_day.png";
		break;
	case DayProperty::IDLE:
		img = ":/images/images/blue_day.png";
		break;
	default:
		break;
	}

	setPixmap(QPixmap(img));
}
