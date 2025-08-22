// ZenithOS Tools Installer V1. Made by zntsproj
// 2024-2025 LICGX.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define BLUE "\033[1;34m"
#define RED  "\033[1;31m"
#define RESET "\033[0m"

#define BIN_DIR "/usr/bin"
#define TOOL_NAME "ztool-installer"

void show_progress_bar(int percent) {
    int bar_width = 30;
    int pos = (percent * bar_width) / 100;
    printf("\r" BLUE "[");
    for(int i = 0; i < bar_width; i++) {
        if(i < pos) printf("#");
        else printf(" ");
    }
    printf("] %3d%%" RESET, percent);
    fflush(stdout);
}

void copy_binary(const char *filename) {
    char src_path[512], dest_path[512];
    snprintf(src_path, sizeof(src_path), "./%s", filename);
    snprintf(dest_path, sizeof(dest_path), "%s/%s", BIN_DIR, filename);

    printf(BLUE "Installing binary %s to %s...\n" RESET, filename, BIN_DIR);

    FILE *src = fopen(src_path, "rb");
    if(!src) { printf(RED "Failed to open %s\n" RESET, src_path); return; }

    FILE *dest = fopen(dest_path, "wb");
    if(!dest) { printf(RED "Failed to write %s\n" RESET, dest_path); fclose(src); return; }

    char buf[4096];
    size_t total = 0, n;
    fseek(src, 0, SEEK_END);
    size_t filesize = ftell(src);
    fseek(src, 0, SEEK_SET);

    while((n = fread(buf, 1, sizeof(buf), src)) > 0) {
        fwrite(buf, 1, n, dest);
        total += n;
        int percent = (int)((total * 100) / filesize);
        if(percent > 100) percent = 100;
        show_progress_bar(percent);
    }

    fclose(src);
    fclose(dest);

    chmod(dest_path, 0755);
    show_progress_bar(100);
    printf("\n" BLUE "%s: OK!\n" RESET, filename);
}

int main() {
    if(geteuid() != 0) { printf(RED "Please run this script with superuser!\n" RESET); return 1; }

    printf(BLUE "ZenithOS Tools Installer V1\n\n" RESET);
    printf("Are you sure you want to install ZenithOS tools? (y/n) ");
    char answer[4];
    if(fgets(answer, sizeof(answer), stdin) == NULL) return 1;
    if(answer[0] != 'y' && answer[0] != 'Y') return 0;

    DIR *dir = opendir(".");
    if(!dir) { printf(RED "Failed to open current directory!\n" RESET); return 1; }

    struct dirent *entry;
    while((entry = readdir(dir)) != NULL) {
        if(entry->d_type == DT_REG) {
            if(strcmp(entry->d_name, TOOL_NAME) == 0) continue;
            copy_binary(entry->d_name);
        }
    }

    closedir(dir);
    printf(BLUE "All binaries installed!\n" RESET);
    return 0;
}

