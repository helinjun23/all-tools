#include "DevLogManage.h"
#include "QTextEditEx.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <thread>
#include <QThread>
#include <QDateTime>
#include <QShortCut>
#include <QLineEdit>
#include <QPushButton>

#include <QTextCursor>
#include <QFontDialog>
#include <QColorDialog>

#include <QCompleter>
#include <QStringListModel>

DevLogManage::DevLogManage(QWidget* parent)
	: QWidget(parent)
{
	initRegisterParam();

	initUi();
}

DevLogManage::~DevLogManage()
{
	m_runState = false;
	//if (nullptr != m_recvThread) {
	//	if (m_recvThread->joinable()) {
	//		spdlog::info("debug recv thread exit success!");
	//		m_recvThread->join();
	//	}
	//}
	if (m_logSaveFile->isOpen()) {
		m_logSaveFile->close();
	}
}

void DevLogManage::initUi()
{
	// 日志文件
	m_logSaveFile = QSharedPointer<QFile>(new QFile(this));
	m_logTextStream = QSharedPointer<QTextStream>(new QTextStream(m_logSaveFile.get()));

	// 接收显示数据区
	m_devLog = new QTextEditEx(this);
	m_devLog->setReadOnly(true);

	QSettings setting;

	QPalette palette = m_devLog->palette();
	palette.setColor(QPalette::Base, setting.value("debugCom/bkgColor").value<QColor>());
	m_devLog->setPalette(palette);

	m_devLog->setFont(setting.value("debugCom/font").value<QFont>());
	m_devLog->setTextColor(setting.value("debugCom/fontColor").value<QColor>());

	m_devLog->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_devLog, &QTextEditEx::customContextMenuRequested, this, &DevLogManage::custom_context_menu_requested);
	connect(this, &DevLogManage::append_data, this, [=](const QString& data) {
		// 保存文件
		if (m_logSaveFile->isOpen()) {
			QTextStream text(data.toStdString().c_str());

			while (!text.atEnd()) {
				(*m_logTextStream) << QDateTime::currentDateTime().toString("\r\n[yyyy/MM/dd hh:mm:ss:zzz]  ");
				(*m_logTextStream) << text.readLine();
				m_logTextStream->flush();
			}
		}
		// 显示界面
		if (m_devLog->toPlainText().size() > 20 * 1024) {
			clear_all();
		}

		m_devLog->append(data);
		}

	);
	connect(m_devLog, &QTextEditEx::textChanged, this, [=] {
		m_devLog->moveCursor(QTextCursor::End);
		});

	// 发送数据区
	m_sendEdit = new QLineEdit(this);
	m_sendEdit->setPlaceholderText("Ctrl+Enter快捷发送指令");

	// shell 指令集自动补全功能
	initShellCompleter();

	m_sendButton = new QPushButton("发送", this);
	m_sendButton->setIcon(QIcon("://res/enter.png"));
	connect(m_sendButton, &QPushButton::clicked, this, [=] {
		if (m_comStat) {
			QString input_str = m_sendEdit->text();
			input_str.append("\r\n");
			if (!input_str.isEmpty()) {
				send_data(input_str.toUtf8());
			}
		}
		}
	);

	QVBoxLayout* vBox = new QVBoxLayout(this);
	vBox->setContentsMargins(0, 0, 0, 0);
	vBox->addWidget(m_devLog);

	QHBoxLayout* hBox = new QHBoxLayout; // 子布局不能从父窗口
	hBox->setContentsMargins(0, 0, 0, 0);
	hBox->addWidget(m_sendEdit);
	hBox->addWidget(m_sendButton);
	vBox->addLayout(hBox);

	this->setLayout(vBox);

	m_runState = true;
	// 数据接收显示
	m_recvThread = new QThread;
	m_devLogWork = new DevLogWork;
	m_devLogWork->moveToThread(m_recvThread);
	connect(m_devLogWork, SIGNAL(send(int, const QByteArray&)), this, SIGNAL(send(int, const QByteArray&)), Qt::DirectConnection);
	connect(m_devLogWork, SIGNAL(recv(int, QByteArray&)), this, SIGNAL(recv(int, QByteArray&)), Qt::BlockingQueuedConnection);
	connect(this, SIGNAL(start()), m_devLogWork, SLOT(start()));
	connect(m_devLogWork, SIGNAL(append_data(const QString&)), this, SIGNAL(append_data(const QString&)));

	m_recvThread->start();
	emit start();

	// 快捷键发送
	QShortcut* pshort = new QShortcut(tr("Ctrl+Return"), this);
	pshort->setAutoRepeat(false);
	connect(pshort, &QShortcut::activated, this, [=] {
		emit m_sendButton->clicked();
		});
}

