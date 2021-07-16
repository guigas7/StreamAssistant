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
    this->widgetFor = this->widgets_init();
    this->configDirectories_init();
    this->infoSection_init();
    this->TeamsSection_init();
    this->RoundSection_init();
    this->CastersSection_init();
    this->SetSection_init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSet_Directories_triggered()
{
    dirConfigWindow = new FilesDialog(this, &this->directoryFor, this->directories);
    dirConfigWindow->show();
    this->configDirectories_init();
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
    QString directory = QDir::currentPath() + "/config";
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


void MainWindow::scoreAutoUpdate(QSpinBox *teamScore, QCheckBox *teamCheckBox)
{
    if (ui->UpdateScoreCheckbox->isChecked()) {
        int score = teamScore->value();
        if (teamCheckBox->isChecked()) {
            teamScore->setValue(score + 1);
        } else {
            teamScore->setValue(score == 0 ? 0 : score - 1);
        }
    }
}

void MainWindow::infoSection_init()
{
    // Logo Combo
    ComboBoxWidget *logoCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->LogoCombo->objectName()])};
    logoCombo->init(this->directoryFor["/ImportingFilesDirectory.txt"], ui->LogoCombo, this->directoryFor["/LogosDirectory.txt"]);
    // Title
    LineEditWidget *title {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->TitleEdit->objectName()])};
    title->init(this->directoryFor["/ImportingFilesDirectory.txt"], ui->TitleEdit);
    // Subtitle
    LineEditWidget *subtitle {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->SubtitleEdit->objectName()])};
    subtitle->init(this->directoryFor["/ImportingFilesDirectory.txt"], ui->SubtitleEdit);
    // Twitter
    LineEditWidget *twitter {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->TwitterEdit->objectName()])};
    twitter->init(this->directoryFor["/ImportingFilesDirectory.txt"], ui->TwitterEdit);
    // Message
    PlainTextWidget *message {dynamic_cast<PlainTextWidget *>(this->widgetFor[ui->MessageEdit->objectName()])};
    message->init(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MessageEdit);
}

void MainWindow::TeamsSection_init()
{
    QString importDir = this->directoryFor["/ImportingFilesDirectory.txt"];
    QString defaultDir = this->directoryFor["/DefaultDirectory.txt"];
    // Team Alpha
    QString alphaDir = this->directoryFor["/AlphaTeamDirectory.txt"];
    LineEditWidget *alphaName {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Team1NameEdit->objectName()])};
    LabelWidget *alphaLogo {dynamic_cast<LabelWidget *>(this->widgetFor[ui->Team1Logo->objectName()])};
    SpinBoxWidget *alphaScore {dynamic_cast<SpinBoxWidget *>(this->widgetFor[ui->Team1Score->objectName()])};
    // Set Team Alpha Autocomplete
    QStringList team1CompletionList = this->getListOf(alphaDir);
    this->Team1Completer = new QCompleter(team1CompletionList, this);
    this->Team1Completer->setCaseSensitivity(Qt::CaseInsensitive);
    this->Team1Completer->setFilterMode(Qt::MatchContains);
    ui->Team1NameEdit->setCompleter(this->Team1Completer);
    // Team Alpha Name
    alphaName->init(importDir, ui->Team1NameEdit);
    // Team Alpha Logo
    alphaLogo->init(importDir, ui->Team1Logo, alphaDir + "/" + ui->Team1NameEdit->text(), defaultDir);
    // Team Alpha Score
    alphaScore->init(importDir, ui->Team1Score);
    // Team Alpha Region
    ComboBoxWidget *alphaRegion {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->Team1RegionCombo->objectName()])};
    alphaRegion->init(importDir, ui->Team1RegionCombo, this->directoryFor["/RegionsDirectory.txt"]);

    // Team Beta
    QString betaDir = this->directoryFor["/BetaTeamDirectory.txt"];
    LineEditWidget *betaName {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Team2NameEdit->objectName()])};
    LabelWidget *betaLogo {dynamic_cast<LabelWidget *>(this->widgetFor[ui->Team2Logo->objectName()])};
    SpinBoxWidget *betaScore {dynamic_cast<SpinBoxWidget *>(this->widgetFor[ui->Team2Score->objectName()])};
    // Set Team Beta Autocomplete
    QStringList team2CompletionList = this->getListOf(betaDir);
    this->Team2Completer = new QCompleter(team2CompletionList, this);
    this->Team2Completer->setCaseSensitivity(Qt::CaseInsensitive);
    this->Team2Completer->setFilterMode(Qt::MatchContains);
    ui->Team2NameEdit->setCompleter(this->Team2Completer);
    // Team Beta Name
    betaName->init(importDir, ui->Team2NameEdit);
    // Team Beta Logo
    betaLogo->init(importDir, ui->Team2Logo, betaDir + "/" + ui->Team2NameEdit->text(), defaultDir);
    // Team Beta Score
    betaScore->init(importDir, ui->Team2Score);
    // Team Beta Region
    ComboBoxWidget *betaRegion {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->Team2RegionCombo->objectName()])};
    betaRegion->init(importDir, ui->Team2RegionCombo, this->directoryFor["/RegionsDirectory.txt"]);
}

void MainWindow::RoundSection_init()
{ 
    QString importDir = this->directoryFor["/ImportingFilesDirectory.txt"];
    // Color Group Combo
    const QStringList colorGroupsList = {"Ranked", "Turf War", "Splatfest"};
    ComboBoxWidget *colorGroup {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ColorGroupCombo->objectName()])};
    colorGroup->init(importDir, ui->ColorGroupCombo, colorGroupsList);
    // Colors Combo
    QStringList colorsList;
    QString comboString;
    QString indivString;
    QDir comboDir;
    QDir indivDir;
    if (ui->ColorGroupCombo->currentData().toString() == "Ranked") {
        // Check if combo or individual
        comboString = this->directoryFor["/RankedColorsDirectory.txt"] + "/combo";
        indivString = this->directoryFor["/RankedColorsDirectory.txt"] + "/alpha";
    } else if (ui->ColorGroupCombo->currentData().toString() == "Turf War") {
        // Check if combo or individual
        comboString = this->directoryFor["/TurfWarColorsDirectory.txt"] + "/combo";
        indivString = this->directoryFor["/TurfWarColorsDirectory.txt"] + "/alpha";
    } else { // Splatfest
        // Check if combo or individual
        comboString = this->directoryFor["/SplatfestColorsDirectory.txt"] + "/combo";
        indivString = this->directoryFor["/SplatfestColorsDirectory.txt"] + "/alpha";
    }
    comboDir.setPath(comboString);
    indivDir.setPath(indivString);
    if (comboDir.exists() && !comboDir.isEmpty()) {
        colorsList = this->getListOf(comboString);
    } else if (indivDir.exists() && !indivDir.isEmpty()) {
        colorsList = this->getListOf(indivString);
    }
    ui->ColorsCombo->addItems(colorsList);
    // Round
    LineEditWidget *round {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->RoundEdit->objectName()])};
    round->init(importDir, ui->RoundEdit);
}

