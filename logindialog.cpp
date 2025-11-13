#include "logindialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <algorithm> // cho std::transform
#include <QPixmap>
#include <QStyle>

LoginDialog::LoginDialog(DanhSachNhanVien& dsnv, QWidget *parent)
    : QDialog(parent), dsnv(dsnv), loggedInUser(nullptr)
{
    // Cài đặt Dialog
    setWindowTitle("Sign In");
    setModal(true);
    setFixedSize(380, 500);
    setObjectName("loginDialog");

    // 1. Khởi tạo các thành phần UI

    // Icon User ở trên
    iconLabel = new QLabel(this);
    QPixmap pixmap = style()->standardIcon(QStyle::SP_ComputerIcon).pixmap(QSize(96, 96));
    iconLabel->setPixmap(pixmap);
    iconLabel->setFixedSize(96, 96);
    iconLabel->setObjectName("iconLabel"); // Đặt tên cho QSS

    // Tiêu đề "Sign In"
    titleLabel = new QLabel("Sign In", this);
    titleLabel->setObjectName("titleLabel");

    // Ô nhập liệu
    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Username");

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);

    // Nút
    loginButton = new QPushButton("LOGIN", this);
    loginButton->setObjectName("loginButton");

    // 2. Thiết lập layout (bố cục)
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(iconLabel, 0, Qt::AlignCenter);
    layout->addWidget(titleLabel, 0, Qt::AlignCenter);
    layout->addSpacing(20);
    layout->addWidget(usernameEdit);
    layout->addSpacing(15);
    layout->addWidget(passwordEdit);
    layout->addSpacing(30);
    layout->addWidget(loginButton);
    layout->addStretch();

    layout->setContentsMargins(40, 40, 40, 40);
    setLayout(layout);

    // 3. Kết nối signals và slots
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::on_loginButton_clicked);

    // 4. Áp dụng Style Sheet (QSS)
    // --- THAY ĐỔI QSS ĐỂ GIỐNG HÌNH MỚI ---
    QString qss = R"(
        QDialog#loginDialog {
            /* Nền gradient xanh lá */
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 #84fab0, stop:1 #8fd3f4);
        }

        QLabel#iconLabel {
            border: 4px solid white;
            border-radius: 48px; /* (96 / 2) */
            background-color: #fdec00; /* Nền vàng cho icon */
        }

        QLabel#titleLabel {
            font-size: 28px;
            font-weight: bold;
            color: white;
            padding-top: 10px;
        }

        QLineEdit {
            font-size: 16px;
            font-weight: bold;      /* <-- SỬA: Chữ đậm */
            padding: 15px 20px;     /* Tăng chiều cao */
            color: black;           /* <-- SỬA: Chữ màu đen */
            background-color: #fdec00; /* Nền vàng */
            border: none;           /* Không viền */
            border-radius: 25px;    /* Bo tròn nhiều hơn */
        }

        QLineEdit::placeholder {
            color: #555;            /* <-- SỬA: Chữ mờ màu xám đậm */
            font-weight: normal;
        }

        QPushButton#loginButton {
            font-size: 16px;
            font-weight: bold;
            color: #333;             /* Chữ màu đen */
            background-color: white; /* Nền trắng */
            padding: 15px;
            border-radius: 25px;     /* Bo tròn nhiều hơn */
            border: none;
        }

        QPushButton#loginButton:hover {
            background-color: #f0f0f0;
        }
    )";
    this->setStyleSheet(qss);
}

// Hàm logic đăng nhập (giữ nguyên, không thay đổi)
void LoginDialog::on_loginButton_clicked()
{
    string username = trim(usernameEdit->text().toStdString());
    string password = trim(passwordEdit->text().toStdString());

    if (username.empty() || password.empty()) {
        QMessageBox::warning(this, "Thieu thong tin", "Vui long nhap day du ten dang nhap va mat khau.");
        return;
    }

    bool found = false;

    for (auto& nv_ptr : dsnv.getDanhSach())
    {
        if (nv_ptr->getUsername() == username && nv_ptr->getPassword() == password) {
            loggedInUser = nv_ptr.get();
            role = trim(nv_ptr->getRole());
            transform(role.begin(), role.end(), role.begin(), ::tolower);
            found = true;
            break;
        }
    }

    if (found) {
        accept();
    } else {
        QMessageBox::warning(this, "Dang nhap that bai", "Ten dang nhap hoac mat khau sai!");
        loggedInUser = nullptr;
    }
}
