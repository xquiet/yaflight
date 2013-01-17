#ifndef MPMDIALOG_H
#define MPMDIALOG_H

#include <QDialog>

#include "mpmapbridge.h"

namespace Ui {
class MPMDialog;
}

class MPMDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MPMDialog(QWidget *parent = 0);
    ~MPMDialog();
    
private slots:
    void on_pbtRefreshMPMap_clicked();

private:
    Ui::MPMDialog *ui;
    void setup_mp();
};

#endif // MPMDIALOG_H
