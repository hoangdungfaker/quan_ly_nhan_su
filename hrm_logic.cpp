#include "hrm_logic.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <iomanip>

// --- Triển khai Hàm tiện ích ---

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}

bool isValidDate(const string& date) {
    if (date.length() != 10) return false;
    if (date[2] != '/' || date[5] != '/') return false;
    try {
        int day = stoi(date.substr(0, 2));
        int month = stoi(date.substr(3, 2));
        int year = stoi(date.substr(6, 4));
        if (year < 1900 || year > 2050 || month < 1 || month > 12) return false;
        int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
            daysInMonth[2] = 29;
        }
        if (day < 1 || day > daysInMonth[month]) return false;
    } catch (...) {
        return false;
    }
    return true;
}

// --- Triển khai Lớp TrinhDo ---
string TrinhDo::getThanhPhanBanThan() const {
    return this->thanhPhanBanThan;
}
string TrinhDo::getTrinhDoVanHoa() const {
    return this->trinhDoVanHoa;
}
string TrinhDo::getChuyenMon() const {
    return this->chuyenMon;
}
string TrinhDo::getNgoaiNgu() const {
    return this->ngoaiNgu;
}
void TrinhDo::setThanhPhanBanThan(const string& s) {
    this->thanhPhanBanThan = s;
}
void TrinhDo::setTrinhDoVanHoa(const string& s) {
    this->trinhDoVanHoa = s;
}
void TrinhDo::setChuyenMon(const string& s) {
    this->chuyenMon = s;
}
void TrinhDo::setNgoaiNgu(const string& s) {
    this->ngoaiNgu = s;
}
void TrinhDo::SaveToFile(ostream& out) const {
    out << this->thanhPhanBanThan << endl;
    out << this->trinhDoVanHoa << endl;
    out << this->chuyenMon << endl;
    out << this->ngoaiNgu << endl;
}
void TrinhDo::LoadFromFile(istream& in) {
    getline(in, this->thanhPhanBanThan);
    getline(in, this->trinhDoVanHoa);
    getline(in, this->chuyenMon);
    getline(in, this->ngoaiNgu);
}

// --- Triển khai Lớp PhongBan ---
PhongBan::PhongBan(string ma, string ten) : maPhongBan(ma), tenPhongBan(ten) {}
string PhongBan::getMaPhongBan() const {
    return this->maPhongBan;
}
string PhongBan::getTenPhongBan() const {
    return this->tenPhongBan;
}
void PhongBan::setMaPhongBan(const string& s) {
    this->maPhongBan = s;
}
void PhongBan::setTenPhongBan(const string& s) {
    this->tenPhongBan = s;
}
void PhongBan::SaveToFile(ostream& out) const {
    out << this->maPhongBan << endl;
    out << this->tenPhongBan << endl;
}
void PhongBan::LoadFromFile(istream& in) {
    getline(in, this->maPhongBan);
    getline(in, this->tenPhongBan);
}

// --- Triển khai Lớp ChucVu ---
ChucVu::ChucVu(string ma, string ten) : maChucVu(ma), tenChucVu(ten) {}
string ChucVu::getMaChucVu() const {
    return this->maChucVu;
}
string ChucVu::getTenChucVu() const {
    return this->tenChucVu;
}
void ChucVu::setMaChucVu(const string& s) {
    this->maChucVu = s;
}
void ChucVu::setTenChucVu(const string& s) {
    this->tenChucVu = s;
}
void ChucVu::SaveToFile(ostream& out) const {
    out << this->maChucVu << endl;
    out << this->tenChucVu << endl;
}
void ChucVu::LoadFromFile(istream& in) {
    getline(in, this->maChucVu);
    getline(in, this->tenChucVu);
}