void MainWindow::CastersSection_init()
{
    // Caster 1 Name
    LineEditWidget *caster1Name {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Caster1NameEdit->objectName()])};
    caster1Name->init(this->directoryFor["/ImportingFilesDirectory.txt"], ui->Caster1NameEdit);
    // Caster 2 Name
    LineEditWidget *caster2Name {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Caster2NameEdit->objectName()])};
    caster2Name->init(this->directoryFor["/ImportingFilesDirectory.txt"], ui->Caster2NameEdit);
    // Caster 3 Name
    LineEditWidget *caster3Name {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Caster3NameEdit->objectName()])};
    caster3Name->init(this->directoryFor["/ImportingFilesDirectory.txt"], ui->Caster3NameEdit);
    // Caster 1 Twitter
    LineEditWidget *caster1Twitter {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Caster1TwitterEdit->objectName()])};
    caster1Twitter->init(this->directoryFor["/ImportingFilesDirectory.txt"], ui->Caster1TwitterEdit);
    // Caster 2 Twitter
    LineEditWidget *caster2Twitter {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Caster2TwitterEdit->objectName()])};
    caster2Twitter->init(this->directoryFor["/ImportingFilesDirectory.txt"], ui->Caster2TwitterEdit);
    // Caster 3 Twitter
    LineEditWidget *caster3Twitter {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Caster3TwitterEdit->objectName()])};
    caster3Twitter->init(this->directoryFor["/ImportingFilesDirectory.txt"], ui->Caster3TwitterEdit);
}

void MainWindow::SetSection_init()
{
    QString importDir = this->directoryFor["/ImportingFilesDirectory.txt"];
    // Mode combos
    ComboBoxWidget *modeGame1 {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame1->objectName()])};
    modeGame1->init(importDir, ui->ModeComboGame1, this->directoryFor["/ModeIconsDirectory.txt"]);
    ComboBoxWidget *modeGame2 {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame2->objectName()])};
    modeGame2->init(importDir, ui->ModeComboGame2, this->directoryFor["/ModeIconsDirectory.txt"]);
    ComboBoxWidget *modeGame3 {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame3->objectName()])};
    modeGame3->init(importDir, ui->ModeComboGame3, this->directoryFor["/ModeIconsDirectory.txt"]);
    ComboBoxWidget *modeGame4 {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame4->objectName()])};
    modeGame4->init(importDir, ui->ModeComboGame4, this->directoryFor["/ModeIconsDirectory.txt"]);
    ComboBoxWidget *modeGame5 {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame5->objectName()])};
    modeGame5->init(importDir, ui->ModeComboGame5, this->directoryFor["/ModeIconsDirectory.txt"]);
    ComboBoxWidget *modeGame6 {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame6->objectName()])};
    modeGame6->init(importDir, ui->ModeComboGame6, this->directoryFor["/ModeIconsDirectory.txt"]);
    ComboBoxWidget *modeGame7 {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame7->objectName()])};
    modeGame7->init(importDir, ui->ModeComboGame7, this->directoryFor["/ModeIconsDirectory.txt"]);
    ComboBoxWidget *modeGame8 {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame8->objectName()])};
    modeGame8->init(importDir, ui->ModeComboGame8, this->directoryFor["/ModeIconsDirectory.txt"]);
    ComboBoxWidget *modeGame9 {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame9->objectName()])};
    modeGame9->init(importDir, ui->ModeComboGame9, this->directoryFor["/ModeIconsDirectory.txt"]);
    // Map Combos
    QString mapsDir = this->directoryFor["/MapsDirectory.txt"];
    QStringList map1CompletionList = this->getListOf(mapsDir);
    this->MapCompleter = new QCompleter(map1CompletionList, this);
    this->MapCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    this->MapCompleter->setFilterMode(Qt::MatchContains);
    ui->MapComboGame1->setCompleter(this->MapCompleter);
    LineEditWidget *mapName1 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame1->objectName()])};
    mapName1->init(importDir, ui->MapComboGame1, ".txt");
    ui->MapComboGame2->setCompleter(this->MapCompleter);
    LineEditWidget *mapName2 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame2->objectName()])};
    mapName2->init(importDir, ui->MapComboGame2, ".txt");
    ui->MapComboGame3->setCompleter(this->MapCompleter);
    LineEditWidget *mapName3 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame3->objectName()])};
    mapName3->init(importDir, ui->MapComboGame3, ".txt");
    ui->MapComboGame4->setCompleter(this->MapCompleter);
    LineEditWidget *mapName4 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame4->objectName()])};
    mapName4->init(importDir, ui->MapComboGame4, ".txt");
    ui->MapComboGame5->setCompleter(this->MapCompleter);
    LineEditWidget *mapName5 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame5->objectName()])};
    mapName5->init(importDir, ui->MapComboGame5, ".txt");
    ui->MapComboGame6->setCompleter(this->MapCompleter);
    LineEditWidget *mapName6 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame6->objectName()])};
    mapName6->init(importDir, ui->MapComboGame6, ".txt");
    ui->MapComboGame7->setCompleter(this->MapCompleter);
    LineEditWidget *mapName7 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame7->objectName()])};
    mapName7->init(importDir, ui->MapComboGame7, ".txt");
    ui->MapComboGame8->setCompleter(this->MapCompleter);
    LineEditWidget *mapName8 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame8->objectName()])};
    mapName8->init(importDir, ui->MapComboGame8, ".txt");
    ui->MapComboGame9->setCompleter(this->MapCompleter);
    LineEditWidget *mapName9 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame9->objectName()])};
    mapName9->init(importDir, ui->MapComboGame9, ".txt");
    // Game Radio Buttons
    RadioButtonWidget *game1 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame1->objectName()])};
    game1->init(importDir, ui->RadioGame1, "1");
    RadioButtonWidget *game2 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame2->objectName()])};
    game2->init(importDir, ui->RadioGame2, "2");
    RadioButtonWidget *game3 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame3->objectName()])};
    game3->init(importDir, ui->RadioGame3, "3");
    RadioButtonWidget *game4 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame4->objectName()])};
    game4->init(importDir, ui->RadioGame4, "4");
    RadioButtonWidget *game5 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame5->objectName()])};
    game5->init(importDir, ui->RadioGame5, "5");
    RadioButtonWidget *game6 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame6->objectName()])};
    game6->init(importDir, ui->RadioGame6, "6");
    RadioButtonWidget *game7 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame7->objectName()])};
    game7->init(importDir, ui->RadioGame7, "7");
    RadioButtonWidget *game8 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame8->objectName()])};
    game8->init(importDir, ui->RadioGame8, "8");
    RadioButtonWidget *game9 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame9->objectName()])};
    game9->init(importDir, ui->RadioGame9, "9");
    // alpha Game Winner Checkboxes
    CheckBoxWidget *alphaWin1 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame1->objectName()])};
    alphaWin1->init(importDir, ui->AlphaWinGame1, ui->Team1NameEdit->text());
    CheckBoxWidget *alphaWin2 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame2->objectName()])};
    alphaWin2->init(importDir, ui->AlphaWinGame2, ui->Team1NameEdit->text());
    CheckBoxWidget *alphaWin3 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame3->objectName()])};
    alphaWin3->init(importDir, ui->AlphaWinGame3, ui->Team1NameEdit->text());
    CheckBoxWidget *alphaWin4 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame4->objectName()])};
    alphaWin4->init(importDir, ui->AlphaWinGame4, ui->Team1NameEdit->text());
    CheckBoxWidget *alphaWin5 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame5->objectName()])};
    alphaWin5->init(importDir, ui->AlphaWinGame5, ui->Team1NameEdit->text());
    CheckBoxWidget *alphaWin6 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame6->objectName()])};
    alphaWin6->init(importDir, ui->AlphaWinGame6, ui->Team1NameEdit->text());
    CheckBoxWidget *alphaWin7 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame7->objectName()])};
    alphaWin7->init(importDir, ui->AlphaWinGame7, ui->Team1NameEdit->text());
    CheckBoxWidget *alphaWin8 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame8->objectName()])};
    alphaWin8->init(importDir, ui->AlphaWinGame8, ui->Team1NameEdit->text());
    CheckBoxWidget *alphaWin9 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame9->objectName()])};
    alphaWin9->init(importDir, ui->AlphaWinGame9, ui->Team1NameEdit->text());
    // Beta Game Winner Checkboxes
    CheckBoxWidget *betaWin1 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame1->objectName()])};
    betaWin1->init(importDir, ui->BetaWinGame1, ui->Team2NameEdit->text());
    CheckBoxWidget *betaWin2 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame2->objectName()])};
    betaWin2->init(importDir, ui->BetaWinGame2, ui->Team2NameEdit->text());
    CheckBoxWidget *betaWin3 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame3->objectName()])};
    betaWin3->init(importDir, ui->BetaWinGame3, ui->Team2NameEdit->text());
    CheckBoxWidget *betaWin4 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame4->objectName()])};
    betaWin4->init(importDir, ui->BetaWinGame4, ui->Team2NameEdit->text());
    CheckBoxWidget *betaWin5 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame5->objectName()])};
    betaWin5->init(importDir, ui->BetaWinGame5, ui->Team2NameEdit->text());
    CheckBoxWidget *betaWin6 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame6->objectName()])};
    betaWin6->init(importDir, ui->BetaWinGame6, ui->Team2NameEdit->text());
    CheckBoxWidget *betaWin7 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame7->objectName()])};
    betaWin7->init(importDir, ui->BetaWinGame7, ui->Team2NameEdit->text());
    CheckBoxWidget *betaWin8 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame8->objectName()])};
    betaWin8->init(importDir, ui->BetaWinGame8, ui->Team2NameEdit->text());
    CheckBoxWidget *betaWin9 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame9->objectName()])};
    betaWin9->init(importDir, ui->BetaWinGame9, ui->Team2NameEdit->text());
}

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
    QString importDir = this->directoryFor["/ImportingFilesDirectory.txt"];
    QString defaultDir = this->directoryFor["/DefaultDirectory.txt"];
    QString alphaDir = this->directoryFor["/AlphaTeamDirectory.txt"];
    LineEditWidget *name {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Team1NameEdit->objectName()])};
    LabelWidget *logo {dynamic_cast<LabelWidget *>(this->widgetFor[ui->Team1Logo->objectName()])};

    name->saveInFile(importDir, ui->Team1NameEdit);
    logo->init(importDir, ui->Team1Logo, alphaDir + "/" + ui->Team1NameEdit->text(), defaultDir);
}

