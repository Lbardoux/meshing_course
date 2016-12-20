#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include <QGLWidget>
#include <gasket.h>
#include <QMouseEvent>

class GLDisplay : public QGLWidget
{
	Q_OBJECT
public:
	static Gasket gasket;
	explicit GLDisplay(QWidget *parent = 0);
	virtual void initializeGL();
	virtual void paintGL();
	virtual void resizeGL(int w, int h);
	void reset(void);

protected:
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );
	virtual void wheelEvent(QWheelEvent* event);

private:
    void drawSierpinski();
	double ZOOM;
    float _angle;
    QPoint _position;
	double _transx;
	double _transy;
    
signals:
    
public slots:
    
};

#endif // GLDISPLAY_H
