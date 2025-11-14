#include "employeeviewdialog.h"
#include "addemployeedialog.h"
#include <QTabWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QMenu>
#include <QLocale>
#include <QProcess>
#include <QApplication>

EmployeeViewDialog::EmployeeViewDialog(NhanVien& nv, DanhSachNhanVien& dsnv, QWidget *parent)
    : QDialog(parent), currentEmployee(nv), dsnv(dsnv)
{
    setWindowTitle("Thong Tin Nhan Vien: " + QString::fromStdString(currentEmployee.getHoTen()));
    resize(750, 500);

    // --- 1. Thanh tiêu đề (Header) ---
    QWidget *titleHeaderContainer = new QWidget(this);
    titleHeaderContainer->setObjectName("titleHeaderContainer");
    QHBoxLayout *titleLayout = new QHBoxLayout(titleHeaderContainer);

    QLabel *titleLabel = new QLabel("<h2>Thong Tin Nhan Vien</h2>");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);

    profileNameLabel = new QLabel(this);
    profileNameLabel->setObjectName("profileNameLabel");
    profileNameLabel->setText(QString::fromStdString(currentEmployee.getHoTen()));

    profileButton = new QPushButton(this);
    profileButton->setObjectName("profileButton");

    if(!currentEmployee.getHoTen().empty()) {
        QString qFullName = QString::fromStdString(currentEmployee.getHoTen()).trimmed();
        QStringList parts = qFullName.split(' ', Qt::SkipEmptyParts);
        if (!parts.isEmpty()) {
            profileButton->setText(parts.last().at(0).toUpper());
        } else {
            profileButton->setText("?");
        }
    } else {
        profileButton->setText("?");
    }

    profileMenu = new QMenu(this);
    QAction *viewInfoAction = profileMenu->addAction("Chinh sua thong tin");
    QAction *logoutAction = profileMenu->addAction("Dang xuat");
    profileButton->setMenu(profileMenu);

    connect(viewInfoAction, &QAction::triggered, this, &EmployeeViewDialog::onViewInfo);
    connect(logoutAction, &QAction::triggered, this, &EmployeeViewDialog::onLogout);

    titleLayout->addWidget(titleLabel, 1, Qt::AlignCenter);
    titleLayout->addWidget(profileNameLabel);
    titleLayout->addWidget(profileButton, 0, Qt::AlignRight);
    titleLayout->setContentsMargins(10, 0, 10, 0);

    tabWidget = new QTabWidget(this);
    tabWidget->addTab(createInfoTab(), "Thong Tin Chung");
    tabWidget->addTab(createContractTab(), "Hop Dong");
    tabWidget->addTab(createSalaryTab(), "Lich Su Luong");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleHeaderContainer);
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);

    QString qss = R"(
        QDialog {
            background-color: #f0f2f5;
            font-family: Arial, sans-serif;
        }
        QWidget#titleHeaderContainer {
            background-color: #34495e;
            padding: 5px;
            border-bottom: 1px solid #2c3e50;
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
            background-color: #fd7e14; /* Màu cam */
            font-size: 16px;
            font-weight: bold;
            border: 2px solid white;
            min-width: 32px;
            max-width: 32px;
            min-height: 32px;
            max-height: 32px;
            border-radius: 18px;
        }
        QPushButton#profileButton::menu-indicator { image: none; }
        QMenu {
            background-color: white;
            border: 1px solid #ccc;
            padding: 5px;
            font-size: 13px;
        }
        QMenu::item { padding: 8px 25px 8px 20px; }
        QMenu::item:selected { background-color: #f0f0f0; }
        QTabWidget::pane {
            border: 1px solid #ccc;
            background-color: white;
            border-radius: 5px;
            padding: 10px;
        }
        QTabBar::tab {
            background-color: #e0e0e0;
            color: #444;
            padding: 10px 20px;
            border-top-left-radius: 5px;
            border-top-right-radius: 5px;
            border: 1px solid #ccc;
            border-bottom: none;
            font-weight: bold;
        }
        QTabBar::tab:selected {
            background-color: #34495e;
            color: white;
            border: 1px solid #2c3e50;
            border-bottom: 2px solid #34495e;
        }
        QTableWidget {
            border: 1px solid #e8e8e8;
            gridline-color: #f0f0f0;
            font-size: 13px;
        }
        QHeaderView::section {
            background-color: #34495e;
            color: white;
            padding: 10px 8px;
            font-weight: bold;
            font-size: 13px;
            border: 1px solid #4a627a;
            border-top: none;
            border-left: none;
        }
        QHeaderView::section:last { border-right: none; }
        QTableWidget::item { padding: 8px; }
        QTableWidget::item:selected {
            background-color: #e6f7ff;
            color: #000;
        }
    )";
    this->setStyleSheet(qss);
}

