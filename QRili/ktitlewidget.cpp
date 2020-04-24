#include "ktitlewidget.h"
#include "ui_ktitlewidget.h"
#include <QMouseEvent>

namespace
{
	const QString qssCloseButton =
		"QPushButton {"
			"border: 0px solid;"
			"image:url(:/images/wechatcustomerservice/close_normal.png);"
		"}"
		"QPushButton:hover:!pressed {"
			"image:url(:/images/wechatcustomerservice/close_hover.png);"
		"}"
		"QPushButton:pressed {"
			"image:url(:/images/wechatcustomerservice/close_down.png);"
		"}";

	const static QString qssCancelButton =
		"QPushButton {"
			"color:#4F5D79;"
			"background-color:rgba(255,255,255,1);"
			"border-radius:4px;"
			"border:1px solid;"
			"border-color:rgba(54,66,90,30%);"
		"}"
		"QPushButton:hover:!pressed {"
			"background-color:#F1F3F5;"
			"color:#36425A;"
			"border-radius:4px;"
			"border:1px solid;"
			"border-color:rgba(54,66,90,30%);"
		"}"
		"QPushButton:pressed {"
			"background-color:rgba(139,151,175,30%);"
			"border-radius:4px;"
			"border:none;"
			"color:#36425A;"
		"}";

	const static QString qssRetryButton =
		"QPushButton {"
			"color:#FFFFFF;"
			"background-color:#417FF9;"
			"border-radius:4px;"
			"border:none;"
		"}"
		"QPushButton:hover:!pressed {"
			"background-color:#608DFA;"
			"color:#FFFFFF;"
			"border-radius:4px;"
			"border:none;"
		"}"
		"QPushButton:pressed {"
			"background-color:#1F72F1;"
			"border-radius:4px;"
			"border:none;"
			"color:#FFFFFF;"
		"}";
}

KTitleWidget::KTitleWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::KTitleWidget)
{
	setObjectName("KTitleWidget");
	setMouseTracking(true);
	ui->setupUi(this);

	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	setStyleSheet("background-color: rgb(122,122,122)");


	connect(ui->miniButton, SIGNAL(clicked()), this, SIGNAL(miniButtonClicked()));
	connect(ui->maxButton, SIGNAL(clicked()), this, SIGNAL(maxButtonClicked()));
	connect(ui->closeButton, SIGNAL(clicked()), this, SIGNAL(closeButtonClicked()));
	connect(ui->normalButton, SIGNAL(clicked()), this, SIGNAL(normalButtonClicked()));

	ui->miniButton->setStyleSheet(qssRetryButton);
	ui->maxButton->setStyleSheet(qssRetryButton);
	ui->closeButton->setStyleSheet(qssRetryButton);
	ui->normalButton->setStyleSheet(qssRetryButton);

	ui->miniButton->setFixedSize(48, 48);
	ui->maxButton->setFixedSize(48, 48);
	ui->closeButton->setFixedSize(48, 48);
	ui->normalButton->setFixedSize(48, 48);

	setMinimumHeight(50);
	setMaximumHeight(50);

	ui->miniButton->setVisible(false);
}

KTitleWidget::~KTitleWidget()
{
	delete ui;
}

void KTitleWidget::mousePressEvent(QMouseEvent* e)
{
	if (e->buttons() & Qt::LeftButton)
	{
		m_bPress = true;
		QMouseEvent *eventMouse = (QMouseEvent*)e;
		QPoint point = this->pos();
		QPoint mousePoint = eventMouse->globalPos();
		m_movePos = mousePoint - point;
		m_parentPos = parentWidget()->pos();
	}

	QWidget::mousePressEvent(e);
}

void KTitleWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (m_bPress && e->buttons() & Qt::LeftButton)
	{
		QMouseEvent *eventMouse = (QMouseEvent*)e;
		QPoint movePoint = eventMouse->globalPos() - m_movePos;
		QMargins margins =  parentWidget()->layout()->contentsMargins();
		QPoint parentMargins(margins.left(), margins.top());
		parentWidget()->move(movePoint + m_parentPos - parentMargins);
	}

	QWidget::mouseMoveEvent(e);
}

void KTitleWidget::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->buttons() & Qt::LeftButton)
		m_bPress = false;

	QWidget::mouseReleaseEvent(e);
}

