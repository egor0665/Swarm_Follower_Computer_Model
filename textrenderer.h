#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <QFont>
#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>



class TextRenderer: protected QOpenGLExtraFunctions
{
    QOpenGLWidget *renderTextWidget;
public:
    TextRenderer();
    void init(QOpenGLWidget *renderTextWidget);

    void renderTextAbsolute(double x, double y, double z, QString text, QColor textColor, QFont textFont = QFont("Helvetica", 12, QFont::Bold));
    void renderText(double x, double y, double z, QString text, QColor textColor, QFont textFont = QFont("Helvetica", 12, QFont::Bold));

protected:
    void transformPoint(GLdouble out[], const GLdouble m[], const GLdouble in[]);
    GLint project(GLdouble objx, GLdouble objy, GLdouble objz, const GLdouble model[], const GLdouble proj[], const GLint viewport[], GLdouble *winx, GLdouble *winy, GLdouble *winz);
};

#endif // TEXTRENDERER_H
