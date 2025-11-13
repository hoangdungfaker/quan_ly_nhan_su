#ifndef EMPLOYEEVIEWDIALOG_H
#define EMPLOYEEVIEWDIALOG_H

#include <QDialog>
#include "hrm_logic.h"

class QTabWidget;
class QTableWidget;
class QTableWidgetItem;
class QLabel;      // <-- THÊM MỚI
class QPushButton; // <-- THÊM MỚI
class QMenu;       // <-- THÊM MỚI

class EmployeeViewDialog : public QDialog
{
    Q_OBJECT

public:
    // --- THAY ĐỔI: Cập nhật constructor ---
    explicit EmployeeViewDialog(NhanVien& nv, DanhSachNhanVien& dsnv, QWidget *parent = nullptr);

private slots:
    void onLogout();
    void onViewInfo(); // <-- THÊM MỚI: Slot cho "Xem thông tin"

private:
    // --- Hàm trợ giúp ---
    QWidget* createInfoTab();
    QWidget* createContractTab();
    QWidget* createSalaryTab();
    void setupTableWidget(QTableWidget *table, const QStringList &headers);
    void styleTableCell(QTableWidgetItem *item, bool isBold = false, const QColor &color = Qt::black, bool alignCenter = false);

    // --- THAY ĐỔI: Quản lý dữ liệu ---
    NhanVien& currentEmployee; // Bỏ const
    DanhSachNhanVien& dsnv;      // Thêm dsnv
    // ---------------------------------

    // UI
    QTabWidget *tabWidget;
    QTableWidget *infoTableWidget;
    QTableWidget *contractTableWidget;
    QTableWidget *salaryTableWidget;

    // --- THÊM MỚI: UI cho profile ---
    QLabel *profileNameLabel;
    QPushButton *profileButton;
    QMenu *profileMenu;
    // -----------------------------
};

#endif // EMPLOYEEVIEWDIALOG_H
