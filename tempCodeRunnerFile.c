#include "shell.h"
#include <stdio.h>
#include <string.h>

int main()
{
    char lenh[DO_DAI_LENH];
    hienThiTroGiup();    // Hiển thị trợ giúp ban đầu
    caiDatXuLyTinHieu(); // Cài đặt xử lý Ctrl+C

    while (1)
    {
        printf("myShell> ");
        fgets(lenh, DO_DAI_LENH, stdin);
        lenh[strcspn(lenh, "\n")] = 0; // Loại bỏ newline

        if (strcmp(lenh, "exit") == 0)
            break;
        else if (strcmp(lenh, "help") == 0)
            hienThiTroGiup();
        else if (strcmp(lenh, "dir") == 0)
            hienThiThuMucHienTai();
        else if (strcmp(lenh, "list") == 0)
            lietKeTienTrinh();
        else if (strncmp(lenh, "kill ", 5) == 0)
            killTienTrinh(atoi(lenh + 5));
        else if (strncmp(lenh, "stop ", 5) == 0)
            dungTienTrinh(atoi(lenh + 5));
        else if (strncmp(lenh, "resume ", 7) == 0)
            tiepTucTienTrinh(atoi(lenh + 7));
        else if (strcmp(lenh, "path") == 0)
            xemPath();
        else if (strncmp(lenh, "addpath ", 8) == 0)
            themPath(lenh + 8);
        else if (strcmp(lenh, "date") == 0 || strcmp(lenh, "time") == 0)
            inNgayGio();
        else
            printf("INVALID COMMAND.\n");
    }

    printf("EXIT Shell.\n");
    return 0;
}
