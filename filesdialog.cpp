#include "filesdialog.h"
#include "ui_filesdialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QDebug>

FilesDialog::FilesDialog(QWidget *parent, QMap<QString, QString> *directoryForPointer, QString *directories) :
    QDialog(parent),
    ui(new Ui::FilesDialog)
{
    ui->setupUi(this);
    this->directoryFor = directoryForPointer;
    directories_init(directories);
}

FilesDialog::~FilesDialog()
{
    delete ui;
}

void FilesDialog::makeDefaultDir(QLineEdit *field, QString configFile, QString configDir)
{
    QString dirName;
    QStringList subDirList;
    if (configFile.compare("/AlphaTeamDirectory.txt") == 0) {
        dirName = configDir + "/TeamsDirectory";
    } else if (configFile.compare("/BravoTeamDirectory.txt") == 0) {
        dirName = configDir + "/TeamsDirectory";
    } else if (configFile.compare("/LogosDirectory.txt") == 0) {
        dirName = configDir + "/LogosDirectory";
    } else if (configFile.compare("/MapsDirectory.txt") == 0) {
        dirName = configDir + "/MapsDirectory";
    } else if (configFile.compare("/ModeIconsDirectory.txt") == 0) {
        dirName = configDir + "/ModeIconsDirectory";
    } else if (configFile.compare("/SplatfestColorsDirectory.txt") == 0) {
        dirName = configDir + "/Colors/SplatfestColorsDirectory";
        QDir().mkdir(configDir + "/Colors");
        subDirList << dirName + "/alpha" << dirName + "/bravo" << dirName + "/combo";
    } else if (configFile.compare("/RankedColorsDirectory.txt") == 0) {
        dirName = configDir + "/Colors/RankedColorsDirectory";
        QDir().mkdir(configDir + "/Colors");
        subDirList << dirName + "/alpha" << dirName + "/bravo" << dirName + "/combo";
    } else if (configFile.compare("/TurfWarColorsDirectory.txt") == 0) {
        dirName = configDir + "/Colors/Turf WarColorsDirectory";
        QDir().mkdir(configDir + "/Colors");
        subDirList << dirName + "/alpha" << dirName + "/bravo" << dirName + "/combo";
    } else if (configFile.compare("/ImportingFilesDirectory.txt") == 0) {
        dirName = configDir + "/ImportingFilesDirectory";
        subDirList << dirName + "/casters" << dirName + "/info" << dirName + "/round";
        subDirList << dirName + "/set" << dirName + "/teamAlpha" << dirName + "/teamBravo";
        subDirList << dirName + "/set/maps" << dirName + "/set/modes";
        subDirList << dirName + "/set/winners" << dirName + "/set/winPoints";
        subDirList << dirName + "/set/weapons" << dirName + "/set/winPoints";
    } else if (configFile.compare("/DefaultDirectory.txt") == 0) {
        dirName = configDir + "/DefaultDirectory";
        subDirList << dirName + "/logo";
    } else if (configFile.compare("/WinPointsDirectory.txt") == 0) {
        dirName = configDir + "/WinPointsDirectory";
        subDirList << dirName + "/teamAlpha" << dirName + "/teamBravo";
    } else if (configFile.compare("/WeaponsDirectory.txt") == 0) {
        dirName = configDir + "/WeaponsDirectory";
    }
    field->setText(dirName);
    QDir().mkdir(dirName); // Make sure default dir exists
    for (int i = 0; i < subDirList.size(); ++i) {
        QDir().mkdir(subDirList.at(i));
    }
    saveConfigDirectory(configDir + configFile, field); // Save default dir in config File
}

void FilesDialog::directories_init(QString *dirs)
{
    // put content from config files on lineEdits
    // Initialize fields and directories
    QLineEdit *widgets[dirAmount] {
        ui->AlphaTeamDirectoryEdit, ui->BravoTeamDirectoryEdit, ui->LogosDirectoryEdit,
        ui->MapsDirectoryEdit, ui->ModeIconsDirectoryEdit, ui->SplatfestColorsDirectoryEdit, ui->TurfWarColorsDirectoryEdit,
        ui->RankedColorsDirectoryEdit, ui->ImportingFilesDirectoryEdit, ui->DefaultDirectoryEdit, ui->WinPointsDirectoryEdit,
        ui->WeaponsDirectoryEdit
    };
    for (int i = 0; i < dirAmount; i++) {
        this->fields[i] = widgets[i];
        this->directories[i] = dirs[i];
    }
    QFile file;
    QString directory = QCoreApplication::applicationDirPath() + "/config";
    QString filename;
    QString dirname;
    QString text;
    bool makeDefault = false;
    // for each field
    for (int i = 0; i < dirAmount; i++) {
        filename = directory + this->directories[i];
        file.setFileName(filename);
        // if no saved location create and save location for default
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
             makeDefault = true;
        // If there's a file
        } else {
            QTextStream in(&file);
            text = in.readAll();
            // Unless the file points to an invalid directory
            if (!QDir(text).exists()) {
                makeDefault = true;
            // Set the file content as the config directory
            } else {
                this->fields[i]->setText(text);
            }
        }
        file.close();
        if (makeDefault) {
            this->makeDefaultDir(this->fields[i], dirs[i], directory);
        }
    }
}

