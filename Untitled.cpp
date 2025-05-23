#include <iostream>
#include <fstream>
#include <string.h>
#include <ctime>
#include <iomanip>
#include <stdlib.h>
#include <exception>

using namespace std;

// Define a custom exception class for the reservation system
class ReservationException : public exception {
private:
    char message[100];
public:
    ReservationException(const char* msg) { strncpy(message, msg, 99); message[99] = '\0'; }
    virtual const char* what() const throw() { return message; }
};

// Validation functions for input data
bool isValidEmail(const char* email) {
    // Check for @ and . characters in the email
    const char* at = strchr(email, '@');
    if (!at) return false;

    const char* dot = strchr(at, '.');
    return dot && dot > at + 1 && *(dot + 1) != '\0';
}

bool isValidPhone(const char* phone) {
    // Check for proper phone format (10+ digits, can have hyphens)
    int digits = 0;
    for (int i = 0; phone[i] != '\0'; i++) {
        if (isdigit(phone[i])) digits++;
        else if (phone[i] != '-' && phone[i] != ' ' && phone[i] != '+' && phone[i] != '(' && phone[i] != ')')
            return false;
    }
    return digits >= 10;
}
bool isValidName(const char* name) {
    int len = strlen(name);
    if (len < 3) return false; // Require at least 3 characters
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return false;
    }
    return true;
}

bool isValidPassport(const char* passport) {
    // Passport should be at least 6 characters and contain both letters and numbers
    int len = strlen(passport);
    if (len < 6) return false;

    bool hasLetter = false, hasDigit = false;
    for (int i = 0; passport[i] != '\0'; i++) {
        if (isalpha(passport[i])) hasLetter = true;
        else if (isdigit(passport[i])) hasDigit = true;
    }
    return hasLetter && hasDigit;
}

bool isValidDate(const char* date) {
    // Check DD/MM/YYYY format
    int len = strlen(date);
    if (len != 10) return false;
    if (date[2] != '/' || date[5] != '/') return false;

    // Check if all other characters are digits
    for (int i = 0; i < len; i++) {
        if (i != 2 && i != 5 && !isdigit(date[i])) return false;
    }

    // Extract day, month, year
    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');

    // Basic validation
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    if (year < 2023) return false; // Assuming no flights in the past

    // Check days in month (simplified)
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
    if (month == 2) {
        bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if ((leap && day > 29) || (!leap && day > 28)) return false;
    }

    return true;
}

bool isValidTime(const char* time) {
    // Check HH:MM format
    int len = strlen(time);
    if (len != 5) return false;
    if (time[2] != ':') return false;

    // Check if other characters are digits
    for (int i = 0; i < len; i++) {
        if (i != 2 && !isdigit(time[i])) return false;
    }

    // Extract hours and minutes
    int hours = (time[0] - '0') * 10 + (time[1] - '0');
    int minutes = (time[3] - '0') * 10 + (time[4] - '0');

    // Validate range
    if (hours < 0 || hours > 23) return false;
    if (minutes < 0 || minutes > 59) return false;

    return true;
}

// ANSI color codes for beautiful CLI
const char* RESET = "\033[0m", * RED = "\033[31m", * GREEN = "\033[32m", * YELLOW = "\033[33m";
const char* BLUE = "\033[34m", * MAGENTA = "\033[35m", * CYAN = "\033[36m", * BOLD = "\033[1m";

// Constants
const int MAX_USERS = 100, MAX_FLIGHTS = 100, MAX_RESERVATIONS = 500;
const int MAX_TICKETS = 500, MAX_PAYMENTS = 500, MAX_STR_LEN = 50;

// Forward declarations
class User;
class Admin;
class Passenger;
class Flight;
class Ticket;
class Reservation;
class Payment;
class ReservationSystem;

// Base User class
class User {
protected:
    int id;
    char username[MAX_STR_LEN], password[MAX_STR_LEN], name[MAX_STR_LEN];
    char email[MAX_STR_LEN], phone[MAX_STR_LEN];
    bool isAdmin;

public:
    User() : id(0), isAdmin(false) {
        username[0] = password[0] = name[0] = email[0] = phone[0] = '\0';
    }

    User(int id, const char* uname, const char* pwd, const char* n,
        const char* em, const char* ph, bool admin) : id(id), isAdmin(admin) {
        strncpy(username, uname, MAX_STR_LEN - 1); username[MAX_STR_LEN - 1] = '\0';
        strncpy(password, pwd, MAX_STR_LEN - 1); password[MAX_STR_LEN - 1] = '\0';
        strncpy(name, n, MAX_STR_LEN - 1); name[MAX_STR_LEN - 1] = '\0';
        strncpy(email, em, MAX_STR_LEN - 1); email[MAX_STR_LEN - 1] = '\0';
        strncpy(phone, ph, MAX_STR_LEN - 1); phone[MAX_STR_LEN - 1] = '\0';
    }

    virtual ~User() {}

    int getId() const { return id; }
    const char* getUsername() const { return username; }
    const char* getPassword() const { return password; }
    const char* getName() const { return name; }
    bool getIsAdmin() const { return isAdmin; }

    virtual const char* getRole() const = 0;
};

// Admin class
class Admin : public User {
public:
    Admin() : User() { isAdmin = true; }
    Admin(int id, const char* uname, const char* pwd, const char* n,
        const char* em, const char* ph) : User(id, uname, pwd, n, em, ph, true) {
    }
    const char* getRole() const { return "Admin"; }
};

