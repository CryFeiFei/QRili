#ifndef FLWIDGET_LINUX_H
#define FLWIDGET_LINUX_H

#include <QWidget>
#include "xutil.h"

#include <QLayout>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QApplication>

class FLWidget_Linux : public QWidget
{
	Q_OBJECT
public:
	explicit FLWidget_Linux(QWidget *parent = nullptr);

signals:

public slots:

public:
	void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;

private:
	void _initTray();

public:
	XUtils::CornerEdge	resizingCornerEdge;
	Qt::WindowFlags		dwindowFlags;
	QSystemTrayIcon*	m_tray;
	QMenu*				m_trayMenu;
};

#endif // FLWIDGET_LINUX_H
