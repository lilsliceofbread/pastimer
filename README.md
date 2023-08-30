# Pastimer
A Linux CLI timer to record time spent on specific tasks. Works by recording the time difference between when you start and stop the timer.

# Install

clone the repository

navigate to directory `cd pastimer`

give permissions to bash script `chmod +x ./setup.sh`

run script `sudo ./setup.sh`

# Commands

`pastimer add [task]` adds a task.

`pastimer start [task]` starts timer for a specific task.

`pastimer stop [task] [message]` stops timer and records the task entry along with an optional message/note (use "" quotes around message).

`pastimer list [task]` provides the information and entries for a specific task.