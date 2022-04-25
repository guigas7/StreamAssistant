#include "lineeditwidget.h"

LineEditWidget::LineEditWidget(QString sec, QString fts): MyWidget(sec, fts) {}

void LineEditWidget::init(QString importingDir, QWidget *wid, QString extension)
{
    QLineEdit *lineEdit = (QLineEdit *)wid;
    QFile file(importingDir + this->getSection() + this->getFileToSave() + extension);
    QString text;
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        text = in.readAll();
        lineEdit->setText(text);
    } else {
        qDebug() << "No file to initialize " + importingDir + this->getSection() + this->getFileToSave();
    }
    file.close();
}

void LineEditWidget::saveInFile(QString importingDir, QWidget *wid, QString section, QString fileToSave, QString content)
{
    QLineEdit *widget = (QLineEdit *) wid;
    QString sec = (section.isEmpty() ? this->getSection() : section);
    QString fts = (fileToSave.isEmpty() ? this->getFileToSave() : fileToSave);
    QString cont = (content.isEmpty() ? widget->text() : content);
    this->writeInFile(importingDir + sec + fts, cont);
}

void LineEditWidget::saveWithImage(QString importingDir, QWidget *wid, QString local, QString defaultDir)
{
    QLineEdit *widget = (QLineEdit *) wid;
    QString copyTo = importingDir + this->getSection() + this->getFileToSave();
    QString copyFrom = local + "/" + widget->text();
    copyFrom = this->findImageWithExtension(copyFrom);
    if (!copyFrom.isEmpty()) {
        this->copyFile(copyFrom, copyTo);
    } else if (!defaultDir.isEmpty()) {
        copyFrom = this->getRandomImageName(defaultDir);
        this->copyFile(copyFrom, copyTo);
    }
    this->writeInFile(copyTo + ".txt", widget->text());
}

