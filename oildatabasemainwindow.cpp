#include "oildatabasemainwindow.h"
#include "ui_oildatabasemainwindow.h"

#include <iostream>

#include <QLabel>
#include <QtNetwork/QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>

#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QTableWidget>


OilDatabaseMainWindow::OilDatabaseMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OilDatabaseMainWindow)
{
    ui->setupUi(this);
    ui->queryResults->setColumnWidth(0, 64);
    ui->queryResults->setColumnWidth(2, 300);

    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(replyFinished(QNetworkReply*)));
}

OilDatabaseMainWindow::~OilDatabaseMainWindow()
{
    delete ui;
    delete manager;
}

void OilDatabaseMainWindow::on_requestButton_clicked()
{
    QUrl url = QUrl("https://adios-stage.orr.noaa.gov/api/oils");

    QUrlQuery query = QUrlQuery();
    query.addQueryItem(QString("limit"), QString("100"));

    url.setQuery(query);

    manager->get(QNetworkRequest(url));
    std::cout << "get the request.";
}

void OilDatabaseMainWindow::replyFinished(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "Error in network reply: " << reply->errorString();
        return;
    }
    else {
        QByteArray responseBody = reply->readAll();

        QJsonParseError error;
        QJsonDocument responseJson = QJsonDocument::fromJson(responseBody, &error);

        if (responseJson.isNull()) {
            qDebug() << "Null JSON response: " << error.errorString();
            return;
        }
        else if (responseJson.isEmpty()) {
            qDebug() << "Empty JSON response.";
            return;
        }
        else if (responseJson.isArray()) {
            qDebug() << "JSON response is a list.  We are not expecting a list.";
            return;
        }
        else if (responseJson.isObject()) {
            QJsonArray data = responseJson.object()[QString("data")].toArray();

            // Todo: If you want a table that uses your own data model you
            // should use QTableView rather than this class
            QTableWidget *queryTable = ui->queryResults;
            qDebug() << "Initial table row count: " << queryTable->rowCount();

            queryTable->setRowCount(queryTable->rowCount() + data.size());

            for (int x{ 0 }, y{ 9 }; x < 10; ++x, --y)
                    std::cout << x << ' ' << y << '\n';

            for (auto idx{0}; idx < data.size(); ++idx) {
                QJsonValue item = data[idx];
                QString id = item["_id"].toString();
                QJsonValue attrs = item["attributes"];

                QString name = attrs["metadata"]["name"].toString();
                QJsonArray status = attrs["status"].toArray();

                qDebug() << "item " << idx << ": id = " << id << ", item name = " << name;

                QPixmap img;

                switch (determine_status(status)) {
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

                img.setDevicePixelRatio(2.5);

                QLabel *myLabel = new QLabel();
                myLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                myLabel->setPixmap(img);

                ui->queryResults->setCellWidget(idx, 0, myLabel);

                QTableWidgetItem *newItem = new QTableWidgetItem(id);
                ui->queryResults->setItem(idx, 1, newItem);

                newItem = new QTableWidgetItem(name);
                ui->queryResults->setItem(idx, 2, newItem);
            }
        }
    }

    reply->deleteLater();
    std::cout << "reply finished.";
}


OilStatus OilDatabaseMainWindow::determine_status(const QJsonArray &jsonStatus) {
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




















