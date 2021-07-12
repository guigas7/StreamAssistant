#ifndef PLAINTEXTWIDGET_H
#define PLAINTEXTWIDGET_H

#include "mywidget.h"
#include <QPlainTextEdit>

class PlainTextWidget : public MyWidget
{
public:
    explicit PlainTextWidget(QString sec, QString fts);
    void saveInFile(QString importingDir, QWidget *wid);
    void init(QString importingDir, QWidget *wid);
};

#endif // PLAINTEXTWIDGET_H
