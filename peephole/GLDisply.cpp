#include "GLDisply.h"
#include "StreamDataTyp.h"
#include "clogcat.h"

CGlDisply::CGlDisply(QWidget* parent) :QOpenGLWidget(parent)
{
	m_viewParam.posLeft = -1.0;
	m_viewParam.posRight = 1.0;
	m_viewParam.posTop = 1.0;
	m_viewParam.posButtom = -1.0;

	m_textureID = 0;
	//
	m_dataW = 1080;
	m_dataH = 1920;

	memset(m_dataBuf, 0, sizeof(m_dataBuf));
	m_bDataPush = false;
}

CGlDisply::~CGlDisply()
{
	if (m_textureID)
	{
		glDeleteTextures(1, (const GLuint*)&m_textureID);
		m_textureID = 0;
	}
}

void CGlDisply::initializeGL()
{
	initializeOpenGLFunctions();
	InitOpengl();
	CreateTexture();
}

void CGlDisply::resizeGL(int width, int height)
{
	m_viewWidth = width;
	m_viewHeight = height;

	glViewport(0, 0, m_viewWidth, m_viewHeight);

	//QString noteStr;
	//noteStr.sprintf("viewW=%d,viewH=%d",m_viewWidth,m_viewHeight);
	//emit ShowNoteInfo(noteStr);
}

void CGlDisply::paintGL()
{

	DataRender(0, m_dataW, m_dataH);
	//ProgramDataRender();
	//glFinish();
	//update();
}

void CGlDisply::SetDisplyArea(int x, int y, int width, int height)
{
	setGeometry(x, y, width, height);
}

void CGlDisply::DataDisply(int width, int height, int fmt, uint8_t* data)
{
	if (NULL == data) {
		ClearDisplay();
		return;
	}
	uint8_t pixelNum = 3;
	if (fmt == RenderDataTypFmtRGB)
	{
		pixelNum = 3;
	}
	if (fmt == RenderDataTypFmtRGBA)
	{
		pixelNum = 4;
	}
	m_mutex.lock();
	m_dataW = width;
	m_dataH = height;
	m_dataFmt = fmt;
	memcpy(m_dataBuf, data, width * height * pixelNum);
	m_bDataPush = true;
	m_mutex.unlock();
	//emit CLogcat::Instance()->ShowNoteInfo("GL push data!",0,true);
#if 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glEnable(GL_TEXTURE_2D);

	if (fmt == RenderDataTypFmtRGB)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	if (fmt == RenderDataTypFmtRGBA)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}
#endif
	update();
}

void CGlDisply::InitOpengl()
{
	//glClearStencil(128);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glEnable(GL_NORMALIZE);

	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	//GLfloat ambient[] = {0.5, 0.5, 0.5, 1};
	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	//glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.1f);
}

void CGlDisply::CreateTexture(void)
{
	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//GL_CLAMP_TO_EDGE);

	//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void CGlDisply::DataRender(int fmt, int width, int height)
{
	//emit CLogcat::Instance()->ShowNoteInfo("render,w="+QString::number(width)+",h="+QString::number(height),1,true);


	m_mutex.lock();
	//if(!m_bDataPush)
	//{
	//    m_mutex.unlock();
	//    return;
	//}
	m_bDataPush = false;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	//glEnable(GL_TEXTURE_2D);

	if (m_dataFmt == RenderDataTypFmtRGB)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_dataBuf);
	}

	if (m_dataFmt == RenderDataTypFmtRGBA)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_dataBuf);
	}

	m_mutex.unlock();
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	float left = 0.0, right = 0.0, top = 0.0, buttom = 0.0;
	float ratio = 0.0;
	float hx = 0.0, wx = 0.0, deltahx = 0.0, deltawx = 0.0;

	if (width > height)
	{
		wx = m_viewParam.posRight - m_viewParam.posLeft;
		ratio = (float)height * 1.0 / width;
		hx = ratio * wx;
		deltahx = (m_viewParam.posTop - m_viewParam.posButtom - hx) / 2;
		left = m_viewParam.posLeft;
		right = m_viewParam.posRight;
		top = m_viewParam.posTop - deltahx;
		buttom = m_viewParam.posButtom + deltahx;
	}

	if (width < height)
	{
		hx = m_viewParam.posTop - m_viewParam.posButtom;
		ratio = (float)width * 1.0 / height;
		wx = hx * ratio;
		deltawx = (m_viewParam.posRight - m_viewParam.posLeft - wx) / 2;
		left = m_viewParam.posLeft + deltawx;
		right = m_viewParam.posRight - deltawx;
		top = m_viewParam.posTop;
		buttom = m_viewParam.posButtom;
	}

	glBegin(GL_QUADS);
	// set the color of the polygon
	//glColor4f(1, 1, 1, 1);

	glTexCoord2f(0.0, 1.0);
	//glVertex2f(viewParam.posLeft,viewParam.posButtom);
	glVertex2f(left, buttom);

	glTexCoord2f(1.0, 1.0);
	//glVertex2f(viewParam.posRight,viewParam.posButtom);
	glVertex2f(right, buttom);

	glTexCoord2f(1.0, 0.0);
	//glVertex2f(viewParam.posRight,viewParam.posTop);
	glVertex2f(right, top);

	glTexCoord2f(0.0, 0.0);
	//glVertex2f(viewParam.posLeft,viewParam.posTop);
	glVertex2f(left, top);

	glEnd();
}
