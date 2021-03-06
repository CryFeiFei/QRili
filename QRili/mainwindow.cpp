#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainwidget.h"

#include <QWebEngineView>
#include <QSystemTrayIcon>
#include <QMouseEvent>
#include <QLayout>

namespace
{
	#define ResizeHandleWidth 10
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

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

	//
	setWindowFlags(Qt::FramelessWindowHint);
	this->layout()->setContentsMargins(ResizeHandleWidth, ResizeHandleWidth, ResizeHandleWidth, ResizeHandleWidth);
	resizingCornerEdge = XUtils::CornerEdge::kInvalid;
	setMouseTracking(true);


	MainWidget* widget = new MainWidget(this);
	setCentralWidget(widget);
	move(300, 300);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
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

void MainWindow::mousePressEvent(QMouseEvent *event)
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

void MainWindow::resizeEvent(QResizeEvent *e)
{
#ifdef Q_OS_LINUX
	int resizeHandleWidth = ResizeHandleWidth;
	XUtils::SetWindowExtents(this, this->layout()->contentsMargins(), resizeHandleWidth);
#endif
	return QWidget::resizeEvent(e);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
#ifdef Q_OS_LINUX
	resizingCornerEdge = XUtils::CornerEdge::kInvalid;
#endif
	return QWidget::mouseReleaseEvent(event);
}

