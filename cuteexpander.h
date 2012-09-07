#ifndef CUTEEXPANDER_H
#define CUTEEXPANDER_H

#include <QWidget>
#include <QObjectList>

namespace Ui{
class CuteExpander;
}

class CuteExpander : public QWidget
{
    Q_OBJECT
public:
    explicit CuteExpander(QWidget *parent = 0);

    void setText(QString text);
    void expand();
    void unexpand();
    
signals:
    void expanded();
    void unexpanded();

public slots:

private slots:
    void on_btnToggle_clicked();

private:
    Ui::CuteExpander *ui;
};

#endif // CUTEEXPANDER_H
