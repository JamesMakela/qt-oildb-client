#include "networkerrordialog.h"
#include "ui_networkerrordialog.h"


NetworkErrorDialog::NetworkErrorDialog(const QString &errorMessage,
                                       NetworkReplyType replyType,
                                       QWidget *parent)
    : QDialog(parent), ui(new Ui::NetworkErrorDialog)
{
    QString uriPath;
    ui->setupUi(this);

    switch(replyType) {
    case NetworkReplyType::oil:
        uriPath = "/oils";
        break;
    case NetworkReplyType::label:
        uriPath = "/labels";
        break;
    case NetworkReplyType::productType:
        uriPath = "/product-types";
        break;
    default:
        break;
    }
    ui->errorMessage->setText(QString("Error: %1 on request %2")
                              .arg(errorMessage).arg(uriPath));
}

NetworkErrorDialog::~NetworkErrorDialog()
{
    delete ui;
}
