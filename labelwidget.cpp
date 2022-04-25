#include "labelwidget.h"

LabelWidget::LabelWidget(QString sec, QString fts): MyWidget(sec, fts) {}

void LabelWidget::init(QString importingDir, QWidget *wid, QString section, QString imageName, QString defaultDir, QString otherTeamSection)
{
    QLabel *logo = (QLabel *)wid;
    QString image = this->findImageWithExtension(section + imageName);
    QString destinyDir = importingDir + this->getSection();
    QString originDir = section;
    if (image.isEmpty()) {
        QDir teamDir(section + imageName);
        if (teamDir.exists()) { // found directory
            originDir = section + imageName + "/";
            image = this->findImageWithExtension(originDir + "logo");
            if (image.isEmpty()) {
                image = this->getRandomImageName(defaultDir + "/" + getFileToSave());
            }
        } else {
            if (!otherTeamSection.isEmpty()) {
                image = this->findImageWithExtension(otherTeamSection + imageName);
            }
            if (image.isEmpty()) {
                image = this->getRandomImageName(defaultDir + "/" + getFileToSave());
            }
        }
    }
    // Copy image to importing file
    this->copyFile(image, destinyDir + this->getFileToSave());
    // Show team logo on screen
    this->keepAspectInLabel(image, logo);
}
