#ifndef WIDGET_H
#define WIDGET_H

#include <QString>
#include <QWidget>

enum Qtype { QLINEEDIT = 0, QLABEL = 1, QCOMBOBOX = 2, QSPINBOX = 3, QCHECKBOX = 4, QRADIOBUTTON = 5 };

class Widget
{
public:
    Widget(QString sec, QString fts, QWidget *wid);
    QString getSection();
    QString getFileToSave();
    Qtype getType();
    QWidget *getWidget();
    void setSection(QString sec);
    void setFileToSave(QString fts);
    void setType(Qtype typ);
    void setWidget(QWidget *wid);
    Qtype convertType(QString typ);

private:
    QString section;
    QString fileToSave;
    Qtype type;
    QWidget *widget;
    void saveInFile(QString);
    void init();
};

#endif // WIDGET_H
