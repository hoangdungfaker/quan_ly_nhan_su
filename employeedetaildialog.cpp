#include "employeedetaildialog.h"
#include "addcontractdialog.h"
#include "addsalarydialog.h"
#include "addtimekeepingdialog.h"
#include "addworkhistorydialog.h"
#include "addtrainingdialog.h"
#include "addrewarddialog.h"
#include "addleavedialog.h"
#include <QTabWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QLocale>

EmployeeDetailDialog::EmployeeDetailDialog(NhanVien* nv, QWidget *parent)
    : QDialog(parent), currentEmployee(nv)
{
    if (!currentEmployee) {
    	QMessageBox::critical(this, "Loi", "Khong co du lieu nhan vien.");
        reject();
        return;

       
    }

    setWindowTitle("Quan Ly Chi Tiet: " + QString::fromStdString(currentEmployee->getHoTen()));
    resize(850, 550);

    tabWidget = new QTabWidget(this);

    // Tạo các tab
    tabWidget->addTab(createContractTab(), "Quan Ly Hop Dong");
    tabWidget->addTab(createSalaryTab(), "Quan Ly Luong");
    tabWidget->addTab(createTimekeepingTab(), "Quan Ly Cham Cong");
    tabWidget->addTab(createWorkHistoryTab(), "Qua Trinh Cong Tac");
    tabWidget->addTab(createTrainingTab(), "Lich Su Dao Tao");
    tabWidget->addTab(createRewardTab(), "Khen Thuong/Ky Luat");
    tabWidget->addTab(createLeaveTab(), "Quan Ly Nghi Phep");

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    // --- Áp dụng QSS cho toàn bộ dialog ---
    QString qss = R"(
        QDialog {
            background-color: #f0f2f5;
            font-family: Arial, sans-serif;
        }
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
        QTabBar::tab:!selected:hover {
            background-color: #d0d0d0;
        }
        QTableWidget {
            border: 1px solid #e8e8e8;
            gridline-color: #f0f0f0;
            font-size: 13px;
        }

        /* --- THAY ĐỔI: Thêm viền cho Header --- */
        QHeaderView::section {
            background-color: #34495e;
            color: white;
            padding: 10px 8px;
            font-weight: bold;
            font-size: 13px;
            border-top: none;
            border-left: none;
            border-bottom: 1px solid #2c3e50; /* Viền dưới đậm hơn */
            border-right: 1px solid #4a627a; /* Viền phải (phân cách) */
        }
        QHeaderView::section:last {
            border-right: none; /* Bỏ viền của cột cuối cùng */
        }
        /* ------------------------------------- */

        QTableWidget::item { padding: 8px; }
        QTableWidget::item:selected {
            background-color: #e6f7ff;
            color: #000;
        }
        QPushButton {
            padding: 9px 18px;
            font-size: 13px;
            font-weight: bold;
            border-radius: 5px;
            border: none;
            color: white;
        }
        QDialogButtonBox QPushButton {
            background-color: #6c757d;
            min-width: 100px;
        }
        QDialogButtonBox QPushButton:hover {
            background-color: #5a6268;
        }
    )";
    this->setStyleSheet(qss);

    // Tô màu riêng cho các nút "Thêm", "Sửa", "Xóa"
    QString addStyle = "background-color: #28a745;";
    QString editStyle = "background-color: #ffc107; color: black;";
    QString deleteStyle = "background-color: #dc3545;";

    addContractButton->setStyleSheet(addStyle);
    editContractButton->setStyleSheet(editStyle);
    deleteContractButton->setStyleSheet(deleteStyle);

    addSalaryButton->setStyleSheet(addStyle);
    editSalaryButton->setStyleSheet(editStyle);
    deleteSalaryButton->setStyleSheet(deleteStyle);

    addTimekeepingButton->setStyleSheet(addStyle);
    editTimekeepingButton->setStyleSheet(editStyle);
    deleteTimekeepingButton->setStyleSheet(deleteStyle);

    addWorkHistoryButton->setStyleSheet(addStyle);
    editWorkHistoryButton->setStyleSheet(editStyle);
    deleteWorkHistoryButton->setStyleSheet(deleteStyle);

    addTrainingButton->setStyleSheet(addStyle);
    editTrainingButton->setStyleSheet(editStyle);
    deleteTrainingButton->setStyleSheet(deleteStyle);

    addRewardButton->setStyleSheet(addStyle);
    editRewardButton->setStyleSheet(editStyle);
    deleteRewardButton->setStyleSheet(deleteStyle);

    addLeaveButton->setStyleSheet(addStyle);
    editLeaveButton->setStyleSheet(editStyle);
    deleteLeaveButton->setStyleSheet(deleteStyle);

    // Tải dữ liệu
    populateContractList();
    populateSalaryList();
    populateTimekeepingList();
    populateWorkHistoryList();
    populateTrainingList();
    populateRewardList();
    populateLeaveList();
}

// --- HÀM TRỢ GIÚP ---
void EmployeeDetailDialog::setupTableWidget(QTableWidget *table, const QStringList &headers)
{
    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setStretchLastSection(true);
}

void EmployeeDetailDialog::styleTableCell(QTableWidgetItem *item, bool isBold, const QColor &color, bool alignCenter)
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
// ------------------------

