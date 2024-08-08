#include "displaywidget.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <QMouseEvent>
#include <math.h>

const int IDLE_TIMER_INTERVAL=100;


struct Offset
{
    float xOffset;
    float yOffset;
    float zOffset;
    Offset(){};
    Offset(float xOffset, float yOffset, float zOffset)
    {
        this->xOffset = xOffset;
        this->yOffset = yOffset;
        this->zOffset = zOffset;
    }
};



DisplayWidget::DisplayWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
}

void DisplayWidget::initializeGL()
{
    textRenderer.init(this);

    vehicleModel = ObjLoader::Instance().load("C:/Qt/5.15.2/mingw81_32/GroundControlStation/drone2.obj");
    glGenTextures(1,&vehicleTextures);
    QImage texture1;

    texture1.load("C:/Qt/5.15.2/mingw81_32/GroundControlStation/DroneTexture.png");

    texture1.convertTo(QImage::Format_RGBA8888);
    glBindTexture(GL_TEXTURE_2D,vehicleTextures);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,3,GLsizei(texture1.width()),
                 GLsizei(texture1.height()),
                 0,
                 GL_RGBA,GL_UNSIGNED_BYTE,
                 texture1.bits());


    glEnable(GL_DEPTH_TEST);
    glClearColor(0.11372,0.13333,0.15686,0);


    glEnable(GL_MULTISAMPLE);

    glEnable(GL_TEXTURE_3D);
    glEnable(GL_CULL_FACE);

    glClearDepth(1.0);

    glDepthFunc(GL_LESS);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void DisplayWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    qreal aspectratio = qreal(w)/qreal(h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0,aspectratio,0.1,400.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void DisplayWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();



    const GLfloat totalSceneDist = 50.0f;
    const GLfloat XsceneOffset = -25.0f;
    const GLfloat ZsceneOffset = -25.0f;
    const GLfloat YsceneOffset = 0.0f;
    const Offset sceneOffset(XsceneOffset, YsceneOffset, ZsceneOffset);
    const GLfloat numSceneLines = 20;

    const GLfloat sceneUnitDist = totalSceneDist/ numSceneLines;


    gluLookAt(lookAt.x + cos(0.01 * ZmouseRot) * (-imgZoom),
            lookAt.y + 0.1 * XmouseRot,
            lookAt.z + sin(0.01 * ZmouseRot) * (-imgZoom),
            lookAt.x,
            lookAt.y,
            lookAt.z,
            LAup[0],
            LAup[1],
            LAup[2]);

    glPushMatrix();
    glLineWidth(5.0f);
    glBegin(GL_LINES); // построение линии
        glColor4f(1.00f, 0.00f, 0.00f, 1.0f);
        glVertex3f( totalSceneDist + sceneOffset.xOffset,  sceneOffset.yOffset,  sceneOffset.zOffset);
        glVertex3f( sceneOffset.xOffset,  sceneOffset.yOffset,  sceneOffset.zOffset);


        glColor4f(0.00f, 1.00f, 0.00f, 1.0f);
        // ось y зеленого цвета
        glVertex3f( sceneOffset.xOffset,  totalSceneDist + sceneOffset.yOffset,  sceneOffset.zOffset);
        glVertex3f( sceneOffset.xOffset,  sceneOffset.yOffset, sceneOffset.zOffset);


        glColor4f(0.00f, 0.00f, 1.00f, 1.0f);
        // ось z синего цвета
        glVertex3f( sceneOffset.xOffset,  sceneOffset.yOffset,  totalSceneDist+  sceneOffset.zOffset);
        glVertex3f( sceneOffset.xOffset,  sceneOffset.yOffset,  sceneOffset.zOffset);

        glEnd();

        GLfloat offset = 15;
        textRenderer.renderText(totalSceneDist + sceneOffset.xOffset + offset,  sceneOffset.yOffset , sceneOffset.zOffset, "X", Qt::red);
        textRenderer.renderText(sceneOffset.xOffset ,  totalSceneDist + sceneOffset.yOffset  + offset, sceneOffset.zOffset, "Y", Qt::green);
        textRenderer.renderText(sceneOffset.xOffset ,  sceneOffset.yOffset , totalSceneDist + sceneOffset.zOffset + offset, "Z", Qt::blue);


        glLineWidth(0.1f);
        glBegin(GL_LINES);
        glColor4f(1.00f, 1.00f, 1.00f, 1.0f);

        for (int i=1;i<numSceneLines+1;i++)
        {
            glVertex3f( totalSceneDist + sceneOffset.xOffset,  i*sceneUnitDist + sceneOffset.yOffset,  sceneOffset.zOffset);
            glVertex3f( sceneOffset.xOffset,  i*sceneUnitDist + sceneOffset.yOffset, sceneOffset.zOffset);

            glVertex3f( totalSceneDist + sceneOffset.xOffset,  sceneOffset.yOffset,  i*sceneUnitDist + sceneOffset.zOffset);
            glVertex3f( sceneOffset.xOffset,  sceneOffset.yOffset,  i*sceneUnitDist + sceneOffset.zOffset);

            glVertex3f( i*sceneUnitDist + sceneOffset.xOffset,  totalSceneDist + sceneOffset.yOffset,  sceneOffset.zOffset);
            glVertex3f( i*sceneUnitDist + sceneOffset.xOffset,  sceneOffset.yOffset,  sceneOffset.zOffset);

            glVertex3f( sceneOffset.xOffset,  totalSceneDist + sceneOffset.yOffset,  i*sceneUnitDist+sceneOffset.zOffset);
            glVertex3f( sceneOffset.xOffset,  sceneOffset.yOffset,  i*sceneUnitDist+sceneOffset.zOffset);

            glVertex3f( i*sceneUnitDist+sceneOffset.xOffset ,  sceneOffset.yOffset,  totalSceneDist+sceneOffset.zOffset);
            glVertex3f( i*sceneUnitDist+sceneOffset.xOffset,  sceneOffset.yOffset,  sceneOffset.zOffset);

            glVertex3f( sceneOffset.xOffset,  i*sceneUnitDist + sceneOffset.yOffset, totalSceneDist+sceneOffset.zOffset);
            glVertex3f( sceneOffset.xOffset,  i*sceneUnitDist + sceneOffset.yOffset,  sceneOffset.zOffset);
        }
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glScalef(0.3, 0.3, 0.3);
    glColor4f(1.00f, 1.00f, 1.00f, 1.0f);
    glCallList(vehicleModel);
    glPopMatrix();

    GLUquadric *quadrick = gluNewQuadric();

    //---
    glPushMatrix();
    gluQuadricDrawStyle(quadrick, GLU_LINE);
    glColor3d(1,0,0.7);
    glLineWidth(0.1f);

    glTranslatef(0.3f, 0.0f, 0.3f);
    gluSphere(quadrick, 0.1, 10, 10);
    glPopMatrix();
    textRenderer.renderText(0.3f, 0.0f, 0.3f,"5C:CF:7F:C7:A1:68",Qt::red);
    //---
    glPushMatrix();
    gluQuadricDrawStyle(quadrick, GLU_LINE);
    glColor3d(1,0,0.7);
    glLineWidth(0.1f);

    glTranslatef(0.3f, 0.0f, -0.3f);
    gluSphere(quadrick, 0.1, 10, 10);
    glPopMatrix();
    textRenderer.renderText(0.3f, 0.0f, -0.3f,"5C:CF:7F:8C:4F:60",Qt::red);
    //---
    glPushMatrix();
    gluQuadricDrawStyle(quadrick, GLU_LINE);
    glColor3d(1,0,0.7);
    glLineWidth(0.1f);

    glTranslatef(-0.3f, 0.0f, -0.3f);
    gluSphere(quadrick, 0.1, 10, 10);
    glPopMatrix();
    textRenderer.renderText(-0.3f, 0.0f, -0.3f,"68:C6:3A:9D:EC:BA",Qt::red);
    //---
    glPushMatrix();
    gluQuadricDrawStyle(quadrick, GLU_LINE);
    glColor3d(1,0,0.7);
    glLineWidth(0.1f);

    glTranslatef(0.0f, 0.2f, 0.0f);
    gluSphere(quadrick, 0.1, 10, 10);
    glPopMatrix();
    textRenderer.renderText(0.0f, 0.2f, 0.0f,"BC:DD:C2:25:51:AB",Qt::red);
    //------------------------------
    glPushMatrix();
    gluQuadricDrawStyle(quadrick, GLU_LINE);
    glColor3d(1,1,1);
    glLineWidth(1.0f);

    glTranslatef(currentSignalPosition.y, currentSignalPosition.z, currentSignalPosition.x);
//    glTranslatef(currentSignalPosition.x, currentSignalPosition.z, currentSignalPosition.y);
    gluSphere(quadrick, 0.5, 10, 10);
    glPopMatrix();
    //------------------------------
    glPushMatrix();
    gluQuadricDrawStyle(quadrick, GLU_LINE);
    glColor3d(1,0,1);
    glLineWidth(1.0f);

    glTranslatef(resultSumPosition.y, resultSumPosition.z, resultSumPosition.x);
//    glTranslatef(resultSumPosition.x, resultSumPosition.z, resultSumPosition.y);
    gluSphere(quadrick, 0.5, 10, 10);
    glPopMatrix();
    //------------------------------

    glPushMatrix();
    gluQuadricDrawStyle(quadrick, GLU_LINE);
    glColor3d(0,0,1);
    glLineWidth(1.0f);

    glTranslatef(resultSumPosition2.y, resultSumPosition2.z, resultSumPosition2.x);
//    glTranslatef(resultSumPosition2.x, resultSumPosition2.z, resultSumPosition2.y);
    gluSphere(quadrick, 0.5, 10, 10);
    glPopMatrix();
    //------------------------------

    glPushMatrix();
    gluQuadricDrawStyle(quadrick, GLU_LINE);
    glColor3d(1,0,0);
    glLineWidth(1.0f);

    glTranslatef(resultSumPosition3.y, resultSumPosition3.z, resultSumPosition3.x);
//    glTranslatef(resultSumPosition3.x, resultSumPosition3.z, resultSumPosition3.y);

    gluSphere(quadrick, 0.5, 10, 10);
    glPopMatrix();
    //------------------------------


    gluDeleteQuadric(quadrick);

}

