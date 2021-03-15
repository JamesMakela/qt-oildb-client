#ifndef ROUTING_H
#define ROUTING_H

#include <QNetworkReply>


enum class NetworkReplyType
{
    oil,
    oilItem,
    label,
    labelItem,
    productType,
    productTypeItem,
    unknown
};


class Routing
{
public:
    Routing();
    static NetworkReplyType getNetworkReplyType(QNetworkReply *reply);
};

#endif // ROUTING_H
