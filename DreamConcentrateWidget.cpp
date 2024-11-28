#include "DreamConcentrateWidget.h"
#include "DataManager.h"

#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>

const int CIRCLE_WIDTH = 581;
const int CIRCLE_HEIGHT = 581;
const int INCH = 30;
const int RING_WIDTH = 65;

DreamConcentrateWidget::DreamConcentrateWidget(QWidget *parent)
	: QWidget(parent)
{
	setStyleSheet(R"(
		QLineEdit {
			width: 111px;
			height: 61px;
			image: url(:/images/images/input.png);
			background-color: #FFFFFF; 
			color: #000000;
			selection-background-color: #A0A0A0;
			selection-color: #F2F2F2;
			font-family: "Microsoft YaHei";
			font-size: 30px;
		}
	)");

	isRunning_ = false;
	isStopped_ = false;
	currentTime_ = 0;
	totalTime_ = 0;

	leTime_ = new QLineEdit(this);
	leTime_->setEnabled(false);
	leTime_->setText(QString::number(totalTime_) + " min");
	leTime_->setAlignment(Qt::AlignHCenter);

	btnleft_ = new QPushButton(this);
	btnleft_->setStyleSheet(R"(
		image: url(:/images/images/minus_button.png);
		width: 61px;
		height: 61px;
		border: none;
	)");
	connect(btnleft_, &QPushButton::clicked, this, &DreamConcentrateWidget::onButtonMinusClick);

	btnright_ = new QPushButton(this);
	btnright_->setStyleSheet(R"(
		image: url(:/images/images/plus_button.png);
		width: 61px;
		height: 61px;
		border: none;
	)");
	connect(btnright_, &QPushButton::clicked, this, &DreamConcentrateWidget::onButtonAddClick);

	btnConfirm_ = new QPushButton(this);
	btnConfirm_->setText(tr("开始专注！"));
	btnConfirm_->setStyleSheet(R"(
		QPushButton {
			image: url(:/images/images/button_normal.png);
			width: 227px;
			height: 51px;
			border: none;
			font-family: "Microsoft YaHei";
			font-size: 25px;
		}

		QPushButton:hover {
			image: url(:/images/images/button_hover.png);
		}

		QPushButton:pressed {
			image: url(:/images/images/button_press.png);
		}
	)");
	connect(btnConfirm_, &QPushButton::clicked, this, &DreamConcentrateWidget::onButtonConfirmClick);

	btnStop_ = new QPushButton(this);
	btnStop_->setText(tr("暂停"));
	btnStop_->hide();
	btnStop_->setStyleSheet(R"(
		QPushButton {
			image: url(:/images/images/button_normal.png);
			width: 227px;
			height: 51px;
			border: none;
			font-family: "Microsoft YaHei";
			font-size: 25px;
		}

		QPushButton:hover {
			image: url(:/images/images/button_hover.png);
		}

		QPushButton:pressed {
			image: url(:/images/images/button_press.png);
		}
	)");
	connect(btnStop_, &QPushButton::clicked, this, &DreamConcentrateWidget::onButtonStopClick);

	btnCancel_ = new QPushButton(this);
	btnCancel_->setText(tr("取消"));
	btnCancel_->hide();
	btnCancel_->setStyleSheet(R"(
		QPushButton {
			image: url(:/images/images/button_normal.png);
			width: 227px;
			height: 51px;
			border: none;
			font-family: "Microsoft YaHei";
			font-size: 25px;
		}

		QPushButton:hover {
			image: url(:/images/images/button_hover.png);
		}

		QPushButton:pressed {
			image: url(:/images/images/button_press.png);
		}
	)");
	connect(btnCancel_, &QPushButton::clicked, this, &DreamConcentrateWidget::onButtonCancelClick);

	timer_ = new QTimer(this);
	connect(timer_, &QTimer::timeout, this, &DreamConcentrateWidget::onTimeout);
}

DreamConcentrateWidget::~DreamConcentrateWidget()
{
	if (leTime_) {
		delete leTime_;
		leTime_ = nullptr;
	}

	if (btnleft_) {
		delete btnleft_;
		btnleft_ = nullptr;
	}

	if (btnright_) {
		delete btnright_;
		btnright_ = nullptr;
	}

	if (btnConfirm_) {
		delete btnConfirm_;
		btnConfirm_ = nullptr;
	}

	if (timer_) {
		if (timer_->isActive()) {
			timer_->stop();
			isRunning_ = false;
		}
	}

	disconnect(btnleft_, &QPushButton::clicked, this, &DreamConcentrateWidget::onButtonMinusClick);
	disconnect(btnright_, &QPushButton::clicked, this, &DreamConcentrateWidget::onButtonAddClick);
	disconnect(btnConfirm_, &QPushButton::clicked, this, &DreamConcentrateWidget::onButtonConfirmClick);
	disconnect(btnStop_, &QPushButton::clicked, this, &DreamConcentrateWidget::onButtonStopClick);
	disconnect(timer_, &QTimer::timeout, this, &DreamConcentrateWidget::onTimeout);
}

