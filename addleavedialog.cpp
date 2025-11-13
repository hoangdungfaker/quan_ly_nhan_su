#include "addleavedialog.h"
#include <QGridLayout> // <-- THÊM MỚI
#include <QLabel>      // <-- THÊM MỚI
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDateEdit>
#include <QPushButton>
#include <QCalendarWidget>

AddLeaveDialog::AddLeaveDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Them Don Nghi Phep");
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

    loaiPhepEdit = new QLineEdit(this);
    loaiPhepEdit->setPlaceholderText("Phep nam, Phep om, Khong luong...");
    soNgayEdit = new QLineEdit(this);
    soNgayEdit->setPlaceholderText("Vi du: 3");

    ngayBatDauEdit = new QDateEdit(this);
    ngayBatDauEdit->setDisplayFormat("dd/MM/yyyy");
    ngayBatDauEdit->setCalendarPopup(true);
    ngayBatDauEdit->setDate(QDate::currentDate());

    ngayKetThucEdit = new QDateEdit(this);
    ngayKetThucEdit->setDisplayFormat("dd/MM/yyyy");
    ngayKetThucEdit->setCalendarPopup(true);
    ngayKetThucEdit->setDate(QDate::currentDate());

    formLayout->addWidget(new QLabel("Loai Phep:"), 0, 0, Qt::AlignLeft);
    formLayout->addWidget(loaiPhepEdit, 0, 1);
    formLayout->addWidget(new QLabel("So Ngay:"), 1, 0, Qt::AlignLeft);
    formLayout->addWidget(soNgayEdit, 1, 1);
    formLayout->addWidget(new QLabel("Ngay Bat Dau (*):"), 2, 0, Qt::AlignLeft);
    formLayout->addWidget(ngayBatDauEdit, 2, 1);
    formLayout->addWidget(new QLabel("Ngay Ket Thuc (*):"), 3, 0, Qt::AlignLeft);
    formLayout->addWidget(ngayKetThucEdit, 3, 1);

    formLayout->setColumnStretch(1, 1);

    // 4. Nút bấm
    buttonBox = new QDialogButtonBox(this);
    QPushButton *saveButton = buttonBox->addButton("Luu", QDialogButtonBox::AcceptRole);
    QPushButton *cancelButton = buttonBox->addButton("Huy Bo", QDialogButtonBox::RejectRole);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &AddLeaveDialog::onAccept);
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
        }
        QLineEdit, QDateEdit {
            padding: 8px;
            border: 1px solid black;
            border-radius: 4px;
            background-color: white;
            font-size: 13px;
            color: black;
        }
        QLineEdit:focus, QDateEdit:focus {
             border: 2px solid #007bff;
        }
        QDateEdit::drop-down {
            width: 20px;
            border-left: 1px solid black;
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

bool AddLeaveDialog::isDateAfter(const QDate& date1, const QDate& date2) {
    return date1 > date2;
}

void AddLeaveDialog::onAccept()
{
    QDate ngayBD = ngayBatDauEdit->date();
    QDate ngayKT = ngayKetThucEdit->date();

    if (isDateAfter(ngayBD, ngayKT)) {
        QMessageBox::warning(this, "Loi", "Ngay Ket Thuc phai sau hoac bang Ngay Bat Dau.");
        return;
    }
    accept();
}

NghiPhep AddLeaveDialog::getLeave() const
{
    NghiPhep np;
    np.setLoaiPhep(trim(loaiPhepEdit->text().toStdString()));
    np.setSoNgay(trim(soNgayEdit->text().toStdString()));
    np.setNgayBatDau(ngayBatDauEdit->date().toString("dd/MM/yyyy").toStdString());
    np.setNgayKetThuc(ngayKetThucEdit->date().toString("dd/MM/yyyy").toStdString());
    return np;
}