// --- Create Tab UI (Đã viết lại) ---
QWidget* EmployeeDetailDialog::createContractTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    contractTableWidget = new QTableWidget(this);

    setupTableWidget(contractTableWidget, {"So Hop Dong", "Ngay Ky", "Loai Hop Dong", "Thoi Gian"});
    contractTableWidget->setColumnWidth(0, 120);
    contractTableWidget->setColumnWidth(1, 100);
    contractTableWidget->setColumnWidth(2, 200);

    addContractButton = new QPushButton("Them Hop Dong", this);
    editContractButton = new QPushButton("Sua Hop Dong", this);
    deleteContractButton = new QPushButton("Xoa Hop Dong", this);
    editContractButton->setEnabled(false);
    deleteContractButton->setEnabled(false);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addContractButton);
    buttonLayout->addWidget(editContractButton);
    buttonLayout->addWidget(deleteContractButton);
    buttonLayout->addStretch();

    layout->addWidget(new QLabel("Danh sach Hop dong:"));
    layout->addWidget(contractTableWidget);
    layout->addLayout(buttonLayout);

    connect(addContractButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onAddContract);
    connect(editContractButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onEditContract);
    connect(deleteContractButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onDeleteContract);
    connect(contractTableWidget, &QTableWidget::cellClicked, this, &EmployeeDetailDialog::onContractSelected);
    return tab;
}

QWidget* EmployeeDetailDialog::createSalaryTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    salaryTableWidget = new QTableWidget(this);

    setupTableWidget(salaryTableWidget, {"Thang", "Luong Co Ban", "Thuong", "Phu Cap", "Tang Ca", "Thuc Lanh"});

    addSalaryButton = new QPushButton("Them Bang Luong", this);
    editSalaryButton = new QPushButton("Sua Bang Luong", this);
    deleteSalaryButton = new QPushButton("Xoa Bang Luong", this);
    editSalaryButton->setEnabled(false);
    deleteSalaryButton->setEnabled(false);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addSalaryButton);
    buttonLayout->addWidget(editSalaryButton);
    buttonLayout->addWidget(deleteSalaryButton);
    buttonLayout->addStretch();

    layout->addWidget(new QLabel("Danh sach Bang Luong (theo thang):"));
    layout->addWidget(salaryTableWidget);
    layout->addLayout(buttonLayout);

    connect(addSalaryButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onAddSalary);
    connect(editSalaryButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onEditSalary);
    connect(deleteSalaryButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onDeleteSalary);
    connect(salaryTableWidget, &QTableWidget::cellClicked, this, &EmployeeDetailDialog::onSalarySelected);
    return tab;
}

QWidget* EmployeeDetailDialog::createTimekeepingTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    timekeepingTableWidget = new QTableWidget(this);

    setupTableWidget(timekeepingTableWidget, {"Ngay Cong", "Phan Ca", "So Gio Them"});
    timekeepingTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    addTimekeepingButton = new QPushButton("Them Ngay Cong", this);
    editTimekeepingButton = new QPushButton("Sua Ngay Cong", this);
    deleteTimekeepingButton = new QPushButton("Xoa Ngay Cong", this);
    editTimekeepingButton->setEnabled(false);
    deleteTimekeepingButton->setEnabled(false);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addTimekeepingButton);
    buttonLayout->addWidget(editTimekeepingButton);
    buttonLayout->addWidget(deleteTimekeepingButton);
    buttonLayout->addStretch();

    layout->addWidget(new QLabel("Danh sach ngay cong:"));
    layout->addWidget(timekeepingTableWidget);
    layout->addLayout(buttonLayout);

    connect(addTimekeepingButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onAddTimekeeping);
    connect(editTimekeepingButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onEditTimekeeping);
    connect(deleteTimekeepingButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onDeleteTimekeeping);
    connect(timekeepingTableWidget, &QTableWidget::cellClicked, this, &EmployeeDetailDialog::onTimekeepingSelected);
    return tab;
}

QWidget* EmployeeDetailDialog::createWorkHistoryTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    workHistoryTableWidget = new QTableWidget(this);

    setupTableWidget(workHistoryTableWidget, {"Ngay Vao Lam", "Bo Phan", "Chuc Vu"});
    workHistoryTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    addWorkHistoryButton = new QPushButton("Them Qua Trinh", this);
    editWorkHistoryButton = new QPushButton("Sua Qua Trinh", this);
    deleteWorkHistoryButton = new QPushButton("Xoa Qua Trinh", this);
    editWorkHistoryButton->setEnabled(false);
    deleteWorkHistoryButton->setEnabled(false);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addWorkHistoryButton);
    buttonLayout->addWidget(editWorkHistoryButton);
    buttonLayout->addWidget(deleteWorkHistoryButton);
    buttonLayout->addStretch();

    layout->addWidget(new QLabel("Danh sach qua trinh cong tac:"));
    layout->addWidget(workHistoryTableWidget);
    layout->addLayout(buttonLayout);

    connect(addWorkHistoryButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onAddWorkHistory);
    connect(editWorkHistoryButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onEditWorkHistory);
    connect(deleteWorkHistoryButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onDeleteWorkHistory);
    connect(workHistoryTableWidget, &QTableWidget::cellClicked, this, &EmployeeDetailDialog::onWorkHistorySelected);
    return tab;
}

