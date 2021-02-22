#ifndef OILDATABASEMAINWINDOW_H
#define OILDATABASEMAINWINDOW_H

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

private slots:
    void on_requestButton_clicked();
    void replyFinished(QNetworkReply*);

private:
    Ui::OilDatabaseMainWindow *ui;
    QNetworkAccessManager *manager;
};

#endif // OILDATABASEMAINWINDOW_H
