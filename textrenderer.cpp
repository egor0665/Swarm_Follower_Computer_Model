#include "textrenderer.h"
#include <QDebug>
#include <QPainter>

TextRenderer::TextRenderer()
{

}

void TextRenderer::init(QOpenGLWidget *renderTextWidget)
{
    initializeOpenGLFunctions();
    this->renderTextWidget = renderTextWidget;
}

void TextRenderer::renderTextAbsolute(double x, double y, double z, QString text, QColor textColor, QFont textFont)
{
    QPainter painter(renderTextWidget);
    painter.setPen(textColor);
    painter.setFont(textFont);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.drawText(x, y, text);
    painter.end();
}

void TextRenderer::renderText(double x, double y, double z, QString text, QColor textColor, QFont textFont)
{

    GLdouble model[4][4], proj[4][4];
    GLint view[4];
    glGetDoublev(GL_MODELVIEW_MATRIX, &model[0][0]);
    glGetDoublev(GL_PROJECTION_MATRIX, &proj[0][0]);
    glGetIntegerv(GL_VIEWPORT, &view[0]);
    GLdouble textPosX = 0, textPosY = 0, textPosZ = 0;

    project(x, y, z,
                &model[0][0], &proj[0][0], &view[0],
                &textPosX, &textPosY, &textPosZ);
    if (textPosZ <= 1)
    {
//    if (text == "М. 0")
//        qDebug()<< x <<" "<< y <<" "<< z <<" "<< textPosX <<" "<< textPosY <<" "<< textPosZ <<" ";

        textPosY = renderTextWidget->height() - textPosY; // y is inverted

        QPainter painter(renderTextWidget);
        painter.setPen(textColor);
        painter.setFont(textFont);

        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        painter.drawText(textPosX, textPosY, text); // z = pointT4.z + distOverOp / 4
        painter.end();
    }
}

inline GLint TextRenderer::project(GLdouble objx, GLdouble objy, GLdouble objz,
    const GLdouble model[16], const GLdouble proj[16],
    const GLint viewport[4],
    GLdouble * winx, GLdouble * winy, GLdouble * winz)
{
    GLdouble in[4], out[4];

    in[0] = objx;
    in[1] = objy;
    in[2] = objz;
    in[3] = 1.0;
    transformPoint(out, model, in);
    transformPoint(in, proj, out);

    if (in[3] == 0.0)
        return GL_FALSE;

    in[0] /= in[3];
    in[1] /= in[3];
    in[2] /= in[3];

    *winx = viewport[0] + (1 + in[0]) * viewport[2] / 2;
    *winy = viewport[1] + (1 + in[1]) * viewport[3] / 2;

    *winz = (1 + in[2]) / 2;
    return GL_TRUE;
}

inline void TextRenderer::transformPoint(GLdouble out[4], const GLdouble m[16], const GLdouble in[4])
{
#define M(row,col)  m[col*4+row]
    out[0] =
        M(0, 0) * in[0] + M(0, 1) * in[1] + M(0, 2) * in[2] + M(0, 3) * in[3];
    out[1] =
        M(1, 0) * in[0] + M(1, 1) * in[1] + M(1, 2) * in[2] + M(1, 3) * in[3];
    out[2] =
        M(2, 0) * in[0] + M(2, 1) * in[1] + M(2, 2) * in[2] + M(2, 3) * in[3];
    out[3] =
        M(3, 0) * in[0] + M(3, 1) * in[1] + M(3, 2) * in[2] + M(3, 3) * in[3];
#undef M
}

