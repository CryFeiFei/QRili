#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QWebEngineView>
#include <QVBoxLayout>
#include "ktitlewidget.h"
#include <QLineEdit>
MainWidget::MainWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MainWidget)
{
	ui->setupUi(this);
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	QString strUrl = QString::fromLatin1("https://rili.wps.cn/home");
	QWebEngineView* webView = new QWebEngineView(this);
	webView->setAttribute(Qt::WA_InputMethodEnabled,true);
	webView->setAttribute(Qt::WA_KeyCompression,true);
	webView->setFont(QFont("Source Code Pro"));
	webView->load(QUrl(strUrl));

	setMouseTracking(true);
	mainLayout->addWidget(webView);

	setLayout(mainLayout);
}

MainWidget::~MainWidget()
{
	delete ui;
}
