#pragma once

#include <QTabBar>

class DreamTabBar  : public QTabBar
{
	Q_OBJECT

public:
	DreamTabBar(QWidget *parent = nullptr);
	~DreamTabBar();

protected:
	QSize tabSizeHint(int index) const override;
	void paintEvent(QPaintEvent*) override;
};
