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
    this->autoComplete_init();
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
    // Other Fields
    LineEditWidget *lin {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->TitleEdit->objectName()])};
    lin->init(this->directoryFor["/ImportingFilesDirectory.txt"], ui->TitleEdit);
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
    LineEditWidget *lin {dynamic_cast<LineEditWidget *>(this->widgetFor[ui->TitleEdit->objectName()])};
    lin->saveInFile(this->directoryFor["/ImportingFilesDirectory.txt"], ui->TitleEdit);
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


QMap<QString, MyWidget*> MainWindow::widgets_init()
{
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
    arr[13] = new LineEditWidget(importDirs[13], filenames[13]);
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
        {ui->AlphaWinRound1 ->objectName(), arr[24]},
        {ui->AlphaWinRound2 ->objectName(), arr[25]},
        {ui->AlphaWinRound3 ->objectName(), arr[26]},
        {ui->AlphaWinRound4 ->objectName(), arr[27]},
        {ui->AlphaWinRound5 ->objectName(), arr[28]},
        {ui->AlphaWinRound6 ->objectName(), arr[29]},
        {ui->AlphaWinRound7 ->objectName(), arr[30]},
        {ui->AlphaWinRound8 ->objectName(), arr[31]},
        {ui->AlphaWinRound9 ->objectName(), arr[32]},
        {ui->BetaWinRound1 ->objectName(), arr[33]},
        {ui->BetaWinRound2 ->objectName(), arr[34]},
        {ui->BetaWinRound3 ->objectName(), arr[35]},
        {ui->BetaWinRound4 ->objectName(), arr[36]},
        {ui->BetaWinRound5 ->objectName(), arr[37]},
        {ui->BetaWinRound6 ->objectName(), arr[38]},
        {ui->BetaWinRound7 ->objectName(), arr[39]},
        {ui->BetaWinRound8 ->objectName(), arr[40]},
        {ui->BetaWinRound9 ->objectName(), arr[41]},
        {ui->CheckboxGame1 ->objectName(), arr[42]},
        {ui->CheckboxGame2 ->objectName(), arr[43]},
        {ui->CheckboxGame3 ->objectName(), arr[44]},
        {ui->CheckboxGame4 ->objectName(), arr[45]},
        {ui->CheckboxGame5 ->objectName(), arr[46]},
        {ui->CheckboxGame6 ->objectName(), arr[47]},
        {ui->CheckboxGame7 ->objectName(), arr[48]},
        {ui->CheckboxGame8 ->objectName(), arr[49]},
        {ui->CheckboxGame9 ->objectName(), arr[50]},
        {ui->MapComboRound1 ->objectName(), arr[51]},
        {ui->MapComboRound2 ->objectName(), arr[52]},
        {ui->MapComboRound3 ->objectName(), arr[53]},
        {ui->MapComboRound4 ->objectName(), arr[54]},
        {ui->MapComboRound5 ->objectName(), arr[55]},
        {ui->MapComboRound6 ->objectName(), arr[56]},
        {ui->MapComboRound7 ->objectName(), arr[57]},
        {ui->MapComboRound8 ->objectName(), arr[58]},
        {ui->MapComboRound9 ->objectName(), arr[59]},
        {ui->ModeComboRound1 ->objectName(), arr[60]},
        {ui->ModeComboRound2 ->objectName(), arr[61]},
        {ui->ModeComboRound3 ->objectName(), arr[62]},
        {ui->ModeComboRound4 ->objectName(), arr[63]},
        {ui->ModeComboRound5 ->objectName(), arr[64]},
        {ui->ModeComboRound6 ->objectName(), arr[65]},
        {ui->ModeComboRound7 ->objectName(), arr[66]},
        {ui->ModeComboRound8 ->objectName(), arr[67]},
        {ui->ModeComboRound9 ->objectName(), arr[68]}
    };
    return widgets;
}