void MainWindow::on_Team1RegionCombo_activated(int index)
{
    QString local = this->directoryFor["/RegionsDirectory.txt"];
    ComboBoxWidget *regionsCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->Team1RegionCombo->objectName()])};
    regionsCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->Team1RegionCombo, "no image", local);
}

/*
/ END Team Alpha Section
*/

/*
/ Team Beta Section
*/

void MainWindow::on_Team2NameEdit_editingFinished()
{
    QString importDir = this->directoryFor["/ImportingFilesDirectory.txt"];
    QString defaultDir = this->directoryFor["/DefaultDirectory.txt"];
    QString betaDir = this->directoryFor["/BetaTeamDirectory.txt"];
    LineEditWidget *name {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Team2NameEdit->objectName()])};
    LabelWidget *logo {dynamic_cast<LabelWidget *>(this->widgetFor[ui->Team2Logo->objectName()])};

    name->saveInFile(importDir, ui->Team2NameEdit);
    logo->init(importDir, ui->Team2Logo, betaDir + "/" + ui->Team2NameEdit->text(), defaultDir);
}

void MainWindow::on_Team2RegionCombo_activated(int index)
{
    QString local = this->directoryFor["/RegionsDirectory.txt"];
    ComboBoxWidget *regionsCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->Team2RegionCombo->objectName()])};
    regionsCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->Team2RegionCombo, "no image", local);
}

/*
/ END Team Beta Section
*/

/*
/ Round Section
*/

void MainWindow::on_RoundEdit_editingFinished()
{
    LineEditWidget *round {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->RoundEdit->objectName()])};
    round->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->RoundEdit);
}

void MainWindow::on_ColorGroupCombo_activated(int index)
{
    ComboBoxWidget *colorGroupCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ColorGroupCombo->objectName()])};
    colorGroupCombo->saveContentInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->ColorGroupCombo);
}


void MainWindow::on_Team1Score_textChanged(const QString &arg1)
{
    SpinBoxWidget *score {dynamic_cast<SpinBoxWidget *>(this->widgetFor[ui->Team1Score->objectName()])};
    score->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->Team1Score);
}

void MainWindow::on_Team2Score_textChanged(const QString &arg1)
{
    SpinBoxWidget *score {dynamic_cast<SpinBoxWidget *>(this->widgetFor[ui->Team2Score->objectName()])};
    score->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->Team2Score);
}

void MainWindow::on_ResetScoresButton_clicked()
{
    ui->Team1Score->setValue(0);
    ui->Team2Score->setValue(0);
    ui->AlphaWinGame1->setChecked(false);
    CheckBoxWidget *alphaWin1 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame1->objectName()])};
    alphaWin1->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame1, "/teamAlpha/");
    ui->AlphaWinGame2->setChecked(false);
    CheckBoxWidget *alphaWin2 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame2->objectName()])};
    alphaWin2->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame2, "/teamAlpha/");
    ui->AlphaWinGame3->setChecked(false);
    CheckBoxWidget *alphaWin3 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame3->objectName()])};
    alphaWin3->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame3, "/teamAlpha/");
    ui->AlphaWinGame4->setChecked(false);
    CheckBoxWidget *alphaWin4 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame4->objectName()])};
    alphaWin4->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame4, "/teamAlpha/");
    ui->AlphaWinGame5->setChecked(false);
    CheckBoxWidget *alphaWin5 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame5->objectName()])};
    alphaWin5->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame5, "/teamAlpha/");
    ui->AlphaWinGame6->setChecked(false);
    CheckBoxWidget *alphaWin6 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame6->objectName()])};
    alphaWin6->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame6, "/teamAlpha/");
    ui->AlphaWinGame7->setChecked(false);
    CheckBoxWidget *alphaWin7 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame7->objectName()])};
    alphaWin7->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame7, "/teamAlpha/");
    ui->AlphaWinGame8->setChecked(false);
    CheckBoxWidget *alphaWin8 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame8->objectName()])};
    alphaWin8->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame8, "/teamAlpha/");
    ui->AlphaWinGame9->setChecked(false);
    CheckBoxWidget *alphaWin9 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame9->objectName()])};
    alphaWin9->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame9, "/teamAlpha/");
    ui->BetaWinGame1->setChecked(false);
    CheckBoxWidget *betaWin1 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame1->objectName()])};
    betaWin1->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BetaWinGame1, "/teamBeta/");
    ui->BetaWinGame2->setChecked(false);
    CheckBoxWidget *betaWin2 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame2->objectName()])};
    betaWin2->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BetaWinGame2, "/teamBeta/");
    ui->BetaWinGame3->setChecked(false);
    CheckBoxWidget *betaWin3 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame3->objectName()])};
    betaWin3->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BetaWinGame3, "/teamBeta/");
    ui->BetaWinGame4->setChecked(false);
    CheckBoxWidget *betaWin4 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame4->objectName()])};
    betaWin4->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BetaWinGame4, "/teamBeta/");
    ui->BetaWinGame5->setChecked(false);
    CheckBoxWidget *betaWin5 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame5->objectName()])};
    betaWin5->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BetaWinGame5, "/teamBeta/");
    ui->BetaWinGame6->setChecked(false);
    CheckBoxWidget *betaWin6 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame6->objectName()])};
    betaWin6->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BetaWinGame6, "/teamBeta/");
    ui->BetaWinGame7->setChecked(false);
    CheckBoxWidget *betaWin7 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame7->objectName()])};
    betaWin7->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BetaWinGame7, "/teamBeta/");
    ui->BetaWinGame8->setChecked(false);
    CheckBoxWidget *betaWin8 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame8->objectName()])};
    betaWin8->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BetaWinGame8, "/teamBeta/");
    ui->BetaWinGame9->setChecked(false);
    CheckBoxWidget *betaWin9 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame9->objectName()])};
    betaWin9->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BetaWinGame9, "/teamBeta/");
    ui->RadioGame1->setChecked(true);
}

