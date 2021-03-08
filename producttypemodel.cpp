#include "producttypemodel.h"


ProductType::ProductType()
    : name{""}
{}

ProductType::ProductType(QString name)
    : name{name}
{}

ProductType::ProductType(QJsonValue value) {
    // This is intended to receive a JSON result fitting a JSON structure.
    //
    // Note: in the case of product types, the server is not really complying
    //       with the JSON:API specification, so we just get the incoming value
    name = value.toString();
}


ProductTypeModel::ProductTypeModel(QObject *parent)
    : QAbstractListModel{parent}
{}

ProductTypeModel::ProductTypeModel(QList<ProductType> &types, QObject *parent)
    : QAbstractListModel{parent}, types{types}
{}

int ProductTypeModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : types.size();
}

int ProductTypeModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 3;
}

QVariant ProductTypeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= types.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        const auto &result = types.at(index.row());

        switch(index.column()) {
        case 0:
            return result.name;
        }
    }

    return QVariant();
}

bool ProductTypeModel::setData(const QModelIndex &index, const QVariant &value,
                               int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        const int row = index.row();
        auto result = types.value(row);

        switch (index.column()) {
        case 0:
            result.name = value.toString();
            break;
        default:
            return false;
        }

        types.replace(row, result);
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

        return true;
    }

    return false;
}

bool ProductTypeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i)
        types.insert(row, ProductType());

    endInsertRows();
    return true;
}

bool ProductTypeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);

    for (int i = 0; i < count; ++i)
        types.removeAt(row);

    endRemoveRows();
    return true;
}

bool ProductTypeModel::appendProductType(const ProductType &type)
{
    int endOfList = rowCount();

    insertRows(endOfList, 1, QModelIndex());

    QModelIndex idx;

    idx = index(endOfList, 0, QModelIndex());
    setData(idx, type.name, Qt::EditRole);

    return true;
}
