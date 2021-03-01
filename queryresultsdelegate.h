#ifndef QUERYRESULTSDELEGATE_H
#define QUERYRESULTSDELEGATE_H

#include "queryresultmodel.h"
#include <QItemDelegate>
#include <QPainter>


class QueryResultsDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    using QItemDelegate::QItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
    //QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
    //                      const QModelIndex &index) const override;
    //void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    //void setModelData(QWidget *editor, QAbstractItemModel *model,
    //                  const QModelIndex &index) const override;

private slots:
    //void commitAndCloseEditor();
};

#endif // QUERYRESULTSDELEGATE_H
