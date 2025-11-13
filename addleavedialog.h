#ifndef ADDLEAVEDIALOG_H
#define ADDLEAVEDIALOG_H

#include <QDialog>
#include "hrm_logic.h"

class QLineEdit;
class QDialogButtonBox;
class QDateEdit; // <-- THÊM MỚI

class AddLeaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddLeaveDialog(QWidget *parent = nullptr);
    NghiPhep getLeave() const;

private slots:
    void onAccept();

private:
    bool isDateAfter(const QDate& date1, const QDate& date2); // <-- THAY ĐỔI: Dùng QDate

    QLineEdit *loaiPhepEdit;
    QLineEdit *soNgayEdit;
    QDateEdit *ngayBatDauEdit; // <-- THAY ĐỔI: Từ QLineEdit
    QDateEdit *ngayKetThucEdit; // <-- THAY ĐỔI: Từ QLineEdit
    QDialogButtonBox *buttonBox;
};

#endif // ADDLEAVEDIALOG_H
