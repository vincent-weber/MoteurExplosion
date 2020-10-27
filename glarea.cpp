#include "princ.h"
#include "glarea.h"
#include "type_cylindre.cpp"
#include <QDebug>
#include <QSurfaceFormat>

static const QString vertexShaderFile   = ":/vertex.glsl";
static const QString fragmentShaderFile = ":/fragment.glsl";

float Cylindre::dist_gh;
float Cylindre::dist_hj;

GLArea::GLArea(QWidget *parent) :
    QOpenGLWidget(parent)
{

    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    sf.setSamples(16);  // nb de sample par pixels : suréchantillonnage por l'antialiasing, en décalant à chaque fois le sommet
                        // cf https://www.khronos.org/opengl/wiki/Multisampling et https://stackoverflow.com/a/14474260
    setFormat(sf);

    setEnabled(true);  // événements clavier et souris
    setFocusPolicy(Qt::StrongFocus); // accepte focus
    setFocus();                      // donne le focus

    m_timer = new QTimer(this);
    m_timer->setInterval(50);  // msec
    connect (m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect (this, SIGNAL(radiusChanged(double)), this, SLOT(setRadius(double)));
    connect(this, SIGNAL(distCameraChanged(double)), this, SLOT(setDistCamera(double)));
    connect(this, SIGNAL(nearChanged(double)), this, SLOT(setNear(double)));
    connect(this, SIGNAL(farChanged(double)), this, SLOT(setFar(double)));
    connect(this, SIGNAL(angleChanged(double)), this, SLOT(setAngle(double)));
    connect(this, SIGNAL(vitesseChanged(double)), this, SLOT(setVitesse(double)));

    TypeCylindre type_axe_rotatif = AXE_ROT;
    TypeCylindre type_axe_fixe = AXE_FIXE;
    TypeCylindre type_para = PARA;
    TypeCylindre type_bielle_g = BIELLE_G;
    TypeCylindre type_bielle_d = BIELLE_D;
    TypeCylindre type_piston_g = PISTON_G;
    TypeCylindre type_piston_d = PISTON_D;

    float r_axe = 0.15f;
    float r_int_demi_cyl = 0.5f;
    float r_ext_demi_cyl = r_int_demi_cyl * 1.2f;

    float ep_axes_rot = 0.25f;
    float ep_axes_fixes = ep_axes_rot * 0.75f;
    float ep_bielle = 2;
    float r_para = sqrt(2)*r_axe;
    float ep_para = 0.75f;

    float nb_fac_axes = 30;
    float nb_fac_bielles = 20;
    float nb_fac_pistons = 20;

    Cylindre axe(1.5f, r_axe, nb_fac_axes, type_axe_fixe,
                 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, -1.0f);
    Cylindre para_11(ep_para, r_para, 4, type_para,
                     0.0f, 1.0f, 0.0f,
                     0.0f, ep_para/2 - r_axe, axe.z_center - axe.ep_cyl/2 - r_axe);
    Cylindre axe_1(ep_axes_rot, r_axe, nb_fac_axes, type_axe_rotatif,
                   0.0f, 0.0f, 1.0f,
                   0.0f, ep_para - 2*r_axe, para_11.z_center - ep_axes_rot/2 - r_axe);
    Cylindre para_12 = para_11;
    para_12.z_center = para_11.z_center - 2*r_axe - ep_axes_rot;

    Cylindre axe_sep_1(ep_axes_fixes, r_axe, nb_fac_axes, type_axe_fixe,
                       1.0f, 0.0f, 0.0f,
                       0.0f, 0.0f, para_12.z_center - r_axe - ep_axes_fixes/2);
    Cylindre para_21 = para_12;
    para_21.z_center = axe_sep_1.z_center - r_axe - ep_axes_fixes/2;
    para_21.degrees_delta = 180.0f;
    Cylindre axe_2 = axe_1;
    axe_2.z_center = para_21.z_center - ep_axes_rot/2 - r_axe;
    axe_2.degrees_delta = 180.0f;
    Cylindre para_22 = para_21;
    para_22.z_center = para_21.z_center - 2*r_axe - ep_axes_rot;
    Cylindre axe_sep_2 = axe_sep_1;
    axe_sep_2.z_center = para_22.z_center - r_axe - ep_axes_fixes/2;
    Cylindre para_31 = para_22;
    para_31.z_center = axe_sep_2.z_center - r_axe - ep_axes_fixes/2;
    para_31.degrees_delta = 90.0f;
    Cylindre axe_3 = axe_2;
    axe_3.z_center = para_31.z_center - ep_axes_rot/2 - r_axe;
    axe_3.degrees_delta = 90.0f;
    Cylindre para_32 = para_31;
    para_32.z_center = para_31.z_center - 2*r_axe - ep_axes_rot;
    Cylindre axe_sep_3 = axe_sep_2;
    axe_sep_3.z_center = para_32.z_center - r_axe - ep_axes_fixes/2;
    Cylindre para_41 = para_32;
    para_41.z_center = axe_sep_3.z_center - r_axe - ep_axes_fixes/2;
    para_41.degrees_delta = 270.0f;
    Cylindre axe_4 = axe_3;
    axe_4.z_center = para_41.z_center - ep_axes_rot/2 - r_axe;
    axe_4.degrees_delta = 270.0f;
    Cylindre para_42 = para_41;
    para_42.z_center = para_41.z_center - 2*r_axe - ep_axes_rot;
    Cylindre axe_sep_4 = axe_sep_3;
    axe_sep_4.z_center = para_42.z_center - r_axe - ep_axes_fixes/2;

    Cylindre::dist_gh = ep_para - 2*r_axe;
    Cylindre::dist_hj = ep_bielle;

    Cylindre bielle_1(ep_bielle, r_axe, nb_fac_bielles, type_bielle_g,
                      0.55f, 0.27f, 0.68f,
                      0,0, axe_1.z_center);
    bielle_1.degrees_delta = axe_1.degrees_delta;
    Cylindre bielle_2 = bielle_1;
    bielle_2.type = type_bielle_d;
    bielle_2.z_center = axe_2.z_center;
    bielle_2.degrees_delta = axe_2.degrees_delta;
    Cylindre bielle_3 = bielle_1;
    bielle_3.z_center = axe_3.z_center;
    bielle_3.degrees_delta = axe_3.degrees_delta;
    Cylindre bielle_4 = bielle_2;
    bielle_4.z_center = axe_4.z_center;
    bielle_4.degrees_delta = axe_4.degrees_delta;
    Cylindre piston_1(ep_bielle, r_int_demi_cyl, nb_fac_pistons, type_piston_g,
                      0.75f, 0.75f, 0.75f,
                      0,0,bielle_1.z_center);
    piston_1.m_xj = bielle_1.m_xj;
    Cylindre piston_2 = piston_1;
    piston_2.type = type_piston_d;
    piston_2.z_center = bielle_2.z_center;
    Cylindre piston_3 = piston_1;
    piston_3.z_center = bielle_3.z_center;
    Cylindre piston_4 = piston_2;
    piston_4.z_center = bielle_4.z_center;

    DemiCylindre cyl_piston_1(ep_bielle + Cylindre::dist_gh*2, r_int_demi_cyl, r_ext_demi_cyl,
                              0.25f, 0.25f, 0.25f,
                              -Cylindre::dist_hj + 0.12f - piston_1.ep_cyl/2, piston_1.y_center, piston_1.z_center);
    DemiCylindre cyl_piston_2(ep_bielle + Cylindre::dist_gh*2, r_int_demi_cyl, r_ext_demi_cyl,
                              0.25f, 0.25f, 0.25f,
                              Cylindre::dist_hj - 0.12f + piston_2.ep_cyl/2, piston_2.y_center, piston_2.z_center);
    DemiCylindre cyl_piston_3(ep_bielle + Cylindre::dist_gh*2, r_int_demi_cyl, r_ext_demi_cyl,
                              0.25f, 0.25f, 0.25f,
                              -Cylindre::dist_hj + 0.12f - piston_3.ep_cyl/2, piston_3.y_center, piston_3.z_center);
    DemiCylindre cyl_piston_4(ep_bielle + Cylindre::dist_gh*2, r_int_demi_cyl, r_ext_demi_cyl,
                              0.25f, 0.25f, 0.25f,
                              Cylindre::dist_hj - 0.12f + piston_4.ep_cyl/2, piston_4.y_center, piston_4.z_center);
    cylindres.push_back(axe);
    cylindres.push_back(para_11);
    cylindres.push_back(axe_1);
    cylindres.push_back(para_12);
    cylindres.push_back(axe_sep_1);
    cylindres.push_back(para_21);
    cylindres.push_back(axe_2);
    cylindres.push_back(para_22);
    cylindres.push_back(axe_sep_2);
    cylindres.push_back(para_31);
    cylindres.push_back(axe_3);
    cylindres.push_back(para_32);
    cylindres.push_back(axe_sep_3);
    cylindres.push_back(para_41);
    cylindres.push_back(axe_4);
    cylindres.push_back(para_42);
    cylindres.push_back(axe_sep_4);

    cylindres.push_back(bielle_1);
    cylindres.push_back(bielle_2);
    cylindres.push_back(piston_1);
    cylindres.push_back(piston_2);
    cylindres.push_back(bielle_3);
    cylindres.push_back(bielle_4);
    cylindres.push_back(piston_3);
    cylindres.push_back(piston_4);

    demi_cylindres.push_back(cyl_piston_1);
    demi_cylindres.push_back(cyl_piston_2);
    demi_cylindres.push_back(cyl_piston_3);
    demi_cylindres.push_back(cyl_piston_4);
}

void GLArea::makeGLObjects() {

    for (Cylindre &cylindre : cylindres) {
        cylindre.stockerSommets();
        for (int i = 0, j = 0 ; i < cylindre.sommets.size() ; i += 3, j += 2) {
            vertData.append(cylindre.sommets[i+0]);
            vertData.append(cylindre.sommets[i+1]);
            vertData.append(cylindre.sommets[i+2]);

            vertData.append(cylindre.colors[i+0]);
            vertData.append(cylindre.colors[i+1]);
            vertData.append(cylindre.colors[i+2]);
            vertData.append(cylindre.tex_faces[j+0]);
            vertData.append(cylindre.tex_faces[j+1]);

            vertData.append(cylindre.sommets_normales[i+0]);
            vertData.append(cylindre.sommets_normales[i+1]);
            vertData.append(cylindre.sommets_normales[i+2]);
        }

        for (int i = 0, j = 0 ; i < cylindre.sommets_facettes.size() ; i += 3, j += 2) {

            vertData.append(cylindre.sommets_facettes[i+0]);
            vertData.append(cylindre.sommets_facettes[i+1]);
            vertData.append(cylindre.sommets_facettes[i+2]);

            vertData.append(cylindre.colors_facettes[i+0]);
            vertData.append(cylindre.colors_facettes[i+1]);
            vertData.append(cylindre.colors_facettes[i+2]);
            vertData.append(cylindre.tex_facettes[j+0]);
            vertData.append(cylindre.tex_facettes[j+1]);

            vertData.append(cylindre.sommets_facettes_normales[i+0]);
            vertData.append(cylindre.sommets_facettes_normales[i+1]);
            vertData.append(cylindre.sommets_facettes_normales[i+2]);
        }
    }

    for (DemiCylindre &demi_cyl : demi_cylindres) {
        demi_cyl.stockerSommets();
        for (int i = 0, j = 0 ; i < demi_cyl.sommets_faces.size() ; i += 3, j += 2) {
            vertData.append(demi_cyl.sommets_faces[i+0]);
            vertData.append(demi_cyl.sommets_faces[i+1]);
            vertData.append(demi_cyl.sommets_faces[i+2]);

            vertData.append(demi_cyl.colors_faces[i+0]);
            vertData.append(demi_cyl.colors_faces[i+1]);
            vertData.append(demi_cyl.colors_faces[i+2]);
            vertData.append(demi_cyl.tex_faces[j+0]);
            vertData.append(demi_cyl.tex_faces[j+1]);

            vertData.append(demi_cyl.sommets_faces_normales[0]);
            vertData.append(demi_cyl.sommets_faces_normales[1]);
            vertData.append(demi_cyl.sommets_faces_normales[2]);
        }

        for (int i = 0, j = 0 ; i < demi_cyl.sommets_ext.size() ; i += 3, j += 2) {
            vertData.append(demi_cyl.sommets_ext[i+0]);
            vertData.append(demi_cyl.sommets_ext[i+1]);
            vertData.append(demi_cyl.sommets_ext[i+2]);

            vertData.append(demi_cyl.colors_ext[i+0]);
            vertData.append(demi_cyl.colors_ext[i+1]);
            vertData.append(demi_cyl.colors_ext[i+2]);
            vertData.append({demi_cyl.tex_ext[j+0]});
            vertData.append({demi_cyl.tex_ext[j+1]});

            vertData.append(demi_cyl.sommets_ext_normales[i+0]);
            vertData.append(demi_cyl.sommets_ext_normales[i+1]);
            vertData.append(demi_cyl.sommets_ext_normales[i+2]);
        }
        for (int i = 0, j = 0 ; i < demi_cyl.sommets_quads.size() ; i += 3, j += 2) {
            vertData.append(demi_cyl.sommets_quads[i+0]);
            vertData.append(demi_cyl.sommets_quads[i+1]);
            vertData.append(demi_cyl.sommets_quads[i+2]);

            vertData.append(demi_cyl.colors_quads[i+0]);
            vertData.append(demi_cyl.colors_quads[i+1]);
            vertData.append(demi_cyl.colors_quads[i+2]);
            vertData.append({demi_cyl.tex_quads[j+0]});
            vertData.append({demi_cyl.tex_quads[j+1]});

            vertData.append(demi_cyl.sommets_quads_normales[0]);
            vertData.append(demi_cyl.sommets_quads_normales[1]);
            vertData.append(demi_cyl.sommets_quads_normales[2]);
        }
    }
    QString nom = QString(":/texture-metal.png");
    QImage image(nom);
    if (image.isNull())
        qDebug() << "load image" << nom << "failed";
    else qDebug() << "load image" << nom << image.size();
    m_texture = new QOpenGLTexture(image);

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
    m_vbo.release();
}

void GLArea::tearGLObjects() {
    m_vbo.destroy();
}

GLArea::~GLArea()
{
    delete m_timer;

    // Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
    // dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
    makeCurrent();
    tearGLObjects();
    doneCurrent();
}

void GLArea::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    // shaders
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFile);  // compile
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFile);
    if (! m_program->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << m_program->log();
    }

    // récupère identifiants de "variables" dans les shaders
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_norAttr = m_program->attributeLocation("norAttr");
    m_texAttr = m_program->attributeLocation("texAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");

    makeGLObjects();
}

