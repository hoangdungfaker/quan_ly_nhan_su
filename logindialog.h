#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "hrm_logic.h"

class QLineEdit;
class QPushButton;
class QLabel; // <-- Thêm thư viện QLabel

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(DanhSachNhanVien& dsnv, QWidget *parent = nullptr);

    string getUserRole() const { return role; }
    NhanVien* getUser() const { return loggedInUser; }

private slots:
    void on_loginButton_clicked();

private:
    // --- THAY ĐỔI: Cập nhật các thành phần UI ---
    QLabel *iconLabel; // Thêm cho icon user
    QLabel *titleLabel; // Thêm cho chữ "Sign In"
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    // Nút "Thoat" (cancelButton) đã bị loại bỏ để giống thiết kế

    DanhSachNhanVien& dsnv;

    NhanVien* loggedInUser;
    string role;
};

#endif // LOGINDIALOG_H
