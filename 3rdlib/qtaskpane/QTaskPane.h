/**
 * @file QTaskPane.h
 * @brief 任务窗格控件实现
 * @author xuenuo (xuenuo@orbbec.com)
 * @version 1.0
 * @date 2023-06-25
 *
 * @copyright Copyright (c) ORBBEC Technology Group Co., Ltd. All rights reserved
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2023-06-25 <td>1.0     <td>wangh     <td>内容
 * </table>
 */
#pragma once

#include <QObject>
#include <QMap>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>

#pragma execution_character_set("utf-8")


class QTaskPane : public QScrollArea
{
	Q_OBJECT

public:
	QTaskPane(QWidget* parent = nullptr);
	~QTaskPane();

	int AddWidget(QString pane_name, QWidget* widget);
	int RemoveWidget(QString pane_name);

	QWidget* GetWidget(QString pane_name);

Q_SIGNALS:
	void allToggle();

public Q_SLOTS:


protected:
	void init();

private:
	QMap<QString, QPair<QPushButton*, QWidget*>> paneGroups;
	QMap<QPushButton*, bool> paneToggle;

	QWidget* paneWidget;
	QVBoxLayout* paneLayout;
	QSpacerItem* paneSpacer;
};
