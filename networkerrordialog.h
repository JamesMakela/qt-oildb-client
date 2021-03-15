#ifndef NETWORKERRORDIALOG_H
#define NETWORKERRORDIALOG_H

#include <QDialog>

#include "routing.h"


namespace Ui {
class NetworkErrorDialog;
}

class NetworkErrorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetworkErrorDialog(const QString &errorMessage,
                                NetworkReplyType replyType = NetworkReplyType::unknown,
                                QWidget *parent = nullptr);
    ~NetworkErrorDialog();

private:
    Ui::NetworkErrorDialog *ui;
};

#endif // NETWORKERRORDIALOG_H
