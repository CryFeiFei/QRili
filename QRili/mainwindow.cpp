#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include <QWebEngineView>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	//QString strUrl = QString::fromLatin1("https://rili.wps.cn");
	QString strUrl = QString::fromLatin1("https://rili.wps.cn/home");
	QWebEngineView* m_webView = new QWebEngineView(this);
	m_webView->load(QUrl(strUrl));
//	m_webView->showNormal();

	setCentralWidget(m_webView);
}

MainWindow::~MainWindow()
{
	delete ui;
}

