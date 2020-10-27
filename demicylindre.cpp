#include "demicylindre.h"

DemiCylindre::DemiCylindre(float ep_cyl, float r_int_cyl, float r_ext_cyl,
                           float color_r, float color_g, float color_b,
                           float x_center, float y_center, float z_center)
{
    this->ep_cyl = ep_cyl;
    this->r_int_cyl = r_int_cyl;
    this->r_ext_cyl= r_ext_cyl;

    this->x_center = x_center;
    this->y_center = y_center;
    this->z_center = z_center;

    this->color_r = color_r;
    this->color_g = color_g;
    this->color_b = color_b;
}

void DemiCylindre::stockerSommets() {

    float angle_offset = 360.0f / nb_fac;
    float angle = 0.0f;
    float angle_radians = 0.0f;
    float angle_radians_suivant = 0.0f;
    float tex_offset = 1.0f / (nb_fac / 2);
    float int_tex_offset = (r_int_cyl / r_ext_cyl) * 0.5;
    float ext_tex_offset = 0.5;
    for (int i = 0 ; i < nb_fac / 2 ; ++i, angle -= angle_offset) {
        angle_radians = radians(angle);
        angle_radians_suivant = radians(angle - angle_offset);

        float x_int = cos(angle_radians) * r_int_cyl;
        float y_int = sin(angle_radians) * r_int_cyl;

        float x_ext = cos(angle_radians) * r_ext_cyl;
        float y_ext = sin(angle_radians) * r_ext_cyl;

        float x_int2 = cos(angle_radians_suivant) * r_int_cyl;
        float y_int2 = sin(angle_radians_suivant) * r_int_cyl;

        float x_ext2 = cos(angle_radians_suivant) * r_ext_cyl;
        float y_ext2 = sin(angle_radians_suivant) * r_ext_cyl;

        sommets_faces.append({x_int,  y_int,  0,
                              x_ext2, y_ext2, 0,
                              x_ext,  y_ext,  0,

                              x_int,  y_int,  0,
                              x_int2, y_int2, 0,
                              x_ext2, y_ext2, 0});
        colors_faces.append({1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1});

        tex_faces.append({(float)(0.5 + int_tex_offset * cos(angle_radians)),         (float)(0.5 + int_tex_offset * sin(angle_radians)),
                          (float)(0.5 + ext_tex_offset * cos(angle_radians_suivant)), (float)(0.5 + ext_tex_offset * sin(angle_radians_suivant)),
                          (float)(0.5 + ext_tex_offset * cos(angle_radians)),         (float)(0.5 + ext_tex_offset * sin(angle_radians)),

                          (float)(0.5 + int_tex_offset * cos(angle_radians)),         (float)(0.5 + int_tex_offset * sin(angle_radians)),
                          (float)(0.5 + int_tex_offset * cos(angle_radians_suivant)), (float)(0.5 + int_tex_offset * sin(angle_radians_suivant)),
                          (float)(0.5 + ext_tex_offset * cos(angle_radians_suivant)), (float)(0.5 + ext_tex_offset * sin(angle_radians_suivant))});


        sommets_int.append({x_int,  y_int,  -ep_cyl/2,
                            x_int,  y_int,  ep_cyl/2,
                            x_int2, y_int2, -ep_cyl/2,

                            x_int,  y_int,  ep_cyl/2,
                            x_int2, y_int2, ep_cyl/2,
                            x_int2, y_int2, -ep_cyl/2});

        colors_int.append({1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1});

        tex_int.append({0, 1.0f - tex_offset * i,
                        1, 1.0f - tex_offset * i,
                        0, 1.0f - tex_offset * (i+1),

                        1, 1.0f - tex_offset * i,
                        1, 1.0f - tex_offset * (i+1),
                        0, 1.0f - tex_offset * (i+1)});

        sommets_ext.append({x_ext,  y_ext,  -ep_cyl/2,
                            x_ext,  y_ext,  ep_cyl/2,
                            x_ext2, y_ext2, -ep_cyl/2,

                            x_ext,  y_ext,  ep_cyl/2,
                            x_ext2, y_ext2, ep_cyl/2,
                            x_ext2, y_ext2, -ep_cyl/2});

        colors_ext.append({1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1});

        tex_ext.append({0, 1.0f - tex_offset * i,
                        1, 1.0f - tex_offset * i,
                        0, 1.0f - tex_offset * (i+1),

                        1, 1.0f - tex_offset * i,
                        1, 1.0f - tex_offset * (i+1),
                        0, 1.0f - tex_offset * (i+1)});

        if (i == 0 || i == (nb_fac / 2 - 1)) {
            if (i == 0) {
                sommets_quads.append({x_int, y_int, -ep_cyl/2,
                                     x_ext,  y_ext, -ep_cyl/2,
                                     x_ext,  y_ext, ep_cyl/2,

                                     x_int,  y_int, -ep_cyl/2,
                                     x_ext,  y_ext, ep_cyl/2,
                                     x_int,  y_int, ep_cyl/2});
            } else {
                sommets_quads.append({x_int2, y_int2, -ep_cyl/2,
                                     x_ext2,  y_ext2, -ep_cyl/2,
                                     x_ext2,  y_ext2, ep_cyl/2,

                                     x_int2,  y_int2, -ep_cyl/2,
                                     x_ext2,  y_ext2, ep_cyl/2,
                                     x_int2,  y_int2, ep_cyl/2});
            }

            colors_quads.append({1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1});

            tex_quads.append({0,0, 0,0.5, 1,0.5,
                              0,0, 1,0.5, 1,0});
        }
    }

    QVector3D vFace1(sommets_faces[3]-sommets_faces[0], sommets_faces[4]-sommets_faces[1], sommets_faces[5]-sommets_faces[2]);
    QVector3D vFace2(sommets_faces[6]-sommets_faces[0], sommets_faces[7]-sommets_faces[1], sommets_faces[8]-sommets_faces[2]);

    QVector3D nFace = QVector3D::normal(vFace1, vFace2);
    sommets_faces_normales.append({nFace.x(), nFace.y(), nFace.z()});

    QVector3D vQuad1(sommets_quads[3]-sommets_quads[0], sommets_quads[4]-sommets_quads[1], sommets_quads[5]-sommets_quads[2]);
    QVector3D vQuad2(sommets_quads[6]-sommets_quads[0], sommets_quads[7]-sommets_quads[1], sommets_quads[8]-sommets_quads[2]);

    QVector3D nQuad = -QVector3D::normal(vQuad1, vQuad2);
    sommets_quads_normales.append({nQuad.x(), nQuad.y(), nQuad.z()});


    for (int i = 0 ; i < nb_fac / 2 ; ++i) {

        //les vecteurs de la facette d'avant
        int i2 = (i == 0) ? nb_fac / 2 : i;
        QVector3D vGH(sommets_int[(i2-1)%(nb_fac/2)*18+3]-sommets_int[(i2-1)%(nb_fac/2)*18+0],
                        sommets_int[(i2-1)%(nb_fac/2)*18+4]-sommets_int[(i2-1)%(nb_fac/2)*18+1],
                        sommets_int[(i2-1)%(nb_fac/2)*18+5]-sommets_int[(i2-1)%(nb_fac/2)*18+2]);

        QVector3D vGB(sommets_int[(i2-1)%(nb_fac/2)*18+6]-sommets_int[(i2-1)%(nb_fac/2)*18+0],
                        sommets_int[(i2-1)%(nb_fac/2)*18+7]-sommets_int[(i2-1)%(nb_fac/2)*18+1],
                        sommets_int[(i2-1)%(nb_fac/2)*18+8]-sommets_int[(i2-1)%(nb_fac/2)*18+2]);

        //les vecteurs de la facette en cours
        QVector3D vAB(sommets_int[i*18+3]-sommets_int[i*18+0],
                      sommets_int[i*18+4]-sommets_int[i*18+1],
                      sommets_int[i*18+5]-sommets_int[i*18+2]);

        QVector3D vAC(sommets_int[i*18+6]-sommets_int[i*18+0],
                      sommets_int[i*18+7]-sommets_int[i*18+1],
                      sommets_int[i*18+8]-sommets_int[i*18+2]);

        //les vecteurs de la facette d'après
        QVector3D vDC(sommets_int[(i+1)%(nb_fac/2)*18+3]-sommets_int[(i+1)%(nb_fac/2)*18+0],
                        sommets_int[(i+1)%(nb_fac/2)*18+4]-sommets_int[(i+1)%(nb_fac/2)*18+1],
                        sommets_int[(i+1)%(nb_fac/2)*18+5]-sommets_int[(i+1)%(nb_fac/2)*18+2]);

        QVector3D vDF(sommets_int[(i+1)%(nb_fac/2)*18+6]-sommets_int[(i+1)%(nb_fac/2)*18+0],
                        sommets_int[(i+1)%(nb_fac/2)*18+7]-sommets_int[(i+1)%(nb_fac/2)*18+1],
                        sommets_int[(i+1)%(nb_fac/2)*18+8]-sommets_int[(i+1)%(nb_fac/2)*18+2]);

        QVector3D nACB = QVector3D::normal(vAB, vAC);
        QVector3D nDFE = QVector3D::normal(vDC, vDF);
        QVector3D nGBA = QVector3D::normal(vGH, vGB);
        QVector3D nCD = -(nACB + nDFE) / 2.0;
        QVector3D nAB = -(nACB + nGBA) / 2.0;

        sommets_int_normales.append({nAB.x(), nAB.y(), nAB.z(),
                                    nAB.x(), nAB.y(), nAB.z(),
                                    nCD.x(), nCD.y(), nCD.z(),
                                    nAB.x(), nAB.y(), nAB.z(),
                                    nCD.x(), nCD.y(), nCD.z(),
                                    nCD.x(), nCD.y(), nCD.z()});

        sommets_ext_normales.append({-nAB.x(), -nAB.y(), -nAB.z(),
                                    -nAB.x(), -nAB.y(), -nAB.z(),
                                    -nCD.x(), -nCD.y(), -nCD.z(),
                                    -nAB.x(), -nAB.y(), -nAB.z(),
                                    -nCD.x(), -nCD.y(), -nCD.z(),
                                    -nCD.x(), -nCD.y(), -nCD.z()});

    }
}

QMatrix4x4 DemiCylindre::dessinerFace(QMatrix4x4 matrix, float z_translation) {
    matrix.translate(x_center + z_translation, y_center, z_center);
    matrix.rotate(90,0,1,0);
    return matrix;
}

QMatrix4x4 DemiCylindre::dessinerInterieur(QMatrix4x4 matrix) {
    matrix.translate(x_center, y_center, z_center);
    matrix.rotate(90,0,1,0);
    return matrix;
}
