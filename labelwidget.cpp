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
    this->copyFile(imageName, copyName);
    this->keepAspectInLabel(imageName, logo, copyName);
}
