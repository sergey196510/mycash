#include "downloader.h"

Downloader::Downloader(QObject *pobj) :
    QObject(pobj)
{
    Globals var;

    m_pnam = new QNetworkAccessManager(this);
    if (var.ProxyEnable()) {
        proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName(var.ProxyServer());
        proxy.setPort(var.ProxyPort());
        proxy.setUser(var.ProxyUser());
        proxy.setPassword(var.ProxyPassword());
        m_pnam->setProxy(proxy);
    }

    connect(m_pnam, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotFinished(QNetworkReply*))
            );
    connect(m_pnam, SIGNAL(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)),
            this, SLOT(proxyAuth(QNetworkProxy,QAuthenticator*))
            );
}

Downloader::~Downloader()
{
//    delete m_pnam;
//    delete proxy;
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

void Downloader::proxyAuth(QNetworkProxy,QAuthenticator*)
{
    qDebug() << "proxy auth";
}
