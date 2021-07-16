#ifndef COMBOBOXWIDGET_H
#define COMBOBOXWIDGET_H

#include "mywidget.h"
#include <QComboBox>
#include <QDir>

class ComboBoxWidget : public MyWidget
{
public:
    explicit ComboBoxWidget(QString sec, QString fts);
    void saveInFile(QString importingDir, QWidget *wid);
    void saveContentInFile(QString importingDir, QWidget *wid);
    void saveInFile(QString importingDir, QWidget *wid, QString mode, QString local);
    void init(QString importingDir, QWidget *wid, QString logosDir);
    void init(QString importingDir, QWidget *wid, QStringList list);
    void saveColors(QString importingDir, QWidget *wid, QString local, QLabel *alphaLabel, QLabel *betaLabel, QLabel *comboLabel);
    void saveColor(QString colorDir, QString color,QString dirToSave, QString fileToSave, QLabel *logo);
    QStringList getListOf(QString dirString);
};

#endif // COMBOBOXWIDGET_H
