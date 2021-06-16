#ifndef LINEEDITWIDGET_H
#define LINEEDITWIDGET_H

#include "mywidget.h"

class LineEditWidget : public MyWidget
{
public:
    explicit LineEditWidget(QString sec, QString fts);
    void saveInFile(QString importingDir, QWidget *wid);
    void init(QString importingDir, QWidget *wid);
};

#endif // LINEEDITWIDGET_H
