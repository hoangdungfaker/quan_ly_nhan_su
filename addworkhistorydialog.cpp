#include "addworkhistorydialog.h"
#include <QGridLayout> // <-- THÊM MỚI
#include <QLabel>      // <-- THÊM MỚI
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDateEdit>
#include <QPushButton>
#include <QCalendarWidget>

AddWorkHistoryDialog::AddWorkHistoryDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Them Qua Trinh Cong Tac");
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

    ngayVaoLamEdit = new QDateEdit(this);
    ngayVaoLamEdit->setDisplayFormat("dd/MM/yyyy");
    ngayVaoLamEdit->setCalendarPopup(true);
    ngayVaoLamEdit->setDate(QDate::currentDate());

    boPhanEdit = new QLineEdit(this);
    chucVuEdit = new QLineEdit(this);

    formLayout->addWidget(new QLabel("Ngay Vao Lam (*):"), 0, 0, Qt::AlignLeft);
    formLayout->addWidget(ngayVaoLamEdit, 0, 1);
    formLayout->addWidget(new QLabel("Bo Phan:"), 1, 0, Qt::AlignLeft);
    formLayout->addWidget(boPhanEdit, 1, 1);
    formLayout->addWidget(new QLabel("Chuc Vu:"), 2, 0, Qt::AlignLeft);
    formLayout->addWidget(chucVuEdit, 2, 1);

    formLayout->setColumnStretch(1, 1);

    // 4. Nút bấm
    buttonBox = new QDialogButtonBox(this);
    QPushButton *saveButton = buttonBox->addButton("Luu", QDialogButtonBox::AcceptRole);
    QPushButton *cancelButton = buttonBox->addButton("Huy Bo", QDialogButtonBox::RejectRole);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &AddWorkHistoryDialog::onAccept);
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

void AddWorkHistoryDialog::onAccept()
{
    accept();
}

QuaTrinhCongTac AddWorkHistoryDialog::getWorkHistory() const
{
    QuaTrinhCongTac qtct;
    qtct.setNgayVaoLam(ngayVaoLamEdit->date().toString("dd/MM/yyyy").toStdString());
    qtct.setBoPhan(trim(boPhanEdit->text().toStdString()));
    qtct.setChucVu(trim(chucVuEdit->text().toStdString()));
    return qtct;
}
