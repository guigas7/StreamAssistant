#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QString>
#include <QWidget>
#include <QFile>
#include <QLineEdit>

enum Qtype { QLINEEDIT = 0, QLABEL = 1, QCOMBOBOX = 2, QSPINBOX = 3, QCHECKBOX = 4, QRADIOBUTTON = 5 };

class MyWidget
{
public:
    MyWidget(QString sec = "", QString fts = "");
    QString getSection();
    QString getFileToSave();
    void setSection(QString sec);
    void setFileToSave(QString fts);
    Qtype convertType(QString typ);
    void writeInFile(QString filename, QString text);
    virtual void saveInFile(QString importingDir, QWidget *wid);
    virtual void init(QString importingDir, QWidget *wid);

private:
    QString section;
    QString fileToSave;
};

#endif // MYWIDGET_H
