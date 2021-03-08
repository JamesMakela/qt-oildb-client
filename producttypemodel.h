#ifndef PRODUCTTYPEMODEL_H
#define PRODUCTTYPEMODEL_H

#include <QAbstractListModel>
#include <QJsonValue>
#include <QJsonArray>


class ProductType {
public:
    ProductType();
    ProductType(QString name);
    ProductType(QJsonValue value);


    QString name;

    bool operator==(const ProductType &other) const {
        return (name == other.name);
    }
};

inline QDataStream &operator<<(QDataStream &stream, const ProductType &type)
{
    return stream << type.name;
}


class ProductTypeModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ProductTypeModel(QObject *parent = nullptr);
    ProductTypeModel(QList<ProductType> &types, QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    // Add/remove data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    bool appendProductType(const ProductType &type);

private:
    QList<ProductType> types;
};

#endif // PRODUCTTYPEMODEL_H
