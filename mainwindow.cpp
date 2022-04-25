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
    this->playerWidgetFor = this->playersSection_init();
    this->labelFor = this->weapons_init();
    this->infoSection_init();
    this->teamsSection_init();
    this->roundSection_init();
    this->castersSection_init();
    this->setSection_init();
    this->weaponsSection_init();
    //qDebug() << ui->tabBox->sizeHint();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSet_Directories_triggered()
{
    FilesDialog *dirConfigWindow = new FilesDialog(this, &this->directoryFor, this->directories);
    QObject::connect(dirConfigWindow, &FilesDialog::allDone, this, &MainWindow::on_FilesDialogAccepted);
    dirConfigWindow->show();
}

void MainWindow::on_actionUpdate_Options_triggered()
{
    this->clearFields();
    this->infoSection_init();
    this->teamsSection_init();
    this->roundSection_init();
    this->castersSection_init();
    this->setSection_init();
    this->weaponsSection_init();
    this->playerWidgetFor = this->playersSection_init();
}

void MainWindow::on_FilesDialogAccepted()
{
    this->configDirectories_init();
    this->clearFields();
    this->infoSection_init();
    this->teamsSection_init();
    this->roundSection_init();
    this->castersSection_init();
    this->setSection_init();
    this->weaponsSection_init();
    this->playerWidgetFor = this->playersSection_init();
}

void MainWindow::clearFields()
{
    // Clear Combo options
    ui->ColorGroupCombo->clear();
    ui->ColorsCombo->clear();
    ui->LogoCombo->clear();
    ui->ModeComboGame1->clear();
    ui->ModeComboGame2->clear();
    ui->ModeComboGame3->clear();
    ui->ModeComboGame4->clear();
    ui->ModeComboGame5->clear();
    ui->ModeComboGame6->clear();
    ui->ModeComboGame7->clear();
    ui->ModeComboGame8->clear();
    ui->ModeComboGame9->clear();
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

void MainWindow::updateFileTimestamp(QString filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite)) {
        return;
    }
    const quint64 size = file.size();
    file.seek(size);
    file.write( QByteArray(1, '0') );
    file.resize(size);
    file.close();
    return;
}

void MainWindow::copyFile(QString source, QString destiny)
{
    QFile::remove(destiny);
    QFile::copy(source, destiny);
    updateFileTimestamp(destiny);
    return;
}

QStringList MainWindow::clearOptionalFiles(QStringList files, QString directory)
{
    QFile file;
    for (int i = 0; i < files.size(); ++i) {
        file.setFileName(directory + files.at(i));
        file.open(QFile::WriteOnly|QFile::Truncate);
        file.close();
    }
    files.clear();
    return files;

}

QStringList MainWindow::saveOptionalFiles(QString originDir, QString destinyDir)
{
    QDir teamDir(originDir);
    QStringList optionalFiles;
    if (teamDir.exists()) {
        QStringList filters;
        QString originFile;
        QString destinyFile;
        // All text files
        filters << "*.txt";
        teamDir.setNameFilters(filters);
        QStringList fileList = teamDir.entryList(filters);
        for (int i = 0; i < fileList.size(); ++i) {
            optionalFiles << fileList.at(i);
            originFile = originDir + fileList.at(i);
            destinyFile = destinyDir + fileList.at(i);
            this->copyFile(originFile, destinyFile);
        }
        // All image files
        filters.clear();
        filters << "*.jpg" << "*.jpeg" << "*.png" << "*.gif" << "*.jfif";
        teamDir.setNameFilters(filters);
        fileList = teamDir.entryList(filters);
        int lastPoint;
        for (int i = 0; i < fileList.size(); ++i) {
            lastPoint = fileList.at(i).lastIndexOf(".");
            originFile = originDir + fileList.at(i);
            if (lastPoint == -1) { // if there's no extension, keep it the same
                destinyFile = destinyDir + fileList.at(i);
                optionalFiles << fileList.at(i);
            } else { // if there's an extension, remove it
                destinyFile = destinyDir + fileList.at(i).left(lastPoint);
                optionalFiles << fileList.at(i).left(lastPoint);
            this->copyFile(originFile, destinyFile);
            }
        }
    }
    return optionalFiles;
}

// Init functions

void MainWindow::configDirectories_init()
{
    // Initialize config filenames
    QString dirs[dirAmount] = {
        "/AlphaTeamDirectory.txt", "/BravoTeamDirectory.txt", "/LogosDirectory.txt",
        "/MapsDirectory.txt", "/ModeIconsDirectory.txt", "/SplatfestColorsDirectory.txt", "/TurfWarColorsDirectory.txt",
        "/RankedColorsDirectory.txt", "/ImportingFilesDirectory.txt", "/DefaultDirectory.txt", "/WinPointsDirectory.txt",
        "/WeaponsDirectory.txt"
    };
    for (int i = 0; i < dirAmount; i++) {
        this->directories[i] = dirs[i];
    }
    QString directory = QCoreApplication::applicationDirPath() + "/config";
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
            this->directoryFor[dirs[i]] = in.readAll();
            file.close();
        }
    }
}

QStringList MainWindow::getListOf(QString dirString, bool includeDirs)
{
    QStringList completionList;
    QStringList noExtensionList;
    int lastPoint;
    QDir directory(dirString);
    completionList = (includeDirs ? directory.entryList(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot) : directory.entryList(QDir::Files));
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

void MainWindow::colorAutoUpdate()
{
    if (ui->UpdateColorCheckbox->isChecked()) {
        int nextIndex = ui->ColorsCombo->currentIndex() + 1;
        ui->ColorsCombo->setCurrentIndex(nextIndex % ui->ColorsCombo->count());
        this->on_ColorsCombo_activated(nextIndex);
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

void MainWindow::teamsSection_init()
{
    QString importDir = this->directoryFor["/ImportingFilesDirectory.txt"];
    QString defaultDir = this->directoryFor["/DefaultDirectory.txt"];
    QString bravoDir = this->directoryFor["/BravoTeamDirectory.txt"];
    QString alphaDir = this->directoryFor["/AlphaTeamDirectory.txt"];
    LineEditWidget *alphaName {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Team1NameEdit->objectName()])};
    LabelWidget *alphaLogo {dynamic_cast<LabelWidget *>(this->widgetFor[ui->Team1Logo->objectName()])};
    SpinBoxWidget *alphaScore {dynamic_cast<SpinBoxWidget *>(this->widgetFor[ui->Team1Score->objectName()])};
    // Set Team Alpha Autocomplete
    QStringList team1CompletionList = this->getListOf(alphaDir, true);

    this->Team1Completer = new QCompleter(team1CompletionList, this);
    this->Team1Completer->setCaseSensitivity(Qt::CaseInsensitive);
    this->Team1Completer->setFilterMode(Qt::MatchContains);
    ui->Team1NameEdit->setCompleter(this->Team1Completer);
    alphaName->init(importDir, ui->Team1NameEdit);
    alphaLogo->init(importDir, ui->Team1Logo, alphaDir, "/" + ui->Team1NameEdit->text(), defaultDir);
    alphaScore->init(importDir, ui->Team1Score);
    this->on_Team1NameEdit_editingFinished();

    // Team Bravo

    LineEditWidget *bravoName {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Team2NameEdit->objectName()])};
    LabelWidget *bravoLogo {dynamic_cast<LabelWidget *>(this->widgetFor[ui->Team2Logo->objectName()])};
    SpinBoxWidget *bravoScore {dynamic_cast<SpinBoxWidget *>(this->widgetFor[ui->Team2Score->objectName()])};
    // Set Team Bravo Autocomplete
    QStringList team2CompletionList = this->getListOf(bravoDir, true);

    this->Team2Completer = new QCompleter(team2CompletionList, this);
    this->Team2Completer->setCaseSensitivity(Qt::CaseInsensitive);
    this->Team2Completer->setFilterMode(Qt::MatchContains);
    ui->Team2NameEdit->setCompleter(this->Team2Completer);
    bravoName->init(importDir, ui->Team2NameEdit);
    bravoLogo->init(importDir, ui->Team2Logo, bravoDir, "/" + ui->Team2NameEdit->text(), defaultDir);
    bravoScore->init(importDir, ui->Team2Score);
    this->on_Team2NameEdit_editingFinished();
}

void MainWindow::colors_init()
{
    QString comboString;
    QString indivString;
    if (ui->ColorGroupCombo->currentText() == "Ranked") {
        // Check if combo or individual
        comboString = this->directoryFor["/RankedColorsDirectory.txt"] + "/combo";
        indivString = this->directoryFor["/RankedColorsDirectory.txt"] + "/alpha";
    } else if (ui->ColorGroupCombo->currentText() == "Turf War") {
        // Check if combo or individual
        comboString = this->directoryFor["/TurfWarColorsDirectory.txt"] + "/combo";
        indivString = this->directoryFor["/TurfWarColorsDirectory.txt"] + "/alpha";
    } else { // Splatfest
        // Check if combo or individual
        comboString = this->directoryFor["/SplatfestColorsDirectory.txt"] + "/combo";
        indivString = this->directoryFor["/SplatfestColorsDirectory.txt"] + "/alpha";
    }
    QDir comboDir(comboString);
    QDir indivDir(indivString);
    QString colorsString;
    if (comboDir.exists() && !comboDir.isEmpty()) {
        colorsString = comboString;
    } else if (indivDir.exists() && !indivDir.isEmpty()) {
        colorsString = indivString;
    }
    ComboBoxWidget *colorsCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ColorsCombo->objectName()])};
    colorsCombo->init(this->directoryFor["/ImportingFilesDirectory.txt"], ui->ColorsCombo, colorsString);
    this->mirrored = false;
    this->on_ColorsCombo_activated(ui->ColorsCombo->currentIndex());
}

void MainWindow::roundSection_init()
{ 
    QString importDir = this->directoryFor["/ImportingFilesDirectory.txt"];
    // Color Group Combo
    const QStringList colorGroupsList = {"Ranked", "Turf War", "Splatfest"};
    ComboBoxWidget *colorGroup {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ColorGroupCombo->objectName()])};
    colorGroup->init(importDir, ui->ColorGroupCombo, colorGroupsList);
    // Colors Combo
    this->colors_init();
    // Round
    LineEditWidget *round {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->RoundEdit->objectName()])};
    round->init(importDir, ui->RoundEdit);
}

void MainWindow::castersSection_init()
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

void MainWindow::setSection_init()
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
    // Bravo Game Winner Checkboxes
    CheckBoxWidget *bravoWin1 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BravoWinGame1->objectName()])};
    bravoWin1->init(importDir, ui->BravoWinGame1, ui->Team2NameEdit->text());
    CheckBoxWidget *bravoWin2 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BravoWinGame2->objectName()])};
    bravoWin2->init(importDir, ui->BravoWinGame2, ui->Team2NameEdit->text());
    CheckBoxWidget *bravoWin3 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BravoWinGame3->objectName()])};
    bravoWin3->init(importDir, ui->BravoWinGame3, ui->Team2NameEdit->text());
    CheckBoxWidget *bravoWin4 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BravoWinGame4->objectName()])};
    bravoWin4->init(importDir, ui->BravoWinGame4, ui->Team2NameEdit->text());
    CheckBoxWidget *bravoWin5 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BravoWinGame5->objectName()])};
    bravoWin5->init(importDir, ui->BravoWinGame5, ui->Team2NameEdit->text());
    CheckBoxWidget *bravoWin6 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BravoWinGame6->objectName()])};
    bravoWin6->init(importDir, ui->BravoWinGame6, ui->Team2NameEdit->text());
    CheckBoxWidget *bravoWin7 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BravoWinGame7->objectName()])};
    bravoWin7->init(importDir, ui->BravoWinGame7, ui->Team2NameEdit->text());
    CheckBoxWidget *bravoWin8 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BravoWinGame8->objectName()])};
    bravoWin8->init(importDir, ui->BravoWinGame8, ui->Team2NameEdit->text());
    CheckBoxWidget *bravoWin9 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BravoWinGame9->objectName()])};
    bravoWin9->init(importDir, ui->BravoWinGame9, ui->Team2NameEdit->text());
}

