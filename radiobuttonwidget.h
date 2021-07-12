#ifndef RADIOBUTTONWIDGET_H
#define RADIOBUTTONWIDGET_H

#include "mywidget.h"
#include <QRadioButton>

class RadioButtonWidget : public MyWidget
{
public:
    explicit RadioButtonWidget(QString sec, QString fts);
    void saveInFile(QString importingDir, QString id);
    void init(QString importingDir, QWidget *wid, QString id);
    void setRound(QString importingDir, QString id);
};

#endif // RADIOBUTTONWIDGET_H
