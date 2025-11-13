#include "addemployeedialog.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QLabel>
#include <QPushButton>
#include <QDateEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QCalendarWidget>
#include <QApplication>

AddEmployeeDialog::AddEmployeeDialog(DanhSachNhanVien& dsnv, const string& maNV, QWidget *parent)
    : QDialog(parent), isEditMode(!maNV.empty()), dsnv(dsnv)
{
    setWindowTitle(isEditMode ? "Sua Thong Tin Nhan Vien" : "Them Nhan Vien Moi");

    if (isEditMode) {
        NhanVien* nv = dsnv.findNhanVienByMa(maNV);
        if (nv) {
            currentNhanVien = *nv;
        }
    }

    // Khởi tạo tất cả widget nhập liệu
    maNhanVienEdit = new QLineEdit();
    hoTenEdit = new QLineEdit();
    gioiTinhEdit = new QLineEdit();
    ngaySinhEdit = new QDateEdit();
    diaChiEdit = new QLineEdit();
    cmndEdit = new QLineEdit();
    queQuanEdit = new QComboBox();
    noiOHienTaiEdit = new QLineEdit();
    dienThoaiEdit = new QLineEdit();
    maPhongBanEdit = new QLineEdit();
    tenPhongBanEdit = new QLineEdit();
    maChucVuEdit = new QLineEdit();
    tenChucVuEdit = new QLineEdit();
    thanhPhanBanThanEdit = new QLineEdit();
    trinhDoVanHoaEdit = new QLineEdit();
    chuyenMonEdit = new QLineEdit();
    ngoaiNguEdit = new QLineEdit();
    soBaoHiemEdit = new QLineEdit();
    noiCapBHEdit = new QLineEdit();
    noiDKKhamEdit = new QLineEdit();
    usernameEdit = new QLineEdit();
    passwordEdit = new QLineEdit();
    roleEdit = new QLineEdit();
    showPasswordCheckBox = new QCheckBox("Hien");

    setupUI();

    if (isEditMode) {
        loadNhanVienData();
        maNhanVienEdit->setReadOnly(true);
    }
}

// --- THAY ĐỔI: Cập nhật hàm để nhận màu ---
void AddEmployeeDialog::addInputRow(QTableWidget *table, int &row, const QString &key, QWidget *valueWidget, const QColor &color)
{
    table->insertRow(row);

    QTableWidgetItem *keyItem = new QTableWidgetItem(key);
    keyItem->setFont(QFont("Arial", -1, QFont::Bold));
    keyItem->setForeground(color); // <-- Dùng màu được truyền vào
    keyItem->setBackground(QColor(245, 247, 250));

    table->setItem(row, 0, keyItem);

    QTableWidgetItem *valueBackground = new QTableWidgetItem();
    valueBackground->setBackground(Qt::white);
    table->setItem(row, 1, valueBackground);
    table->setCellWidget(row, 1, valueWidget);

    row++;
}
// ------------------------------------------

void AddEmployeeDialog::addSeparatorRow(QTableWidget *table, int &row, const QString &text)
{
    table->insertRow(row);

    QTableWidgetItem *separatorItem = new QTableWidgetItem(text);
    separatorItem->setBackground(QColor(52, 73, 94)); // #34495e
    separatorItem->setForeground(QColor(Qt::white));
    QFont font;
    font.setBold(true);
    separatorItem->setFont(font);
    separatorItem->setTextAlignment(Qt::AlignCenter);

    table->setItem(row, 0, separatorItem);
    table->setSpan(row, 0, 1, 2);

    row++;
}