void MainWindow::weaponsSection_init()
{
    QString importDir = this->directoryFor["/ImportingFilesDirectory.txt"];
    QString weaponsDir = this->directoryFor["/WeaponsDirectory.txt"];
    // Set Team Alpha Autocomplete
    QStringList weaponsCompletionList = this->getListOf(weaponsDir, true);
    this->WeaponsCompleter = new QCompleter(weaponsCompletionList, this);
    this->WeaponsCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    this->WeaponsCompleter->setFilterMode(Qt::MatchContains);
    // Init Field
    QList<QString> keys = widgetFor.keys();
    MyWidget *wid;
    QLineEdit *qwid;
    LineEditWidget *weapon;
    for (int i = 67; i < 139; ++i) {
        wid = widgetFor.value(keys.at(i));
        qwid = ui->AlphaWeapons->findChild<QLineEdit *>(keys.at(i));
        if (qwid == nullptr) qwid = ui->BravoWeapons->findChild<QLineEdit *>(keys.at(i));
        qwid->setCompleter(this->WeaponsCompleter);
        weapon = {dynamic_cast<LineEditWidget *>(wid)};
        weapon->init(importDir, qwid, ".txt");
        qwid->editingFinished();
    }
    QRegularExpression re("(Player|Weapon)(Alpha|Bravo)LabelR[1-9]");
    QList<ClickableLabel *> labels = ui->TeamsTabs->findChildren<ClickableLabel *>(re, Qt::FindChildrenRecursively);
    for (int i = 0; i < labels.count(); ++i) {
        connect(labels.at(i), &ClickableLabel::dblClicked, this, &MainWindow::labelClicked);
    }

}

QMap <QString, MyWidget*> MainWindow::playersSection_init()
{
    connect(ui->PlayersResetAlpha, &QPushButton::clicked, this, &MainWindow::resetFields);
    connect(ui->PlayersResetBravo, &QPushButton::clicked, this, &MainWindow::resetFields);

    QString importDir = this->directoryFor["/ImportingFilesDirectory.txt"];
    QString alphaDir = this->directoryFor["/AlphaTeamDirectory.txt"];
    QString bravoDir = this->directoryFor["/BravoTeamDirectory.txt"];
    QComboBox *playerWidgets[72] = {
        ui->PlayersR1_1, ui->PlayersR1_2, ui->PlayersR1_3, ui->PlayersR1_4,
        ui->PlayersR2_1, ui->PlayersR2_2, ui->PlayersR2_3, ui->PlayersR2_4,
        ui->PlayersR3_1, ui->PlayersR3_2, ui->PlayersR3_3, ui->PlayersR3_4,
        ui->PlayersR4_1, ui->PlayersR4_2, ui->PlayersR4_3, ui->PlayersR4_4,
        ui->PlayersR5_1, ui->PlayersR5_2, ui->PlayersR5_3, ui->PlayersR5_4,
        ui->PlayersR6_1, ui->PlayersR6_2, ui->PlayersR6_3, ui->PlayersR6_4,
        ui->PlayersR7_1, ui->PlayersR7_2, ui->PlayersR7_3, ui->PlayersR7_4,
        ui->PlayersR8_1, ui->PlayersR8_2, ui->PlayersR8_3, ui->PlayersR8_4,
        ui->PlayersR9_1, ui->PlayersR9_2, ui->PlayersR9_3, ui->PlayersR9_4,
        ui->PlayersR1_5, ui->PlayersR1_6, ui->PlayersR1_7, ui->PlayersR1_8,
        ui->PlayersR2_5, ui->PlayersR2_6, ui->PlayersR2_7, ui->PlayersR2_8,
        ui->PlayersR3_5, ui->PlayersR3_6, ui->PlayersR3_7, ui->PlayersR3_8,
        ui->PlayersR4_5, ui->PlayersR4_6, ui->PlayersR4_7, ui->PlayersR4_8,
        ui->PlayersR5_5, ui->PlayersR5_6, ui->PlayersR5_7, ui->PlayersR5_8,
        ui->PlayersR6_5, ui->PlayersR6_6, ui->PlayersR6_7, ui->PlayersR6_8,
        ui->PlayersR7_5, ui->PlayersR7_6, ui->PlayersR7_7, ui->PlayersR7_8,
        ui->PlayersR8_5, ui->PlayersR8_6, ui->PlayersR8_7, ui->PlayersR8_8,
        ui->PlayersR9_5, ui->PlayersR9_6, ui->PlayersR9_7, ui->PlayersR9_8,
    };
    int max = sizeof(playerWidgets) / sizeof(QComboBox *);
    QMap<QString, MyWidget*> players;
    for (int i = 0; i < max; ++i) {
        players[playerWidgets[i]->objectName()] = new ComboBoxWidget("/set/players/", playerWidgets[i]->objectName().right(4));
        ComboBoxWidget *player {dynamic_cast<ComboBoxWidget *>(players[playerWidgets[i]->objectName()])};
        player->initPlayers(importDir, playerWidgets[i]);
        connect(playerWidgets[i], &QComboBox::activated, this, &MainWindow::updatePlayer);
    }
    return players;
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

    // Clean early optional files
    this->teamAlphaOptionalFiles = this->clearOptionalFiles(this->teamAlphaOptionalFiles, importDir + "/teamAlpha/");

    name->saveInFile(importDir, ui->Team1NameEdit);
    // Save file with both team names
    QString teamNames = ui->Team1NameEdit->text() + " - " + ui->Team2NameEdit->text();
    name->saveInFile(importDir, ui->Team1NameEdit, "/round/", "teamNames.txt", teamNames);
    logo->init(importDir, ui->Team1Logo, alphaDir, "/" + ui->Team1NameEdit->text(), defaultDir);

    // Add new optional files
    this->teamAlphaOptionalFiles = this->saveOptionalFiles(alphaDir + "/" + ui->Team1NameEdit->text() + "/", importDir + "/teamAlpha/");

    this->updateAlphaWins();
    this->updatePlayersFromAlpha();
}

void MainWindow::updateAlphaWins()
{
    ui->Team1Score->setValue(0);
    if (ui->AlphaWinGame1->isChecked()) {
        this->on_AlphaWinGame1_clicked();
    }
    if (ui->AlphaWinGame2->isChecked()) {
        this->on_AlphaWinGame2_clicked();
    }
    if (ui->AlphaWinGame3->isChecked()) {
        this->on_AlphaWinGame3_clicked();
    }
    if (ui->AlphaWinGame4->isChecked()) {
        this->on_AlphaWinGame4_clicked();
    }
    if (ui->AlphaWinGame5->isChecked()) {
        this->on_AlphaWinGame5_clicked();
    }
    if (ui->AlphaWinGame6->isChecked()) {
        this->on_AlphaWinGame6_clicked();
    }
    if (ui->AlphaWinGame7->isChecked()) {
        this->on_AlphaWinGame7_clicked();
    }
    if (ui->AlphaWinGame8->isChecked()) {
        this->on_AlphaWinGame8_clicked();
    }
    if (ui->AlphaWinGame9->isChecked()) {
        this->on_AlphaWinGame9_clicked();
    }
}

/*
/ END Team Alpha Section
*/

/*
/ Team Bravo Section
*/

void MainWindow::on_Team2NameEdit_editingFinished()
{
    QString importDir = this->directoryFor["/ImportingFilesDirectory.txt"];
    QString defaultDir = this->directoryFor["/DefaultDirectory.txt"];
    QString bravoDir = this->directoryFor["/BravoTeamDirectory.txt"];
    LineEditWidget *name {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Team2NameEdit->objectName()])};
    LabelWidget *logo {dynamic_cast<LabelWidget *>(this->widgetFor[ui->Team2Logo->objectName()])};

    // Clean early optional files
    this->teamBravoOptionalFiles = this->clearOptionalFiles(this->teamBravoOptionalFiles, importDir + "/teamBravo/");

    name->saveInFile(importDir, ui->Team2NameEdit);
    // Save file with both team names
    QString teamNames = ui->Team1NameEdit->text() + " - " + ui->Team2NameEdit->text();
    name->saveInFile(importDir, ui->Team1NameEdit, "/round/", "teamNames.txt", teamNames);
    logo->init(importDir, ui->Team2Logo, bravoDir, "/" + ui->Team2NameEdit->text(), defaultDir);

    // Add new optional files
    this->teamBravoOptionalFiles = this->saveOptionalFiles(bravoDir + "/" + ui->Team2NameEdit->text() + "/", importDir + "/teamBravo/");

    this->updateBravoWins();
    this->updatePlayersFromBravo();
}

void MainWindow::updateBravoWins()
{
    ui->Team2Score->setValue(0);
    if (ui->BravoWinGame1->isChecked()) {
        this->on_BravoWinGame1_clicked();
    }
    if (ui->BravoWinGame2->isChecked()) {
        this->on_BravoWinGame2_clicked();
    }
    if (ui->BravoWinGame3->isChecked()) {
        this->on_BravoWinGame3_clicked();
    }
    if (ui->BravoWinGame4->isChecked()) {
        this->on_BravoWinGame4_clicked();
    }
    if (ui->BravoWinGame5->isChecked()) {
        this->on_BravoWinGame5_clicked();
    }
    if (ui->BravoWinGame6->isChecked()) {
        this->on_BravoWinGame6_clicked();
    }
    if (ui->BravoWinGame7->isChecked()) {
        this->on_BravoWinGame7_clicked();
    }
    if (ui->BravoWinGame8->isChecked()) {
        this->on_BravoWinGame8_clicked();
    }
    if (ui->BravoWinGame9->isChecked()) {
        this->on_BravoWinGame9_clicked();
    }
}

