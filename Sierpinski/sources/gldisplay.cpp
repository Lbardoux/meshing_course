#include "gldisplay.h"

#define FRUSTUM_SIZE 1.0f

Gasket GLDisplay::gasket;

GLDisplay::GLDisplay(QWidget *parent) : QGLWidget(parent),
                                        _angle(0.0f)
{
	this->ZOOM = 1.0;
}

void GLDisplay::reset(void)
{
	this->_transx = 0.0;
	this->_transy = 0.0;
	this->_position = QPoint(0.0, 0.0);
	this->_angle = 0.0f;
}

void GLDisplay::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glColor3f(1.0, 1.0, 0.0);
}

void GLDisplay::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLoadIdentity();
	glScaled(this->ZOOM, this->ZOOM, this->ZOOM);
	glTranslated(_transx, _transy, 0.0);
    glRotatef(_angle, 0.0f, 1.0f, 0.0f);

    gasket.draw();
}

void GLDisplay::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glViewport(0, 0, w, h);
    glOrtho(-FRUSTUM_SIZE, FRUSTUM_SIZE,
            -FRUSTUM_SIZE, FRUSTUM_SIZE,
            -FRUSTUM_SIZE, FRUSTUM_SIZE);
    glMatrixMode(GL_MODELVIEW);
}

void GLDisplay::mouseMoveEvent(QMouseEvent *event)
{
    if( event != NULL )
	{
		QPoint position = event->pos();
		if (event->buttons() == Qt::RightButton)
		{
			_transx += (position.x() - _position.x())/250.0;
			_transy -= (position.y() - _position.y())/250.0;
		}
		else
		{
			_angle += (position.x() - _position.x());
		}
        _position = position;

        updateGL();
    }
}

void GLDisplay::mousePressEvent(QMouseEvent *event)
{
    if( event != NULL )
        _position = event->pos();
}

void GLDisplay::wheelEvent(QWheelEvent* event)
{
	if( event != NULL )
	{
		int essai = event->delta();
		if (essai < 0)
		{
			this->ZOOM *= 0.9;
		}
		else if (essai > 0)
		{
			this->ZOOM *= 1.1;
		}
		updateGL();
	}
}
