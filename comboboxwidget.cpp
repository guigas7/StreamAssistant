#include "comboboxwidget.h"

ComboBoxWidget::ComboBoxWidget(QString sec, QString fts): MyWidget(sec, fts) {}

void ComboBoxWidget::init(QString importingDir, QWidget *wid, QString listDir)
{
    QComboBox *comboBox = (QComboBox *)wid;
    QFile file(importingDir + this->getSection() + this->getFileToSave() + ".txt");
    QString text;
    // init the autocomplete list TODO
    const QStringList itemsList = this->getListOf(listDir);
    comboBox->addItems(itemsList);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        text = in.readAll();
        comboBox->setCurrentText(text);
    } else {
        qDebug() << "Couldn't read file " + importingDir + this->getSection() + this->getFileToSave() + ".txt";
    }
    file.close();
}

void ComboBoxWidget::init(QString importingDir, QWidget *wid, QStringList list)
{
    QComboBox *comboBox = (QComboBox *)wid;
    QFile file(importingDir + this->getSection() + this->getFileToSave() + ".txt");
    QString text;
    comboBox->addItems(list);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        text = in.readAll();
        comboBox->setCurrentText(text);
    } else {
        qDebug() << "Couldn't read file " + importingDir + this->getSection() + this->getFileToSave() + ".txt";
    }
    file.close();
}


void ComboBoxWidget::saveInFile(QString importingDir, QWidget *wid)
{
    QComboBox *widget = (QComboBox *) wid;
    this->writeInFile(importingDir + this->getSection() + this->getFileToSave() + ".txt", widget->currentText());
}

void ComboBoxWidget::saveContentInFile(QString importingDir, QWidget *wid)
{
    QComboBox *widget = (QComboBox *) wid;
    QString copyTo = importingDir + this->getSection() + this->getFileToSave();
    QString nameToSave = widget->currentText();
    this->writeInFile(copyTo + ".txt", nameToSave);
}

void ComboBoxWidget::saveInFile(QString importingDir, QWidget *wid, QString mode, QString local)
{
    QComboBox *widget = (QComboBox *) wid;
    QString copyTo = importingDir + this->getSection() + this->getFileToSave();
    QString nameToSave = local + "/" + widget->currentText();
    if (mode.compare("image") == 0) {
        nameToSave = this->findImageWithExtension(nameToSave);
        this->copyFile(nameToSave, copyTo);
    }
    QFileInfo path(nameToSave);
    nameToSave = path.baseName();
    this->writeInFile(copyTo + ".txt", nameToSave);
}

QStringList ComboBoxWidget::getListOf(QString dirString)
{
    QStringList completionList;
    QStringList noExtensionList;
    int lastPoint;
    QDir directory(dirString);
    completionList = directory.entryList(QDir::Files);
    for (int i = 0; i < completionList.size(); ++i) {
        lastPoint = completionList.at(i).lastIndexOf(".");
        if (lastPoint == -1) { // if there's no extension, keep it the same
            noExtensionList << completionList.at(i);
        } else { // if there's and extension, remove it
            noExtensionList << completionList.at(i).left(lastPoint);
        }
    }
    return noExtensionList;
}
