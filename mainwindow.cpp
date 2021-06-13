#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <iostream>
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // init functions
    qDebug() << ui->ColorsCombo->staticMetaObject.className();
//    this->widgetsInit();
//    this->configDirectories_init();
//    this->autoComplete_init();
//    this->infoSection_init();
//    this->TeamsSection_init();
//    this->RoundSection_init();
//    this->CastersSection_init();
//    this->SetSection_init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::widgetsInit()
{
    QWidget *widgets[] = {
        ui->Team1NameEdit, ui->Team1RegionCombo, ui->Team1Logo, ui->Team1Color, ui->Team1Score, ui->Team2NameEdit,
        ui->Team2RegionCombo, ui->Team2Logo, ui->Team2Color, ui->Team2Score, ui->TitleEdit, ui->SubtitleEdit, ui->TwitterEdit,
        ui->MessageEdit, ui->LogoCombo, ui->Caster1NameEdit, ui->Caster2NameEdit, ui->Caster3NameEdit, ui->Caster1TwitterEdit,
        ui->Caster2TwitterEdit, ui->Caster3TwitterEdit, ui->RoundEdit, ui->ColorGroupCombo, ui->ColorsCombo,ui->AlphaWinRound1,
        ui->AlphaWinRound2, ui->AlphaWinRound3, ui->AlphaWinRound4, ui->AlphaWinRound5, ui->AlphaWinRound6, ui->AlphaWinRound7,
        ui->AlphaWinRound8, ui->AlphaWinRound9, ui->BetaWinRound1, ui->BetaWinRound2, ui->BetaWinRound3, ui->BetaWinRound4,
        ui->BetaWinRound5, ui->BetaWinRound6, ui->BetaWinRound7, ui->BetaWinRound8, ui->BetaWinRound9, ui->CheckboxGame1,
        ui->CheckboxGame2, ui->CheckboxGame3, ui->CheckboxGame4, ui->CheckboxGame5, ui->CheckboxGame6, ui->CheckboxGame7,
        ui->CheckboxGame8, ui->CheckboxGame9, ui->MapComboRound1, ui->MapComboRound2, ui->MapComboRound3, ui->MapComboRound4,
        ui->MapComboRound5, ui->MapComboRound6, ui->MapComboRound7, ui->MapComboRound8, ui->MapComboRound9, ui->ModeComboRound1,
        ui->ModeComboRound2, ui->ModeComboRound3, ui->ModeComboRound4, ui->ModeComboRound5, ui->ModeComboRound6, ui->ModeComboRound7,
        ui->ModeComboRound8, ui->ModeComboRound9
    };
    QString filenames[] = {
        "name.txt", "region.txt", "logo", "color", "score.txt", "name.txt",
        "region.txt", "logo", "color", "score.txt", "title.txt", "subtitle.txt", "twitter.txt",
        "message.txt", "tournamentLogo", "1Name.txt", "2Name.txt", "3Name.txt", "1Twitter.txt",
        "2Twitter.txt", "3Twitter.txt", "round.txt", "colorGroups.txt", "colors", "1",
        "2", "3", "4", "5", "6", "7",
        "8", "9", "1", "2", "3", "4",
        "5", "6", "7", "8", "9", "currentGame.txt",
        "currentGame.txt", "currentGame.txt", "currentGame.txt", "currentGame.txt", "currentGame.txt", "currentGame.txt",
        "currentGame.txt", "currentGame.txt", "1", "2", "3", "4",
        "5", "6", "7", "8", "9", "1",
        "2", "3", "4", "5", "6", "7",
        "8", "9",
    };
    QString importDirs[] = {
        "teamAlpha", "teamAlpha", "teamAlpha", "teamAlpha", "teamAlpha", "teamBeta",
        "teamBeta", "teamBeta", "teamBeta", "teamBeta", "info", "info", "info",
        "info", "info", "casters", "casters", "casters", "casters",
        "casters", "casters", "round", "round", "round", "set/winners",
        "set/winners", "set/winners", "set/winners", "set/winners", "set/winners", "set/winners",
        "set/winners", "set/winners", "set/winners", "set/winners", "set/winners", "set/winners",
        "set/winners", "set/winners", "set/winners", "set/winners", "set/winners", "set",
        "set", "set", "set", "set", "set", "set",
        "set", "set", "set/maps", "set/maps", "set/maps", "set/maps",
        "set/maps", "set/maps", "set/maps", "set/maps", "set/maps", "set/modes",
        "set/modes", "set/modes", "set/modes", "set/modes", "set/modes", "set/modes",
        "set/modes", "set/modes",
    };
}

