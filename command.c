#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

void hienThiThuMucHienTai() {
    system("dir");
}

// Phân tích lệnh và kiểm tra chế độ nền (nếu có dấu &)
void phanTichLenh(char *lenh, char **thamSo, int *cheDoNen) {
    *cheDoNen = 0; // Mặc định không chạy nền
    int i = 0;

    char *token = strtok(lenh, " \n");
    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            *cheDoNen = 1; // Phát hiện chế độ background
        } else {
            thamSo[i++] = token; // Lưu tham số lệnh
        }
        token = strtok(NULL, " \n");
    }
    thamSo[i] = NULL; // Kết thúc danh sách tham số
}

// Hiển thị giá trị hiện tại của biến môi trường PATH.
void xemPath() {
    char *path = getenv("PATH");
    if (path) {
        printf("Current PATH:\n%s\n", path);
    } else {
        fprintf(stderr, "Cannot retrieve the PATH variable.\n");
    }
}

// Thêm đường dẫn vào PATH
void themPath(char *duongDan) {
    char *path = getenv("PATH");
    if (path) {
        char pathMoi[DO_DAI_LENH];
        snprintf(pathMoi, sizeof(pathMoi), "%s;%s", path, duongDan);
        if (_putenv_s("PATH", pathMoi) == 0) {
            printf("Đã thêm '%s' vào PATH.\n", duongDan);
        } else {
            fprintf(stderr, "Không thể thêm '%s' vào PATH.\n", duongDan);
        }
    } else {
        fprintf(stderr, "Không thể lấy biến PATH.\n");
    }
}

void thucHienFileBat(char *tenFile) {
    char lenh[DO_DAI_LENH];
    snprintf(lenh, sizeof(lenh), "cmd /c %s", tenFile);
    system(lenh);
}

// Hiển thị trợ giúp
void hienThiTroGiup() {
    printf("\nWELCOME TO MY SHELL\n");
    printf("List:\n");
    printf("  dir               : List the contents of the current directory.\n");
    printf("  list              : List of the process\n");
    printf("  add              : List of the process\n");
    printf("  kill <pid>        : Kill process PID\n");
    printf("  stop <pid>        : Stop process PID\n");
    printf("  resume <pid>      : Continue process PID\n");
    printf("  path              : Display the current PATH variable.\n");
    printf("  addpath <duongdan>: Add the path .\n");
    printf("  help              : Display help\n");
    printf("  exit              : exit shell\n");
    printf("  <cmd>  : Execute the program given by <cmd>\n");
    printf("  *.bat  : Execute the batch file\n");
    printf("  date/time: Display current date and time\n\n");
}
