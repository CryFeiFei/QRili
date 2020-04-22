#include "flwidget_linux.h"
#include "mainwidget.h"
#include "ktitlewidget.h"

#include <QLayout>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QApplication>

namespace
{
	#define ResizeHandleWidth 2
}

FLWidget_Linux::FLWidget_Linux(QWidget *parent) : QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

	setStyleSheet("background-color:white;");

	QAction* miniSizeAction = new QAction("最小化(&N)",this);
	QAction* maxSizeAction = new QAction("最大化(&X)",this);
	QAction* restoreWinAction = new QAction("还 原(&R)",this);
	QAction* quitAction = new QAction("退出(&Q)",this);
	connect(miniSizeAction,SIGNAL(triggered()),this,SLOT(hide()));
	connect(maxSizeAction,SIGNAL(triggered()),this,SLOT(showMaximized()));
	connect(restoreWinAction,SIGNAL(triggered()),this,SLOT(showNormal()));
	connect(quitAction,SIGNAL(triggered()),qApp,SLOT(quit()));

	// 设置托盘图标
	QMenu* menu = new QMenu(this);
	menu->addAction(miniSizeAction);
	menu->addAction(maxSizeAction);
	menu->addAction(restoreWinAction);
	menu->addAction(quitAction);

	QSystemTrayIcon* systray = new QSystemTrayIcon(this);
	systray->setIcon(QIcon(":/application/image/icon.svg"));
	systray->setContextMenu(menu);
	systray->show();

	QVBoxLayout* layoutMain = new QVBoxLayout(this);
	layoutMain->setContentsMargins(ResizeHandleWidth, ResizeHandleWidth, ResizeHandleWidth, ResizeHandleWidth);

	KTitleWidget* titleWidget = new KTitleWidget(this);
	layoutMain->addWidget(titleWidget);
	MainWidget* widget = new MainWidget(this);
	layoutMain->addWidget(widget);

	setLayout(layoutMain);

	resizingCornerEdge = XUtils::CornerEdge::kInvalid;
	setMouseTracking(true);
	XUtils::SetMouseTransparent(this, true);
	setAttribute(Qt::WA_ShowModal);
	resize(400, 400);
}


void FLWidget_Linux::mouseMoveEvent(QMouseEvent *event)
{
#ifdef Q_OS_LINUX
	const int x = event->x();
	const int y = event->y();

	if (resizingCornerEdge == XUtils::CornerEdge::kInvalid)
	{
		XUtils::UpdateCursorShape(this, x, y, this->layout()->contentsMargins(), ResizeHandleWidth);
	}
#endif

	return QWidget::mouseMoveEvent(event);
}

void FLWidget_Linux::mousePressEvent(QMouseEvent *event)
{
#ifdef Q_OS_LINUX
	const int x = event->x();
	const int y = event->y();
	if (event->button() == Qt::LeftButton)
	{
		const XUtils::CornerEdge ce = XUtils::GetCornerEdge(this, x, y, this->layout()->contentsMargins(), ResizeHandleWidth);
		if (ce != XUtils::CornerEdge::kInvalid)
		{
			resizingCornerEdge = ce;
			//send x11 move event dont send mouserrelease event
			XUtils::SendButtonRelease(this, event->pos(), event->globalPos());
			XUtils::StartResizing(this, QCursor::pos(), ce);
		}
	}
#endif
	return QWidget::mousePressEvent(event);
}

void FLWidget_Linux::resizeEvent(QResizeEvent *e)
{
#ifdef Q_OS_LINUX
	int resizeHandleWidth = ResizeHandleWidth;
	XUtils::SetWindowExtents(this, this->layout()->contentsMargins(), resizeHandleWidth);
#endif
	return QWidget::resizeEvent(e);
}

void FLWidget_Linux::mouseReleaseEvent(QMouseEvent *event)
{
#ifdef Q_OS_LINUX
	resizingCornerEdge = XUtils::CornerEdge::kInvalid;
#endif
	return QWidget::mouseReleaseEvent(event);
}

