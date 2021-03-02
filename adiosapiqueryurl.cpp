#include "adiosapiqueryurl.h"


AdiosApiQueryUrl::AdiosApiQueryUrl()
    : m_url{""}, m_limit{20}, m_page{0}, m_sort{""}, m_dir{"asc"},
      m_queryText{""}, m_queryType{""}, m_queryLabels{""},
      m_minApi{0.0}, m_maxApi{100.0}
{}

AdiosApiQueryUrl::AdiosApiQueryUrl(const QString &url)
    : m_url{url}, m_limit{20}, m_page{0}, m_sort{""}, m_dir{"asc"},
      m_queryText{""}, m_queryType{""}, m_queryLabels{""},
      m_minApi{0.0}, m_maxApi{100.0}
{}

QUrl AdiosApiQueryUrl::url()
{
    QUrlQuery query = QUrlQuery{};

    if (m_limit > 0) {
        query.addQueryItem(QString("limit"), QString::number(m_limit));
    }

    if (m_page) {
        query.addQueryItem(QString("page"), QString::number(m_page));
    }

    if (m_sort.count() > 0) {
        query.addQueryItem(QString("sort"), m_sort);
        query.addQueryItem(QString("dir"), m_dir);
    }

    if (m_queryText.count() > 0) {
        query.addQueryItem(QString("q"), m_queryText);
    }

    if (m_minApi > 0 || m_maxApi < 100) {
        query.addQueryItem(QString("qApi"),
                           QObject::tr("%1,%2").arg(m_minApi).arg(m_maxApi));
    }

    if (m_queryType.count() > 0) {
        query.addQueryItem(QString("qType"), m_queryType);
    }

    if (m_queryLabels.count() > 0) {
        query.addQueryItem(QString("qLabels"), m_queryLabels);
    }

    QUrl url{m_url};
    url.setQuery(query);

    return url;
}

// Todo: Maybe replace the direction arg with an enum?
void AdiosApiQueryUrl::setDir(const QString &direction) {
    if (QString::compare(direction, "asc", Qt::CaseInsensitive) ||
            QString::compare(direction, "ascending", Qt::CaseInsensitive))
    {
        m_dir = QString("asc");
    }
    else if (QString::compare(direction, "desc", Qt::CaseInsensitive) ||
             QString::compare(direction, "descending", Qt::CaseInsensitive))
    {
        m_dir = QString("desc");
    }
    else {
        qDebug() << "Tried to set invalid sort direction: " << direction;
    }
}