int FilesDialog::saveConfigDirectory(QString filename, QLineEdit *lineEdit)
{
    QFile file(filename);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << lineEdit->text();
        file.flush();
        file.close();
        return 0;
    } else {
        return 1;
    }
}

void FilesDialog::on_DirectoriesDialogButtonBox_accepted()
{
    // Initialize config directory
    QString directory = QCoreApplication::applicationDirPath() + "/config";
    QDir dir;
    dir.mkdir(directory);
    // Storing config Directories on config files
    QString filename;
    for (int i = 0; i < dirAmount; i++) {
        filename = directory + this->directories[i];
        saveConfigDirectory(filename, this->fields[i]);
        (*this->directoryFor)[this->directories[i]] = this->fields[i]->text();
    }
    // create subfolders for specific directories
    // Importing Files Directory
    QString dirname = (*this->directoryFor)["/ImportingFilesDirectory.txt"];
    QDir().mkdir(dirname + "/casters");
    QDir().mkdir(dirname + "/info");
    QDir().mkdir(dirname + "/round");
    QDir().mkdir(dirname + "/set");
    QDir().mkdir(dirname + "/teamAlpha");
    QDir().mkdir(dirname + "/teamBravo");
    emit allDone();
}

void FilesDialog::on_AlphaTeamDirectoryHelp_clicked()
{
    QMessageBox::information(this, "Alpha Team Directory", "This Directory will contain the list of teams you will can use for the Alpha Team.\n\nTo add a team, put in this directory an image file of the team logo.\n\nThe team name will be shown as the image file's name (excluding extension [.png / .jpg / .jpeg / .gif])");
}

void FilesDialog::on_BravoTeamDirectoryHelp_clicked()
{
    QMessageBox::information(this, "Bravo Team Directory", "This Directory will contain the list of teams you will can use for the Bravo Team.\n\nYou probably want it to be the same as the Alpha Team directory!\n\nTo add a team, put in this directory an image file of the team logo.\n\nThe team name will be shown as the image file's name (excluding extension [.png / .jpg / .jpeg / .gif])");
}

void FilesDialog::on_LogosDirectoryHelp_clicked()
{
    QMessageBox::information(this, "Logos Directory", "This Directory will contain the Logos.\n\nTo add a logo, put in this directory an image file of the logo.\n\nThe logo name will be shown as the image file's name (excluding extension [.png / .jpg / .jpeg / .gif])");
}

void FilesDialog::on_MapsDirectoryHelp_clicked()
{
    QMessageBox::information(this, "Maps Directory", "This Directory will contain the maps.\n\nTo add a map, put in this directory an image file of the map.\n\nThe map name will be shown as the image file's name (excluding extension [.png / .jpg / .jpeg / .gif])");
}

void FilesDialog::on_ModeIconsDirectoryHelp_clicked()
{
    QMessageBox::information(this, "Modes Directory", "This Directory will contain the modes.\n\nTo add a mode, put in this directory an image file of the mode.\n\nThe mode name will be shown as the image file's name (excluding extension [.png / .jpg / .jpeg / .gif])");
}

void FilesDialog::on_SplatfestColorsDirectoryHelp_clicked()
{
    QMessageBox::information(this, "Splatfest Colors Directory", "This Directory will contain the colors for Splatfest matches.\n\nIf you use images that contain the color for both teams, puth them inside a \"combo\" subdirectory.\nEX: /combo/blueVSorange.png.\n\n If you use separate images for each team color, puth each file with the same name under the \"alpha\" and \"bravo\" subdirectories.\nEX: /alpha/blueVSorange.png and /bravo/blueVSorange.png\n\n If you have more than one set of images that you use (for example, square shaped ones and triangle shaped ones, for some reason), you can put them in subdirectories, with the same name as the ones in the root directory. Ex: you can have \"combo/Blue vs Green.png\", \"combo/inGame/Blue vs Green.png\" and \"combo/triangleShaped/Blue vs Green.png\" and they'll all be coppied to the importing file as \"color\", \"ingame_color\" and \"triangleShaped_color\", as long as you have color images inside the root directory.");
}

