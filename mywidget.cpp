#include "mywidget.h"

MyWidget::MyWidget(QString sec, QString fts)
{
    this->section = sec;
    this->fileToSave = fts;
}

Qtype MyWidget::convertType(QString typ)
{
    if (QString::compare(typ, "QLINEEDIT", Qt::CaseInsensitive)) return QLINEEDIT;
    else if (QString::compare(typ, "QLabel", Qt::CaseInsensitive)) return QLABEL;
    else if (QString::compare(typ, "QComboBox", Qt::CaseInsensitive)) return QCOMBOBOX;
    else if (QString::compare(typ, "QSpinBox", Qt::CaseInsensitive)) return QSPINBOX;
    else if (QString::compare(typ, "QCheckBox", Qt::CaseInsensitive)) return QCHECKBOX;
    else if (QString::compare(typ, "QRadioButton", Qt::CaseInsensitive)) return QRADIOBUTTON;
    return QLINEEDIT;
}

QString MyWidget::getSection()
{
    return this->section;
}

QString MyWidget::getFileToSave()
{
    return this->fileToSave;
}

void MyWidget::setSection(QString sec)
{
    this->section = sec;
}

void MyWidget::setFileToSave(QString fts)
{
    this->fileToSave= fts;
}

void MyWidget::writeInFile(QString filename, QString text)
{
    QFile file(filename);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << text;
        file.flush();
        file.close();
    } else {
        qDebug() << "couldn't open file " + filename;
    }
}

void MyWidget::init(QString importingDir, QWidget *wid)
{
    qDebug() << "Got in the base class init";
    return;
}

void MyWidget::saveInFile(QString importingDir, QWidget *wid)
{
    qDebug() << "Got in the base class saveInFile";
    return;
}
