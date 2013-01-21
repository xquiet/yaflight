#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    // ugly focus workaround until the issue is resolved
    // press ESC to exit close the help dialog :-/
#ifdef Q_OS_MACX
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
#endif
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::setUrl(QString url)
{
    ui->wbvOnlineHelp->setUrl(url);
    ui->lblTitle->setText(url.mid(url.lastIndexOf("/")+1,url.length()));
}

QString HelpDialog::getCurrentUrl()
{
    return ui->wbvOnlineHelp->url().toString();
}