QWidget* EmployeeDetailDialog::createTrainingTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    trainingTableWidget = new QTableWidget(this);

    setupTableWidget(trainingTableWidget, {"Ma Khoa Hoc", "Ten Khoa Hoc", "Ngay Bat Dau", "Ngay Ket Thuc", "Don Vi Dao Tao"});
    trainingTableWidget->setColumnWidth(0, 100);
    trainingTableWidget->setColumnWidth(1, 200);
    trainingTableWidget->setColumnWidth(2, 100);
    trainingTableWidget->setColumnWidth(3, 100);

    addTrainingButton = new QPushButton("Them Khoa Hoc", this);
    editTrainingButton = new QPushButton("Sua Khoa Hoc", this);
    deleteTrainingButton = new QPushButton("Xoa Khoa Hoc", this);
    editTrainingButton->setEnabled(false);
    deleteTrainingButton->setEnabled(false);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addTrainingButton);
    buttonLayout->addWidget(editTrainingButton);
    buttonLayout->addWidget(deleteTrainingButton);
    buttonLayout->addStretch();

    layout->addWidget(new QLabel("Danh sach khoa hoc da tham gia:"));
    layout->addWidget(trainingTableWidget);
    layout->addLayout(buttonLayout);

    connect(addTrainingButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onAddTraining);
    connect(editTrainingButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onEditTraining);
    connect(deleteTrainingButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onDeleteTraining);
    connect(trainingTableWidget, &QTableWidget::cellClicked, this, &EmployeeDetailDialog::onTrainingSelected);
    return tab;
}

QWidget* EmployeeDetailDialog::createRewardTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    rewardTableWidget = new QTableWidget(this);

    setupTableWidget(rewardTableWidget, {"Loai", "Ngay", "Noi Dung", "So Quyet Dinh", "So Tien"});
    rewardTableWidget->setColumnWidth(0, 100);
    rewardTableWidget->setColumnWidth(1, 100);
    rewardTableWidget->setColumnWidth(2, 250);

    addRewardButton = new QPushButton("Them Moi", this);
    editRewardButton = new QPushButton("Sua", this);
    deleteRewardButton = new QPushButton("Xoa", this);
    editRewardButton->setEnabled(false);
    deleteRewardButton->setEnabled(false);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addRewardButton);
    buttonLayout->addWidget(editRewardButton);
    buttonLayout->addWidget(deleteRewardButton);
    buttonLayout->addStretch();

    layout->addWidget(new QLabel("Danh sach Khen thuong / Ky luat:"));
    layout->addWidget(rewardTableWidget);
    layout->addLayout(buttonLayout);

    connect(addRewardButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onAddReward);
    connect(editRewardButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onEditReward);
    connect(deleteRewardButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onDeleteReward);
    connect(rewardTableWidget, &QTableWidget::cellClicked, this, &EmployeeDetailDialog::onRewardSelected);
    return tab;
}

QWidget* EmployeeDetailDialog::createLeaveTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    leaveTableWidget = new QTableWidget(this);

    setupTableWidget(leaveTableWidget, {"Loai Phep", "So Ngay", "Ngay Bat Dau", "Ngay Ket Thuc"});
    leaveTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    addLeaveButton = new QPushButton("Them Don Phep", this);
    editLeaveButton = new QPushButton("Sua Don Phep", this);
    deleteLeaveButton = new QPushButton("Xoa Don Phep", this);
    editLeaveButton->setEnabled(false);
    deleteLeaveButton->setEnabled(false);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addLeaveButton);
    buttonLayout->addWidget(editLeaveButton);
    buttonLayout->addWidget(deleteLeaveButton);
    buttonLayout->addStretch();

    layout->addWidget(new QLabel("Danh sach cac lan nghi phep:"));
    layout->addWidget(leaveTableWidget);
    layout->addLayout(buttonLayout);

    connect(addLeaveButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onAddLeave);
    connect(editLeaveButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onEditLeave);
    connect(deleteLeaveButton, &QPushButton::clicked, this, &EmployeeDetailDialog::onDeleteLeave);
    connect(leaveTableWidget, &QTableWidget::cellClicked, this, &EmployeeDetailDialog::onLeaveSelected);
    return tab;
}

// --- HÀM TÙY CHỈNH QMessageBox ---
static int showCustomMessageBox(QWidget *parent, QMessageBox::Icon icon, const QString &title, const QString &text,
                                const QString &yesText, const QString &noText)
{
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setIcon(icon);

    QPushButton *yesButton = msgBox.addButton(yesText, QMessageBox::YesRole);
    QPushButton *noButton = noText.isEmpty() ? nullptr : msgBox.addButton(noText, QMessageBox::NoRole);
    if(noButton) noButton->setFocus();

    QString style = R"(
        QMessageBox { background-color: #f0f2f5; font-size: 14px; }
        QLabel#qt_msgbox_label { color: #333; }
        QAbstractButton {
            padding: 10px 28px; font-size: 14px; font-weight: bold;
            border-radius: 5px; border: none; color: white; min-width: 100px;
        }
    )";

    if(icon == QMessageBox::Question) {
        yesButton->setStyleSheet("background-color: #dc3545;");
        noButton->setStyleSheet("background-color: #6c757d;");
    } else {
        yesButton->setStyleSheet("background-color: #007bff;");
    }

    msgBox.setStyleSheet(style);
    msgBox.exec();

    if (msgBox.clickedButton() == yesButton) return QMessageBox::Yes;
    return QMessageBox::No;
}


