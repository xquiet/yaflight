#include "dialogmpdetails.h"
#include "ui_dialogmpdetails.h"

DialogMPDetails::DialogMPDetails(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMPDetails)
{
    ui->setupUi(this);
}

DialogMPDetails::~DialogMPDetails()
{
    delete ui;
}

void DialogMPDetails::closeEvent(QCloseEvent *)
{

}

void DialogMPDetails::on_buttonBox_clicked(QAbstractButton *button)
{
//    QDialogButtonBox clickedButton();
//    if( clickedButton.standardButton(button) != QDialogButtonBox::Ok )
//        return;
//    fgenv = new FGEnvironment();
//    if(!curr_settings)
//        curr_settings = new Settings(fgenv->getYFHome()+"/conf.ini");

//    curr_settings->setMPIN(ui->lnedtIn->text().trimmed());
//    curr_settings->setMPOUT(ui->lnedtOut->text().trimmed());
//    if(!curr_settings->storeData())
//    {
//        qFatal("Something went wrong\n" + MPIN + " and " + MPOUT + " were not properly stored\n");
//    }
}
