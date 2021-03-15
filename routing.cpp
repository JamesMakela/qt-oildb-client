#include "routing.h"

Routing::Routing()
{}


NetworkReplyType Routing::getNetworkReplyType(QNetworkReply* reply)
{
    QStringList pathParts = (reply->request().url().path()
                             .split('/', QString::SkipEmptyParts));

    if (pathParts.last() == "oils") {
        return NetworkReplyType::oil;
    }
    else if (pathParts.last() == "labels") {
        return NetworkReplyType::label;
    }
    else if (pathParts.last() == "product-types") {
        return NetworkReplyType::productType;
    }
    else {
        return NetworkReplyType::unknown;
    }
    // we will get more sophisticated later
}