// --- Tab 1: Hợp Đồng (Đã viết lại) ---
void EmployeeDetailDialog::populateContractList() {
    contractTableWidget->setRowCount(0);
    for (const auto& hd : currentEmployee->getDSHopDong()) {
        int row = contractTableWidget->rowCount();
        contractTableWidget->insertRow(row);

        QTableWidgetItem *soHdItem = new QTableWidgetItem(QString::fromStdString(hd.getSoHopDong()));
        QTableWidgetItem *ngayKyItem = new QTableWidgetItem(QString::fromStdString(hd.getNgayKy()));
        QTableWidgetItem *loaiHdItem = new QTableWidgetItem(QString::fromStdString(hd.getLoaiHopDong()));
        QTableWidgetItem *thoiGianItem = new QTableWidgetItem(QString::fromStdString(hd.getThoiGianHopDong()));

        soHdItem->setData(Qt::UserRole, QString::fromStdString(hd.getSoHopDong()));

        styleTableCell(soHdItem, true, QColor(0, 92, 178), true); // Căn giữa
        styleTableCell(ngayKyItem, false, Qt::black, true); // Căn giữa
        styleTableCell(loaiHdItem, false, Qt::black, false); // Căn trái
        styleTableCell(thoiGianItem, false, Qt::black, true); // Căn giữa

        contractTableWidget->setItem(row, 0, soHdItem);
        contractTableWidget->setItem(row, 1, ngayKyItem);
        contractTableWidget->setItem(row, 2, loaiHdItem);
        contractTableWidget->setItem(row, 3, thoiGianItem);
    }
    editContractButton->setEnabled(false);
    deleteContractButton->setEnabled(false);
}
void EmployeeDetailDialog::onAddContract() {
    AddContractDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        HopDong newContract = dialog.getContract();
        for (const auto& hd : currentEmployee->getDSHopDong()) {
            if (hd.getSoHopDong() == newContract.getSoHopDong()) {
                showCustomMessageBox(this, QMessageBox::Warning, "Loi", "So Hop Dong nay da ton tai.", "OK", "");
                return;
            }
        }
        currentEmployee->themHopDong(newContract);
        populateContractList();
    }
}
void EmployeeDetailDialog::onEditContract() {
    int row = contractTableWidget->currentRow();
    if (row < 0) return;
    QString soHD = contractTableWidget->item(row, 0)->data(Qt::UserRole).toString();

    HopDong* contractToEdit = nullptr;
    for (auto& hd : currentEmployee->getDSHopDong()) {
        if (hd.getSoHopDong() == soHD.toStdString()) {
            contractToEdit = &hd;
            break;
        }
    }
    if (!contractToEdit) {
        showCustomMessageBox(this, QMessageBox::Critical, "Loi", "Khong tim thay du lieu hop dong goc.", "OK", "");
        return;
    }

    AddContractDialog dialog(*contractToEdit, this);
    if (dialog.exec() == QDialog::Accepted) {
        *contractToEdit = dialog.getContract();
        populateContractList();
    }
}
void EmployeeDetailDialog::onDeleteContract() {
    int row = contractTableWidget->currentRow();
    if (row < 0) return;
    QString soHD = contractTableWidget->item(row, 0)->data(Qt::UserRole).toString();

    int reply = showCustomMessageBox(this, QMessageBox::Question, "Xac nhan Xoa",
                                     "Ban co chac chan muon xoa Hop Dong so <b>" + soHD + "</b>?", "Dong Y Xoa", "Huy Bo");

    if (reply == QMessageBox::Yes) {
        vector<HopDong>& dsHD = currentEmployee->getDSHopDong();
        auto it = std::remove_if(dsHD.begin(), dsHD.end(), [&](const HopDong& hd) {
            return hd.getSoHopDong() == soHD.toStdString();
        });
        if (it != dsHD.end()) {
            dsHD.erase(it, dsHD.end());
            populateContractList();
        }
    }
}
void EmployeeDetailDialog::onContractSelected(int row, int /*column*/) {
    if (row >= 0) {
        editContractButton->setEnabled(true);
        deleteContractButton->setEnabled(true);
    }
}

