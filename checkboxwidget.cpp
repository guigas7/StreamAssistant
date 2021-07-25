#include "checkboxwidget.h"

CheckBoxWidget::CheckBoxWidget(QString sec, QString fts): MyWidget(sec, fts) {}

void CheckBoxWidget::init(QString importingDir, QWidget *wid, QString teamName)
{
    QCheckBox *checkBox = (QCheckBox *)wid;
    QFile file(importingDir + this->getSection() + this->getFileToSave() + ".txt");
    QString text;
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        text = in.readAll();
    } else {
        text = "";
    }
    file.close();
    if (text.compare(teamName) == 0) {
        checkBox->setChecked(true);
    }
}

void CheckBoxWidget::handleCheck(QString importingDir, QWidget *wid, QString teamDir)
{
    QCheckBox *widget = (QCheckBox *) wid;
    QString winnersDir = importingDir + this->getSection();
    if (widget->isChecked()) {
        QString teamImage = importingDir + teamDir + "logo";
        QString teamName = importingDir + teamDir + "name.txt";
        this->copyFile(teamImage, winnersDir + this->getFileToSave());
        this->copyFile(teamName, winnersDir + this->getFileToSave() + ".txt");
    } else {
        QFile logo(winnersDir + this->getFileToSave());
        QFile name(winnersDir + this->getFileToSave() + ".txt");
        logo.remove();
        name.remove();
    }
}
