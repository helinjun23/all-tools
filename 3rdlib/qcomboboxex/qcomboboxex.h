#pragma once

#include <QComboBox>

class QComboBoxEx : public QComboBox
{
	Q_OBJECT

public:
	using pBeShowFunc = void(*)(); // ��ʾ�����б�ǰ������
	explicit QComboBoxEx(QWidget* parent = nullptr);
	~QComboBoxEx();

	virtual void showPopup() override;
private:
	pBeShowFunc pFuncCb = nullptr;
};