void DisplayWidget::setCurrentSignalPosition(Position position)
{
    currentSignalPosition = position;

    lastPosVect.append(position);
    lastPosVect2.append(position);
    lastPosVect3.append(position);
    sumCount ++;

    Position sumPosition = {0,0,0};
    for (int i=0;i<lastPosVect.length();i++){
        sumPosition.x += lastPosVect[i].x;
        sumPosition.y += lastPosVect[i].y;
        sumPosition.z += lastPosVect[i].z;
    }

    if (lastPosVect.length()>=10){
        lastPosVect.remove(0);
    }
    sumPosition.x /= lastPosVect.length();
    sumPosition.y /= lastPosVect.length();
    sumPosition.z /= lastPosVect.length();
    resultSumPosition = sumPosition;

    sumPosition = {0,0,0};
    for (int i=0;i<lastPosVect2.length();i++){
        sumPosition.x += lastPosVect2[i].x;
        sumPosition.y += lastPosVect2[i].y;
        sumPosition.z += lastPosVect2[i].z;
    }
    if (lastPosVect2.length()>=20){
        lastPosVect2.remove(0);
    }
    sumPosition.x /= lastPosVect2.length();
    sumPosition.y /= lastPosVect2.length();
    sumPosition.z /= lastPosVect2.length();
    resultSumPosition2 = sumPosition;


    sumPosition = {0,0,0};
    for (int i=0;i<lastPosVect3.length();i++){
        sumPosition.x += lastPosVect3[i].x;
        sumPosition.y += lastPosVect3[i].y;
        sumPosition.z += lastPosVect3[i].z;
    }
    if (lastPosVect3.length()>=30){
        lastPosVect3.remove(0);
    }
    sumPosition.x /= lastPosVect3.length();
    sumPosition.y /= lastPosVect3.length();
    sumPosition.z /= lastPosVect3.length();
    resultSumPosition3 = sumPosition;



    update();
}

void DisplayWidget::mousePressEvent(QMouseEvent *mo)
{
    mousePos = mo->pos();
}

void DisplayWidget::mouseMoveEvent(QMouseEvent *mo)
{
    XmouseRot += 0.1/ M_PI * (mo->pos().y()-mousePos.y());
    ZmouseRot += 0.1/ M_PI * (mo->pos().x()-mousePos.x());
    update();
}

void DisplayWidget::wheelEvent(QWheelEvent *event)
{
    imgZoom += event->angleDelta().ry() / 100;
    if (imgZoom>=-1) imgZoom = -1;
    update();
}

void DisplayWidget::updateImage()
{
    update();
}
