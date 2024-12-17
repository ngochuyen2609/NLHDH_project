#include "shell.h"
#include <stdio.h>

void inNgayGio() {
    SYSTEMTIME thoiGian;
    GetLocalTime(&thoiGian);
    printf("Ngày: %02d/%02d/%04d\n", thoiGian.wDay, thoiGian.wMonth, thoiGian.wYear);
    printf("Giờ: %02d:%02d:%02d\n", thoiGian.wHour, thoiGian.wMinute, thoiGian.wSecond);
}

BOOL xuLyTinHieuCtrlC(DWORD loaiTinHieu) {
    if (loaiTinHieu == CTRL_C_EVENT) {
        printf("Nhấn Ctrl+C: Dừng tiến trình foreground...\n");
        return TRUE;
    }
    return FALSE;
}

void caiDatXuLyTinHieu() {
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)xuLyTinHieuCtrlC, TRUE);
}
