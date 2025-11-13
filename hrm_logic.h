#ifndef HRM_LOGIC_H
#define HRM_LOGIC_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory> // Thêm thư viện này

// Khai báo fstream để dùng trong .cpp
using namespace std;


// --- Giao diện (Interface) ---
class IPersistable {
public:
    virtual void SaveToFile(ostream& out) const = 0;
    virtual void LoadFromFile(istream& in) = 0;
    virtual ~IPersistable() = default;
};

// --- Các hàm tiện ích ---
string trim(const string& s);
bool isValidDate(const string& date);

// --- Các lớp dữ liệu (Model) ---

class TrinhDo : public IPersistable {
private:
    string thanhPhanBanThan;
    string trinhDoVanHoa;
    string chuyenMon;
    string ngoaiNgu;
public:
    string getThanhPhanBanThan() const;
    string getTrinhDoVanHoa() const;
    string getChuyenMon() const;
    string getNgoaiNgu() const;
    void setThanhPhanBanThan(const string& s);
    void setTrinhDoVanHoa(const string& s);
    void setChuyenMon(const string& s);
    void setNgoaiNgu(const string& s);

    void SaveToFile(ostream& out) const override;
    void LoadFromFile(istream& in) override;
};

class PhongBan : public IPersistable {
private:
    string maPhongBan;
    string tenPhongBan;
public:
    PhongBan(string ma = "", string ten = "");
    string getMaPhongBan() const;
    string getTenPhongBan() const;
    void setMaPhongBan(const string& s);
    void setTenPhongBan(const string& s);

    void SaveToFile(ostream& out) const override;
    void LoadFromFile(istream& in) override;
};

class ChucVu : public IPersistable {
private:
    string maChucVu;
    string tenChucVu;
public:
    ChucVu(string ma = "", string ten = "");
    string getMaChucVu() const;
    string getTenChucVu() const;
    void setMaChucVu(const string& s);
    void setTenChucVu(const string& s);

    void SaveToFile(ostream& out) const override;
    void LoadFromFile(istream& in) override;
};

class BaoHiem : public IPersistable {
private:
    string soBaoHiem;
    string noiCap;
    string noiDKKhamChuaBenh;
public:
    string getSoBaoHiem() const;
    string getNoiCap() const;
    string getNoiDKKhamChuaBenh() const;
    void setSoBaoHiem(const string& s);
    void setNoiCap(const string& s);
    void setNoiDKKhamChuaBenh(const string& s);

    void SaveToFile(ostream& out) const override;
    void LoadFromFile(istream& in) override;
};

class TaiKhoan : public IPersistable {
private:
    string username;
    string passwordHash; // Đổi tên để rõ ràng hơn
    string role;
public:
    string getUsername() const;
    string getPassword() const; // Vẫn giữ tên hàm getPassword
    string getRole() const;
    void setUsername(const string& s);
    void setPassword(const string& s); // Hàm này sẽ nhận hash
    void setRole(const string& s);

    void SaveToFile(ostream& out) const override;
    void LoadFromFile(istream& in) override;
};

class HopDong : public IPersistable {
private:
    string soHopDong;
    string ngayKy;
    string loaiHopDong;
    string thoiGianHopDong;
public:
    string getSoHopDong() const;
    string getNgayKy() const;
    string getLoaiHopDong() const;
    string getThoiGianHopDong() const;
    void setSoHopDong(const string& s);
    void setNgayKy(const string& s);
    void setLoaiHopDong(const string& s);
    void setThoiGianHopDong(const string& s);

    void SaveToFile(ostream& out) const override;
    void LoadFromFile(istream& in) override;
};

class QuaTrinhCongTac : public IPersistable {
private:
    string ngayVaoLam;
    string boPhan;
    string chucVu;
public:
    string getNgayVaoLam() const;
    string getBoPhan() const;
    string getChucVu() const;
    void setNgayVaoLam(const string& s);
    void setBoPhan(const string& s);
    void setChucVu(const string& s);

    void SaveToFile(ostream& out) const override;
    void LoadFromFile(istream& in) override;
};

