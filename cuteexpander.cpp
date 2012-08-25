#include "cuteexpander.h"
#include "ui_cuteexpander.h"

CuteExpander::CuteExpander(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CuteExpander)
{
    ui->setupUi(this);
    ui->btnToggle->setIcon(QIcon(":/icons/icons/arrowUp.png"));
    ui->btnToggle->setIconSize(QSize(16,16));
}

void CuteExpander::on_btnToggle_clicked()
{
    if(this->height() == ui->btnToggle->height()+1)
    {
        expand();
    }
    else
    {
        unexpand();
    }
}

void CuteExpander::setText(QString text)
{
    ui->btnToggle->setText(text);
}

void CuteExpander::expand()
{
    this->setGeometry(this->x(),this->y(),this->width(),this->maximumHeight());
    ui->btnToggle->setIcon(QIcon(":/icons/icons/arrowUp.png"));
}

void CuteExpander::unexpand()
{
    this->setGeometry(this->x(),this->y(),this->width(),ui->btnToggle->height()+1);
    ui->btnToggle->setIcon(QIcon(":/icons/icons/arrowDown.png"));
}
