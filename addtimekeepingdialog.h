#ifndef ADDTIMEKEEPINGDIALOG_H
#define ADDTIMEKEEPINGDIALOG_H

#include <QDialog>
#include "hrm_logic.h"

class QLineEdit;
class QComboBox;
class QDoubleSpinBox;
class QDialogButtonBox;
class QDateEdit; // <-- THÊM MỚI

class AddTimekeepingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTimekeepingDialog(QWidget *parent = nullptr);
    ChamCong getTimekeeping() const;

private slots:
    void onAccept();

private:
    QDateEdit *ngayCongEdit; // <-- THAY ĐỔI: Từ QLineEdit
    QComboBox *phanCaCombo;
    QDoubleSpinBox *soGioThemSpin;
    QDialogButtonBox *buttonBox;
};

#endif // ADDTIMEKEEPINGDIALOG_H
