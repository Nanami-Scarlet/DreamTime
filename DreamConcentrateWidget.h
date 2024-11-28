#pragma once

#include <QWidget>

class QPushButton;
class QLineEdit;
class QTimer;

class DreamConcentrateWidget  : public QWidget
{
	Q_OBJECT

public:
	DreamConcentrateWidget(QWidget *parent);
	~DreamConcentrateWidget();

signals:
	void update();

protected:
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

private:
	void onButtonAddClick();
	void onButtonMinusClick();
	void onButtonConfirmClick();
	void onButtonStopClick();
	void onButtonCancelClick();
	void onTimeout();

	void startConcentrate();
	void stopConcentrate();
	void finishConcentrate();

private:
	QLineEdit* leTime_;
	QPushButton* btnleft_;
	QPushButton* btnright_;
	QPushButton* btnConfirm_;
	QPushButton* btnStop_;
	QPushButton* btnCancel_;

	QTimer* timer_;

	bool isRunning_;
	bool isStopped_;
	int currentTime_;
	int totalTime_;
};
