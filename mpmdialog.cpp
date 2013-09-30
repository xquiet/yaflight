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
    ui->webViewMMap->reload();
}
