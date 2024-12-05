#include "QTaskPane.h"



/*
***************************************
* pane1 name						  *
***************************************
*									  *
*		widget1 display				  *
*									  *
*									  *
***************************************
* pane2 name						  *
***************************************
*									  *
*		widget2 display				  *
*									  *
*									  *
***************************************
*/

QTaskPane::QTaskPane(QWidget* parent)
	: QScrollArea(parent)
{
	init();
}

QTaskPane::~QTaskPane()
{}

int QTaskPane::AddWidget(QString pane_name, QWidget* widget)
{
	// ����������
	QPushButton* paneTitle = new QPushButton;
	paneTitle->setText(pane_name);
	paneTitle->setMinimumWidth(100);

	// ���񴰸�
	QWidget* paneBox = widget;


	if (!paneGroups.contains(pane_name)) {
		// 
		paneGroups.insert(pane_name, QPair<QPushButton*, QWidget*>(paneTitle, paneBox));
		// 
		int idx = paneLayout->count();
		idx = idx == 0 ? 1 : idx;
		paneLayout->insertWidget(idx - 1, paneTitle);
		paneLayout->insertWidget(idx, paneBox);

		connect(paneTitle, &QPushButton::clicked, this, [=](bool checked) {
			QPushButton* pb = qobject_cast<QPushButton*>(sender());
			if (!paneToggle.contains(pb)) {
				paneToggle.insert(pb, true); // Ĭ���۵�
			}

			bool& bFold = paneToggle[pb];
			QString btsheet("image-position: right;text-align: center;font:bold;"); // font: 8pt \"΢���ź�\";
			if (bFold) {
				btsheet.append("image: url(:/res/fold.png);");
			}
			else {
				btsheet.append("image: url(:/res/unfold.png);");
			}

			paneTitle->setStyleSheet(btsheet);
			bFold = !bFold;
			paneBox->setVisible(bFold);

			}
		);

		emit paneTitle->clicked();

		return 0;
	}

	return -1;
}

int QTaskPane::RemoveWidget(QString pane_name)
{
	auto paneMap = paneGroups.find(pane_name);
	if (paneMap != paneGroups.end()) {
		QPushButton* paneBar = paneMap.value().first;
		QWidget* paneWidget = paneMap.value().second;
		paneLayout->removeWidget(paneBar);
		paneBar->setParent(nullptr);
		paneLayout->removeWidget(paneWidget);
		paneWidget->setParent(nullptr);
		//
		paneGroups.remove(pane_name);
		paneToggle.remove(paneBar);

		return 0;
	}

	return -1;
}

QWidget* QTaskPane::GetWidget(QString pane_name)
{
	auto paneMap = paneGroups.find(pane_name);
	if (paneMap != paneGroups.end()) {
		return paneMap.value().second;
	}
	return nullptr;
}

void QTaskPane::init()
{
	// ��������
	paneWidget = new QWidget(this);
	paneLayout = new QVBoxLayout(paneWidget);
	paneLayout->setContentsMargins(0, 3, 0, 3);
	paneLayout->setSpacing(1);

	paneSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
	paneLayout->addItem(paneSpacer);

	this->setWidgetResizable(true);
	this->setWidget(paneWidget);
}
