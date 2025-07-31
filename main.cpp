#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

struct Appointment {
    std::string patientName;
    std::string roomType;
    int startHour;
    int endHour;
    std::string date;
    int appointmentId;
    
    Appointment(const std::string& name, const std::string& room, int start, int end, 
                const std::string& d, int id) 
        : patientName(name), roomType(room), startHour(start), endHour(end), date(d), appointmentId(id) {}
};

class HospitalBookingSystem {
private:
    std::vector<Appointment> appointments;
    int nextId;
    const std::string filename = "appointments.txt";
    
    void loadFromFile() {
        std::ifstream file(filename);
        if (!file.is_open()) {
            nextId = 1;
            return;
        }
        
        appointments.clear();
        std::string line;
        int maxId = 0;
        
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string name, room, date;
            int start, end, id;
            
            if (std::getline(ss, name, '|') &&
                std::getline(ss, room, '|') &&
                ss >> start && ss.ignore() &&
                ss >> end && ss.ignore() &&
                std::getline(ss, date, '|') &&
                ss >> id) {
                
                appointments.emplace_back(name, room, start, end, date, id);
                maxId = std::max(maxId, id);
            }
        }
        
        nextId = maxId + 1;
        file.close();
    }
    
    void saveToFile() {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "Error: Could not save appointments to file.\n";
            return;
        }
        
        for (const auto& apt : appointments) {
            file << apt.patientName << "|" << apt.roomType << "|" 
                 << apt.startHour << "|" << apt.endHour << "|" 
                 << apt.date << "|" << apt.appointmentId << "\n";
        }
        
        file.close();
    }
    
    bool hasOverlap(const std::string& room, int start, int end, const std::string& date, int excludeId = -1) {
        for (const auto& apt : appointments) {
            if (apt.roomType == room && apt.date == date && apt.appointmentId != excludeId) {
                if ((start < apt.endHour) && (end > apt.startHour)) {
                    return true;
                }
            }
        }
        return false;
    }
    
    bool isValidTime(int hour) {
        return hour >= 8 && hour <= 18; // Hospital hours: 8 AM to 6 PM
    }
    
    std::string formatTime(int hour) {
        if (hour == 12) return "12:00 PM";
        if (hour > 12) return std::to_string(hour - 12) + ":00 PM";
        if (hour == 0) return "12:00 AM";
        return std::to_string(hour) + ":00 AM";
    }

