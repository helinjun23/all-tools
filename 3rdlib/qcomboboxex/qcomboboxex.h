#pragma once

#include <QComboBox>

class QComboBoxEx : public QComboBox
{
	Q_OBJECT

public:
	using pBeShowFunc = void(*)(); // 显示下拉列表前处理函数
	explicit QComboBoxEx(QWidget* parent = nullptr);
	~QComboBoxEx();

	virtual void showPopup() override;
private:
	pBeShowFunc pFuncCb = nullptr;
};
