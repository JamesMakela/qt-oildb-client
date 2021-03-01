#include "queryresultsdelegate.h"
#include <QDebug>


void QueryResultsDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{

    if (index.column() == 0) {
        OilStatus status = index.data().value<OilStatus>();

        QPixmap img;

        switch (status) {
        case OilStatus::good:
            img = QPixmap( ":/image/Good.png" );
            break;
        case OilStatus::warning:
            img = QPixmap( ":/image/Warning.png" );
            break;
        default:
            img = QPixmap( ":/image/Error.png" );
            break;
        }

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);

        QRect source(0, 0, 64, 64);
        QRect dest(option.rect.center().x() - option.rect.height() / 2,
                   option.rect.center().y() - option.rect.height() / 2,
                   option.rect.height(),
                   option.rect.height());
        dest -= QMargins(2, 2, 2, 2);
        painter->drawPixmap(dest, img, source);

        painter->restore();
    } else {
        QItemDelegate::paint(painter, option, index);
    }
}


QSize QueryResultsDelegate::sizeHint(const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    if (index.column() == 0) {
        return QSize(32, 32);
    }
    return QItemDelegate::sizeHint(option, index);
}
