#ifndef LINEEDITWIDGET_H
#define LINEEDITWIDGET_H

#include "mywidget.h"
#include <QDir>

class LineEditWidget : public MyWidget
{
public:
    explicit LineEditWidget(QString sec, QString fts);
    void saveInFile(QString importingDir, QWidget *wid, QString section = "", QString fileToSave = "", QString content = "");
    void init(QString importingDir, QWidget *wid, QString extension = "");
    void saveWithImage(QString importingDir, QWidget *wid, QString local, QString defaultDir = "");
};

#endif // LINEEDITWIDGET_H