// --- Triển khai Lớp BaoHiem ---
string BaoHiem::getSoBaoHiem() const {
    return this->soBaoHiem;
}
string BaoHiem::getNoiCap() const {
    return this->noiCap;
}
string BaoHiem::getNoiDKKhamChuaBenh() const {
    return this->noiDKKhamChuaBenh;
}
void BaoHiem::setSoBaoHiem(const string& s) {
    this->soBaoHiem = s;
}
void BaoHiem::setNoiCap(const string& s) {
    this->noiCap = s;
}
void BaoHiem::setNoiDKKhamChuaBenh(const string& s) {
    this->noiDKKhamChuaBenh = s;
}
void BaoHiem::SaveToFile(ostream& out) const {
    out << this->soBaoHiem << endl;
    out << this->noiCap << endl;
    out << this->noiDKKhamChuaBenh << endl;
}
void BaoHiem::LoadFromFile(istream& in) {
    getline(in, this->soBaoHiem);
    getline(in, this->noiCap);
    getline(in, this->noiDKKhamChuaBenh);
}

// --- Triển khai Lớp TaiKhoan ---
string TaiKhoan::getUsername() const {
    return this->username;
}
string TaiKhoan::getPassword() const {
    return this->passwordHash; // Trả về hash
}
string TaiKhoan::getRole() const {
    return this->role;
}
void TaiKhoan::setUsername(const string& s) {
    this->username = s;
}
void TaiKhoan::setPassword(const string& s) {
    this->passwordHash = s; // Lưu hash
}
void TaiKhoan::setRole(const string& s) {
    this->role = s;
}
void TaiKhoan::SaveToFile(ostream& out) const {
    out << this->username << endl;
    out << this->passwordHash << endl;
    out << this->role << endl;
}
void TaiKhoan::LoadFromFile(istream& in) {
    getline(in, this->username);
    getline(in, this->passwordHash);
    getline(in, this->role);
}

// --- Triển khai Lớp HopDong ---
string HopDong::getSoHopDong() const {
    return this->soHopDong;
}
string HopDong::getNgayKy() const {
    return this->ngayKy;
}
string HopDong::getLoaiHopDong() const {
    return this->loaiHopDong;
}
string HopDong::getThoiGianHopDong() const {
    return this->thoiGianHopDong;
}
void HopDong::setSoHopDong(const string& s) {
    this->soHopDong = s;
}
void HopDong::setNgayKy(const string& s) {
    this->ngayKy = s;
}
void HopDong::setLoaiHopDong(const string& s) {
    this->loaiHopDong = s;
}
void HopDong::setThoiGianHopDong(const string& s) {
    this->thoiGianHopDong = s;
}
void HopDong::SaveToFile(ostream& out) const {
    out << this->soHopDong << endl;
    out << this->ngayKy << endl;
    out << this->loaiHopDong << endl;
    out << this->thoiGianHopDong << endl;
}
void HopDong::LoadFromFile(istream& in) {
    getline(in, this->soHopDong);
    getline(in, this->ngayKy);
    getline(in, this->loaiHopDong);
    getline(in, this->thoiGianHopDong);
}

// --- Triển khai Lớp QuaTrinhCongTac ---
string QuaTrinhCongTac::getNgayVaoLam() const {
    return this->ngayVaoLam;
}
string QuaTrinhCongTac::getBoPhan() const {
    return this->boPhan;
}
string QuaTrinhCongTac::getChucVu() const {
    return this->chucVu;
}
void QuaTrinhCongTac::setNgayVaoLam(const string& s) {
    this->ngayVaoLam = s;
}
void QuaTrinhCongTac::setBoPhan(const string& s) {
    this->boPhan = s;
}
void QuaTrinhCongTac::setChucVu(const string& s) {
    this->chucVu = s;
}
void QuaTrinhCongTac::SaveToFile(ostream& out) const {
    out << this->ngayVaoLam << endl;
    out << this->boPhan << endl;
    out << this->chucVu << endl;
}
void QuaTrinhCongTac::LoadFromFile(istream& in) {
    getline(in, this->ngayVaoLam);
    getline(in, this->boPhan);
    getline(in, this->chucVu);
}