void MainWindow::on_UpdateScoreCheckbox_stateChanged(int arg1)
{
    if (arg1) {
        ui->Team1Score->setDisabled(true);
        ui->Team2Score->setDisabled(true);
        int count = 0;
        if (ui->AlphaWinGame1->isChecked()) count++;
        if (ui->AlphaWinGame2->isChecked()) count++;
        if (ui->AlphaWinGame3->isChecked()) count++;
        if (ui->AlphaWinGame4->isChecked()) count++;
        if (ui->AlphaWinGame5->isChecked()) count++;
        if (ui->AlphaWinGame6->isChecked()) count++;
        if (ui->AlphaWinGame7->isChecked()) count++;
        if (ui->AlphaWinGame8->isChecked()) count++;
        if (ui->AlphaWinGame9->isChecked()) count++;
        ui->Team1Score->setValue(count);
        count = 0;
        if (ui->BetaWinGame1->isChecked()) count++;
        if (ui->BetaWinGame2->isChecked()) count++;
        if (ui->BetaWinGame3->isChecked()) count++;
        if (ui->BetaWinGame4->isChecked()) count++;
        if (ui->BetaWinGame5->isChecked()) count++;
        if (ui->BetaWinGame6->isChecked()) count++;
        if (ui->BetaWinGame7->isChecked()) count++;
        if (ui->BetaWinGame8->isChecked()) count++;
        if (ui->BetaWinGame9->isChecked()) count++;
        ui->Team2Score->setValue(count);
    } else {
        ui->Team1Score->setDisabled(false);
        ui->Team2Score->setDisabled(false);
    }
}

void MainWindow::on_UpdateRoundCheckbox_stateChanged(int arg1)
{
    if (ui->UpdateRoundCheckbox->isChecked()) {
        ui->RadioGame1->setDisabled(true);
        ui->RadioGame2->setDisabled(true);
        ui->RadioGame3->setDisabled(true);
        ui->RadioGame4->setDisabled(true);
        ui->RadioGame5->setDisabled(true);
        ui->RadioGame6->setDisabled(true);
        ui->RadioGame7->setDisabled(true);
        ui->RadioGame8->setDisabled(true);
        ui->RadioGame9->setDisabled(true);
        QRadioButton *currentGame = ui->RadioGame1;
        QCheckBox *alphaGames[9] = {
            ui->AlphaWinGame1, ui->AlphaWinGame2, ui->AlphaWinGame3, ui->AlphaWinGame4,
            ui->AlphaWinGame5, ui->AlphaWinGame6, ui->AlphaWinGame7, ui->AlphaWinGame8, ui->AlphaWinGame9
        };
        QCheckBox *betaGames[9] = {
            ui->BetaWinGame1, ui->BetaWinGame2, ui->BetaWinGame3, ui->BetaWinGame4,
            ui->BetaWinGame5, ui->BetaWinGame6, ui->BetaWinGame7, ui->BetaWinGame8, ui->BetaWinGame9
        };
        QRadioButton *rounds[9] = {
            ui->RadioGame1, ui->RadioGame2, ui->RadioGame3, ui->RadioGame4,
            ui->RadioGame5, ui->RadioGame6, ui->RadioGame7, ui->RadioGame8, ui->RadioGame9
        };
        for (int i = 0; i < 9; i++) {
            if (!alphaGames[i]->isChecked() && !betaGames[i]->isChecked()) {
                currentGame = rounds[i];
                break;
            }
        }
        currentGame->setChecked(true);
    } else {
        ui->RadioGame1->setDisabled(false);
        ui->RadioGame2->setDisabled(false);
        ui->RadioGame3->setDisabled(false);
        ui->RadioGame4->setDisabled(false);
        ui->RadioGame5->setDisabled(false);
        ui->RadioGame6->setDisabled(false);
        ui->RadioGame7->setDisabled(false);
        ui->RadioGame8->setDisabled(false);
        ui->RadioGame9->setDisabled(false);
    }
}

/*
/ END Round Section
*/

/*
/ Info Section
*/

void MainWindow::on_TitleEdit_editingFinished()
{
    LineEditWidget *title {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->TitleEdit->objectName()])};
    title->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->TitleEdit);
}

void MainWindow::on_SubtitleEdit_editingFinished()
{
    LineEditWidget *subtitle {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->SubtitleEdit->objectName()])};
    subtitle->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->SubtitleEdit);
}

void MainWindow::on_TwitterEdit_editingFinished()
{
    LineEditWidget *twitter {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->TwitterEdit->objectName()])};
    twitter->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->TwitterEdit);
}

void MainWindow::on_MessageEdit_textChanged()
{
    PlainTextWidget *twitter {dynamic_cast<PlainTextWidget *>(this->widgetFor[ui->MessageEdit->objectName()])};
    twitter->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MessageEdit);
}

void MainWindow::on_LogoCombo_activated(int index)
{
    QString local = this->directoryFor["/LogosDirectory.txt"];
    ComboBoxWidget *logoCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->LogoCombo->objectName()])};
    logoCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->LogoCombo, "image", local);
}

/*
/ END Info Section
*/

/*
/ Casters Section
*/

void MainWindow::on_Caster1NameEdit_editingFinished()
{
    LineEditWidget *name {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Caster1NameEdit->objectName()])};
    name->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->Caster1NameEdit);
}


void MainWindow::on_Caster2NameEdit_editingFinished()
{
    LineEditWidget *name {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Caster2NameEdit->objectName()])};
    name->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->Caster2NameEdit);
}


void MainWindow::on_Caster3NameEdit_editingFinished()
{
    LineEditWidget *name {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Caster3NameEdit->objectName()])};
    name->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->Caster3NameEdit);
}


void MainWindow::on_Caster1TwitterEdit_editingFinished()
{
    LineEditWidget *twitter {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Caster1TwitterEdit->objectName()])};
    twitter->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->Caster1TwitterEdit);
}


void MainWindow::on_Caster2TwitterEdit_editingFinished()
{
    LineEditWidget *twitter {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Caster2TwitterEdit->objectName()])};
    twitter->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->Caster2TwitterEdit);
}


void MainWindow::on_Caster3TwitterEdit_editingFinished()
{
    LineEditWidget *twitter {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Caster3TwitterEdit->objectName()])};
    twitter->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->Caster3TwitterEdit);
}
/*
/ END Casters Section
*/

/*
/ Set Section
*/

void MainWindow::on_ModeComboGame1_activated(int index)
{
    QString local = this->directoryFor["/ModeIconsDirectory.txt"];
    ComboBoxWidget *modeCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame1->objectName()])};
    modeCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->ModeComboGame1, "image", local);
    if (ui->RadioGame1->isChecked()) {
        this->on_RadioGame1_clicked();
    }
}

void MainWindow::on_ModeComboGame2_activated(int index)
{
    QString local = this->directoryFor["/ModeIconsDirectory.txt"];
    ComboBoxWidget *modeCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame2->objectName()])};
    modeCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->ModeComboGame2, "image", local);
    if (ui->RadioGame2->isChecked()) {
        this->on_RadioGame2_clicked();
    }
}

