#ifndef COMBOBOXWIDGET_H
#define COMBOBOXWIDGET_H

#define _POSIX_SOURCE
#include "mywidget.h"
#include <QComboBox>
#include <QDir>
#include <QFileDevice>
#include <fcntl.h>
#include <sys/types.h>
#include <utime.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

class ComboBoxWidget : public MyWidget
{
public:
    explicit ComboBoxWidget(QString sec, QString fts);
    void saveInFile(QString importingDir, QWidget *wid);
    void saveContentInFile(QString importingDir, QWidget *wid);
    void saveInFile(QString importingDir, QWidget *wid, QString mode, QString local);
    void init(QString importingDir, QWidget *wid, QString logosDir);
    void init(QString importingDir, QWidget *wid, QStringList list);
    void init(QString importingDir, QWidget *wid, QStringList list, QString filename);
    void initPlayers(QString importingDir, QWidget *wid);
    bool handleColorFile(QString colorFile, QString savingFile, QLabel *logo, bool &showedInAssistant, bool mirrored);
    void saveColors(QString importingDir, QWidget *wid, QString local, QLabel *alphaLabel, QLabel *bravoLabel, QLabel *comboLabel, bool mirrored = false);
    bool saveColor(QString colorDir, QString color,QString dirToSave, QString fileToSave, QLabel *logo, bool mirrored = false);
    QStringList getListOf(QString dirString);
};

#endif // COMBOBOXWIDGET_H
