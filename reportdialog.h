#ifndef REPORTDIALOG_H
#define REPORTDIALOG_H

#include <QDialog>
#include "hrm_logic.h"

class QTabWidget;
class QTableWidget;
class QTableWidgetItem;

class ReportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportDialog(DanhSachNhanVien& dsnv, QWidget *parent = nullptr);

private:
    void generateSalaryReport();
    void generateGenderReport();

    // --- THÊM MỚI: Các hàm trợ giúp ---
    void setupTableWidget(QTableWidget *table, const QStringList &headers);
    // THAY ĐỔI: Bỏ tham số alignRight
    void styleTableCell(QTableWidgetItem *item, bool isBold = false, const QColor &color = Qt::black);
    // --------------------------------

    DanhSachNhanVien& dsnv;

    // UI components
    QTabWidget *tabWidget;
    QTableWidget *salaryReportTable;
    QTableWidget *genderReportTable;
};

#endif // REPORTDIALOG_H
