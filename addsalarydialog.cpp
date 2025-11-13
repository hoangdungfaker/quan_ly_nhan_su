#include "addsalarydialog.h"
#include <QGridLayout> // <-- THÊM MỚI
#include <QLabel>      // <-- THÊM MỚI
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPushButton>

AddSalaryDialog::AddSalaryDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Them Bang Luong Moi");
    setupUI();
}

void AddSalaryDialog::setupUI()
{
    // Bỏ viền cửa sổ chuẩn
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    // --- Bố cục chính (Layout) ---
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(1, 1, 1, 1);
    mainLayout->setSpacing(0);

    // 1. Header (Tiêu đề)
    QLabel *titleLabel = new QLabel(this->windowTitle());
    titleLabel->setObjectName("titleHeader");
    titleLabel->setAlignment(Qt::AlignCenter);

    // 2. Vùng nội dung (Content)
    QWidget *contentWidget = new QWidget(this);
    contentWidget->setObjectName("contentWidget");
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(20, 20, 20, 20);
    contentLayout->setSpacing(15);

    // 3. Form nhập liệu (Dùng QGridLayout)
    QGridLayout *formLayout = new QGridLayout();
    formLayout->setVerticalSpacing(15);
    formLayout->setHorizontalSpacing(10);

    thangEdit = new QLineEdit(this);
    thangEdit->setPlaceholderText("MM/YYYY (Vi du: 01/2024)");

    luongCoBanSpin = new QDoubleSpinBox(this);
    thuongSpin = new QDoubleSpinBox(this);
    phuCapSpin = new QDoubleSpinBox(this);
    tangCaSpin = new QDoubleSpinBox(this);
    baoHiemSpin = new QDoubleSpinBox(this);
    thueTNCNEspin = new QDoubleSpinBox(this);

    auto setupSpinBox = [](QDoubleSpinBox* spin) {
        spin->setRange(0, 999999999);
        spin->setGroupSeparatorShown(true);
        spin->setDecimals(0);
    };

    setupSpinBox(luongCoBanSpin);
    setupSpinBox(thuongSpin);
    setupSpinBox(phuCapSpin);
    setupSpinBox(tangCaSpin);
    setupSpinBox(baoHiemSpin);
    setupSpinBox(thueTNCNEspin);

    formLayout->addWidget(new QLabel("Thang (*):"), 0, 0, Qt::AlignLeft);
    formLayout->addWidget(thangEdit, 0, 1);
    formLayout->addWidget(new QLabel("Luong Co Ban:"), 1, 0, Qt::AlignLeft);
    formLayout->addWidget(luongCoBanSpin, 1, 1);
    formLayout->addWidget(new QLabel("Thuong:"), 2, 0, Qt::AlignLeft);
    formLayout->addWidget(thuongSpin, 2, 1);
    formLayout->addWidget(new QLabel("Phu Cap:"), 3, 0, Qt::AlignLeft);
    formLayout->addWidget(phuCapSpin, 3, 1);
    formLayout->addWidget(new QLabel("Tien Tang Ca:"), 4, 0, Qt::AlignLeft);
    formLayout->addWidget(tangCaSpin, 4, 1);
    formLayout->addWidget(new QLabel("Bao Hiem (Khau tru):"), 5, 0, Qt::AlignLeft);
    formLayout->addWidget(baoHiemSpin, 5, 1);
    formLayout->addWidget(new QLabel("Thue TNCN (Khau tru):"), 6, 0, Qt::AlignLeft);
    formLayout->addWidget(thueTNCNEspin, 6, 1);

    formLayout->setColumnStretch(1, 1);

    // 4. Nút bấm
    buttonBox = new QDialogButtonBox(this);
    QPushButton *saveButton = buttonBox->addButton("Luu", QDialogButtonBox::AcceptRole);
    QPushButton *cancelButton = buttonBox->addButton("Huy Bo", QDialogButtonBox::RejectRole);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &AddSalaryDialog::onAccept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    contentLayout->addLayout(formLayout);
    contentLayout->addSpacing(15);
    contentLayout->addWidget(buttonBox, 0, Qt::AlignRight);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(contentWidget);

    // --- GIAO DIỆN MỚI (Header/Content) ---
    QString qss = R"(
        QDialog {
            background-color: #34495e; /* Màu nền viền */
        }

        QLabel#titleHeader {
            background-color: #34495e; /* Xanh đậm */
            color: white;
            padding: 12px;
            font-size: 16px;
            font-weight: bold;
        }

        QWidget#contentWidget {
            background-color: white; /* Nền trắng */
        }

        QLabel {
            font-weight: bold;
            font-size: 13px;
            color: #333;
        }

        QLineEdit, QDoubleSpinBox {
            padding: 8px;
            border: 1px solid black;
            border-radius: 4px;
            background-color: white;
            font-size: 13px;
            color: black;
        }
        QLineEdit:focus, QDoubleSpinBox:focus {
             border: 2px solid #007bff;
        }
        QDoubleSpinBox::up-button, QDoubleSpinBox::down-button {
            border-left: 1px solid black;
            width: 18px;
        }
        QPushButton {
            padding: 10px 28px;
            font-size: 14px;
            font-weight: bold;
            border-radius: 5px;
            border: none;
            color: white;
            min-width: 80px;
        }
    )";
    this->setStyleSheet(qss);

    saveButton->setStyleSheet("background-color: #007bff; color: white;");
    cancelButton->setStyleSheet("background-color: #6c757d; color: white;");

    adjustSize();
}

void AddSalaryDialog::onAccept()
{
    string thang = trim(thangEdit->text().toStdString());
    if (thang.length() != 7 || thang[2] != '/') {
        QMessageBox::warning(this, "Loi", "Dinh dang Thang khong hop le. Vui long nhap (MM/YYYY).");
        return;
    }
    accept();
}

Luong AddSalaryDialog::getSalary() const
{
    Luong luong;
    luong.setThang(trim(thangEdit->text().toStdString()));
    luong.setLuongCoBan(luongCoBanSpin->value());
    luong.setThuong(thuongSpin->value());
    luong.setPhuCap(phuCapSpin->value());
    luong.setTangCa(tangCaSpin->value());
    luong.setBaoHiem(baoHiemSpin->value());
    luong.setThueTNCN(thueTNCNEspin->value());
    luong.TinhThucLanh();
    return luong;
}
