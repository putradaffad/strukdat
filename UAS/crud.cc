#include <iostream>
#include <mysql/mysql.h>
#include <sstream>

using namespace std;

const char* hostname = "127.0.0.1";
const char* user = "root";
const char* pass = "123";
const char* dbname = "perpus_crud";
unsigned int port = 31168;
const char* unixsocket = NULL;
unsigned long clientflag = 0;

MYSQL* connect_db() {
    MYSQL* conn = mysql_init(0);
    if (conn) {
        conn = mysql_real_connect(conn, hostname, user, pass, dbname, port, unixsocket, clientflag);
        if (conn) {
            cout << "Sukses terhubung ke database." << endl;
        } else {
            cerr << "Koneksi gagal: " << mysql_error(conn) << endl;
        }
    } else {
        cerr << "mysql_init gagal" << endl;
    }
    return conn;
}

void tambah_buku(const string& judul_buku, const string& pengarang, const string& penerbit, const string& tahun_terbit, const string& genre) {
    MYSQL* conn = connect_db();
    if (conn) {
        string query = "INSERT INTO buku (judul_buku, pengarang, penerbit, tahun_terbit, genre, status) VALUES ('" + judul_buku + "', '" + pengarang + "', '" + penerbit + "', '" + tahun_terbit + "', '" + genre + "', 'tersedia')";
        if (mysql_query(conn, query.c_str())) {
            cerr << "Gagal menambahkan buku: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil ditambahkan ke database." << endl;
        }
        mysql_close(conn);
    }
}

void lihat_buku() {
    MYSQL* conn = connect_db();
    if (conn) {
        if (mysql_query(conn, "SELECT * FROM buku")) {
            cerr << "Gagal menampilkan buku: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (res == nullptr) {
            cerr << "Gagal menyimpan hasil: " << mysql_error(conn) << endl;
            mysql_close(conn);
            return;
        }

        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << ", Judul Buku: " << row[1] << ", Pengarang: " << row[2] << ", Penerbit: " << row[3] << ", Tahun Terbit: " << row[4] << ", Genre: " << row[5] << ", Status: " << row[6] << endl;
        }
        mysql_free_result(res);
        mysql_close(conn);
    }
}

void hapus_buku(int id) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream ss;
        ss << "DELETE FROM buku WHERE id = " << id;
        if (mysql_query(conn, ss.str().c_str())) {
            cerr << "Gagal menghapus buku: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil dihapus dari database." << endl;
        }
        mysql_close(conn);
    }
}

void update_buku(int id, const string& judul_buku, const string& pengarang, const string& penerbit, const string& tahun_terbit, const string& genre) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream ss;
        ss << "UPDATE buku SET judul_buku = '" << judul_buku << "', pengarang = '" << pengarang << "', penerbit = '" << penerbit << "', tahun_terbit = '" << tahun_terbit << "', genre = '" << genre << "' WHERE id = " << id;
        if (mysql_query(conn, ss.str().c_str())) {
            cerr << "Gagal memperbarui buku: " << mysql_error(conn) << endl;
        } else {
            cout << "Buku berhasil diperbarui di database." << endl;
        }
        mysql_close(conn);
    }
}

void ambil_buku(int id) {
    MYSQL* conn = connect_db();
    if (conn) {
        stringstream ss;
        ss << "UPDATE buku SET status = 'diambil' WHERE id = " << id << " AND status = 'tersedia'";
        if (mysql_query(conn, ss.str().c_str())) {
            cerr << "Gagal mengupdate status buku: " << mysql_error(conn) << endl;
        } else {
            if (mysql_affected_rows(conn) > 0) {
                cout << "Buku berhasil diambil." << endl;
            } else {
                cout << "Buku tidak tersedia atau sudah diambil." << endl;
            }
        }
        mysql_close(conn);
    }
}

void menu_admin() {
    int pilihan;
    do {
        cout << "=== Menu Admin ===" << endl;
        cout << "1. Tambah Buku Baru" << endl;
        cout << "2. Lihat Daftar Buku" << endl;
        cout << "3. Hapus Buku" << endl;
        cout << "4. Update Informasi Buku" << endl;
        cout << "5. Keluar" << endl;
        cout << "Pilih opsi: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 1: {
                string judul_buku, pengarang, penerbit, tahun_terbit, genre;
                cout << "Judul Buku: ";
                getline(cin, judul_buku);
                cout << "Pengarang: ";
                getline(cin, pengarang);
                cout << "Penerbit: ";
                getline(cin, penerbit);
                cout << "Tahun Terbit: ";
                getline(cin, tahun_terbit);
                cout << "Genre: ";
                getline(cin, genre);
                tambah_buku(judul_buku, pengarang, penerbit, tahun_terbit, genre);
                break;
            }
            case 2:
                lihat_buku();
                break;
            case 3: {
                int id;
                cout << "ID Buku yang akan dihapus: ";
                cin >> id;
                hapus_buku(id);
                break;
            }
            case 4: {
                int id;
                string judul_buku, pengarang, penerbit, tahun_terbit, genre;
                cout << "ID Buku yang akan diperbarui: ";
                cin >> id;
                cin.ignore();
                cout << "Judul Buku baru: ";
                getline(cin, judul_buku);
                cout << "Pengarang baru: ";
                getline(cin, pengarang);
                cout << "Penerbit baru: ";
                getline(cin, penerbit);
                cout << "Tahun Terbit baru: ";
                getline(cin, tahun_terbit);
                cout << "Genre baru: ";
                getline(cin, genre);
                update_buku(id, judul_buku, pengarang, penerbit, tahun_terbit, genre);
                break;
            }
            case 5:
                cout << "Keluar dari menu admin." << endl;
                break;
            default:
                cout << "Pilihan tidak valid." << endl;
                break;
        }
    } while (pilihan != 5);
}

void menu_user() {
    int pilihan;
    do {
        cout << "=== Menu User ===" << endl;
        cout << "1. Lihat Daftar Buku" << endl;
        cout << "2. Ambil Buku" << endl;
        cout << "3. Keluar" << endl;
        cout << "Pilih opsi : ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 1:
                lihat_buku();
                break;
            case 2: {
                int id;
                cout << "ID Buku yang akan diambil: ";
                cin >> id;
                ambil_buku(id);
                break;
            }
            case 3:
                cout << "Keluar dari menu user." << endl;
                break;
            default:
                cout << "Pilihan tidak valid." << endl;
                break;
        }
    } while (pilihan != 3);
}

int main() {
    int role;
    cout << "Masukkan peran " << endl;
    cout << "1. Admin" << endl;
    cout << "2. user" << endl;
    cout << "Pilih 1/2 : ";
    cin >> role;
    cin.ignore();

    if (role == 1) {
        menu_admin();
    } else if (role == 2) {
        menu_user();
    } else {
        cout << "Peran tidak valid." << endl;
    }

    return 0;
}