// Passenger class
class Passenger : public User {
private:
    char passportNumber[MAX_STR_LEN], address[MAX_STR_LEN];

public:
    Passenger() : User() {
        isAdmin = false; passportNumber[0] = address[0] = '\0';
    }

    Passenger(int id, const char* uname, const char* pwd, const char* n,
        const char* em, const char* ph, const char* passport, const char* addr)
        : User(id, uname, pwd, n, em, ph, false) {
        strncpy(passportNumber, passport, MAX_STR_LEN - 1); passportNumber[MAX_STR_LEN - 1] = '\0';
        strncpy(address, addr, MAX_STR_LEN - 1); address[MAX_STR_LEN - 1] = '\0';
    }

    const char* getPassportNumber() const { return passportNumber; }
    const char* getAddress() const { return address; }
    const char* getRole() const { return "Passenger"; }
};

// Flight class
class Flight {
private:
    int id, totalSeats, availableSeats;
    char flightNumber[MAX_STR_LEN], origin[MAX_STR_LEN], destination[MAX_STR_LEN];
    char departureDate[MAX_STR_LEN], departureTime[MAX_STR_LEN], airline[MAX_STR_LEN];
    double price;

public:
    Flight() : id(0), totalSeats(0), availableSeats(0), price(0) {
        flightNumber[0] = origin[0] = destination[0] = '\0';
        departureDate[0] = departureTime[0] = airline[0] = '\0';
    }

    Flight(int id, const char* flight, const char* org, const char* dest,
        const char* date, const char* time, double p, int seats, const char* air)
        : id(id), totalSeats(seats), availableSeats(seats), price(p) {
        strncpy(flightNumber, flight, MAX_STR_LEN - 1); flightNumber[MAX_STR_LEN - 1] = '\0';
        strncpy(origin, org, MAX_STR_LEN - 1); origin[MAX_STR_LEN - 1] = '\0';
        strncpy(destination, dest, MAX_STR_LEN - 1); destination[MAX_STR_LEN - 1] = '\0';
        strncpy(departureDate, date, MAX_STR_LEN - 1); departureDate[MAX_STR_LEN - 1] = '\0';
        strncpy(departureTime, time, MAX_STR_LEN - 1); departureTime[MAX_STR_LEN - 1] = '\0';
        strncpy(airline, air, MAX_STR_LEN - 1); airline[MAX_STR_LEN - 1] = '\0';
    }

    int getId() const { return id; }
    const char* getFlightNumber() const { return flightNumber; }
    const char* getOrigin() const { return origin; }
    const char* getDestination() const { return destination; }
    const char* getDepartureDate() const { return departureDate; }
    const char* getDepartureTime() const { return departureTime; }
    const char* getAirline() const { return airline; }
    double getPrice() const { return price; }
    int getTotalSeats() const { return totalSeats; }
    int getAvailableSeats() const { return availableSeats; }

    void setId(int i) { id = i; }
    bool bookSeat() { if (availableSeats > 0) { availableSeats--; return true; } return false; }
    void cancelSeat() { if (availableSeats < totalSeats) availableSeats++; }

    void display() const {
        cout << BOLD << "Flight Details:" << RESET << endl;
        cout << "Flight ID: " << id << endl << "Flight Number: " << flightNumber << endl;
        cout << "Airline: " << airline << endl << "Origin: " << origin << endl;
        cout << "Destination: " << destination << endl;
        cout << "Departure: " << departureDate << " at " << departureTime << endl;
        cout << "Price: $" << fixed << setprecision(2) << price << endl;
        cout << "Available Seats: " << availableSeats << "/" << totalSeats << endl;
    }

    void displayRow() const {
        cout << left << setw(5) << id << setw(10) << flightNumber << setw(15) << airline
            << setw(15) << origin << setw(15) << destination << setw(15) << departureDate
            << setw(10) << departureTime << setw(7) << availableSeats
            << "$" << fixed << setprecision(2) << price << endl;
    }
};

// Reservation class
class Reservation {
private:
    int id, passengerId, flightId;
    char date[MAX_STR_LEN], status[MAX_STR_LEN]; // "Confirmed" or "Cancelled"

public:
    Reservation() : id(0), passengerId(0), flightId(0) {
        date[0] = '\0'; strcpy(status, "Confirmed");
    }

    Reservation(int i, int pid, int fid, const char* d)
        : id(i), passengerId(pid), flightId(fid) {
        strncpy(date, d, MAX_STR_LEN - 1); date[MAX_STR_LEN - 1] = '\0';
        strcpy(status, "Confirmed");
    }

    int getId() const { return id; }
    int getPassengerId() const { return passengerId; }
    int getFlightId() const { return flightId; }
    const char* getStatus() const { return status; }

    void setStatus(const char* s) {
        strncpy(status, s, MAX_STR_LEN - 1); status[MAX_STR_LEN - 1] = '\0';
    }
};

// Payment class
class Payment {
private:
    int id, reservationId;
    double amount;
    char date[MAX_STR_LEN], method[MAX_STR_LEN], status[MAX_STR_LEN]; // "Completed" or "Refunded"

public:
    Payment() : id(0), reservationId(0), amount(0) {
        date[0] = method[0] = '\0'; strcpy(status, "Completed");
    }

    Payment(int i, int rid, double amt, const char* d, const char* m)
        : id(i), reservationId(rid), amount(amt) {
        strncpy(date, d, MAX_STR_LEN - 1); date[MAX_STR_LEN - 1] = '\0';
        strncpy(method, m, MAX_STR_LEN - 1); method[MAX_STR_LEN - 1] = '\0';
        strcpy(status, "Completed");
    }

