#ifndef SPINBOXWIDGET_H
#define SPINBOXWIDGET_H

#include "mywidget.h"
#include <QSpinBox>

class SpinBoxWidget : public MyWidget
{
public:
    explicit SpinBoxWidget(QString sec, QString fts);
    void saveInFile(QString importingDir, QWidget *wid);
    void init(QString importingDir, QWidget *wid);
};

#endif // SPINBOXWIDGET_H
