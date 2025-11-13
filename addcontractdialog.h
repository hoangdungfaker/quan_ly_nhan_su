#ifndef ADDCONTRACTDIALOG_H
#define ADDCONTRACTDIALOG_H

#include <QDialog>
#include "hrm_logic.h"

class QLineEdit;
class QDialogButtonBox;
class QDateEdit;

class AddContractDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddContractDialog(QWidget *parent = nullptr);
    explicit AddContractDialog(const HopDong& contractToEdit, QWidget *parent = nullptr);

    HopDong getContract() const;

private slots:
    void onAccept();

private:
    void setupUI();
    void loadContractData();

    HopDong currentContract;
    bool isEditMode;

    QLineEdit *soHopDongEdit;
    QDateEdit *ngayKyEdit;
    QLineEdit *loaiHopDongEdit;
    QLineEdit *thoiGianHopDongEdit;
    QDialogButtonBox *buttonBox;
};

#endif // ADDCONTRACTDIALOG_H