    int getReservationId() const { return reservationId; }
    const char* getStatus() const { return status; }

    void setStatus(const char* s) {
        strncpy(status, s, MAX_STR_LEN - 1); status[MAX_STR_LEN - 1] = '\0';
    }
};

// Ticket class
class Ticket {
private:
    int id, reservationId, passengerId, flightId;
    char seatNumber[MAX_STR_LEN], ticketNumber[MAX_STR_LEN], issueDate[MAX_STR_LEN], status[MAX_STR_LEN];

public:
    Ticket() : id(0), reservationId(0), passengerId(0), flightId(0) {
        seatNumber[0] = ticketNumber[0] = issueDate[0] = '\0';
        strcpy(status, "Confirmed");
    }

    Ticket(int i, int rid, int pid, int fid, const char* seat, const char* tnum, const char* date)
        : id(i), reservationId(rid), passengerId(pid), flightId(fid) {
        strncpy(seatNumber, seat, MAX_STR_LEN - 1); seatNumber[MAX_STR_LEN - 1] = '\0';
        strncpy(ticketNumber, tnum, MAX_STR_LEN - 1); ticketNumber[MAX_STR_LEN - 1] = '\0';
        strncpy(issueDate, date, MAX_STR_LEN - 1); issueDate[MAX_STR_LEN - 1] = '\0';
        strcpy(status, "Confirmed");
    }

    int getId() const { return id; }
    int getReservationId() const { return reservationId; }
    int getPassengerId() const { return passengerId; }
    int getFlightId() const { return flightId; }
    const char* getSeatNumber() const { return seatNumber; }
    const char* getTicketNumber() const { return ticketNumber; }
    const char* getStatus() const { return status; }

    void setStatus(const char* s) {
        strncpy(status, s, MAX_STR_LEN - 1); status[MAX_STR_LEN - 1] = '\0';
    }

    void printTicket(const Flight& flight, const Passenger& passenger) const {
        cout << CYAN << "----------------------------------------------------------------\n"
            << "             BOARDING PASS - " << flight.getAirline() << "\n"
            << "----------------------------------------------------------------\n" << RESET;

        cout << BOLD << "TICKET: " << RESET << ticketNumber << "\n";
        cout << BOLD << "STATUS: " << RESET << status << "\n\n";
        cout << BOLD << "PASSENGER: " << RESET << passenger.getName() << "\n";
        cout << BOLD << "FLIGHT: " << RESET << flight.getFlightNumber() << "\n";
        cout << BOLD << "SEAT: " << RESET << seatNumber << "\n\n";
        cout << BOLD << "FROM: " << RESET << flight.getOrigin() << "\n";
        cout << BOLD << "TO: " << RESET << flight.getDestination() << "\n";
        cout << BOLD << "DATE: " << RESET << flight.getDepartureDate() << "\n";
        cout << BOLD << "TIME: " << RESET << flight.getDepartureTime() << "\n\n";

        cout << CYAN << "----------------------------------------------------------------\n"
            << "           Thank you for flying with " << flight.getAirline() << "!\n"
            << "----------------------------------------------------------------\n" << RESET;
    }
};

// Function declarations
void clearScreen();
void pause();
void displayLoginMenu();
void displayAdminMenu();
void displayPassengerMenu();

class ReservationSystem {
private:
    User* users[MAX_USERS];
    Flight flights[MAX_FLIGHTS];
    Reservation reservations[MAX_RESERVATIONS];
    Payment payments[MAX_PAYMENTS];
    Ticket tickets[MAX_TICKETS];

    int userCount, flightCount, reservationCount, paymentCount, ticketCount;
    int nextUserId, nextFlightId, nextReservationId, nextPaymentId, nextTicketId;
    User* currentUser;

    // Helper functions
    void generateSeatNumber(char* seat) {
        static const char rows[] = "ABCDEFGHJK";
        int row = rand() % 10;
        int seatNum = rand() % 30 + 1;
        sprintf(seat, "%c%d", rows[row], seatNum);
    }

    void generateTicketNumber(char* ticket) {
        strcpy(ticket, "TKT");
        for (int i = 3; i < 9; i++) ticket[i] = '0' + rand() % 10;
        ticket[9] = '\0';
    }

    void getCurrentDate(char* date) {
        time_t now = time(NULL);
        struct tm* ltm = localtime(&now);
        sprintf(date, "%02d/%02d/%04d", ltm->tm_mday, ltm->tm_mon + 1, 1900 + ltm->tm_year);
    }

    // File handling
    void saveUserToFile(ofstream& file, User* user) {
        if (user->getIsAdmin()) {
            file << "ADMIN," << user->getId() << "," << user->getUsername()
                << "," << user->getPassword() << "," << user->getName() << endl;
        }
        else {
            Passenger* passenger = static_cast<Passenger*>(user);
            file << "PASSENGER," << user->getId() << "," << user->getUsername()
                << "," << user->getPassword() << "," << user->getName()
                << "," << passenger->getPassportNumber() << "," << passenger->getAddress() << endl;
        }
    }

    void saveFlightToFile(ofstream& file, const Flight& flight) {
        file << flight.getId() << "," << flight.getFlightNumber() << ","
            << flight.getOrigin() << "," << flight.getDestination() << ","
            << flight.getDepartureDate() << "," << flight.getDepartureTime() << ","
            << flight.getAirline() << "," << flight.getPrice() << ","
            << flight.getTotalSeats() << "," << flight.getAvailableSeats() << endl;
    }