void MainWindow::on_ModeComboGame3_activated(int index)
{
    QString local = this->directoryFor["/ModeIconsDirectory.txt"];
    ComboBoxWidget *modeCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame3->objectName()])};
    modeCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->ModeComboGame3, "image", local);
    if (ui->RadioGame3->isChecked()) {
        this->on_RadioGame3_clicked();
    }
}

void MainWindow::on_ModeComboGame4_activated(int index)
{
    QString local = this->directoryFor["/ModeIconsDirectory.txt"];
    ComboBoxWidget *modeCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame4->objectName()])};
    modeCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->ModeComboGame4, "image", local);
    if (ui->RadioGame4->isChecked()) {
        this->on_RadioGame4_clicked();
    }
}

void MainWindow::on_ModeComboGame5_activated(int index)
{
    QString local = this->directoryFor["/ModeIconsDirectory.txt"];
    ComboBoxWidget *modeCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame5->objectName()])};
    modeCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->ModeComboGame5, "image", local);
    if (ui->RadioGame5->isChecked()) {
        this->on_RadioGame5_clicked();
    }
}

void MainWindow::on_ModeComboGame6_activated(int index)
{
    QString local = this->directoryFor["/ModeIconsDirectory.txt"];
    ComboBoxWidget *modeCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame6->objectName()])};
    modeCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->ModeComboGame6, "image", local);
    if (ui->RadioGame6->isChecked()) {
        this->on_RadioGame6_clicked();
    }
}

void MainWindow::on_ModeComboGame7_activated(int index)
{
    QString local = this->directoryFor["/ModeIconsDirectory.txt"];
    ComboBoxWidget *modeCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame7->objectName()])};
    modeCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->ModeComboGame7, "image", local);
    if (ui->RadioGame7->isChecked()) {
        this->on_RadioGame7_clicked();
    }
}

void MainWindow::on_ModeComboGame8_activated(int index)
{
    QString local = this->directoryFor["/ModeIconsDirectory.txt"];
    ComboBoxWidget *modeCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame8->objectName()])};
    modeCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->ModeComboGame8, "image", local);
    if (ui->RadioGame8->isChecked()) {
        this->on_RadioGame8_clicked();
    }
}

void MainWindow::on_ModeComboGame9_activated(int index)
{
    QString local = this->directoryFor["/ModeIconsDirectory.txt"];
    ComboBoxWidget *modeCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame9->objectName()])};
    modeCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->ModeComboGame9, "image", local);
    if (ui->RadioGame9->isChecked()) {
        this->on_RadioGame9_clicked();
    }
}

void MainWindow::on_MapComboGame1_editingFinished()
{
    QString local = this->directoryFor["/MapsDirectory.txt"];
    LineEditWidget *mapCombo1 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame1->objectName()])};
    mapCombo1->saveWithImage(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MapComboGame1, local);
    if (ui->RadioGame1->isChecked()) {
        this->on_RadioGame1_clicked();
    }
}

void MainWindow::on_MapComboGame2_editingFinished()
{
    QString local = this->directoryFor["/MapsDirectory.txt"];
    LineEditWidget *mapCombo2 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame2->objectName()])};
    mapCombo2->saveWithImage(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MapComboGame2, local);
    if (ui->RadioGame2->isChecked()) {
        this->on_RadioGame2_clicked();
    }
}

void MainWindow::on_MapComboGame3_editingFinished()
{
    QString local = this->directoryFor["/MapsDirectory.txt"];
    LineEditWidget *mapCombo3 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame3->objectName()])};
    mapCombo3->saveWithImage(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MapComboGame3, local);
    if (ui->RadioGame3->isChecked()) {
        this->on_RadioGame3_clicked();
    }
}

void MainWindow::on_MapComboGame4_editingFinished()
{
    QString local = this->directoryFor["/MapsDirectory.txt"];
    LineEditWidget *mapCombo4 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame4->objectName()])};
    mapCombo4->saveWithImage(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MapComboGame4, local);
    if (ui->RadioGame4->isChecked()) {
        this->on_RadioGame4_clicked();
    }
}

void MainWindow::on_MapComboGame5_editingFinished()
{
    QString local = this->directoryFor["/MapsDirectory.txt"];
    LineEditWidget *mapCombo5 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame5->objectName()])};
    mapCombo5->saveWithImage(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MapComboGame5, local);
    if (ui->RadioGame5->isChecked()) {
        this->on_RadioGame5_clicked();
    }
}

void MainWindow::on_MapComboGame6_editingFinished()
{
    QString local = this->directoryFor["/MapsDirectory.txt"];
    LineEditWidget *mapCombo6 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame6->objectName()])};
    mapCombo6->saveWithImage(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MapComboGame6, local);
    if (ui->RadioGame6->isChecked()) {
        this->on_RadioGame6_clicked();
    }
}

void MainWindow::on_MapComboGame7_editingFinished()
{
    QString local = this->directoryFor["/MapsDirectory.txt"];
    LineEditWidget *mapCombo7 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame7->objectName()])};
    mapCombo7->saveWithImage(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MapComboGame7, local);
    if (ui->RadioGame7->isChecked()) {
        this->on_RadioGame7_clicked();
    }
}

void MainWindow::on_MapComboGame8_editingFinished()
{
    QString local = this->directoryFor["/MapsDirectory.txt"];
    LineEditWidget *mapCombo8 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame8->objectName()])};
    mapCombo8->saveWithImage(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MapComboGame8, local);
    if (ui->RadioGame8->isChecked()) {
        this->on_RadioGame8_clicked();
    }
}

void MainWindow::on_MapComboGame9_editingFinished()
{
    QString local = this->directoryFor["/MapsDirectory.txt"];
    LineEditWidget *mapCombo9 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame9->objectName()])};
    mapCombo9->saveWithImage(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MapComboGame9, local);
    if (ui->RadioGame9->isChecked()) {
        this->on_RadioGame9_clicked();
    }
}

void MainWindow::on_RadioGame1_clicked()
{
    RadioButtonWidget *game1 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame1->objectName()])};
    game1->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], "1");
}

void MainWindow::on_RadioGame2_clicked()
{
    RadioButtonWidget *game2 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame2->objectName()])};
    game2->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], "2");
}

void MainWindow::on_RadioGame3_clicked()
{
    RadioButtonWidget *game3 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame3->objectName()])};
    game3->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], "3");
}

void MainWindow::on_RadioGame4_clicked()
{
    RadioButtonWidget *game4 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame4->objectName()])};
    game4->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], "4");
}

void MainWindow::on_RadioGame5_clicked()
{
    RadioButtonWidget *game5 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame5->objectName()])};
    game5->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], "5");
}

void MainWindow::on_RadioGame6_clicked()
{
    RadioButtonWidget *game6 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame6->objectName()])};
    game6->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], "6");
}

void MainWindow::on_RadioGame7_clicked()
{
    RadioButtonWidget *game7 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame7->objectName()])};
    game7->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], "7");
}

void MainWindow::on_RadioGame8_clicked()
{
    RadioButtonWidget *game8 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame8->objectName()])};
    game8->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], "8");
}

void MainWindow::on_RadioGame9_clicked()
{
    RadioButtonWidget *game9 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame9->objectName()])};
    game9->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], "9");
}

void MainWindow::on_AlphaWinGame1_clicked()
{
    CheckBoxWidget *alphaWin {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame1->objectName()])};
    alphaWin->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame1, "/teamAlpha/");
    ui->BetaWinGame1->setChecked(false);
    if (ui->RadioGame1->isChecked()) {
        this->on_RadioGame1_clicked();
        if (ui->UpdateRoundCheckbox->isChecked()) {
            ui->RadioGame2->setChecked(true);
            this->on_RadioGame2_clicked();
        }
    }
    this->scoreAutoUpdate(ui->Team1Score, ui->AlphaWinGame1);
}

