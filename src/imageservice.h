#ifndef IMAGESERVICE_H
#define IMAGESERVICE_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QPixmap>

class ImageService : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager *manager;
    QPixmap pixmap;

public:
    ImageService(QObject *parent);
    QPixmap Pixmap();
    ~ImageService();
    void downloadImage();
signals:
    void imageDownloaded(const QPixmap &pixmap);
private slots:
    void replyFinished(QNetworkReply *reply);
};

#endif // IMAGESERVICE_H