    void saveReservationToFile(ofstream& file, const Reservation& res) {
        file << res.getId() << "," << res.getPassengerId() << ","
            << res.getFlightId() << "," << res.getStatus() << endl;
    }

    void saveTicketToFile(ofstream& file, const Ticket& ticket) {
        file << ticket.getId() << "," << ticket.getReservationId() << ","
            << ticket.getPassengerId() << "," << ticket.getFlightId() << ","
            << ticket.getSeatNumber() << "," << ticket.getTicketNumber() << ","
            << ticket.getStatus() << endl;
    }

    void savePaymentToFile(ofstream& file, const Payment& payment) {
        file << payment.getReservationId() << "," << payment.getStatus() << endl;
    }

public:
    ReservationSystem() : userCount(0), flightCount(0), reservationCount(0),
        paymentCount(0), ticketCount(0), currentUser(NULL) {
        nextUserId = nextFlightId = nextReservationId = nextPaymentId = nextTicketId = 1;

        // Create default admin
        Admin* admin = new Admin(nextUserId++, "admin", "admin123", "System Admin",
            "admin@airline.com", "123-456-7890");
        users[userCount++] = admin;

        // Add 10 default flights
        addDefaultFlights();
    }

    ~ReservationSystem() {
        for (int i = 0; i < userCount; i++) delete users[i];
    }

    void addDefaultFlights() {
        // Create 10 default flights with different routes
        const char* airlines[] = { "AirIndia", "IndiGo", "SpiceJet", "Vistara", "Emirates" };
        const char* origins[] = { "Delhi", "Mumbai", "Bangalore", "Chennai", "Kolkata" };
        const char* destinations[] = { "New York", "London", "Tokyo", "Dubai", "Singapore", "Paris", "Sydney" };
        const char* dates[] = { "10/06/2023", "15/06/2023", "20/06/2023", "25/06/2023", "30/06/2023" };
        const char* times[] = { "08:00", "10:30", "14:45", "19:20", "22:10" };

        // Set up specific flights to ensure variety
        struct FlightData {
            const char* flight;
            const char* airline;
            const char* origin;
            const char* dest;
            const char* date;
            const char* time;
            double price;
            int seats;
        };

        FlightData predefinedFlights[] = {
            {"AI101", "AirIndia", "Delhi", "New York", "15/06/2023", "10:30", 1200.0, 300},
            {"IG202", "IndiGo", "Mumbai", "London", "20/06/2023", "08:00", 950.0, 220},
            {"SJ303", "SpiceJet", "Bangalore", "Tokyo", "25/06/2023", "22:10", 1050.0, 250},
            {"VS404", "Vistara", "Chennai", "Dubai", "10/06/2023", "14:45", 650.0, 180},
            {"EK505", "Emirates", "Kolkata", "Singapore", "30/06/2023", "19:20", 750.0, 200},
            {"AI606", "AirIndia", "Mumbai", "Paris", "15/06/2023", "08:00", 1100.0, 280},
            {"IG707", "IndiGo", "Delhi", "Dubai", "20/06/2023", "14:45", 600.0, 190},
            {"SJ808", "SpiceJet", "Chennai", "London", "25/06/2023", "10:30", 1000.0, 240},
            {"VS909", "Vistara", "Bangalore", "Singapore", "30/06/2023", "22:10", 800.0, 210},
            {"EK010", "Emirates", "Kolkata", "Sydney", "10/06/2023", "19:20", 1300.0, 320}
        };

        for (int i = 0; i < 10 && i < MAX_FLIGHTS; i++) {
            Flight flight(
                nextFlightId++,
                predefinedFlights[i].flight,
                predefinedFlights[i].origin,
                predefinedFlights[i].dest,
                predefinedFlights[i].date,
                predefinedFlights[i].time,
                predefinedFlights[i].price,
                predefinedFlights[i].seats,
                predefinedFlights[i].airline
            );
            flights[flightCount++] = flight;
        }
    }

    void saveDataToFiles() {
        try {
            // Save users
            ofstream userFile("users.dat");
            if (!userFile) throw ReservationException("Cannot open users.dat for writing");
            for (int i = 0; i < userCount; i++) saveUserToFile(userFile, users[i]);
            userFile.close();

            // Save flights
            ofstream flightFile("flights.dat");
            if (!flightFile) throw ReservationException("Cannot open flights.dat for writing");
            for (int i = 0; i < flightCount; i++) saveFlightToFile(flightFile, flights[i]);
            flightFile.close();

            // Save reservations
            ofstream resFile("reservations.dat");
            if (!resFile) throw ReservationException("Cannot open reservations.dat for writing");
            for (int i = 0; i < reservationCount; i++) saveReservationToFile(resFile, reservations[i]);
            resFile.close();

            // Save tickets
            ofstream ticketFile("tickets.dat");
            if (!ticketFile) throw ReservationException("Cannot open tickets.dat for writing");
            for (int i = 0; i < ticketCount; i++) saveTicketToFile(ticketFile, tickets[i]);
            ticketFile.close();

            // Save payments
            ofstream paymentFile("payments.dat");
            if (!paymentFile) throw ReservationException("Cannot open payments.dat for writing");
            for (int i = 0; i < paymentCount; i++) savePaymentToFile(paymentFile, payments[i]);
            paymentFile.close();
        }
        catch (const exception& e) {
            throw ReservationException(e.what());
        }
    }

