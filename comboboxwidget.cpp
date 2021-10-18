#include "comboboxwidget.h"

ComboBoxWidget::ComboBoxWidget(QString sec, QString fts): MyWidget(sec, fts) {}

void ComboBoxWidget::init(QString importingDir, QWidget *wid, QString listDir)
{
    if (!listDir.isEmpty()) {
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
        for (int i = 0; i < itemsList.size(); ++i) {
            comboBox->setItemIcon(i, QIcon(listDir + "/" + itemsList.at(i)));
        }
        file.close();
    }
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

void ComboBoxWidget::init(QString importingDir, QWidget *wid, QStringList list, QString filename)
{
    QComboBox *comboBox = (QComboBox *)wid;
    QFile file(filename);
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

bool ComboBoxWidget::handleColorFile(QString colorFile, QString savingFile, QLabel *logo, bool &showedInAssistant, bool mirrored)
{
    bool saved = false;
    if (QFile::exists(colorFile)) {
        if (!mirrored) {
            this->copyFile(colorFile, savingFile);
        } else {
            QImage toCopy(colorFile);
            toCopy.mirror(1,1);
            toCopy.save(savingFile + ".png", nullptr, 100);
            this->copyFile(savingFile + ".png", savingFile);
            QFile::remove(savingFile + ".png");
        }
        if (!showedInAssistant) {
            this->ignoreAspectInLabel(colorFile, logo, mirrored);
            showedInAssistant = true;
        }
        saved = true;
    } else {
        QFile importingColor(savingFile);
        importingColor.remove();
    }
    return saved;
}

bool ComboBoxWidget::saveColor(QString colorDir, QString color, QString dirToSave, QString fileToSave, QLabel *logo, bool mirrored)
{
    QString colorFile;
    QString savingFile;
    bool saved = false;
    bool showedInAssistant = false;
    // for colors on colorDir
    colorFile = this->findImageWithExtension(colorDir + color);
    savingFile = dirToSave + fileToSave;
    saved = handleColorFile(colorFile, savingFile, logo, showedInAssistant, mirrored);
    // for colors on //colorDir's subdirectories
    QDir directory(colorDir);
    if (directory.exists()) {
        QStringList dirList = directory.entryList(QDir::Dirs);
        dirList.removeFirst(); // Take .
        dirList.removeFirst(); // Take ..
        for (int i = 0; i < dirList.size(); ++i) {
            colorFile = this->findImageWithExtension(colorDir + dirList.at(i) + "/" + color);
            savingFile = dirToSave + dirList.at(i) + "_" + fileToSave;
            saved = handleColorFile(colorFile, savingFile, logo, showedInAssistant, mirrored);
        }
    }
    return saved;
}

void ComboBoxWidget::saveColors(QString importingDir, QWidget *wid, QString local, QLabel *alphaLabel, QLabel *betaLabel, QLabel *comboLabel, bool mirrored)
{
    QComboBox *widget = (QComboBox *) wid;
    QString color = widget->currentText();
    QString colorNameFile = importingDir + this->getSection() + this->getFileToSave() + ".txt";
    bool savedColorName = false;
    QString alphaImportingDir = mirrored ? importingDir + "/teamBeta/" : importingDir + "/teamAlpha/";
    QString betaImportingDir = mirrored ? importingDir + "/teamAlpha/" : importingDir + "/teamBeta/";
    if(this->saveColor(local + "/alpha/", color, alphaImportingDir, this->getFileToSave(), mirrored ? betaLabel : alphaLabel)) {
        writeInFile(colorNameFile, color);
        savedColorName = true;
    }
    if (this->saveColor(local + "/beta/", color, betaImportingDir, this->getFileToSave(), mirrored ? alphaLabel : betaLabel)) {
        if (!savedColorName) {
            writeInFile(colorNameFile, color);
            savedColorName = true;
        }
    }
    if (this->saveColor(local + "/combo/", color, importingDir + this->getSection(), this->getFileToSave() + "s", comboLabel, mirrored) && !savedColorName)
    {
        if (!savedColorName) {
            writeInFile(colorNameFile, color);
            savedColorName = true;
        }
    }
    if (!savedColorName) {
        QFile::remove(colorNameFile);
    }
    return;
}

