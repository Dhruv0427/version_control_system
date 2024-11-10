
# Custom Version Control System (VCS)

This project is a custom Version Control System (VCS) built in C++. It allows users to add files to a staging area, create commits with a message, and store delta changes to track modifications between different versions of files. The VCS also maintains metadata for tracking file versions, commit messages, and timestamps.

## Features

- **Add Files to Staging Area**: Add files to a temporary staging area to prepare them for committing.
- **Commit Changes**: Create commits with descriptive messages and automatically log them in the `METADATA` file.
- **Track File Versions**: Store each file's delta (differences) in a versioned structure to efficiently track changes over time.
- **Restore File**: To restore file to any of the previous verison
- **Metadata Logging**: Log the number of committed files, commit messages, and timestamps for easy tracking.


## Setup Instructions

1. **Clone the Repository**:
   ```bash
   gh repo clone Dhruv0427/version_control_system 
2. **Build the Project**:
    use this command to build this project
    ```bash
    make

## USAGE:

1. **initialization**
    use this command to initialize version control system
   ```bash
    ./myvcs init
2. **add file** to add file use this command
   ```bash
   ./myvcs add <filename> <filename>
3. **commit file** to commit file use this command
   ```bash
   ./myvcs commit -m "message"
4. **log feature** to view log of the file use command
   ```bash
   ./myvcs log
5. **restore**  to restore any file use this command
   ```bash
   ./myvcs restore
**Restore format**
  to restore file after using above command it will ask for filename and corresponding version to which we want to restore   