    void loadDataFromFiles() {
        try {
            // Load users (implementation simplified)
            ifstream userFile("users.dat");
            if (!userFile) throw ReservationException("Users file not found - starting with new database");

            char line[256];
            while (userFile.getline(line, sizeof(line)) && userCount < MAX_USERS) {
                // Parse and load user data
                // Implementation details skipped for brevity
            }
            userFile.close();

            // Load other data similarly
        }
        catch (const exception& e) {
            throw ReservationException(e.what());
        }
    }

    bool login(const char* username, const char* password) {
        for (int i = 0; i < userCount; i++) {
            if (strcmp(users[i]->getUsername(), username) == 0 &&
                strcmp(users[i]->getPassword(), password) == 0) {
                currentUser = users[i];
                return true;
            }
        }
        return false;
    }

    void logout() { currentUser = NULL; }
    bool isLoggedIn() const { return currentUser != NULL; }
    bool isAdmin() const { return currentUser != NULL && currentUser->getIsAdmin(); }
    User* getCurrentUser() const { return currentUser; }

    bool registerPassenger(const char* username, const char* password, const char* name,
        const char* email, const char* phone, const char* passport,
        const char* address) {
        if (userCount >= MAX_USERS) return false;

        // Validate inputs
        if (!isValidName(name)) throw ReservationException("Invalid full name. Use only letters and spaces.");
        if (!isValidEmail(email)) throw ReservationException("Invalid email format");
        if (!isValidPhone(phone)) throw ReservationException("Invalid phone number format");
        if (!isValidPassport(passport)) throw ReservationException("Invalid passport format");

        // Check if username exists
        for (int i = 0; i < userCount; i++)
            if (strcmp(users[i]->getUsername(), username) == 0) return false;

        Passenger* passenger = new Passenger(nextUserId++, username, password, name,
            email, phone, passport, address);
        users[userCount++] = passenger;
        return true;
    }

    void displayAllFlights() const {
        if (flightCount == 0) {
            cout << YELLOW << "No flights available." << RESET << endl;
            return;
        }

        cout << BOLD << CYAN << "-------------------------------------------------------------------------\n"
            << "                        AVAILABLE FLIGHTS                                \n"
            << "-------------------------------------------------------------------------\n" << RESET;

        cout << BOLD << left
            << setw(5) << "ID" << setw(10) << "Flight#" << setw(15) << "Airline"
            << setw(15) << "Origin" << setw(15) << "Destination" << setw(15) << "Date"
            << setw(10) << "Time" << setw(7) << "Seats" << "Price" << endl
            << "-------------------------------------------------------------------------\n" << RESET;

        for (int i = 0; i < flightCount; i++) flights[i].displayRow();

        cout << CYAN << "-------------------------------------------------------------------------\n" << RESET;
    }

    bool addFlight(const char* flightNumber, const char* origin, const char* destination,
        const char* date, const char* time, const char* airline,
        double price, int seats) {
        if (flightCount >= MAX_FLIGHTS) return false;

        // Validate inputs
        if (!isValidDate(date)) throw ReservationException("Invalid date format (use DD/MM/YYYY)");
        if (!isValidTime(time)) throw ReservationException("Invalid time format (use HH:MM)");
        if (price <= 0) throw ReservationException("Price must be greater than zero");
        if (seats <= 0) throw ReservationException("Seats must be greater than zero");

        Flight flight(nextFlightId++, flightNumber, origin, destination, date, time,
            price, seats, airline);
        flights[flightCount++] = flight;
        return true;
    }

    Flight* getFlightById(int id) {
        for (int i = 0; i < flightCount; i++)
            if (flights[i].getId() == id) return &flights[i];
        return NULL;
    }

    bool bookFlight(int flightId, const char* paymentMethod) {
        if (reservationCount >= MAX_RESERVATIONS || ticketCount >= MAX_TICKETS || paymentCount >= MAX_PAYMENTS)
            return false;

        // Find the flight
        Flight* flight = getFlightById(flightId);
        if (!flight || flight->getAvailableSeats() <= 0) return false;

        int passengerId = currentUser->getId();

        // Book the seat
        if (!flight->bookSeat()) return false;

        // Create reservation
        char currentDate[MAX_STR_LEN];
        getCurrentDate(currentDate);

        Reservation reservation(nextReservationId++, passengerId, flightId, currentDate);
        reservations[reservationCount++] = reservation;

        // Create payment
        Payment payment(nextPaymentId++, reservation.getId(), flight->getPrice(),
            currentDate, paymentMethod);
        payments[paymentCount++] = payment;

        // Create ticket
        char seatNumber[MAX_STR_LEN], ticketNumber[MAX_STR_LEN];
        generateSeatNumber(seatNumber);
        generateTicketNumber(ticketNumber);

        Ticket ticket(nextTicketId++, reservation.getId(), passengerId, flightId,
            seatNumber, ticketNumber, currentDate);
        tickets[ticketCount++] = ticket;

        return true;
    }

    int getPassengerTickets(int passengerId, Ticket* result, int maxResults) {
        int count = 0;
        for (int i = 0; i < ticketCount && count < maxResults; i++)
            if (tickets[i].getPassengerId() == passengerId)
                result[count++] = tickets[i];
        return count;
    }

