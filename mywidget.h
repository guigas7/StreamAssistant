#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QString>
#include <QWidget>
#include <QFile>
#include <QLineEdit>
#include <QDir>
#include <QRandomGenerator>
#include <QLabel>
#include <QMetaEnum>

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
    void showImageInLabel(QString imageName, QLabel *logo, QString copyName, Qt::AspectRatioMode);
    void ignoreAspectInLabel(QString imageName, QLabel *logo, QString copyName);
    void keepAspectInLabel(QString imageName, QLabel *logo, QString copyName);
    virtual ~MyWidget();

private:
    QString section;
    QString fileToSave;
};

#endif // MYWIDGET_H
