
# WASH (Wicked Awesome Shell) — with a silent 'W'

----

## Overview
WASH is a modern, feature-rich terminal shell designed to deepen my understanding of shell and terminal operations. Built using the Notcurses library for a vibrant text user interface (TUI), WASH combines a sleek, contemporary design inspired by the Catppuccin theme with powerful command execution capabilities. The goal of WASH is to provide an intuitive and visually appealing shell experience while allowing me to explore how commands are processed in a terminal environment.

WASH leverages `fork` and `execvp` to execute commands, supporting most standard shell commands. Currently, it handles command output effectively but has limited support for input due to constraints in connecting the stdin descriptor via pipes. and it doesnt scroll Future updates aim to address this limitation.

## Features
- **Notcurses-Powered TUI**: Utilizes the Notcurses library for a modern, visually engaging terminal interface.
- **Parallel Command Execution**: Efficiently processes commands using `fork` and `execvp` for seamless execution.
- **Integrated Git Support**: Built-in functionality for Git commands, enhancing version control workflows.
- **Tab Completion**: Streamlines command input with tab-to-complete functionality.
- **Multi-Tab Interface**: Supports multiple tabs for managing concurrent tasks within the shell.
- **Modern UI Design**: Inspired by the Catppuccin theme, offering a clean and aesthetically pleasing interface.
- **Command History**: Stores and retrieves command history for quick access (search functionality planned for future releases).
- **Planned Features**:
  - Command history search (in progress).
  - Background customization (currently limited by resource constraints).
## demo


https://github.com/user-attachments/assets/6bfcf40d-99de-4bd1-8c49-88a0f820debe


## Shortcuts
The following shortcuts are currently implemented in WASH:

- [x] `Ctrl+A` - Move cursor to the beginning of the line
- [x] `Ctrl+B` - Move cursor backward one character
- [x] `Ctrl+D` - Quit the shell
- [x] `Ctrl+E` - Move cursor to the end of the line
- [x] `Ctrl+F` - Move cursor forward one character
- [x] `Ctrl+s+a` - Create Tab
- [x] `Ctrl+s+x` - Close Tab
- [x] `Ctrl+s+ left arrow ` - Create access left tab
- [x] `Ctrl+s+ right arrow ` - Create access left tab
- [x] `Ctrl+Shift+v` - Paste
- [ ] `Ctrl+K` - Cut all text after the cursor
- [ ] `Ctrl+L` - Clear the screen while preserving the current command
- [ ] `Ctrl+U` - Cut all text before the cursor
- [ ] `Ctrl+R` - Reverse search through command history
- [ ] `Ctrl+s+ :` - execute commands in status line 

## Built-in Commands
The following built-in commands are currently implemented in WASH:

- [x] `exit` - Terminates the shell session
- [x] `cd` - Changes the current working directory
- [x] `history` - Displays the command history
- [x] `echo` - Outputs text or variable values to the terminal
- [x] `env` - Lists environment variables
- [x] `printenv` - Prints the value of a specific environment variable
- [x] `which` - Displays the path of an executable
- [x] `pwd` - Prints the current working directory
- [ ] `kill` - Sends a signal to terminate a process
- [ ] `setenv` - Sets an environment variable
- [ ] `unsetenv` - Removes an environment variable
- [ ] `setpath` - Modifies the PATH environment variable
- [ ] `alias` - Creates or lists command aliases

## Technical Details
- **Command Execution**: WASH uses `fork` and `execvp` to execute commands, enabling compatibility with most standard shell commands.
- **Piping**: The shell currently supports output redirection through pipes but does not fully support stdin due to limitations in the parent-child process communication. This means interactive programs requiring user input may not function as expected.
- **Notcurses Integration**: The TUI is powered by Notcurses, which provides advanced terminal rendering capabilities, including support for colors, cursor positioning, and multi-tab interfaces.
- **Theme**: The UI is styled with inspiration from the Catppuccin theme, offering a modern and visually cohesive experience.

## Limitations
- **Input Support**: Due to current limitations with stdin handling in pipes, WASH only supports command output. Interactive input for programs is not yet available.
- **Background Customization**: Limited by resource constraints, background customization is not currently supported but is planned for future releases.
- **History Search**: While command history is stored, searching through it is not yet implemented.
- **Doesn't Scroll**: Efforts to implement terminal scrolling were unsuccessful due to limited resources for Notcurses.
## Installation
1. Ensure the Notcurses library is installed on your system.
2. Clone the WASH repository:
   ```bash
   git clone https://github.com/Swaraj-1925/Wash.git
   ```
3. Navigate to the project directory and compile:
   ```bash
   cd wash
   make
   ```
4. Run the shell:
   ```bash
   ./wash
   ```

## Contributing
Contributions to WASH are welcome! To contribute:
1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Submit a pull request with a clear description of your changes.

Please ensure your code adheres to the project's coding style and includes appropriate documentation.


