#ifndef _GL_DISPLY_H_
#define _GL_DISPLY_H_
#include <QWidget>
//#include <GL/glut.h>
//#include <GL/glu.h>
#include<QOpenGLWidget>

#include <QDateTime>
#include <QThread>
#include <QMutex>
#include"QOpenGLFunctions"
#include "cdatadisply.h"


typedef struct DataViewStruc {
	//pos param
	float posLeft;
	float posRight;
	float posTop;
	float posButtom;
}DataViewStruc;

class CGlDisply : public QOpenGLWidget, protected QOpenGLFunctions, public CDataDisply
{
public:
	CGlDisply(QWidget* parent = nullptr);
	~CGlDisply();
	void initializeGL()  Q_DECL_OVERRIDE;
	void resizeGL(int w, int h)Q_DECL_OVERRIDE;
	void paintGL()Q_DECL_OVERRIDE;

	void SetDisplyArea(int x, int y, int width, int height);
	void DataDisply(int width, int height, int fmt, uint8_t* data);

	void ClearDisplay() {
		m_mutex.lock();
		memset(m_dataBuf, 0, sizeof(m_dataBuf));
		m_mutex.unlock();
	}
protected:
	void InitOpengl(void);
	void CreateTexture(void);
	void DataRender(int fmt, int width, int height);
private:
	int    m_viewWidth;
	int    m_viewHeight;
	DataViewStruc m_viewParam;
	//
	int m_dataW, m_dataH;
	int m_dataFmt;
	QMutex m_mutex;
	bool m_bDataPush;
	//
	GLuint m_textureID;
	uint8_t m_dataBuf[8294400];
};


#endif
