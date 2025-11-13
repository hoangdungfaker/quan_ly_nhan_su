#ifndef ADDWORKHISTORYDIALOG_H
#define ADDWORKHISTORYDIALOG_H

#include <QDialog>
#include "hrm_logic.h"

class QLineEdit;
class QDialogButtonBox;
class QDateEdit; // <-- THÊM MỚI

class AddWorkHistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddWorkHistoryDialog(QWidget *parent = nullptr);
    QuaTrinhCongTac getWorkHistory() const;

private slots:
    void onAccept();

private:
    QDateEdit *ngayVaoLamEdit; // <-- THAY ĐỔI: Từ QLineEdit
    QLineEdit *boPhanEdit;
    QLineEdit *chucVuEdit;
    QDialogButtonBox *buttonBox;
};

#endif // ADDWORKHISTORYDIALOG_H
