#include "reportdialog.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLocale>
#include <QTableWidget>
#include <QHeaderView>
#include <QFont>
#include <algorithm>

ReportDialog::ReportDialog(DanhSachNhanVien& dsnv, QWidget *parent)
    : QDialog(parent), dsnv(dsnv)
{
    setWindowTitle("Bao Cao Thong Ke");
    resize(750, 500);

    tabWidget = new QTabWidget(this);

    salaryReportTable = new QTableWidget(this);
    genderReportTable = new QTableWidget(this);

    // Cài đặt bảng Lương
    setupTableWidget(salaryReportTable,
                     {"Phong Ban", "So Luong NV", "Tong Luong", "Luong Trung Binh"});
    salaryReportTable->setColumnWidth(0, 200);

    // Cài đặt bảng Giới tính
    setupTableWidget(genderReportTable,
                     {"Phong Ban", "Nam", "Nu", "Khac", "Tong Cong"});

    tabWidget->addTab(salaryReportTable, "Thong Ke Luong (Theo Phong Ban)");
    tabWidget->addTab(genderReportTable, "Thong Ke Nhan Vien (Theo PB & Gioi Tinh)");

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    QString qss = R"(
        QDialog {
            background-color: #f0f2f5;
            font-family: Arial, sans-serif;
        }
        QTabWidget::pane {
            border: 1px solid #ccc;
            background-color: white;
            border-radius: 5px;
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
            border: none;
        }
        QTableWidget::item { padding: 8px; }
        QTableWidget::item:selected {
            background-color: #e6f7ff;
            color: #000;
        }
        QPushButton {
            padding: 10px 28px;
            font-size: 14px;
            font-weight: bold;
            border-radius: 5px;
            border: none;
            color: white;
            min-width: 100px;
            background-color: #6c757d;
        }
        QPushButton:hover {
            background-color: #5a6268;
        }
    )";
    this->setStyleSheet(qss);

    generateSalaryReport();
    generateGenderReport();
}

void ReportDialog::setupTableWidget(QTableWidget *table, const QStringList &headers)
{
    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setAlternatingRowColors(true);
}

// --- THAY ĐỔI: Luôn căn giữa (AlignCenter) ---
void ReportDialog::styleTableCell(QTableWidgetItem *item, bool isBold, const QColor &color)
{
    if (isBold) {
        QFont font = item->font();
        font.setBold(true);
        item->setFont(font);
    }
    item->setForeground(color);
    item->setTextAlignment(Qt::AlignCenter); // <-- LUÔN CĂN GIỮA
}

void ReportDialog::generateSalaryReport()
{
    map<string, pair<double, int>> reportData = dsnv.getReportLuongTheoPhongBan();
    salaryReportTable->setRowCount(0);

    QLocale vietLocale(QLocale::Vietnamese, QLocale::Vietnam);
    double grandTotalSalary = 0;
    int grandTotalEmployees = 0;

    for (const auto& entry : reportData) {
        int row = salaryReportTable->rowCount();
        salaryReportTable->insertRow(row);

        string tenPB = entry.first;
        double tongLuong = entry.second.first;
        int soLuong = entry.second.second;
        double luongTB = (soLuong > 0) ? (tongLuong / soLuong) : 0.0;

        grandTotalSalary += tongLuong;
        grandTotalEmployees += soLuong;

        QTableWidgetItem *pbItem = new QTableWidgetItem(QString::fromStdString(tenPB));
        QTableWidgetItem *slItem = new QTableWidgetItem(QString::number(soLuong));
        QTableWidgetItem *tongLuongItem = new QTableWidgetItem(vietLocale.toString(tongLuong, 'f', 0) + " VND");
        QTableWidgetItem *luongTBItem = new QTableWidgetItem(vietLocale.toString(luongTB, 'f', 0) + " VND");

        // --- THAY ĐỔI: Đã bỏ tham số alignRight ---
        styleTableCell(pbItem, true, QColor(106, 27, 154)); // Tím
        styleTableCell(slItem, false, Qt::black);
        styleTableCell(tongLuongItem, false, QColor(0, 92, 178)); // Xanh
        styleTableCell(luongTBItem, true, QColor(56, 142, 60)); // Xanh lá

        salaryReportTable->setItem(row, 0, pbItem);
        salaryReportTable->setItem(row, 1, slItem);
        salaryReportTable->setItem(row, 2, tongLuongItem);
        salaryReportTable->setItem(row, 3, luongTBItem);
    }

    if (grandTotalEmployees > 0) {
        int row = salaryReportTable->rowCount();
        salaryReportTable->insertRow(row);
        double avgTotal = grandTotalSalary / grandTotalEmployees;

        QTableWidgetItem *pbItem = new QTableWidgetItem("TONG CONG TY");
        QTableWidgetItem *slItem = new QTableWidgetItem(QString::number(grandTotalEmployees));
        QTableWidgetItem *tongLuongItem = new QTableWidgetItem(vietLocale.toString(grandTotalSalary, 'f', 0) + " VND");
        QTableWidgetItem *luongTBItem = new QTableWidgetItem(vietLocale.toString(avgTotal, 'f', 0) + " VND");

        // --- THAY ĐỔI: Đã bỏ tham số alignRight ---
        styleTableCell(pbItem, true, Qt::red);
        styleTableCell(slItem, true, Qt::red);
        styleTableCell(tongLuongItem, true, Qt::red);
        styleTableCell(luongTBItem, true, Qt::red);

        salaryReportTable->setItem(row, 0, pbItem);
        salaryReportTable->setItem(row, 1, slItem);
        salaryReportTable->setItem(row, 2, tongLuongItem);
        salaryReportTable->setItem(row, 3, luongTBItem);
    }
}