// Serialization functions

void MainWindow::serialize(const char * filename, void * data, int size)
{
    FILE * file = std::fopen(filename, "w");
    char * stream = (char *) data;
    std::fwrite(stream, sizeof(char), size, file);
    std::fclose(file);
}

void MainWindow::deserialize(const char * filename, void * data, int size)
{
    FILE * file = std::fopen(filename, "r");
    char * stream = (char *) data;
    std::fread(stream, sizeof(char), size, file);
    std::fclose(file);
}

// Init functions

void MainWindow::configDirectories_init()
{
    // Initialize config filenames
    QString dirs[dirAmount] = {
        "/AlphaTeamDirectory.txt", "/BetaTeamDirectory.txt", "/RegionsDirectory.txt", "/LogosDirectory.txt",
        "/MapsDirectory.txt", "/ModeIconsDirectory.txt", "/SplatfestColorsDirectory.txt", "/TurfWarColorsDirectory.txt",
        "/RankedColorsDirectory.txt", "/ImportingFilesDirectory.txt", "/DefaultDirectory.txt"
    };
    for (int i = 0; i < dirAmount; i++) {
        this->directories[i] = dirs[i];
    }
    QString directory = QDir::currentPath();
    directory.append("/config");
    QString filename;
    QFile file;
    QDir dir;
    dir.mkdir(directory);
    for (int i = 0; i < dirAmount; i++) {
        filename = directory;
        filename.append(this->directories[i]);
        file.setFileName(filename);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&file);
            this->directoryFor[this->directories[i]] = in.readAll();
            file.close();
        }
    }
}

QStringList MainWindow::getListOf(QString dirString)
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

void MainWindow::autoComplete_init()
{
    // Set TeamAlpha Autocomplete
    QStringList team1CompletionList = this->getListOf(this->directoryFor["/AlphaTeamDirectory.txt"]);
    Team1Completer = new QCompleter(team1CompletionList, this);
    Team1Completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->Team1NameEdit->setCompleter(Team1Completer);
    // Set TeamBeta Autocomplete
    QStringList team2completionList = this->getListOf(this->directoryFor["/BetaTeamDirectory.txt"]);
    Team2Completer = new QCompleter(team2completionList, this);
    Team2Completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->Team1NameEdit->setCompleter(Team2Completer);
}

void MainWindow::infoSection_init()
{
    // Logo Combo
    const QStringList logosList = this->getListOf(this->directoryFor["/LogosDirectory.txt"]);
    ui->LogoCombo->addItems(logosList);
}

void MainWindow::TeamsSection_init()
{
    // Regions Combo
    const QStringList RegionsList = this->getListOf(this->directoryFor["/RegionsDirectory.txt"]);
    ui->Team1RegionCombo->addItems(RegionsList);
    ui->Team2RegionCombo->addItems(RegionsList);
}

void MainWindow::RoundSection_init()
{
    // Color Group Combo
    const QStringList ColorGroupsList = {"Ranked", "Turf War", "Spletfest"};
    ui->ColorGroupCombo->addItems(ColorGroupsList);
    // Colors Combo
    QStringList colorsList;
    QString comboString;
    QString indivString;
    if (ui->ColorGroupCombo->currentData().toString() == "Ranked") {
        // Check if combo or individual
        comboString = this->directoryFor["/RankedColorsDirectory.txt"] + "/combo";
        indivString = this->directoryFor["/RankedColorsDirectory.txt"] + "/alpha";
        QDir comboDir(comboString);
        QDir indivDir(indivString);
        if (comboDir.exists() && !comboDir.isEmpty()) {
            colorsList = this->getListOf(comboString);
        } else if (indivDir.exists() && !indivDir.isEmpty()) {
            colorsList = this->getListOf(indivString);
        }
    } else if (ui->ColorGroupCombo->currentData().toString() == "Turf War") {
        // Check if combo or individual
        comboString = this->directoryFor["/TurfWarColorsDirectory.txt"] + "/combo";
        indivString = this->directoryFor["/TurfWarColorsDirectory.txt"] + "/alpha";
        QDir comboDir(comboString);
        QDir indivDir(indivString);
        if (comboDir.exists() && !comboDir.isEmpty()) {
            colorsList = this->getListOf(comboString);
        } else if (indivDir.exists() && !indivDir.isEmpty()) {
            colorsList = this->getListOf(indivString);
        }
    } else { // Splatfest
        // Check if combo or individual
        comboString = this->directoryFor["/SplatfestColorsDirectory.txt"] + "/combo";
        indivString = this->directoryFor["/SplatfestColorsDirectory.txt"] + "/alpha";
        QDir comboDir(comboString);
        QDir indivDir(indivString);
        if (comboDir.exists() && !comboDir.isEmpty()) {
            colorsList = this->getListOf(comboString);
        } else if (indivDir.exists() && !indivDir.isEmpty()) {
            colorsList = this->getListOf(indivString);
        }
    }
    ui->ColorsCombo->addItems(colorsList);
}

