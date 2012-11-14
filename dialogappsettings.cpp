#include "dialogappsettings.h"
#include "ui_dialogappsettings.h"

DialogAppSettings::DialogAppSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAppSettings)
{
    ui->setupUi(this);

    connect(ui->buttonBox->buttons().at(0), SIGNAL(clicked()), this, SLOT(close()));
}

DialogAppSettings::~DialogAppSettings()
{
    delete ui;
}

void DialogAppSettings::closeEvent(QCloseEvent *event)
{
    fgenv = new FGEnvironment();
    QDir dir(fgenv->getYFScenery());
    if(!dir.exists())
    {
        //if(dir.mkpath(getYFScenery()))
        if(!dir.mkdir(fgenv->getYFScenery()))
        {
            qFatal("Cannot create dir: %s",fgenv->getYFScenery().toStdString().data());
        }
    }
    appsettings *appsett = new appsettings(fgenv->getYFHome() + "/appconf.ini");
    appsett->setFGDataPath(ui->lnedtFGDataDir->text().trimmed().replace("\\\\","/"));
    appsett->set_fgfs_bin_path(ui->lnedtFGFSBinaryPath->text().trimmed().replace("\\\\","/"));
    if(!appsett->storeData())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Failure"));
        msgBox.setText(tr("App Configuration NOT stored!"));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.addButton(QMessageBox::Close);
        msgBox.setModal(true);
        msgBox.exec();
    }
    //event->accept();
}

void DialogAppSettings::setup_default_paths()
{
    ui->lnedtFGDataDir->setText(fgenv->getRootPath());
    ui->lnedtFGFSBinaryPath->setText(fgenv->getFgfsBinPath());
}

void DialogAppSettings::on_pbtSearchFGData_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select FGDATA directory"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->lnedtFGDataDir->setText(dir);
}

void DialogAppSettings::on_pbtSearchFGFSBin_clicked()
{
    /*QString dir = QFileDialog::getOpenFileName(this, tr("Select fgfs binary path"),
                                                    "",
                                                    QFileDialog::DontResolveSymlinks);*/
    QString path = QFileDialog::getOpenFileName(this,tr("Select fgfs binary path"), "");
    ui->lnedtFGFSBinaryPath->setText(path);
}

void DialogAppSettings::on_pbtSetupDefaultFGSettings_clicked()
{
    setup_default_paths();
}
