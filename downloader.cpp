#include "downloader.h"

Downloader::Downloader(QObject *pobj) :
    QObject(pobj)
{
    m_pnam = new QNetworkAccessManager(this);
    connect(m_pnam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotFinished(QNetworkReply*))
            );
}

Downloader::~Downloader()
{
//    delete m_pnam;
}

void Downloader::slotFinished(QNetworkReply *pnr)
{
    if (pnr->error() != QNetworkReply::NoError) {
        emit error();
    }
    else {
        QUrl url = pnr->url();
        QByteArray arr = pnr->readAll();
        emit done(url, arr);
    }
    pnr->deleteLater();
}

void Downloader::download(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *pnr = m_pnam->get(request);
    connect(pnr, SIGNAL(downloadProgress(qint64, qint64)),
            this, SIGNAL(downloadProgress(qint64,qint64))
            );
}