void AddEmployeeDialog::setupUI()
{
    resize(700, 700);

    // --- 1. Tạo Bảng ---
    infoTableWidget = new QTableWidget(this);
    infoTableWidget->setColumnCount(2);
    infoTableWidget->setRowCount(0);
    infoTableWidget->horizontalHeader()->setVisible(false);
    infoTableWidget->verticalHeader()->setVisible(false);
    infoTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    infoTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    infoTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    infoTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    infoTableWidget->setAlternatingRowColors(false);
    infoTableWidget->setShowGrid(true);

    // --- 2. Cấu hình các Widget nhập liệu ---
    ngaySinhEdit->setDisplayFormat("dd/MM/yyyy");
    ngaySinhEdit->setCalendarPopup(true);
    ngaySinhEdit->setDate(QDate(2000, 1, 1));

    queQuanEdit->setEditable(true);
    QStringList provinces = {
        "", "An Giang", "Bà Rịa - Vũng Tàu", "Bắc Giang", "Bắc Kạn", "Bạc Liêu", "Bắc Ninh",
        "Bến Tre", "Bình Định", "Bình Dương", "Bình Phước", "Bình Thuận", "Cà Mau", "Cần Thơ",
        "Cao Bằng", "Đà Nẵng", "Đắk Lắk", "Đắk Nông", "Điện Biên", "Đồng Nai", "Đồng Tháp",
        "Gia Lai", "Hà Giang", "Hà Nam", "Hà Nội", "Hà Tĩnh", "Hải Dương", "Hải Phòng",
        "Hậu Giang", "Hòa Bình", "Hưng Yên", "Khánh Hòa", "Kiên Giang", "Kon Tum", "Lai Châu",
        "Lâm Đồng", "Lạng Sơn", "Lào Cai", "Long An", "Nam Định", "Nghệ An", "Ninh Bình",
        "Ninh Thuận", "Phú Thọ", "Phú Yên", "Quảng Bình", "Quảng Nam", "Quảng Ngãi", "Quảng Ninh",
        "Quảng Trị", "Sóc Trăng", "Sơn La", "Tây Ninh", "Thái Bình", "Thái Nguyên", "Thanh Hóa",
        "Thừa Thiên Huế", "Tiền Giang", "TP. Hồ Chí Minh", "Trà Vinh", "Tuyên Quang", "Vĩnh Long",
        "Vĩnh Phúc", "Yên Bái"
    };
    queQuanEdit->addItems(provinces);
    queQuanEdit->lineEdit()->setPlaceholderText("Chọn hoặc nhập tỉnh thành...");

    passwordEdit->setEchoMode(QLineEdit::Password);
    if (isEditMode) {
        passwordEdit->setPlaceholderText("[Để trống nếu không đổi]");
    }
    roleEdit->setPlaceholderText("admin hoac nhan vien");

    // --- 3. Thêm các hàng vào Bảng (với màu) ---
    int row = 0;
    QColor blue(0, 92, 178);
    QColor black(Qt::black); // Giữ màu đen cho Ho Ten
    QColor purple(106, 27, 154);
    QColor green(56, 142, 60);
    QColor red(220, 53, 69);

    addInputRow(infoTableWidget, row, "Ma Nhan Vien (*):", maNhanVienEdit, blue);
    addInputRow(infoTableWidget, row, "Ho Ten:", hoTenEdit, black);
    addInputRow(infoTableWidget, row, "Gioi Tinh:", gioiTinhEdit, black);
    addInputRow(infoTableWidget, row, "Ngay Sinh (dd/mm/yyyy):", ngaySinhEdit, black);
    addInputRow(infoTableWidget, row, "Dia Chi:", diaChiEdit, black);
    addInputRow(infoTableWidget, row, "CMND/CCCD:", cmndEdit, black);
    addInputRow(infoTableWidget, row, "Que Quan:", queQuanEdit, black);
    addInputRow(infoTableWidget, row, "Noi O Hien Tai:", noiOHienTaiEdit, black);
    addInputRow(infoTableWidget, row, "Dien Thoai:", dienThoaiEdit, black);

    addSeparatorRow(infoTableWidget, row, "--- Cong Viec ---");
    addInputRow(infoTableWidget, row, "Ma Phong Ban:", maPhongBanEdit, purple);
    addInputRow(infoTableWidget, row, "Ten Phong Ban:", tenPhongBanEdit, purple);
    addInputRow(infoTableWidget, row, "Ma Chuc Vu:", maChucVuEdit, purple);
    addInputRow(infoTableWidget, row, "Ten Chuc Vu:", tenChucVuEdit, purple);

    addSeparatorRow(infoTableWidget, row, "--- Trinh Do ---");
    addInputRow(infoTableWidget, row, "Thanh phan ban than:", thanhPhanBanThanEdit, green);
    addInputRow(infoTableWidget, row, "Trinh do van hoa:", trinhDoVanHoaEdit, green);
    addInputRow(infoTableWidget, row, "Chuyen mon:", chuyenMonEdit, green);
    addInputRow(infoTableWidget, row, "Ngoai ngu:", ngoaiNguEdit, green);

    addSeparatorRow(infoTableWidget, row, "--- Bao Hiem ---");
    addInputRow(infoTableWidget, row, "So Bao Hiem:", soBaoHiemEdit, red);
    addInputRow(infoTableWidget, row, "Noi cap BH:", noiCapBHEdit, red);
    addInputRow(infoTableWidget, row, "Noi DK Kham Benh:", noiDKKhamEdit, red);

    addSeparatorRow(infoTableWidget, row, "--- Tai Khoan ---");
    addInputRow(infoTableWidget, row, "Username (*):", usernameEdit, blue);

    QWidget *passwordContainer = new QWidget();
    QHBoxLayout *passwordLayout = new QHBoxLayout(passwordContainer);
    passwordLayout->setContentsMargins(0, 0, 0, 0);
    passwordLayout->setSpacing(10);
    passwordLayout->addWidget(passwordEdit, 1);
    passwordLayout->addWidget(showPasswordCheckBox, 0);
    addInputRow(infoTableWidget, row, "Password (*):", passwordContainer, blue);

    addInputRow(infoTableWidget, row, "Role (admin/nhan vien) (*):", roleEdit, blue);

    // Ép chiều cao cho các hàng
    for(int i = 0; i < infoTableWidget->rowCount(); ++i)
    {
        if(infoTableWidget->item(i, 0)->text().startsWith("---"))
        {
            infoTableWidget->setRowHeight(i, 35);
        } else {
            infoTableWidget->setRowHeight(i, 42);
        }
    }

    // --- 4. Nút bấm ---
    buttonBox = new QDialogButtonBox(this);
    QPushButton *saveButton = buttonBox->addButton("Luu", QDialogButtonBox::AcceptRole);
    QPushButton *cancelButton = buttonBox->addButton("Huy Bo", QDialogButtonBox::RejectRole);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &AddEmployeeDialog::onAccept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(showPasswordCheckBox, &QCheckBox::toggled, this, &AddEmployeeDialog::onShowPasswordToggled);

    // --- 5. Layout chính ---
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(infoTableWidget);
    mainLayout->addWidget(buttonBox, 0, Qt::AlignRight);
    setLayout(mainLayout);

    // --- 6. Áp dụng QSS ---
    QColor baseColor = qApp->palette().color(QPalette::Window);
    QString qss = QString(R"(
        QDialog {
            background-color: %1;
            font-family: Arial, sans-serif;
        }
        QTableWidget {
            border: 1px solid #e8e8e8;
            gridline-color: #f0f0f0;
            font-size: 13px;
        }
        QTableWidget::item {
            padding-left: 8px;
            padding-right: 8px;
            selection-background-color: transparent;
            selection-color: %2;
        }

        QTableWidget QLineEdit, QTableWidget QDateEdit, QTableWidget QComboBox {
            padding: 0 8px;
            background-color: transparent;
            font-size: 13px;
            color: black;
            border: none;
            border-radius: 0;
            outline: none;
        }

        QTableWidget QLineEdit:focus,
        QTableWidget QDateEdit:focus,
        QTableWidget QComboBox:focus {
             border: 2px solid #007bff;
        }

        QTableWidget QDateEdit::drop-down, QTableWidget QComboBox::drop-down {
            width: 20px;
            border-left: 1px solid #000;
            background-color: #f0f0f0;
        }
        QTableWidget QComboBox QAbstractItemView {
             border: 1px solid black;
             background-color: white;
             selection-background-color: #007bff;
        }
        QTableWidget QCheckBox {
            background-color: transparent;
            color: #333;
            font-weight: bold;
            padding: 0px;
            margin-left: 5px;
            margin-right: 5px;
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
    )")
                      .arg(baseColor.name())
                      .arg(qApp->palette().color(QPalette::Text).name());

    this->setStyleSheet(qss);

    saveButton->setStyleSheet("background-color: #007bff; color: white;");
    cancelButton->setStyleSheet("background-color: #6c757d; color: white;");
}

void AddEmployeeDialog::loadNhanVienData()
{
    maNhanVienEdit->setText(QString::fromStdString(currentNhanVien.getMaNhanVien()));
    hoTenEdit->setText(QString::fromStdString(currentNhanVien.getHoTen()));
    gioiTinhEdit->setText(QString::fromStdString(currentNhanVien.getGioiTinh()));
    QDate date = QDate::fromString(QString::fromStdString(currentNhanVien.getNgaySinh()), "dd/MM/yyyy");
    if (date.isValid()) {
        ngaySinhEdit->setDate(date);
    }
    diaChiEdit->setText(QString::fromStdString(currentNhanVien.getDiaChi()));
    cmndEdit->setText(QString::fromStdString(currentNhanVien.getCmnd()));
    queQuanEdit->setCurrentText(QString::fromStdString(currentNhanVien.getQueQuan()));
    noiOHienTaiEdit->setText(QString::fromStdString(currentNhanVien.getNoiOHienTai()));
    dienThoaiEdit->setText(QString::fromStdString(currentNhanVien.getDienThoai()));
    maPhongBanEdit->setText(QString::fromStdString(currentNhanVien.getPhongBan().getMaPhongBan()));
    tenPhongBanEdit->setText(QString::fromStdString(currentNhanVien.getPhongBan().getTenPhongBan()));
    maChucVuEdit->setText(QString::fromStdString(currentNhanVien.getChucVu().getMaChucVu()));
    tenChucVuEdit->setText(QString::fromStdString(currentNhanVien.getChucVu().getTenChucVu()));
    thanhPhanBanThanEdit->setText(QString::fromStdString(currentNhanVien.getTrinhDo().getThanhPhanBanThan()));
    trinhDoVanHoaEdit->setText(QString::fromStdString(currentNhanVien.getTrinhDo().getTrinhDoVanHoa()));
    chuyenMonEdit->setText(QString::fromStdString(currentNhanVien.getTrinhDo().getChuyenMon()));
    ngoaiNguEdit->setText(QString::fromStdString(currentNhanVien.getTrinhDo().getNgoaiNgu()));
    soBaoHiemEdit->setText(QString::fromStdString(currentNhanVien.getBaoHiem().getSoBaoHiem()));
    noiCapBHEdit->setText(QString::fromStdString(currentNhanVien.getBaoHiem().getNoiCap()));
    noiDKKhamEdit->setText(QString::fromStdString(currentNhanVien.getBaoHiem().getNoiDKKhamChuaBenh()));
    usernameEdit->setText(QString::fromStdString(currentNhanVien.getTaiKhoan().getUsername()));
    passwordEdit->setText(QString::fromStdString(currentNhanVien.getTaiKhoan().getPassword()));
    roleEdit->setText(QString::fromStdString(currentNhanVien.getTaiKhoan().getRole()));
}

void AddEmployeeDialog::saveNhanVienData()
{
    currentNhanVien.setMaNhanVien(trim(maNhanVienEdit->text().toStdString()));
    currentNhanVien.setHoTen(trim(hoTenEdit->text().toStdString()));
    currentNhanVien.setGioiTinh(trim(gioiTinhEdit->text().toStdString()));
    currentNhanVien.setNgaySinh(ngaySinhEdit->date().toString("dd/MM/yyyy").toStdString());
    currentNhanVien.setDiaChi(trim(diaChiEdit->text().toStdString()));
    currentNhanVien.setCmnd(trim(cmndEdit->text().toStdString()));
    currentNhanVien.setQueQuan(trim(queQuanEdit->currentText().toStdString()));
    currentNhanVien.setNoiOHienTai(trim(noiOHienTaiEdit->text().toStdString()));
    currentNhanVien.setDienThoai(trim(dienThoaiEdit->text().toStdString()));
    currentNhanVien.getPhongBan().setMaPhongBan(trim(maPhongBanEdit->text().toStdString()));
    currentNhanVien.getPhongBan().setTenPhongBan(trim(tenPhongBanEdit->text().toStdString()));
    currentNhanVien.getChucVu().setMaChucVu(trim(maChucVuEdit->text().toStdString()));
    currentNhanVien.getChucVu().setTenChucVu(trim(tenChucVuEdit->text().toStdString()));
    currentNhanVien.getTrinhDo().setThanhPhanBanThan(trim(thanhPhanBanThanEdit->text().toStdString()));
    currentNhanVien.getTrinhDo().setTrinhDoVanHoa(trim(trinhDoVanHoaEdit->text().toStdString()));
    currentNhanVien.getTrinhDo().setChuyenMon(trim(chuyenMonEdit->text().toStdString()));
    currentNhanVien.getTrinhDo().setNgoaiNgu(trim(ngoaiNguEdit->text().toStdString()));
    currentNhanVien.getBaoHiem().setSoBaoHiem(trim(soBaoHiemEdit->text().toStdString()));
    currentNhanVien.getBaoHiem().setNoiCap(trim(noiCapBHEdit->text().toStdString()));
    currentNhanVien.getBaoHiem().setNoiDKKhamChuaBenh(trim(noiDKKhamEdit->text().toStdString()));
    currentNhanVien.getTaiKhoan().setUsername(trim(usernameEdit->text().toStdString()));
    currentNhanVien.getTaiKhoan().setRole(trim(roleEdit->text().toStdString()));
    string newPassword = trim(passwordEdit->text().toStdString());
    if (!isEditMode || !newPassword.empty()) {
        currentNhanVien.getTaiKhoan().setPassword(newPassword);
    }
}

void AddEmployeeDialog::onAccept()
{
    string maNV = trim(maNhanVienEdit->text().toStdString());
    string username = trim(usernameEdit->text().toStdString());
    string password = trim(passwordEdit->text().toStdString());
    string role = trim(roleEdit->text().toStdString());

    if (maNV.empty() || username.empty() || role.empty() || (!isEditMode && password.empty())) {
        QMessageBox::warning(this, "Thieu thong tin", "Cac truong co dau (*) la bat buoc.");
        return;
    }
    if (!isEditMode && dsnv.kiemTraMaNhanVien(maNV)) {
        QMessageBox::warning(this, "Loi", "Ma Nhan Vien da ton tai. Vui long chon ma khac.");
        return;
    }
    saveNhanVienData();
    accept();
}

NhanVien AddEmployeeDialog::getNhanVien() const
{
    return currentNhanVien;
}

void AddEmployeeDialog::onShowPasswordToggled(bool checked)
{
    if (checked) {
        passwordEdit->setEchoMode(QLineEdit::Normal);
    } else {
        passwordEdit->setEchoMode(QLineEdit::Password);
    }
}
