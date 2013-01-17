#include "mpmdialog.h"
#include "ui_mpmdialog.h"

MPMDialog::MPMDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MPMDialog)
{
    ui->setupUi(this);
    setup_mp();
}

MPMDialog::~MPMDialog()
{
    delete ui;
}

void MPMDialog::setup_mp()
{
    mpmapbridge *bridge = new mpmapbridge();
    ui->webViewMMap->setUrl(bridge->getUrl());
}

void MPMDialog::on_pbtRefreshMPMap_clicked()
{
//    mpmapbridge *bridge = new mpmapbridge();
//    if(ui->ckbFollowPilot->isChecked())
//    {
//        bridge->setPilotToFollow(ui->lnedtPilot->text().trimmed());

//        if(ui->ckbShowMPMapMenu->isChecked())
//        {
//            ui->webViewMMap->setUrl(bridge->getFollowPilotUrl());
//        }
//        else
//        {
//            ui->webViewMMap->setUrl(bridge->getFollowPilotUrlWithoutMenu());
//        }
//    }
//    else
//    {
//        if(ui->ckbShowMPMapMenu->isChecked())
//        {
//            ui->webViewMMap->setUrl(bridge->getUrl());
//        }
//        else
//        {
//            ui->webViewMMap->setUrl(bridge->getUrlWithoutMenu());
//        }
//    }
    ui->webViewMMap->reload();
}
