#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QCompleter>
#include "filesdialog.h"
#include "mywidget.h"
#include "lineeditwidget.h"
#include "comboboxwidget.h"
#include "checkboxwidget.h"
#include "spinboxwidget.h"
#include "radiobuttonwidget.h"
#include "labelwidget.h"
#include "plaintextwidget.h"
#include "clickablelabel.h"

#define widAmount 67



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    typedef void (MainWindow::*clickedFunction)(void);

private slots:
    // Team Alpha Section
    void on_Team1NameEdit_editingFinished();
    // Round Section
    void on_Team1Score_textChanged(const QString &arg1);
    void on_Team2Score_textChanged(const QString &arg1);
    void on_ResetScoresButton_clicked();
    // Team Bravo Section
    // Info Section
    void on_TitleEdit_editingFinished();
    void on_SubtitleEdit_editingFinished();
    void on_TwitterEdit_editingFinished();
    void on_MessageEdit_textChanged();
    // Casters Section
    void on_Caster1NameEdit_editingFinished();
    void on_Caster2NameEdit_editingFinished();
    void on_Caster3NameEdit_editingFinished();
    void on_Caster1TwitterEdit_editingFinished();
    void on_Caster2TwitterEdit_editingFinished();
    void on_Caster3TwitterEdit_editingFinished();
    void on_RoundEdit_editingFinished();
    void on_Team2NameEdit_editingFinished();
    void on_LogoCombo_activated(int index);
    void on_ColorGroupCombo_activated(int index);
    void on_ModeComboGame1_activated(int index);
    void on_ModeComboGame2_activated(int index);
    void on_ModeComboGame3_activated(int index);
    void on_ModeComboGame4_activated(int index);
    void on_ModeComboGame5_activated(int index);
    void on_ModeComboGame6_activated(int index);
    void on_ModeComboGame7_activated(int index);
    void on_ModeComboGame8_activated(int index);
    void on_ModeComboGame9_activated(int index);
    void on_MapComboGame1_editingFinished();
    void on_MapComboGame2_editingFinished();
    void on_MapComboGame3_editingFinished();
    void on_MapComboGame4_editingFinished();
    void on_MapComboGame5_editingFinished();
    void on_MapComboGame6_editingFinished();
    void on_MapComboGame7_editingFinished();
    void on_MapComboGame8_editingFinished();
    void on_MapComboGame9_editingFinished();
    void on_RadioGame1_clicked();
    void on_RadioGame2_clicked();
    void on_RadioGame3_clicked();
    void on_RadioGame4_clicked();
    void on_RadioGame5_clicked();
    void on_RadioGame6_clicked();
    void on_RadioGame7_clicked();
    void on_RadioGame8_clicked();
    void on_RadioGame9_clicked();
    void handleWinner(QCheckBox *WinGame, QCheckBox *OpponentWinGame, QSpinBox *score, QSpinBox *opponentScore, QString teamDir, QRadioButton *round, QRadioButton *nextGame, clickedFunction nextGameClicked);
    void on_AlphaWinGame1_clicked();
    void on_AlphaWinGame2_clicked();
    void on_AlphaWinGame3_clicked();
    void on_AlphaWinGame4_clicked();
    void on_AlphaWinGame5_clicked();
    void on_AlphaWinGame6_clicked();
    void on_AlphaWinGame7_clicked();
    void on_AlphaWinGame8_clicked();
    void on_AlphaWinGame9_clicked();
    void on_BravoWinGame1_clicked();
    void on_BravoWinGame2_clicked();
    void on_BravoWinGame3_clicked();
    void on_BravoWinGame4_clicked();
    void on_BravoWinGame5_clicked();
    void on_BravoWinGame6_clicked();
    void on_BravoWinGame7_clicked();
    void on_BravoWinGame8_clicked();
    void on_BravoWinGame9_clicked();
    void saveRound(int index);
    void on_actionSet_Directories_triggered();
    void on_actionUpdate_Options_triggered();
    void on_UpdateScoreCheckbox_stateChanged(int arg1);
    void on_UpdateRoundCheckbox_stateChanged(int arg1);
    void on_ColorsCombo_activated(int index);
    void on_SwapColorsButton_clicked();
    void on_SwapTeamsButton_clicked();
    void on_FilesDialogAccepted();
    void updateAlphaWins();
    void updateBravoWins();
    void updateWeapon();
    void updatePlayer();
    void labelClicked();
    void roundLabelHovered();
    void resetFields();

private:
    Ui::MainWindow *ui;
    void configDirectories_init();
    void infoSection_init();
    void teamsSection_init();
    void roundSection_init();
    void castersSection_init();
    void setSection_init();
    void colors_init();
    void weaponsSection_init();
    QMap<QString, MyWidget*> playersSection_init();
    QMap<QString, MyWidget*> widgets_init();
    QMap<QString, QLabel*> weapons_init();
    QStringList getListOf(QString dirIndex, bool includeDirs = false);
    void writeInFile(QString, QString);
    void serialize(const char *, void *, int);
    void deserialize(const char *, void *, int);
    void scoreAutoUpdate(QSpinBox *teamScore, QCheckBox *teamCheckBox);
    void colorAutoUpdate();
    void roundAutoUpdate();
    void clearFields();
    QStringList clearOptionalFiles(QStringList, QString);
    QStringList saveOptionalFiles(QString, QString);
    void copyFile(QString, QString);
    void updateFileTimestamp(QString);
    QString getColorsLocal();
    void showSections_init();
    void updatePlayersFromAlpha();
    void updatePlayersFromBravo();
    void fillFirstPlayers(QString team);
    QCompleter *Team1Completer;
    QCompleter *Team2Completer;
    QCompleter *MapCompleter;
    QCompleter *WeaponsCompleter;
    QMap<QString, QString> directoryFor;
    QMap<QString, MyWidget *> widgetFor;
    QMap<QString, QLabel *> labelFor;
    QMap<QString, MyWidget *> playerWidgetFor;
    QString directories[dirAmount];
    bool mirrored;
    QStringList teamAlphaOptionalFiles;
    QStringList teamBravoOptionalFiles;
};

#endif // MAINWINDOW_H
