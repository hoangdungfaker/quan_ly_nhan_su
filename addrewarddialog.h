#ifndef ADDREWARDDIALOG_H
#define ADDREWARDDIALOG_H

#include <QDialog>
#include "hrm_logic.h"

class QLineEdit;
class QComboBox;
class QDateEdit;
class QDoubleSpinBox;
class QTextEdit;
class QDialogButtonBox;

class AddRewardDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddRewardDialog(QWidget *parent = nullptr);
    KhenThuongKyLuat getRewardDiscipline() const;

private slots:
    void onAccept();

private:
    QComboBox *loaiCombo;
    QDateEdit *ngayEdit; // <-- THAY ĐỔI: Từ QLineEdit
    QTextEdit *noiDungEdit;
    QLineEdit *quyetDinhEdit;
    QDoubleSpinBox *soTienSpin;
    QDialogButtonBox *buttonBox;
};

#endif // ADDREWARDDIALOG_H
