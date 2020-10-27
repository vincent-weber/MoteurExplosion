#ifndef DEMICYLINDRE_H
#define DEMICYLINDRE_H

#include "math.h"
#include "util.h"
#include <QVector>
#include <QOpenGLFunctions>
#include <QDebug>
#include <QMatrix4x4>

class DemiCylindre
{
public:
    float r_int_cyl;
    float r_ext_cyl;
    static const int nb_fac = 20;
    float ep_cyl;

    float x_center;
    float y_center;
    float z_center;

    float color_r;
    float color_g;
    float color_b;
    DemiCylindre(float ep_cyl, float r_int_cyl, float r_ext_cyl,
                 float color_r = 0.0f, float color_g = 0.0f, float color_b = 0.0f,
                 float x_center = 0, float y_center = 0, float z_center = 0);

    QVector<GLfloat> sommets_faces;
    QVector<GLfloat> colors_faces;
    QVector<GLfloat> tex_faces;

    QVector<GLfloat> sommets_int;
    QVector<GLfloat> colors_int;
    QVector<GLfloat> tex_int;

    QVector<GLfloat> sommets_ext;
    QVector<GLfloat> colors_ext;
    QVector<GLfloat> tex_ext;

    QVector<GLfloat> sommets_quads;
    QVector<GLfloat> colors_quads;
    QVector<GLfloat> tex_quads;

    QVector<GLfloat> sommets_faces_normales;
    QVector<GLfloat> sommets_int_normales;
    QVector<GLfloat> sommets_ext_normales;
    QVector<GLfloat> sommets_quads_normales;


    void stockerSommets();
    QMatrix4x4 dessinerFace(QMatrix4x4 matrix, float z_translation);
    QMatrix4x4 dessinerInterieur(QMatrix4x4 matrix);
};

#endif // DEMICYLINDRE_H
