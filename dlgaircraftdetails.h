#ifndef DLGAIRCRAFTDETAILS_H
#define DLGAIRCRAFTDETAILS_H

#include <QDialog>
#include <QStringList>
#include <QTableWidgetItem>

#include "fgenvironment.h"
#include "imagepreview.h"

namespace Ui {
class dlgAircraftDetails;
}

class dlgAircraftDetails : public QDialog
{
    Q_OBJECT
    
public:
    explicit dlgAircraftDetails(QStringList details,
                                FGEnvironment *fgenv,
                                QWidget *parent = 0);
    ~dlgAircraftDetails();
    
private:
    Ui::dlgAircraftDetails *ui;
};

#endif // DLGAIRCRAFTDETAILS_H