void MainWindow::on_AlphaWinGame2_clicked()
{
    CheckBoxWidget *alphaWin {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame2->objectName()])};
    alphaWin->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame2, "/teamAlpha/");
    ui->BetaWinGame2->setChecked(false);
    if (ui->RadioGame2->isChecked()) {
        this->on_RadioGame2_clicked();
        if (ui->UpdateRoundCheckbox->isChecked()) {
            ui->RadioGame3->setChecked(true);
            this->on_RadioGame3_clicked();
        }
    }
    this->scoreAutoUpdate(ui->Team1Score, ui->AlphaWinGame2);
}

void MainWindow::on_AlphaWinGame3_clicked()
{
    CheckBoxWidget *alphaWin {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame3->objectName()])};
    alphaWin->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame3, "/teamAlpha/");
    ui->BetaWinGame3->setChecked(false);
    if (ui->RadioGame3->isChecked()) {
        this->on_RadioGame3_clicked();
        if (ui->UpdateRoundCheckbox->isChecked()) {
            ui->RadioGame4->setChecked(true);
            this->on_RadioGame4_clicked();
        }
    }
    this->scoreAutoUpdate(ui->Team1Score, ui->AlphaWinGame3);
}

void MainWindow::on_AlphaWinGame4_clicked()
{
    CheckBoxWidget *alphaWin {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame4->objectName()])};
    alphaWin->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame4, "/teamAlpha/");
    ui->BetaWinGame4->setChecked(false);
    if (ui->RadioGame4->isChecked()) {
        this->on_RadioGame4_clicked();
        if (ui->UpdateRoundCheckbox->isChecked()) {
            ui->RadioGame5->setChecked(true);
            this->on_RadioGame5_clicked();
        }
    }
    this->scoreAutoUpdate(ui->Team1Score, ui->AlphaWinGame4);
}

void MainWindow::on_AlphaWinGame5_clicked()
{
    CheckBoxWidget *alphaWin {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame5->objectName()])};
    alphaWin->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame5, "/teamAlpha/");
    ui->BetaWinGame5->setChecked(false);
    if (ui->RadioGame5->isChecked()) {
        this->on_RadioGame5_clicked();
        if (ui->UpdateRoundCheckbox->isChecked()) {
            ui->RadioGame6->setChecked(true);
            this->on_RadioGame6_clicked();
        }
    }
    this->scoreAutoUpdate(ui->Team1Score, ui->AlphaWinGame5);
}

void MainWindow::on_AlphaWinGame6_clicked()
{
    CheckBoxWidget *alphaWin {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame6->objectName()])};
    alphaWin->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame6, "/teamAlpha/");
    ui->BetaWinGame6->setChecked(false);
    if (ui->RadioGame6->isChecked()) {
        this->on_RadioGame6_clicked();
        if (ui->UpdateRoundCheckbox->isChecked()) {
            ui->RadioGame7->setChecked(true);
            this->on_RadioGame7_clicked();
        }
    }
    this->scoreAutoUpdate(ui->Team1Score, ui->AlphaWinGame6);
}

void MainWindow::on_AlphaWinGame7_clicked()
{
    CheckBoxWidget *alphaWin {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame7->objectName()])};
    alphaWin->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame7, "/teamAlpha/");
    ui->BetaWinGame7->setChecked(false);
    if (ui->RadioGame7->isChecked()) {
        this->on_RadioGame7_clicked();
        if (ui->UpdateRoundCheckbox->isChecked()) {
            ui->RadioGame8->setChecked(true);
            this->on_RadioGame8_clicked();
        }
    }
    this->scoreAutoUpdate(ui->Team1Score, ui->AlphaWinGame7);
}

void MainWindow::on_AlphaWinGame8_clicked()
{
    CheckBoxWidget *alphaWin {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame8->objectName()])};
    alphaWin->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame8, "/teamAlpha/");
    ui->BetaWinGame8->setChecked(false);
    if (ui->RadioGame8->isChecked()) {
        this->on_RadioGame8_clicked();
        if (ui->UpdateRoundCheckbox->isChecked()) {
            ui->RadioGame9->setChecked(true);
            this->on_RadioGame9_clicked();
        }
    }
    this->scoreAutoUpdate(ui->Team1Score, ui->AlphaWinGame8);
}

void MainWindow::on_AlphaWinGame9_clicked()
{
    CheckBoxWidget *alphaWin {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame9->objectName()])};
    alphaWin->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame9, "/teamAlpha/");
    ui->BetaWinGame9->setChecked(false);
    this->scoreAutoUpdate(ui->Team1Score, ui->AlphaWinGame9);
}

void MainWindow::on_BetaWinGame1_clicked()
{
    CheckBoxWidget *betaWin {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame1->objectName()])};
    betaWin->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BetaWinGame1, "/teamBeta/");
    ui->AlphaWinGame1->setChecked(false);
    if (ui->RadioGame1->isChecked()) {
        this->on_RadioGame1_clicked();
        if (ui->UpdateRoundCheckbox->isChecked()) {
            ui->RadioGame2->setChecked(true);
            this->on_RadioGame2_clicked();
        }
    }
    this->scoreAutoUpdate(ui->Team2Score, ui->BetaWinGame1);
}

void MainWindow::on_BetaWinGame2_clicked()
{
    CheckBoxWidget *betaWin {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame2->objectName()])};
    betaWin->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BetaWinGame2, "/teamBeta/");
    ui->AlphaWinGame2->setChecked(false);
    if (ui->RadioGame2->isChecked()) {
        this->on_RadioGame2_clicked();
        if (ui->UpdateRoundCheckbox->isChecked()) {
            ui->RadioGame3->setChecked(true);
            this->on_RadioGame3_clicked();
        }
    }
    this->scoreAutoUpdate(ui->Team2Score, ui->BetaWinGame2);
}

void MainWindow::on_BetaWinGame3_clicked()
{
    CheckBoxWidget *betaWin {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame3->objectName()])};
    betaWin->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BetaWinGame3, "/teamBeta/");
    ui->AlphaWinGame3->setChecked(false);
    if (ui->RadioGame3->isChecked()) {
        this->on_RadioGame3_clicked();
        if (ui->UpdateRoundCheckbox->isChecked()) {
            ui->RadioGame4->setChecked(true);
            this->on_RadioGame4_clicked();
        }
    }
    this->scoreAutoUpdate(ui->Team2Score, ui->BetaWinGame3);
}

void MainWindow::on_BetaWinGame4_clicked()
{
    CheckBoxWidget *betaWin {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame4->objectName()])};
    betaWin->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BetaWinGame4, "/teamBeta/");
    ui->AlphaWinGame4->setChecked(false);
    if (ui->RadioGame4->isChecked()) {
        this->on_RadioGame4_clicked();
        if (ui->UpdateRoundCheckbox->isChecked()) {
            ui->RadioGame5->setChecked(true);
            this->on_RadioGame5_clicked();
        }
    }
    this->scoreAutoUpdate(ui->Team2Score, ui->BetaWinGame4);
}

void MainWindow::on_BetaWinGame5_clicked()
{
    CheckBoxWidget *betaWin {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame5->objectName()])};
    betaWin->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BetaWinGame5, "/teamBeta/");
    ui->AlphaWinGame5->setChecked(false);
    if (ui->RadioGame5->isChecked()) {
        this->on_RadioGame5_clicked();
        if (ui->UpdateRoundCheckbox->isChecked()) {
            ui->RadioGame6->setChecked(true);
            this->on_RadioGame6_clicked();
        }
    }
    this->scoreAutoUpdate(ui->Team2Score, ui->BetaWinGame5);
}

