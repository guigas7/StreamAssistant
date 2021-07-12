#include "plaintextwidget.h"

PlainTextWidget::PlainTextWidget(QString sec, QString fts): MyWidget(sec, fts) {}

void PlainTextWidget::init(QString importingDir, QWidget *wid)
{
    QPlainTextEdit *plainText = (QPlainTextEdit *)wid;
    QFile file(importingDir + this->getSection() + this->getFileToSave());
    QString text;
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        text = in.readAll();
        plainText->setPlainText(text);
    } else {
        qDebug() << "Couldn't read file " + importingDir + this->getSection() + this->getFileToSave();
    }
    file.close();
}

void PlainTextWidget::saveInFile(QString importingDir, QWidget *wid)
{
    QPlainTextEdit *widget = (QPlainTextEdit *) wid;
    this->writeInFile(importingDir + this->getSection() + this->getFileToSave(), widget->toPlainText());
}
