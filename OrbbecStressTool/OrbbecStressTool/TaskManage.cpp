#include "TaskManage.h"
#include <QHboxLayout>
#include <QMenu>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>

#include <qstyleditemdelegate.h>
#include <qstyleoption.h>
#include <QDebug>
#include <QRegexp>
#include <QThread>
#include <thread>
#include <QMessageBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>

#include "ProtocolManage.h"

// 默认添加行数
static constexpr int kDefualtRows = 3;
const QStringList kTableHead{ "ID","立即发送","上/下电","通道号","上电等待\r\nms","发送等待\r\nms","下电等待\r\nms","备注","命令码" };
// 表头id顺序
enum enTableHeadIdx {
	IDX_NUM = 0,
	IDX_SEND_BT,
	IDX_PWR_SW,
	IDX_CH_SW,
	IDX_PWR_ON_WAIT,
	IDX_SEND_WAIT,
	IDX_PWR_OFF_WAIT,
	IDX_NOTE,
	IDX_CMD_DATA,
};

TaskManage::TaskManage(QWidget* parent)
	: QWidget(parent)
{
	m_taskTable = new QTableWidget(this);
	QHBoxLayout* hlayout(new QHBoxLayout(this));
	hlayout->addWidget(m_taskTable);
	hlayout->setContentsMargins(0, 0, 0, 0);
	setLayout(hlayout);

	initUi();

	spdlog::debug("{} thread id {}", __FUNCTION__, QThread::currentThreadId());

}

TaskManage::~TaskManage()
{

}

void TaskManage::Start(int test_nums, int max_recv_timeout, bool relay_en)
{
	if (m_runState) {
		return;
	}

	m_test_nums = test_nums;
	m_max_recv_timeout = max_recv_timeout;
	m_relay_en = relay_en;

	m_taskPolicy->SetRunParam(m_max_recv_timeout, m_relay_en);
	auto_run_task_lists();
}

void TaskManage::Stop()
{
	m_runState = false;
	m_taskPolicy->stop();
}

void TaskManage::sync_com_stat(bool stat)
{
	m_comStat = stat;
	m_taskTable->setEnabled(m_comStat);
}

