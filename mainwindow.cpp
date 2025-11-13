#include "mainwindow.h"
#include "addemployeedialog.h"
#include "reportdialog.h"
#include "employeedetaildialog.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QProcess>
#include <QApplication>
#include <QMenu>

MainWindow::MainWindow(DanhSachNhanVien& dsnv, NhanVien* user, QWidget *parent)
    : QMainWindow(parent), loggedInUser(user), dsnv(dsnv)
{
    setWindowTitle("He Thong Quan Ly Nhan Su (Admin)");
    resize(900, 700);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setObjectName("centralWidget");

    // --- 1. Thanh tiêu đề (Title Bar) ---
    QWidget *titleHeaderContainer = new QWidget(this);
    titleHeaderContainer->setObjectName("titleHeaderContainer");
    QHBoxLayout *titleLayout = new QHBoxLayout(titleHeaderContainer);

    titleLabel = new QLabel("<h2>Danh Sach Nhan Vien</h2>");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);

    profileNameLabel = new QLabel(this);
    profileNameLabel->setObjectName("profileNameLabel");
    if(loggedInUser) {
        profileNameLabel->setText(QString::fromStdString(loggedInUser->getHoTen()));
    }

    profileButton = new QPushButton(this);
    profileButton->setObjectName("profileButton");

    // --- THAY ĐỔI: Logic lấy chữ cái đầu của TÊN ---
    if(loggedInUser && !loggedInUser->getHoTen().empty()) {
        QString qFullName = QString::fromStdString(loggedInUser->getHoTen()).trimmed();
        // Bỏ phần (Chức vụ) nếu có
        int parenthesis = qFullName.indexOf('(');
        if (parenthesis != -1) {
            qFullName = qFullName.left(parenthesis).trimmed();
        }
        // Tách tên
        QStringList parts = qFullName.split(' ', Qt::SkipEmptyParts);
        if (!parts.isEmpty()) {
            profileButton->setText(parts.last().at(0).toUpper()); // Lấy chữ cái đầu của từ cuối cùng
        } else {
            profileButton->setText("?");
        }
    } else {
        profileButton->setText("?");
    }
    // ------------------------------------------

    profileMenu = new QMenu(this);
    QAction *viewInfoAction = profileMenu->addAction("Xem thong tin");
    QAction *logoutAction = profileMenu->addAction("Dang xuat");
    profileButton->setMenu(profileMenu);

    connect(viewInfoAction, &QAction::triggered, this, &MainWindow::onViewInfo);
    connect(logoutAction, &QAction::triggered, this, &MainWindow::onLogout);

    titleLayout->addWidget(titleLabel, 1, Qt::AlignCenter);
    titleLayout->addWidget(profileNameLabel);
    titleLayout->addWidget(profileButton, 0, Qt::AlignRight);
    titleLayout->setContentsMargins(10, 0, 10, 0);

    // 2. Thanh tìm kiếm
    searchWidgetContainer = new QWidget(this);
    searchWidgetContainer->setObjectName("searchWidgetContainer");
    searchLayout = new QHBoxLayout(searchWidgetContainer);

    searchLabel = new QLabel("<strong>Tim kiem:</strong>");
    searchLabel->setObjectName("searchLabel");

    searchTypeCombo = new QComboBox(this);
    searchTypeCombo->addItems({"Tim theo Ten", "Tim theo Ma Nhan Vien", "Tim theo Phong Ban"});

    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("Nhap tu khoa tim kiem...");

    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchTypeCombo, 1);
    searchLayout->addWidget(searchEdit, 3);
    searchLayout->setContentsMargins(15, 10, 15, 10);

    // 3. Bảng
    employeeTableWidget = new QTableWidget(this);
    employeeTableWidget->setColumnCount(4);
    QStringList headers = {"Ma Nhan Vien", "Ho Ten", "Chuc Vu", "Phong Ban"};
    employeeTableWidget->setHorizontalHeaderLabels(headers);
    employeeTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    employeeTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    employeeTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    employeeTableWidget->verticalHeader()->setVisible(false);
    employeeTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 4. Thanh nút bấm
    bottomWidgetContainer = new QWidget(this);
    bottomWidgetContainer->setObjectName("bottomWidgetContainer");
    buttonLayout = new QHBoxLayout(bottomWidgetContainer);

    addButton = new QPushButton("Them Nhan Vien", this);
    editButton = new QPushButton("Sua Thong Tin", this);
    deleteButton = new QPushButton("Xoa Nhan Vien", this);
    reportButton = new QPushButton("Bao Cao Thong Ke", this);
    detailButton = new QPushButton("Quan Ly Chi Tiet...", this);
    saveButton = new QPushButton("Luu Du Lieu Vao File", this);

    addButton->setObjectName("addButton");
    editButton->setObjectName("editButton");
    deleteButton->setObjectName("deleteButton");
    reportButton->setObjectName("reportButton");
    detailButton->setObjectName("detailButton");
    saveButton->setObjectName("saveButton");

    editButton->setEnabled(false);
    deleteButton->setEnabled(false);
    detailButton->setEnabled(false);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(reportButton);
    buttonLayout->addWidget(detailButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);
    buttonLayout->setContentsMargins(10, 10, 10, 10);

    // 5. Layout chính
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(titleHeaderContainer);
    mainLayout->addWidget(searchWidgetContainer);
    mainLayout->addWidget(employeeTableWidget);
    mainLayout->addWidget(bottomWidgetContainer);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    // 6. Kết nối signals và slots
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddEmployee);
    connect(editButton, &QPushButton::clicked, this, &MainWindow::onEditEmployee);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteEmployee);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveData);
    connect(employeeTableWidget, &QTableWidget::cellClicked, this, &MainWindow::onEmployeeSelected);
    connect(reportButton, &QPushButton::clicked, this, &MainWindow::onShowReports);
    connect(detailButton, &QPushButton::clicked, this, &MainWindow::onShowDetails);
    connect(searchEdit, &QLineEdit::textChanged, this, &MainWindow::filterEmployeeList);
    connect(searchTypeCombo, &QComboBox::currentIndexChanged, this, &MainWindow::filterEmployeeList);

    // 7. ÁP DỤNG QSS (STYLE SHEET)
    this->setStyleSheet(R"(
        QWidget#centralWidget {
            background-color: #ffffff;
            font-family: Arial, sans-serif;
        }
        QWidget#titleHeaderContainer {
            background-color: #28a745;
            padding: 5px;
            border-top-left-radius: 8px;
            border-top-right-radius: 8px;
            border-bottom: 1px solid #218838;
        }
        QLabel#titleLabel h2 {
            color: white;
            margin: 0;
            padding: 0;
        }
        QLabel#profileNameLabel {
            color: white;
            font-weight: bold;
            font-size: 14px;
            padding-right: 10px;
        }
        QPushButton#profileButton {
            color: white;
            background-color: #dc3545; /* Màu đỏ */
            font-size: 16px;
            font-weight: bold;
            border: 2px solid white;
            min-width: 32px;
            max-width: 32px;
            min-height: 32px;
            max-height: 32px;
            border-radius: 18px;
        }
        QPushButton#profileButton::menu-indicator {
            image: none;
        }
        QMenu {
            background-color: white;
            border: 1px solid #ccc;
            padding: 5px;
            font-size: 13px;
        }
        QMenu::item {
            padding: 8px 25px 8px 20px;
        }
        QMenu::item:selected {
            background-color: #f0f0f0;
        }
        QWidget#searchWidgetContainer {
            background-color: #f5f7fa;
            border-radius: 8px;
        }
        QLabel#searchLabel {
            font-size: 13px;
            font-weight: bold;
            color: #333;
        }
        QLineEdit, QComboBox {
            padding: 8px 10px;
            border: 1px solid #dcdfe6;
            border-radius: 5px;
            background-color: white;
            font-size: 13px;
            color: #333;
        }
        QLineEdit:focus, QComboBox:focus {
            border: 1px solid #007bff;
        }
        QComboBox::drop-down { border: none; padding-right: 5px; }
        QTableWidget {
            border: 1px solid #e8e8e8;
            gridline-color: #f0f0f0;
            font-size: 13px;
            color: #333;
        }
        QHeaderView::section {
            background-color: #34495e;
            color: white;
            padding: 10px 8px;
            font-weight: bold;
            font-size: 13px;
            border: none;
        }
        QTableWidget::item { padding: 8px; }
        QTableWidget::item:selected {
            background-color: #e6f7ff;
            color: #000;
        }
        QWidget#bottomWidgetContainer {
            background-color: #f5f7fa;
            border-top: 1px solid #e8e8e8;
        }
        QPushButton {
            padding: 9px 18px;
            font-size: 13px;
            font-weight: bold;
            border-radius: 5px;
            border: none;
            color: white;
        }
        QPushButton#addButton    { background-color: #28a745; }
        QPushButton#editButton   { background-color: #ffc107; color: black; }
        QPushButton#deleteButton { background-color: #dc3545; }
        QPushButton#reportButton { background-color: #17a2b8; }
        QPushButton#detailButton { background-color: #6c757d; }
        QPushButton#saveButton   { background-color: #007bff; }

        QPushButton#addButton:hover    { background-color: #218838; }
        QPushButton#editButton:hover   { background-color: #e0a800; }
        QPushButton#deleteButton:hover { background-color: #c82333; }
        QPushButton#reportButton:hover { background-color: #138496; }
        QPushButton#detailButton:hover { background-color: #5a6268; }
        QPushButton#saveButton:hover   { background-color: #0069d9; }

        QPushButton:disabled {
            background-color: #ced4da;
            color: #6c757d;
        }
    )");

    populateEmployeeList();
}

