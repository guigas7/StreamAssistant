#include "mywidget.h"

MyWidget::MyWidget(QString sec, QString fts)
{
    this->section = sec;
    this->fileToSave = fts;
}

Qtype MyWidget::convertType(QString typ)
{
    if (QString::compare(typ, "QLineEdit", Qt::CaseInsensitive)) return QLINEEDIT;
    else if (QString::compare(typ, "QLabel", Qt::CaseInsensitive)) return QLABEL;
    else if (QString::compare(typ, "QComboBox", Qt::CaseInsensitive)) return QCOMBOBOX;
    else if (QString::compare(typ, "QSpinBox", Qt::CaseInsensitive)) return QSPINBOX;
    else if (QString::compare(typ, "QCheckBox", Qt::CaseInsensitive)) return QCHECKBOX;
    else if (QString::compare(typ, "QRadioButton", Qt::CaseInsensitive)) return QRADIOBUTTON;
    else if (QString::compare(typ, "QPlainTextEdit", Qt::CaseInsensitive)) return QPLAINTEXTEDIT;
    return QLINEEDIT;
}

QString MyWidget::getSection()
{
    return this->section;
}

QString MyWidget::getFileToSave()
{
    return this->fileToSave;
}

void MyWidget::setSection(QString sec)
{
    this->section = sec;
}

void MyWidget::setFileToSave(QString fts)
{
    this->fileToSave= fts;
}

void MyWidget::writeInFile(QString filename, QString text)
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

QString MyWidget::findImageWithExtension(QString filename)
{
    if (filename.last(1) == "/") {
        return "";
    }
    if (QFile::exists(filename + ".png")) {
        return filename + ".png";
    } else if (QFile::exists(filename + ".jpg")) {
        return filename + ".jpg";
    } else if (QFile::exists(filename)) {
        return filename;
    }
    return "";
}

void MyWidget::copyFile(QString source, QString destiny)
{
    QFile::remove(destiny);
    QFile::copy(source, destiny);
    updateFileTimestamp(destiny);
    return;
}

MyWidget::~MyWidget()
{
    return;
}

QString MyWidget::getRandomImageName(QString defaultDir)
{
    QDir directory(defaultDir + "/" + this->getFileToSave());
    QStringList imagesList = directory.entryList(QDir::Files);
    int max = imagesList.size();
    QRandomGenerator::global()->generate();
    if (max > 0) {
        int picked = QRandomGenerator::global()->bounded(max);
        if (picked > -1) {
            return defaultDir + "/" + getFileToSave() + "/" + imagesList.at(picked);
        }
    }
    return "Nenhuma imagem padrão para escolher";
}

void MyWidget::showImageInLabel(QString imageName, QLabel *logo, Qt::AspectRatioMode aspectRatioMode, bool mirrored)
{
    QImage image;
    bool valid = image.load(imageName);
    if (valid) {
        image = image.scaled(logo->width(), logo->height(), aspectRatioMode, Qt::SmoothTransformation);
        if (mirrored) {
            image.mirror(1, 1);
        }
        logo->setPixmap(QPixmap::fromImage(image));
    } else {
        qDebug() << "couldn't load image " + imageName;
    }
}

void MyWidget::ignoreAspectInLabel(QString imageName, QLabel *logo, bool mirrored)
{
    if (mirrored) {
        showImageInLabel(imageName, logo, Qt::IgnoreAspectRatio, mirrored);
    } else {
        showImageInLabel(imageName, logo, Qt::IgnoreAspectRatio);
    }
}

void MyWidget::keepAspectInLabel(QString imageName, QLabel *logo, bool mirrored)
{
    if (mirrored) {
        showImageInLabel(imageName, logo, Qt::KeepAspectRatio, mirrored);
    } else {
        showImageInLabel(imageName, logo, Qt::KeepAspectRatio);
    }
}

void MyWidget::updateFileTimestamp(QString filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite)) {
        return;
    }
    const quint64 size = file.size();
    file.seek(size);
    file.write( QByteArray(1, '0') );
    file.resize(size);
    return;
}
