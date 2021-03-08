#include "queryresultmodel.h"
#include <QDebug>


QueryResult::QueryResult()
    : status{OilStatus::good}, oil_id{""}, name{""}
{}

QueryResult::QueryResult(QString oil_id, QString name, OilStatus status)
    : status{status}, oil_id{oil_id}, name{name}
{}

QueryResult::QueryResult(QJsonValue result) {
    // This is intended to receive a JSON result fitting the structure of the
    // JSON:API specification
    //
    // See https://jsonapi.org/ for more information.
    QJsonValue attrs = result["attributes"];

    oil_id = result["_id"].toString();
    name = attrs["metadata"]["name"].toString();
    status = determine_status(attrs["status"].toArray());
}

OilStatus QueryResult::determine_status(const QJsonArray &jsonStatus) {
    if (jsonStatus.size() == 0) {
        return OilStatus::good;
    }

    auto begin{ jsonStatus.begin() };
    auto end{ jsonStatus.end() };

    for (auto p{ begin }; p != end; ++p) {
        QString msg = p->toString();

        if (msg.startsWith(QString("E"))) {
            return OilStatus::error;
        }
    }

    return OilStatus::warning;
}


QueryResultModel::QueryResultModel(QObject *parent)
    : QAbstractTableModel{parent}
{}

QueryResultModel::QueryResultModel(QList<QueryResult> &results, QObject *parent)
    : QAbstractTableModel{parent}, results{results}
{}

QVariant QueryResultModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Status");
        case 1:
            return tr("ID");
        case 2:
            return tr("Name");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

int QueryResultModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : results.size();
}

int QueryResultModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 3;
}

QVariant QueryResultModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= results.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        const auto &result = results.at(index.row());

        switch(index.column()) {
        case 0:
            return QVariant::fromValue(result.status);
        case 1:
            return result.oil_id;
        case 2:
            return result.name;
        }
    }

    return QVariant();
}

bool QueryResultModel::setData(const QModelIndex &index, const QVariant &value,
                               int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        const int row = index.row();
        auto result = results.value(row);

        switch (index.column()) {
        case 0:
            result.status = value.value<OilStatus>();
            break;
        case 1:
            result.oil_id = value.toString();
            break;
        case 2:
            result.name = value.toString();
            break;
        default:
            return false;
        }

        results.replace(row, result);
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

        return true;
    }

    return false;
}

bool QueryResultModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i)
        results.insert(row, QueryResult());

    endInsertRows();
    return true;
}

bool QueryResultModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i)
        results.removeAt(row);

    endRemoveRows();
    return true;
}

bool QueryResultModel::appendQueryResult(const QueryResult &result)
{
    int endOfList = rowCount();

    insertRows(endOfList, 1, QModelIndex());

    QModelIndex idx;

    idx = index(endOfList, 0, QModelIndex());
    setData(idx, QVariant::fromValue(result.status), Qt::EditRole);

    idx = index(endOfList, 1, QModelIndex());
    setData(idx, result.oil_id, Qt::EditRole);

    idx = index(endOfList, 2, QModelIndex());
    setData(idx, result.name, Qt::EditRole);

    return true;
}