/*
/ END Team Bravo Section
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
    // reinit colors combo
    ui->ColorsCombo->clear();
    this->colors_init();
    ui->ColorsCombo->setCurrentIndex(0);
    this->on_ColorsCombo_activated(0);
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
    QString winPointsDir = this->directoryFor["/WinPointsDirectory.txt"];
    CheckBoxWidget *alphaWin1 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame1->objectName()])};
    alphaWin1->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame1, "/teamAlpha/", winPointsDir);
    ui->AlphaWinGame2->setChecked(false);
    CheckBoxWidget *alphaWin2 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame2->objectName()])};
    alphaWin2->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame2, "/teamAlpha/", winPointsDir);
    ui->AlphaWinGame3->setChecked(false);
    CheckBoxWidget *alphaWin3 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame3->objectName()])};
    alphaWin3->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame3, "/teamAlpha/", winPointsDir);
    ui->AlphaWinGame4->setChecked(false);
    CheckBoxWidget *alphaWin4 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame4->objectName()])};
    alphaWin4->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame4, "/teamAlpha/", winPointsDir);
    ui->AlphaWinGame5->setChecked(false);
    CheckBoxWidget *alphaWin5 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame5->objectName()])};
    alphaWin5->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame5, "/teamAlpha/", winPointsDir);
    ui->AlphaWinGame6->setChecked(false);
    CheckBoxWidget *alphaWin6 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame6->objectName()])};
    alphaWin6->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame6, "/teamAlpha/", winPointsDir);
    ui->AlphaWinGame7->setChecked(false);
    CheckBoxWidget *alphaWin7 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame7->objectName()])};
    alphaWin7->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame7, "/teamAlpha/", winPointsDir);
    ui->AlphaWinGame8->setChecked(false);
    CheckBoxWidget *alphaWin8 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame8->objectName()])};
    alphaWin8->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame8, "/teamAlpha/", winPointsDir);
    ui->AlphaWinGame9->setChecked(false);
    CheckBoxWidget *alphaWin9 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->AlphaWinGame9->objectName()])};
    alphaWin9->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->AlphaWinGame9, "/teamAlpha/", winPointsDir);
    ui->BravoWinGame1->setChecked(false);
    CheckBoxWidget *bravoWin1 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BravoWinGame1->objectName()])};
    bravoWin1->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BravoWinGame1, "/teamBravo/", winPointsDir);
    ui->BravoWinGame2->setChecked(false);
    CheckBoxWidget *bravoWin2 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BravoWinGame2->objectName()])};
    bravoWin2->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BravoWinGame2, "/teamBravo/", winPointsDir);
    ui->BravoWinGame3->setChecked(false);
    CheckBoxWidget *bravoWin3 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BravoWinGame3->objectName()])};
    bravoWin3->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BravoWinGame3, "/teamBravo/", winPointsDir);
    ui->BravoWinGame4->setChecked(false);
    CheckBoxWidget *bravoWin4 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BravoWinGame4->objectName()])};
    bravoWin4->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BravoWinGame4, "/teamBravo/", winPointsDir);
    ui->BravoWinGame5->setChecked(false);
    CheckBoxWidget *bravoWin5 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BravoWinGame5->objectName()])};
    bravoWin5->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BravoWinGame5, "/teamBravo/", winPointsDir);
    ui->BravoWinGame6->setChecked(false);
    CheckBoxWidget *bravoWin6 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BravoWinGame6->objectName()])};
    bravoWin6->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BravoWinGame6, "/teamBravo/", winPointsDir);
    ui->BravoWinGame7->setChecked(false);
    CheckBoxWidget *bravoWin7 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BravoWinGame7->objectName()])};
    bravoWin7->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BravoWinGame7, "/teamBravo/", winPointsDir);
    ui->BravoWinGame8->setChecked(false);
    CheckBoxWidget *bravoWin8 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BravoWinGame8->objectName()])};
    bravoWin8->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BravoWinGame8, "/teamBravo/", winPointsDir);
    ui->BravoWinGame9->setChecked(false);
    CheckBoxWidget *bravoWin9 {dynamic_cast<CheckBoxWidget *>(this->widgetFor[ui->BravoWinGame9->objectName()])};
    bravoWin9->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], ui->BravoWinGame9, "/teamBravo/", winPointsDir);
    ui->RadioGame1->setChecked(true);
    this->on_RadioGame1_clicked();
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
        if (ui->BravoWinGame1->isChecked()) count++;
        if (ui->BravoWinGame2->isChecked()) count++;
        if (ui->BravoWinGame3->isChecked()) count++;
        if (ui->BravoWinGame4->isChecked()) count++;
        if (ui->BravoWinGame5->isChecked()) count++;
        if (ui->BravoWinGame6->isChecked()) count++;
        if (ui->BravoWinGame7->isChecked()) count++;
        if (ui->BravoWinGame8->isChecked()) count++;
        if (ui->BravoWinGame9->isChecked()) count++;
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
        QCheckBox *bravoGames[9] = {
            ui->BravoWinGame1, ui->BravoWinGame2, ui->BravoWinGame3, ui->BravoWinGame4,
            ui->BravoWinGame5, ui->BravoWinGame6, ui->BravoWinGame7, ui->BravoWinGame8, ui->BravoWinGame9
        };
        QRadioButton *rounds[9] = {
            ui->RadioGame1, ui->RadioGame2, ui->RadioGame3, ui->RadioGame4,
            ui->RadioGame5, ui->RadioGame6, ui->RadioGame7, ui->RadioGame8, ui->RadioGame9
        };
        for (int i = 0; i < 9; i++) {
            if (!alphaGames[i]->isChecked() && !bravoGames[i]->isChecked()) {
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
        this->saveRound(1);
    }
}

void MainWindow::on_ModeComboGame2_activated(int index)
{
    QString local = this->directoryFor["/ModeIconsDirectory.txt"];
    ComboBoxWidget *modeCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame2->objectName()])};
    modeCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->ModeComboGame2, "image", local);
    if (ui->RadioGame2->isChecked()) {
        this->saveRound(2);
    }
}

void MainWindow::on_ModeComboGame3_activated(int index)
{
    QString local = this->directoryFor["/ModeIconsDirectory.txt"];
    ComboBoxWidget *modeCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame3->objectName()])};
    modeCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->ModeComboGame3, "image", local);
    if (ui->RadioGame3->isChecked()) {
        this->saveRound(3);
    }
}

void MainWindow::on_ModeComboGame4_activated(int index)
{
    QString local = this->directoryFor["/ModeIconsDirectory.txt"];
    ComboBoxWidget *modeCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame4->objectName()])};
    modeCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->ModeComboGame4, "image", local);
    if (ui->RadioGame4->isChecked()) {
        this->saveRound(4);
    }
}

void MainWindow::on_ModeComboGame5_activated(int index)
{
    QString local = this->directoryFor["/ModeIconsDirectory.txt"];
    ComboBoxWidget *modeCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame5->objectName()])};
    modeCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->ModeComboGame5, "image", local);
    if (ui->RadioGame5->isChecked()) {
        this->saveRound(5);
    }
}

void MainWindow::on_ModeComboGame6_activated(int index)
{
    QString local = this->directoryFor["/ModeIconsDirectory.txt"];
    ComboBoxWidget *modeCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame6->objectName()])};
    modeCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->ModeComboGame6, "image", local);
    if (ui->RadioGame6->isChecked()) {
        this->saveRound(6);
    }
}

void MainWindow::on_ModeComboGame7_activated(int index)
{
    QString local = this->directoryFor["/ModeIconsDirectory.txt"];
    ComboBoxWidget *modeCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame7->objectName()])};
    modeCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->ModeComboGame7, "image", local);
    if (ui->RadioGame7->isChecked()) {
        this->saveRound(7);
    }
}

void MainWindow::on_ModeComboGame8_activated(int index)
{
    QString local = this->directoryFor["/ModeIconsDirectory.txt"];
    ComboBoxWidget *modeCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame8->objectName()])};
    modeCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->ModeComboGame8, "image", local);
    if (ui->RadioGame8->isChecked()) {
        this->saveRound(8);
    }
}

void MainWindow::on_ModeComboGame9_activated(int index)
{
    QString local = this->directoryFor["/ModeIconsDirectory.txt"];
    ComboBoxWidget *modeCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ModeComboGame9->objectName()])};
    modeCombo->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->ModeComboGame9, "image", local);
    if (ui->RadioGame9->isChecked()) {
        this->saveRound(9);
    }
}

void MainWindow::on_MapComboGame1_editingFinished()
{
    QString local = this->directoryFor["/MapsDirectory.txt"];
    LineEditWidget *mapCombo1 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame1->objectName()])};
    mapCombo1->saveWithImage(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MapComboGame1, local);
    if (ui->RadioGame1->isChecked()) {
        this->saveRound(1);
    }
}

void MainWindow::on_MapComboGame2_editingFinished()
{
    QString local = this->directoryFor["/MapsDirectory.txt"];
    LineEditWidget *mapCombo2 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame2->objectName()])};
    mapCombo2->saveWithImage(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MapComboGame2, local);
    if (ui->RadioGame2->isChecked()) {
        this->saveRound(2);
    }
}

void MainWindow::on_MapComboGame3_editingFinished()
{
    QString local = this->directoryFor["/MapsDirectory.txt"];
    LineEditWidget *mapCombo3 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame3->objectName()])};
    mapCombo3->saveWithImage(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MapComboGame3, local);
    if (ui->RadioGame3->isChecked()) {
        this->saveRound(3);
    }
}

void MainWindow::on_MapComboGame4_editingFinished()
{
    QString local = this->directoryFor["/MapsDirectory.txt"];
    LineEditWidget *mapCombo4 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame4->objectName()])};
    mapCombo4->saveWithImage(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MapComboGame4, local);
    if (ui->RadioGame4->isChecked()) {
        this->saveRound(4);
    }
}

void MainWindow::on_MapComboGame5_editingFinished()
{
    QString local = this->directoryFor["/MapsDirectory.txt"];
    LineEditWidget *mapCombo5 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame5->objectName()])};
    mapCombo5->saveWithImage(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MapComboGame5, local);
    if (ui->RadioGame5->isChecked()) {
        this->saveRound(5);
    }
}

void MainWindow::on_MapComboGame6_editingFinished()
{
    QString local = this->directoryFor["/MapsDirectory.txt"];
    LineEditWidget *mapCombo6 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame6->objectName()])};
    mapCombo6->saveWithImage(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MapComboGame6, local);
    if (ui->RadioGame6->isChecked()) {
        this->saveRound(6);
    }
}

void MainWindow::on_MapComboGame7_editingFinished()
{
    QString local = this->directoryFor["/MapsDirectory.txt"];
    LineEditWidget *mapCombo7 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame7->objectName()])};
    mapCombo7->saveWithImage(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MapComboGame7, local);
    if (ui->RadioGame7->isChecked()) {
        this->saveRound(7);
    }
}

void MainWindow::on_MapComboGame8_editingFinished()
{
    QString local = this->directoryFor["/MapsDirectory.txt"];
    LineEditWidget *mapCombo8 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame8->objectName()])};
    mapCombo8->saveWithImage(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MapComboGame8, local);
    if (ui->RadioGame8->isChecked()) {
        this->saveRound(8);
    }
}

void MainWindow::on_MapComboGame9_editingFinished()
{
    QString local = this->directoryFor["/MapsDirectory.txt"];
    LineEditWidget *mapCombo9 {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->MapComboGame9->objectName()])};
    mapCombo9->saveWithImage(this->directoryFor["/ImportingFilesDirectory.txt"], ui->MapComboGame9, local);
    if (ui->RadioGame9->isChecked()) {
        this->saveRound(9);
    }
}

void MainWindow::saveRound(int index)
{
    RadioButtonWidget *game1 {dynamic_cast<RadioButtonWidget *>(this->widgetFor["RadioGame" + QString::number(index)])};
    game1->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], QString::number(index));
}

void MainWindow::on_RadioGame1_clicked()
{
    RadioButtonWidget *game1 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame1->objectName()])};
    game1->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], "1");
    this->colorAutoUpdate();
}

void MainWindow::on_RadioGame2_clicked()
{
    RadioButtonWidget *game2 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame2->objectName()])};
    game2->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], "2");
    this->colorAutoUpdate();
}

void MainWindow::on_RadioGame3_clicked()
{
    RadioButtonWidget *game3 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame3->objectName()])};
    game3->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], "3");
    this->colorAutoUpdate();
}

void MainWindow::on_RadioGame4_clicked()
{
    RadioButtonWidget *game4 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame4->objectName()])};
    game4->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], "4");
    this->colorAutoUpdate();
}

void MainWindow::on_RadioGame5_clicked()
{
    RadioButtonWidget *game5 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame5->objectName()])};
    game5->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], "5");
    this->colorAutoUpdate();
}

void MainWindow::on_RadioGame6_clicked()
{
    RadioButtonWidget *game6 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame6->objectName()])};
    game6->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], "6");
    this->colorAutoUpdate();
}

void MainWindow::on_RadioGame7_clicked()
{
    RadioButtonWidget *game7 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame7->objectName()])};
    game7->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], "7");
    this->colorAutoUpdate();
}

void MainWindow::on_RadioGame8_clicked()
{
    RadioButtonWidget *game8 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame8->objectName()])};
    game8->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], "8");
    this->colorAutoUpdate();
}

void MainWindow::on_RadioGame9_clicked()
{
    RadioButtonWidget *game9 {dynamic_cast<RadioButtonWidget *>(this->widgetFor[ui->RadioGame9->objectName()])};
    game9->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], "9");
    this->colorAutoUpdate();
}

void MainWindow::handleWinner(QCheckBox *WinGame, QCheckBox *OpponentWinGame, QSpinBox *score, QSpinBox *opponentScore,
                              QString teamDir, QRadioButton *currentGame, QRadioButton *nextGame, clickedFunction nextGameClicked) {
    QString winPointsDir = this->directoryFor["/WinPointsDirectory.txt"];
    CheckBoxWidget *win {dynamic_cast<CheckBoxWidget *>(this->widgetFor[WinGame->objectName()])};
    win->handleCheck(this->directoryFor["/ImportingFilesDirectory.txt"], WinGame, teamDir, winPointsDir);
    if (OpponentWinGame->isChecked()) {
        OpponentWinGame->setChecked(false);
        this->scoreAutoUpdate(opponentScore, OpponentWinGame);
    }
    if (currentGame->isChecked() && ui->UpdateRoundCheckbox->isChecked()) {
        nextGame->setChecked(true);
        (this->*nextGameClicked)();
    }
    this->scoreAutoUpdate(score, WinGame);
}

void MainWindow::on_AlphaWinGame1_clicked()
{
    MainWindow::clickedFunction function = &MainWindow::on_RadioGame2_clicked;
    handleWinner(ui->AlphaWinGame1, ui->BravoWinGame1, ui->Team1Score, ui->Team2Score, "/teamAlpha/", ui->RadioGame1, ui->RadioGame2, function);
}

void MainWindow::on_AlphaWinGame2_clicked()
{
    MainWindow::clickedFunction function = &MainWindow::on_RadioGame3_clicked;
    handleWinner(ui->AlphaWinGame2, ui->BravoWinGame2, ui->Team1Score, ui->Team2Score, "/teamAlpha/", ui->RadioGame2, ui->RadioGame3, function);
}

void MainWindow::on_AlphaWinGame3_clicked()
{
    MainWindow::clickedFunction function = &MainWindow::on_RadioGame4_clicked;
    handleWinner(ui->AlphaWinGame3, ui->BravoWinGame3, ui->Team1Score, ui->Team2Score, "/teamAlpha/", ui->RadioGame3, ui->RadioGame4, function);
}

void MainWindow::on_AlphaWinGame4_clicked()
{
    MainWindow::clickedFunction function = &MainWindow::on_RadioGame5_clicked;
    handleWinner(ui->AlphaWinGame4, ui->BravoWinGame4, ui->Team1Score, ui->Team2Score, "/teamAlpha/", ui->RadioGame4, ui->RadioGame5, function);
}

void MainWindow::on_AlphaWinGame5_clicked()
{
    MainWindow::clickedFunction function = &MainWindow::on_RadioGame6_clicked;
    handleWinner(ui->AlphaWinGame5, ui->BravoWinGame5, ui->Team1Score, ui->Team2Score, "/teamAlpha/", ui->RadioGame5, ui->RadioGame6, function);
}

void MainWindow::on_AlphaWinGame6_clicked()
{
    MainWindow::clickedFunction function = &MainWindow::on_RadioGame7_clicked;
    handleWinner(ui->AlphaWinGame6, ui->BravoWinGame6, ui->Team1Score, ui->Team2Score, "/teamAlpha/", ui->RadioGame6, ui->RadioGame7, function);
}

void MainWindow::on_AlphaWinGame7_clicked()
{
    MainWindow::clickedFunction function = &MainWindow::on_RadioGame8_clicked;
    handleWinner(ui->AlphaWinGame7, ui->BravoWinGame7, ui->Team1Score, ui->Team2Score, "/teamAlpha/", ui->RadioGame7, ui->RadioGame8, function);
}

void MainWindow::on_AlphaWinGame8_clicked()
{
    MainWindow::clickedFunction function = &MainWindow::on_RadioGame9_clicked;
    handleWinner(ui->AlphaWinGame8, ui->BravoWinGame8, ui->Team1Score, ui->Team2Score, "/teamAlpha/", ui->RadioGame8, ui->RadioGame9, function);
}

void MainWindow::on_AlphaWinGame9_clicked()
{
    MainWindow::clickedFunction function = &MainWindow::on_RadioGame9_clicked;
    handleWinner(ui->AlphaWinGame9, ui->BravoWinGame9, ui->Team1Score, ui->Team2Score, "/teamAlpha/", ui->RadioGame9, ui->RadioGame9, function);
}

void MainWindow::on_BravoWinGame1_clicked()
{
    MainWindow::clickedFunction function = &MainWindow::on_RadioGame2_clicked;
    handleWinner(ui->BravoWinGame1, ui->AlphaWinGame1, ui->Team2Score, ui->Team1Score, "/teamBravo/", ui->RadioGame1, ui->RadioGame2, function);
}

void MainWindow::on_BravoWinGame2_clicked()
{
    MainWindow::clickedFunction function = &MainWindow::on_RadioGame3_clicked;
    handleWinner(ui->BravoWinGame2, ui->AlphaWinGame2, ui->Team2Score, ui->Team1Score, "/teamBravo/", ui->RadioGame2, ui->RadioGame3, function);
}

void MainWindow::on_BravoWinGame3_clicked()
{
    MainWindow::clickedFunction function = &MainWindow::on_RadioGame4_clicked;
    handleWinner(ui->BravoWinGame3, ui->AlphaWinGame3, ui->Team2Score, ui->Team1Score, "/teamBravo/", ui->RadioGame3, ui->RadioGame4, function);
}

void MainWindow::on_BravoWinGame4_clicked()
{
    MainWindow::clickedFunction function = &MainWindow::on_RadioGame5_clicked;
    handleWinner(ui->BravoWinGame4, ui->AlphaWinGame4, ui->Team2Score, ui->Team1Score, "/teamBravo/", ui->RadioGame4, ui->RadioGame5, function);
}

void MainWindow::on_BravoWinGame5_clicked()
{
    MainWindow::clickedFunction function = &MainWindow::on_RadioGame6_clicked;
    handleWinner(ui->BravoWinGame5, ui->AlphaWinGame5, ui->Team2Score, ui->Team1Score, "/teamBravo/", ui->RadioGame5, ui->RadioGame6, function);
}

void MainWindow::on_BravoWinGame6_clicked()
{
    MainWindow::clickedFunction function = &MainWindow::on_RadioGame7_clicked;
    handleWinner(ui->BravoWinGame6, ui->AlphaWinGame6, ui->Team2Score, ui->Team1Score, "/teamBravo/", ui->RadioGame6, ui->RadioGame7, function);
}

void MainWindow::on_BravoWinGame7_clicked()
{
    MainWindow::clickedFunction function = &MainWindow::on_RadioGame8_clicked;
    handleWinner(ui->BravoWinGame7, ui->AlphaWinGame7, ui->Team2Score, ui->Team1Score, "/teamBravo/", ui->RadioGame7, ui->RadioGame8, function);
}

void MainWindow::on_BravoWinGame8_clicked()
{
    MainWindow::clickedFunction function = &MainWindow::on_RadioGame9_clicked;
    handleWinner(ui->BravoWinGame8, ui->AlphaWinGame8, ui->Team2Score, ui->Team1Score, "/teamBravo/", ui->RadioGame8, ui->RadioGame9, function);
}

void MainWindow::on_BravoWinGame9_clicked()
{
    MainWindow::clickedFunction function = &MainWindow::on_RadioGame9_clicked;
    handleWinner(ui->BravoWinGame9, ui->AlphaWinGame9, ui->Team2Score, ui->Team1Score, "/teamBravo/", ui->RadioGame9, ui->RadioGame9, function);
}

/*
/ END Set Section
*/

