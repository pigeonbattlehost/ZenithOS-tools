#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

#define VENDOR_ID  0x18D1
#define PRODUCT_ID 0x4EE7

#define BLUE "\033[1;34m"
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"

unsigned short custom_vid = VENDOR_ID;
unsigned short custom_pid = PRODUCT_ID;

int devicereset() {
    libusb_context *ctx = NULL;
    libusb_device_handle *dev_handle = NULL;
    int r;

    r = libusb_init(&ctx);
    if(r < 0) return r;

    dev_handle = libusb_open_device_with_vid_pid(ctx, custom_vid, custom_pid);
    if(dev_handle == NULL) {
        libusb_exit(ctx);
        return -1;
    }

    r = libusb_reset_device(dev_handle);

    libusb_close(dev_handle);
    libusb_exit(ctx);
    return r;
}

void print_banner() {
    printf(BLUE
        "░█████████ ░███████   ░████████   \n"
        "      ░██  ░██   ░██  ░██    ░██  \n"
        "     ░██   ░██    ░██ ░██    ░██  \n"
        "   ░███    ░██    ░██ ░████████   \n"
        "  ░██      ░██    ░██ ░██     ░██ \n"
        " ░██       ░██   ░██  ░██     ░██ \n"
        "░█████████ ░███████   ░█████████  \n"
        "                                  \n"
        "ZenithOS Debug Bridge v0.012 by LICGX [zntsproj]\n" RESET
    );
}

void print_version() {
    printf(BLUE
        "01011010 01000100 01000010 \n"
        "01011010 01000100 01000010 \n"
        "01011010 01000100 01000010 \n"
    RESET);
    printf(GREEN
        "ooooooooooo ooooooooo  oooooooooo  \n"
        "88    888    888    88o 888    888 \n"
        "    888      888    888 888oooo88  \n"
        "  888    oo  888    888 888    888 \n"
        "o888oooo888 o888ooo88  o888ooo888  \n"
    RESET);
    printf(BLUE
        "01011010 01000100 01000010 \n"
        "01011010 01000100 01000010 \n"
        "01011010 01000100 01000010 \n" RESET
    );
    printf("ZenithOS Debug Bridge \nversion: 0.012\nOfficial build by LICGX [zntsproj]\n");
}

void set_custom_vid_pid() {
    unsigned int vid, pid;
    int ret;
    while(1) {
        printf("Enter custom VID (hex, without 0x): ");
        ret = scanf("%x", &vid);
        while(getchar() != '\n');
        if(ret == 1) break;
        printf(RED "Invalid VID, try again\n" RESET);
    }
    while(1) {
        printf("Enter custom PID (hex, without 0x): ");
        ret = scanf("%x", &pid);
        while(getchar() != '\n');
        if(ret == 1) break;
        printf(RED "Invalid PID, try again\n" RESET);
    }
    custom_vid = (unsigned short)vid;
    custom_pid = (unsigned short)pid;
    printf(GREEN "Custom VID/PID set to %04X/%04X\n" RESET, custom_vid, custom_pid);
}

void print_help() {
    printf(BLUE
        "Available commands:\n"
        "devicereset  - Reset the device\n"
        "version      - Show ASCII banner and version\n"
        "custompvid   - Set custom VID/PID\n"
        "restart      - Restart the device\n"
        "help         - Show this message\n"
        "exit         - Exit ZDB\n" RESET
    );
}

int main() {
    char cmd[128];
    print_banner();

    while(1) {
        printf(BLUE "zdb>>> " RESET);
        if(fgets(cmd, sizeof(cmd), stdin) == NULL) break;

        cmd[strcspn(cmd, "\n")] = 0;

        if(strlen(cmd) == 0) continue;

        if(strcmp(cmd, "devicereset") == 0) {
            int res = devicereset();
            if(res == 0) printf(GREEN "Device reset successful!\n" RESET);
            else printf(RED "Failed to reset device!\n" RESET);
        } else if(strcmp(cmd, "version") == 0) {
            print_version();
        } else if(strcmp(cmd, "custompvid") == 0) {
            set_custom_vid_pid();
        } else if(strcmp(cmd, "restart") == 0) {
            printf("Restarting device...\n");
            int res = devicereset();
            if(res == 0) printf(GREEN "Device restarted successfully!\n" RESET);
            else printf(RED "Failed to restart device!\n" RESET);
        } else if(strcmp(cmd, "help") == 0) {
            print_help();
        } else if(strcmp(cmd, "exit") == 0) {
            printf("Exiting ZDB...\n");
            break;
        } else {
            printf(RED "Unknown command: %s\n" RESET, cmd);
        }
    }

    return 0;
}

