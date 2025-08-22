#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#define BUILD_DIR "build-out"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("Usage: %s <file1.c> <file2.c> ...\n", argv[0]);
        return 1;
    }

    // создаем build-out если нет
    mkdir(BUILD_DIR, 0755);

    int total = argc - 1;

    printf("\n         M\"\"\"\"\"`'\"\"\"`YM          dP                   d88  \n");
    printf("         M  mm.  mm.  M          88                    88  \n");
    printf("d888888b M  MMM  MMM  M .d8888b. 88  .dP  .d8888b.     88  \n");
    printf("   .d8P' M  MMM  MMM  M 88'  `88 88888\"   88ooood8     88  \n");
    printf(" .Y8P    M  MMM  MMM  M 88.  .88 88  `8b. 88.  ...     88  \n");
    printf("d888888P M  MMM  MMM  M `88888P8 dP   `YP `88888P'    d88P \n");
    printf("         MMMMMMMMMMMMMM                                    \n\n");
    printf("Starting zMake...\n\n");

    sleep(1);

    int compiled = 0;
    for(int i = 1; i < argc; i++) {
        char *file = argv[i];
        const char *baseName = strrchr(file, '/');
        if(baseName) baseName++; else baseName = file;

        // имя бинарника без .c
        char binName[512];
        strncpy(binName, baseName, sizeof(binName));
        char *ext = strrchr(binName, '.');
        if(ext) *ext = 0;

        char outputPath[512];
        snprintf(outputPath, sizeof(outputPath), "%s/%s", BUILD_DIR, binName);

        printf("Compiling C object - %s...\n", baseName);

        // строим команду gcc
        char cmd[1024];
        snprintf(cmd, sizeof(cmd), "gcc \"%s\" -o \"%s\" -lusb -lcurl", file, outputPath);

        int res = system(cmd);
        compiled++;

        printf("[%s%d%%%s] Compiling %s, %d/%d files.\n\n", GREEN, (compiled*100)/total, RESET, baseName, compiled, total);

        if(res != 0) {
            printf("Build failed!\nFile response:\n%s\n", baseName);
            return 1;
        }
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

