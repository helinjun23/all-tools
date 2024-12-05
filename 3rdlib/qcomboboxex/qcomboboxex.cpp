#include "qcomboboxex.h"
#include "qserialex/serialportex.h"

QComboBoxEx::QComboBoxEx(QWidget* parent)
	: QComboBox(parent)
{}

QComboBoxEx::~QComboBoxEx()
{}


void QComboBoxEx::showPopup()
{
	auto serialInfoList = SerialPortEx::FindAllComs();
	clear();
	for (auto& serial : serialInfoList)
	{
		QString strCom = QString("%1 #%2").arg(serial.portName()).arg(serial.description());
		addItem(strCom);
	}
	repaint();
	QComboBox::showPopup();
}
