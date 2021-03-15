#include "oildatabasemainwindow.h"
#include "ui_oildatabasemainwindow.h"
#include "networkerrordialog.h"
#include "queryresultmodel.h"
#include "producttypemodel.h"
#include "queryresultsdelegate.h"
#include "routing.h"

#include <iostream>

#include <QLabel>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkConfiguration>
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

    queryModel = new QueryResultModel(this);
    ui->queryResultsView->setModel(queryModel);
    ui->queryResultsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->queryResultsView->horizontalHeader()->setStretchLastSection(true);
    ui->queryResultsView->verticalHeader()->hide();
    ui->queryResultsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->queryResultsView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->queryResultsView->setItemDelegate(new QueryResultsDelegate);

    productTypeModel = new ProductTypeModel(this);
    ui->oilTypeListView->setModel(productTypeModel);

    queryUrl = new AdiosApiQueryUrl("https://adios-stage.orr.noaa.gov/api/oils/");

    manager = new QNetworkAccessManager(this);
    QNetworkConfiguration config = manager->configuration();
    config.setConnectTimeout(5000);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(replyFinished(QNetworkReply*)));

    setupListControls();
}


OilDatabaseMainWindow::~OilDatabaseMainWindow()
{
    delete ui;
    delete manager;
    delete queryModel;
}


void OilDatabaseMainWindow::setupListControls() {
    manager->get(QNetworkRequest(QUrl("https://adios-stage.orr.noaa.gov/api/product-types/")));
    manager->get(QNetworkRequest(QUrl("https://adios-stage.orr.noaa.gov/api/labels/")));
}


void OilDatabaseMainWindow::on_requestButton_clicked()
{
    queryModel->removeRows(0, queryModel->rowCount(), QModelIndex());

    // in Qt 5.15, we have a nice convenience function setTransferTimeout(),
    // but we are not there yet, currently at 5.11.  So instead, we set the
    // timeout in the configuration of the NetworkAccessManager.

    QNetworkRequest req = QNetworkRequest(queryUrl->url());
    //req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    manager->get(req);
}

void OilDatabaseMainWindow::replyFinished(QNetworkReply* reply) {
    if (reply->error()) {
        qDebug() << "Error in network reply: " << reply->errorString();
        NetworkErrorDialog *errorDialog = new NetworkErrorDialog (
            reply->errorString(),
            Routing::getNetworkReplyType(reply),
            this
        );

        errorDialog->exec();
        delete errorDialog;
        return;
    }
    else {
        qDebug() << "request:" << "\t" << reply->request().url().path();
        QStringList pathParts = (reply->request().url().path()
                                 .split('/', QString::SkipEmptyParts));

        switch(Routing::getNetworkReplyType(reply)) {
        case NetworkReplyType::oil:
            qDebug() << "Network Reply Type: oil";
            handleOilReply(reply);
            break;
        case NetworkReplyType::label:
            qDebug() << "Network Reply Type: label";
            break;
        case NetworkReplyType::productType:
            qDebug() << "Network Reply Type: Product Type";
            handleProductTypeReply(reply);
            break;
        default:
            qDebug() << "Network Reply Type: unknown";
            break;
        }
    }

    reply->deleteLater();
    std::cout << "reply finished.";
}


void OilDatabaseMainWindow::handleOilReply(QNetworkReply* reply) {
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
            queryModel->appendQueryResult(QueryResult{data[idx]});
        }
    }
}


void OilDatabaseMainWindow::handleProductTypeReply(QNetworkReply* reply) {
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
    else if (responseJson.isObject()) {
        qDebug() << "JSON response is an object.  We are not expecting an object.";
        return;
    }
    else if (responseJson.isArray()) {
        QJsonValue data = responseJson[0];
        QJsonArray productTypes = data["product_types"].toArray();

        for (auto idx{0}; idx < productTypes.size(); ++idx) {
            productTypeModel->appendProductType(ProductType{productTypes[idx]});
        }
    }
}


void OilDatabaseMainWindow::on_queryText_textChanged(const QString &text) {
    queryUrl->setQueryText(text);
}

void OilDatabaseMainWindow::on_apiMinValue_valueChanged(double minApi)
{
    queryUrl->setMinApi(minApi);
}

void OilDatabaseMainWindow::on_apiMaxValue_valueChanged(double maxApi)
{
    queryUrl->setMaxApi(maxApi);
}

void OilDatabaseMainWindow::on_oilTypeListView_clicked(const QModelIndex &index)
{
    queryUrl->setQueryType(productTypeModel->data(index).toString());
}
