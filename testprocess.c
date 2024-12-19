#include <stdio.h>
#include <windows.h>

int main() {
    printf("Test process started. PID = %lu\n", GetCurrentProcessId());
    for (int i = 0; i < 1000; i++) {
        printf("Working... %d\n", i);
        Sleep(1000); 
    }
    printf("Test process finished.\n");
    return 0;
}