void FilesDialog::on_TurfWarColorsDirectoryHelp_clicked()
{
    QMessageBox::information(this, "Turf War Colors Directory", "This Directory will contain the colors for Turf War matches.\n\nIf you use images that contain the color for both teams, puth them inside a \"combo\" subdirectory.\nEX: /combo/blueVSorange.png.\n\n If you use separate images for each team color, puth each file with the same name under the \"alpha\" and \"bravo\" subdirectories.\nEX: /alpha/blueVSorange.png and /bravo/blueVSorange.png\n\n If you have more than one set of images that you use (for example, square shaped ones and triangle shaped ones, for some reason), you can put them in subdirectories, with the same name as the ones in the root directory. Ex: you can have \"combo/Blue vs Green.png\", \"combo/inGame/Blue vs Green.png\" and \"combo/triangleShaped/Blue vs Green.png\" and they'll all be coppied to the importing file as \"color\", \"ingame_color\" and \"triangleShaped_color\", as long as you have color images inside the root directory.");
}

void FilesDialog::on_RankedColorsDirectoryHelp_clicked()
{
    QMessageBox::information(this, "Ranked Colors Directory", "This Directory will contain the colors for Ranked modes matches.\n\nIf you use images that contain the color for both teams, puth them inside a \"combo\" subdirectory.\nEX: /combo/blueVSorange.png.\n\n If you use separate images for each team color, puth each file with the same name under the \"alpha\" and \"bravo\" subdirectories.\nEX: /alpha/blueVSorange.png and /bravo/blueVSorange.png\n\n If you have more than one set of images that you use (for example, square shaped ones and triangle shaped ones, for some reason), you can put them in subdirectories, with the same name as the ones in the root directory. Ex: you can have \"combo/Blue vs Green.png\", \"combo/inGame/Blue vs Green.png\" and \"combo/triangleShaped/Blue vs Green.png\" and they'll all be coppied to the importing file as \"color\", \"ingame_color\" and \"triangleShaped_color\", as long as you have color images inside the root directory.");
}

void FilesDialog::on_ImportingFilesDirectoryHelp_clicked()
{
    QMessageBox::information(this, "Importing Files Directory", "This directory will contain all files that the scoreboard uses to save your selections (maps, modes, teams, colors, info, casters... everything).\n\nDon't change the default structure of this folder, because the scoreboard will look for specific subdirectories to save the correct files.\n\n The subdirectories of this folders are: casters, info, round, set, teamAlpha and teamBravo. The files within these directories will contain your selection of each of these topics, so you'll only need to import these files to OBS / SLOBS");
}

void FilesDialog::on_DefaultDirectoryHelp_clicked()
{
    QMessageBox::information(this, "Default Directory", "This Directory will contain the default images for teams that don't have logos (or when you didn't save it).\n\nBy putting an image here (with any name) it will be used as the default team logo.\n\nYou can also place a bunch of default images in here and the scoreboard will pick one at random each time you enter a team without a saved logo.");
}

void FilesDialog::on_WinPointsDirectoryHelp_clicked()
{
    QMessageBox::information(this, "Win Points Directory", "This Directory will contain the images for win points that will be assigned depending on the winner of each game.\n\nYou can set as many win points you want for alpha an for bravo, and they'll all be assigned on each winner on the importingDirectory/set/winpoints.");
}

void FilesDialog::on_WeaponsDirectoryHelp_clicked()
{
    QMessageBox::information(this, "Weapons Directory", "This Directory will contain the images for weapons that can be assigned to the players of each game.\n\nThe weapon name will be shown as the image file's name (excluding extension [.png / .jpg / .jpeg / .gif])");
}

