#ifndef ADDEMPLOYEEDIALOG_H
#define ADDEMPLOYEEDIALOG_H

#include <QDialog>
#include <QColor> // <-- THÊM MỚI
#include "hrm_logic.h"

class QLineEdit;
class QDialogButtonBox;
class QDateEdit;
class QComboBox;
class QTableWidget;
class QCheckBox;

class AddEmployeeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddEmployeeDialog(DanhSachNhanVien& dsnv, const string& maNV = "", QWidget *parent = nullptr);
    NhanVien getNhanVien() const;

private slots:
    void onAccept();
    void onShowPasswordToggled(bool checked);

private:
    void setupUI();
    void loadNhanVienData();
    void saveNhanVienData();
    // --- THAY ĐỔI: Thêm tham số QColor ---
    void addInputRow(QTableWidget *table, int &row, const QString &key, QWidget *valueWidget, const QColor &color);
    void addSeparatorRow(QTableWidget *table, int &row, const QString &text);

    NhanVien currentNhanVien;
    bool isEditMode;
    DanhSachNhanVien& dsnv;

    QTableWidget *infoTableWidget;
    QDialogButtonBox *buttonBox;

    // (Các widget nhập liệu)
    QLineEdit *maNhanVienEdit;
    QLineEdit *hoTenEdit;
    QLineEdit *gioiTinhEdit;
    QDateEdit *ngaySinhEdit;
    QLineEdit *diaChiEdit;
    QLineEdit *cmndEdit;
    QComboBox *queQuanEdit;
    QLineEdit *noiOHienTaiEdit;
    QLineEdit *dienThoaiEdit;
    QLineEdit *maPhongBanEdit;
    QLineEdit *tenPhongBanEdit;
    QLineEdit *maChucVuEdit;
    QLineEdit *tenChucVuEdit;
    QLineEdit *thanhPhanBanThanEdit;
    QLineEdit *trinhDoVanHoaEdit;
    QLineEdit *chuyenMonEdit;
    QLineEdit *ngoaiNguEdit;
    QLineEdit *soBaoHiemEdit;
    QLineEdit *noiCapBHEdit;
    QLineEdit *noiDKKhamEdit;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *roleEdit;

    QCheckBox *showPasswordCheckBox;
};

#endif // ADDEMPLOYEEDIALOG_H
