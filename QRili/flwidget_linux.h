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

#include <QMainWindow>

class FLWidget_Linux : public QWidget
{
	Q_OBJECT
public:
	explicit FLWidget_Linux(QWidget *parent = nullptr);

signals:

private slots:
	void trayClicked(QSystemTrayIcon::ActivationReason reason);

	void showNormalSize();
//	void rightClicked();

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
	QRect				m_initGeometry;
};

#endif // FLWIDGET_LINUX_H