void GLArea::resizeGL(int w, int h)
{
    // C'est fait par défaut
    glViewport(0, 0, w, h);

    m_ratio = (double) w / h;
}

void GLArea::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind(); // active le shader program

    QMatrix4x4 proj_mat;
    GLfloat hr = m_radius, wr = hr * m_ratio;
    proj_mat.frustum(-wr, wr, -hr, hr, m_near, m_far);

    QMatrix4x4 cam_mat;
    cam_mat.translate(-m_dist_camera_x, -m_dist_camera_y, -m_dist_camera_z);

    QMatrix4x4 world_mat;
    world_mat.translate(0,0,-3);
    world_mat.rotate(m_angle_y, 0, 1, 0);
    world_mat.rotate(m_angle_x, 1, 0, 0);
    world_mat.translate(0,0,3);

    QMatrix3x3 normal_mat = world_mat.normalMatrix();

    m_program->setUniformValue("projMatrix", proj_mat);
    m_program->setUniformValue("mvMatrix", cam_mat*world_mat);
    m_program->setUniformValue("norMatrix", normal_mat);

    QMatrix4x4 base_world_mat = world_mat;

    //utilisation d'un VAA pour l'intérieur du cylindre uniquement pour le changement de couleur (rouge)
    m_texture->bind();
    bool decalage = false;
    int cnt = 0;
    for (DemiCylindre& demi_cyl : demi_cylindres) {
        if(cnt == 2) decalage = true;
        world_mat = demi_cyl.dessinerInterieur(base_world_mat);
        normal_mat = world_mat.normalMatrix();
        m_program->setUniformValue("norMatrix", normal_mat);
        m_program->setUniformValue("mvMatrix", cam_mat * world_mat);
        glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, demi_cyl.sommets_int.constData());
        QVector<GLfloat> colors_int;
        float color = m_alpha <= 180 ? 0.25 + (m_alpha / 180) * 0.5 : 0.75 - ((m_alpha-180) / 180) * 0.5;
        if (decalage) {
            float alpha_comp = (m_alpha > 90) ? m_alpha - 90 : m_alpha + 270;
            color = alpha_comp <= 180 ? 0.25 + ((alpha_comp) / 180) * 0.5 : 0.75 - ((alpha_comp-180) / 180) * 0.5;
        }

        for (int i = 0 ; i < 60 ; ++i)
            colors_int.append({color, 0.15, 0.15});
        glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors_int.constData());
        glVertexAttribPointer(m_texAttr, 2, GL_FLOAT, GL_FALSE, 0, demi_cyl.tex_int.constData());

        glEnableVertexAttribArray(m_posAttr);  // rend le VAA accessible pour glDrawArrays
        glEnableVertexAttribArray(m_colAttr);
        glEnableVertexAttribArray(m_texAttr);

        glDrawArrays(GL_TRIANGLES, 0, 60);

        glDisableVertexAttribArray(m_posAttr);
        glDisableVertexAttribArray(m_colAttr);
        glDisableVertexAttribArray(m_texAttr);
        ++cnt;
    }

    m_vbo.bind();

    m_program->setAttributeBuffer(m_posAttr, GL_FLOAT, 0 * sizeof(GLfloat), 3, 11 * sizeof(GLfloat));
    m_program->setAttributeBuffer(m_colAttr, GL_FLOAT, 3 * sizeof(GLfloat), 3, 11 * sizeof(GLfloat));
    m_program->setAttributeBuffer(m_texAttr, GL_FLOAT, 6 * sizeof(GLfloat), 2, 11 * sizeof(GLfloat));
    m_program->setAttributeBuffer(m_norAttr, GL_FLOAT, 8 * sizeof(GLfloat), 3, 11 * sizeof(GLfloat));

    m_program->enableAttributeArray(m_posAttr);  // rend le VBO accessible pour glDrawArrays
    m_program->enableAttributeArray(m_colAttr);
    m_program->enableAttributeArray(m_texAttr);
    m_program->enableAttributeArray(m_norAttr);

    int offset = 0;
    for (Cylindre &cylindre : cylindres) {
        cylindre.miseAJourCylindreEnMouvement(m_alpha);

        world_mat = cylindre.dessinerFacesCylindre(base_world_mat, -cylindre.ep_cyl / 2, m_alpha);
        normal_mat = world_mat.normalMatrix();
        m_program->setUniformValue("norMatrix", normal_mat);
        m_program->setUniformValue("mvMatrix", cam_mat * world_mat);

        glDrawArrays(GL_TRIANGLES, offset, cylindre.nb_fac * 3);

        world_mat = cylindre.dessinerFacesCylindre(base_world_mat, cylindre.ep_cyl / 2, m_alpha);
        normal_mat = world_mat.normalMatrix();
        m_program->setUniformValue("norMatrix", normal_mat);
        m_program->setUniformValue("mvMatrix", cam_mat * world_mat);

        glDrawArrays(GL_TRIANGLES, offset, cylindre.nb_fac * 3);

        world_mat = cylindre.dessinerFacettesCylindre(base_world_mat, m_alpha);
        normal_mat = world_mat.normalMatrix();
        m_program->setUniformValue("norMatrix", normal_mat);
        m_program->setUniformValue("mvMatrix", cam_mat * world_mat);

        int offsetSup = offset + cylindre.nb_fac * 3;
        glDrawArrays(GL_TRIANGLES, offsetSup, cylindre.nb_fac * 6);
        m_program->setUniformValue("mvMatrix", cam_mat * base_world_mat);

        offset += cylindre.nb_fac * 9;
    }

    for (DemiCylindre &demi_cyl : demi_cylindres) {
        world_mat = demi_cyl.dessinerFace(base_world_mat, demi_cyl.ep_cyl / 2);
        normal_mat = world_mat.normalMatrix();
        m_program->setUniformValue("norMatrix", normal_mat);
        m_program->setUniformValue("mvMatrix", cam_mat * world_mat);
        glDrawArrays(GL_TRIANGLES, offset, demi_cyl.nb_fac * 3);

        world_mat = demi_cyl.dessinerFace(base_world_mat, -demi_cyl.ep_cyl / 2);
        normal_mat = world_mat.normalMatrix();
        m_program->setUniformValue("norMatrix", normal_mat);
        m_program->setUniformValue("mvMatrix", cam_mat * world_mat);
        glDrawArrays(GL_TRIANGLES, offset, demi_cyl.nb_fac * 3);

        world_mat = demi_cyl.dessinerInterieur(base_world_mat);
        normal_mat = world_mat.normalMatrix();
        m_program->setUniformValue("norMatrix", normal_mat);
        m_program->setUniformValue("mvMatrix", cam_mat * world_mat);
        glDrawArrays(GL_TRIANGLES, offset +  demi_cyl.nb_fac * 3, demi_cyl.nb_fac * 3);
        glDrawArrays(GL_TRIANGLES, offset +  demi_cyl.nb_fac * 6, 12);

        m_program->setUniformValue("mvMatrix", cam_mat * base_world_mat);
        offset += demi_cyl.nb_fac * 6 + 12;
    }

    m_program->disableAttributeArray(m_posAttr);
    m_program->disableAttributeArray(m_colAttr);
    m_program->disableAttributeArray(m_texAttr);
    m_program->disableAttributeArray(m_norAttr);

    m_vbo.release();
    m_texture->release();
    m_program->release();
}