void FilesDialog::on_AlphaTeamDirectoryFind_clicked()
{
    QString alphaTeamDirectory = QFileDialog::getExistingDirectory(
        this,
        tr("Open the Alpha team directory"),
        QCoreApplication::applicationDirPath() + "/config",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    if (alphaTeamDirectory.length() != 0) {
        ui->AlphaTeamDirectoryEdit->setText(alphaTeamDirectory);
    }
}

void FilesDialog::on_BravoTeamDirectoryFind_clicked()
{
    QString bravoTeamDirectory = QFileDialog::getExistingDirectory(
        this,
        tr("Open the Bravo team directory"),
        QCoreApplication::applicationDirPath() + "/config",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    if (bravoTeamDirectory.length() != 0) {
        ui->BravoTeamDirectoryEdit->setText(bravoTeamDirectory);
    }
}

void FilesDialog::on_LogosDirectoryFind_clicked()
{
    QString logosDirectory = QFileDialog::getExistingDirectory(
        this,
        tr("Open the logos directory"),
        QCoreApplication::applicationDirPath() + "/config",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    if (logosDirectory.length() != 0) {
        ui->LogosDirectoryEdit->setText(logosDirectory);
    }
}


void FilesDialog::on_MapsDirectoryFind_clicked()
{
    QString mapsDirectory = QFileDialog::getExistingDirectory(
        this,
        tr("Open the maps directory"),
        QCoreApplication::applicationDirPath() + "/config",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    if (mapsDirectory.length() != 0) {
        ui->MapsDirectoryEdit->setText(mapsDirectory);
    }
}


void FilesDialog::on_ModeIconsDirectoryFind_clicked()
{
    QString modeIconsDirectory = QFileDialog::getExistingDirectory(
        this,
        tr("Open the mode icons directory"),
        QCoreApplication::applicationDirPath() + "/config",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    if (modeIconsDirectory.length() != 0) {
        ui->ModeIconsDirectoryEdit->setText(modeIconsDirectory);
    }
}

void FilesDialog::on_SplatfestColorsDirectoryFind_clicked()
{
    QString splatfestColorsDirectory = QFileDialog::getExistingDirectory(
        this,
        tr("Open the Splatfest colors directory"),
        QCoreApplication::applicationDirPath() + "/config",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    if (splatfestColorsDirectory.length() != 0) {
        ui->SplatfestColorsDirectoryEdit->setText(splatfestColorsDirectory);
    }
}

void FilesDialog::on_TurfWarColorsDirectoryFind_clicked()
{
    QString turfWarColorsDirectory = QFileDialog::getExistingDirectory(
        this,
        tr("Open the Turf War Colors directory"),
        QCoreApplication::applicationDirPath() + "/config",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    if (turfWarColorsDirectory.length() != 0) {
        ui->TurfWarColorsDirectoryEdit->setText(turfWarColorsDirectory);
    }
}

void FilesDialog::on_RankedColorsDirectoryFind_clicked()
{
    QString rankedColorsDirectory = QFileDialog::getExistingDirectory(
        this,
        tr("Open the ranked colors directory"),
        QCoreApplication::applicationDirPath() + "/config",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    if (rankedColorsDirectory.length() != 0) {
        ui->RankedColorsDirectoryEdit->setText(rankedColorsDirectory);
    }
}

void FilesDialog::on_ImportingFilesDirectoryFind_clicked()
{
    QString importingFilesDirectory = QFileDialog::getExistingDirectory(
        this,
        tr("Open the importing files directory"),
        QCoreApplication::applicationDirPath() + "/config",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    if (importingFilesDirectory.length() != 0) {
        ui->ImportingFilesDirectoryEdit->setText(importingFilesDirectory);
    }
}

void FilesDialog::on_DefaultDirectoryFind_clicked()
{
    QString defaultDirectory = QFileDialog::getExistingDirectory(
        this,
        tr("Open the Default directory"),
        QCoreApplication::applicationDirPath() + "/config",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    if (defaultDirectory.length() != 0) {
        ui->DefaultDirectoryEdit->setText(defaultDirectory);
    }
}

void FilesDialog::on_WinPointsDirectoryFind_clicked()
{
    QString winPointDirectory = QFileDialog::getExistingDirectory(
        this,
        tr("Open the Win Points directory"),
        QCoreApplication::applicationDirPath() + "/config",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    if (winPointDirectory.length() != 0) {
        ui->DefaultDirectoryEdit->setText(winPointDirectory);
    }
}

void FilesDialog::on_WeaponsDirectoryFind_clicked()
{
    QString weaponsDirectory = QFileDialog::getExistingDirectory(
        this,
        tr("Open the weapons directory"),
        QCoreApplication::applicationDirPath() + "/config",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    if (weaponsDirectory.length() != 0) {
        ui->WeaponsDirectoryEdit->setText(weaponsDirectory);
    }
}
