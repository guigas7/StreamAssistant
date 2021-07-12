#ifndef LABELWIDGET_H
#define LABELWIDGET_H

#include "mywidget.h"
#include <QLabel>

class LabelWidget : public MyWidget
{
public:
    explicit LabelWidget(QString sec, QString fts);
    void init(QString importingDir, QWidget *wid, QString imageName, QString defaultDir);
};

#endif // LABELWIDGET_H
