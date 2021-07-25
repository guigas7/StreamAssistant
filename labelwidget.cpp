#include "labelwidget.h"

LabelWidget::LabelWidget(QString sec, QString fts): MyWidget(sec, fts) {}

void LabelWidget::init(QString importingDir, QWidget *wid, QString section, QString imageName, QString defaultDir, QString otherTeamSection)
{
    QLabel *logo = (QLabel *)wid;
    QString image = this->findImageWithExtension(section + imageName);
    qDebug() << "image is " + image;
    if (image.isEmpty()) {
        qDebug() << "was empty";
        if (!otherTeamSection.isEmpty()) {
            qDebug() << "Looking in the other team";
            image = this->findImageWithExtension(otherTeamSection + imageName);
        }
        if (image.isEmpty()) {
            qDebug() << "was empty";
            image = this->getRandomImageName(defaultDir);
            qDebug() << "got random";
        }
    }
    qDebug() << image;
    // Copy image to importing file
    this->copyFile(image, importingDir + this->getSection() + this->getFileToSave());
    // Show team logo on screen
    this->keepAspectInLabel(image, logo);
}
