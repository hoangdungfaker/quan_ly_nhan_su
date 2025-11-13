#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "hrm_logic.h"

class QTableWidget;
class QTableWidgetItem;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QLineEdit;
class QComboBox;
class QLabel;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(DanhSachNhanVien& dsnv, NhanVien* user, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void populateEmployeeList();
    void onAddEmployee();
    void onEditEmployee();
    void onDeleteEmployee();
    void onSaveData();
    void onEmployeeSelected(int row, int column);
    void onShowReports();
    void onShowDetails();
    void filterEmployeeList();
    void onLogout();
    void onViewInfo();

private:
    // Bố cục
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QHBoxLayout *searchLayout;

    // Thành phần UI
    QTableWidget *employeeTableWidget;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *saveButton;
    QPushButton *reportButton;
    QPushButton *detailButton;

    QLineEdit *searchEdit;
    QComboBox *searchTypeCombo;

    QLabel *titleLabel;
    QLabel *searchLabel;
    QWidget *searchWidgetContainer;
    QWidget *bottomWidgetContainer;

    // --- THAY ĐỔI: Thêm profileNameLabel ---
    NhanVien* loggedInUser;
    QLabel *profileNameLabel; // <-- THÊM MỚI
    QPushButton *profileButton;
    QMenu *profileMenu;
    // ---------------------------------

    DanhSachNhanVien& dsnv;
};

#endif // MAINWINDOW_H