QMap<QString, QLabel*> MainWindow::weapons_init()
{
    connect(ui->WeaponsResetAlpha, &QPushButton::clicked, this, &MainWindow::resetFields);
    connect(ui->WeaponsResetBravo, &QPushButton::clicked, this, &MainWindow::resetFields);

    connect(ui->WeaponsR1_1, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR1_2, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR1_3, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR1_4, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR2_1, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR2_2, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR2_3, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR2_4, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR3_1, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR3_2, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR3_3, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR3_4, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR4_1, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR4_2, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR4_3, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR4_4, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR5_1, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR5_2, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR5_3, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR5_4, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR6_1, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR6_2, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR6_3, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR6_4, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR7_1, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR7_2, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR7_3, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR7_4, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR8_1, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR8_2, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR8_3, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR8_4, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR9_1, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR9_2, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR9_3, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR9_4, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR1_5, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR1_6, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR1_7, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR1_8, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR2_5, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR2_6, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR2_7, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR2_8, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR3_5, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR3_6, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR3_7, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR3_8, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR4_5, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR4_6, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR4_7, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR4_8, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR5_5, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR5_6, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR5_7, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR5_8, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR6_5, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR6_6, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR6_7, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR6_8, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR7_5, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR7_6, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR7_7, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR7_8, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR8_5, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR8_6, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR8_7, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR8_8, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR9_5, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR9_6, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR9_7, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    connect(ui->WeaponsR9_8, &QLineEdit::editingFinished, this, &MainWindow::updateWeapon);
    QMap<QString, QLabel*> labels{
        {ui->WeaponsR1_1->objectName(), ui->WeaponLabelR1_1},
        {ui->WeaponsR1_2->objectName(), ui->WeaponLabelR1_2},
        {ui->WeaponsR1_3->objectName(), ui->WeaponLabelR1_3},
        {ui->WeaponsR1_4->objectName(), ui->WeaponLabelR1_4},
        {ui->WeaponsR2_1->objectName(), ui->WeaponLabelR2_1},
        {ui->WeaponsR2_2->objectName(), ui->WeaponLabelR2_2},
        {ui->WeaponsR2_3->objectName(), ui->WeaponLabelR2_3},
        {ui->WeaponsR2_4->objectName(), ui->WeaponLabelR2_4},
        {ui->WeaponsR3_1->objectName(), ui->WeaponLabelR3_1},
        {ui->WeaponsR3_2->objectName(), ui->WeaponLabelR3_2},
        {ui->WeaponsR3_3->objectName(), ui->WeaponLabelR3_3},
        {ui->WeaponsR3_4->objectName(), ui->WeaponLabelR3_4},
        {ui->WeaponsR4_1->objectName(), ui->WeaponLabelR4_1},
        {ui->WeaponsR4_2->objectName(), ui->WeaponLabelR4_2},
        {ui->WeaponsR4_3->objectName(), ui->WeaponLabelR4_3},
        {ui->WeaponsR4_4->objectName(), ui->WeaponLabelR4_4},
        {ui->WeaponsR5_1->objectName(), ui->WeaponLabelR5_1},
        {ui->WeaponsR5_2->objectName(), ui->WeaponLabelR5_2},
        {ui->WeaponsR5_3->objectName(), ui->WeaponLabelR5_3},
        {ui->WeaponsR5_4->objectName(), ui->WeaponLabelR5_4},
        {ui->WeaponsR6_1->objectName(), ui->WeaponLabelR6_1},
        {ui->WeaponsR6_2->objectName(), ui->WeaponLabelR6_2},
        {ui->WeaponsR6_3->objectName(), ui->WeaponLabelR6_3},
        {ui->WeaponsR6_4->objectName(), ui->WeaponLabelR6_4},
        {ui->WeaponsR7_1->objectName(), ui->WeaponLabelR7_1},
        {ui->WeaponsR7_2->objectName(), ui->WeaponLabelR7_2},
        {ui->WeaponsR7_3->objectName(), ui->WeaponLabelR7_3},
        {ui->WeaponsR7_4->objectName(), ui->WeaponLabelR7_4},
        {ui->WeaponsR8_1->objectName(), ui->WeaponLabelR8_1},
        {ui->WeaponsR8_2->objectName(), ui->WeaponLabelR8_2},
        {ui->WeaponsR8_3->objectName(), ui->WeaponLabelR8_3},
        {ui->WeaponsR8_4->objectName(), ui->WeaponLabelR8_4},
        {ui->WeaponsR9_1->objectName(), ui->WeaponLabelR9_1},
        {ui->WeaponsR9_2->objectName(), ui->WeaponLabelR9_2},
        {ui->WeaponsR9_3->objectName(), ui->WeaponLabelR9_3},
        {ui->WeaponsR9_4->objectName(), ui->WeaponLabelR9_4},
        {ui->WeaponsR1_5->objectName(), ui->WeaponLabelR1_5},
        {ui->WeaponsR1_6->objectName(), ui->WeaponLabelR1_6},
        {ui->WeaponsR1_7->objectName(), ui->WeaponLabelR1_7},
        {ui->WeaponsR1_8->objectName(), ui->WeaponLabelR1_8},
        {ui->WeaponsR2_5->objectName(), ui->WeaponLabelR2_5},
        {ui->WeaponsR2_6->objectName(), ui->WeaponLabelR2_6},
        {ui->WeaponsR2_7->objectName(), ui->WeaponLabelR2_7},
        {ui->WeaponsR2_8->objectName(), ui->WeaponLabelR2_8},
        {ui->WeaponsR3_5->objectName(), ui->WeaponLabelR3_5},
        {ui->WeaponsR3_6->objectName(), ui->WeaponLabelR3_6},
        {ui->WeaponsR3_7->objectName(), ui->WeaponLabelR3_7},
        {ui->WeaponsR3_8->objectName(), ui->WeaponLabelR3_8},
        {ui->WeaponsR4_5->objectName(), ui->WeaponLabelR4_5},
        {ui->WeaponsR4_6->objectName(), ui->WeaponLabelR4_6},
        {ui->WeaponsR4_7->objectName(), ui->WeaponLabelR4_7},
        {ui->WeaponsR4_8->objectName(), ui->WeaponLabelR4_8},
        {ui->WeaponsR5_5->objectName(), ui->WeaponLabelR5_5},
        {ui->WeaponsR5_6->objectName(), ui->WeaponLabelR5_6},
        {ui->WeaponsR5_7->objectName(), ui->WeaponLabelR5_7},
        {ui->WeaponsR5_8->objectName(), ui->WeaponLabelR5_8},
        {ui->WeaponsR6_5->objectName(), ui->WeaponLabelR6_5},
        {ui->WeaponsR6_6->objectName(), ui->WeaponLabelR6_6},
        {ui->WeaponsR6_7->objectName(), ui->WeaponLabelR6_7},
        {ui->WeaponsR6_8->objectName(), ui->WeaponLabelR6_8},
        {ui->WeaponsR7_5->objectName(), ui->WeaponLabelR7_5},
        {ui->WeaponsR7_6->objectName(), ui->WeaponLabelR7_6},
        {ui->WeaponsR7_7->objectName(), ui->WeaponLabelR7_7},
        {ui->WeaponsR7_8->objectName(), ui->WeaponLabelR7_8},
        {ui->WeaponsR8_5->objectName(), ui->WeaponLabelR8_5},
        {ui->WeaponsR8_6->objectName(), ui->WeaponLabelR8_6},
        {ui->WeaponsR8_7->objectName(), ui->WeaponLabelR8_7},
        {ui->WeaponsR8_8->objectName(), ui->WeaponLabelR8_8},
        {ui->WeaponsR9_5->objectName(), ui->WeaponLabelR9_5},
        {ui->WeaponsR9_6->objectName(), ui->WeaponLabelR9_6},
        {ui->WeaponsR9_7->objectName(), ui->WeaponLabelR9_7},
        {ui->WeaponsR9_8->objectName(), ui->WeaponLabelR9_8}
    };
    return labels;
}

QMap<QString, MyWidget*> MainWindow::widgets_init()
{
    QString filenames[] = {
        "name.txt", "logo", "color", "score.txt", "name.txt",
        "logo", "color", "score.txt", "title.txt", "subtitle.txt", "twitter.txt",
        "message.txt", "tournamentLogo", "1Name.txt", "2Name.txt", "3Name.txt", "1Twitter.txt",
        "2Twitter.txt", "3Twitter.txt", "round.txt", "colorGroups", "color", "1",
        "2", "3", "4", "5", "6", "7",
        "8", "9", "1", "2", "3", "4",
        "5", "6", "7", "8", "9", "currentGame.txt",
        "currentGame.txt", "currentGame.txt", "currentGame.txt", "currentGame.txt", "currentGame.txt", "currentGame.txt",
        "currentGame.txt", "currentGame.txt", "1", "2", "3", "4",
        "5", "6", "7", "8", "9", "1",
        "2", "3", "4", "5", "6", "7",
        "8", "9"
    };
    QString importDirs[] = {
        "/teamAlpha/", "/teamAlpha/", "/teamAlpha/", "/teamAlpha/", "/teamBravo/",
        "/teamBravo/", "/teamBravo/", "/teamBravo/", "/info/", "/info/", "/info/",
        "/info/", "/info/", "/casters/", "/casters/", "/casters/", "/casters/",
        "/casters/", "/casters/", "/round/", "/round/", "/round/", "/set/winners/",
        "/set/winners/", "/set/winners/", "/set/winners/", "/set/winners/", "/set/winners/", "/set/winners/",
        "/set/winners/", "/set/winners/", "/set/winners/", "/set/winners/", "/set/winners/", "/set/winners/",
        "/set/winners/", "/set/winners/", "/set/winners/", "/set/winners/", "/set/winners/", "/set/",
        "/set/", "/set/", "/set/", "/set/", "/set/", "/set/",
        "/set/", "/set/", "/set/maps/", "/set/maps/", "/set/maps/", "/set/maps/",
        "/set/maps/", "/set/maps/", "/set/maps/", "/set/maps/", "/set/maps/", "/set/modes/",
        "/set/modes/", "/set/modes/", "/set/modes/", "/set/modes/", "/set/modes/", "/set/modes/",
        "/set/modes/", "/set/modes/"
    };
    MyWidget *arr[widAmount];
    arr[0] = new LineEditWidget(importDirs[0], filenames[0]);
    arr[1] = new LabelWidget(importDirs[1], filenames[1]);
    arr[2] = new LabelWidget(importDirs[2], filenames[2]);
    arr[3] = new SpinBoxWidget(importDirs[3], filenames[3]);
    arr[4] = new LineEditWidget(importDirs[4], filenames[4]);
    arr[5] = new LabelWidget(importDirs[5], filenames[5]);
    arr[6] = new LabelWidget(importDirs[6], filenames[6]);
    arr[7] = new SpinBoxWidget(importDirs[7], filenames[7]);
    arr[8] = new LineEditWidget(importDirs[8], filenames[8]);
    arr[9] = new LineEditWidget(importDirs[9], filenames[9]);
    arr[10] = new LineEditWidget(importDirs[10], filenames[10]);
    arr[11] = new PlainTextWidget(importDirs[11], filenames[11]);
    arr[12] = new ComboBoxWidget(importDirs[12], filenames[12]);
    arr[13] = new LineEditWidget(importDirs[13], filenames[13]);
    arr[14] = new LineEditWidget(importDirs[14], filenames[14]);
    arr[15] = new LineEditWidget(importDirs[15], filenames[15]);
    arr[16] = new LineEditWidget(importDirs[16], filenames[16]);
    arr[17] = new LineEditWidget(importDirs[17], filenames[17]);
    arr[18] = new LineEditWidget(importDirs[18], filenames[18]);
    arr[19] = new LineEditWidget(importDirs[19], filenames[19]);
    arr[20] = new ComboBoxWidget(importDirs[20], filenames[20]);
    arr[21] = new ComboBoxWidget(importDirs[21], filenames[21]);
    arr[22] = new CheckBoxWidget(importDirs[22], filenames[22]);
    arr[23] = new CheckBoxWidget(importDirs[23], filenames[23]);
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
    arr[40] = new RadioButtonWidget(importDirs[40], filenames[40]);
    arr[41] = new RadioButtonWidget(importDirs[41], filenames[41]);
    arr[42] = new RadioButtonWidget(importDirs[42], filenames[42]);
    arr[43] = new RadioButtonWidget(importDirs[43], filenames[43]);
    arr[44] = new RadioButtonWidget(importDirs[44], filenames[44]);
    arr[45] = new RadioButtonWidget(importDirs[45], filenames[45]);
    arr[46] = new RadioButtonWidget(importDirs[46], filenames[46]);
    arr[47] = new RadioButtonWidget(importDirs[47], filenames[47]);
    arr[48] = new RadioButtonWidget(importDirs[48], filenames[48]);
    arr[49] = new LineEditWidget(importDirs[49], filenames[49]);
    arr[50] = new LineEditWidget(importDirs[50], filenames[50]);
    arr[51] = new LineEditWidget(importDirs[51], filenames[51]);
    arr[52] = new LineEditWidget(importDirs[52], filenames[52]);
    arr[53] = new LineEditWidget(importDirs[53], filenames[53]);
    arr[54] = new LineEditWidget(importDirs[54], filenames[54]);
    arr[55] = new LineEditWidget(importDirs[55], filenames[55]);
    arr[56] = new LineEditWidget(importDirs[56], filenames[56]);
    arr[57] = new LineEditWidget(importDirs[57], filenames[57]);
    arr[58] = new ComboBoxWidget(importDirs[58], filenames[58]);
    arr[59] = new ComboBoxWidget(importDirs[59], filenames[59]);
    arr[60] = new ComboBoxWidget(importDirs[60], filenames[60]);
    arr[61] = new ComboBoxWidget(importDirs[61], filenames[61]);
    arr[62] = new ComboBoxWidget(importDirs[62], filenames[62]);
    arr[63] = new ComboBoxWidget(importDirs[63], filenames[63]);
    arr[64] = new ComboBoxWidget(importDirs[64], filenames[64]);
    arr[65] = new ComboBoxWidget(importDirs[65], filenames[65]);
    arr[66] = new ComboBoxWidget(importDirs[66], filenames[66]);
    QMap<QString, MyWidget*> widgets{
        {ui->Team1NameEdit ->objectName(), arr[0]},
        {ui->Team1Logo ->objectName(), arr[1]},
        {ui->Team1Color ->objectName(), arr[2]},
        {ui->Team1Score ->objectName(), arr[3]},
        {ui->Team2NameEdit ->objectName(), arr[4]},
        {ui->Team2Logo ->objectName(), arr[5]},
        {ui->Team2Color ->objectName(), arr[6]},
        {ui->Team2Score ->objectName(), arr[7]},
        {ui->TitleEdit ->objectName(), arr[8]},
        {ui->SubtitleEdit ->objectName(), arr[9]},
        {ui->TwitterEdit ->objectName(), arr[10]},
        {ui->MessageEdit ->objectName(), arr[11]},
        {ui->LogoCombo ->objectName(), arr[12]},
        {ui->Caster1NameEdit ->objectName(), arr[13]},
        {ui->Caster2NameEdit ->objectName(), arr[14]},
        {ui->Caster3NameEdit ->objectName(), arr[15]},
        {ui->Caster1TwitterEdit ->objectName(), arr[16]},
        {ui->Caster2TwitterEdit ->objectName(), arr[17]},
        {ui->Caster3TwitterEdit ->objectName(), arr[18]},
        {ui->RoundEdit ->objectName(), arr[19]},
        {ui->ColorGroupCombo ->objectName(), arr[20]},
        {ui->ColorsCombo ->objectName(), arr[21]},
        {ui->AlphaWinGame1 ->objectName(), arr[22]},
        {ui->AlphaWinGame2 ->objectName(), arr[23]},
        {ui->AlphaWinGame3 ->objectName(), arr[24]},
        {ui->AlphaWinGame4 ->objectName(), arr[25]},
        {ui->AlphaWinGame5 ->objectName(), arr[26]},
        {ui->AlphaWinGame6 ->objectName(), arr[27]},
        {ui->AlphaWinGame7 ->objectName(), arr[28]},
        {ui->AlphaWinGame8 ->objectName(), arr[29]},
        {ui->AlphaWinGame9 ->objectName(), arr[30]},
        {ui->BravoWinGame1 ->objectName(), arr[31]},
        {ui->BravoWinGame2 ->objectName(), arr[32]},
        {ui->BravoWinGame3 ->objectName(), arr[33]},
        {ui->BravoWinGame4 ->objectName(), arr[34]},
        {ui->BravoWinGame5 ->objectName(), arr[35]},
        {ui->BravoWinGame6 ->objectName(), arr[36]},
        {ui->BravoWinGame7 ->objectName(), arr[37]},
        {ui->BravoWinGame8 ->objectName(), arr[38]},
        {ui->BravoWinGame9 ->objectName(), arr[39]},
        {ui->RadioGame1 ->objectName(), arr[40]},
        {ui->RadioGame2 ->objectName(), arr[41]},
        {ui->RadioGame3 ->objectName(), arr[42]},
        {ui->RadioGame4 ->objectName(), arr[43]},
        {ui->RadioGame5 ->objectName(), arr[44]},
        {ui->RadioGame6 ->objectName(), arr[45]},
        {ui->RadioGame7 ->objectName(), arr[46]},
        {ui->RadioGame8 ->objectName(), arr[47]},
        {ui->RadioGame9 ->objectName(), arr[48]},
        {ui->MapComboGame1 ->objectName(), arr[49]},
        {ui->MapComboGame2 ->objectName(), arr[50]},
        {ui->MapComboGame3 ->objectName(), arr[51]},
        {ui->MapComboGame4 ->objectName(), arr[52]},
        {ui->MapComboGame5 ->objectName(), arr[53]},
        {ui->MapComboGame6 ->objectName(), arr[54]},
        {ui->MapComboGame7 ->objectName(), arr[55]},
        {ui->MapComboGame8 ->objectName(), arr[56]},
        {ui->MapComboGame9 ->objectName(), arr[57]},
        {ui->ModeComboGame1 ->objectName(), arr[58]},
        {ui->ModeComboGame2 ->objectName(), arr[59]},
        {ui->ModeComboGame3 ->objectName(), arr[60]},
        {ui->ModeComboGame4 ->objectName(), arr[61]},
        {ui->ModeComboGame5 ->objectName(), arr[62]},
        {ui->ModeComboGame6 ->objectName(), arr[63]},
        {ui->ModeComboGame7 ->objectName(), arr[64]},
        {ui->ModeComboGame8 ->objectName(), arr[65]},
        {ui->ModeComboGame9 ->objectName(), arr[66]},
        {ui->WeaponsR1_1->objectName(), new LineEditWidget("/set/weapons/", "R1_1")},
        {ui->WeaponsR1_2->objectName(), new LineEditWidget("/set/weapons/", "R1_2")},
        {ui->WeaponsR1_3->objectName(), new LineEditWidget("/set/weapons/", "R1_3")},
        {ui->WeaponsR1_4->objectName(), new LineEditWidget("/set/weapons/", "R1_4")},
        {ui->WeaponsR2_1->objectName(), new LineEditWidget("/set/weapons/", "R2_1")},
        {ui->WeaponsR2_2->objectName(), new LineEditWidget("/set/weapons/", "R2_2")},
        {ui->WeaponsR2_3->objectName(), new LineEditWidget("/set/weapons/", "R2_3")},
        {ui->WeaponsR2_4->objectName(), new LineEditWidget("/set/weapons/", "R2_4")},
        {ui->WeaponsR3_1->objectName(), new LineEditWidget("/set/weapons/", "R3_1")},
        {ui->WeaponsR3_2->objectName(), new LineEditWidget("/set/weapons/", "R3_2")},
        {ui->WeaponsR3_3->objectName(), new LineEditWidget("/set/weapons/", "R3_3")},
        {ui->WeaponsR3_4->objectName(), new LineEditWidget("/set/weapons/", "R3_4")},
        {ui->WeaponsR4_1->objectName(), new LineEditWidget("/set/weapons/", "R4_1")},
        {ui->WeaponsR4_2->objectName(), new LineEditWidget("/set/weapons/", "R4_2")},
        {ui->WeaponsR4_3->objectName(), new LineEditWidget("/set/weapons/", "R4_3")},
        {ui->WeaponsR4_4->objectName(), new LineEditWidget("/set/weapons/", "R4_4")},
        {ui->WeaponsR5_1->objectName(), new LineEditWidget("/set/weapons/", "R5_1")},
        {ui->WeaponsR5_2->objectName(), new LineEditWidget("/set/weapons/", "R5_2")},
        {ui->WeaponsR5_3->objectName(), new LineEditWidget("/set/weapons/", "R5_3")},
        {ui->WeaponsR5_4->objectName(), new LineEditWidget("/set/weapons/", "R5_4")},
        {ui->WeaponsR6_1->objectName(), new LineEditWidget("/set/weapons/", "R6_1")},
        {ui->WeaponsR6_2->objectName(), new LineEditWidget("/set/weapons/", "R6_2")},
        {ui->WeaponsR6_3->objectName(), new LineEditWidget("/set/weapons/", "R6_3")},
        {ui->WeaponsR6_4->objectName(), new LineEditWidget("/set/weapons/", "R6_4")},
        {ui->WeaponsR7_1->objectName(), new LineEditWidget("/set/weapons/", "R7_1")},
        {ui->WeaponsR7_2->objectName(), new LineEditWidget("/set/weapons/", "R7_2")},
        {ui->WeaponsR7_3->objectName(), new LineEditWidget("/set/weapons/", "R7_3")},
        {ui->WeaponsR7_4->objectName(), new LineEditWidget("/set/weapons/", "R7_4")},
        {ui->WeaponsR8_1->objectName(), new LineEditWidget("/set/weapons/", "R8_1")},
        {ui->WeaponsR8_2->objectName(), new LineEditWidget("/set/weapons/", "R8_2")},
        {ui->WeaponsR8_3->objectName(), new LineEditWidget("/set/weapons/", "R8_3")},
        {ui->WeaponsR8_4->objectName(), new LineEditWidget("/set/weapons/", "R8_4")},
        {ui->WeaponsR9_1->objectName(), new LineEditWidget("/set/weapons/", "R9_1")},
        {ui->WeaponsR9_2->objectName(), new LineEditWidget("/set/weapons/", "R9_2")},
        {ui->WeaponsR9_3->objectName(), new LineEditWidget("/set/weapons/", "R9_3")},
        {ui->WeaponsR9_4->objectName(), new LineEditWidget("/set/weapons/", "R9_4")},
        {ui->WeaponsR1_5->objectName(), new LineEditWidget("/set/weapons/", "R1_5")},
        {ui->WeaponsR1_6->objectName(), new LineEditWidget("/set/weapons/", "R1_6")},
        {ui->WeaponsR1_7->objectName(), new LineEditWidget("/set/weapons/", "R1_7")},
        {ui->WeaponsR1_8->objectName(), new LineEditWidget("/set/weapons/", "R1_8")},
        {ui->WeaponsR2_5->objectName(), new LineEditWidget("/set/weapons/", "R2_5")},
        {ui->WeaponsR2_6->objectName(), new LineEditWidget("/set/weapons/", "R2_6")},
        {ui->WeaponsR2_7->objectName(), new LineEditWidget("/set/weapons/", "R2_7")},
        {ui->WeaponsR2_8->objectName(), new LineEditWidget("/set/weapons/", "R2_8")},
        {ui->WeaponsR3_5->objectName(), new LineEditWidget("/set/weapons/", "R3_5")},
        {ui->WeaponsR3_6->objectName(), new LineEditWidget("/set/weapons/", "R3_6")},
        {ui->WeaponsR3_7->objectName(), new LineEditWidget("/set/weapons/", "R3_7")},
        {ui->WeaponsR3_8->objectName(), new LineEditWidget("/set/weapons/", "R3_8")},
        {ui->WeaponsR4_5->objectName(), new LineEditWidget("/set/weapons/", "R4_5")},
        {ui->WeaponsR4_6->objectName(), new LineEditWidget("/set/weapons/", "R4_6")},
        {ui->WeaponsR4_7->objectName(), new LineEditWidget("/set/weapons/", "R4_7")},
        {ui->WeaponsR4_8->objectName(), new LineEditWidget("/set/weapons/", "R4_8")},
        {ui->WeaponsR5_5->objectName(), new LineEditWidget("/set/weapons/", "R5_5")},
        {ui->WeaponsR5_6->objectName(), new LineEditWidget("/set/weapons/", "R5_6")},
        {ui->WeaponsR5_7->objectName(), new LineEditWidget("/set/weapons/", "R5_7")},
        {ui->WeaponsR5_8->objectName(), new LineEditWidget("/set/weapons/", "R5_8")},
        {ui->WeaponsR6_5->objectName(), new LineEditWidget("/set/weapons/", "R6_5")},
        {ui->WeaponsR6_6->objectName(), new LineEditWidget("/set/weapons/", "R6_6")},
        {ui->WeaponsR6_7->objectName(), new LineEditWidget("/set/weapons/", "R6_7")},
        {ui->WeaponsR6_8->objectName(), new LineEditWidget("/set/weapons/", "R6_8")},
        {ui->WeaponsR7_5->objectName(), new LineEditWidget("/set/weapons/", "R7_5")},
        {ui->WeaponsR7_6->objectName(), new LineEditWidget("/set/weapons/", "R7_6")},
        {ui->WeaponsR7_7->objectName(), new LineEditWidget("/set/weapons/", "R7_7")},
        {ui->WeaponsR7_8->objectName(), new LineEditWidget("/set/weapons/", "R7_8")},
        {ui->WeaponsR8_5->objectName(), new LineEditWidget("/set/weapons/", "R8_5")},
        {ui->WeaponsR8_6->objectName(), new LineEditWidget("/set/weapons/", "R8_6")},
        {ui->WeaponsR8_7->objectName(), new LineEditWidget("/set/weapons/", "R8_7")},
        {ui->WeaponsR8_8->objectName(), new LineEditWidget("/set/weapons/", "R8_8")},
        {ui->WeaponsR9_5->objectName(), new LineEditWidget("/set/weapons/", "R9_5")},
        {ui->WeaponsR9_6->objectName(), new LineEditWidget("/set/weapons/", "R9_6")},
        {ui->WeaponsR9_7->objectName(), new LineEditWidget("/set/weapons/", "R9_7")},
        {ui->WeaponsR9_8->objectName(), new LineEditWidget("/set/weapons/", "R9_8")},
    };
    return widgets;
}

QString MainWindow::getColorsLocal()
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
    return local;
}

