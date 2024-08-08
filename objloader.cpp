#include "objloader.h"

using std::vector;
using std::string;

GLuint ObjLoader::load(const QString &filename) //, GLint translateX, GLint translateY, GLint translateZ
{
    vector<string>().swap(coord);
    vector<QVector3D>().swap(vertex);
    vector<QVector2D>().swap(uvs);
    vector<QVector3D>().swap(normals);
    vector<face>().swap(faces);

    QFile in(filename);

    if(!in.open(QFile::ReadOnly | QFile::Text)){
        qDebug()<< "Не удалось открыть файл";
    }
    char buf[1024];
    while(!in.atEnd())
    {
        in.readLine(buf,1024);
        ObjLoader::coord.push_back(buf);
    }
    size_t N_coord = coord.size();
    for(size_t i=0;i<N_coord;++i)
    {
        if(coord[i][0]!='#')
        {
            if (coord[i][0] == 'v' && coord[i][1] == ' ')
            {
                float tmpx,tmpy,tmpz;
                sscanf(coord[i].c_str(),"v %f %f %f", &tmpx, &tmpy, &tmpz);
                vertex.push_back(QVector3D(tmpx,tmpy,tmpz));
            } else if (coord[i][0] == 'v' && coord[i][1] == 'n')
            {
                float tmpx,tmpy,tmpz;
                sscanf(coord[i].c_str(),"vn %f %f %f", &tmpx, &tmpy, &tmpz);
                normals.push_back(QVector3D(tmpx,tmpy,tmpz));
            } else if (coord[i][0] == 'v' && coord[i][1] == 't')
            {
                float tmpx,tmpy;
                sscanf(coord[i].c_str(),"vt %f %f", &tmpx, &tmpy);
                uvs.push_back(QVector2D(tmpx,tmpy));
            } else if (coord[i][0] == 'f')
            {
                face::vertex v1,v2,v3;
                sscanf(coord[i].c_str(),"f %i/%i/%i %i/%i/%i %i/%i/%i", &v1.v_i, &v1.vt_i, &v1.vn_i , &v2.v_i, &v2.vt_i, &v2.vn_i, &v3.v_i, &v3.vt_i, &v3.vn_i);
                faces.push_back(face(v1,v2,v3));
            }
        }
    }
    GLuint num = draw(); //translateX, translateY, translateZ
    return num;
}

GLuint ObjLoader::draw(GLfloat offset) // GLint translateX, GLint translateY, GLint translateZ,
{
    GLuint num = glGenLists(1);
//    glTranslatef(translateX,translateY,translateZ);
    glNewList(num,GL_COMPILE);
    size_t N_faces = faces.size();
    for (size_t i=0; i<N_faces; ++i)
    {
        face f = faces[i];
        glBegin(GL_TRIANGLES);

        QVector3D normal = normals[f.v[0].vn_i - 1];
        glNormal3f(normal.x(), normal.y(), normal.z());

        glTexCoord2f(uvs[f.v[0].vt_i-1].x()+offset, uvs[f.v[0].vt_i-1].y()+offset);
        glVertex3f(vertex[f.v[0].v_i-1].x(),vertex[f.v[0].v_i-1].y(),vertex[f.v[0].v_i-1].z());

        normal = normals[f.v[1].vn_i-1];
        glNormal3f(normal.x(), normal.y(), normal.z());

        glTexCoord2f(uvs[f.v[1].vt_i-1].x()+offset, uvs[f.v[1].vt_i-1].y()+offset);
        glVertex3f(vertex[f.v[1].v_i-1].x(),vertex[f.v[1].v_i-1].y(),vertex[f.v[1].v_i-1].z());

        normal = normals[f.v[2].vn_i-1];
        glNormal3f(normal.x(), normal.y(), normal.z());

        glTexCoord2f(uvs[f.v[2].vt_i-1].x()+offset, uvs[f.v[2].vt_i-1].y()+offset);
        glVertex3f(vertex[f.v[2].v_i-1].x(),vertex[f.v[2].v_i-1].y(),vertex[f.v[2].v_i-1].z());

        glEnd();

    }
    glEndList();
    return num;
}