    bool cancelReservation(int reservationId) {
        // Find the reservation
        int resIndex = -1;
        for (int i = 0; i < reservationCount; i++) {
            if (reservations[i].getId() == reservationId &&
                strcmp(reservations[i].getStatus(), "Confirmed") == 0) {
                resIndex = i;
                break;
            }
        }

        if (resIndex == -1) return false;

        // Find the ticket
        int ticketIndex = -1;
        for (int i = 0; i < ticketCount; i++) {
            if (tickets[i].getReservationId() == reservationId) {
                ticketIndex = i;
                break;
            }
        }

        if (ticketIndex == -1) return false;

        // Find the payment
        int paymentIndex = -1;
        for (int i = 0; i < paymentCount; i++) {
            if (payments[i].getReservationId() == reservationId) {
                paymentIndex = i;
                break;
            }
        }

        // Cancel everything
        tickets[ticketIndex].setStatus("Cancelled");
        reservations[resIndex].setStatus("Cancelled");

        if (paymentIndex != -1) payments[paymentIndex].setStatus("Refunded");

        // Release the seat
        Flight* flight = getFlightById(reservations[resIndex].getFlightId());
        if (flight) flight->cancelSeat();

        return true;
    }

    int getPassengersForFlight(int flightId, Passenger** result, int maxResults) {
        int count = 0;

        for (int i = 0; i < ticketCount && count < maxResults; i++) {
            if (tickets[i].getFlightId() == flightId &&
                strcmp(tickets[i].getStatus(), "Confirmed") == 0) {

                int passengerId = tickets[i].getPassengerId();

                // Find the passenger
                for (int j = 0; j < userCount; j++) {
                    if (!users[j]->getIsAdmin() && users[j]->getId() == passengerId) {
                        Passenger* passenger = static_cast<Passenger*>(users[j]);
                        result[count++] = passenger;
                        break;
                    }
                }

                if (count >= maxResults) break;
            }
        }

        return count;
    }

    void displayTicket(int ticketId) {
        for (int i = 0; i < ticketCount; i++) {
            if (tickets[i].getId() == ticketId) {
                Flight* flight = getFlightById(tickets[i].getFlightId());

                if (!flight) {
                    cout << RED << "Error: Flight not found for this ticket." << RESET << endl;
                    return;
                }

                // Find the passenger
                Passenger* passenger = NULL;
                for (int j = 0; j < userCount; j++) {
                    if (!users[j]->getIsAdmin() && users[j]->getId() == tickets[i].getPassengerId()) {
                        passenger = static_cast<Passenger*>(users[j]);
                        break;
                    }
                }

                if (!passenger) {
                    cout << RED << "Error: Passenger not found for this ticket." << RESET << endl;
                    return;
                }

                tickets[i].printTicket(*flight, *passenger);
                return;
            }
        }

        cout << YELLOW << "Ticket not found." << RESET << endl;
    }
};

// Helper functions implementation
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    cout << "\nPress Enter to continue...";
    cin.get();
}

void displayLoginMenu() {
    clearScreen();
    cout << BLUE
        << "--------------------------------------------\n"
        << "        FLIGHT RESERVATION SYSTEM           \n"
        << "--------------------------------------------\n" << RESET;

    cout << BOLD << " 1. Login\n 2. Register as Passenger\n 3. Exit\n" << RESET;

    cout << BLUE
        << "--------------------------------------------\n" << RESET
        << "Enter your choice: ";
}

void displayAdminMenu() {
    clearScreen();
    cout << CYAN
        << "--------------------------------------------\n"
        << "             ADMIN DASHBOARD                \n"
        << "--------------------------------------------\n" << RESET;

    cout << BOLD
        << " 1. Add Flight\n"
        << " 2. View All Flights\n"
        << " 3. View Passengers by Flight\n"
        << " 4. Save Data\n"
        << " 5. Logout\n" << RESET;

    cout << CYAN
        << "--------------------------------------------\n" << RESET
        << "Enter your choice: ";
}

void displayPassengerMenu() {
    clearScreen();
    cout << MAGENTA
        << "--------------------------------------------\n"
        << "           PASSENGER DASHBOARD              \n"
        << "--------------------------------------------\n" << RESET;

    cout << BOLD
        << " 1. View Available Flights\n"
        << " 2. Book a Flight\n"
        << " 3. View My Tickets\n"
        << " 4. Cancel Reservation\n"
        << " 5. Logout\n" << RESET;

    cout << MAGENTA
        << "--------------------------------------------\n" << RESET
        << "Enter your choice: ";
}

