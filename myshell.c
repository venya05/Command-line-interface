#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/sysctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <pthread.h>

#define MAX_COMMAND_LEN 1024
#define HISTORY_SIZE 50

char history[HISTORY_SIZE][MAX_COMMAND_LEN];
int history_count = 0;


// Add command to history
void add_to_history(const char *command) {
    if (history_count < HISTORY_SIZE) {
        strcpy(history[history_count++], command);
    } else {
        for (int i = 1; i < HISTORY_SIZE; i++) {
            strcpy(history[i - 1], history[i]);
        }
        strcpy(history[HISTORY_SIZE - 1], command);
    }
}

// Show command history
void show_history() {
    printf("=== Command History ===\n");
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i + 1, history[i]);
    }
}


// Clear command history
void clear_history() {
    history_count = 0;
    printf("History cleared.\n");
}

// Find file recursively
void find_file(const char *filename, const char *directory) {
    DIR *dir = opendir(directory);
    if (!dir) {
        perror("Error opening directory");
        return;
    }

    struct dirent *entry;
    char path[MAX_COMMAND_LEN];
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);
        struct stat st;
        stat(path, &st);

        if (S_ISDIR(st.st_mode)) {
            find_file(filename, path);
        } else if (strcmp(entry->d_name, filename) == 0) {
            printf("File found: %s\n", path);
        }
    }

    closedir(dir);
}

// Display CPU info
void show_cpu_info() {
    FILE *fp = popen("sysctl -n machdep.cpu.brand_string", "r");
    if (!fp) {
        perror("Error retrieving CPU info");
        return;
    }

    char cpu_info[256];
    fgets(cpu_info, sizeof(cpu_info), fp);
    printf("CPU Info: %s\n", cpu_info);

    pclose(fp);
}

// Display memory info
void show_memory_info() {
    FILE *fp = popen("vm_stat", "r");
    if (!fp) {
        perror("Error retrieving memory info");
        return;
    }

    printf("Memory Info:\n");
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }

    pclose(fp);
}


// Fetch weather using OpenWeatherMap API
void fetch_weather(const char *city) {
    if (strlen(city) == 0) {
        printf("Please provide a city name.\n");
        return;
    }

    char command[MAX_COMMAND_LEN];
    snprintf(command, sizeof(command),
             "curl -s \"https://api.openweathermap.org/data/2.5/weather?q=%s&appid=677ebda7aeb31546e762469e0e382cc0&units=metric\" | jq '.main.temp, .weather[0].description'",
             city);

    printf("Fetching weather for %s...\n", city);
    system(command);
}
// Kill a process
void kill_process(int pid) {
    if (kill(pid, SIGKILL) == 0) {
        printf("Process %d terminated successfully.\n", pid);
    } else {
        perror("Error terminating process");
    }
}

// Show uptime
void show_uptime() {
    FILE *fp = popen("uptime", "r");
    if (!fp) {
        perror("Error retrieving uptime");
        return;
    }

    char uptime_info[256];
    fgets(uptime_info, sizeof(uptime_info), fp);
    printf("System Uptime: %s", uptime_info);

    pclose(fp);
}

// Rename a file
void rename_file(const char *oldname, const char *newname) {
    if (rename(oldname, newname) == 0) {
        printf("File renamed from '%s' to '%s'.\n", oldname, newname);
    } else {
        perror("Error renaming file");
    }
}

// Shutdown the system
void shutdown_system() {
    printf("Shutting down the system...\n");
    system("sudo shutdown -h now");
}

// Reboot the system
void reboot_system() {
    printf("Rebooting the system...\n");
    system("sudo reboot");
}

// Display file permissions
void file_permissions(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        printf("File permissions for %s: %o\n", filename, st.st_mode & 0777);
    } else {
        perror("Error retrieving file permissions");
    }
}

// Check internet connectivity
void check_network() {
    printf("Checking internet connectivity...\n");
    if (system("ping -c 1 google.com > /dev/null 2>&1") == 0) {
        printf("Internet is working.\n");
    } else {
        printf("No internet connection.\n");
    }
}

// Create a folder
void create_folder(const char *foldername) {
    if (mkdir(foldername, 0777) == 0) {
        printf("Folder '%s' created successfully.\n", foldername);
    } else {
        perror("Error creating folder");
    }
}

