#include "addrewarddialog.h"
#include <QGridLayout> // <-- THAY ĐỔI: Dùng QGridLayout
#include <QLabel>      // <-- THÊM MỚI
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QCalendarWidget>

AddRewardDialog::AddRewardDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Them Khen Thuong / Ky Luat");
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

    loaiCombo = new QComboBox(this);
    loaiCombo->addItems({"Khen thuong", "Ky luat"});

    ngayEdit = new QDateEdit(this);
    ngayEdit->setDisplayFormat("dd/MM/yyyy");
    ngayEdit->setCalendarPopup(true);
    ngayEdit->setDate(QDate::currentDate());

    noiDungEdit = new QTextEdit(this);
    noiDungEdit->setFixedHeight(80);

    quyetDinhEdit = new QLineEdit(this);

    soTienSpin = new QDoubleSpinBox(this);
    soTienSpin->setRange(0, 999999999);
    soTienSpin->setGroupSeparatorShown(true);
    soTienSpin->setDecimals(0);
    soTienSpin->setSuffix(" VND");

    formLayout->addWidget(new QLabel("Phan loai (*):"), 0, 0, Qt::AlignLeft);
    formLayout->addWidget(loaiCombo, 0, 1);
    formLayout->addWidget(new QLabel("Ngay (*):"), 1, 0, Qt::AlignLeft);
    formLayout->addWidget(ngayEdit, 1, 1);
    formLayout->addWidget(new QLabel("Noi dung:"), 2, 0, Qt::AlignLeft); // <-- Lỗi ở đây
    formLayout->addWidget(noiDungEdit, 2, 1);
    formLayout->addWidget(new QLabel("So Quyet Dinh (*):"), 3, 0, Qt::AlignLeft);
    formLayout->addWidget(quyetDinhEdit, 3, 1);
    formLayout->addWidget(new QLabel("So Tien:"), 4, 0, Qt::AlignLeft);
    formLayout->addWidget(soTienSpin, 4, 1);

    formLayout->setColumnStretch(1, 1);

    // 4. Nút bấm
    buttonBox = new QDialogButtonBox(this);
    QPushButton *saveButton = buttonBox->addButton("Luu", QDialogButtonBox::AcceptRole);
    QPushButton *cancelButton = buttonBox->addButton("Huy Bo", QDialogButtonBox::RejectRole);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &AddRewardDialog::onAccept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    contentLayout->addLayout(formLayout);
    contentLayout->addSpacing(15);
    contentLayout->addWidget(buttonBox, 0, Qt::AlignRight);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(contentWidget);

    // --- GIAO DIỆN MỚI (Header/Content) ---
    QString qss = R"(
        QDialog {
            background-color: #34495e;
        }
        QLabel#titleHeader {
            background-color: #34495e;
            color: white;
            padding: 12px;
            font-size: 16px;
            font-weight: bold;
        }
        QWidget#contentWidget {
            background-color: white;
        }
        QLabel {
            font-weight: bold;
            font-size: 13px;
            color: #333;
            /* Không cần set background-color vì QGridLayout ổn định */
        }
        QLineEdit, QDateEdit, QComboBox, QDoubleSpinBox, QTextEdit {
            padding: 8px;
            border: 1px solid black;
            border-radius: 4px;
            background-color: white;
            font-size: 13px;
            color: black;
        }
        QLineEdit:focus, QDateEdit:focus, QComboBox:focus, QDoubleSpinBox:focus, QTextEdit:focus {
             border: 2px solid #007bff;
        }
        QDateEdit::drop-down, QComboBox::drop-down {
            width: 20px;
            border-left: 1px solid black;
        }
        QComboBox QAbstractItemView {
             border: 1px solid black;
             background-color: white;
             selection-background-color: #007bff;
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

void AddRewardDialog::onAccept()
{
    if (trim(quyetDinhEdit->text().toStdString()).empty()) {
        QMessageBox::warning(this, "Loi", "So Quyet Dinh la bat buoc.");
        return;
    }
    accept();
}

KhenThuongKyLuat AddRewardDialog::getRewardDiscipline() const
{
    KhenThuongKyLuat ktkl;
    ktkl.setLoai(loaiCombo->currentText().toStdString());
    ktkl.setNgay(ngayEdit->date().toString("dd/MM/yyyy").toStdString());
    ktkl.setNoiDung(trim(noiDungEdit->toPlainText().toStdString()));
    ktkl.setQuyetDinh(trim(quyetDinhEdit->text().toStdString()));
    ktkl.setSoTien(soTienSpin->value());
    return ktkl;
}
