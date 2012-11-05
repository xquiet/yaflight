#ifndef DIALOGAPPSETTINGS_H
#define DIALOGAPPSETTINGS_H

#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>

#include "appsettings.h"
#include "fgenvironment.h"

namespace Ui {
class DialogAppSettings;
}

class DialogAppSettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogAppSettings(QWidget *parent = 0);
    ~DialogAppSettings();
    
private:
    Ui::DialogAppSettings *ui;
    FGEnvironment *fgenv;
    void setup_default_paths();

private slots:
    void closeEvent(QCloseEvent *);
    void on_pbtSearchFGData_clicked();
    void on_pbtSearchFGFSBin_clicked();
    void on_pbtSetupDefaultFGSettings_clicked();
};

#endif // DIALOGAPPSETTINGS_H
