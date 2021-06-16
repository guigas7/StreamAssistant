#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QCompleter>
#include "filesdialog.h"
#include "mywidget.h"
#include "lineeditwidget.h"

#define widAmount 69

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Team Alpha Section
    void on_Team1NameEdit_editingFinished();
    // Round Section
    void on_SetDirectoriesButton_clicked();
    void on_Team1Score_textChanged(const QString &arg1);
    void on_Team2Score_textChanged(const QString &arg1);
    void on_ResetScoresButton_clicked();
    // Team Beta Section
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

    void on_MapComboRound1_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    FilesDialog *dirConfigWindow;
    void configDirectories_init();
    void autoComplete_init();
    void infoSection_init();
    void TeamsSection_init();
    void RoundSection_init();
    void CastersSection_init();
    void SetSection_init();
    QMap<QString, MyWidget*> widgets_init();
    QStringList getListOf(QString dirIndex);
    void writeInFile(QString filename, QString text);
    void serialize(const char *, void *, int);
    void deserialize(const char *, void *, int);
    QCompleter *Team1Completer;
    QCompleter *Team2Completer;
    QCompleter *MapCompleter;
    QMap<QString, QString> directoryFor;
    QMap<QString, MyWidget *> widgetFor;
    QString directories[dirAmount];
};

#endif // MAINWINDOW_H
