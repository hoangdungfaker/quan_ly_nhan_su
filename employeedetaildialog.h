#ifndef EMPLOYEEDETAILDIALOG_H
#define EMPLOYEEDETAILDIALOG_H

#include <QDialog>
#include "hrm_logic.h"

class QTabWidget;
class QTableWidget; // <-- THAY ĐỔI
class QPushButton;
class QTableWidgetItem; // <-- THÊM MỚI

class EmployeeDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmployeeDetailDialog(NhanVien* nv, QWidget *parent = nullptr);

private slots:
    // --- THAY ĐỔI: Cập nhật các slot cho bảng ---
    // (column) được thêm vào nhưng sẽ được đánh dấu là không sử dụng

    // Tab Hợp Đồng
    void populateContractList();
    void onAddContract();
    void onEditContract();
    void onDeleteContract();
    void onContractSelected(int row, int column);

    // Tab Lương
    void populateSalaryList();
    void onAddSalary();
    void onEditSalary();
    void onDeleteSalary();
    void onSalarySelected(int row, int column);

    // Tab Chấm Công
    void populateTimekeepingList();
    void onAddTimekeeping();
    void onEditTimekeeping();
    void onDeleteTimekeeping();
    void onTimekeepingSelected(int row, int column);

    // Tab QTCT
    void populateWorkHistoryList();
    void onAddWorkHistory();
    void onEditWorkHistory();
    void onDeleteWorkHistory();
    void onWorkHistorySelected(int row, int column);

    // Tab Đào Tạo
    void populateTrainingList();
    void onAddTraining();
    void onEditTraining();
    void onDeleteTraining();
    void onTrainingSelected(int row, int column);

    // Tab KTKL
    void populateRewardList();
    void onAddReward();
    void onEditReward();
    void onDeleteReward();
    void onRewardSelected(int row, int column);

    // Tab Nghỉ Phép
    void populateLeaveList();
    void onAddLeave();
    void onEditLeave();
    void onDeleteLeave();
    void onLeaveSelected(int row, int column);

private:
    // --- Hàm trợ giúp ---
    void setupTableWidget(QTableWidget *table, const QStringList &headers);
    void styleTableCell(QTableWidgetItem *item, bool isBold = false, const QColor &color = Qt::black, bool alignCenter = false);

    // --- Hàm tạo Tab ---
    QWidget* createContractTab();
    QWidget* createSalaryTab();
    QWidget* createTimekeepingTab();
    QWidget* createWorkHistoryTab();
    QWidget* createTrainingTab();
    QWidget* createRewardTab();
    QWidget* createLeaveTab();

    NhanVien* currentEmployee;

    // UI chung
    QTabWidget *tabWidget;

    // --- THAY ĐỔI: Toàn bộ UI dùng QTableWidget ---
    QTableWidget *contractTableWidget;
    QPushButton *addContractButton;
    QPushButton *editContractButton;
    QPushButton *deleteContractButton;

    QTableWidget *salaryTableWidget;
    QPushButton *addSalaryButton;
    QPushButton *editSalaryButton;
    QPushButton *deleteSalaryButton;

    QTableWidget *timekeepingTableWidget;
    QPushButton *addTimekeepingButton;
    QPushButton *editTimekeepingButton;
    QPushButton *deleteTimekeepingButton;

    QTableWidget *workHistoryTableWidget;
    QPushButton *addWorkHistoryButton;
    QPushButton *editWorkHistoryButton;
    QPushButton *deleteWorkHistoryButton;

    QTableWidget *trainingTableWidget;
    QPushButton *addTrainingButton;
    QPushButton *editTrainingButton;
    QPushButton *deleteTrainingButton;

    QTableWidget *rewardTableWidget;
    QPushButton *addRewardButton;
    QPushButton *editRewardButton;
    QPushButton *deleteRewardButton;

    QTableWidget *leaveTableWidget;
    QPushButton *addLeaveButton;
    QPushButton *editLeaveButton;
    QPushButton *deleteLeaveButton;
};

#endif // EMPLOYEEDETAILDIALOG_H
