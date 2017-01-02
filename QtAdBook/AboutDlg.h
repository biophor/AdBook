#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include <QDialog>
#include "ui_AboutDlg.h"

class AboutDlg : public QDialog
{
    Q_OBJECT

public:
    AboutDlg(QWidget *parent = 0);
    ~AboutDlg();
private slots:
    void OnOk();
private:
    Ui::AboutDlg ui;
};

#endif // ABOUTDLG_H
