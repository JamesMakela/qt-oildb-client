#include "networkerrordialog.h"
#include "ui_networkerrordialog.h"


NetworkErrorDialog::NetworkErrorDialog(QWidget *parent,
                                       NetworkReplyType replyType)
    : QDialog(parent), ui(new Ui::NetworkErrorDialog)
{
    ui->setupUi(this);

    switch(replyType) {
    case NetworkReplyType::oil:
        ui->errorMessage->setText("Error: timeout on request /oils");
        break;
    case NetworkReplyType::label:
        ui->errorMessage->setText("Error: timeout on request /labels");
        break;
    case NetworkReplyType::productType:
        ui->errorMessage->setText("Error: timeout on request /product-types");
        break;
    default:
        ui->errorMessage->setText("Error: timeout on request");
        break;
    }
}

NetworkErrorDialog::~NetworkErrorDialog()
{
    delete ui;
}