void DreamConcentrateWidget::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.save();

	painter.translate(width() / 2, height() / 2);
	painter.drawPixmap(QRect(-CIRCLE_WIDTH / 2, -CIRCLE_HEIGHT / 2, CIRCLE_WIDTH, CIRCLE_HEIGHT), QString(":/images/images/circle progress.png"));

	if (isRunning_) {
		// 单位统一
		float ratio = (float)currentTime_ / (totalTime_ * 60);

		// 先画蓝色的圆
		painter.save();
		painter.setPen(QPen(QColor(38, 120, 255), 2));
		painter.setBrush(QColor(38, 120, 255));
		painter.translate(-CIRCLE_WIDTH / 2, -CIRCLE_HEIGHT / 2);
		QRect drawRectBlue(0, 0, CIRCLE_WIDTH, CIRCLE_WIDTH);
		painter.drawPie(drawRectBlue, 90 * 16, -360 * ratio * 16);
		painter.restore();

		// 再画内圆
		painter.save();
		painter.setPen(QPen(QColor(240, 240, 240), 2));
		painter.setBrush(QColor(240, 240, 240));
		painter.translate(-(CIRCLE_WIDTH - RING_WIDTH) / 2, -(CIRCLE_WIDTH - RING_WIDTH) / 2);
		QRect drawRectWhite(0, 0, CIRCLE_WIDTH - RING_WIDTH, CIRCLE_WIDTH - RING_WIDTH);
		painter.drawPie(drawRectWhite, 90 * 16, -360 * 16);
		painter.restore();

		QFont font;
		font.setFamily("Microsoft YaHei");
		font.setWeight(80);
		font.setPixelSize(80);
		painter.setFont(font);
		QFontMetrics metrics = painter.fontMetrics();
		QString ratioStr = QString("%1%").arg((int)(ratio * 100));
		int textWidth = metrics.width(ratioStr);
		int textHeight = metrics.height();
		painter.drawText(-textWidth / 2, -textHeight / 2, ratioStr);
	}

	painter.restore();
}

void DreamConcentrateWidget::resizeEvent(QResizeEvent* event)
{
	int widthCenterX = width() / 2;
	int widthCenterY = height() / 2;

	int lePosX = widthCenterX - leTime_->width() / 2;
	int lePoxY = widthCenterY - leTime_->height() / 2 - 50;
	leTime_->move(lePosX, lePoxY);

	btnleft_->move(widthCenterX - 150, lePoxY);
	btnright_->move(widthCenterX + 150 - 61, lePoxY);
	btnConfirm_->move(widthCenterX - 227 / 2, widthCenterY + 25);
	btnStop_->move(widthCenterX - 227 / 2, widthCenterY + 25);
	btnCancel_->move(widthCenterX - 227 / 2, widthCenterY + 100);
}

void DreamConcentrateWidget::onButtonAddClick()
{
	totalTime_ = std::min(totalTime_ + INCH, 900);
	leTime_->setText(QString::number(totalTime_) + " min");
}

void DreamConcentrateWidget::onButtonMinusClick()
{
	totalTime_ = std::max(totalTime_ - INCH, 0);
	leTime_->setText(QString::number(totalTime_) + " min");
}

void DreamConcentrateWidget::onButtonConfirmClick()
{
	if (totalTime_ == 0) {
		return;
	}

	startConcentrate();
}

void DreamConcentrateWidget::onButtonStopClick()
{
	if (!isStopped_) {
		assert(timer_->isActive());

		btnStop_->setText("继续");
		timer_->stop();
		isStopped_ = true;
	}
	else {
		assert(!timer_->isActive());

		btnStop_->setText("暂停");
		timer_->start(1000);
		isStopped_ = false;
	}
}

void DreamConcentrateWidget::onButtonCancelClick()
{
	QMessageBox msgBox(QMessageBox::Question, "取消专注?", "将会保存已经专注的时间，确定取消吗?");
	msgBox.addButton(tr("是"), QMessageBox::YesRole);
	msgBox.addButton(tr("否"), QMessageBox::RejectRole);
	
	timer_->stop();

	int ret = msgBox.exec();
	if (ret == QMessageBox::AcceptRole) {
		finishConcentrate();
		stopConcentrate();
	}
	else {
		timer_->start(1000);
	}
}

void DreamConcentrateWidget::onTimeout()
{
	++currentTime_;
	// 单位统一
	if (currentTime_ == totalTime_ * 60) {
		if (timer_->isActive()) {
			timer_->stop();
			finishConcentrate();
			stopConcentrate();
			currentTime_ = 0;
		}
	}

	repaint();
}

void DreamConcentrateWidget::startConcentrate()
{
	isRunning_ = true;
	timer_->start(1000);

	leTime_->hide();
	btnleft_->hide();
	btnright_->hide();
	btnConfirm_->hide();
	btnStop_->show();
	btnCancel_->show();

	repaint();
}

void DreamConcentrateWidget::stopConcentrate()
{
	isRunning_ = false;
	timer_->stop();
	currentTime_ = 0;
	totalTime_ = 0;
	leTime_->setText(QString::number(totalTime_) + " min");

	leTime_->show();
	btnleft_->show();
	btnright_->show();
	btnConfirm_->show();
	btnStop_->show();
	btnStop_->hide();
	btnCancel_->hide();

	repaint();
}

void DreamConcentrateWidget::finishConcentrate()
{
	QDate currentDate = QDate::currentDate();
	DataManager::getInstance()->updateData(currentDate.year(), currentDate.month(), currentDate.day(), currentTime_);
	emit update();
}
