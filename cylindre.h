#ifndef CYLINDRE_H
#define CYLINDRE_H

#include "math.h"
#include "sens_rotation.cpp"
#include "type_cylindre.cpp"
#include "util.h"
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QVector3D>

class Cylindre
{
public:
    float ep_cyl;
    float r_cyl;

    float x_center;
    float y_center;
    float z_center;
    int nb_fac;

    float color_r;
    float color_g;
    float color_b;

    QVector<GLfloat> sommets;
    QVector<GLfloat> sommets_normales;
    QVector<GLfloat> sommets_facettes;
    QVector<GLfloat> sommets_facettes_normales;
    QVector<GLfloat> ind_nor;
    QVector<GLfloat> colors;
    QVector<GLfloat> colors_facettes;
    QVector<GLfloat> tex_faces;
    QVector<GLfloat> tex_facettes;

    TypeCylindre type;

    float degrees_delta = 0.0f;

    static float dist_gh;
    static float dist_hj;

    float m_yh;
    float m_xj;
    float m_xh;
    static float m_xk;

    static float xj_const;

    Cylindre(float ep_cyl, float r_cyl, int nb_fac, TypeCylindre type,
             float color_r = 0.0f, float color_g = 0.0f, float color_b = 0.0f,
             float x_center = 0, float y_center = 0, float z_center = 0);


    void stockerSommets();
    QMatrix4x4 dessinerFacesCylindre(QMatrix4x4 currentMatrix, float z_translation, float m_alpha);
    QMatrix4x4 dessinerFacettesCylindre(QMatrix4x4 currentMatrix, float m_alpha);
    void miseAJourCylindreEnMouvement(float m_alpha);

};

#endif // CYLINDRE_H
