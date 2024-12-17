#ifndef SHELL_H
#define SHELL_H

#include <windows.h>
#define DO_DAI_LENH 1024
#define SO_THAM_SO_TOI_DA 100
#define MAX_TIENS_TRINH 100

typedef struct {
    HANDLE handle;        
    DWORD pid;            
    char name[DO_DAI_LENH];
    int status;          
} TienTrinh;

void themTienTrinh(HANDLE handle, DWORD pid, char *name);
void lietKeTienTrinh();
void dungTienTrinh(DWORD pid);
void tiepTucTienTrinh(DWORD pid);
void killTienTrinh(DWORD pid);
void thucThiTienTrinh(char **thamSo, int cheDoNen);

// Các hàm trong command.c
void hienThiThuMucHienTai();
void xemPath();
void themPath(char *duongDan);
void thucHienFileBat(char *tenFile);
void hienThiTroGiup();
void phanTichLenh(char *lenh, char **thamSo, int *cheDoNen);

// Các hàm trong utils.c
void caiDatXuLyTinHieu();
void inNgayGio();
BOOL xuLyTinHieuCtrlC(DWORD loaiTinHieu);

#endif