void ReportDialog::generateGenderReport()
{
    map<string, map<string, int>> reportData = dsnv.getReportTheoPhongBanGioiTinh();
    genderReportTable->setRowCount(0);

    int grandTotalNam = 0;
    int grandTotalNu = 0;
    int grandTotalKhac = 0;
    int grandTotal = 0;

    for (const auto& pbEntry : reportData) {
        int row = genderReportTable->rowCount();
        genderReportTable->insertRow(row);

        string tenPB = pbEntry.first;
        int nam = 0, nu = 0, khac = 0, tong = 0;

        for (const auto& gtEntry : pbEntry.second) {
            string gioiTinh = gtEntry.first;
            int count = gtEntry.second;

            std::transform(gioiTinh.begin(), gioiTinh.end(), gioiTinh.begin(), ::tolower);

            if (gioiTinh == "nam") nam += count;
            else if (gioiTinh == "nu" || gioiTinh == "nữ") nu += count;
            else khac += count;

            tong += count;
        }

        grandTotalNam += nam;
        grandTotalNu += nu;
        grandTotalKhac += khac;
        grandTotal += tong;

        QTableWidgetItem *pbItem = new QTableWidgetItem(QString::fromStdString(tenPB));
        QTableWidgetItem *namItem = new QTableWidgetItem(QString::number(nam));
        QTableWidgetItem *nuItem = new QTableWidgetItem(QString::number(nu));
        QTableWidgetItem *khacItem = new QTableWidgetItem(QString::number(khac));
        QTableWidgetItem *tongItem = new QTableWidgetItem(QString::number(tong));

        // --- THAY ĐỔI: Đã bỏ tham số alignRight ---
        styleTableCell(pbItem, true, QColor(106, 27, 154)); // Tím
        styleTableCell(namItem, false, Qt::black);
        styleTableCell(nuItem, false, Qt::black);
        styleTableCell(khacItem, false, Qt::black);
        styleTableCell(tongItem, true, Qt::black);

        genderReportTable->setItem(row, 0, pbItem);
        genderReportTable->setItem(row, 1, namItem);
        genderReportTable->setItem(row, 2, nuItem);
        genderReportTable->setItem(row, 3, khacItem);
        genderReportTable->setItem(row, 4, tongItem);
    }

    // Thêm dòng tổng cộng
    int row = genderReportTable->rowCount();
    genderReportTable->insertRow(row);

    QTableWidgetItem *pbItem = new QTableWidgetItem("TONG CONG TY");
    QTableWidgetItem *namItem = new QTableWidgetItem(QString::number(grandTotalNam));
    QTableWidgetItem *nuItem = new QTableWidgetItem(QString::number(grandTotalNu));
    QTableWidgetItem *khacItem = new QTableWidgetItem(QString::number(grandTotalKhac));
    QTableWidgetItem *tongItem = new QTableWidgetItem(QString::number(grandTotal));

    // --- THAY ĐỔI: Đã bỏ tham số alignRight ---
    styleTableCell(pbItem, true, Qt::red);
    styleTableCell(namItem, true, Qt::red);
    styleTableCell(nuItem, true, Qt::red);
    styleTableCell(khacItem, true, Qt::red);
    styleTableCell(tongItem, true, Qt::red);

    genderReportTable->setItem(row, 0, pbItem);
    genderReportTable->setItem(row, 1, namItem);
    genderReportTable->setItem(row, 2, nuItem);
    genderReportTable->setItem(row, 3, khacItem);
    genderReportTable->setItem(row, 4, tongItem);
}