// --- Tab 2: Lương (Đã viết lại) ---
void EmployeeDetailDialog::populateSalaryList() {
    salaryTableWidget->setRowCount(0);
    QLocale vietLocale(QLocale::Vietnamese, QLocale::Vietnam);
    for (const auto& luong : currentEmployee->getDSLuong()) {
        int row = salaryTableWidget->rowCount();
        salaryTableWidget->insertRow(row);

        QTableWidgetItem *thangItem = new QTableWidgetItem(QString::fromStdString(luong.getThang()));
        QTableWidgetItem *lcbItem = new QTableWidgetItem(vietLocale.toString(luong.getLuongCoBan(), 'f', 0));
        QTableWidgetItem *thuongItem = new QTableWidgetItem(vietLocale.toString(luong.getThuong(), 'f', 0));
        QTableWidgetItem *phuCapItem = new QTableWidgetItem(vietLocale.toString(luong.getPhuCap(), 'f', 0));
        QTableWidgetItem *tangCaItem = new QTableWidgetItem(vietLocale.toString(luong.getTangCa(), 'f', 0));
        QTableWidgetItem *thucLanhItem = new QTableWidgetItem(vietLocale.toString(luong.getThucLanh(), 'f', 0));

        thangItem->setData(Qt::UserRole, QString::fromStdString(luong.getThang()));

        styleTableCell(thangItem, true, QColor(0, 92, 178), true);
        styleTableCell(lcbItem, false, Qt::black, true);
        styleTableCell(thuongItem, false, Qt::black, true);
        styleTableCell(phuCapItem, false, Qt::black, true);
        styleTableCell(tangCaItem, false, Qt::black, true);
        styleTableCell(thucLanhItem, true, QColor(56, 142, 60), true);

        salaryTableWidget->setItem(row, 0, thangItem);
        salaryTableWidget->setItem(row, 1, lcbItem);
        salaryTableWidget->setItem(row, 2, thuongItem);
        salaryTableWidget->setItem(row, 3, phuCapItem);
        salaryTableWidget->setItem(row, 4, tangCaItem);
        salaryTableWidget->setItem(row, 5, thucLanhItem);
    }
    editSalaryButton->setEnabled(false);
    deleteSalaryButton->setEnabled(false);
}
void EmployeeDetailDialog::onAddSalary() {
    AddSalaryDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Luong newSalary = dialog.getSalary();
        for (const auto& luong : currentEmployee->getDSLuong()) {
            if (luong.getThang() == newSalary.getThang()) {
                showCustomMessageBox(this, QMessageBox::Warning, "Loi", "Bang luong cho thang nay da ton tai.", "OK", "");
                return;
            }
        }
        currentEmployee->themLuong(newSalary);
        populateSalaryList();
    }
}
void EmployeeDetailDialog::onEditSalary() {
    showCustomMessageBox(this, QMessageBox::Information, "Thong bao", "Chuc nang Sua Bang Luong se duoc trien khai sau.", "OK", "");
}
void EmployeeDetailDialog::onDeleteSalary() {
    int row = salaryTableWidget->currentRow();
    if (row < 0) return;
    QString thang = salaryTableWidget->item(row, 0)->data(Qt::UserRole).toString();

    int reply = showCustomMessageBox(this, QMessageBox::Question, "Xac nhan Xoa",
                                     "Ban co chac chan muon xoa Bang luong thang <b>" + thang + "</b>?", "Dong Y Xoa", "Huy Bo");

    if (reply == QMessageBox::Yes) {
        vector<Luong>& dsLuong = currentEmployee->getDSLuong();
        auto it = std::remove_if(dsLuong.begin(), dsLuong.end(), [&](const Luong& luong) {
            return luong.getThang() == thang.toStdString();
        });
        if (it != dsLuong.end()) {
            dsLuong.erase(it, dsLuong.end());
            populateSalaryList();
        }
    }
}
void EmployeeDetailDialog::onSalarySelected(int row, int /*column*/) {
    if (row >= 0) {
        editSalaryButton->setEnabled(true);
        deleteSalaryButton->setEnabled(true);
    }
}

// --- Tab 3: Chấm Công (Đã viết lại) ---
void EmployeeDetailDialog::populateTimekeepingList() {
    timekeepingTableWidget->setRowCount(0);
    for (const auto& cc : currentEmployee->getDSChamCong()) {
        int row = timekeepingTableWidget->rowCount();
        timekeepingTableWidget->insertRow(row);

        QTableWidgetItem *ngayCongItem = new QTableWidgetItem(QString::fromStdString(cc.getNgayCong()));
        QTableWidgetItem *phanCaItem = new QTableWidgetItem(QString::fromStdString(cc.getPhanCa()));
        QTableWidgetItem *gioThemItem = new QTableWidgetItem(QString::number(cc.getSoGioThem()) + " gio");

        ngayCongItem->setData(Qt::UserRole, QString::fromStdString(cc.getNgayCong()));

        styleTableCell(ngayCongItem, true, QColor(0, 92, 178), true);
        styleTableCell(phanCaItem, false, Qt::black, true);
        styleTableCell(gioThemItem, false, Qt::black, true);

        timekeepingTableWidget->setItem(row, 0, ngayCongItem);
        timekeepingTableWidget->setItem(row, 1, phanCaItem);
        timekeepingTableWidget->setItem(row, 2, gioThemItem);
    }
    editTimekeepingButton->setEnabled(false);
    deleteTimekeepingButton->setEnabled(false);
}
void EmployeeDetailDialog::onAddTimekeeping() {
    AddTimekeepingDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        ChamCong newCC = dialog.getTimekeeping();
        for (const auto& cc : currentEmployee->getDSChamCong()) {
            if (cc.getNgayCong() == newCC.getNgayCong()) {
                showCustomMessageBox(this, QMessageBox::Warning, "Loi", "Bang cham cong cho ngay nay da ton tai.", "OK", "");
                return;
            }
        }
        currentEmployee->themChamCong(newCC);
        populateTimekeepingList();
    }
}
void EmployeeDetailDialog::onEditTimekeeping() {
    showCustomMessageBox(this, QMessageBox::Information, "Thong bao", "Chuc nang Sua Cham Cong se duoc trien khai sau.", "OK", "");
}
void EmployeeDetailDialog::onDeleteTimekeeping() {
    int row = timekeepingTableWidget->currentRow();
    if (row < 0) return;
    QString ngayCong = timekeepingTableWidget->item(row, 0)->data(Qt::UserRole).toString();

    int reply = showCustomMessageBox(this, QMessageBox::Question, "Xac nhan Xoa",
                                     "Ban co chac chan muon xoa bang cham cong ngay <b>" + ngayCong + "</b>?", "Dong Y Xoa", "Huy Bo");

    if (reply == QMessageBox::Yes) {
        vector<ChamCong>& dsCC = currentEmployee->getDSChamCong();
        auto it = std::remove_if(dsCC.begin(), dsCC.end(), [&](const ChamCong& cc) {
            return cc.getNgayCong() == ngayCong.toStdString();
        });
        if (it != dsCC.end()) {
            dsCC.erase(it, dsCC.end());
            populateTimekeepingList();
        }
    }
}
void EmployeeDetailDialog::onTimekeepingSelected(int row, int /*column*/) {
    if (row >= 0) {
        editTimekeepingButton->setEnabled(true);
        deleteTimekeepingButton->setEnabled(true);
    }
}

