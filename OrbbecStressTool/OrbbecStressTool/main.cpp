#include "OrbbecStressTool.h"
#include <QtWidgets/QApplication>
#include <QDateTime>
#include <windows.h>
#include <QSettings>

#include "qbreakpad/include/QBreakpadHandler.h"
#ifdef NDEBUG
#pragma comment(lib, "../../3rdlib/qbreakpad/lib/x64/release/qBreakpad.lib")
#else
#pragma comment(lib, "../../3rdlib/qbreakpad/lib/x64/debug/qBreakpad.lib")
#endif

#include "common.h"

int main(int argc, char* argv[])
{
#ifndef NDEBUG
	AllocConsole();
	freopen("CONOUT$", "r+t", stdout);
#endif
	QApplication a(argc, argv);

	// 指定dmp目录
	QBreakpadInstance.setDumpPath(QApplication::applicationDirPath() + "/crashs");
	//设置不应用操作系统设置比如字体
	QApplication::setDesktopSettingsAware(false);
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
	QApplication::setAttribute(Qt::AA_Use96Dpi);
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5,14,0))
	QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Floor);
#endif

	QString ver = "V" + QApplication::applicationVersion();
	QString windtitle = "奥比猫眼压测工具 " + ver;

	// 全局使用QSetting，配置下方参数即可，无需再传入组名
	QCoreApplication::setOrganizationName("Orbbec");
	QCoreApplication::setOrganizationDomain("www.orbbec.com.cn");
	QCoreApplication::setApplicationName("Peephole Stress Tool");

	QSettings setting;
	setting.setValue("version", QApplication::applicationVersion());
	setting.setValue("name", windtitle);

	// 系统日志初始化
	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	//console_sink->set_level(spdlog::level::trace);
	//console_sink->set_pattern("[main_sink] [%^%l%$] %v");
	std::string log_name = QDateTime::currentDateTime().toString("yyyyMMddhh").toStdString().append("_sys.log");
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/" + log_name, true);
	//file_sink->set_level(spdlog::level::trace);

	auto logger = std::make_shared<spdlog::logger>(std::string("main"), spdlog::sinks_init_list{ console_sink, file_sink });
	//std::shared_ptr<spdlog::logger> logger(new spdlog::logger("main", { console_sink, file_sink }));
	logger->flush_on(spdlog::level::trace);
	spdlog::set_default_logger(logger);

	//change log pattern
	//spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

	spdlog::info("Ready Start Test!!!!");
	spdlog::info("version {}", ver.toStdString());
	spdlog::set_level((spdlog::level::level_enum)setting.value("log_level", spdlog::level::info).toInt());

	QFont font;
	font.setFamily("Microsoft Yahei");
	font.setPixelSize(13);
	a.setFont(font);

	OrbbecStressTool w;
	w.setWindowTitle(windtitle);
	w.setMinimumSize(QSize(900, 600));
	w.setWindowIcon(QIcon(":/res/orbbec.ico"));

	w.show();
	return a.exec();
}
