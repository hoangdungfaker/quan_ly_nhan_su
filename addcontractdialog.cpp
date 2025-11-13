#include "addcontractdialog.h"
#include <QGridLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDateEdit>
#include <QPushButton>
#include <QCalendarWidget>
#include <QLabel>

AddContractDialog::AddContractDialog(QWidget *parent)
    : QDialog(parent), isEditMode(false)
{
    setWindowTitle("Them Hop Dong Moi");
    setupUI();
    ngayKyEdit->setDate(QDate::currentDate());
}

AddContractDialog::AddContractDialog(const HopDong& contractToEdit, QWidget *parent)
    : QDialog(parent), currentContract(contractToEdit), isEditMode(true)
{
    setWindowTitle("Sua Thong Tin Hop Dong");
    setupUI();
    loadContractData();
    soHopDongEdit->setReadOnly(true);
}

void AddContractDialog::setupUI()
{
    // Bỏ viền cửa sổ chuẩn
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    // --- Bố cục chính (Layout) ---
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(1, 1, 1, 1); // Viền mỏng 1px
    mainLayout->setSpacing(0); // Không khoảng cách giữa Header và Content

    // 1. Header (Tiêu đề)
    QLabel *titleLabel = new QLabel(this->windowTitle()); // Lấy tiêu đề
    titleLabel->setObjectName("titleHeader");
    titleLabel->setAlignment(Qt::AlignCenter);

    // 2. Vùng nội dung (Content)
    QWidget *contentWidget = new QWidget(this);
    contentWidget->setObjectName("contentWidget");
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(20, 20, 20, 20);
    contentLayout->setSpacing(15);

    // 3. Form nhập liệu
    QGridLayout *formLayout = new QGridLayout();
    formLayout->setVerticalSpacing(15);
    formLayout->setHorizontalSpacing(10);

    soHopDongEdit = new QLineEdit(this);
    ngayKyEdit = new QDateEdit(this);
    ngayKyEdit->setDisplayFormat("dd/MM/yyyy");
    ngayKyEdit->setCalendarPopup(true);
    loaiHopDongEdit = new QLineEdit(this);
    thoiGianHopDongEdit = new QLineEdit(this);

    formLayout->addWidget(new QLabel("So Hop Dong (*):"), 0, 0, Qt::AlignLeft);
    formLayout->addWidget(soHopDongEdit, 0, 1);
    formLayout->addWidget(new QLabel("Ngay Ky (*):"), 1, 0, Qt::AlignLeft);
    formLayout->addWidget(ngayKyEdit, 1, 1);
    formLayout->addWidget(new QLabel("Loai Hop Dong:"), 2, 0, Qt::AlignLeft);
    formLayout->addWidget(loaiHopDongEdit, 2, 1);
    formLayout->addWidget(new QLabel("Thoi Gian Hop Dong:"), 3, 0, Qt::AlignLeft);
    formLayout->addWidget(thoiGianHopDongEdit, 3, 1);
    formLayout->setColumnStretch(1, 1);

    // 4. Nút bấm
    buttonBox = new QDialogButtonBox(this);
    QPushButton *saveButton = buttonBox->addButton("Luu", QDialogButtonBox::AcceptRole);
    QPushButton *cancelButton = buttonBox->addButton("Huy Bo", QDialogButtonBox::RejectRole);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &AddContractDialog::onAccept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Thêm Form và Nút vào vùng Content
    contentLayout->addLayout(formLayout);
    contentLayout->addSpacing(15);
    contentLayout->addWidget(buttonBox, 0, Qt::AlignRight); // Đẩy nút về bên phải

    // Thêm Header và Content vào Layout chính
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

void AddContractDialog::loadContractData()
{
    soHopDongEdit->setText(QString::fromStdString(currentContract.getSoHopDong()));
    QDate date = QDate::fromString(QString::fromStdString(currentContract.getNgayKy()), "dd/MM/yyyy");
    if (date.isValid()) {
        ngayKyEdit->setDate(date);
    }
    loaiHopDongEdit->setText(QString::fromStdString(currentContract.getLoaiHopDong()));
    thoiGianHopDongEdit->setText(QString::fromStdString(currentContract.getThoiGianHopDong()));
}

void AddContractDialog::onAccept()
{
    string soHD = trim(soHopDongEdit->text().toStdString());
    if (soHD.empty()) {
        QMessageBox::warning(this, "Loi", "So Hop Dong la bat buoc.");
        return;
    }
    accept();
}

HopDong AddContractDialog::getContract() const
{
    HopDong hd = currentContract;
    hd.setSoHopDong(trim(soHopDongEdit->text().toStdString()));
    hd.setNgayKy(ngayKyEdit->date().toString("dd/MM/yyyy").toStdString());
    hd.setLoaiHopDong(trim(loaiHopDongEdit->text().toStdString()));
    hd.setThoiGianHopDong(trim(thoiGianHopDongEdit->text().toStdString()));
    return hd;
}