// --- Tab 4: Quá Trình Công Tác (Đã viết lại) ---
void EmployeeDetailDialog::populateWorkHistoryList() {
    workHistoryTableWidget->setRowCount(0);
    for (size_t i = 0; i < currentEmployee->getDSQuaTrinhCongTac().size(); ++i) {
        const auto& qtct = currentEmployee->getDSQuaTrinhCongTac()[i];
        int row = workHistoryTableWidget->rowCount();
        workHistoryTableWidget->insertRow(row);

        QTableWidgetItem *ngayItem = new QTableWidgetItem(QString::fromStdString(qtct.getNgayVaoLam()));
        QTableWidgetItem *boPhanItem = new QTableWidgetItem(QString::fromStdString(qtct.getBoPhan()));
        QTableWidgetItem *chucVuItem = new QTableWidgetItem(QString::fromStdString(qtct.getChucVu()));

        ngayItem->setData(Qt::UserRole, (int)i);

        styleTableCell(ngayItem, true, QColor(0, 92, 178), true);
        styleTableCell(boPhanItem, false, Qt::black, true);
        styleTableCell(chucVuItem, false, Qt::black, true);

        workHistoryTableWidget->setItem(row, 0, ngayItem);
        workHistoryTableWidget->setItem(row, 1, boPhanItem);
        workHistoryTableWidget->setItem(row, 2, chucVuItem);
    }
    editWorkHistoryButton->setEnabled(false);
    deleteWorkHistoryButton->setEnabled(false);
}
void EmployeeDetailDialog::onAddWorkHistory() {
    AddWorkHistoryDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QuaTrinhCongTac newQTCT = dialog.getWorkHistory();
        currentEmployee->themQuaTrinhCongTac(newQTCT);
        populateWorkHistoryList();
    }
}
void EmployeeDetailDialog::onEditWorkHistory() {
    showCustomMessageBox(this, QMessageBox::Information, "Thong bao", "Chuc nang Sua Qua Trinh Cong Tac se duoc trien khai sau.", "OK", "");
}
void EmployeeDetailDialog::onDeleteWorkHistory() {
    int row = workHistoryTableWidget->currentRow();
    if (row < 0) return;

    int index = workHistoryTableWidget->item(row, 0)->data(Qt::UserRole).toInt();
    QString text = workHistoryTableWidget->item(row, 0)->text() + " - " +
                   workHistoryTableWidget->item(row, 1)->text();

    int reply = showCustomMessageBox(this, QMessageBox::Question, "Xac nhan Xoa",
                                     "Ban co chac chan muon xoa muc qua trinh cong tac nay?<br><b>" + text + "</b>", "Dong Y Xoa", "Huy Bo");

    if (reply == QMessageBox::Yes) {
        vector<QuaTrinhCongTac>& dsQTCT = currentEmployee->getDSQuaTrinhCongTac();
        if (index >= 0 && index < (int)dsQTCT.size()) {
            dsQTCT.erase(dsQTCT.begin() + index);
            populateWorkHistoryList();
        }
    }
}
void EmployeeDetailDialog::onWorkHistorySelected(int row, int /*column*/) {
    if (row >= 0) {
        editWorkHistoryButton->setEnabled(true);
        deleteWorkHistoryButton->setEnabled(true);
    }
}

