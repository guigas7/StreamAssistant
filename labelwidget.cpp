#include "labelwidget.h"

LabelWidget::LabelWidget(QString sec, QString fts): MyWidget(sec, fts) {}

void LabelWidget::init(QString importingDir, QWidget *wid, QString imageName, QString defaultDir)
{
    QLabel *logo = (QLabel *)wid;
    QString copyName = importingDir + this->getSection() + this->getFileToSave();

    imageName = this->findImageWithExtension(imageName);
    if (imageName.isEmpty()) {
        imageName = this->getRandomImageName(defaultDir);
    }
    // Show team logo on screen
    QImage image;
    bool valid = image.load(imageName);
    if (valid) {
        image = image.scaled(logo->width(), logo->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        logo->setPixmap(QPixmap::fromImage(image));
        // Copy team logo to file
        this->copyFile(imageName, copyName);
    } else {
        qDebug() << "couldn't load image " + imageName;
    }
}
