#include "shell.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

TienTrinh dsTienTrinh[MAX_TIENS_TRINH]; // Danh sách tiến trình
int soTienTrinh = 0;                    // Số lượng tiến trình hiện có

// Thêm tiến trình mới vào danh sách quản lý
void themTienTrinh(HANDLE handle, DWORD pid, char *name) {
    if (soTienTrinh < MAX_TIENS_TRINH) {
        dsTienTrinh[soTienTrinh].handle = handle;
        dsTienTrinh[soTienTrinh].pid = pid;
        strcpy(dsTienTrinh[soTienTrinh].name, name);
        dsTienTrinh[soTienTrinh].status = 0; // Trạng thái mặc định là "Running"
        soTienTrinh++;
    } else {
        fprintf(stderr, "Process list is full. Cannot add more processes.\n");
    }
}

// Liệt kê tất cả tiến trình trong danh sách
void lietKeTienTrinh() {
    printf("PID\tProcess Name\tState\n");
    for (int i = 0; i < soTienTrinh; i++) {
        printf("%lu\t%s\t%s\n", dsTienTrinh[i].pid, dsTienTrinh[i].name,
               dsTienTrinh[i].status == 0 ? "Running" : "Stopped");
    }
}

// Tạm dừng một tiến trình dựa vào PID
void dungTienTrinh(DWORD pid) {
    for (int i = 0; i < soTienTrinh; i++) {
        if (dsTienTrinh[i].pid == pid) {
            if (SuspendThread(dsTienTrinh[i].handle) != (DWORD)-1) {
                dsTienTrinh[i].status = 1; // Trạng thái "Stopped"
                printf("Process PID %lu has been paused.\n", pid);
            } else {
                fprintf(stderr, "Failed to pause process PID %lu.\n", pid);
            }
            return;
        }
    }
    fprintf(stderr, "Process PID %lu not found.\n", pid);
}

// Tiếp tục một tiến trình đã bị dừng
void tiepTucTienTrinh(DWORD pid) {
    for (int i = 0; i < soTienTrinh; i++) {
        if (dsTienTrinh[i].pid == pid) {
            if (ResumeThread(dsTienTrinh[i].handle) != (DWORD)-1) {
                dsTienTrinh[i].status = 0; // Trạng thái "Running"
                printf("Process PID %lu has resumed.\n", pid);
            } else {
                fprintf(stderr, "Failed to resume process PID %lu.\n", pid);
            }
            return;
        }
    }
    fprintf(stderr, "Process PID %lu not found.\n", pid);
}

// Dừng hoàn toàn một tiến trình dựa vào PID
void killTienTrinh(DWORD pid) {
    for (int i = 0; i < soTienTrinh; i++) {
        if (dsTienTrinh[i].pid == pid) {
            if (TerminateProcess(dsTienTrinh[i].handle, 0)) {
                CloseHandle(dsTienTrinh[i].handle);
                printf("Process PID %lu has been terminated.\n", pid);

                // Xóa tiến trình khỏi danh sách
                dsTienTrinh[i] = dsTienTrinh[--soTienTrinh];
            } else {
                fprintf(stderr, "Failed to terminate process PID %lu.\n", pid);
            }
            return;
        }
    }
    fprintf(stderr, "Process PID %lu not found.\n", pid);
}

// Thực thi tiến trình mới (foreground hoặc background)
void thucThiTienTrinh(char **thamSo, int cheDoNen) {
    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;

    // Ghép lệnh từ các tham số thành một chuỗi duy nhất
    char lenh[DO_DAI_LENH] = "";
    for (int i = 0; thamSo[i] != NULL; i++) {
        strcat(lenh, thamSo[i]);
        strcat(lenh, " ");
    }

    // Tạo tiến trình
    if (CreateProcess(NULL, lenh, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        if (cheDoNen) { // Background mode
            themTienTrinh(pi.hProcess, pi.dwProcessId, lenh);
            printf("Process running in background: PID = %lu\n", pi.dwProcessId);
        } else { // Foreground mode
            printf("Foreground process is executing...\n");
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            printf("Foreground process has finished.\n");
        }
    } else {
        fprintf(stderr, "Failed to execute command: %s\n", thamSo[0]);
    }
}