void MainWindow::on_BetaWinGame6_clicked()
{
    CheckBoxWidget *betaWin {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame6->objectName()])};
    betaWin->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BetaWinGame6, "/teamBeta/");
    ui->AlphaWinGame6->setChecked(false);
    if (ui->RadioGame6->isChecked()) {
        this->on_RadioGame6_clicked();
        if (ui->UpdateRoundCheckbox->isChecked()) {
            ui->RadioGame7->setChecked(true);
            this->on_RadioGame7_clicked();
        }
    }
    this->scoreAutoUpdate(ui->Team2Score, ui->BetaWinGame6);
}

void MainWindow::on_BetaWinGame7_clicked()
{
    CheckBoxWidget *betaWin {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame7->objectName()])};
    betaWin->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BetaWinGame7, "/teamBeta/");
    ui->AlphaWinGame7->setChecked(false);
    if (ui->RadioGame7->isChecked()) {
        this->on_RadioGame7_clicked();
        if (ui->UpdateRoundCheckbox->isChecked()) {
            ui->RadioGame8->setChecked(true);
            this->on_RadioGame8_clicked();
        }
    }
    this->scoreAutoUpdate(ui->Team2Score, ui->BetaWinGame7);
}

void MainWindow::on_BetaWinGame8_clicked()
{
    CheckBoxWidget *betaWin {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame8->objectName()])};
    betaWin->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BetaWinGame8, "/teamBeta/");
    ui->AlphaWinGame8->setChecked(false);
    if (ui->RadioGame8->isChecked()) {
        this->on_RadioGame8_clicked();
        if (ui->UpdateRoundCheckbox->isChecked()) {
            ui->RadioGame9->setChecked(true);
            this->on_RadioGame9_clicked();
        }
    }
    this->scoreAutoUpdate(ui->Team2Score, ui->BetaWinGame8);
}

void MainWindow::on_BetaWinGame9_clicked()
{
    CheckBoxWidget *betaWin {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BetaWinGame9->objectName()])};
    betaWin->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BetaWinGame9, "/teamBeta/");
    ui->AlphaWinGame9->setChecked(false);
    this->scoreAutoUpdate(ui->Team2Score, ui->BetaWinGame9);
}

/*
/ END Set Section
*/

