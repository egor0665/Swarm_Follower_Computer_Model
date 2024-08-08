#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QOpenGLWidget>
#include<QVector>
#include<QVector2D>
#include<QVector3D>
#include<string>
#include<QFile>
#include<QDebug>

class ObjLoader
{
    struct face
    {
        struct vertex {
            GLuint v_i;
            GLuint vt_i;
            GLuint vn_i;
        };
        vertex v[3];
        face (vertex v1, vertex v2, vertex v3)
        {
            v[0]=v1;
            v[1]=v2;
            v[2]=v3;
        }
    };
    std::vector<std::string> coord;
    std::vector<QVector3D> vertex;
    std::vector<QVector2D> uvs;
    std::vector<QVector3D> normals;
    std::vector<face> faces;

    ObjLoader(){};
    ObjLoader(const ObjLoader &) = delete;
    ObjLoader(const ObjLoader &&) = delete;
    ObjLoader& operator=(const ObjLoader &) = delete;
    ObjLoader& operator=(const ObjLoader &&) = delete;
public:
    static ObjLoader& Instance()
    {
        static ObjLoader theSingleInstance;
        return theSingleInstance;
    }
    GLuint load(const QString &filename); //, GLint translateX = 0, GLint translateY = 0, GLint translateZ = 0
    GLuint draw(GLfloat offset = 0); //GLint translateX, GLint translateY, GLint translateZ,
};

#endif // OBJLOADER_H
