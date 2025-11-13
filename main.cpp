#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include "hrm_logic.h"
#include "logindialog.h"
#include "mainwindow.h"
#include "employeeviewdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. Tải dữ liệu
    DanhSachNhanVien dsnv;
    QString appPath = QApplication::applicationDirPath();
    QString filenameStr = appPath + "/danhsachnhanvien.txt";
    string filename = filenameStr.toStdString();
    qDebug() << "Dang tim file tai:" << filenameStr;
    dsnv.LoadFromFile(filename);

    // 2. Hiển thị Dialog Đăng nhập
    LoginDialog loginDialog(dsnv);

    if (loginDialog.exec() == QDialog::Accepted) {
        string role = loginDialog.getUserRole();
        NhanVien* user = loginDialog.getUser();

        // 3. Quyết định hiển thị cửa sổ nào
        if (role == "admin" || role == "quan ly") {
            MainWindow *w = new MainWindow(dsnv, user);
            w->show();
        }
        else if (role == "nhan vien") {
            // --- THAY ĐỔI: Truyền "dsnv" vào ---
            EmployeeViewDialog dialog(*user, dsnv, nullptr);
            dialog.exec();
            // ---------------------------------
            return 0;
        }
        else {
            QMessageBox::critical(nullptr, "Loi", "Vai tro khong hop le!");
            return 1;
        }

        return a.exec();
    }

    return 0;
}
