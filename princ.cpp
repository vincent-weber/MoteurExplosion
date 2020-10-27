#include "princ.h"
#include "dialog.h"
#include <QDebug>

Princ::Princ(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    optionsDialogBox = new Dialog(nullptr, glarea);
    connect(button_options, SIGNAL(clicked()), this, SLOT(openOptionsDialogBox()));
}

void Princ::openOptionsDialogBox() {
    optionsDialogBox->exec();
}
