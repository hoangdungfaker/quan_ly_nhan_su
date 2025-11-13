#ifndef ADDSALARYDIALOG_H
#define ADDSALARYDIALOG_H

#include <QDialog>
#include "hrm_logic.h"

class QLineEdit;
class QDoubleSpinBox;
class QDialogButtonBox;

class AddSalaryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSalaryDialog(QWidget *parent = nullptr);
    Luong getSalary() const;

private slots:
    void onAccept();

private:
    void setupUI();

    QLineEdit *thangEdit;
    QDoubleSpinBox *luongCoBanSpin;
    QDoubleSpinBox *thuongSpin;
    QDoubleSpinBox *phuCapSpin;
    QDoubleSpinBox *tangCaSpin;
    QDoubleSpinBox *baoHiemSpin;
    QDoubleSpinBox *thueTNCNEspin;

    QDialogButtonBox *buttonBox;
};

#endif // ADDSALARYDIALOG_H
