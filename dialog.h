#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>
#include <QDialog>
#include "ui_dialog.h"
#include "glarea.h"

class Dialog : public QDialog, private Ui::dialog
{
    Q_OBJECT

private:
    GLArea* glarea;

public:
    explicit Dialog(QWidget *parent = nullptr, GLArea* glarea = nullptr);

public slots:
    void onSliderRadius(int value);
    void setSliderRadius(double radius);

    void onSliderDistCamera(int value);
    void setSliderDistCamera(double dist_camera);

    void onSliderNear(int value);
    void setSliderNear(double near);

    void onSliderFar(int value);
    void setSliderFar(double far);

    void onSliderAngle(int value);
    void setSliderAngle(double angle);

    void onSliderVitesse(int value);
    void setSliderVitesse(double vitesse);

signals:

};

#endif // DIALOG_H