void MainWindow::on_ColorsCombo_activated(int index)
{
    QString local = this->getColorsLocal();
    ComboBoxWidget *colorsCombo {dynamic_cast<ComboBoxWidget *>(this->widgetFor[ui->ColorsCombo->objectName()])};
    colorsCombo->saveColors(
        this->directoryFor["/ImportingFilesDirectory.txt"],
        ui->ColorsCombo,
        local,
        ui->Team1Color,
        ui->Team2Color,
        ui->BothTeamsColor,
        this->mirrored
    );
}

void MainWindow::on_SwapColorsButton_clicked()
{
    this->mirrored = !this->mirrored;
    this->on_ColorsCombo_activated(ui->ColorsCombo->currentIndex());
    if (this->mirrored) {
        ui->SwapColorsButton->setText("Unreverse Colors");
    } else {
        ui->SwapColorsButton->setText("Reverse Colors");
    }
}


void MainWindow::on_SwapTeamsButton_clicked()
{
    QString importDir = this->directoryFor["/ImportingFilesDirectory.txt"];
    QString defaultDir = this->directoryFor["/DefaultDirectory.txt"];
    // Team Alpha
    QString alphaDir = this->directoryFor["/AlphaTeamDirectory.txt"];
    LineEditWidget *alphaName {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Team1NameEdit->objectName()])};
    LabelWidget *alphaLogo {dynamic_cast<LabelWidget *>(this->widgetFor[ui->Team1Logo->objectName()])};
    SpinBoxWidget *alphaScore {dynamic_cast<SpinBoxWidget *>(this->widgetFor[ui->Team1Score->objectName()])};
    // Team Bravo
    QString bravoDir = this->directoryFor["/BravoTeamDirectory.txt"];
    LineEditWidget *bravoName {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->Team2NameEdit->objectName()])};
    LabelWidget *bravoLogo {dynamic_cast<LabelWidget *>(this->widgetFor[ui->Team2Logo->objectName()])};
    SpinBoxWidget *bravoScore {dynamic_cast<SpinBoxWidget *>(this->widgetFor[ui->Team2Score->objectName()])};
    // Aux = Alpha
    QString auxName = ui->Team1NameEdit->text();
    int auxScore = ui->Team1Score->value();
    // Alpha = Bravo
    this->teamAlphaOptionalFiles = this->clearOptionalFiles(this->teamAlphaOptionalFiles, importDir + "/teamAlpha/");
    ui->Team1NameEdit->setText(ui->Team2NameEdit->text());
    ui->Team1Score->setValue(ui->Team2Score->value());
    alphaName->saveInFile(importDir, ui->Team1NameEdit);
    alphaLogo->init(importDir, ui->Team1Logo, bravoDir, "/" + ui->Team1NameEdit->text(), defaultDir, alphaDir);
    alphaScore->saveInFile(importDir, ui->Team1Score);
    this->teamAlphaOptionalFiles = this->saveOptionalFiles(alphaDir + "/" + ui->Team1NameEdit->text() + "/", importDir + "/teamAlpha/");
    // Bravo = Aux
    this->teamBravoOptionalFiles = this->clearOptionalFiles(this->teamBravoOptionalFiles, importDir + "/teamBravo/");
    ui->Team2NameEdit->setText(auxName);
    ui->Team2Score->setValue(auxScore);
    bravoName->saveInFile(importDir, ui->Team2NameEdit);
    bravoLogo->init(importDir, ui->Team2Logo, alphaDir, "/" + ui->Team2NameEdit->text(), defaultDir, bravoDir);
    bravoScore->saveInFile(importDir, ui->Team2Score);
    this->teamBravoOptionalFiles = this->saveOptionalFiles(alphaDir + "/" + ui->Team2NameEdit->text() + "/", importDir + "/teamBravo/");
    // Save file with both team names
    QString teamNames = ui->Team1NameEdit->text() + " - " + ui->Team2NameEdit->text();
    alphaName->saveInFile(importDir, ui->Team1NameEdit, "/round/", "teamNames.txt", teamNames);
    // Change Wins
    // Aux = Alpha
    bool aux1 = ui->AlphaWinGame1->isChecked();
    bool aux2 = ui->AlphaWinGame2->isChecked();
    bool aux3 = ui->AlphaWinGame3->isChecked();
    bool aux4 = ui->AlphaWinGame4->isChecked();
    bool aux5 = ui->AlphaWinGame5->isChecked();
    bool aux6 = ui->AlphaWinGame6->isChecked();
    bool aux7 = ui->AlphaWinGame7->isChecked();
    bool aux8 = ui->AlphaWinGame8->isChecked();
    bool aux9 = ui->AlphaWinGame9->isChecked();
    // Alpha = Bravo
    ui->AlphaWinGame1->setChecked(ui->BravoWinGame1->isChecked());
    ui->AlphaWinGame2->setChecked(ui->BravoWinGame2->isChecked());
    ui->AlphaWinGame3->setChecked(ui->BravoWinGame3->isChecked());
    ui->AlphaWinGame4->setChecked(ui->BravoWinGame4->isChecked());
    ui->AlphaWinGame5->setChecked(ui->BravoWinGame5->isChecked());
    ui->AlphaWinGame6->setChecked(ui->BravoWinGame6->isChecked());
    ui->AlphaWinGame7->setChecked(ui->BravoWinGame7->isChecked());
    ui->AlphaWinGame8->setChecked(ui->BravoWinGame8->isChecked());
    ui->AlphaWinGame9->setChecked(ui->BravoWinGame9->isChecked());
    // Bravo = Aux
    ui->BravoWinGame1->setChecked(aux1);
    ui->BravoWinGame2->setChecked(aux2);
    ui->BravoWinGame3->setChecked(aux3);
    ui->BravoWinGame4->setChecked(aux4);
    ui->BravoWinGame5->setChecked(aux5);
    ui->BravoWinGame6->setChecked(aux6);
    ui->BravoWinGame7->setChecked(aux7);
    ui->BravoWinGame8->setChecked(aux8);
    ui->BravoWinGame9->setChecked(aux9);
}