void MainWindow::CastersSection_init()
{
    QFile file;
    QString directory = this->directoryFor["/ImportingFilesDirectory.txt"] + "/casters";
    QString text;
    QLineEdit *casterFields[6] = {
        ui->Caster1NameEdit, ui->Caster2NameEdit, ui->Caster3NameEdit,
        ui->Caster1TwitterEdit, ui->Caster2TwitterEdit, ui->Caster3TwitterEdit
    };
    QString filenames[6] = {
        "/1Name.txt", "/2Name.txt", "/3Name.txt",
        "/1Twitter.txt", "/2Twitter.txt", "/3Twitter.txt"
    };
    for (int i = 0;i < 6; i++) {
        file.setFileName(directory + filenames[i]);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&file);
            text = in.readAll();
            casterFields[i]->setText(text);
        }
        file.close();
    }
}

void MainWindow::SetSection_init()
{
    // Regions Combo
    QComboBox *modeCombos[9] = {
        ui->ModeComboRound1, ui->ModeComboRound2, ui->ModeComboRound3,
        ui->ModeComboRound4, ui->ModeComboRound5, ui->ModeComboRound6,
        ui->ModeComboRound7, ui->ModeComboRound8, ui->ModeComboRound9
    };
    const QStringList ModesList = this->getListOf(this->directoryFor["/ModeIconsDirectory.txt"]);
    for (int i = 0;i < 9; i++) {
        modeCombos[i]->addItems(ModesList);
    }
}

// qDebug() << "debug text"

void MainWindow::writeInFile(QString filename, QString text)
{
    QFile file(filename);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << text;
        file.flush();
        file.close();
    } else {
        qDebug() << "couldn't open file " + filename;
    }
}

/*
/ Team Alpha Section
*/