// --- Triển khai Lớp ChamCong ---
ChamCong::ChamCong() : soGioThem(0.0) {}
string ChamCong::getNgayCong() const {
    return this->ngayCong;
}
string ChamCong::getPhanCa() const {
    return this->phanCa;
}
double ChamCong::getSoGioThem() const {
    return this->soGioThem;
}
void ChamCong::setNgayCong(const string& s) {
    this->ngayCong = s;
}
void ChamCong::setPhanCa(const string& s) {
    this->phanCa = s;
}
void ChamCong::setSoGioThem(double d) {
    this->soGioThem = d;
}
void ChamCong::SaveToFile(ostream& out) const {
    out << this->ngayCong << endl;
    out << this->phanCa << endl;
    out << this->soGioThem << endl;
}
void ChamCong::LoadFromFile(istream& in) {
    getline(in, this->ngayCong);
    getline(in, this->phanCa);
    if (!(in >> this->soGioThem)) {
        this->soGioThem = 0.0;
    }
    in.ignore(numeric_limits<streamsize>::max(), '\n');
}

// --- Triển khai Lớp Luong ---
Luong::Luong() : luongCoBan(0.0), thuong(0.0), phuCap(0.0), tangCa(0.0), baoHiem(0.0), thueTNCN(0.0), thucLanh(0.0) {}
void Luong::TinhThucLanh() {
    this->thucLanh = this->luongCoBan + this->thuong + this->phuCap + this->tangCa - this->baoHiem - this->thueTNCN;
}
string Luong::getThang() const {
    return this->thang;
}
double Luong::getLuongCoBan() const {
    return this->luongCoBan;
}
double Luong::getThuong() const {
    return this->thuong;
}
double Luong::getPhuCap() const {
    return this->phuCap;
}
double Luong::getTangCa() const {
    return this->tangCa;
}
double Luong::getBaoHiem() const {
    return this->baoHiem;
}
double Luong::getThueTNCN() const {
    return this->thueTNCN;
}
double Luong::getThucLanh() const {
    return this->thucLanh;
}
void Luong::setThang(const string& s) {
    this->thang = s;
}
void Luong::setLuongCoBan(double d) {
    this->luongCoBan = d;
}
void Luong::setThuong(double d) {
    this->thuong = d;
}
void Luong::setPhuCap(double d) {
    this->phuCap = d;
}
void Luong::setTangCa(double d) {
    this->tangCa = d;
}
void Luong::setBaoHiem(double d) {
    this->baoHiem = d;
}
void Luong::setThueTNCN(double d) {
    this->thueTNCN = d;
}
void Luong::SaveToFile(ostream& out) const {
    out << this->thang << endl;
    out << this->luongCoBan << endl;
    out << this->thuong << endl;
    out << this->phuCap << endl;
    out << this->tangCa << endl;
    out << this->baoHiem << endl;
    out << this->thueTNCN << endl;
    // thucLanh không cần lưu, sẽ được tính lại
}
void Luong::LoadFromFile(istream& in) {
    getline(in, this->thang);
    if (!(in >> this->luongCoBan >> this->thuong >> this->phuCap >> this->tangCa >> this->baoHiem >> this->thueTNCN)) {
        this->luongCoBan = this->thuong = this->phuCap = this->tangCa = this->baoHiem = this->thueTNCN = 0.0;
    }
    in.ignore(numeric_limits<streamsize>::max(), '\n');
    this->TinhThucLanh(); // Tính lại thực lãnh sau khi tải
}

