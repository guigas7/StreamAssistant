#ifndef FILESDIALOG_H
#define FILESDIALOG_H

#include <QDialog>
#include <QFile>
#include <QLineEdit>
#include <QMap>

#define dirAmount 11

namespace Ui {
class FilesDialog;
}

class FilesDialog : public QDialog
{
    Q_OBJECT

signals:
    void allDone();

public:
    explicit FilesDialog(QWidget *parent = nullptr, QMap<QString, QString> *directoryFor = nullptr, QString *directories = nullptr);
    ~FilesDialog();

private slots:
    void on_AlphaTeamDirectoryHelp_clicked();
    void on_BetaTeamDirectoryHelp_clicked();
    void on_RegionsDirectoryHelp_clicked();
    void on_LogosDirectoryHelp_clicked();
    void on_MapsDirectoryHelp_clicked();
    void on_ModeIconsDirectoryHelp_clicked();
    void on_SplatfestColorsDirectoryHelp_clicked();
    void on_TurfWarColorsDirectoryHelp_clicked();
    void on_RankedColorsDirectoryHelp_clicked();
    void on_ImportingFilesDirectoryHelp_clicked();
    void on_DefaultDirectoryHelp_clicked();
    void on_AlphaTeamDirectoryFind_clicked();
    void on_BetaTeamDirectoryFind_clicked();
    void on_RegionsDirectoryFind_clicked();
    void on_LogosDirectoryFind_clicked();
    void on_MapsDirectoryFind_clicked();
    void on_ModeIconsDirectoryFind_clicked();
    void on_SplatfestColorsDirectoryFind_clicked();
    void on_TurfWarColorsDirectoryFind_clicked();
    void on_RankedColorsDirectoryFind_clicked();
    void on_ImportingFilesDirectoryFind_clicked();
    void on_DefaultDirectoryFind_clicked();
    void on_DirectoriesDialogButtonBox_accepted();

private:
    Ui::FilesDialog *ui;
    QMap<QString, QString> *directoryFor;
    QLineEdit *fields[dirAmount];
    QString directories[dirAmount];
    int saveConfigDirectory(QString, QLineEdit*);
    void directories_init(QString *);
    void makeDefaultDir(QLineEdit *field, QString configFile, QString configDir);
};

#endif // FILESDIALOG_H