// Function to display the help file
void show_help() {
    printf("Available Commands:\n");
    printf("  help               - Show this help file\n");
    printf("  exit               - Exit the program\n");
    printf("  history            - Show command history\n");
    printf("  clear_history      - Clear command history\n");
    printf("  cpu_info           - Display CPU information\n");
    printf("  memory_info        - Display memory information\n");
    printf("  fetch_weather CITY - Fetch weather information for the given city\n");
    printf("  find_file          - Search for a file in a directory\n");
    printf("  rename_file        - Rename a file\n");
    printf("  check_network      - Check network connectivity\n");
    printf("  disk_usage         - Show disk usage information\n");
    printf("  list_files         - List files in a directory\n");
    printf("  processes          - Show running processes\n");
    printf("  create_file        - Create a new file\n");
    printf("  system_info        - Show system information\n");
    printf("  network_interfaces - Show active network interfaces\n");
    printf("  file_type          - Display the file type\n");
    printf("  search_text        - Search for text in a file\n");
    printf("  send_email         - Send an email\n");
    printf("  load_average       - Show system load average\n");
}

// Delete a file
void delete_file(const char *filename) {
    if (remove(filename) == 0) {
        printf("File '%s' deleted successfully.\n", filename);
    } else {
        perror("Error deleting file");
    }
}

// Search text in a file
void search_text(const char *filename, const char *text) {
    char command[MAX_COMMAND_LEN];
    snprintf(command, sizeof(command), "grep '%s' %s", text, filename);
    system(command);
}



// Display disk usage
void show_disk_usage() {
    FILE *fp = popen("df -h", "r");
    if (!fp) {
        perror("Error retrieving disk usage");
        return;
    }

    printf("Disk Usage Info:\n");
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }

    pclose(fp);
}

// List files in directory
void list_files(const char *directory) {
    DIR *dir = opendir(directory);
    if (!dir) {
        perror("Error opening directory");
        return;
    }

    struct dirent *entry;
    printf("Files in directory %s:\n", directory);
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

// Display running processes
void show_processes() {
    FILE *fp = popen("ps aux", "r");
    if (!fp) {
        perror("Error retrieving processes");
        return;
    }

    printf("Running Processes:\n");
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }

    pclose(fp);
}

// Create a new file
void create_file(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Error creating file");
        return;
    }
    printf("File '%s' created successfully.\n", filename);
    fclose(fp);
}

// Show system info
void show_system_info() {
    char info[256];
    FILE *fp = popen("sysctl -a", "r");
    if (!fp) {
        perror("Error retrieving system info");
        return;
    }

    printf("System Info:\n");
    while (fgets(info, sizeof(info), fp)) {
        printf("%s", info);
    }

    pclose(fp);
}

// Show active network interfaces
void show_network_interfaces() {
    FILE *fp = popen("ifconfig", "r");
    if (!fp) {
        perror("Error retrieving network interfaces");
        return;
    }

    printf("Active Network Interfaces:\n");
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }

    pclose(fp);
}

// Search file by name
void search_file_by_name(const char *directory, const char *filename) {
    char command[MAX_COMMAND_LEN];
    snprintf(command, sizeof(command), "find %s -name '%s'", directory, filename);
    system(command);
}

// Show file type
void show_file_type(const char *filename) {
    char command[MAX_COMMAND_LEN];
    snprintf(command, sizeof(command), "file %s", filename);
    system(command);
}

// Send an email
void send_email(const char *recipient, const char *subject, const char *body) {
    char command[MAX_COMMAND_LEN];
    snprintf(command, sizeof(command), "echo '%s' | mail -s '%s' %s", body, subject, recipient);
    system(command);
    printf("Email sent to %s.\n", recipient);
}

// Show system load average
void show_load_average() {
    FILE *fp = popen("uptime", "r");
    if (!fp) {
        perror("Error retrieving load average");
        return;
    }

    char line[256];
    fgets(line, sizeof(line), fp);
    printf("System Load Average: %s", line);

    pclose(fp);
}

