#include "cylindre.h"

Cylindre::Cylindre(float ep_cyl, float r_cyl, int nb_fac, TypeCylindre type,
                   float color_r, float color_g, float color_b,
                   float x_center, float y_center, float z_center)
{
    this->ep_cyl = ep_cyl;
    this->r_cyl = r_cyl;
    this->type = type;
    this->nb_fac = nb_fac;

    this->x_center = x_center;
    this->y_center = y_center;
    this->z_center = z_center;

    this->color_r = color_r;
    this->color_g = color_g;
    this->color_b = color_b;
}


void Cylindre::stockerSommets() {

    float angle_offset = 360.0f / nb_fac;
    float angle = 0.0f;
    float angle_radians = 0.0f;
    float angle_radians_suivant = 0.0f;
    float tex_offset = 1.0f / nb_fac;
    for (int i = 0 ; i < nb_fac ; ++i, angle += angle_offset) {
        angle_radians = radians(angle);
        angle_radians_suivant = radians(angle + angle_offset);
        float x = cos(angle_radians) * r_cyl;
        float y = sin(angle_radians) * r_cyl;
        float x2 = cos(angle_radians_suivant) * r_cyl;
        float y2 = sin(angle_radians_suivant) * r_cyl;

        sommets.append({x,  y,  0,
                        x2, y2, 0,
                        0,  0,  0});

        colors.append({1,1,1, 1,1,1, 1,1,1});

        tex_faces.append({(float)(0.5 + 0.5 * cos(angle_radians)), (float)(0.5 + 0.5 * sin(angle_radians)),
                         (float)(0.5 + 0.5 * cos(angle_radians_suivant)), (float)(0.5 + 0.5 * sin(angle_radians_suivant)),
                         0.5, 0.5});

        sommets_facettes.append({x,y,-ep_cyl / 2,
                                 x,y,ep_cyl / 2,
                                 x2,y2,ep_cyl / 2,

                                 x,y,-ep_cyl / 2,
                                 x2,y2,ep_cyl / 2,
                                 x2,y2,-ep_cyl / 2});

        colors_facettes.append({1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1});

        tex_facettes.append({0, 1.0f - tex_offset * i,
                             1, 1.0f - tex_offset * i,
                             1, 1.0f - tex_offset * (i+1),

                             0, 1.0f - tex_offset * i,
                             1, 1.0f - tex_offset * (i+1),
                             0, 1.0f - tex_offset * (i+1)});

    }

    for (int i = 0 ; i < nb_fac ; ++i) {

        //les vecteurs de la facette d'avant
        int i2 = (i == 0) ? nb_fac : i;
        QVector3D vGH(sommets_facettes[(i2-1)%nb_fac*18+3]-sommets_facettes[(i2-1)%nb_fac*18+0],
                        sommets_facettes[(i2-1)%nb_fac*18+4]-sommets_facettes[(i2-1)%nb_fac*18+1],
                        sommets_facettes[(i2-1)%nb_fac*18+5]-sommets_facettes[(i2-1)%nb_fac*18+2]);

        QVector3D vGB(sommets_facettes[(i2-1)%nb_fac*18+6]-sommets_facettes[(i2-1)%nb_fac*18+0],
                        sommets_facettes[(i2-1)%nb_fac*18+7]-sommets_facettes[(i2-1)%nb_fac*18+1],
                        sommets_facettes[(i2-1)%nb_fac*18+8]-sommets_facettes[(i2-1)%nb_fac*18+2]);

        //les vecteurs de la facette en cours
        QVector3D vAB(sommets_facettes[i*18+3]-sommets_facettes[i*18+0],
                      sommets_facettes[i*18+4]-sommets_facettes[i*18+1],
                      sommets_facettes[i*18+5]-sommets_facettes[i*18+2]);

        QVector3D vAC(sommets_facettes[i*18+6]-sommets_facettes[i*18+0],
                      sommets_facettes[i*18+7]-sommets_facettes[i*18+1],
                      sommets_facettes[i*18+8]-sommets_facettes[i*18+2]);

        //les vecteurs de la facette d'après
        QVector3D vDC(sommets_facettes[(i+1)%nb_fac*18+3]-sommets_facettes[(i+1)%nb_fac*18+0],
                        sommets_facettes[(i+1)%nb_fac*18+4]-sommets_facettes[(i+1)%nb_fac*18+1],
                        sommets_facettes[(i+1)%nb_fac*18+5]-sommets_facettes[(i+1)%nb_fac*18+2]);

        QVector3D vDF(sommets_facettes[(i+1)%nb_fac*18+6]-sommets_facettes[(i+1)%nb_fac*18+0],
                        sommets_facettes[(i+1)%nb_fac*18+7]-sommets_facettes[(i+1)%nb_fac*18+1],
                        sommets_facettes[(i+1)%nb_fac*18+8]-sommets_facettes[(i+1)%nb_fac*18+2]);

        QVector3D vFace1(sommets[3]-sommets[0], sommets[4]-sommets[1], sommets[5]-sommets[2]);
        QVector3D vFace2(sommets[6]-sommets[0], sommets[7]-sommets[1], sommets[8]-sommets[2]);

        QVector3D nFace = QVector3D::normal(vFace1, vFace2);

        QVector3D nACB = QVector3D::normal(vAB, vAC);
        QVector3D nDFE = QVector3D::normal(vDC, vDF);
        QVector3D nGBA = QVector3D::normal(vGH, vGB);
        QVector3D nCD = -(nACB + nDFE) / 2.0;
        QVector3D nAB = -(nACB + nGBA) / 2.0;

        sommets_normales.append({nFace.x(), nFace.y(), nFace.z(),
                                 nFace.x(), nFace.y(), nFace.z(),
                                 nFace.x(), nFace.y(), nFace.z()});

        sommets_facettes_normales.append({nAB.x(), nAB.y(), nAB.z(),
                                          nAB.x(), nAB.y(), nAB.z(),
                                          nCD.x(), nCD.y(), nCD.z(),
                                          nAB.x(), nAB.y(), nAB.z(),
                                          nCD.x(), nCD.y(), nCD.z(),
                                          nCD.x(), nCD.y(), nCD.z()});
    }
}

