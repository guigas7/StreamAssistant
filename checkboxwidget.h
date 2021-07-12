#ifndef CHECKBOXWIDGET_H
#define CHECKBOXWIDGET_H

#include "mywidget.h"
#include <QCheckBox>

class CheckBoxWidget : public MyWidget
{
public:
    explicit CheckBoxWidget(QString sec, QString fts);
    void handleCheck(QString importingDir, QWidget *wid, QString teamDir);
    void init(QString importingDir, QWidget *wid, QString teamName);
};

#endif // CHECKBOXWIDGET_H