MainWindow::~MainWindow() {}

void MainWindow::onViewInfo()
{
    if (loggedInUser) {
        AddEmployeeDialog dialog(dsnv, loggedInUser->getMaNhanVien(), this);
        if (dialog.exec() == QDialog::Accepted) {
            NhanVien updatedNv = dialog.getNhanVien();
            dsnv.updateNhanVien(updatedNv);

            *loggedInUser = updatedNv;

            profileNameLabel->setText(QString::fromStdString(loggedInUser->getHoTen()));

            // --- THAY ĐỔI: Logic lấy chữ cái đầu của TÊN ---
            if(!loggedInUser->getHoTen().empty()) {
                QString qFullName = QString::fromStdString(loggedInUser->getHoTen()).trimmed();
                int parenthesis = qFullName.indexOf('(');
                if (parenthesis != -1) {
                    qFullName = qFullName.left(parenthesis).trimmed();
                }
                QStringList parts = qFullName.split(' ', Qt::SkipEmptyParts);
                if (!parts.isEmpty()) {
                    profileButton->setText(parts.last().at(0).toUpper());
                } else {
                    profileButton->setText("?");
                }
            }
            // ------------------------------------------

            populateEmployeeList();
        }
    }
}

void MainWindow::onLogout()
{
    QString appPath = QApplication::applicationFilePath();
    QProcess::startDetached(appPath, QApplication::arguments());
    qApp->quit();
}

