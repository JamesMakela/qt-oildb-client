#ifndef OILDATABASEMAINWINDOW_H
#define OILDATABASEMAINWINDOW_H

#include "queryresultmodel.h"
#include "producttypemodel.h"
#include "adiosapiqueryurl.h"

#include <QMainWindow>

#include <QtNetwork/QNetworkAccessManager>


namespace Ui {
class OilDatabaseMainWindow;
}

class OilDatabaseMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OilDatabaseMainWindow(QWidget *parent = nullptr);
    ~OilDatabaseMainWindow();
    void setupListControls();
    void handleOilReply(QNetworkReply *reply);
    void handleProductTypeReply(QNetworkReply *reply);

private slots:
    void on_requestButton_clicked();
    void replyFinished(QNetworkReply*);
    void on_queryText_textChanged(const QString &arg1);
    void on_apiMinValue_valueChanged(double arg1);
    void on_apiMaxValue_valueChanged(double arg1);

    void on_oilTypeListView_clicked(const QModelIndex &index);

private:
    Ui::OilDatabaseMainWindow *ui;
    QNetworkAccessManager *manager;
    QueryResultModel *queryModel;
    ProductTypeModel *productTypeModel;

    AdiosApiQueryUrl *queryUrl;
};

#endif // OILDATABASEMAINWINDOW_H