// --- Tab 5: Đào Tạo (Đã viết lại) ---
void EmployeeDetailDialog::populateTrainingList() {
    trainingTableWidget->setRowCount(0);
    for (const auto& dt : currentEmployee->getDSDaoTao()) {
        int row = trainingTableWidget->rowCount();
        trainingTableWidget->insertRow(row);

        QTableWidgetItem *maItem = new QTableWidgetItem(QString::fromStdString(dt.getMaKhoaHoc()));
        QTableWidgetItem *tenItem = new QTableWidgetItem(QString::fromStdString(dt.getTenKhoaHoc()));
        QTableWidgetItem *bdItem = new QTableWidgetItem(QString::fromStdString(dt.getNgayBatDau()));
        QTableWidgetItem *ktItem = new QTableWidgetItem(QString::fromStdString(dt.getNgayKetThuc()));
        QTableWidgetItem *dvItem = new QTableWidgetItem(QString::fromStdString(dt.getDonViDaoTao()));

        maItem->setData(Qt::UserRole, QString::fromStdString(dt.getMaKhoaHoc()));

        styleTableCell(maItem, true, QColor(0, 92, 178), true);
        styleTableCell(tenItem);
        styleTableCell(bdItem, false, Qt::black, true);
        styleTableCell(ktItem, false, Qt::black, true);
        styleTableCell(dvItem);

        trainingTableWidget->setItem(row, 0, maItem);
        trainingTableWidget->setItem(row, 1, tenItem);
        trainingTableWidget->setItem(row, 2, bdItem);
        trainingTableWidget->setItem(row, 3, ktItem);
        trainingTableWidget->setItem(row, 4, dvItem);
    }
    editTrainingButton->setEnabled(false);
    deleteTrainingButton->setEnabled(false);
}
void EmployeeDetailDialog::onAddTraining() {
    AddTrainingDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        DaoTao newDT = dialog.getTraining();
        for (const auto& dt : currentEmployee->getDSDaoTao()) {
            if (dt.getMaKhoaHoc() == newDT.getMaKhoaHoc() && !newDT.getMaKhoaHoc().empty()) {
                showCustomMessageBox(this, QMessageBox::Warning, "Loi", "Ma khoa hoc nay da ton tai.", "OK", "");
                return;
            }
        }
        currentEmployee->themDaoTao(newDT);
        populateTrainingList();
    }
}
void EmployeeDetailDialog::onEditTraining() {
    showCustomMessageBox(this, QMessageBox::Information, "Thong bao", "Chuc nang Sua Khoa Hoc se duoc trien khai sau.", "OK", "");
}
void EmployeeDetailDialog::onDeleteTraining() {
    int row = trainingTableWidget->currentRow();
    if (row < 0) return;

    QString maKH = trainingTableWidget->item(row, 0)->data(Qt::UserRole).toString();
    QString tenKH = trainingTableWidget->item(row, 1)->text();

    int reply = showCustomMessageBox(this, QMessageBox::Question, "Xac nhan Xoa",
                                     "Ban co chac chan muon xoa khoa hoc nay?<br><b>" + maKH + " - " + tenKH + "</b>", "Dong Y Xoa", "Huy Bo");

    if (reply == QMessageBox::Yes) {
        vector<DaoTao>& dsDT = currentEmployee->getDSDaoTao();
        auto it = std::remove_if(dsDT.begin(), dsDT.end(), [&](const DaoTao& dt) {
            return dt.getMaKhoaHoc() == maKH.toStdString();
        });
        if (it != dsDT.end()) {
            dsDT.erase(it, dsDT.end());
            populateTrainingList();
        }
    }
}
void EmployeeDetailDialog::onTrainingSelected(int row, int /*column*/) {
    if (row >= 0) {
        editTrainingButton->setEnabled(true);
        deleteTrainingButton->setEnabled(true);
    }
}