// --- Triển khai Lớp DaoTao ---
string DaoTao::getMaKhoaHoc() const {
    return this->maKhoaHoc;
}
string DaoTao::getTenKhoaHoc() const {
    return this->tenKhoaHoc;
}
string DaoTao::getNgayBatDau() const {
    return this->ngayBatDau;
}
string DaoTao::getNgayKetThuc() const {
    return this->ngayKetThuc;
}
string DaoTao::getDonViDaoTao() const {
    return this->donViDaoTao;
}
void DaoTao::setMaKhoaHoc(const string& s) {
    this->maKhoaHoc = s;
}
void DaoTao::setTenKhoaHoc(const string& s) {
    this->tenKhoaHoc = s;
}
void DaoTao::setNgayBatDau(const string& s) {
    this->ngayBatDau = s;
}
void DaoTao::setNgayKetThuc(const string& s) {
    this->ngayKetThuc = s;
}
void DaoTao::setDonViDaoTao(const string& s) {
    this->donViDaoTao = s;
}
void DaoTao::SaveToFile(ostream& out) const {
    out << this->maKhoaHoc << endl;
    out << this->tenKhoaHoc << endl;
    out << this->ngayBatDau << endl;
    out << this->ngayKetThuc << endl;
    out << this->donViDaoTao << endl;
}
void DaoTao::LoadFromFile(istream& in) {
    getline(in, this->maKhoaHoc);
    getline(in, this->tenKhoaHoc);
    getline(in, this->ngayBatDau);
    getline(in, this->ngayKetThuc);
    getline(in, this->donViDaoTao);
}

// --- Triển khai Lớp KhenThuongKyLuat ---
KhenThuongKyLuat::KhenThuongKyLuat() : soTien(0.0) {}
string KhenThuongKyLuat::getLoai() const {
    return this->loai;
}
string KhenThuongKyLuat::getNgay() const {
    return this->ngay;
}
string KhenThuongKyLuat::getNoiDung() const {
    return this->noiDung;
}
string KhenThuongKyLuat::getQuyetDinh() const {
    return this->quyetDinh;
}
double KhenThuongKyLuat::getSoTien() const {
    return this->soTien;
}
void KhenThuongKyLuat::setLoai(const string& s) {
    this->loai = s;
}
void KhenThuongKyLuat::setNgay(const string& s) {
    this->ngay = s;
}
void KhenThuongKyLuat::setNoiDung(const string& s) {
    this->noiDung = s;
}
void KhenThuongKyLuat::setQuyetDinh(const string& s) {
    this->quyetDinh = s;
}
void KhenThuongKyLuat::setSoTien(double d) {
    this->soTien = d;
}
void KhenThuongKyLuat::SaveToFile(ostream& out) const {
    out << this->loai << endl;
    out << this->ngay << endl;
    out << this->noiDung << endl;
    out << this->quyetDinh << endl;
    out << this->soTien << endl;
}
void KhenThuongKyLuat::LoadFromFile(istream& in) {
    getline(in, this->loai);
    getline(in, this->ngay);
    getline(in, this->noiDung);
    getline(in, this->quyetDinh);
    if (!(in >> this->soTien)) {
        this->soTien = 0.0;
    }
    in.ignore(numeric_limits<streamsize>::max(), '\n');
}

// --- Triển khai Lớp NghiPhep ---
string NghiPhep::getLoaiPhep() const {
    return this->loaiPhep;
}
string NghiPhep::getSoNgay() const {
    return this->soNgay;
}
string NghiPhep::getNgayBatDau() const {
    return this->ngayBatDau;
}
string NghiPhep::getNgayKetThuc() const {
    return this->ngayKetThuc;
}
void NghiPhep::setLoaiPhep(const string& s) {
    this->loaiPhep = s;
}
void NghiPhep::setSoNgay(const string& s) {
    this->soNgay = s;
}
void NghiPhep::setNgayBatDau(const string& s) {
    this->ngayBatDau = s;
}
void NghiPhep::setNgayKetThuc(const string& s) {
    this->ngayKetThuc = s;
}
void NghiPhep::SaveToFile(ostream& out) const {
    out << this->loaiPhep << endl;
    out << this->soNgay << endl;
    out << this->ngayBatDau << endl;
    out << this->ngayKetThuc << endl;
}
void NghiPhep::LoadFromFile(istream& in) {
    getline(in, this->loaiPhep);
    getline(in, this->soNgay);
    getline(in, this->ngayBatDau);
    getline(in, this->ngayKetThuc);
}

