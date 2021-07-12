#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QString>
#include <QWidget>
#include <QFile>
#include <QLineEdit>
#include <QDir>
#include <QRandomGenerator>

enum Qtype { QLINEEDIT = 0, QLABEL = 1, QCOMBOBOX = 2, QSPINBOX = 3, QCHECKBOX = 4, QRADIOBUTTON = 5, QPLAINTEXTEDIT = 6 };

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
    QString findImageWithExtension(QString filename);
    void copyFile(QString source, QString destiny);
    QString getRandomImageName(QString defaultDir);
    virtual ~MyWidget();

private:
    QString section;
    QString fileToSave;
};

#endif // MYWIDGET_H
