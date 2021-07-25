#ifndef LABELWIDGET_H
#define LABELWIDGET_H

#include "mywidget.h"
#include <QLabel>

class LabelWidget : public MyWidget
{
public:
    explicit LabelWidget(QString sec, QString fts);
    void init(QString importingDir, QWidget *wid, QString section, QString imageName, QString defaultDir, QString otherTeamSection = "");
};

#endif // LABELWIDGET_H
