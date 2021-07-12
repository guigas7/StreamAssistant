#include "spinboxwidget.h"

SpinBoxWidget::SpinBoxWidget(QString sec, QString fts): MyWidget(sec, fts) {}

void SpinBoxWidget::init(QString importingDir, QWidget *wid)
{
    QSpinBox *spinBox = (QSpinBox *)wid;
    QFile file(importingDir + this->getSection() + this->getFileToSave());
    QString text;
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        text = in.readAll();
        spinBox->setValue(text.toInt());
    } else {
        qDebug() << "Couldn't read file " + importingDir + this->getSection() + this->getFileToSave();
    }
    file.close();
}

void SpinBoxWidget::saveInFile(QString importingDir, QWidget *wid)
{
    QSpinBox *widget = (QSpinBox *) wid;
    this->writeInFile(importingDir + this->getSection() + this->getFileToSave(), QString::number(widget->value()));
}

