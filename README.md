# MY SHELL - Command Line Utility

## Description

MY SHELL is a command-line utility that simulates various shell operations such as file management, system monitoring, and network checks. It provides several built-in commands for interacting with the system, viewing system information, managing files, and more.

The application allows users to interact with a shell-like interface, supporting commands like file searching, memory and CPU information, weather fetching, process management, system reboot and shutdown, and network checks.

## Features

- **Command History**: Keeps track of the last 50 commands.
- **CPU Info**: Displays detailed information about the CPU.
- **Memory Info**: Shows memory statistics from the system.
- **Weather Fetch**: Fetches weather data for a given city from OpenWeatherMap API.
- **Process Management**: Allows killing processes and viewing running processes.
- **File Management**: Supports creating, deleting, renaming, and searching for files.
- **Network and Connectivity**: Checks network connectivity and shows active network interfaces.
- **System Info**: Displays detailed system information, including disk usage, uptime, and more.
- **Email Sending**: Allows sending an email using the command line.

## Commands

Below are the available commands:

### General Commands:
- **help**: Display available commands.
- **exit**: Exit the program.

### File Management:
- **history**: Show command history.
- **clear_history**: Clear the command history.
- **create_file**: Create a new file.
- **rename_file**: Rename an existing file.
- **delete_file**: Delete a specified file.
- **search_text**: Search for a text string in a file.
- **find_file**: Search for a file within a directory.
- **file_permissions**: Display file permissions for a specified file.
- **file_type**: Show the file type of a given file.

### System Information:
- **cpu_info**: Display information about the CPU.
- **memory_info**: Display memory statistics.
- **disk_usage**: Show disk usage statistics.
- **system_info**: Display detailed system information.
- **uptime**: Display the system uptime.
- **load_average**: Show the system load average.
- **network_interfaces**: Show the active network interfaces.

### Process Management:
- **processes**: Display a list of running processes.
- **kill_process**: Terminate a process by its PID.

### Networking:
- **check_network**: Check the internet connectivity by pinging Google.
- **fetch_weather**: Fetch weather data for a specified city.
- **send_email**: Send an email using the specified recipient, subject, and body.

### System Control:
- **shutdown**: Shutdown the system.
- **reboot**: Reboot the system.
- **create_folder**: Create a new directory.

## Setup and Requirements

This program is written in C and works on macOS. To compile and run the program, follow these steps:

### Prerequisites:
- macOS (or Unix-based system)
- GCC or Clang compiler installed
- `jq` (for JSON parsing) installed. You can install it using Homebrew:
- 
### Steps to Compile:
1. Clone the repository
2. Compile the program
3.  Run the program

## Dependencies

- **GCC/Clang**: For compiling the program.
- **jq**: To parse weather API responses in JSON format.
- **curl**: For fetching weather data from the OpenWeatherMap API.

## Notes
- The `fetch_weather` command requires an active internet connection and a valid OpenWeatherMap API key. The default API key is included in the code, but you may want to replace it with your own key for better performance or if you encounter API limit issues.
- Some commands, like `shutdown` and `reboot`, require superuser privileges. You may need to enter your password when prompted.

## Author
- GitHub: [venya05](https://github.com/venya05)
- macOS: This application was developed and tested on macOS.