// --- Tab 6: Khen Thưởng/Kỷ Luật (Đã viết lại) ---
void EmployeeDetailDialog::populateRewardList() {
    rewardTableWidget->setRowCount(0);
    QLocale vietLocale(QLocale::Vietnamese, QLocale::Vietnam);
    for (const auto& ktkl : currentEmployee->getDSKTKL()) {
        int row = rewardTableWidget->rowCount();
        rewardTableWidget->insertRow(row);

        QTableWidgetItem *loaiItem = new QTableWidgetItem(QString::fromStdString(ktkl.getLoai()));
        QTableWidgetItem *ngayItem = new QTableWidgetItem(QString::fromStdString(ktkl.getNgay()));
        QTableWidgetItem *ndItem = new QTableWidgetItem(QString::fromStdString(ktkl.getNoiDung()));
        QTableWidgetItem *qdItem = new QTableWidgetItem(QString::fromStdString(ktkl.getQuyetDinh()));
        QTableWidgetItem *tienItem = new QTableWidgetItem(vietLocale.toString(ktkl.getSoTien(), 'f', 0) + " VND");

        qdItem->setData(Qt::UserRole, QString::fromStdString(ktkl.getQuyetDinh()));

        styleTableCell(loaiItem, true, (ktkl.getLoai() == "Khen thuong" ? QColor(56, 142, 60) : QColor(220, 53, 69)), true);
        styleTableCell(ngayItem, false, Qt::black, true);
        styleTableCell(ndItem);
        styleTableCell(qdItem, false, QColor(0, 92, 178), true);
        styleTableCell(tienItem, true, Qt::black, true);

        rewardTableWidget->setItem(row, 0, loaiItem);
        rewardTableWidget->setItem(row, 1, ngayItem);
        rewardTableWidget->setItem(row, 2, ndItem);
        rewardTableWidget->setItem(row, 3, qdItem);
        rewardTableWidget->setItem(row, 4, tienItem);
    }
    editRewardButton->setEnabled(false);
    deleteRewardButton->setEnabled(false);
}
void EmployeeDetailDialog::onAddReward() {
    AddRewardDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        currentEmployee->themKTKL(dialog.getRewardDiscipline());
        populateRewardList();
    }
}
void EmployeeDetailDialog::onEditReward() {
    showCustomMessageBox(this, QMessageBox::Information, "Thong bao", "Chuc nang Sua se duoc trien khai sau.", "OK", "");
}
void EmployeeDetailDialog::onDeleteReward() {
    int row = rewardTableWidget->currentRow();
    if (row < 0) return;

    QString soQD = rewardTableWidget->item(row, 3)->data(Qt::UserRole).toString();
    QString loai = rewardTableWidget->item(row, 0)->text();

    int reply = showCustomMessageBox(this, QMessageBox::Question, "Xac nhan Xoa",
                                     "Ban co chac chan muon xoa muc <b>" + loai + "</b> nay?<br>(So QD: " + soQD + ")", "Dong Y Xoa", "Huy Bo");

    if (reply == QMessageBox::Yes) {
        vector<KhenThuongKyLuat>& dsKTKL = currentEmployee->getDSKTKL();
        auto it = std::remove_if(dsKTKL.begin(), dsKTKL.end(), [&](const KhenThuongKyLuat& ktkl) {
            return ktkl.getQuyetDinh() == soQD.toStdString();
        });
        if (it != dsKTKL.end()) {
            dsKTKL.erase(it, dsKTKL.end());
            populateRewardList();
        }
    }
}
void EmployeeDetailDialog::onRewardSelected(int row, int /*column*/) {
    if (row >= 0) {
        editRewardButton->setEnabled(true);
        deleteRewardButton->setEnabled(true);
    }
}

// --- Tab 7: Nghỉ Phép (Đã viết lại) ---
void EmployeeDetailDialog::populateLeaveList() {
    leaveTableWidget->setRowCount(0);
    for (const auto& np : currentEmployee->getDSNghiPhep()) {
        int row = leaveTableWidget->rowCount();
        leaveTableWidget->insertRow(row);

        QTableWidgetItem *loaiItem = new QTableWidgetItem(QString::fromStdString(np.getLoaiPhep()));
        QTableWidgetItem *soNgayItem = new QTableWidgetItem(QString::fromStdString(np.getSoNgay()));
        QTableWidgetItem *bdItem = new QTableWidgetItem(QString::fromStdString(np.getNgayBatDau()));
        QTableWidgetItem *ktItem = new QTableWidgetItem(QString::fromStdString(np.getNgayKetThuc()));

        bdItem->setData(Qt::UserRole, QString::fromStdString(np.getNgayBatDau()));

        styleTableCell(loaiItem, true, QColor(0, 92, 178));
        styleTableCell(soNgayItem, false, Qt::black, true);
        styleTableCell(bdItem, false, Qt::black, true);
        styleTableCell(ktItem, false, Qt::black, true);

        leaveTableWidget->setItem(row, 0, loaiItem);
        leaveTableWidget->setItem(row, 1, soNgayItem);
        leaveTableWidget->setItem(row, 2, bdItem);
        leaveTableWidget->setItem(row, 3, ktItem);
    }
    editLeaveButton->setEnabled(false);
    deleteLeaveButton->setEnabled(false);
}
void EmployeeDetailDialog::onAddLeave() {
    AddLeaveDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        currentEmployee->themNghiPhep(dialog.getLeave());
        populateLeaveList();
    }
}
void EmployeeDetailDialog::onEditLeave() {
    showCustomMessageBox(this, QMessageBox::Information, "Thong bao", "Chuc nang Sua se duoc trien khai sau.", "OK", "");
}
void EmployeeDetailDialog::onDeleteLeave() {
    int row = leaveTableWidget->currentRow();
    if (row < 0) return;

    QString ngayBD = leaveTableWidget->item(row, 2)->data(Qt::UserRole).toString();
    QString loaiPhep = leaveTableWidget->item(row, 0)->text();

    int reply = showCustomMessageBox(this, QMessageBox::Question, "Xac nhan Xoa",
                                     "Ban co chac chan muon xoa don nghi phep nay?<br><b>" + loaiPhep + " (tu " + ngayBD + ")</b>", "Dong Y Xoa", "Huy Bo");

    if (reply == QMessageBox::Yes) {
        vector<NghiPhep>& dsNP = currentEmployee->getDSNghiPhep();
        auto it = std::remove_if(dsNP.begin(), dsNP.end(), [&](const NghiPhep& np) {
            return np.getNgayBatDau() == ngayBD.toStdString();
        });
        if (it != dsNP.end()) {
            dsNP.erase(it, dsNP.end());
            populateLeaveList();
        }
    }
}
void EmployeeDetailDialog::onLeaveSelected(int row, int /*column*/) {
    if (row >= 0) {
        editLeaveButton->setEnabled(true);
        deleteLeaveButton->setEnabled(true);
    }
}
