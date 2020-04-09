#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainwidget.h"

#include <QWebEngineView>
#include <QSystemTrayIcon>

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

	MainWidget* widget = new MainWidget(this);
	setCentralWidget(widget);
}

MainWindow::~MainWindow()
{
	delete ui;
}

