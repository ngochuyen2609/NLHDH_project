#include "shell.h"
#include <stdio.h>
#include <string.h>

char customPath[MAX_CUSTOM_PATH] = ""; 

int main() {
    char lenh[DO_DAI_LENH];
    hienThiTroGiup();  // Hiển thị trợ giúp ban đầu
    caiDatXuLyTinHieu();  // Cài đặt xử lý Ctrl+C

    loadNtFunctions(); // Load các hàm cần thiết từ ntdll.dll
    printf("NtSuspendProcess and NtResumeProcess loaded successfully.\n");

    while (1) {
        char lenh[DO_DAI_LENH];
        char *thamSo[SO_THAM_SO_TOI_DA];
        int cheDoNen;
        
        printf("myShell> ");
        fgets(lenh, DO_DAI_LENH, stdin);
        lenh[strcspn(lenh, "\n")] = 0; // Loại bỏ newline

        if (strcmp(lenh, "exit") == 0) break;
        else if (strcmp(lenh, "help") == 0) hienThiTroGiup();
        else if (strcmp(lenh, "dir") == 0) hienThiThuMucHienTai();
        else if (strcmp(lenh, "list") == 0) lietKeTienTrinh();
        else if (strncmp(lenh, "kill ", 5) == 0) {
            int pid = atoi(lenh + 5);
            if (pid > 0) killTienTrinh(pid);
            else printf("Invalid PID for kill command.\n");
        }
        else if (strncmp(lenh, "stop ", 5) == 0) {
            int pid = atoi(lenh + 5);
            if (pid > 0) dungTienTrinh(pid);
            else printf("Invalid PID for stop command.\n");
        }
        else if (strncmp(lenh, "resume ", 7) == 0) {
            int pid = atoi(lenh + 7);
            if (pid > 0) tiepTucTienTrinh(pid);
            else printf("Invalid PID for resume command.\n");
        }
        else if (strcmp(lenh, "path") == 0) xemPath();
        else if (strncmp(lenh, "addpath ", 8) == 0) themPath(lenh + 8);
        else if (strcmp(lenh, "date") == 0 || strcmp(lenh, "time") == 0) inNgayGio();
        else if (strstr(lenh, ".bat") != NULL) thucHienFileBat(lenh);
        else {
            // Tiến trình con: Tạo tiến trình foreground hoặc background
            phanTichLenh(lenh, thamSo, &cheDoNen);
            if (thamSo[0] != NULL) {
                thucThiTienTrinh(thamSo, cheDoNen);
            } else {
                printf("Invalid command or syntax. Type 'help' to see available commands.\n");
            }
        }
    }



    printf("Thoát shell.\n");
    return 0;
}