//void MainWindow::on_actionInfoAndCasters_toggled(bool arg1)
//{
//    if (arg1) {
//        ui->InfoGroupbox->show();
//        ui->CastersGroupbox->show();
//        QApplication::processEvents();
//        this->adjustSize();
//    } else {
//        ui->InfoGroupbox->hide();
//        ui->CastersGroupbox->hide();
//        QApplication::processEvents();
//        this->adjustSize();
//    }
//}

void MainWindow::updateWeapon()
{
    QObject *wid = QObject::sender();
    LineEditWidget *weapon {dynamic_cast<LineEditWidget *>(this->widgetFor[wid->objectName()])};
    LabelWidget *label {dynamic_cast<LabelWidget *>(this->widgetFor[wid->objectName()])};
    QString importDir = this->directoryFor["/ImportingFilesDirectory.txt"];
    QString defaultDir = this->directoryFor["/DefaultDirectory.txt"];
    QString weaponsDir = this->directoryFor["/WeaponsDirectory.txt"];
    weapon->saveWithImage(importDir, dynamic_cast<QLineEdit *>(wid), weaponsDir, defaultDir + "/weapons/");
    QLineEdit *weap = (QLineEdit *)wid;
    if (weap->text().isEmpty()) {
        int seq = wid->objectName().right(1).toInt();
        if (seq > 4) seq -= 4;
        ui->TeamsTabs->findChild<QLabel *>("WeaponLabel" + wid->objectName().right(4))->setText(QString::number(seq));
        return;
    }
    label->keepAspectInLabel(weaponsDir + "/" + dynamic_cast<QLineEdit *>(wid)->text(), this->labelFor[wid->objectName()]);
}

