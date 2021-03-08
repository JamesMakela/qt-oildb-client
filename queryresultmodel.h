#ifndef QUERYRESULTMODEL_H
#define QUERYRESULTMODEL_H

#include <QAbstractTableModel>
#include <QJsonValue>
#include <QJsonArray>


enum class OilStatus
{
    good,
    warning,
    error
};
Q_DECLARE_METATYPE(OilStatus)


class QueryResult {
private:
    OilStatus determine_status(const QJsonArray &jsonStatus);

public:
    QueryResult();
    QueryResult(QString oil_id, QString name, OilStatus status);
    QueryResult(QJsonValue result);

    OilStatus status;
    QString oil_id;
    QString name;


    bool operator==(const QueryResult &other) const {
        return (name == other.name &&
                oil_id == other.oil_id &&
                status == other.status);
    }
};

inline QDataStream &operator<<(QDataStream &stream, const QueryResult &queryResult)
{
    return stream << queryResult.oil_id << queryResult.name;
}


class QueryResultModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit QueryResultModel(QObject *parent = nullptr);
    QueryResultModel(QList<QueryResult> &results, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    // Add/remove data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    bool appendQueryResult(const QueryResult &result);

private:
    QList<QueryResult> results;
};

#endif // QUERYRESULTMODEL_H
