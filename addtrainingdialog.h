#ifndef ADDTRAININGDIALOG_H
#define ADDTRAININGDIALOG_H

#include <QDialog>
#include "hrm_logic.h"

class QLineEdit;
class QDialogButtonBox;
class QDateEdit; // <-- THÊM MỚI

class AddTrainingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTrainingDialog(QWidget *parent = nullptr);
    DaoTao getTraining() const;

private slots:
    void onAccept();

private:
    bool isDateAfter(const QDate& date1, const QDate& date2); // <-- THAY ĐỔI: Dùng QDate

    QLineEdit *maKhoaHocEdit;
    QLineEdit *tenKhoaHocEdit;
    QDateEdit *ngayBatDauEdit; // <-- THAY ĐỔI: Từ QLineEdit
    QDateEdit *ngayKetThucEdit; // <-- THAY ĐỔI: Từ QLineEdit
    QLineEdit *donViDaoTaoEdit;
    QDialogButtonBox *buttonBox;
};

#endif // ADDTRAININGDIALOG_H