int main() {

    printf("==========================================\n");
    printf("      Welcome to MY SHELL!\n");
    printf("  Type 'help' to see available commands.\n");
    printf("==========================================\n\n");

    char command[MAX_COMMAND_LEN];
    while (1) {
        printf("Enter command: ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;  // Remove newline

        // Add to history
        add_to_history(command);

        if (strcmp(command, "exit") == 0) {
            break;
        }
        else if (strcmp(command, "history") == 0) {
            show_history();
        }
        else if (strcmp(command, "clear_history") == 0) {
            clear_history();
        }
        else if (strcmp(command, "disk_usage") == 0) {
            show_disk_usage();
        }
        else if (strcmp(command, "list_files") == 0) {
            char directory[MAX_COMMAND_LEN];
            printf("Enter directory path: ");
            fgets(directory, sizeof(directory), stdin);
            directory[strcspn(directory, "\n")] = 0;
            list_files(directory);
        }
        else if (strcmp(command, "processes") == 0) {
            show_processes();
        }
        else if (strcmp(command, "create_file") == 0) {
            char filename[MAX_COMMAND_LEN];
            printf("Enter filename: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            create_file(filename);
        }
          else if (strncmp(command, "kill_process", 12) == 0) {
           int pid;
            sscanf(command + 13, "%d", &pid); // Extract PID from the command
            kill_process(pid);
}
        else if (strcmp(command, "system_info") == 0) {
            show_system_info();
        }
       
        else if (strstr(command, "check_network") != NULL) {
         check_network();
}
        else if (strstr(command, "fetch_weather") != NULL) {
            char city[MAX_COMMAND_LEN];
           sscanf(command, "fetch_weather %s", city);
           fetch_weather(city);
}
        else if (strcmp(command, "search_file") == 0) {
            char directory[MAX_COMMAND_LEN], filename[MAX_COMMAND_LEN];
            printf("Enter directory path: ");
            fgets(directory, sizeof(directory), stdin);
            directory[strcspn(directory, "\n")] = 0;
            printf("Enter filename to search: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            search_file_by_name(directory, filename);
        }
        else if (strcmp(command, "file_type") == 0) {
            char filename[MAX_COMMAND_LEN];
            printf("Enter filename: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            show_file_type(filename);
        }
        else if (strcmp(command, "send_email") == 0) {
            char recipient[MAX_COMMAND_LEN], subject[MAX_COMMAND_LEN], body[MAX_COMMAND_LEN];
            printf("Enter recipient email: ");
            fgets(recipient, sizeof(recipient), stdin);
            recipient[strcspn(recipient, "\n")] = 0;
            printf("Enter email subject: ");
            fgets(subject, sizeof(subject), stdin);
            subject[strcspn(subject, "\n")] = 0;
            printf("Enter email body: ");
            fgets(body, sizeof(body), stdin);
            body[strcspn(body, "\n")] = 0;
            send_email(recipient, subject, body);
        }
        else if (strcmp(command, "load_average") == 0) {
            show_load_average();
        }
        else if (strcmp(command, "find_file") == 0) {
           char directory[MAX_COMMAND_LEN], filename[MAX_COMMAND_LEN];
            printf("Enter directory path: ");
            fgets(directory, sizeof(directory), stdin);
            directory[strcspn(directory, "\n")] = 0;
            printf("Enter filename to search: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            find_file(filename, directory);
        }
        else if (strcmp(command, "cpu_info") == 0) {
    show_cpu_info();
}
else if (strcmp(command, "memory_info") == 0) {
    show_memory_info();
}
else if (strcmp(command, "rename_file") == 0) {
    char oldname[MAX_COMMAND_LEN], newname[MAX_COMMAND_LEN];
    printf("Enter current filename: ");
    fgets(oldname, sizeof(oldname), stdin);
    oldname[strcspn(oldname, "\n")] = 0;
    printf("Enter new filename: ");
    fgets(newname, sizeof(newname), stdin);
    newname[strcspn(newname, "\n")] = 0;
    rename_file(oldname, newname);
}
else if (strcmp(command, "shutdown") == 0) {
    shutdown_system();
}
else if (strcmp(command, "shutdown") == 0) {
    shutdown_system();
}
else if (strcmp(command, "reboot") == 0) {
    reboot_system();
}
else if (strcmp(command, "file_permissions") == 0) {
    char filename[MAX_COMMAND_LEN];
    printf("Enter filename: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;
    file_permissions(filename);
}
else if (strcmp(command, "delete_file") == 0) {
    char filename[MAX_COMMAND_LEN];
    printf("Enter filename to delete: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;
    delete_file(filename);
}
else if (strcmp(command, "search_text") == 0) {
    char filename[MAX_COMMAND_LEN], text[MAX_COMMAND_LEN];
    printf("Enter filename: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;
    printf("Enter text to search: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = 0;
    search_text(filename, text);
}
else if (strcmp(command, "create_folder") == 0) {
    char foldername[MAX_COMMAND_LEN];
    printf("Enter folder name to create: ");
    fgets(foldername, sizeof(foldername), stdin);
    foldername[strcspn(foldername, "\n")] = 0;
    create_folder(foldername);
}
else if (strcmp(command, "network_interfaces") == 0) {
    show_network_interfaces();
}
else if (strcmp(command, "help") == 0) {
            show_help();  // Call the help function
}
        
    }
    return 0;
}