void MainWindow::on_Team1NameEdit_editingFinished()
{
    // Set text right
    QString filename = this->directoryFor["/ImportingFilesDirectory.txt"] + "/teamAlpha/name.txt";
    this->writeInFile(filename, ui->Team1NameEdit->text());
    QString imageName = this->directoryFor["/AlphaTeamDirectory.txt"] + "/" + ui->Team1NameEdit->text();
    QString copyName = this->directoryFor["/ImportingFilesDirectory.txt"] + "/teamAlpha/logo";
    // Find correct extension
    if (QFile::exists(imageName + ".png")) {
        imageName.append(".png");
    } else if ((QFile::exists(imageName + ".jpg"))) {
        imageName.append(".jpg");
    } else {
        qDebug() << imageName + " not found!";
        // get random pic
        return;
    }
    // Copy team logo to file
    QFile::copy(imageName, copyName);
    // Show team logo on screen
    QImage image;
    bool valid = image.load(imageName);
    if (valid) {
        image = image.scaled(ui->Team1Logo->width(), ui->Team1Logo->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->Team1Logo->setPixmap(QPixmap::fromImage(image));
    } else {
        qDebug() << "couldn't load image";
    }
}

/*
/ END Team Alpha Section
*/

/*
/ Round Section
*/

/*
/ END Round Section
*/

void MainWindow::on_SetDirectoriesButton_clicked()
{
    dirConfigWindow = new FilesDialog(this, &this->directoryFor, this->directories);
    dirConfigWindow->show();
    this->configDirectories_init();
}

/*
/ Team Beta Section
*/

/*
/ END Team Beta Section
*/

/*
/ Info Section
*/

void MainWindow::on_TitleEdit_editingFinished()
{
    QString filename = this->directoryFor["/ImportingFilesDirectory.txt"];
    filename.append("/info/title.txt");
    this->writeInFile(filename, ui->TitleEdit->text());
}

void MainWindow::on_SubtitleEdit_editingFinished()
{
    QString filename = this->directoryFor["/ImportingFilesDirectory.txt"];
    filename.append("/info/subtitle.txt");
    this->writeInFile(filename, ui->SubtitleEdit->text());
}

void MainWindow::on_TwitterEdit_editingFinished()
{
    QString filename = this->directoryFor["/ImportingFilesDirectory.txt"];
    filename.append("/info/twitter.txt");
    this->writeInFile(filename, ui->TwitterEdit->text());
}

void MainWindow::on_MessageEdit_textChanged()
{
    QString filename = this->directoryFor["/ImportingFilesDirectory.txt"];
    filename.append("/info/message.txt");
    this->writeInFile(filename, ui->MessageEdit->toPlainText());
}

/*
/ END Info Section
*/

/*
/ Casters Section
*/

void MainWindow::on_Caster1NameEdit_editingFinished()
{
    QString filename = this->directoryFor["/ImportingFilesDirectory.txt"];
    filename.append("/casters/1Name.txt");
    this->writeInFile(filename, ui->Caster1NameEdit->text());
}


void MainWindow::on_Caster2NameEdit_editingFinished()
{
    QString filename = this->directoryFor["/ImportingFilesDirectory.txt"];
    filename.append("/casters/2Name.txt");
    this->writeInFile(filename, ui->Caster2NameEdit->text());
}


void MainWindow::on_Caster3NameEdit_editingFinished()
{
    QString filename = this->directoryFor["/ImportingFilesDirectory.txt"];
    filename.append("/casters/3Name.txt");
    this->writeInFile(filename, ui->Caster3NameEdit->text());
}


void MainWindow::on_Caster1TwitterEdit_editingFinished()
{
    QString filename = this->directoryFor["/ImportingFilesDirectory.txt"];
    filename.append("/casters/1Twitter.txt");
    this->writeInFile(filename, ui->Caster1TwitterEdit->text());
}


void MainWindow::on_Caster2TwitterEdit_editingFinished()
{
    QString filename = this->directoryFor["/ImportingFilesDirectory.txt"];
    filename.append("/casters/2Twitter.txt");
    this->writeInFile(filename, ui->Caster2TwitterEdit->text());
}


void MainWindow::on_Caster3TwitterEdit_editingFinished()
{
    QString filename = this->directoryFor["/ImportingFilesDirectory.txt"];
    filename.append("/casters/3Twitter.txt");
    this->writeInFile(filename, ui->Caster3TwitterEdit->text());
}
/*
/ END Casters Section
*/

// qDebug() << "debug text"

void MainWindow::on_Team1Score_textChanged(const QString &arg1)
{
    QString filename = this->directoryFor["/ImportingFilesDirectory.txt"];
    filename.append("/teamAlpha/score.txt");
    this->writeInFile(filename, arg1);
}


void MainWindow::on_Team2Score_textChanged(const QString &arg1)
{
    QString filename = this->directoryFor["/ImportingFilesDirectory.txt"];
    filename.append("/teamBeta/score.txt");
    this->writeInFile(filename, arg1);
}

void MainWindow::on_ResetScoresButton_clicked()
{
    ui->Team1Score->setValue(0);
    ui->Team2Score->setValue(0);
    ui->AlphaWinRound1->setChecked(false);
    ui->AlphaWinRound2->setChecked(false);
    ui->AlphaWinRound3->setChecked(false);
    ui->AlphaWinRound4->setChecked(false);
    ui->AlphaWinRound5->setChecked(false);
    ui->AlphaWinRound6->setChecked(false);
    ui->AlphaWinRound7->setChecked(false);
    ui->AlphaWinRound8->setChecked(false);
    ui->AlphaWinRound9->setChecked(false);
    ui->BetaWinRound1->setChecked(false);
    ui->BetaWinRound2->setChecked(false);
    ui->BetaWinRound3->setChecked(false);
    ui->BetaWinRound4->setChecked(false);
    ui->BetaWinRound5->setChecked(false);
    ui->BetaWinRound6->setChecked(false);
    ui->BetaWinRound7->setChecked(false);
    ui->BetaWinRound8->setChecked(false);
    ui->BetaWinRound9->setChecked(false);
}

/*
/ Set Section
*/

void MainWindow::on_MapComboRound1_textChanged(const QString &arg1)
{
    QString filename = this->directoryFor["/ImportingFilesDirectory.txt"];
}

/*
/ END Set Section
*/