void MainWindow::labelClicked()
{
    QObject *wid = QObject::sender();
    QString round = wid->objectName().right(1);
    QString type = wid->objectName().left(6);
    QString team = wid->objectName().left(11).right(5);
    QString prevRound = QString::number(round.toInt() - 1);
    if (round == "1") prevRound = "1";
    QString label = type + "sR" + round + "_";
    QString content;
    int offset = 0;
    if (team == "Bravo") offset += 4;
    if (type == "Weapon") {
        QLineEdit *prev1;
        QLineEdit *current1;
        QLineEdit *prev2;
        QLineEdit *current2;
        QLineEdit *prev3;
        QLineEdit *current3;
        QLineEdit *prev4;
        QLineEdit *current4;
        current1 = ui->TeamsTabs->findChild<QLineEdit *>(label + QString::number(1 + offset));
        current2 = ui->TeamsTabs->findChild<QLineEdit *>(label + QString::number(2 + offset));
        current3 = ui->TeamsTabs->findChild<QLineEdit *>(label + QString::number(3 + offset));
        current4 = ui->TeamsTabs->findChild<QLineEdit *>(label + QString::number(4 + offset));
        content = current1->text() + current2->text() + current3->text() + current4->text();
        if (!content.isEmpty()) {
            current1->clear();
            current2->clear();
            current3->clear();
            current4->clear();
        } else {
            prev1 = ui->TeamsTabs->findChild<QLineEdit *>(type + "sR" + prevRound + "_" + QString::number(1 + offset));
            prev2 = ui->TeamsTabs->findChild<QLineEdit *>(type + "sR" + prevRound + "_" + QString::number(2 + offset));
            prev3 = ui->TeamsTabs->findChild<QLineEdit *>(type + "sR" + prevRound + "_" + QString::number(3 + offset));
            prev4 = ui->TeamsTabs->findChild<QLineEdit *>(type + "sR" + prevRound + "_" + QString::number(4 + offset));
            current1->setText(prev1->text());
            current2->setText(prev2->text());
            current3->setText(prev3->text());
            current4->setText(prev4->text());
        }
        emit current1->editingFinished();
        emit current2->editingFinished();
        emit current3->editingFinished();
        emit current4->editingFinished();
    } else {
        QComboBox *prev1;
        QComboBox *current1;
        QComboBox *prev2;
        QComboBox *current2;
        QComboBox *prev3;
        QComboBox *current3;
        QComboBox *prev4;
        QComboBox *current4;
        current1 = ui->TeamsTabs->findChild<QComboBox *>(label + QString::number(1 + offset));
        current2 = ui->TeamsTabs->findChild<QComboBox *>(label + QString::number(2 + offset));
        current3 = ui->TeamsTabs->findChild<QComboBox *>(label + QString::number(3 + offset));
        current4 = ui->TeamsTabs->findChild<QComboBox *>(label + QString::number(4 + offset));
        content = current1->currentText() + current2->currentText() + current3->currentText() + current4->currentText();
        if (!content.isEmpty()) {
            current1->setCurrentText("");
            emit current1->activated(current1->currentIndex());
            current2->setCurrentText("");
            emit current2->activated(current2->currentIndex());
            current3->setCurrentText("");
            emit current3->activated(current3->currentIndex());
            current4->setCurrentText("");
            emit current4->activated(current4->currentIndex());
        } else {
            if (prevRound == "1" && round == "1") {
                this->fillFirstPlayers(team);
                return;
            }
            prev1 = ui->TeamsTabs->findChild<QComboBox *>(type + "sR" + prevRound + "_" + QString::number(1 + offset));
            prev2 = ui->TeamsTabs->findChild<QComboBox *>(type + "sR" + prevRound + "_" + QString::number(2 + offset));
            prev3 = ui->TeamsTabs->findChild<QComboBox *>(type + "sR" + prevRound + "_" + QString::number(3 + offset));
            prev4 = ui->TeamsTabs->findChild<QComboBox *>(type + "sR" + prevRound + "_" + QString::number(4 + offset));
            current1->setCurrentText(prev1->currentText());
            current2->setCurrentText(prev2->currentText());
            current3->setCurrentText(prev3->currentText());
            current4->setCurrentText(prev4->currentText());
        }
        emit current1->activated(current1->currentIndex());
        emit current2->activated(current2->currentIndex());
        emit current3->activated(current3->currentIndex());
        emit current4->activated(current4->currentIndex());
    }
}