void EmployeeViewDialog::onViewInfo()
{
    AddEmployeeDialog dialog(dsnv, currentEmployee.getMaNhanVien(), this);
    if (dialog.exec() == QDialog::Accepted) {
        NhanVien updatedNv = dialog.getNhanVien();
        dsnv.updateNhanVien(updatedNv);

        currentEmployee = *dsnv.findNhanVienByMa(updatedNv.getMaNhanVien());

        profileNameLabel->setText(QString::fromStdString(currentEmployee.getHoTen()));

        if(!currentEmployee.getHoTen().empty()) {
            QString qFullName = QString::fromStdString(currentEmployee.getHoTen()).trimmed();
            QStringList parts = qFullName.split(' ', Qt::SkipEmptyParts);
            if (!parts.isEmpty()) {
                profileButton->setText(parts.last().at(0).toUpper());
            } else {
                profileButton->setText("?");
            }
        }

        delete tabWidget->widget(0);
        tabWidget->insertTab(0, createInfoTab(), "Thong Tin Chung");
        tabWidget->setCurrentIndex(0);
        setWindowTitle("Thong Tin Nhan Vien: " + QString::fromStdString(currentEmployee.getHoTen()));
    }
}

// --- SỬA LỖI: Đóng cửa sổ trước khi restart ---
void EmployeeViewDialog::onLogout()
{
    // 1. Đóng cửa sổ hiện tại ngay lập tức
    this->close();

    // 2. Khởi động tiến trình mới
    QString appPath = QApplication::applicationFilePath();
    QProcess::startDetached(appPath, QApplication::arguments());

    // 3. Thoát tiến trình cũ
    qApp->quit();
}
// -------------------------------------------

void EmployeeViewDialog::setupTableWidget(QTableWidget *table, const QStringList &headers)
{
    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setAlternatingRowColors(true);
}

void EmployeeViewDialog::styleTableCell(QTableWidgetItem *item, bool isBold, const QColor &color, bool alignCenter)
{
    if (isBold) {
        QFont font = item->font();
        font.setBold(true);
        item->setFont(font);
    }
    item->setForeground(color);
    if (alignCenter) {
        item->setTextAlignment(Qt::AlignCenter);
    } else {
        item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    }
}

QWidget* EmployeeViewDialog::createInfoTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(0,0,0,0);

    infoTableWidget = new QTableWidget(this);
    infoTableWidget->setColumnCount(2);
    infoTableWidget->setRowCount(0);

    infoTableWidget->horizontalHeader()->setVisible(false);
    infoTableWidget->verticalHeader()->setVisible(false);

    infoTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    infoTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    infoTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    infoTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    infoTableWidget->setAlternatingRowColors(true);

    auto addInfoRow = [&](const QString &key, const QString &value) {
        int row = infoTableWidget->rowCount();
        infoTableWidget->insertRow(row);

        QTableWidgetItem *keyItem = new QTableWidgetItem(key);
        QTableWidgetItem *valueItem = new QTableWidgetItem(value);

        styleTableCell(keyItem, true, QColor(0, 92, 178));
        styleTableCell(valueItem, false, Qt::black);

        infoTableWidget->setItem(row, 0, keyItem);
        infoTableWidget->setItem(row, 1, valueItem);
    };

    auto addSeparatorRow = [&](const QString &text) {
        int row = infoTableWidget->rowCount();
        infoTableWidget->insertRow(row);

        QTableWidgetItem *separatorItem = new QTableWidgetItem(text);
        separatorItem->setBackground(QColor(52, 73, 94));
        separatorItem->setForeground(QColor(Qt::white));
        QFont font;
        font.setBold(true);
        separatorItem->setFont(font);
        separatorItem->setTextAlignment(Qt::AlignCenter);

        infoTableWidget->setItem(row, 0, separatorItem);
        infoTableWidget->setSpan(row, 0, 1, 2);
    };

    addInfoRow("Ma Nhan Vien:", QString::fromStdString(currentEmployee.getMaNhanVien()));
    addInfoRow("Ho Ten:", QString::fromStdString(currentEmployee.getHoTen()));
    addInfoRow("Gioi Tinh:", QString::fromStdString(currentEmployee.getGioiTinh()));
    addInfoRow("Ngay Sinh:", QString::fromStdString(currentEmployee.getNgaySinh()));
    addInfoRow("Noi O Hien Tai:", QString::fromStdString(currentEmployee.getNoiOHienTai()));
    addInfoRow("Dien Thoai:", QString::fromStdString(currentEmployee.getDienThoai()));
    addInfoRow("CMND/CCCD:", QString::fromStdString(currentEmployee.getCmnd()));

    addSeparatorRow("--- Cong Viec ---");

    addInfoRow("Phong Ban:", QString::fromStdString(currentEmployee.getPhongBan().getTenPhongBan()));
    addInfoRow("Chuc Vu:", QString::fromStdString(currentEmployee.getChucVu().getTenChucVu()));

    addSeparatorRow("--- Tai Khoan ---");

    addInfoRow("Username:", QString::fromStdString(currentEmployee.getTaiKhoan().getUsername()));

    layout->addWidget(infoTableWidget);
    return tab;
}