// --- Triển khai Lớp ConNguoi ---
string ConNguoi::getHoTen() const {
    return this->hoTen;
}
string ConNguoi::getGioiTinh() const {
    return this->gioiTinh;
}
string ConNguoi::getNgaySinh() const {
    return this->ngaySinh;
}
string ConNguoi::getDiaChi() const {
    return this->diaChi;
}
string ConNguoi::getCmnd() const {
    return this->cmnd;
}
string ConNguoi::getQueQuan() const {
    return this->queQuan;
}
string ConNguoi::getNoiOHienTai() const {
    return this->noiOHienTai;
}
string ConNguoi::getDienThoai() const {
    return this->dienThoai;
}
void ConNguoi::setHoTen(const string& s) {
    this->hoTen = s;
}
void ConNguoi::setGioiTinh(const string& s) {
    this->gioiTinh = s;
}
void ConNguoi::setNgaySinh(const string& s) {
    this->ngaySinh = s;
}
void ConNguoi::setDiaChi(const string& s) {
    this->diaChi = s;
}
void ConNguoi::setCmnd(const string& s) {
    this->cmnd = s;
}
void ConNguoi::setQueQuan(const string& s) {
    this->queQuan = s;
}
void ConNguoi::setNoiOHienTai(const string& s) {
    this->noiOHienTai = s;
}
void ConNguoi::setDienThoai(const string& s) {
    this->dienThoai = s;
}

// --- Triển khai Lớp NhanVien ---
NhanVien::NhanVien(string ma) : maNhanVien(ma) {}
string NhanVien::getMaNhanVien() const {
    return this->maNhanVien;
}
string NhanVien::getTenPhongBan() const {
    return this->phongBan.getTenPhongBan();
}
string NhanVien::getUsername() const {
    return this->taiKhoan.getUsername();
}
string NhanVien::getPassword() const {
    return this->taiKhoan.getPassword();
}
string NhanVien::getRole() const {
    return this->taiKhoan.getRole();
}
double NhanVien::getLuongThucLanhMoiNhat() const {
    return this->dsLuong.empty() ? 0.0 : this->dsLuong.back().getThucLanh();
}

PhongBan& NhanVien::getPhongBan() {
    return this->phongBan;
}
ChucVu& NhanVien::getChucVu() {
    return this->chucVu;
}
TrinhDo& NhanVien::getTrinhDo() {
    return this->trinhDo;
}
BaoHiem& NhanVien::getBaoHiem() {
    return this->baoHiem;
}
TaiKhoan& NhanVien::getTaiKhoan() {
    return this->taiKhoan;
}
const PhongBan& NhanVien::getPhongBan() const {
    return this->phongBan;
}
const ChucVu& NhanVien::getChucVu() const {
    return this->chucVu;
}
const TrinhDo& NhanVien::getTrinhDo() const {
    return this->trinhDo;
}
const BaoHiem& NhanVien::getBaoHiem() const {
    return this->baoHiem;
}
const TaiKhoan& NhanVien::getTaiKhoan() const {
    return this->taiKhoan;
}

vector<HopDong>& NhanVien::getDSHopDong() {
    return this->dsHopDong;
}
vector<QuaTrinhCongTac>& NhanVien::getDSQuaTrinhCongTac() {
    return this->dsQuaTrinhCongTac;
}
vector<ChamCong>& NhanVien::getDSChamCong() {
    return this->dsChamCong;
}
vector<Luong>& NhanVien::getDSLuong() {
    return this->dsLuong;
}
vector<DaoTao>& NhanVien::getDSDaoTao() {
    return this->dsDaoTao;
}
vector<KhenThuongKyLuat>& NhanVien::getDSKTKL() {
    return this->dsKTKL;
}
vector<NghiPhep>& NhanVien::getDSNghiPhep() {
    return this->dsNghiPhep;
}

