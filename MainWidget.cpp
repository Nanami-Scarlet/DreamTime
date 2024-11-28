#include "MainWidget.h"
#include "DreamConcentrateWidget.h"
#include "DreamCalendarWidget.h"
#include "DreamTabWidget.h"

#include <QVBoxLayout>

const int WIDGET_WIDTH = 1280;
const int WIDGET_HEIGHT = 720;

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    // ui.setupUi(this);
	setWindowTitle("DreamTime");
	setFixedSize(WIDGET_WIDTH, WIDGET_HEIGHT);
   

    QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
    DreamTabWidget* tabWidget = new DreamTabWidget(this);
    vBoxLayout->addWidget(tabWidget);

    concentrateWidget_ = new DreamConcentrateWidget(tabWidget);
    calendarWidget_ = new DreamCalendarWidget(tabWidget);
    connect(concentrateWidget_, &DreamConcentrateWidget::update, calendarWidget_, &DreamCalendarWidget::updateDayGrid);

    tabWidget->addTab(concentrateWidget_, tr("专注"));
    tabWidget->addTab(calendarWidget_, tr("日历"));
}

MainWidget::~MainWidget()
{}
