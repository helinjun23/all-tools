#pragma once

// 3rd
#include "utility/crc_clac.h"
#include "utility/time_duration.hpp"
#include "utility/utility.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/async.h"
#include "spdlog/stopwatch.h"
#include "spdlog/fmt/ranges.h"
#include "spdlog/fmt/bin_to_hex.h"

// STL
#include <iostream>
#include <string>


// QT
#include <QSettings>
#include <QTimer>
#include <QTextStream>
#include <QStringList>
#include <QTextCodec>
#include <QButtonGroup>
#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QApplication>
#include <QFileDialog>
#include <QAtomicInteger>
#include <QThread>
#include <QMetaType>
#include <QCompleter>


// �豸��������
enum enDevComType {
	DEV_COM_TYP_CMMC = 0,  // ͨ�Ŵ���
	DEV_COM_TYP_DEBUG, // ���Դ���
	DEV_COM_TYP_RELAY, // �̵�������
};

// �첽ִ�к�ʱ���ã��ݲ�֧�ֳ�Ա����
template<typename Func, typename...Args>
void asyncproc(Func proc, Args...args)
{
	auto future = QtConcurrent::run(proc, args...);
	while (!future.isFinished())
	{
		QApplication::processEvents(QEventLoop::AllEvents, 100);
	}
}
