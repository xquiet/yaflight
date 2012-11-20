#include "dialogmpdetails.h"
#include "ui_dialogmpdetails.h"

DialogMPDetails::DialogMPDetails(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMPDetails)
{
    ui->setupUi(this);

    connect((QObject *)ui->buttonBox->buttons().at(0), SIGNAL(clicked()), this, SLOT(close()));
}

DialogMPDetails::~DialogMPDetails()
{
    delete ui;
}

void DialogMPDetails::closeEvent(QCloseEvent *)
{
    portIn = ui->lnedtIn->text().trimmed().toInt();
    portOut = ui->lnedtOut->text().trimmed().toInt();
}

void DialogMPDetails::showEvent(QShowEvent *)
{
    ui->lnedtIn->setText(QString::number(portIn));
    ui->lnedtOut->setText(QString::number(portOut));
}

QString DialogMPDetails::getIn()
{
    return QString::number(portIn);
}

QString DialogMPDetails::getOut()
{
    return QString::number(portOut);
}

void DialogMPDetails::setIn(int pIn)
{
    portIn = pIn;
}

void DialogMPDetails::setOut(int pOut)
{
    portOut = pOut;
}
