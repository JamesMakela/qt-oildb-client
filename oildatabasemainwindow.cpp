#include "oildatabasemainwindow.h"
#include "ui_oildatabasemainwindow.h"

#include <iostream>

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
    ui->queryResults->setColumnWidth(1, 300);

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
    query.addQueryItem(QString("limit"), QString("20"));

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
        ui->jsonResponse->setPlainText(responseBody);  // just to see what we got

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
                qDebug() << "item id: " << item["_id"]
                         << ", item name: " << item["attributes"]["metadata"]["name"];

                QString id = item["_id"].toString();
                QString name = item["attributes"]["metadata"]["name"].toString();

                QTableWidgetItem *newItem = new QTableWidgetItem(id);
                ui->queryResults->setItem(idx, 0, newItem);

                newItem = new QTableWidgetItem(name);
                ui->queryResults->setItem(idx, 1, newItem);
            }
        }
    }

    reply->deleteLater();
    std::cout << "reply finished.";
}
