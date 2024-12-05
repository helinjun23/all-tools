#pragma once

#include <QTextEdit>

class QTextEditEx : public QTextEdit
{
	Q_OBJECT

public:
	QTextEditEx(QWidget* parent = nullptr);
	~QTextEditEx();
};