void NhanVien::setMaNhanVien(const string& s) {
    this->maNhanVien = s;
}
void NhanVien::themHopDong(const HopDong& hd) {
    this->dsHopDong.push_back(hd);
}
void NhanVien::themQuaTrinhCongTac(const QuaTrinhCongTac& qtct) {
    this->dsQuaTrinhCongTac.push_back(qtct);
}
void NhanVien::themChamCong(const ChamCong& cc) {
    this->dsChamCong.push_back(cc);
}
void NhanVien::themLuong(const Luong& l) {
    this->dsLuong.push_back(l);
}
void NhanVien::themDaoTao(const DaoTao& dt) {
    this->dsDaoTao.push_back(dt);
}
void NhanVien::themKTKL(const KhenThuongKyLuat& ktkl) {
    this->dsKTKL.push_back(ktkl);
}
void NhanVien::themNghiPhep(const NghiPhep& np) {
    this->dsNghiPhep.push_back(np);
}

void NhanVien::SaveToFile(ostream& out) const {
    out << this->maNhanVien << endl;
    out << this->hoTen << endl;
    out << this->gioiTinh << endl;
    out << this->ngaySinh << endl;
    out << this->diaChi << endl;
    out << this->cmnd << endl;
    out << this->queQuan << endl;
    out << this->noiOHienTai << endl;
    out << this->dienThoai << endl;
    this->phongBan.SaveToFile(out);
    this->chucVu.SaveToFile(out);
    this->trinhDo.SaveToFile(out);
    this->baoHiem.SaveToFile(out);
    this->taiKhoan.SaveToFile(out);

    auto save_vector = [&](const auto& vec) {
        out << vec.size() << endl;
        for (const auto& item : vec)
            item.SaveToFile(out);
    };
    save_vector(this->dsHopDong);
    save_vector(this->dsQuaTrinhCongTac);
    save_vector(this->dsChamCong);
    save_vector(this->dsLuong);
    save_vector(this->dsDaoTao);
    save_vector(this->dsKTKL);
    save_vector(this->dsNghiPhep);
}
void NhanVien::LoadFromFile(istream& in) {
    getline(in, this->maNhanVien);
    if (this->maNhanVien.empty() && in.eof()) return;
    getline(in, this->hoTen);
    getline(in, this->gioiTinh);
    getline(in, this->ngaySinh);
    getline(in, this->diaChi);
    getline(in, this->cmnd);
    getline(in, this->queQuan);
    getline(in, this->noiOHienTai);
    getline(in, this->dienThoai);
    this->phongBan.LoadFromFile(in);
    this->chucVu.LoadFromFile(in);
    this->trinhDo.LoadFromFile(in);
    this->baoHiem.LoadFromFile(in);
    this->taiKhoan.LoadFromFile(in);

    auto load_vector = [&](auto& vec) {
        size_t size;
        if (!(in >> size)) {
            size = 0;
        }
        in.ignore(numeric_limits<streamsize>::max(), '\n');
        vec.resize(size);
        for (auto& item : vec)
            item.LoadFromFile(in);
    };
    load_vector(this->dsHopDong);
    load_vector(this->dsQuaTrinhCongTac);
    load_vector(this->dsChamCong);
    load_vector(this->dsLuong);
    load_vector(this->dsDaoTao);
    load_vector(this->dsKTKL);
    load_vector(this->dsNghiPhep);
}

const vector<HopDong>& NhanVien::getDSHopDong() const {
    return this->dsHopDong;
}
const vector<QuaTrinhCongTac>& NhanVien::getDSQuaTrinhCongTac() const {
    return this->dsQuaTrinhCongTac;
}
const vector<ChamCong>& NhanVien::getDSChamCong() const {
    return this->dsChamCong;
}
const vector<Luong>& NhanVien::getDSLuong() const {
    return this->dsLuong;
}
const vector<DaoTao>& NhanVien::getDSDaoTao() const {
    return this->dsDaoTao;
}
const vector<KhenThuongKyLuat>& NhanVien::getDSKTKL() const {
    return this->dsKTKL;
}
const vector<NghiPhep>& NhanVien::getDSNghiPhep() const {
    return this->dsNghiPhep;
}