void MainWindow::filterEmployeeList()
{
    QString query = searchEdit->text().trimmed().toLower();
    int searchTypeIndex = searchTypeCombo->currentIndex();
    int columnToSearch;

    switch (searchTypeIndex) {
    case 0: columnToSearch = 1; break;
    case 1: columnToSearch = 0; break;
    case 2: columnToSearch = 3; break;
    default: columnToSearch = 1;
    }

    for (int i = 0; i < employeeTableWidget->rowCount(); ++i)
    {
        if (query.isEmpty()) {
            employeeTableWidget->setRowHidden(i, false);
            continue;
        }
        QTableWidgetItem *item = employeeTableWidget->item(i, columnToSearch);
        if (item) {
            QString cellText = item->text().toLower();
            bool match = cellText.contains(query);
            employeeTableWidget->setRowHidden(i, !match);
        }
    }
}

void MainWindow::populateEmployeeList()
{
    employeeTableWidget->setRowCount(0);

    for (const auto& nv_ptr : dsnv.getDanhSach()) {
        int row = employeeTableWidget->rowCount();
        employeeTableWidget->insertRow(row);

        QTableWidgetItem *maNvItem = new QTableWidgetItem(QString::fromStdString(nv_ptr->getMaNhanVien()));
        QTableWidgetItem *hoTenItem = new QTableWidgetItem(QString::fromStdString(nv_ptr->getHoTen()));
        QTableWidgetItem *chucVuItem = new QTableWidgetItem(QString::fromStdString(nv_ptr->getChucVu().getTenChucVu()));
        QTableWidgetItem *phongBanItem = new QTableWidgetItem(QString::fromStdString(nv_ptr->getPhongBan().getTenPhongBan()));

        maNvItem->setForeground(QColor(0, 92, 178));
        hoTenItem->setForeground(QColor(Qt::black));
        chucVuItem->setForeground(QColor(56, 142, 60));
        phongBanItem->setForeground(QColor(106, 27, 154));

        maNvItem->setTextAlignment(Qt::AlignCenter);
        hoTenItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        chucVuItem->setTextAlignment(Qt::AlignCenter);
        phongBanItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);

        employeeTableWidget->setItem(row, 0, maNvItem);
        employeeTableWidget->setItem(row, 1, hoTenItem);
        employeeTableWidget->setItem(row, 2, chucVuItem);
        employeeTableWidget->setItem(row, 3, phongBanItem);
    }

    editButton->setEnabled(false);
    deleteButton->setEnabled(false);
    detailButton->setEnabled(false);
}

