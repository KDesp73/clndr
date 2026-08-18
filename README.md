# clndr

A lightweight command-line calendar utility written in C. Display monthly and yearly calendars, count days until or since a date, check leap years, and more.

## Installation

### Build from source

```bash
# Debug build (default)
make all

# Release build
make all type=RELEASE
```

### Install system-wide

```bash
make install    # installs to /usr/local/bin/clndr
make uninstall  # removes the installed binary
```

## Usage

```
clndr <command> [options]
```

### Examples

```bash
# Display this month's calendar
clndr month --today

# Display the calendar for August 2003, highlighting the 27th
clndr month -D27 -M8 -Y2003

# Use a custom date format
clndr month -f"%d/%m/%Y" -d"27/8/2003"

# Show the full year calendar
clndr year -Y2025

# Count days until a date
clndr until -Y2025 -M12 -D25

# Check if a year is a leap year
clndr leap -Y2024

# Print today's date
clndr today
```

## License

This project does not currently include a license file. All rights reserved by the author.

---

Written by [KDesp73](https://github.com/KDesp73)