// --- Triển khai Lớp DanhSachNhanVien ---
const vector<shared_ptr<NhanVien>>& DanhSachNhanVien::getDanhSach() const {
    return this->dsNhanVien;
}
vector<shared_ptr<NhanVien>>& DanhSachNhanVien::getDanhSach() {
    return this->dsNhanVien;
}

bool DanhSachNhanVien::kiemTraMaNhanVien(const string& maNV) const {
    for (const auto& nv_ptr : this->dsNhanVien) {
        if (nv_ptr->getMaNhanVien() == maNV) return true;
    }
    return false;
}

void DanhSachNhanVien::addNhanVien(NhanVien nv) {
    this->dsNhanVien.push_back(make_shared<NhanVien>(nv));
}

void DanhSachNhanVien::updateNhanVien(const NhanVien& updatedNv) {
    for (auto& nv_ptr : this->dsNhanVien) {
        if (nv_ptr->getMaNhanVien() == updatedNv.getMaNhanVien()) {
            *nv_ptr = updatedNv;
            return;
        }
    }
}

NhanVien* DanhSachNhanVien::findNhanVienByMa(const string& maNV) {
    for (auto& nv_ptr : this->dsNhanVien) {
        if (nv_ptr->getMaNhanVien() == maNV) return nv_ptr.get();
    }
    return nullptr;
}

bool DanhSachNhanVien::deleteNhanVien(const string& maNV) {
    auto it = remove_if(this->dsNhanVien.begin(), this->dsNhanVien.end(), [&](const shared_ptr<NhanVien>& nv_ptr) {
        return nv_ptr->getMaNhanVien() == maNV;
    });
    if (it != this->dsNhanVien.end()) {
        this->dsNhanVien.erase(it, this->dsNhanVien.end());
        return true;
    }
    return false;
}

map<string, pair<double, int>> DanhSachNhanVien::getReportLuongTheoPhongBan() const {
    map<string, pair<double, int>> thongKeLuong;
    for (const auto& nv_ptr : this->dsNhanVien) {
        string tenPB = trim(nv_ptr->getTenPhongBan());
        double luong = nv_ptr->getLuongThucLanhMoiNhat();
        if (tenPB.empty())
            tenPB = "Chua co thong tin Phong ban";
        thongKeLuong[tenPB].first += luong;
        thongKeLuong[tenPB].second++;
    }
    return thongKeLuong;
}

map<string, map<string, int>> DanhSachNhanVien::getReportTheoPhongBanGioiTinh() const {
    map<string, map<string, int>> thongKe;
    for (const auto& nv_ptr : this->dsNhanVien) {
        string tenPB = trim(nv_ptr->getTenPhongBan());
        string gioiTinh = trim(nv_ptr->getGioiTinh());
        if (tenPB.empty()) tenPB = "Khong xac dinh";
        if (gioiTinh.empty()) gioiTinh = "Khong ro";
        thongKe[tenPB][gioiTinh]++;
    }
    return thongKe;
}

void DanhSachNhanVien::SaveToFile(const string& filename) const {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << this->dsNhanVien.size() << endl;
        for (const auto& nv_ptr : this->dsNhanVien)
            nv_ptr->SaveToFile(outFile);
        outFile.close();
    } else {
        // Nên dùng QMessageBox trong GUI để báo lỗi này
    }
}

void DanhSachNhanVien::LoadFromFile(const string& filename) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        this->dsNhanVien.clear();
        size_t size;
        if (!(inFile >> size)) {
            inFile.close();
            return;
        }
        inFile.ignore(numeric_limits<streamsize>::max(), '\n');
        for (size_t i = 0; i < size; ++i) {
            auto nv_ptr = make_shared<NhanVien>();
            nv_ptr->LoadFromFile(inFile);
            if(nv_ptr->getMaNhanVien().empty() && inFile.eof()) {
                break;
            }
            this->dsNhanVien.push_back(nv_ptr);
        }
        inFile.close();
    } else {
        // Nên dùng QMessageBox trong GUI để báo lỗi này
    }
}
