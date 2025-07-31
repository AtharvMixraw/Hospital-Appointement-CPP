# Hospital Room Appointment Booking System

A command-line C++ program that allows hospital staff to book, view, and cancel room appointments for medical procedures like X-ray, CT, and MRI scans.

## Features

- **Book Appointments**: Schedule room appointments with patient details
- **Prevent Overlapping**: Automatic conflict detection for time slots
- **Cancel Appointments**: Remove existing appointments by ID
- **View Schedule**: Display appointments for specific dates or all dates
- **Check Availability**: View available time slots for specific rooms and dates
- **File Persistence**: Appointments are saved to and loaded from `appointments.txt`

## Room Types Available
- X-ray
- CT
- MRI

## Operating Hours
- 8:00 AM to 6:00 PM (24-hour format: 8-18)

## How to Compile and Run

### Prerequisites
- C++ compiler (g++, clang++, or MSVC)
- VS Code with C++ extension (recommended)

### Compilation Instructions

#### Option 1: Using g++ (Command Line)
```bash
g++ -o hospital_booking main.cpp
./hospital_booking
```

#### Option 2: Using VS Code
1. Open the project folder in VS Code
2. Install the "C/C++" extension by Microsoft if not already installed
3. Press `Ctrl+Shift+P` (or `Cmd+Shift+P` on Mac)
4. Type "C/C++: Edit Configurations" and select it
5. This creates a `.vscode/c_cpp_properties.json` file
6. Press `F5` to build and run, or use the terminal:
   ```bash
   g++ -o hospital_booking main.cpp && ./hospital_booking
   ```

#### Option 3: Using VS Code Tasks (Recommended)
1. Press `Ctrl+Shift+P` and select "Tasks: Configure Task"
2. Choose "Create tasks.json file from template"
3. Select "Others"
4. Replace the content with the provided `tasks.json`
5. Press `Ctrl+Shift+P` and select "Tasks: Run Task"
6. Choose "Build and Run C++"

### Windows Users
If using Windows, replace `./hospital_booking` with `hospital_booking.exe`

## Usage

1. **Start the program**: Run the compiled executable
2. **Main Menu**: Choose from 5 options:
   - Book Appointment
   - View Schedule  
   - Cancel Appointment
   - Check Available Slots
   - Exit

3. **Booking Process**:
   - Enter patient name
   - Select room type (X-ray/CT/MRI)
   - Enter date (YYYY-MM-DD format)
   - Enter start and end hours (8-18)

4. **Data Storage**: All appointments are automatically saved to `appointments.txt`

## Example Usage

```
=== BOOK NEW APPOINTMENT ===
Patient Name: John Doe
Room Type (X-ray/CT/MRI): X-ray
Date (YYYY-MM-DD): 2024-01-15
Start Hour (8-18, 24-hour format): 10
End Hour (8-18, 24-hour format): 11
✓ Appointment booked successfully! ID: 1
```

## File Structure

```
hospital-booking/
├── main.cpp              # Main source code
├── README.md             # This file
├── tasks.json            # VS Code build tasks
├── launch.json           # VS Code debug configuration
└── appointments.txt      # Generated data file (created automatically)
```

## Error Handling

- Prevents double-booking of rooms
- Validates operating hours (8 AM - 6 PM)
- Ensures start time is before end time
- Handles file I/O errors gracefully
- Input validation for all user entries

## Data Format

Appointments are stored in `appointments.txt` with the format:
```
PatientName|RoomType|StartHour|EndHour|Date|AppointmentID
```

Example:
```
John Doe|X-ray|10|11|2024-01-15|1
Jane Smith|MRI|14|16|2024-01-15|2
```