QWidget* EmployeeViewDialog::createContractTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(0,0,0,0);

    contractTableWidget = new QTableWidget(this);
    setupTableWidget(contractTableWidget, {"So Hop Dong", "Ngay Ky", "Loai Hop Dong", "Thoi Gian"});
    contractTableWidget->setColumnWidth(0, 120);
    contractTableWidget->setColumnWidth(1, 100);
    contractTableWidget->setColumnWidth(2, 200);

    for (const auto& hd : currentEmployee.getDSHopDong()) {
        int row = contractTableWidget->rowCount();
        contractTableWidget->insertRow(row);

        QTableWidgetItem *soHdItem = new QTableWidgetItem(QString::fromStdString(hd.getSoHopDong()));
        QTableWidgetItem *ngayKyItem = new QTableWidgetItem(QString::fromStdString(hd.getNgayKy()));
        QTableWidgetItem *loaiHdItem = new QTableWidgetItem(QString::fromStdString(hd.getLoaiHopDong()));
        QTableWidgetItem *thoiGianItem = new QTableWidgetItem(QString::fromStdString(hd.getThoiGianHopDong()));

        styleTableCell(soHdItem, true, QColor(0, 92, 178), true);
        styleTableCell(ngayKyItem, false, Qt::black, true);
        styleTableCell(loaiHdItem);
        styleTableCell(thoiGianItem, false, Qt::black, true);

        contractTableWidget->setItem(row, 0, soHdItem);
        contractTableWidget->setItem(row, 1, ngayKyItem);
        contractTableWidget->setItem(row, 2, loaiHdItem);
        contractTableWidget->setItem(row, 3, thoiGianItem);
    }

    layout->addWidget(contractTableWidget);
    return tab;
}

QWidget* EmployeeViewDialog::createSalaryTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(0,0,0,0);

    salaryTableWidget = new QTableWidget(this);
    setupTableWidget(salaryTableWidget, {"Thang", "Luong Co Ban", "Thuong", "Phu Cap", "Thuc Lanh"});

    QLocale vietLocale(QLocale::Vietnamese, QLocale::Vietnam);
    for (const auto& luong : currentEmployee.getDSLuong()) {
        int row = salaryTableWidget->rowCount();
        salaryTableWidget->insertRow(row);

        QTableWidgetItem *thangItem = new QTableWidgetItem(QString::fromStdString(luong.getThang()));
        QTableWidgetItem *lcbItem = new QTableWidgetItem(vietLocale.toString(luong.getLuongCoBan(), 'f', 0));
        QTableWidgetItem *thuongItem = new QTableWidgetItem(vietLocale.toString(luong.getThuong(), 'f', 0));
        QTableWidgetItem *phuCapItem = new QTableWidgetItem(vietLocale.toString(luong.getPhuCap(), 'f', 0));
        QTableWidgetItem *thucLanhItem = new QTableWidgetItem(vietLocale.toString(luong.getThucLanh(), 'f', 0));

        styleTableCell(thangItem, true, QColor(0, 92, 178), true);
        styleTableCell(lcbItem, false, Qt::black, true);
        styleTableCell(thuongItem, false, Qt::black, true);
        styleTableCell(phuCapItem, false, Qt::black, true);
        styleTableCell(thucLanhItem, true, QColor(56, 142, 60), true);

        salaryTableWidget->setItem(row, 0, thangItem);
        salaryTableWidget->setItem(row, 1, lcbItem);
        salaryTableWidget->setItem(row, 2, thuongItem);
        salaryTableWidget->setItem(row, 3, phuCapItem);
        salaryTableWidget->setItem(row, 4, thucLanhItem);
    }

    layout->addWidget(salaryTableWidget);
    return tab;
}
