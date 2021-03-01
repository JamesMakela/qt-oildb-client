#include "oildatabasemainwindow.h"
#include "ui_oildatabasemainwindow.h"
#include "queryresultmodel.h"
#include "queryresultsdelegate.h"

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

    model = new QueryResultModel(this);
    ui->queryResultsView->setModel(model);
    ui->queryResultsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->queryResultsView->horizontalHeader()->setStretchLastSection(true);
    ui->queryResultsView->verticalHeader()->hide();
    ui->queryResultsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->queryResultsView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->queryResultsView->setItemDelegate(new QueryResultsDelegate);

    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(replyFinished(QNetworkReply*)));
}

OilDatabaseMainWindow::~OilDatabaseMainWindow()
{
    delete ui;
    delete manager;
    delete model;
}

void OilDatabaseMainWindow::on_requestButton_clicked()
{
    model->removeRows(0, model->rowCount(), QModelIndex());

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
        QJsonParseError error;
        QJsonDocument responseJson = QJsonDocument::fromJson(reply->readAll(),
                                                             &error);

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

            for (auto idx{0}; idx < data.size(); ++idx) {
                model->appendQueryResult(QueryResult{data[idx]});
            }
        }
    }

    reply->deleteLater();
    std::cout << "reply finished.";
}





