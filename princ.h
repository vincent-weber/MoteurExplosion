#ifndef PRINC_H
#define PRINC_H

#include "ui_princ.h"

class Princ : public QMainWindow, private Ui::Princ
{
    Q_OBJECT

public:
    explicit Princ(QWidget *parent = 0);
    QDialog* optionsDialogBox = nullptr;

public slots:
    void openOptionsDialogBox();
};

#endif // PRINC_H
