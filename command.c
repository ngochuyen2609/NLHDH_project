#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void hienThiThuMucHienTai()
{
    system("dir");
}

// Phân tích lệnh và kiểm tra chế độ nền (nếu có dấu &)
void phanTichLenh(char *lenh, char **thamSo, int *cheDoNen)
{
    *cheDoNen = 0; // Mặc định không chạy nền
    int i = 0;

    char *token = strtok(lenh, " \n");
    while (token != NULL)
    {
        if (strcmp(token, "&") == 0)
        {
            *cheDoNen = 1; // Phát hiện chế độ background
        }
        else
        {
            thamSo[i++] = token; // Lưu tham số lệnh
        }
        token = strtok(NULL, " \n");
    }
    thamSo[i] = NULL; // Kết thúc danh sách tham số
}

// Hiển thị giá trị hiện tại của biến môi trường PATH.
void xemPath()
{
    char *path = getenv("PATH");
    if (path)
    {
        printf("CURRENT PATH:\n%s\n", path);
    }
    else
    {
        fprintf(stderr, "⚠ PATH NOT FOUND.\n");
    }
}

// Thêm đường dẫn vào PATH
void themPath(char *duongDan)
{
    if (duongDan == NULL || strlen(duongDan) == 0)
    {
        printf("⚠ INVALID PATH. TRY AGAIN.");
        return;
    }

    // Kiểm tra xem đường dẫn có phải là thư mục hợp lệ
    DWORD attributes = GetFileAttributes(duongDan);
    if (attributes == INVALID_FILE_ATTRIBUTES || !(attributes & FILE_ATTRIBUTE_DIRECTORY))
    {
        fprintf(stderr, "⚠ PAHT '%s' INVALID PATH or PATH NOT EXIST.\n", duongDan);
        return;
    }

    if (strstr(customPath, duongDan))
    {
        printf("⚠ PATH '%s' existed in custom PATH.\n", duongDan);
        return;
    }

    // Thêm đường dẫn vào customPath
    if (strlen(customPath) + strlen(duongDan) + 2 > sizeof(customPath))
    {
        fprintf(stderr, "⚠ Can't add '%s', length of PATH exceed limitation.\n", duongDan);
        return;
    }

    if (strlen(customPath) > 0)
    {
        strcat(customPath, ";");
    }
    strcat(customPath, duongDan);

    printf("✔ ADDED'%s' into custom PATH.\n", duongDan);
    printf("CURRENT PATH:\n%s\n", customPath);
}

void thucHienFileBat(char *tenFile)
{
    char lenh[DO_DAI_LENH];
    snprintf(lenh, sizeof(lenh), "cmd /c %s", tenFile);
    system(lenh);
}

// Hiển thị trợ giúp
void hienThiTroGiup()
{
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