int main() {
    try {
        srand(time(NULL));

        ReservationSystem system;
        bool running = true;

        while (running) {
            if (!system.isLoggedIn()) {
                displayLoginMenu();

                int choice;
                cin >> choice;
                cin.ignore();

                switch (choice) {
                case 1: { // Login
                    clearScreen();
                    cout << BLUE << "-------- LOGIN --------\n" << RESET;

                    char username[MAX_STR_LEN], password[MAX_STR_LEN];
                    cout << "Enter username: "; cin.getline(username, MAX_STR_LEN);
                    cout << "Enter password: "; cin.getline(password, MAX_STR_LEN);

                    if (system.login(username, password)) {
                        cout << GREEN << "Login successful!" << RESET << endl;
                    }
                    else {
                        cout << RED << "Invalid username or password." << RESET << endl;
                        pause();
                    }
                    break;
                }
                case 2: { // Register
                    clearScreen();
                    cout << BLUE << "-------- REGISTER AS PASSENGER --------\n" << RESET;

                    char username[MAX_STR_LEN], password[MAX_STR_LEN];
                    char name[MAX_STR_LEN], email[MAX_STR_LEN], phone[MAX_STR_LEN];
                    char passport[MAX_STR_LEN], address[MAX_STR_LEN];

                    cout << "Enter username: "; cin.getline(username, MAX_STR_LEN);
                    cout << "Enter password: "; cin.getline(password, MAX_STR_LEN);
                    cout << "Enter full name: "; cin.getline(name, MAX_STR_LEN);
                    cout << "Enter email (example@domain.com): "; cin.getline(email, MAX_STR_LEN);
                    cout << "Enter phone (10+ digits): "; cin.getline(phone, MAX_STR_LEN);
                    cout << "Enter passport number (letters and numbers): "; cin.getline(passport, MAX_STR_LEN);
                    cout << "Enter address: "; cin.getline(address, MAX_STR_LEN);

                    try {
                        if (system.registerPassenger(username, password, name, email, phone, passport, address)) {
                            cout << GREEN << "Registration successful! You can now login." << RESET << endl;
                        }
                        else {
                            cout << RED << "Registration failed. Username may already exist." << RESET << endl;
                        }
                    }
                    catch (const ReservationException& e) {
                        cout << RED << "Registration error: " << e.what() << RESET << endl;
                    }

                    pause();
                    break;
                }
                case 3: // Exit
                    running = false;
                    break;
                default:
                    cout << RED << "Invalid choice. Please try again." << RESET << endl;
                    pause();
                    break;
                }
            }
            else if (system.isAdmin()) {
                displayAdminMenu();

                int choice;
                cin >> choice;
                cin.ignore();

                switch (choice) {
                case 1: { // Add Flight
                    clearScreen();
                    cout << CYAN << "-------- ADD FLIGHT --------\n" << RESET;

                    char flightNumber[MAX_STR_LEN], origin[MAX_STR_LEN], destination[MAX_STR_LEN];
                    char date[MAX_STR_LEN], time[MAX_STR_LEN], airline[MAX_STR_LEN];
                    double price;
                    int seats;

                    cout << "Enter flight number: "; cin.getline(flightNumber, MAX_STR_LEN);
                    cout << "Enter airline: "; cin.getline(airline, MAX_STR_LEN);
                    cout << "Enter origin: "; cin.getline(origin, MAX_STR_LEN);
                    cout << "Enter destination: "; cin.getline(destination, MAX_STR_LEN);
                    cout << "Enter date (DD/MM/YYYY): "; cin.getline(date, MAX_STR_LEN);
                    cout << "Enter time (HH:MM): "; cin.getline(time, MAX_STR_LEN);
                    cout << "Enter price: $"; cin >> price;
                    cout << "Enter total seats: "; cin >> seats;
                    cin.ignore();

                    try {
                        if (system.addFlight(flightNumber, origin, destination, date, time, airline, price, seats)) {
                            cout << GREEN << "Flight added successfully!" << RESET << endl;
                        }
                        else {
                            cout << RED << "Failed to add flight. System may be full." << RESET << endl;
                        }
                    }
                    catch (const ReservationException& e) {
                        cout << RED << "Error: " << e.what() << RESET << endl;
                    }

                    pause();
                    break;
                }
                case 2: { // View All Flights
                    clearScreen();
                    cout << CYAN << "-------- ALL FLIGHTS --------\n" << RESET;
                    system.displayAllFlights();
                    pause();
                    break;
                }
                case 3: { // View Passengers by Flight
                    clearScreen();
                    cout << CYAN << "-------- VIEW PASSENGERS BY FLIGHT --------\n" << RESET;

                    system.displayAllFlights();

                    int flightId;
                    cout << "\nEnter flight ID to view passengers: ";
                    cin >> flightId;
                    cin.ignore();

                    try {
                        Flight* flight = system.getFlightById(flightId);
                        if (!flight) {
                            cout << RED << "Flight not found." << RESET << endl;
                            pause();
                            break;
                        }

                        Passenger* passengers[MAX_USERS];
                        int count = system.getPassengersForFlight(flightId, passengers, MAX_USERS);

                        if (count == 0) {
                            cout << YELLOW << "No passengers booked on this flight yet." << RESET << endl;
                        }
                        else {
                            cout << GREEN << "Passengers for Flight #" << flight->getFlightNumber()
                                << " (" << flight->getOrigin() << " to " << flight->getDestination()
                                << " on " << flight->getDepartureDate() << "):\n" << RESET;

                            cout << BOLD << left
                                << setw(5) << "ID" << setw(20) << "Name"
                                << setw(30) << "Passport" << "Email" << endl
                                << "-----------------------------------------------------------------\n" << RESET;

                            for (int i = 0; i < count; i++) {
                                cout << left << setw(5) << passengers[i]->getId()
                                    << setw(20) << passengers[i]->getName()
                                    << setw(30) << passengers[i]->getPassportNumber()
                                    << passengers[i]->getUsername() << endl;
                            }
                        }
                    }
                    catch (const ReservationException& e) {
                        cout << RED << "Error: " << e.what() << RESET << endl;
                    }

                    pause();
                    break;
                }
                case 4: { // Save Data
                    clearScreen();
                    cout << CYAN << "-------- SAVE DATA --------\n" << RESET;

                    try {
                        system.saveDataToFiles();
                        cout << GREEN << "Data saved successfully!" << RESET << endl;
                    }
                    catch (const ReservationException& e) {
                        cout << RED << "Error saving data: " << e.what() << RESET << endl;
                    }

                    pause();
                    break;
                }
                case 5: { // Logout
                    system.logout();
                    cout << GREEN << "Logged out successfully!" << RESET << endl;
                    pause(); // Added pause here to fix the issue
                    break;
                }
                default:
                    cout << RED << "Invalid choice. Please try again." << RESET << endl;
                    pause();
                    break;
                }
            }
            else { // Passenger menu
                displayPassengerMenu();

                int choice;
                cin >> choice;
                cin.ignore();

                switch (choice) {
                case 1: { // View Flights
                    clearScreen();
                    cout << MAGENTA << "-------- AVAILABLE FLIGHTS --------\n" << RESET;
                    system.displayAllFlights();
                    pause();
                    break;
                }
                case 2: { // Book Flight
                    clearScreen();
                    cout << MAGENTA << "-------- BOOK FLIGHT --------\n" << RESET;

                    system.displayAllFlights();

                    int flightId;
                    cout << "\nEnter flight ID to book: ";
                    cin >> flightId;
                    cin.ignore();

                    Flight* flight = system.getFlightById(flightId);
                    if (!flight) {
                        cout << RED << "Flight not found." << RESET << endl;
                        pause();
                        break;
                    }

                    if (flight->getAvailableSeats() <= 0) {
                        cout << RED << "Sorry, this flight is fully booked." << RESET << endl;
                        pause();
                        break;
                    }

                    char paymentMethod[MAX_STR_LEN];
                    cout << "Enter payment method (Credit Card, PayPal, etc.): ";
                    cin.getline(paymentMethod, MAX_STR_LEN);

                    try {
                        if (system.bookFlight(flightId, paymentMethod)) {
                            cout << GREEN << "Flight booked successfully!" << RESET << endl;
                        }
                        else {
                            cout << RED << "Failed to book flight." << RESET << endl;
                        }
                    }
                    catch (const ReservationException& e) {
                        cout << RED << "Error: " << e.what() << RESET << endl;
                    }

                    pause();
                    break;
                }
                case 3: { // View Tickets
                    clearScreen();
                    cout << MAGENTA << "-------- MY TICKETS --------\n" << RESET;

                    Ticket tickets[MAX_TICKETS];
                    int count = system.getPassengerTickets(system.getCurrentUser()->getId(), tickets, MAX_TICKETS);

                    if (count == 0) {
                        cout << YELLOW << "You have no tickets." << RESET << endl;
                        pause();
                        break;
                    }

                    cout << BOLD << left << setw(5) << "ID" << setw(15) << "Ticket Number"
                        << setw(10) << "Flight" << setw(15) << "Status" << endl
                        << "---------------------------------------------\n" << RESET;

                    for (int i = 0; i < count; i++) {
                        Flight* flight = system.getFlightById(tickets[i].getFlightId());
                        cout << left << setw(5) << tickets[i].getId()
                            << setw(15) << tickets[i].getTicketNumber()
                            << setw(10) << (flight ? flight->getFlightNumber() : "Unknown")
                            << setw(15) << tickets[i].getStatus() << endl;
                    }

                    cout << "\nEnter ticket ID to view details (or 0 to return): ";
                    int ticketId;
                    cin >> ticketId;
                    cin.ignore();

                    if (ticketId > 0) system.displayTicket(ticketId);

                    pause();
                    break;
                }
                case 4: { // Cancel Reservation
                    clearScreen();
                    cout << MAGENTA << "-------- CANCEL RESERVATION --------\n" << RESET;

                    Ticket tickets[MAX_TICKETS];
                    int count = system.getPassengerTickets(system.getCurrentUser()->getId(), tickets, MAX_TICKETS);

                    if (count == 0) {
                        cout << YELLOW << "You have no tickets to cancel." << RESET << endl;
                        pause();
                        break;
                    }

                    cout << BOLD << left << setw(15) << "Reservation ID" << setw(15) << "Ticket Number"
                        << setw(15) << "Status" << endl
                        << "---------------------------------------\n" << RESET;

                    for (int i = 0; i < count; i++) {
                        if (strcmp(tickets[i].getStatus(), "Confirmed") == 0) {
                            cout << left << setw(15) << tickets[i].getReservationId()
                                << setw(15) << tickets[i].getTicketNumber()
                                << tickets[i].getStatus() << endl;
                        }
                    }

                    cout << "\nEnter Reservation ID to cancel (or 0 to return): ";
                    int reservationId;
                    cin >> reservationId;
                    cin.ignore();

                    if (reservationId > 0) {
                        try {
                            if (system.cancelReservation(reservationId)) {
                                cout << GREEN << "Reservation cancelled successfully!" << RESET << endl;
                            }
                            else {
                                cout << RED << "Failed to cancel reservation." << RESET << endl;
                            }
                        }
                        catch (const ReservationException& e) {
                            cout << RED << "Error: " << e.what() << RESET << endl;
                        }
                    }

                    pause();
                    break;
                }
                case 5: { // Logout
                    system.logout();
                    cout << GREEN << "Logged out successfully!" << RESET << endl;
                    pause(); // Added pause here to fix the issue
                    break;
                }
                default:
                    cout << RED << "Invalid choice. Please try again." << RESET << endl;
                    pause();
                    break;
                }
            }
        }

        // Exit message
        clearScreen();
        cout << CYAN << "--------------------------------------------\n"
            << "    Thank you for using our Flight System    \n"
            << "--------------------------------------------\n" << RESET;

        return 0;
    }
    catch (const ReservationException& e) {
        cout << RED << "ERROR: " << e.what() << RESET << endl;
        return 1;
    }
    catch (const exception& e) {
        cout << RED << "SYSTEM ERROR: " << e.what() << RESET << endl;
        return 1;
    }
    catch (...) {
        cout << RED << "UNKNOWN ERROR OCCURRED" << RESET << endl;
        return 1;
    }
}
