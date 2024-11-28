#include "DreamTabWidget.h"
#include "DreamTabBar.h"

DreamTabWidget::DreamTabWidget(QWidget *parent)
	: QTabWidget(parent)
{
	setTabBar(new DreamTabBar);
	setTabPosition(QTabWidget::West);

	setStyleSheet(R"(
		QTabWidget::pane {
			border-top:1px solid #EAEAEA;
			position:absolute;
			top:-0.1px;
		}

		QTabBar::tab {
			font-size:18px;
			font-family:Microsoft YaHei;
			font-weight:400;
			background:#FFFFFF;
			border:2px solid #FFFFFF;
			border-bottom-color:#FFFFFF;
			border-top-left-radius:4px;
			border-top-right-radius:4px;
			padding:2px;
		}

		QTabBar::tab:selected {
			color:#333333;
			border-color:#FFFFFF;
			border-bottom-color:#4BA4F2;
		}

		QTabBar::tab:!selected {
            color:#B2B2B2;
            border-color:#FFFFFF;
            border-bottom-color:#FFFFFF;
        }
	)");
}