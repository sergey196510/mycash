#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

class Downloader : public QObject
{
    Q_OBJECT

private:
    QNetworkAccessManager *m_pnam;

public:
    Downloader(QObject *pobj = 0);
    ~Downloader();
    void download(const QUrl &);

signals:
    void downloadProgress(qint64, qint64);
    void done(const QUrl &, const QByteArray &);
    void error();

private slots:
    void slotFinished(QNetworkReply *);
};

#endif // DOWNLOADER_H
