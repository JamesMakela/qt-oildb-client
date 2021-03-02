#ifndef ADIOSAPIQUERYURL_H
#define ADIOSAPIQUERYURL_H

#include <QObject>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>

class AdiosApiQueryUrl
{
public:
    AdiosApiQueryUrl();
    AdiosApiQueryUrl(const QString &url);

    void setUrl(const QString &url) {m_url = url;}
    QUrl url();

    void setLimit(int limit) {m_limit = limit;}
    void setPage(int page) {m_page = page;}
    void setSort(const QString &text) {m_sort = text;}
    void setDir(const QString &text);

    void setQueryText(const QString &text) {m_queryText = text;}
    void setQueryType(const QString &text) {m_queryType = text;}
    void setQueryLabels(const QString &text) {m_queryLabels = text;}
    void setMinApi(double minApi) {m_minApi = minApi;}
    void setMaxApi(double maxApi) {m_maxApi = maxApi;}
private:
    QString m_url;

    int m_limit;  // Basically the page size
    int m_page;  // The page number
    QString m_sort;  // what field to sort the query on
    QString m_dir;  // the sort direction {'asc', 'desc'}

    QString m_queryText;
    QString m_queryType;
    QString m_queryLabels;

    double m_minApi;
    double m_maxApi;
};

#endif // ADIOSAPIQUERYURL_H
