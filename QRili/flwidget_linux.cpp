#include "flwidget_linux.h"
#include "mainwidget.h"
#include "ktitlewidget.h"

#include <QLayout>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QApplication>

#include <QDebug>

namespace
{
	#define ResizeHandleWidth 2
}

FLWidget_Linux::FLWidget_Linux(QWidget *parent) : QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

	setStyleSheet("background-color:white;");

	_initTray();

	QVBoxLayout* layoutMain = new QVBoxLayout(this);
	layoutMain->setContentsMargins(ResizeHandleWidth, ResizeHandleWidth, ResizeHandleWidth, ResizeHandleWidth);

	KTitleWidget* titleWidget = new KTitleWidget(this);
	layoutMain->addWidget(titleWidget);
	connect(titleWidget, SIGNAL(closeButtonClicked()), this, SLOT(hide()));
	connect(titleWidget, SIGNAL(maxButtonClicked()), this, SLOT(showMaximized()));
	connect(titleWidget, SIGNAL(normalButtonClicked()), this, SLOT(showNormalSize()));


	MainWidget* widget = new MainWidget(this);
	layoutMain->addWidget(widget);

	setLayout(layoutMain);

	resizingCornerEdge = XUtils::CornerEdge::kInvalid;
	setMouseTracking(true);
	XUtils::SetMouseTransparent(this, true);
	resize(800, 800);
	m_initGeometry = this->geometry();
}

void FLWidget_Linux::_initTray()
{
	QAction* miniSizeAction = new QAction("最小化(&N)",this);
	QAction* maxSizeAction = new QAction("最大化(&X)",this);
	QAction* restoreWinAction = new QAction("还 原(&R)",this);
	QAction* quitAction = new QAction("退出(&Q)",this);
	connect(miniSizeAction,SIGNAL(triggered()),this,SLOT(hide()));
	connect(maxSizeAction,SIGNAL(triggered()),this,SLOT(showMaximized()));
	connect(restoreWinAction,SIGNAL(triggered()),this,SLOT(showNormal()));
	connect(quitAction,SIGNAL(triggered()),qApp,SLOT(quit()));
	// 设置托盘图标
	m_trayMenu = new QMenu(this);
	m_trayMenu->addAction(miniSizeAction);
	m_trayMenu->addAction(maxSizeAction);
	m_trayMenu->addAction(restoreWinAction);
	m_trayMenu->addAction(quitAction);

	m_tray = new QSystemTrayIcon(this);
	m_tray->setIcon(QIcon(":/application/image/icon.svg"));
	m_tray->setContextMenu(m_trayMenu);

	connect(m_tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayClicked(QSystemTrayIcon::ActivationReason)));

	m_tray->show();
}

void FLWidget_Linux::showNormalSize()
{
//	showMinimized();
//	this->resize(800, 800);
//	this->move(900, 900);
	setGeometry(600,600, 800, 800);
//	this->showNormal();
}

void FLWidget_Linux::trayClicked(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
	case QSystemTrayIcon::Trigger:
		this->hide();
		break;
	case QSystemTrayIcon::DoubleClick:
		this->showNormal();
		break;
	default:
		break;
	}
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