void DevLogManage::custom_context_menu_requested(const QPoint& point)
{
	if (nullptr == m_contextMenu) {
		m_contextMenu = new QMenu(this);
		auto action_font = m_contextMenu->addAction("字体");
		connect(action_font, &QAction::triggered, this, [=] {
			m_devLog->setFont(QFontDialog::getFont(0, m_devLog->font(), m_devLog, "设置字体"));

			QSettings setting;
			setting.setValue("debugCom/font", m_devLog->font());
			}
		);

		auto action_color = m_contextMenu->addAction("背景色");
		connect(action_color, &QAction::triggered, this, [=] {
			QColor bk_color = QColorDialog::getColor(m_devLog->palette().color(QPalette::Base),
				m_devLog, "设置背景色", QColorDialog::DontUseNativeDialog);
			if (!bk_color.isValid()) {
				return;
			}

			QPalette palette = m_devLog->palette();
			palette.setColor(QPalette::Base, bk_color);
			m_devLog->setPalette(palette);

			QSettings setting;
			setting.setValue("debugCom/bkgColor", bk_color);
			}
		);

		auto action_textcolor = m_contextMenu->addAction("文本颜色");
		connect(action_textcolor, &QAction::triggered, this, [=] {
			QColor text_color = QColorDialog::getColor(m_devLog->textColor(), m_devLog, "设置文本颜色");
			if (!text_color.isValid()) {
				return;
			}
			m_devLog->setTextColor(text_color);

			auto char_format = m_devLog->currentCharFormat();
			char_format.setForeground(text_color);

			QTextCursor cursor = m_devLog->textCursor();
			cursor.select(QTextCursor::Document);
			cursor.mergeCharFormat(char_format);
			cursor.clearSelection();

			QSettings setting;
			setting.setValue("debugCom/fontColor", text_color);
			}
		);

		auto action_file_save = m_contextMenu->addAction("开始保存日志");
		action_file_save->setCheckable(true);
		connect(action_file_save, &QAction::triggered, this, [=](bool checked) {
			if (m_logSaveFile->isOpen()) {
				m_logSaveFile->close();
			}
			if (checked) {
				QString filename = QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + "_debug.log";
				spdlog::info("enbale save print info to file path logs/{}", filename.toStdString());
				m_logSaveFile->setFileName("logs/" + filename);
				if (!m_logSaveFile->open(QIODevice::WriteOnly)) {
					spdlog::error("open log file {} failed!", filename.toStdString());
				}
			}
			else {
				spdlog::info("disable save print info to file");
			}}
		);

		auto action_all_clear = m_contextMenu->addAction("全部清除");
		connect(action_all_clear, &QAction::triggered, this, [=] {
			clear_all();
			spdlog::info("manual clear all log data!");
			}
		);
	}
	m_contextMenu->popup(this->cursor().pos());
}

void DevLogManage::initRegisterParam()
{
	QSettings setting;
	if (setting.contains("debugCom/font")) {
		return;
	}
	// default
	setting.setValue("debugCom/font", QFont("consolas", 13));
	setting.setValue("debugCom/bkgColor", QColor(Qt::black));
	setting.setValue("debugCom/fontColor", QColor(Qt::white));
}

void DevLogManage::clear_all()
{
	// clear api
	// The undo/redo history is also cleared.
	// currentCharFormat() is reset, unless textCursor() is already at the beginning of the document.
	auto old_charforamt = m_devLog->currentCharFormat();
	m_devLog->clear();
	m_devLog->setCurrentCharFormat(old_charforamt);
}

void DevLogManage::initShellCompleter()
{
	QFile file("shell_cmds.txt");
	if (!file.open(QIODevice::ReadOnly)) {
		spdlog::error("open shell_cmds.txt failed!");
		return;
	}

	QStringList shell_lists;
	QTextStream text_stream(&file);

	// 解决中文乱码
	text_stream.setCodec(QTextCodec::codecForName("utf-8"));
	while (!text_stream.atEnd())
	{
		shell_lists << text_stream.readLine();
	}

	// 配置自动补全功能
	QCompleter* completer = new QCompleter(shell_lists, this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	m_sendEdit->setCompleter(completer);
}

void DevLogManage::send_data(const QByteArray& data)
{
	emit send(DEV_COM_TYP_DEBUG, data);
}

void DevLogManage::recv_data(QByteArray& data)
{
	emit recv(DEV_COM_TYP_DEBUG, data);
}