public:
    HospitalBookingSystem() {
        loadFromFile();
    }
    
    ~HospitalBookingSystem() {
        saveToFile();
    }
    
    void bookAppointment() {
        std::string patientName, roomType, date;
        int startHour, endHour;
        
        std::cout << "\n=== BOOK NEW APPOINTMENT ===\n";
        std::cout << "Patient Name: ";
        std::cin.ignore();
        std::getline(std::cin, patientName);
        
        std::cout << "Room Type (X-ray/CT/MRI): ";
        std::getline(std::cin, roomType);
        
        std::cout << "Date (YYYY-MM-DD): ";
        std::getline(std::cin, date);
        
        std::cout << "Start Hour (8-18, 24-hour format): ";
        std::cin >> startHour;
        
        std::cout << "End Hour (8-18, 24-hour format): ";
        std::cin >> endHour;
        
        // Validation
        if (!isValidTime(startHour) || !isValidTime(endHour)) {
            std::cout << "Error: Hours must be between 8 and 18 (hospital operating hours).\n";
            return;
        }
        
        if (startHour >= endHour) {
            std::cout << "Error: Start time must be before end time.\n";
            return;
        }
        
        if (hasOverlap(roomType, startHour, endHour, date)) {
            std::cout << "Error: Time slot conflicts with existing appointment.\n";
            return;
        }
        
        appointments.emplace_back(patientName, roomType, startHour, endHour, date, nextId++);
        saveToFile();
        
        std::cout << "✓ Appointment booked successfully! ID: " << (nextId - 1) << "\n";
    }
    
    void viewSchedule() {
        std::string date;
        std::cout << "\n=== VIEW SCHEDULE ===\n";
        std::cout << "Enter date (YYYY-MM-DD) or 'all' for all dates: ";
        std::cin.ignore();
        std::getline(std::cin, date);
        
        std::vector<Appointment> filtered;
        
        if (date == "all") {
            filtered = appointments;
        } else {
            for (const auto& apt : appointments) {
                if (apt.date == date) {
                    filtered.push_back(apt);
                }
            }
        }
        
        if (filtered.empty()) {
            std::cout << "No appointments found.\n";
            return;
        }
        
        // Sort by date, then by start time
        std::sort(filtered.begin(), filtered.end(), 
                  [](const Appointment& a, const Appointment& b) {
                      if (a.date != b.date) return a.date < b.date;
                      return a.startHour < b.startHour;
                  });
        
        std::cout << "\n" << std::setw(5) << "ID" 
                  << std::setw(20) << "Patient" 
                  << std::setw(10) << "Room" 
                  << std::setw(12) << "Date"
                  << std::setw(15) << "Time" << "\n";
        std::cout << std::string(62, '-') << "\n";
        
        for (const auto& apt : filtered) {
            std::cout << std::setw(5) << apt.appointmentId
                      << std::setw(20) << apt.patientName
                      << std::setw(10) << apt.roomType
                      << std::setw(12) << apt.date
                      << std::setw(15) << (formatTime(apt.startHour) + "-" + formatTime(apt.endHour))
                      << "\n";
        }
    }
    
    void cancelAppointment() {
        int appointmentId;
        std::cout << "\n=== CANCEL APPOINTMENT ===\n";
        std::cout << "Enter Appointment ID to cancel: ";
        std::cin >> appointmentId;
        
        auto it = std::find_if(appointments.begin(), appointments.end(),
                               [appointmentId](const Appointment& apt) {
                                   return apt.appointmentId == appointmentId;
                               });
        
        if (it != appointments.end()) {
            std::cout << "Cancelling appointment for: " << it->patientName 
                      << " (" << it->roomType << " room on " << it->date << ")\n";
            std::cout << "Are you sure? (y/n): ";
            char confirm;
            std::cin >> confirm;
            
            if (confirm == 'y' || confirm == 'Y') {
                appointments.erase(it);
                saveToFile();
                std::cout << "✓ Appointment cancelled successfully.\n";
            } else {
                std::cout << "Cancellation aborted.\n";
            }
        } else {
            std::cout << "Error: Appointment ID not found.\n";
        }
    }
    
    void showAvailableSlots() {
        std::string roomType, date;
        std::cout << "\n=== CHECK AVAILABLE SLOTS ===\n";
        std::cout << "Room Type (X-ray/CT/MRI): ";
        std::cin.ignore();
        std::getline(std::cin, roomType);
        std::cout << "Date (YYYY-MM-DD): ";
        std::getline(std::cin, date);
        
        std::vector<bool> occupied(11, false); // Hours 8-18 (index 0 = 8AM, index 10 = 6PM)
        
        for (const auto& apt : appointments) {
            if (apt.roomType == roomType && apt.date == date) {
                for (int hour = apt.startHour; hour < apt.endHour; hour++) {
                    if (hour >= 8 && hour < 18) {
                        occupied[hour - 8] = true;
                    }
                }
            }
        }
        
        std::cout << "\nAvailable slots for " << roomType << " room on " << date << ":\n";
        std::cout << std::string(40, '-') << "\n";
        
        bool hasAvailable = false;
        for (int hour = 8; hour < 18; hour++) {
            if (!occupied[hour - 8]) {
                std::cout << formatTime(hour) << " - " << formatTime(hour + 1) << "\n";
                hasAvailable = true;
            }
        }
        
        if (!hasAvailable) {
            std::cout << "No available slots for this room on this date.\n";
        }
    }
    
    void displayMenu() {
        std::cout << "\n╔══════════════════════════════════════╗\n";
        std::cout << "║     HOSPITAL ROOM BOOKING SYSTEM     ║\n";
        std::cout << "╠══════════════════════════════════════╣\n";
        std::cout << "║ 1. Book Appointment                  ║\n";
        std::cout << "║ 2. View Schedule                     ║\n";
        std::cout << "║ 3. Cancel Appointment                ║\n";
        std::cout << "║ 4. Check Available Slots             ║\n";
        std::cout << "║ 5. Exit                              ║\n";
        std::cout << "╚══════════════════════════════════════╝\n";
        std::cout << "Choose an option (1-5): ";
    }
    
    void run() {
        int choice;
        
        std::cout << "Welcome to Hospital Room Booking System!\n";
        std::cout << "Operating Hours: 8:00 AM - 6:00 PM\n";
        std::cout << "Available Rooms: X-ray, CT, MRI\n";
        
        while (true) {
            displayMenu();
            std::cin >> choice;
            
            switch (choice) {
                case 1:
                    bookAppointment();
                    break;
                case 2:
                    viewSchedule();
                    break;
                case 3:
                    cancelAppointment();
                    break;
                case 4:
                    showAvailableSlots();
                    break;
                case 5:
                    std::cout << "Thank you for using Hospital Room Booking System!\n";
                    return;
                default:
                    std::cout << "Invalid option. Please choose 1-5.\n";
            }
            
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore();
            std::cin.get();
        }
    }
};

int main() {
    HospitalBookingSystem system;
    system.run();
    return 0;
}