QMap<QString, MyWidget*> MainWindow::widgets_init()
{
    QString filenames[] = {
        "name.txt", "region", "logo", "color", "score.txt", "name.txt",
        "region", "logo", "color", "score.txt", "title.txt", "subtitle.txt", "twitter.txt",
        "message.txt", "tournamentLogo", "1Name.txt", "2Name.txt", "3Name.txt", "1Twitter.txt",
        "2Twitter.txt", "3Twitter.txt", "round.txt", "colorGroups", "color", "1",
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
        "/teamAlpha/", "/teamAlpha/", "/teamAlpha/", "/teamAlpha/", "/teamAlpha/", "/teamBeta/",
        "/teamBeta/", "/teamBeta/", "/teamBeta/", "/teamBeta/", "/info/", "/info/", "/info/",
        "/info/", "/info/", "/casters/", "/casters/", "/casters/", "/casters/",
        "/casters/", "/casters/", "/round/", "/round/", "/round/", "/set/winners/",
        "/set/winners/", "/set/winners/", "/set/winners/", "/set/winners/", "/set/winners/", "/set/winners/",
        "/set/winners/", "/set/winners/", "/set/winners/", "/set/winners/", "/set/winners/", "/set/winners/",
        "/set/winners/", "/set/winners/", "/set/winners/", "/set/winners/", "/set/winners/", "/set/",
        "/set/", "/set/", "/set/", "/set/", "/set/", "/set/",
        "/set/", "/set/", "/set/maps/", "/set/maps/", "/set/maps/", "/set/maps/",
        "/set/maps/", "/set/maps/", "/set/maps/", "/set/maps/", "/set/maps/", "/set/modes/",
        "/set/modes/", "/set/modes/", "/set/modes/", "/set/modes/", "/set/modes/", "/set/modes/",
        "/set/modes/", "/set/modes/",
    };
    MyWidget *arr[widAmount];
    arr[0] = new LineEditWidget(importDirs[0], filenames[0]);
    arr[1] = new ComboBoxWidget(importDirs[1], filenames[1]);
    arr[2] = new LabelWidget(importDirs[2], filenames[2]);
    arr[3] = new LabelWidget(importDirs[3], filenames[3]);
    arr[4] = new SpinBoxWidget(importDirs[4], filenames[4]);
    arr[5] = new LineEditWidget(importDirs[5], filenames[5]);
    arr[6] = new ComboBoxWidget(importDirs[6], filenames[6]);
    arr[7] = new LabelWidget(importDirs[7], filenames[7]);
    arr[8] = new LabelWidget(importDirs[8], filenames[8]);
    arr[9] = new SpinBoxWidget(importDirs[9], filenames[9]);
    arr[10] = new LineEditWidget(importDirs[10], filenames[10]);
    arr[11] = new LineEditWidget(importDirs[11], filenames[11]);
    arr[12] = new LineEditWidget(importDirs[12], filenames[12]);
    arr[13] = new PlainTextWidget(importDirs[13], filenames[13]);
    arr[14] = new ComboBoxWidget(importDirs[14], filenames[14]);
    arr[15] = new LineEditWidget(importDirs[15], filenames[15]);
    arr[16] = new LineEditWidget(importDirs[16], filenames[16]);
    arr[17] = new LineEditWidget(importDirs[17], filenames[17]);
    arr[18] = new LineEditWidget(importDirs[18], filenames[18]);
    arr[19] = new LineEditWidget(importDirs[19], filenames[19]);
    arr[20] = new LineEditWidget(importDirs[20], filenames[20]);
    arr[21] = new LineEditWidget(importDirs[21], filenames[21]);
    arr[22] = new ComboBoxWidget(importDirs[22], filenames[22]);
    arr[23] = new ComboBoxWidget(importDirs[23], filenames[23]);
    arr[24] = new CheckBoxWidget(importDirs[24], filenames[24]);
    arr[25] = new CheckBoxWidget(importDirs[25], filenames[25]);
    arr[26] = new CheckBoxWidget(importDirs[26], filenames[26]);
    arr[27] = new CheckBoxWidget(importDirs[27], filenames[27]);
    arr[28] = new CheckBoxWidget(importDirs[28], filenames[28]);
    arr[29] = new CheckBoxWidget(importDirs[29], filenames[29]);
    arr[30] = new CheckBoxWidget(importDirs[30], filenames[30]);
    arr[31] = new CheckBoxWidget(importDirs[31], filenames[31]);
    arr[32] = new CheckBoxWidget(importDirs[32], filenames[32]);
    arr[33] = new CheckBoxWidget(importDirs[33], filenames[33]);
    arr[34] = new CheckBoxWidget(importDirs[34], filenames[34]);
    arr[35] = new CheckBoxWidget(importDirs[35], filenames[35]);
    arr[36] = new CheckBoxWidget(importDirs[36], filenames[36]);
    arr[37] = new CheckBoxWidget(importDirs[37], filenames[37]);
    arr[38] = new CheckBoxWidget(importDirs[38], filenames[38]);
    arr[39] = new CheckBoxWidget(importDirs[39], filenames[39]);
    arr[40] = new CheckBoxWidget(importDirs[40], filenames[40]);
    arr[41] = new CheckBoxWidget(importDirs[41], filenames[41]);
    arr[42] = new RadioButtonWidget(importDirs[42], filenames[42]);
    arr[43] = new RadioButtonWidget(importDirs[43], filenames[43]);
    arr[44] = new RadioButtonWidget(importDirs[44], filenames[44]);
    arr[45] = new RadioButtonWidget(importDirs[45], filenames[45]);
    arr[46] = new RadioButtonWidget(importDirs[46], filenames[46]);
    arr[47] = new RadioButtonWidget(importDirs[47], filenames[47]);
    arr[48] = new RadioButtonWidget(importDirs[48], filenames[48]);
    arr[49] = new RadioButtonWidget(importDirs[49], filenames[49]);
    arr[50] = new RadioButtonWidget(importDirs[50], filenames[50]);
    arr[51] = new LineEditWidget(importDirs[51], filenames[51]);
    arr[52] = new LineEditWidget(importDirs[52], filenames[52]);
    arr[53] = new LineEditWidget(importDirs[53], filenames[53]);
    arr[54] = new LineEditWidget(importDirs[54], filenames[54]);
    arr[55] = new LineEditWidget(importDirs[55], filenames[55]);
    arr[56] = new LineEditWidget(importDirs[56], filenames[56]);
    arr[57] = new LineEditWidget(importDirs[57], filenames[57]);
    arr[58] = new LineEditWidget(importDirs[58], filenames[58]);
    arr[59] = new LineEditWidget(importDirs[59], filenames[59]);
    arr[60] = new ComboBoxWidget(importDirs[60], filenames[60]);
    arr[61] = new ComboBoxWidget(importDirs[61], filenames[61]);
    arr[62] = new ComboBoxWidget(importDirs[62], filenames[62]);
    arr[63] = new ComboBoxWidget(importDirs[63], filenames[63]);
    arr[64] = new ComboBoxWidget(importDirs[64], filenames[64]);
    arr[65] = new ComboBoxWidget(importDirs[65], filenames[65]);
    arr[66] = new ComboBoxWidget(importDirs[66], filenames[66]);
    arr[67] = new ComboBoxWidget(importDirs[67], filenames[67]);
    arr[68] = new ComboBoxWidget(importDirs[68], filenames[68]);
    QMap<QString, MyWidget*> widgets{
        {ui->Team1NameEdit ->objectName(), arr[0]},
        {ui->Team1RegionCombo ->objectName(), arr[1]},
        {ui->Team1Logo ->objectName(), arr[2]},
        {ui->Team1Color ->objectName(), arr[3]},
        {ui->Team1Score ->objectName(), arr[4]},
        {ui->Team2NameEdit ->objectName(), arr[5]},
        {ui->Team2RegionCombo ->objectName(), arr[6]},
        {ui->Team2Logo ->objectName(), arr[7]},
        {ui->Team2Color ->objectName(), arr[8]},
        {ui->Team2Score ->objectName(), arr[9]},
        {ui->TitleEdit ->objectName(), arr[10]},
        {ui->SubtitleEdit ->objectName(), arr[11]},
        {ui->TwitterEdit ->objectName(), arr[12]},
        {ui->MessageEdit ->objectName(), arr[13]},
        {ui->LogoCombo ->objectName(), arr[14]},
        {ui->Caster1NameEdit ->objectName(), arr[15]},
        {ui->Caster2NameEdit ->objectName(), arr[16]},
        {ui->Caster3NameEdit ->objectName(), arr[17]},
        {ui->Caster1TwitterEdit ->objectName(), arr[18]},
        {ui->Caster2TwitterEdit ->objectName(), arr[19]},
        {ui->Caster3TwitterEdit ->objectName(), arr[20]},
        {ui->RoundEdit ->objectName(), arr[21]},
        {ui->ColorGroupCombo ->objectName(), arr[22]},
        {ui->ColorsCombo ->objectName(), arr[23]},
        {ui->AlphaWinGame1 ->objectName(), arr[24]},
        {ui->AlphaWinGame2 ->objectName(), arr[25]},
        {ui->AlphaWinGame3 ->objectName(), arr[26]},
        {ui->AlphaWinGame4 ->objectName(), arr[27]},
        {ui->AlphaWinGame5 ->objectName(), arr[28]},
        {ui->AlphaWinGame6 ->objectName(), arr[29]},
        {ui->AlphaWinGame7 ->objectName(), arr[30]},
        {ui->AlphaWinGame8 ->objectName(), arr[31]},
        {ui->AlphaWinGame9 ->objectName(), arr[32]},
        {ui->BetaWinGame1 ->objectName(), arr[33]},
        {ui->BetaWinGame2 ->objectName(), arr[34]},
        {ui->BetaWinGame3 ->objectName(), arr[35]},
        {ui->BetaWinGame4 ->objectName(), arr[36]},
        {ui->BetaWinGame5 ->objectName(), arr[37]},
        {ui->BetaWinGame6 ->objectName(), arr[38]},
        {ui->BetaWinGame7 ->objectName(), arr[39]},
        {ui->BetaWinGame8 ->objectName(), arr[40]},
        {ui->BetaWinGame9 ->objectName(), arr[41]},
        {ui->RadioGame1 ->objectName(), arr[42]},
        {ui->RadioGame2 ->objectName(), arr[43]},
        {ui->RadioGame3 ->objectName(), arr[44]},
        {ui->RadioGame4 ->objectName(), arr[45]},
        {ui->RadioGame5 ->objectName(), arr[46]},
        {ui->RadioGame6 ->objectName(), arr[47]},
        {ui->RadioGame7 ->objectName(), arr[48]},
        {ui->RadioGame8 ->objectName(), arr[49]},
        {ui->RadioGame9 ->objectName(), arr[50]},
        {ui->MapComboGame1 ->objectName(), arr[51]},
        {ui->MapComboGame2 ->objectName(), arr[52]},
        {ui->MapComboGame3 ->objectName(), arr[53]},
        {ui->MapComboGame4 ->objectName(), arr[54]},
        {ui->MapComboGame5 ->objectName(), arr[55]},
        {ui->MapComboGame6 ->objectName(), arr[56]},
        {ui->MapComboGame7 ->objectName(), arr[57]},
        {ui->MapComboGame8 ->objectName(), arr[58]},
        {ui->MapComboGame9 ->objectName(), arr[59]},
        {ui->ModeComboGame1 ->objectName(), arr[60]},
        {ui->ModeComboGame2 ->objectName(), arr[61]},
        {ui->ModeComboGame3 ->objectName(), arr[62]},
        {ui->ModeComboGame4 ->objectName(), arr[63]},
        {ui->ModeComboGame5 ->objectName(), arr[64]},
        {ui->ModeComboGame6 ->objectName(), arr[65]},
        {ui->ModeComboGame7 ->objectName(), arr[66]},
        {ui->ModeComboGame8 ->objectName(), arr[67]},
        {ui->ModeComboGame9 ->objectName(), arr[68]}
    };
    return widgets;
}

void MainWindow::on_ColorsCombo_activated(int index)
{
    QString local;

    if (ui->ColorGroupCombo->currentText().compare("Ranked") == 0) {
        local = this->directoryFor["/RankedColorsDirectory.txt"];
    }
    if (ui->ColorGroupCombo->currentText().compare("Splatfest") == 0) {
        local = this->directoryFor["/SplatfestColorsDirectory.txt"];
    }
    if (ui->ColorGroupCombo->currentText().compare("Turf War") == 0) {
        local = this->directoryFor["/TurfWarColorsDirectory.txt"];
    }
    qDebug() << local;
    ComboBoxWidget *colorsCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ColorsCombo->objectName()])};
    colorsCombo->saveColors(
        this->directoryFor["/ImportingFilesDirectory.txt"],
        ui->ColorsCombo,
        local,
        ui->Team1Color,
        ui->Team2Color,
        ui->BothTeamsColor
    );
}