class ChamCong : public IPersistable {
private:
    string ngayCong;
    string phanCa;
    double soGioThem;
public:
    ChamCong(); // Cần constructor để khởi tạo soGioThem = 0
    string getNgayCong() const;
    string getPhanCa() const;
    double getSoGioThem() const;
    void setNgayCong(const string& s);
    void setPhanCa(const string& s);
    void setSoGioThem(double d);

    void SaveToFile(ostream& out) const override;
    void LoadFromFile(istream& in) override;
};

class Luong : public IPersistable {
private:
    string thang;
    double luongCoBan;
    double thuong;
    double phuCap;
    double tangCa;
    double baoHiem;
    double thueTNCN;
    double thucLanh;
public:
    Luong(); // Constructor khởi tạo các giá trị = 0
    void TinhThucLanh();

    // Getters
    string getThang() const;
    double getLuongCoBan() const;
    double getThuong() const;
    double getPhuCap() const;
    double getTangCa() const;
    double getBaoHiem() const;
    double getThueTNCN() const;
    double getThucLanh() const;

    // Setters
    void setThang(const string& s);
    void setLuongCoBan(double d);
    void setThuong(double d);
    void setPhuCap(double d);
    void setTangCa(double d);
    void setBaoHiem(double d);
    void setThueTNCN(double d);

    void SaveToFile(ostream& out) const override;
    void LoadFromFile(istream& in) override;
};

class DaoTao : public IPersistable {
private:
    string maKhoaHoc;
    string tenKhoaHoc;
    string ngayBatDau;
    string ngayKetThuc;
    string donViDaoTao;
public:
    string getMaKhoaHoc() const;
    string getTenKhoaHoc() const;
    string getNgayBatDau() const;
    string getNgayKetThuc() const;
    string getDonViDaoTao() const;
    void setMaKhoaHoc(const string& s);
    void setTenKhoaHoc(const string& s);
    void setNgayBatDau(const string& s);
    void setNgayKetThuc(const string& s);
    void setDonViDaoTao(const string& s);

    void SaveToFile(ostream& out) const override;
    void LoadFromFile(istream& in) override;
};

class KhenThuongKyLuat : public IPersistable {
private:
    string loai;
    string ngay;
    string noiDung;
    string quyetDinh;
    double soTien;
public:
    KhenThuongKyLuat(); // Khởi tạo soTien = 0
    string getLoai() const;
    string getNgay() const;
    string getNoiDung() const;
    string getQuyetDinh() const;
    double getSoTien() const;
    void setLoai(const string& s);
    void setNgay(const string& s);
    void setNoiDung(const string& s);
    void setQuyetDinh(const string& s);
    void setSoTien(double d);

    void SaveToFile(ostream& out) const override;
    void LoadFromFile(istream& in) override;
};

class NghiPhep : public IPersistable {
private:
    string loaiPhep;
    string soNgay;
    string ngayBatDau;
    string ngayKetThuc;
public:
    string getLoaiPhep() const;
    string getSoNgay() const;
    string getNgayBatDau() const;
    string getNgayKetThuc() const;
    void setLoaiPhep(const string& s);
    void setSoNgay(const string& s);
    void setNgayBatDau(const string& s);
    void setNgayKetThuc(const string& s);

    void SaveToFile(ostream& out) const override;
    void LoadFromFile(istream& in) override;
};


// --- Lớp ConNguoi ---
class ConNguoi {
protected:
    string hoTen;
    string gioiTinh;
    string ngaySinh;
    string diaChi;
    string cmnd;
    string queQuan;
    string noiOHienTai;
    string dienThoai;
public:
    string getHoTen() const;
    string getGioiTinh() const;
    string getNgaySinh() const;
    string getDiaChi() const;
    string getCmnd() const;
    string getQueQuan() const;
    string getNoiOHienTai() const;
    string getDienThoai() const;
    void setHoTen(const string& s);
    void setGioiTinh(const string& s);
    void setNgaySinh(const string& s);
    void setDiaChi(const string& s);
    void setCmnd(const string& s);
    void setQueQuan(const string& s);
    void setNoiOHienTai(const string& s);
    void setDienThoai(const string& s);
};

