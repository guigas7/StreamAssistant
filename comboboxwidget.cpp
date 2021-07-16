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
    QStringList noExtensionList;
    int lastPoint;
    QDir directory(dirString);
    QStringList completionList = directory.entryList(QDir::Files);
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

void ComboBoxWidget::saveColor(QString colorDir, QString color, QString dirToSave, QString fileToSave, QLabel *logo)
{
    QString colorFile;
    QString savingFile;
    bool showedInAssistant = false;
    // for colors on colorDir
    colorFile = this->findImageWithExtension(colorDir + color);
    savingFile = dirToSave + fileToSave;
    if (QFile::exists(colorFile)) {
        this->copyFile(colorFile, savingFile);
        this->ignoreAspectInLabel(colorFile, logo, savingFile);
        showedInAssistant = true;
    } else {
        QFile importingColor(savingFile);
        importingColor.remove();
    }
    // for colors on //colorDir's subdirectories
    QDir directory(colorDir);
    QStringList dirList = directory.entryList(QDir::Dirs);
    dirList.removeFirst(); // Take .
    dirList.removeFirst(); // Take ..
    for (int i = 0; i < dirList.size(); ++i) {
        colorFile = this->findImageWithExtension(colorDir + dirList.at(i) + "/" + color);
        savingFile = dirToSave + dirList.at(i) + "_" + fileToSave;
        // creates dir to save if there isn't one
        if (QFile::exists(colorFile)) {
            this->copyFile(colorFile, savingFile);
            if (!showedInAssistant) {
                this->ignoreAspectInLabel(colorFile, logo, savingFile);
                showedInAssistant = true;
            }
        } else {
            QFile importingColor(savingFile);
            importingColor.remove();
        }
    }
}

void ComboBoxWidget::saveColors(QString importingDir, QWidget *wid, QString local, QLabel *alphaLabel, QLabel *betaLabel, QLabel *comboLabel)
{
    QComboBox *widget = (QComboBox *) wid;
    QString color = widget->currentText();

    this->saveColor(local + "/alpha/", color, importingDir + "/teamAlpha/", this->getFileToSave(), alphaLabel);
    this->saveColor(local + "/beta/", color, importingDir + "/teamBeta/", this->getFileToSave(), betaLabel);
    this->saveColor(local + "/combo/", color, importingDir + this->getSection(), this->getFileToSave() + "s", comboLabel);
    return;
}