void GLArea::keyPressEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();

    switch(ev->key()) {
        case Qt::Key_G :
        case Qt::Key_Space :
            m_angle_y += 5;
            if (m_angle_y >= 360) m_angle_y -= 360;
            update();
            break;
        case Qt::Key_J :
            m_angle_y -= 5;
            if (m_angle_y < 0) m_angle_y += 360;
            update();
            break;
        case Qt::Key_Y :
            m_angle_x += 5;
            if (m_angle_x >= 360) m_angle_x -= 360;
            update();
            break;
        case Qt::Key_H :
            m_angle_x -= 5;
            if (m_angle_x < 0) m_angle_x += 360;
            update();
            break;
        case Qt::Key_Down :
            m_dist_camera_y -= 0.05;
            update();
            break;
        case Qt::Key_Up :
            m_dist_camera_y += 0.05;
            update();
            break;
        case Qt::Key_Left :
            m_dist_camera_x -= 0.05;
            update();
            break;
        case Qt::Key_Right :
            m_dist_camera_x += 0.05;
            update();
            break;
        case Qt::Key_T :
            if (m_timer->isActive())
                m_timer->stop();
            else m_timer->start();
            break;
        case Qt::Key_D :
            if (!m_timer->isActive())
                m_timer->start();
            break;
        case Qt::Key_S :
            if (m_timer->isActive())
                m_timer->stop();
            break;
        case Qt::Key_A :
            if (ev->text() == "a") {
                --m_alpha_inc;
            } else {
                ++m_alpha_inc;
            }
            break;
    }
}

