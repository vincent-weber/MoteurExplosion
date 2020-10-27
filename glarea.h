#ifndef GLAREA_H
#define GLAREA_H

#include "cylindre.h"
#include "demicylindre.h"
#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>


class GLArea : public QOpenGLWidget,
               protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLArea(QWidget *parent = 0);
    ~GLArea();

public slots:
    void setRadius(double radius);
    void setDistCamera(double dist_camera);
    void setNear(double near);
    void setFar(double far);
    void setAngle(double angle);
    void setVitesse(double vitesse);

    void onButArreter();
    void onButDemarrer();

signals:  // On ne les implémente pas, elles seront générées par MOC ;
          // les paramètres seront passés aux slots connectés.
    void radiusChanged(double newRadius);
    void distCameraChanged(double newDistCamera);
    void nearChanged(double newNear);
    void farChanged(double newFar);
    void angleChanged(double newAngle);
    void vitesseChanged(double newVitesse);

protected slots:
    void onTimeout();

protected:
    void initializeGL() override;
    void doProjection();
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *ev) override;
    void keyReleaseEvent(QKeyEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;

    float radians(float angle_deg);

private:
    float m_angle_x = 0;
    float m_angle_y = 0;
    QTimer *m_timer = nullptr;
    double m_anim = 0;

    double m_radius = 0.5;
    double m_dist_camera_z = 3.0;
    double m_dist_camera_x = 0.0;
    double m_dist_camera_y = 0.5;
    double m_near = 1.0;
    double m_far = 20.0;

    double m_ratio = 1;
    float m_alpha = 0;
    float m_alpha_inc = 1;

    std::vector<Cylindre> cylindres;
    std::vector<DemiCylindre> demi_cylindres;

    // Pour utiliser les shaders
    QOpenGLShaderProgram *m_program;
    int m_posAttr;
    int m_colAttr;
    int m_norAttr;
    int m_texAttr;

    int m_matrixUniform;
    QOpenGLTexture *m_texture;
    QVector<GLfloat> vertData;

    QOpenGLBuffer m_vbo;
    void makeGLObjects();
    void tearGLObjects();


};

#endif // GLAREA_H