void MainWindow::onAddEmployee()
{
    AddEmployeeDialog dialog(dsnv, "", this);
    if (dialog.exec() == QDialog::Accepted) {
        NhanVien nv = dialog.getNhanVien();
        dsnv.addNhanVien(nv);
        populateEmployeeList();
        filterEmployeeList();
    }
}

void MainWindow::onEditEmployee()
{
    int selectedRow = employeeTableWidget->currentRow();
    if (selectedRow < 0) return;

    string maNV = employeeTableWidget->item(selectedRow, 0)->text().toStdString();

    AddEmployeeDialog dialog(dsnv, maNV, this);
    if (dialog.exec() == QDialog::Accepted) {
        NhanVien updatedNv = dialog.getNhanVien();
        dsnv.updateNhanVien(updatedNv);
        populateEmployeeList();
        filterEmployeeList();
    }
}

void MainWindow::onDeleteEmployee()
{
    int selectedRow = employeeTableWidget->currentRow();
    if (selectedRow < 0) return;

    string maNV = employeeTableWidget->item(selectedRow, 0)->text().toStdString();
    string tenNV = employeeTableWidget->item(selectedRow, 1)->text().toStdString();
    string chucVu = employeeTableWidget->item(selectedRow, 2)->text().toStdString();
    string phongBan = employeeTableWidget->item(selectedRow, 3)->text().toStdString();

    QString text = QString(R"(
        <p style='font-size: 14px;'>Ban co chac chan muon xoa nhan vien nay?</p>
        <table style='width: 100%; margin-top: 10px; border: 1px solid #ccc; border-collapse: collapse;'>
            <tr style='background-color: #f0f0f0;'>
                <td style='padding: 5px; border: 1px solid #ccc; font-weight: bold; color: #005cb2;'>Ma NV:</td>
                <td style='padding: 5px; border: 1px solid #ccc; color: #005cb2;'>%1</td>
            </tr>
            <tr>
                <td style='padding: 5px; border: 1px solid #ccc; font-weight: bold; color: #000;'>Ho Ten:</td>
                <td style='padding: 5px; border: 1px solid #ccc; color: #000;'>%2</td>
            </tr>
            <tr style='background-color: #f0f0f0;'>
                <td style='padding: 5px; border: 1px solid #ccc; font-weight: bold; color: #388e3c;'>Chuc Vu:</td>
                <td style='padding: 5px; border: 1px solid #ccc; color: #388e3c;'>%3</td>
            </tr>
            <tr>
                <td style='padding: 5px; border: 1px solid #ccc; font-weight: bold; color: #6a1b9a;'>Phong Ban:</td>
                <td style='padding: 5px; border: 1px solid #ccc; color: #6a1b9a;'>%4</td>
            </tr>
        </table>
    )").arg(QString::fromStdString(maNV),
                            QString::fromStdString(tenNV),
                            QString::fromStdString(chucVu),
                            QString::fromStdString(phongBan));

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Xac nhan Xoa");
    msgBox.setText(text);
    msgBox.setIcon(QMessageBox::Question);
    QPushButton *yesButton = msgBox.addButton("Dong Y Xoa", QMessageBox::YesRole);
    QPushButton *noButton = msgBox.addButton("Huy Bo", QMessageBox::NoRole);
    noButton->setFocus();
    msgBox.setStyleSheet(R"(
        QMessageBox { background-color: #f0f2f5; font-size: 14px; width: 350px; }
        QLabel#qt_msgbox_label { color: #333; }
        QAbstractButton {
            padding: 10px 28px; font-size: 14px; font-weight: bold;
            border-radius: 5px; border: none; color: white; min-width: 100px;
        }
        QAbstractButton[text='Dong Y Xoa'] { background-color: #dc3545; }
        QAbstractButton[text='Dong Y Xoa']:hover { background-color: #c82333; }
        QAbstractButton[text='Huy Bo'] { background-color: #6c757d; }
        QAbstractButton[text='Huy Bo']:hover { background-color: #5a6268; }
    )");
    msgBox.exec();

    if (msgBox.clickedButton() == yesButton) {
        if (dsnv.deleteNhanVien(maNV)) {
            populateEmployeeList();
            filterEmployeeList();
        } else {
            QMessageBox errorBox(this);
            errorBox.setWindowTitle("Loi");
            errorBox.setText("Khong the xoa nhan vien.");
            errorBox.setIcon(QMessageBox::Critical);
            errorBox.addButton("OK", QMessageBox::AcceptRole);
            errorBox.setStyleSheet("QMessageBox { background-color: #f0f2f5; font-size: 14px; } QAbstractButton { padding: 10px 28px; background-color: #007bff; color: white; border-radius: 5px; min-width: 100px; }");
            errorBox.exec();
        }
    }
}

void MainWindow::onSaveData()
{
    dsnv.SaveToFile("danhsachnhanvien.txt");

    QMessageBox infoBox(this);
    infoBox.setWindowTitle("Thanh cong");
    infoBox.setText("Da luu du lieu vao file danhsachnhanvien.txt");
    infoBox.setIcon(QMessageBox::Information);
    infoBox.addButton("OK", QMessageBox::AcceptRole);
    infoBox.setStyleSheet(R"(
        QMessageBox { background-color: #f0f2f5; font-family: Arial, sans-serif; font-size: 14px; }
        QLabel#qt_msgbox_label { color: #333; }
        QAbstractButton {
            padding: 10px 28px; font-size: 14px; font-weight: bold;
            border-radius: 5px; border: none; color: white;
            min-width: 100px; background-color: #007bff;
        }
        QAbstractButton:hover { background-color: #0069d9; }
    )");
    infoBox.exec();
}

void MainWindow::onShowReports()
{
    ReportDialog dialog(dsnv, this);
    dialog.exec();
}

void MainWindow::onEmployeeSelected(int row, int /*column*/)
{
    if (row >= 0) {
        editButton->setEnabled(true);
        deleteButton->setEnabled(true);
        detailButton->setEnabled(true);
    }
}

void MainWindow::onShowDetails()
{
    int selectedRow = employeeTableWidget->currentRow();
    if (selectedRow < 0) return;

    string maNV = employeeTableWidget->item(selectedRow, 0)->text().toStdString();
    NhanVien* nv = dsnv.findNhanVienByMa(maNV);

    if (nv) {
        EmployeeDetailDialog dialog(nv, this);
        dialog.exec();
    } else {
        QMessageBox::critical(this, "Loi", "Khong tim thay du lieu nhan vien.");
    }
}
