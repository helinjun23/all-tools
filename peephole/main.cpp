#include "mainwindow.h"
#include <QApplication>


//#pragma comment(lib,"avformat.lib")
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <libavformat/version.h>
#include <libavutil/time.h>
#include <libavutil/mathematics.h>
}

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	QFont font;
	font.setFamily("Microsoft Yahei");
	font.setPixelSize(12);
	a.setFont(font);

	MainWindow w;
	w.show();

	return a.exec();
}