QMatrix4x4 Cylindre::dessinerFacesCylindre(QMatrix4x4 currentMatrix, float z_translation, float m_alpha) {
    if (type == PARA) {
        currentMatrix.rotate(-m_alpha + degrees_delta, 0, 0, 1);
        currentMatrix.translate(x_center, y_center + z_translation, z_center);
        currentMatrix.rotate(45, 0, 1, 0);
        currentMatrix.rotate(90, 1, 0, 0);
    } else if (type == AXE_ROT) {
        currentMatrix.rotate(-m_alpha + degrees_delta, 0, 0, 1);
        currentMatrix.translate(x_center, y_center, z_center);
    } else if (type == BIELLE_G || type == BIELLE_D) {
        float xh = dist_gh * cos(radians(-m_alpha + 90 + degrees_delta));
        float yh = dist_gh * sin(radians(-m_alpha + 90 + degrees_delta));
        currentMatrix.translate(x_center, y_center, z_center);
        currentMatrix.rotate(180,0,1,0);
        currentMatrix.rotate(atan(yh / (std::abs(m_xj - xh))) * (180.0f / PI), 0, 0, 1);
    } else if (type == PISTON_G || type == PISTON_D) {
        currentMatrix.translate(x_center + z_translation, y_center, z_center);
        currentMatrix.rotate(90,0,1,0);
    } else if (type == AXE_FIXE){
        currentMatrix.translate(x_center, y_center, z_center + z_translation);
        currentMatrix.rotate(-m_alpha + degrees_delta,0,0,1);
    }
    return currentMatrix;

}

QMatrix4x4 Cylindre::dessinerFacettesCylindre(QMatrix4x4 currentMatrix, float m_alpha) {
    if (type == PARA) {
        currentMatrix.rotate(-m_alpha + degrees_delta, 0, 0, 1);
        currentMatrix.translate(x_center, y_center, z_center);
        currentMatrix.rotate(45, 0, 1, 0);
        currentMatrix.rotate(90, 1, 0, 0);
    } else if (type == AXE_ROT) {
        currentMatrix.rotate(-m_alpha + degrees_delta, 0, 0, 1);
        currentMatrix.translate(x_center, y_center, z_center);
    } else {
        currentMatrix.translate(x_center, y_center, z_center);
        if (type == AXE_FIXE) {
            currentMatrix.rotate(-m_alpha + degrees_delta,0,0,1);
        }
    }
    if (type == BIELLE_G) {
        float xh = dist_gh * cos(radians(-m_alpha + 90 + degrees_delta));
        float yh = dist_gh * sin(radians(-m_alpha + 90 + degrees_delta));
        if (std::abs(m_xj - xh) != 0)
            currentMatrix.rotate(atan(yh / (std::abs(m_xj - xh))) * (180.0f / PI), 0, 0, 1);
        currentMatrix.rotate(90, 0, 1, 0);
    } else if (type == BIELLE_D) {
        float xh = dist_gh * cos(radians(-m_alpha + 90 + degrees_delta));
        float yh = dist_gh * sin(radians(-m_alpha + 90 + degrees_delta));
        if (std::abs(m_xj - xh) != 0)
            currentMatrix.rotate(-atan(yh / (std::abs(m_xj - xh))) * (180.0f / PI), 0, 0, 1);
        currentMatrix.rotate(90, 0, 1, 0);
    } else if (type == PISTON_G || type == PISTON_D) {
        currentMatrix.rotate(90,0,1,0);
    }

    return currentMatrix;
}


float Cylindre::xj_const;
void Cylindre::miseAJourCylindreEnMouvement(float m_alpha) {
    if (type == BIELLE_G) {
        float xh = dist_gh * cos(radians(-m_alpha + 90 + degrees_delta));
        float yh = dist_gh * sin(radians(-m_alpha + 90 + degrees_delta));
        m_xj = xh - (sqrt(pow(dist_hj, 2) - pow(dist_gh * sin(radians(m_alpha)), 2)));
        xj_const = m_xj;
        x_center = xh + (m_xj - xh) / 2;
        y_center = yh / 2;
    } else if (type == BIELLE_D) {
        float xh = dist_gh * cos(radians(-m_alpha + 90 + degrees_delta));
        float yh = dist_gh * sin(radians(-m_alpha + 90 + degrees_delta));
        m_xj = -xj_const;
        x_center = xh + (m_xj - xh) / 2;
        y_center = yh / 2;
    } else if (type == PISTON_G) {
        x_center = xj_const - ep_cyl / 2 + 0.07f;
    } else if (type == PISTON_D) {
        x_center = -(xj_const - ep_cyl / 2 + 0.07f);
    }
}
