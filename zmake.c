#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>

#define BUILD_DIR "build-out"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"
#define BLUE "\033[0;34m"

int main() {
    // создаем build-out если нет
    mkdir(BUILD_DIR, 0755);

    printf(BLUE"\n         M\"\"\"\"\"`'\"\"\"`YM          dP                   d88  \n");
    printf("         M  mm.  mm.  M          88                    88  \n");
    printf("d888888b M  MMM  MMM  M .d8888b. 88  .dP  .d8888b.     88  \n");
    printf("   .d8P' M  MMM  MMM  M 88'  `88 88888\"   88ooood8     88  \n");
    printf(" .Y8P    M  MMM  MMM  M 88.  .88 88  `8b. 88.  ...     88  \n");
    printf("d888888P M  MMM  MMM  M `88888P8 dP   `YP `88888P'    d88P \n");
    printf("         MMMMMMMMMMMMMM                                    \n\n");
    printf("Starting zMake...\n\n"RESET);

    sleep(1);

    DIR *d = opendir(".");
    if(!d) { perror("opendir"); return 1; }

    struct dirent *entry;
    char *files[1024];
    int total = 0;

    while((entry = readdir(d)) != NULL) {
        if(strlen(entry->d_name) > 2) {
            const char *ext = strrchr(entry->d_name, '.');
            if(ext && strcmp(ext, ".c") == 0) {
                files[total++] = strdup(entry->d_name);
            }
        }
    }
    closedir(d);

    if(total == 0) { printf("No .c files found.\n"); return 0; }

    int compiled = 0;
    for(int i = 0; i < total; i++) {
        char *file = files[i];

        // имя бинарника без .c
        char binName[512];
        strncpy(binName, file, sizeof(binName));
        char *ext = strrchr(binName, '.');
        if(ext) *ext = 0;

        char outputPath[512];
        snprintf(outputPath, sizeof(outputPath), "%s/%s", BUILD_DIR, binName);

        printf("Compiling C object - %s...\n", file);

        // строим команду gcc
        char cmd[1024];
        snprintf(cmd, sizeof(cmd), "gcc \"%s\" -o \"%s\" -lusb-1.0 -lcurl", file, outputPath);

        int res = system(cmd);
        compiled++;
        
        printf("[%s%d%%%s] Compiling %s, %d/%d files.\n\n", GREEN, (compiled*100)/total, RESET, file, compiled, total);

        if(res != 0) {
            printf("Build failed!\nFile response:\n%s\n", file);
            return 1;
        }

        free(file);
    }

    // создаем buildinfo
    char host[256];
    gethostname(host, sizeof(host));

    char buildInfoPath[512];
    snprintf(buildInfoPath, sizeof(buildInfoPath), "%s/buildinfo", BUILD_DIR);
    FILE *f = fopen(buildInfoPath, "w");
    if(f) {
        time_t now = time(NULL);
        fprintf(f, "Builder's hostname: %s\n", host);
        fprintf(f, "Build date: %s", ctime(&now));
        fclose(f);
    }

    printf("Build success!\n");

    return 0;
}

