#include "shell.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

TienTrinh dsTienTrinh[MAX_TIENS_TRINH]; // Danh sách tiến trình
int soTienTrinh = 0;                    // Số lượng tiến trình hiện có

// Define function pointer types for NtSuspendProcess and NtResumeProcess
typedef LONG (WINAPI *PFN_NT_SUSPEND_RESUME_PROCESS)(HANDLE ProcessHandle);
PFN_NT_SUSPEND_RESUME_PROCESS NtSuspendProcess = NULL;
PFN_NT_SUSPEND_RESUME_PROCESS NtResumeProcess = NULL;

void loadNtFunctions() {
    HMODULE ntdll = LoadLibrary("ntdll.dll");
    if (ntdll) {
        NtSuspendProcess = (PFN_NT_SUSPEND_RESUME_PROCESS)GetProcAddress(ntdll, "NtSuspendProcess");
        NtResumeProcess = (PFN_NT_SUSPEND_RESUME_PROCESS)GetProcAddress(ntdll, "NtResumeProcess");
    }
    if (!NtSuspendProcess || !NtResumeProcess) {
        fprintf(stderr, "Failed to load NtSuspendProcess or NtResumeProcess from ntdll.dll.\n");
        exit(EXIT_FAILURE);
    }
}

void themTienTrinh(HANDLE handle, DWORD pid, char *name) {
    if (soTienTrinh < MAX_TIENS_TRINH) {
        dsTienTrinh[soTienTrinh].handle = handle;
        dsTienTrinh[soTienTrinh].pid = pid;
        strncpy(dsTienTrinh[soTienTrinh].name, name, sizeof(dsTienTrinh[soTienTrinh].name) - 1);
        dsTienTrinh[soTienTrinh].name[sizeof(dsTienTrinh[soTienTrinh].name) - 1] = '\0';
        dsTienTrinh[soTienTrinh].status = 0; // Mặc định trạng thái Running
        soTienTrinh++;
    } else {
        fprintf(stderr, "Cannot add process. Process list is full.\n");
    }
}

void dungTienTrinh(DWORD pid) {
    for (int i = 0; i < soTienTrinh; i++) {
        if (dsTienTrinh[i].pid == pid) {
            HANDLE hProcess = OpenProcess(PROCESS_SUSPEND_RESUME, FALSE, pid);
            if (hProcess == NULL) {
                fprintf(stderr, "Failed to open process handle for PID %lu. Error code: %lu\n", pid, GetLastError());
                return;
            }
            if (NtSuspendProcess && NtSuspendProcess(hProcess) == 0) { // NtSuspendProcess suspends the process
                dsTienTrinh[i].status = 1; // Đánh dấu trạng thái Stopped
                printf("Process PID %lu has been paused.\n", pid);
            } else {
                fprintf(stderr, "Failed to pause process PID %lu. Error code: %lu\n", pid, GetLastError());
            }
            CloseHandle(hProcess);
            return;
        }
    }
    fprintf(stderr, "Process PID %lu not found.\n", pid);
}

void tiepTucTienTrinh(DWORD pid) {
    for (int i = 0; i < soTienTrinh; i++) {
        if (dsTienTrinh[i].pid == pid) {
            HANDLE hProcess = OpenProcess(PROCESS_SUSPEND_RESUME, FALSE, pid);
            if (hProcess == NULL) {
                fprintf(stderr, "Failed to open process handle for PID %lu. Error code: %lu\n", pid, GetLastError());
                return;
            }
            if (NtResumeProcess && NtResumeProcess(hProcess) == 0) { // NtResumeProcess resumes the process
                dsTienTrinh[i].status = 0; // Đánh dấu trạng thái Running
                printf("Process PID %lu has resumed.\n", pid);
            } else {
                fprintf(stderr, "Failed to resume process PID %lu. Error code: %lu\n", pid, GetLastError());
            }
            CloseHandle(hProcess);
            return;
        }
    }
    fprintf(stderr, "Process PID %lu not found.\n", pid);
}

void killTienTrinh(DWORD pid) {
    for (int i = 0; i < soTienTrinh; i++) {
        if (dsTienTrinh[i].pid == pid) {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE | PROCESS_SUSPEND_RESUME, FALSE, pid);
            if (hProcess == NULL) {
                fprintf(stderr, "Failed to open process handle for PID %lu. Error code: %lu\n", pid, GetLastError());
                return;
            }

            if (dsTienTrinh[i].status == 1 && NtResumeProcess) {
                NtResumeProcess(hProcess);
            }

            if (TerminateProcess(hProcess, 0)) {
                CloseHandle(dsTienTrinh[i].handle);
                CloseHandle(hProcess);
                printf("Process PID %lu has been terminated.\n", pid);
                dsTienTrinh[i] = dsTienTrinh[--soTienTrinh]; // Xóa khỏi danh sách
            } else {
                fprintf(stderr, "Failed to terminate process PID %lu. Error code: %lu\n", pid, GetLastError());
            }
            return;
        }
    }
    fprintf(stderr, "Process PID %lu not found.\n", pid);
}

void lietKeTienTrinh() {
    printf("PID\tProcess Name\tState\n");
    for (int i = 0; i < soTienTrinh; i++) {
        printf("%lu\t%s\t%s\n", dsTienTrinh[i].pid, dsTienTrinh[i].name,
               dsTienTrinh[i].status == 0 ? "Running" : "Stopped");
    }
}

void thucThiTienTrinh(char **thamSo, int cheDoNen) {
    char fullPath[MAX_PATH];
    char customPathCopy[MAX_CUSTOM_PATH];
    int found = 0;

    strncpy(customPathCopy, customPath, sizeof(customPathCopy));
    customPathCopy[sizeof(customPathCopy) - 1] = '\0';

    printf("Custom Path Copy: %s\n", customPathCopy); // Thêm dòng in ra để kiểm tra

    char *dir = strtok(customPathCopy, ";");
    while (dir != NULL) {
        snprintf(fullPath, sizeof(fullPath), "%s\\%s", dir, thamSo[0]);
        if (GetFileAttributes(fullPath) != INVALID_FILE_ATTRIBUTES) {
            found = 1;
            break;
        }
        dir = strtok(NULL, ";");
    }

    if (!found) {
        snprintf(fullPath, sizeof(fullPath), "%s", thamSo[0]);
    }

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;

    if (CreateProcess(NULL, fullPath, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        if (cheDoNen) {
            themTienTrinh(pi.hProcess, pi.dwProcessId, fullPath);
            printf("Process running in background: %s (PID: %lu)\n", fullPath, pi.dwProcessId);
        } else {
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            printf("Foreground process finished: %s\n", fullPath);
        }
        CloseHandle(pi.hThread);
    } else {
        fprintf(stderr, "⚠ Không thể thực thi lệnh '%s'. Lỗi: %lu\n", fullPath, GetLastError());
    }
}
