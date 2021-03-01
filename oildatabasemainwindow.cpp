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
    ui->queryResults->setColumnWidth(0, 64);
    ui->queryResults->setColumnWidth(2, 300);

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

            QTableWidget *queryTable = ui->queryResults;
            qDebug() << "Initial table row count: " << queryTable->rowCount();

            queryTable->setRowCount(queryTable->rowCount() + data.size());

            for (int x{ 0 }, y{ 9 }; x < 10; ++x, --y)
                    std::cout << x << ' ' << y << '\n';

            for (auto idx{0}; idx < data.size(); ++idx) {
                QueryResult item{data[idx]};

                QLabel *myLabel = new QLabel();
                myLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                myLabel->setPixmap(get_status_img(item.status));

                ui->queryResults->setCellWidget(idx, 0, myLabel);

                QTableWidgetItem *newItem = new QTableWidgetItem(item.oil_id);
                ui->queryResults->setItem(idx, 1, newItem);

                newItem = new QTableWidgetItem(item.name);
                ui->queryResults->setItem(idx, 2, newItem);

                // Todo: If you want a table that uses your own data model you
                // should use QTableView instead.  Here we use a model, and
                // the TableView should automatically be updated.
                model->appendQueryResult(item);
            }
        }
    }

    reply->deleteLater();
    std::cout << "reply finished.";
}

QPixmap OilDatabaseMainWindow::get_status_img(OilStatus status) {
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

    img.setDevicePixelRatio(2.5);

    return img;
}






