void MainWindow::resetFields(){
    QString team = QObject::sender()->objectName().right(5);
    QString type = QObject::sender()->objectName().left(7);
    QLineEdit *field;
    QComboBox *combo;
    int offset = 0;
    if (team == "Bravo") offset += 4;
    if (type == "Weapons") {
        for (int i = 1 + offset; i < 5 + offset; ++i) {
            field = ui->TeamsTabs->findChild<QLineEdit *>(type + "R" + "1" + "_" + QString::number(i));
            field->clear();
            emit field->editingFinished();
            field = ui->TeamsTabs->findChild<QLineEdit *>(type + "R" + "2" + "_" + QString::number(i));
            field->clear();
            emit field->editingFinished();
            field = ui->TeamsTabs->findChild<QLineEdit *>(type + "R" + "3" + "_" + QString::number(i));
            field->clear();
            emit field->editingFinished();
            field = ui->TeamsTabs->findChild<QLineEdit *>(type + "R" + "4" + "_" + QString::number(i));
            field->clear();
            emit field->editingFinished();
            field = ui->TeamsTabs->findChild<QLineEdit *>(type + "R" + "5" + "_" + QString::number(i));
            field->clear();
            emit field->editingFinished();
            field = ui->TeamsTabs->findChild<QLineEdit *>(type + "R" + "6" + "_" + QString::number(i));
            field->clear();
            emit field->editingFinished();
            field = ui->TeamsTabs->findChild<QLineEdit *>(type + "R" + "7" + "_" + QString::number(i));
            field->clear();
            emit field->editingFinished();
            field = ui->TeamsTabs->findChild<QLineEdit *>(type + "R" + "8" + "_" + QString::number(i));
            field->clear();
            emit field->editingFinished();
            field = ui->TeamsTabs->findChild<QLineEdit *>(type + "R" + "9" + "_" + QString::number(i));
            field->clear();
            emit field->editingFinished();
        }
    } else {
        for (int i = 1 + offset; i < 5 + offset; ++i) {
            combo = ui->TeamsTabs->findChild<QComboBox *>(type + "R" + "1" + "_" + QString::number(i));
            combo->setCurrentIndex(0);
            emit combo->activated(0);
            combo = ui->TeamsTabs->findChild<QComboBox *>(type + "R" + "2" + "_" + QString::number(i));
            combo->setCurrentIndex(0);
            emit combo->activated(combo->currentIndex());
            combo = ui->TeamsTabs->findChild<QComboBox *>(type + "R" + "3" + "_" + QString::number(i));
            combo->setCurrentIndex(0);
            emit combo->activated(combo->currentIndex());
            combo = ui->TeamsTabs->findChild<QComboBox *>(type + "R" + "4" + "_" + QString::number(i));
            combo->setCurrentIndex(0);
            emit combo->activated(combo->currentIndex());
            combo = ui->TeamsTabs->findChild<QComboBox *>(type + "R" + "5" + "_" + QString::number(i));
            combo->setCurrentIndex(0);
            emit combo->activated(combo->currentIndex());
            combo = ui->TeamsTabs->findChild<QComboBox *>(type + "R" + "6" + "_" + QString::number(i));
            combo->setCurrentIndex(0);
            emit combo->activated(combo->currentIndex());
            combo = ui->TeamsTabs->findChild<QComboBox *>(type + "R" + "7" + "_" + QString::number(i));
            combo->setCurrentIndex(0);
            emit combo->activated(combo->currentIndex());
            combo = ui->TeamsTabs->findChild<QComboBox *>(type + "R" + "8" + "_" + QString::number(i));
            combo->setCurrentIndex(0);
            emit combo->activated(combo->currentIndex());
            combo = ui->TeamsTabs->findChild<QComboBox *>(type + "R" + "9" + "_" + QString::number(i));
            combo->setCurrentIndex(0);
            emit combo->activated(0);
        }
    }
}

void MainWindow::roundLabelHovered()
{
    QLabel *label = (QLabel *)QObject::sender();
    label->setText("teste");
}

void MainWindow::updatePlayer()
{
    QObject *wid = QObject::sender();
    ComboBoxWidget *player {dynamic_cast<ComboBoxWidget *>(this->playerWidgetFor[wid->objectName()])};
    player->saveContentInFile(this->directoryFor["/ImportingFilesDirectory.txt"], (QComboBox *)wid);
}

void MainWindow::updatePlayersFromAlpha()
{
    QComboBox *playerWidgets[36] = {
        ui->PlayersR1_1, ui->PlayersR1_2, ui->PlayersR1_3, ui->PlayersR1_4,
        ui->PlayersR2_1, ui->PlayersR2_2, ui->PlayersR2_3, ui->PlayersR2_4,
        ui->PlayersR3_1, ui->PlayersR3_2, ui->PlayersR3_3, ui->PlayersR3_4,
        ui->PlayersR4_1, ui->PlayersR4_2, ui->PlayersR4_3, ui->PlayersR4_4,
        ui->PlayersR5_1, ui->PlayersR5_2, ui->PlayersR5_3, ui->PlayersR5_4,
        ui->PlayersR6_1, ui->PlayersR6_2, ui->PlayersR6_3, ui->PlayersR6_4,
        ui->PlayersR7_1, ui->PlayersR7_2, ui->PlayersR7_3, ui->PlayersR7_4,
        ui->PlayersR8_1, ui->PlayersR8_2, ui->PlayersR8_3, ui->PlayersR8_4,
        ui->PlayersR9_1, ui->PlayersR9_2, ui->PlayersR9_3, ui->PlayersR9_4
    };
    for (int i = 0; i < 36; ++i) {
        playerWidgets[i]->clear();
        ComboBoxWidget *player {dynamic_cast<ComboBoxWidget *>(this->playerWidgetFor[playerWidgets[i]->objectName()])};
        player->initPlayers(this->directoryFor["/ImportingFilesDirectory.txt"], playerWidgets[i]);
    }
}

void MainWindow::updatePlayersFromBravo()
{
    QComboBox *playerWidgets[36] = {
        ui->PlayersR1_5, ui->PlayersR1_6, ui->PlayersR1_7, ui->PlayersR1_8,
        ui->PlayersR2_5, ui->PlayersR2_6, ui->PlayersR2_7, ui->PlayersR2_8,
        ui->PlayersR3_5, ui->PlayersR3_6, ui->PlayersR3_7, ui->PlayersR3_8,
        ui->PlayersR4_5, ui->PlayersR4_6, ui->PlayersR4_7, ui->PlayersR4_8,
        ui->PlayersR5_5, ui->PlayersR5_6, ui->PlayersR5_7, ui->PlayersR5_8,
        ui->PlayersR6_5, ui->PlayersR6_6, ui->PlayersR6_7, ui->PlayersR6_8,
        ui->PlayersR7_5, ui->PlayersR7_6, ui->PlayersR7_7, ui->PlayersR7_8,
        ui->PlayersR8_5, ui->PlayersR8_6, ui->PlayersR8_7, ui->PlayersR8_8,
        ui->PlayersR9_5, ui->PlayersR9_6, ui->PlayersR9_7, ui->PlayersR9_8
    };
    for (int i = 0; i < 36; ++i) {
        playerWidgets[i]->clear();
        ComboBoxWidget *player {dynamic_cast<ComboBoxWidget *>(this->playerWidgetFor[playerWidgets[i]->objectName()])};
        player->initPlayers(this->directoryFor["/ImportingFilesDirectory.txt"], playerWidgets[i]);
    }
}

void MainWindow::fillFirstPlayers(QString team) {
    QComboBox *playerWidgets[4];
    if (team == "Alpha") {
        playerWidgets[0] = ui->PlayersR1_1;
        playerWidgets[1] = ui->PlayersR1_2;
        playerWidgets[2] = ui->PlayersR1_3;
        playerWidgets[3] = ui->PlayersR1_4;
    } else {
        playerWidgets[0] = ui->PlayersR1_5;
        playerWidgets[1] = ui->PlayersR1_6;
        playerWidgets[2] = ui->PlayersR1_7;
        playerWidgets[3] = ui->PlayersR1_8;
    }
    for (int i = 1; i < 5; ++i) {
        if (i < playerWidgets[i - 1]->count()) {
            playerWidgets[i - 1]->setCurrentIndex(i);
            emit playerWidgets[i - 1]->activated(i);
        }
    }
}