void GLArea::keyReleaseEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();
}

void GLArea::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void GLArea::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void GLArea::mouseMoveEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y();
}

void GLArea::onTimeout()
{
    m_alpha += m_alpha_inc;
    if (m_alpha >= 360) m_alpha = 0;
    update();
}

void GLArea::setRadius(double radius)
{
    if (radius != m_radius && radius > 0.01 && radius <= 10) {
        m_radius = radius;
        update();
    }
}

void GLArea::setDistCamera(double newDistCamera) {
    if (newDistCamera != m_dist_camera_z && newDistCamera > 1.0 && newDistCamera <= 10) {
        m_dist_camera_z = newDistCamera;
        update();
    }
}

void GLArea::setNear(double near) {
    if (near != m_near && near > 1.0 && near<= 10) {
        m_near = near;
        //emit nearChanged(near);
        update();
    }
}

void GLArea::setFar(double far) {
    if (far != m_far && far > 1.0 && far<= 10) {
        m_far = far;
        //emit farChanged(far);
        update();
    }
}

void GLArea::setAngle(double angle) {
    if (angle != m_angle_y && angle > 0.1 && angle <= 10) {
        m_angle_y = angle * 36;
        //emit angleChanged(angle);
        update();
    }
}

void GLArea::setVitesse(double vitesse) {
    if (vitesse != m_alpha_inc && vitesse > 0 && vitesse <= 25) {
        m_alpha_inc = vitesse;
        //emit angleChanged(angle);
        update();
    }
}

void GLArea::onButArreter() {
    if (m_timer->isActive())
        m_timer->stop();
}

void GLArea::onButDemarrer() {
    if (!m_timer->isActive())
        m_timer->start();
}