void TaskManage::initUi()
{
	QString styles("QHeaderView::section,QTableCornerButton:section{\
		padding:3px; margin:0px; color:#DCDCDC;  border:1px solid #242424;\
		border-left-width:0px; border-right-width:1px; border-top-width:0px; border-bottom-width:1px;\
		background:qlineargradient(spread : pad, x1 : 0, y1 : 0, x2 : 0, y2 : 1, stop : 0 #646464, stop:1 #525252);}\
		QTableWidget { background-color:white; border:none; outline:none; }\
		QTableWidget::item:hover{ background-color:rgb(92,188,227) }\
		QTableWidget::item:selected{ background-color:#1B89A1;border:none; outline:none;}\
		");

	m_taskTable->setStyleSheet(styles);
	m_taskTable->setContextMenuPolicy(Qt::CustomContextMenu);

	// 初始化命令行自动补全
	initCmdCompleter();

	connect(m_taskTable, &QTableWidget::customContextMenuRequested, this, [=](const QPoint& pos) {
		if (nullptr == m_menu) {
			m_menu = new QMenu(this);
			auto action_add = m_menu->addAction("新增");
			connect(action_add, &QAction::triggered, this, [=] {
				stTaskData taskItem;
				add_data(taskItem);
				});

			auto action_del = m_menu->addAction("删除");
			connect(action_del, &QAction::triggered, this, [=] {
				delete_data();
				});

			auto action_select_all = m_menu->addAction("全选/反选");
			connect(action_select_all, &QAction::triggered, this, [=] {
				select_data();
				});

			auto action_load_task_file = m_menu->addAction("加载任务");
			connect(action_load_task_file, &QAction::triggered, this, [=] {
				load_data();
				});

			auto action_save_task_file = m_menu->addAction("保存任务");
			connect(action_save_task_file, &QAction::triggered, this, [=] {
				save_data();
				});
		}
		m_menu->popup(this->cursor().pos());
		});

	auto header = m_taskTable->horizontalHeader();
	header->setHighlightSections(false);

	header->setFont(QFont("黑体", 12));
	QFont font = header->font();
	font.setBold(true);
	header->setFont(font);

	header->setStretchLastSection(true);
	m_taskTable->setColumnCount(kTableHead.size());
	m_taskTable->setHorizontalHeaderLabels(kTableHead);
	// 表头宽度自适应内容宽度
	header->setSectionResizeMode(QHeaderView::Stretch);
	for (int i = 0; i < kTableHead.size(); i++) {
		if (i != IDX_CMD_DATA) {
			header->setSectionResizeMode(i, QHeaderView::ResizeToContents);
		}
	}
	// 打开行选中
	//m_taskTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	// 隐藏水平表头
	//m_taskTable->verticalHeader()->hide();
	for (size_t i = 0; i < kDefualtRows; i++)
	{
		stTaskData taskItem;
		add_row_data(i, i + 1, taskItem);
	}


	connect(m_taskTable, &QTableWidget::itemPressed, this, [=](QTableWidgetItem* item) {
		auto id = item;
		if (id->isSelected() && id->column() == 0) {
			id->setCheckState(id->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
		}
		}
	);

	m_threadRun = new QThread();
	m_taskPolicy = new TaskPolicy();
	m_taskPolicy->moveToThread(m_threadRun);
	m_threadRun->start();

	// custom metatype
	qRegisterMetaType<uint32_t>("uint32_t");
	qRegisterMetaType<uint64_t>("uint64_t");
	qRegisterMetaType<QByteArray>("QByteArray&");
	qRegisterMetaType<QVector<stTaskData>>("const QVector<stTaskData>&");

	connect(this, SIGNAL(start_work(const QVector<stTaskData>&, uint32_t)), m_taskPolicy,
		SLOT(start(const QVector<stTaskData>&, uint32_t)));
	connect(m_taskPolicy, &TaskPolicy::disbaled_ui_action, this, [=](bool enabled) { m_taskTable->setEnabled(enabled); });
	connect(m_taskPolicy, &TaskPolicy::update_test_process, this, &TaskManage::update_test_process);
	connect(m_taskPolicy, &TaskPolicy::relay_sw, this, &TaskManage::relay_sw);
	connect(m_taskPolicy, &TaskPolicy::select_highlight_row, this, [=](int row) { m_taskTable->selectRow(row); });
	connect(m_taskPolicy, &TaskPolicy::send, this, &TaskManage::send, Qt::DirectConnection);
	connect(m_taskPolicy, SIGNAL(recv(int, QByteArray&)), this, SIGNAL(recv(int, QByteArray&)), Qt::BlockingQueuedConnection);
	connect(m_taskPolicy, &TaskPolicy::reopen_serial, this, &TaskManage::reopen_serial);

	connect(this, SIGNAL(send_data(const QByteArray&)), m_taskPolicy, SLOT(send_data(const QByteArray&)));
	connect(this, SIGNAL(recv_data(QByteArray&)), m_taskPolicy, SLOT(recv_data(QByteArray&)), Qt::BlockingQueuedConnection);


	m_taskTable->setEnabled(false);
}

void TaskManage::add_row_data(int row, int idx, const stTaskData& data)
{
	QFont font("黑体", 10);
	m_taskTable->insertRow(row);

	// 序号
	QTableWidgetItem* tbItem = new QTableWidgetItem(QString::number(data.id == -1 ? idx : data.id));
	tbItem->setFlags(tbItem->flags() ^ Qt::ItemIsEditable);
	tbItem->setCheckState(Qt::Unchecked);
	tbItem->setFont(font);
	//tbItem->setIcon(QIcon(":/res/checkbox_off.png"));
	////tbItem->setTextAlignment(Qt::AlignCenter);+
	m_taskTable->setItem(row, IDX_NUM, tbItem);

	// 立即发送按钮
	QPushButton* pb = new QPushButton("发送", m_taskTable);
	pb->setFont(QFont("黑体", 11));
	pb->setStyleSheet("QPushButton:pressed{color: white;background-color: gray;}");
	// QPushButton:hover{background-color:rgb(0, 145, 217);color: rgb(255, 255, 255);border-top:2px solid #000000;}
	m_taskTable->setCellWidget(row, IDX_SEND_BT, pb);

	connect(pb, &QPushButton::clicked, this, [=] {
		pb->setEnabled(false);
		asyncproc([=] {
			// 命令码数据
			int current_row = m_taskTable->currentRow();
			QString cmd = get_task_cmd_data(current_row);
			QByteArray data_buf = inputstr_convert(cmd);
			emit send_data(data_buf);
			data_buf.clear();
			emit recv_data(data_buf);
			if (data_buf.isEmpty()) {
				//QMessageBox::information(nullptr, "提示", "接收数据失败，请重新发送!");
				spdlog::warn("recv data failed,please try again!");
			}
			});
		pb->setEnabled(true); }
	);

	// 上下电使能
	QCheckBox* pEnPowerSw = new QCheckBox(m_taskTable);
	pEnPowerSw->setStyleSheet("QCheckBox::indicator {width: 32px;height: 32px;color: rgb(0, 0, 0);subcontrol-position:center center;}\
							QCheckBox::indicator:unchecked{image:url(:/res/sw_off.png);}\
							QCheckBox::indicator:checked{image:url(:/res/sw_on.png)}");
	m_taskTable->setCellWidget(row, IDX_PWR_SW, pEnPowerSw);

	// 上下电通道
	QComboBox* pChPwrSwCombox = new QComboBox(m_taskTable);
	pChPwrSwCombox->addItems({ "1","2","3","4" });
	m_taskTable->setCellWidget(row, IDX_CH_SW, pChPwrSwCombox);
	pChPwrSwCombox->setEnabled(data.pwr_sw);
	pChPwrSwCombox->setCurrentText(QString::number(data.chn));

	// 上电等待	
	auto pwrOnItem = new QTableWidgetItem(QString::number(data.pwr_on_duration_ms));
	m_taskTable->setItem(row, IDX_PWR_ON_WAIT, pwrOnItem);
	pwrOnItem->setFlags(tbItem->flags() ^ Qt::ItemIsEnabled);

	// 备注
	QLineEdit* note_edit = new QLineEdit(m_taskTable);
	note_edit->setText(QString::fromUtf8(data.note.c_str()));
	note_edit->setFrame(false);
	m_taskTable->setCellWidget(row, IDX_NOTE, note_edit);

	// 命令码
	QLineEdit* cmd_edit = new QLineEdit(m_taskTable);
	cmd_edit->setText(QString::fromUtf8(data.cmd.c_str()));
	cmd_edit->setFrame(false);
	cmd_edit->setCompleter(m_cmdCompleter);

	m_taskTable->setCellWidget(row, IDX_CMD_DATA, cmd_edit);

	connect(cmd_edit, &QLineEdit::editingFinished, this, [=] {
		int current_row = m_taskTable->currentRow();
		auto item = (QLineEdit*)m_taskTable->cellWidget(current_row, IDX_CMD_DATA);
		if (item->text().isEmpty()) {
			return;
		}
		spdlog::debug("cmd edit finished. row {} col {}", current_row, IDX_CMD_DATA);
		QString cmd_data = item->text();
		QByteArray data_buf = inputstr_convert(cmd_data);
		// 协议类型判断
		QVector<uint8_t> cmd_buf(data_buf.begin(), data_buf.end());
		int ret = ProtocolManage::JudgeProtocolType(cmd_buf);
		if (ret >= 0) {
			QByteArray byte_data((char*)cmd_buf.data(), cmd_buf.size());
			// 16进制转字符串
			QString strItem;
			for (auto& ch : byte_data) {
				strItem.append(QByteArray(1, ch).toHex()).append(' ');
			}
			item->setText(strItem.toUpper());
		}
		}
	);


	// 发送间隔
	m_taskTable->setItem(row, IDX_SEND_WAIT, new QTableWidgetItem(QString::number(data.send_duration_ms)));


	// 下电等待
	auto pwrOffItem = new QTableWidgetItem(QString::number(data.pwr_off_duration_ms));
	m_taskTable->setItem(row, IDX_PWR_OFF_WAIT, pwrOffItem);
	pwrOffItem->setFlags(tbItem->flags() ^ Qt::ItemIsEnabled);


	connect(pEnPowerSw, &QCheckBox::clicked, this, [=](bool bcheck) {
		spdlog::debug("power sw checked {}!", bcheck);
		pChPwrSwCombox->setEnabled(bcheck);
		if (!bcheck) {
			pwrOnItem->setFlags(tbItem->flags() ^ Qt::ItemIsEnabled ^ Qt::ItemIsEditable);
			pwrOffItem->setFlags(tbItem->flags() ^ Qt::ItemIsEnabled ^ Qt::ItemIsEditable);
		}
		else
		{
			pwrOnItem->setFlags(tbItem->flags() | Qt::ItemIsEnabled | Qt::ItemIsEditable);
			pwrOffItem->setFlags(tbItem->flags() | Qt::ItemIsEnabled | Qt::ItemIsEditable);
		}}
	);

	pEnPowerSw->setChecked(data.pwr_sw);
	pEnPowerSw->clicked(data.pwr_sw);

}

void TaskManage::delete_last_row()
{
	int row_count = m_taskTable->rowCount();
	m_taskTable->removeRow(row_count - 1);
	m_taskTable->scrollToBottom();
}

void TaskManage::select_all_item()
{
	for (int i = 0; i < m_taskTable->rowCount(); i++)
	{
		auto id_item = m_taskTable->item(i, IDX_NUM);
		id_item->setCheckState(id_item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
	}
}

void TaskManage::delete_all_data()
{
	m_taskTable->clearContents();
	m_taskTable->setRowCount(0);
}

void TaskManage::add_data(const stTaskData& data)
{
	int row_index = m_taskTable->rowCount();
	int last_idx = row_index;
	if (row_index > 0) {
		last_idx = m_taskTable->item(row_index - 1, IDX_NUM)->text().toInt();
	}
	add_row_data(row_index, last_idx + 1, data);
	m_taskTable->scrollToBottom();
}

void TaskManage::delete_data()
{
	bool bCheck = false;
	int i = 0;
	// 删除选中项
	do {
		if (m_taskTable->item(i, IDX_NUM)->checkState() == Qt::Checked) {
			m_taskTable->removeRow(i);
			bCheck = true;
		}
		else {
			i++;
		}
	} while (i < m_taskTable->rowCount());


	// 未找到选中项，则删除最后一项
	if (!bCheck) {
		delete_last_row();
	}
}

void TaskManage::select_data()
{
	select_all_item();
}

void TaskManage::load_data()
{
	QString fileName = QFileDialog::getOpenFileName(this, "加载任务文件", "./", "文件名 (*.csv)");
	if (fileName.isEmpty())
	{
		spdlog::warn("load task file data failed!");
		return;
	}

	QList<QStringList> datalist;
	if (!readCsv(fileName, datalist))
	{
		spdlog::error("read csv {} failed!", fileName.toStdString());
		return;
	}

	// 清空数据
	delete_all_data();

	// 同步数据
	stTaskData taskdata;
	for (int row = 1; row < datalist.size(); row++) {
		QStringList col = datalist[row];
		// 严格对照csv列数
		taskdata.id = col[0].toInt();
		taskdata.pwr_sw = col[1].toInt();
		taskdata.chn = col[2].toInt();
		taskdata.pwr_on_duration_ms = col[3].toInt();
		taskdata.send_duration_ms = col[4].toInt();
		taskdata.pwr_off_duration_ms = col[5].toInt();

		if (col.size() >= 7)
		{
			taskdata.note = col[6].toStdString();
		}
		if (col.size() >= 8)
		{
			taskdata.cmd = col[7].toStdString();
		}

		add_data(taskdata);
	}

	return;

}

void TaskManage::save_data()
{
	// new csv file
	QString savepath = QFileDialog::getSaveFileName(this, "保存任务文件", "", "文件 (*.csv)");
	if (savepath.isEmpty()) {
		spdlog::warn("save data file path invalid!");
		return;
	}

	// load table data
	const QStringList tableHead{ "ID","上/下电","通道号","上电等待ms","发送等待ms","下电等待ms","备注","命令码" };
	QList<QStringList> items;
	items.push_back(tableHead);
	for (int i = 0; i < m_taskTable->rowCount(); i++) {
		items.push_back(get_row_data(i));
	}

	// write csv
	if (!writeCsv(savepath, items)) {
		spdlog::warn("save csv file failed!");
	}
	QMessageBox::information(this, "提示", QString("文件%1保存成功").arg(savepath));
}

void TaskManage::auto_run_task_lists()
{
	//1. 获取选中任务列表
	int i = 0;
	QVector<stTaskData> taskItems;
	do {
		if (get_task_checked(i)) {
			stTaskData item;
			get_row_data(i, item);
			taskItems.push_back(item);
		}
	} while (++i < m_taskTable->rowCount());

	emit start_work(taskItems, m_test_nums);
}

QString TaskManage::get_task_cmd_data(int row)
{
	//auto item = m_taskTable->item(row, IDX_CMD_DATA);//item(row, IDX_CMD_DATA); (QLineEdit*)
	auto item = (QLineEdit*)m_taskTable->cellWidget(row, IDX_CMD_DATA);
	if (nullptr == item) {
		return QString("");
	}
	return item->text();
}

bool TaskManage::get_task_checked(int row)
{
	QTableWidgetItem* item = m_taskTable->item(row, IDX_NUM);
	return item->checkState() == Qt::Checked;
}

int TaskManage::get_task_send_duration(int row)
{
	if (-1 == row) {
		return 0;
	}
	QTableWidgetItem* item = m_taskTable->item(row, IDX_SEND_WAIT);
	return item->text().toInt();
}

int TaskManage::get_task_pwr_on_wait(int row)
{
	if (-1 == row) {
		return 0;
	}
	QTableWidgetItem* item = m_taskTable->item(row, IDX_PWR_ON_WAIT);
	return item->text().toInt();
}

int TaskManage::get_task_pwr_off_wait(int row)
{
	if (-1 == row) {
		return 0;
	}
	QTableWidgetItem* item = m_taskTable->item(row, IDX_PWR_OFF_WAIT);
	return item->text().toInt();
}

int TaskManage::get_task_id(int row)
{
	if (-1 == row) {
		return 0;
	}
	QTableWidgetItem* item = m_taskTable->item(row, IDX_NUM);
	return item->text().toInt();
}

QString TaskManage::get_task_note(int row)
{
	auto item = (QLineEdit*)m_taskTable->cellWidget(row, IDX_NOTE);
	if (nullptr == item) {
		return QString("");
	}
	return item->text();
}

std::pair<int, bool> TaskManage::get_task_sw_checked(int row) const
{
	QTableWidgetItem* item = m_taskTable->item(row, IDX_PWR_SW);
	QCheckBox* checkbox = (QCheckBox*)m_taskTable->cellWidget(row, IDX_PWR_SW);
	QComboBox* comb = (QComboBox*)m_taskTable->cellWidget(row, IDX_CH_SW);
	return std::make_pair(comb->currentText().toInt(), checkbox->checkState() == Qt::Checked);
}

void TaskManage::get_row_data(int row, stTaskData& data)
{
	data.id = get_task_id(row);

	auto ch_sw = get_task_sw_checked(row);
	data.chn = ch_sw.first;
	data.pwr_sw = ch_sw.second;
	data.pwr_on_duration_ms = get_task_pwr_off_wait(row);
	data.pwr_off_duration_ms = get_task_pwr_off_wait(row);
	data.send_duration_ms = get_task_send_duration(row);
	data.note = get_task_note(row).toStdString();
	data.cmd = get_task_cmd_data(row).toStdString();
}

QStringList TaskManage::get_row_data(int row)
{
	QStringList strRowData;
	if (row < 0) {
		return strRowData;
	}

	stTaskData item;
	get_row_data(row, item);
	strRowData << QString::number(item.id)
		<< QString::number(item.pwr_sw)
		<< QString::number(item.chn)
		<< QString::number(item.pwr_on_duration_ms)
		<< QString::number(item.send_duration_ms)
		<< QString::number(item.pwr_off_duration_ms)
		<< QString::fromUtf8(item.note.c_str())
		<< QString::fromUtf8(item.cmd.c_str());

	return strRowData;
}

void TaskManage::initCmdCompleter()
{
	QFile file("orbcmd_books.txt");
	if (!file.open(QIODevice::ReadOnly)) {
		spdlog::error("open orbcmd_books.txt failed!");
		return;
	}

	QStringList cmd_lists;
	QTextStream text_stream(&file);

	// 解决中文乱码
	text_stream.setCodec(QTextCodec::codecForName("utf-8"));
	while (!text_stream.atEnd())
	{
		cmd_lists << text_stream.readLine();
	}

	// 配置自动补全功能
	m_cmdCompleter = new QCompleter(cmd_lists, this);
	// 设置匹配模式，只要包含就显示
	m_cmdCompleter->setFilterMode(Qt::MatchContains);
	m_cmdCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	// 设置向用户提供补全的方式
	m_cmdCompleter->setCompletionMode(QCompleter::PopupCompletion);
}