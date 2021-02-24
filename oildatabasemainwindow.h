#ifndef OILDATABASEMAINWINDOW_H
#define OILDATABASEMAINWINDOW_H

#include <QMainWindow>

#include <QtNetwork/QNetworkAccessManager>

enum class OilStatus
{
    good,
    warning,
    error
};

namespace Ui {
class OilDatabaseMainWindow;
}

class OilDatabaseMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OilDatabaseMainWindow(QWidget *parent = nullptr);
    ~OilDatabaseMainWindow();

private slots:
    void on_requestButton_clicked();
    void replyFinished(QNetworkReply*);

private:
    Ui::OilDatabaseMainWindow *ui;
    QNetworkAccessManager *manager;

    OilStatus determine_status(const QJsonArray &jsonStatus);
};

#endif // OILDATABASEMAINWINDOW_H
