#include "imageservice.h"

ImageService::ImageService(QObject *parent) : QObject(parent)  // Wywołanie konstruktora QObject
{
    manager = new QNetworkAccessManager(this);  // Ustawienie parenta dla managera
    connect(manager, &QNetworkAccessManager::finished, this, &ImageService::replyFinished);
}


QPixmap ImageService::Pixmap()
{
    return pixmap;
}

ImageService::~ImageService()
{
    delete manager;
}

void ImageService::downloadImage()
{
    manager->get(QNetworkRequest(QUrl("https://picsum.photos/1920/1080?blur=5")));
}

void ImageService::replyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray imageData = reply->readAll();
        QImage img;
        img.loadFromData(imageData);

        if (!img.isNull()) {
            QPixmap pixmap = QPixmap::fromImage(img);
            emit imageDownloaded(pixmap);  // Emitujemy sygnał z obrazem
        }
    } else {
        qDebug() << "Error downloading image:" << reply->errorString();
    }
    reply->deleteLater();
}
