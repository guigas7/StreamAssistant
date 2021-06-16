#include "lineeditwidget.h"

LineEditWidget::LineEditWidget(QString sec, QString fts): MyWidget(sec, fts) {}

void LineEditWidget::init(QString importingDir, QWidget *wid)
{
    QLineEdit *lineEdit = (QLineEdit *)wid;
    QFile file(importingDir + this->getSection() + this->getFileToSave());
    QString text;
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        text = in.readAll();
        lineEdit->setText(text);
    } else {
        qDebug() << "Couldn't read file " + importingDir + this->getSection() + this->getFileToSave();
    }
    file.close();
}

void LineEditWidget::saveInFile(QString importingDir, QWidget *wid)
{
    QLineEdit *widget = (QLineEdit *) wid;
    this->writeInFile(importingDir + this->getSection() + this->getFileToSave(), widget->text());
}
