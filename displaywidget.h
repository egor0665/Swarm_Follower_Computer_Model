#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QTimer>
#include <textrenderer.h>
#include <objloader.h>

struct Position
{
    float x=0,y=0,z=0;
    Position(){};
    Position(float x,float y,float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

class DisplayWidget : public QOpenGLWidget
{
    Q_OBJECT

    Position sumPosition;
    int sumCount;
    Position resultSumPosition;
    Position resultSumPosition2;
    Position resultSumPosition3;
    QVector<Position> lastPosVect = {};
    QVector<Position> lastPosVect2 = {};
    QVector<Position> lastPosVect3 = {};

    GLdouble LAeyes[3]={10,5,10}, LAcenter[3]={0,0,0}, LAup[3]={0,1,0};
    Position lookAt = {0,0,0};
    QPoint mousePos;
    GLfloat imgZoom=-20;
    GLfloat XmouseRot = 0, ZmouseRot = 0;
    TextRenderer textRenderer;

    GLuint vehicleModel;
    GLuint vehicleTextures;

    Position currentSignalPosition = {0,0,0};

public:
    explicit DisplayWidget(QWidget *parent = nullptr);

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void setCurrentSignalPosition(Position position);
protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent *event) override;

protected slots:
    void updateImage();

signals:

};

#endif // DISPLAYWIDGET_H
