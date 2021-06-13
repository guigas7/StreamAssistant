#include "widget.h"

Widget::Widget(QString sec, QString fts, QWidget *wid)
{
    this->section = sec;
    this->fileToSave = fts;
    this->type = this->convertType(wid->staticMetaObject.className());
    this->widget = wid;
}

Qtype Widget::convertType(QString typ)
{
    if (QString::compare(typ, "QLINEEDIT", Qt::CaseInsensitive)) return QLINEEDIT;
    else if (QString::compare(typ, "QLabel", Qt::CaseInsensitive)) return QLABEL;
    else if (QString::compare(typ, "QComboBox", Qt::CaseInsensitive)) return QCOMBOBOX;
    else if (QString::compare(typ, "QSpinBox", Qt::CaseInsensitive)) return QSPINBOX;
    else if (QString::compare(typ, "QCheckBox", Qt::CaseInsensitive)) return QCHECKBOX;
    else if (QString::compare(typ, "QRadioButton", Qt::CaseInsensitive)) return QRADIOBUTTON;
    return QLINEEDIT;
}

QString Widget::getSection()
{
    return this->section;
}

QString Widget::getFileToSave()
{
    return this->fileToSave;
}

Qtype Widget::getType()
{
    return this->type;
}

void Widget::setSection(QString sec)
{
    this->section = sec;
}

void Widget::setFileToSave(QString fts)
{
    this->fileToSave= fts;
}

void Widget::setType(Qtype typ)
{
    this->type = typ;
}

void Widget::init()
{
    switch (this->type) {
    case QLINEEDIT:

        break;
    case QLABEL:

        break;
    case QCOMBOBOX:

        break;
    case QSPINBOX:

        break;
    case QCHECKBOX:

        break;
    case QRADIOBUTTON:

        break;
    default:

        break;
    }
}
