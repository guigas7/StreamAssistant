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

void CheckBoxWidget::handleCheck(QString importingDir, QWidget *wid, QString teamDir, QString winPointsDir)
{
    QCheckBox *widget = (QCheckBox *) wid;
    QString imageFile = importingDir + this->getSection() + this->getFileToSave();
    QString nameFile = imageFile + ".txt";
    QString teamPointsDir = winPointsDir + teamDir;
    QString savingDir = importingDir + "/set/winPoints/";
    if (widget->isChecked()) {
        QString teamImage = importingDir + teamDir + "logo";
        QString teamName = importingDir + teamDir + "name.txt";
        this->copyFile(teamImage, imageFile);
        this->copyFile(teamName, nameFile);
        // Copy winpoints
        QString winPointFile;
        QString savingPointFile;
        QDir directory(teamPointsDir);
        if (directory.exists()) {
            QStringList winPointsList = directory.entryList(QDir::Files);
            for (int i = 0; i < winPointsList.size(); ++i) {
                // Get original source winpoint file
                winPointFile = this->findImageWithExtension(teamPointsDir + "/" + winPointsList.at(i));
                // Save on sets folder with Game number _ file without extension
                savingPointFile = savingDir + this->getFileToSave() + "_" + winPointsList.at(i).split(".").at(0);
                this->copyFile(winPointFile, savingPointFile);
            }
        }
    } else {
        QFile logo(imageFile);
        QFile name(nameFile);
        logo.remove();
        name.remove();
        // Delete winpoints
        QDir directory(savingDir);
        if (directory.exists()) {
            QStringList search;
            search << this->getFileToSave() + "_*";
            QStringList filesList = directory.entryList(search, QDir::Files);
            QFile toDelete;
            for (int i = 0; i < filesList.size(); ++i) {
                toDelete.setFileName(savingDir + "/" + filesList.at(i));
                toDelete.remove();
            }
        }
    }
}
