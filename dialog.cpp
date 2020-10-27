#include "dialog.h"
#include "glarea.h"

Dialog::Dialog(QWidget *parent, GLArea* glarea) : QDialog(parent)
{
    this->glarea = glarea;
    setupUi(this);
    connect(slider_radius, SIGNAL(valueChanged(int)), this, SLOT(onSliderRadius(int)));
    connect (glarea, SIGNAL(radiusChanged(double)), this, SLOT(setSliderRadius(double)));

    connect(slider_dist_camera, SIGNAL(valueChanged(int)), this, SLOT(onSliderDistCamera(int)));
    connect (glarea, SIGNAL(distCameraChanged(double)), this, SLOT(setSliderDistCamera(double)));

    connect(slider_near, SIGNAL(valueChanged(int)), this, SLOT(onSliderNear(int)));
    connect (glarea, SIGNAL(nearChanged(double)), this, SLOT(setSliderNear(double)));

    connect(slider_far, SIGNAL(valueChanged(int)), this, SLOT(onSliderFar(int)));
    connect (glarea, SIGNAL(farChanged(double)), this, SLOT(setSliderFar(double)));

    connect(slider_angle, SIGNAL(valueChanged(int)), this, SLOT(onSliderAngle(int)));
    connect (glarea, SIGNAL(angleChanged(double)), this, SLOT(setSliderAngle(double)));

    connect(slider_vitesse, SIGNAL(valueChanged(int)), this, SLOT(onSliderVitesse(int)));
    connect (glarea, SIGNAL(vitesseChanged(double)), this, SLOT(setSliderVitesse(double)));

    connect(but_arreter, SIGNAL(clicked()), glarea, SLOT(onButArreter()));
    connect(but_demarrer, SIGNAL(clicked()), glarea, SLOT(onButDemarrer()));
}

void Dialog::onSliderRadius(int value)
{
    emit glarea->radiusChanged(value/20.0);
}

void Dialog::setSliderRadius(double radius)
{
    int value = radius * 20;
    if (slider_radius->value() != value) {
        slider_radius->setValue(value);
    }
}


void Dialog::onSliderDistCamera(int value) {
    emit glarea->distCameraChanged(value / 20.0);
}
void Dialog::setSliderDistCamera(double dist_camera) {
    int value = dist_camera * 20;
    if (slider_dist_camera->value() != value) {
        slider_dist_camera->setValue(value);
    }
}


void Dialog::onSliderNear(int value) {
    emit glarea->nearChanged(value / 20.0);
}
void Dialog::setSliderNear(double near) {
    int value = near * 20;
    if (slider_near->value() != value) {
        slider_near->setValue(value);
    }
}

void Dialog::onSliderFar(int value) {
    emit glarea->farChanged(value / 20.0);
}
void Dialog::setSliderFar(double far) {
    int value = far * 20;
    if (slider_far->value() != value) {
        slider_far->setValue(value);
    }
}

void Dialog::onSliderAngle(int value) {
    emit glarea->angleChanged(value / 20.0);
}
void Dialog::setSliderAngle(double angle) {
    int value = angle * 20;
    if (slider_angle->value() != value) {
        slider_angle->setValue(value);
    }
}

void Dialog::onSliderVitesse(int value) {
    emit glarea->vitesseChanged(value / 4.0);
}
void Dialog::setSliderVitesse(double vitesse) {
    int value = vitesse * 4;
    if (slider_vitesse->value() != value) {
        slider_vitesse->setValue(value);
    }
}
