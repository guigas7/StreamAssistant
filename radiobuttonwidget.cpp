#include "radiobuttonwidget.h"

RadioButtonWidget::RadioButtonWidget(QString sec, QString fts): MyWidget(sec, fts) {}

void RadioButtonWidget::init(QString importingDir, QWidget *wid, QString id)
{
    QRadioButton *radioButton = (QRadioButton *)wid;
    QFile file(importingDir + this->getSection() + this->getFileToSave());
    QString text;
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        text = in.readAll();
        // If the content of the file is the id, check the radio
        if (text.compare(id) == 0) {
            radioButton->setChecked(true);
        }
    } else {
        qDebug() << "Couldn't read file " + importingDir + this->getSection() + this->getFileToSave();
    }
    file.close();
}

void RadioButtonWidget::saveInFile(QString importingDir, QString id)
{
    this->writeInFile(importingDir + this->getSection() + this->getFileToSave(), id);
    this->setRound(importingDir, id);
}

void RadioButtonWidget::setRound(QString importingDir, QString id)
{
    QString mapFile = importingDir + "/set/maps/" + id;
    QString modeFile = importingDir + "/set/modes/" + id;
    QString winnerFile = importingDir + "/set/winners/" + id;
    QString playerFile;
    QString weaponFile;
    QString setDir = importingDir + "/set/";
    this->copyFile(mapFile, setDir + "currentMap");
    this->copyFile(mapFile + ".txt", setDir + "currentMap.txt");
    this->copyFile(modeFile, setDir + "currentMode");
    this->copyFile(modeFile + ".txt", setDir + "currentMode.txt");
    this->copyFile(winnerFile, setDir + "currentWinner");
    this->copyFile(winnerFile + ".txt", setDir + "currentWinner.txt");
    for (int i = 1; i < 9; ++i) {
        playerFile = importingDir + "/set/players/" + "R" + id + "_" + QString::number(i);
        this->copyFile(playerFile + ".txt", setDir + "Player" + QString::number(i) + ".txt");
    }
    for (int i = 1; i < 9; ++i) {
        weaponFile = importingDir + "/set/weapons/" + "R" + id + "_" + QString::number(i);
        this->copyFile(weaponFile + ".txt", setDir + "Weapon" + QString::number(i) + ".txt");
        this->copyFile(weaponFile, setDir + "Weapon" + QString::number(i));
    }
}