// --- Lớp NhanVien ---
class NhanVien : public ConNguoi, public IPersistable {
private:
    string maNhanVien;
    PhongBan phongBan;
    ChucVu chucVu;
    TrinhDo trinhDo;
    BaoHiem baoHiem;
    TaiKhoan taiKhoan;
    vector<HopDong> dsHopDong;
    vector<QuaTrinhCongTac> dsQuaTrinhCongTac;
    vector<ChamCong> dsChamCong;
    vector<Luong> dsLuong;
    vector<DaoTao> dsDaoTao;
    vector<KhenThuongKyLuat> dsKTKL;
    vector<NghiPhep> dsNghiPhep;
public:
    NhanVien(string ma = "");

    string getMaNhanVien() const;
    string getTenPhongBan() const;
    string getUsername() const;
    string getPassword() const;
    string getRole() const;
    double getLuongThucLanhMoiNhat() const;

    PhongBan& getPhongBan();
    ChucVu& getChucVu();
    TrinhDo& getTrinhDo();
    BaoHiem& getBaoHiem();
    TaiKhoan& getTaiKhoan();
    const PhongBan& getPhongBan() const;
    const ChucVu& getChucVu() const;
    const TrinhDo& getTrinhDo() const;
    const BaoHiem& getBaoHiem() const;
    const TaiKhoan& getTaiKhoan() const;

    // Getters cho các vector
    vector<HopDong>& getDSHopDong();
    vector<QuaTrinhCongTac>& getDSQuaTrinhCongTac();
    vector<ChamCong>& getDSChamCong();
    vector<Luong>& getDSLuong();
    vector<DaoTao>& getDSDaoTao();
    vector<KhenThuongKyLuat>& getDSKTKL();
    vector<NghiPhep>& getDSNghiPhep();

    // SỬA LỖI: Thêm các hàm const overloads
    const vector<HopDong>& getDSHopDong() const;
    const vector<QuaTrinhCongTac>& getDSQuaTrinhCongTac() const;
    const vector<ChamCong>& getDSChamCong() const;
    const vector<Luong>& getDSLuong() const;
    const vector<DaoTao>& getDSDaoTao() const;
    const vector<KhenThuongKyLuat>& getDSKTKL() const;
    const vector<NghiPhep>& getDSNghiPhep() const;

    void setMaNhanVien(const string& s);

    // Các hàm "Them..."
    void themHopDong(const HopDong& hd);
    void themQuaTrinhCongTac(const QuaTrinhCongTac& qtct);
    void themChamCong(const ChamCong& cc);
    void themLuong(const Luong& l);
    void themDaoTao(const DaoTao& dt);
    void themKTKL(const KhenThuongKyLuat& ktkl);
    void themNghiPhep(const NghiPhep& np);

    void SaveToFile(ostream& out) const override;
    void LoadFromFile(istream& in) override;
};

// --- Lớp DanhSachNhanVien ---
class DanhSachNhanVien {
private:
    // THAY ĐỔI: Dùng shared_ptr để quản lý bộ nhớ an toàn
    vector<shared_ptr<NhanVien>> dsNhanVien;

public:
    // THAY ĐỔI: Cập nhật kiểu trả về
    const vector<shared_ptr<NhanVien>>& getDanhSach() const;
    vector<shared_ptr<NhanVien>>& getDanhSach();

    bool kiemTraMaNhanVien(const string& maNV) const;

    // THAY ĐỔI: addNhanVien nhận NhanVien (by value) và tự tạo shared_ptr
    void addNhanVien(NhanVien nv);

    void updateNhanVien(const NhanVien& updatedNv);

    // THAY ĐỔI: Trả về con trỏ thô (raw pointer) an toàn
    NhanVien* findNhanVienByMa(const string& maNV);

    bool deleteNhanVien(const string& maNV);

    map<string, pair<double, int>> getReportLuongTheoPhongBan() const;
    map<string, map<string, int>> getReportTheoPhongBanGioiTinh() const;

    void SaveToFile(const string& filename) const;
    void LoadFromFile(const string& filename);
};

#endif // HRM_LOGIC_H
