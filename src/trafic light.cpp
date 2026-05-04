// ============================================================
//  Traffic Control System
//  OOP Version - All logic inside classes
//  Compile: g++ -o traffic traffic_control.cpp
// ============================================================

#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <cstdlib>
#include <limits>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
  #include <windows.h>
  #include <conio.h>
  #define SLEEP_MS(x) Sleep(x)
#else
  #include <unistd.h>
  #include <termios.h>
  #define SLEEP_MS(x) usleep((x) * 1000)
#endif

using namespace std;

// ============================================================
//  CLASS: Terminal
//  Handles all display, color, and input operations.
//  All constants are private static members (no globals).
// ============================================================
class Terminal {
private:
    static const string RESET;
    static const string RED;
    static const string GREEN;
    static const string YELLOW;
    static const string BLUE;
    static const string MAGENTA;
    static const string CYAN;
    static const string WHITE;
    static const string BOLD;

    static const string ADMIN_USER;
    static const string ADMIN_PASS;
    static const string DATA_FILE;
    static const int    MAX_RECORDS;
    static const int    SPEED_LIMIT;

public:
    static string red    (const string& s) { return RED     + s + RESET; }
    static string green  (const string& s) { return GREEN   + s + RESET; }
    static string yellow (const string& s) { return YELLOW  + s + RESET; }
    static string cyan   (const string& s) { return CYAN    + s + RESET; }
    static string magenta(const string& s) { return MAGENTA + s + RESET; }
    static string blue   (const string& s) { return BLUE    + s + RESET; }
    static string bold   (const string& s) { return BOLD    + s + RESET; }

    static void clearScreen() { cout << "\033[2J\033[H" << flush; }

    static void printHeader(const string& title) {
        cout << BLUE   << "\n\t\t=========================================\n"
             << CYAN   << BOLD << "\t\t  " << title << "\n"
             << BLUE   << "\t\t=========================================\n"
             << RESET;
    }

    static void separator() {
        cout << MAGENTA << "\n\t\t--------------------------------------------\n" << RESET;
    }

    static void animateText(const string& text, int delayMs = 10) {
        for (char c : text) { cout << c << flush; SLEEP_MS(delayMs); }
    }

    static void drawCar(const string& colorCode) {
        cout << colorCode;
        animateText("\n\n\t\t\t\t    ______\n");
        animateText("\t\t\t\t   /|_||_\\`.__\n");
        animateText("\t\t\t\t  (   _    _ _\\\n");
        animateText("\t\t\t\t  =`-(_)--(_)-'\n\n");
        cout << RESET;
    }

    static void pressEnter() {
        cout << RED << "\n\n\t\t   Press ENTER to continue..." << RESET;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    static void printSuccess(const string& msg) { cout << GREEN   << "\n\t\t   [OK]    " << msg << RESET << "\n"; }
    static void printError  (const string& msg) { cout << RED     << "\n\t\t   [ERROR] " << msg << RESET << "\n"; }
    static void printInfo   (const string& msg) { cout << CYAN    << "\n\t\t   [INFO]  " << msg << RESET << "\n"; }

    static int getKey() {
#ifdef _WIN32
        return _getch();
#else
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        int ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
#endif
    }

    static string readLine(const string& prompt) {
        string input;
        cout << CYAN << "\t\t  " << prompt << ": " << RESET;
        cin >> input;
        return input;
    }

    static int readInt(const string& prompt) {
        int val = 0;
        cout << CYAN << "\t\t  " << prompt << ": " << RESET;
        cin >> val;
        return val;
    }

    static string getAdminUser()  { return ADMIN_USER;  }
    static string getAdminPass()  { return ADMIN_PASS;  }
    static string getDataFile()   { return DATA_FILE;   }
    static int    getMaxRecords() { return MAX_RECORDS; }
    static int    getSpeedLimit() { return SPEED_LIMIT; }

    static string getColorCode(int idx) {
        const string palette[] = { WHITE, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE };
        return palette[idx % 8];
    }
};

const string Terminal::RESET      = "\033[0m";
const string Terminal::RED        = "\033[31m";
const string Terminal::GREEN      = "\033[32m";
const string Terminal::YELLOW     = "\033[33m";
const string Terminal::BLUE       = "\033[34m";
const string Terminal::MAGENTA    = "\033[35m";
const string Terminal::CYAN       = "\033[36m";
const string Terminal::WHITE      = "\033[37m";
const string Terminal::BOLD       = "\033[1m";
const string Terminal::ADMIN_USER = "admin";
const string Terminal::ADMIN_PASS = "1234";
const string Terminal::DATA_FILE  = "trafficdata.txt";
const int    Terminal::MAX_RECORDS = 100;
const int    Terminal::SPEED_LIMIT = 30;

// ============================================================
//  CLASS: Vehicle
//  One vehicle: stores data and contains all detection,
//  calculation, display, and serialization logic.
// ============================================================
class Vehicle {
private:
    string regNo;
    string color;
    string timestamp;
    int    taxAmount;
    string violationStatus;
    int    speed;
    bool   identified;
    bool   hasViolation;

    static const string CHAR_POOL;
    static const string DIGIT_POOL;

    void buildViolationStatus() {
        violationStatus = "";
        hasViolation    = false;
        if (!identified) {
            violationStatus +=
                "\n\t\t\t  *** Unidentified Vehicle ***\n"
                "\t\t    ----------------------------------------\n";
            hasViolation = true;
        }
        if (speed > Terminal::getSpeedLimit()) {
            violationStatus +=
                "\t\t    Speed limit exceeded! "
                "(Detected: " + to_string(speed) + " km/h, "
                "Limit: " + to_string(Terminal::getSpeedLimit()) + " km/h)\n";
            hasViolation = true;
        }
    }

public:
    Vehicle()
        : regNo("0000-0000"), color("Unknown"), timestamp(""),
          taxAmount(0), violationStatus(""), speed(0),
          identified(true), hasViolation(false) {}

    // Generate random license plate (XXXX-9999)
    void generateRegNo() {
        regNo = "0000-0000";
        for (int k = 0; k < 4; k++)
            regNo[k] = CHAR_POOL[(rand() % (int)(CHAR_POOL.size() - 1)) + 1];
        for (int k = 5; k < 9; k++)
            regNo[k] = DIGIT_POOL[(rand() % (int)(DIGIT_POOL.size() - 1)) + 1];
    }

    // Assign color name from numeric index
    void assignColor(int code) {
        const string colors[] = { "White","Blue","Green","Aqua","Red","Purple","Yellow","White" };
        color = (code >= 0 && code < 8) ? colors[code] : "White";
    }

    // Record current system time
    void recordTimestamp() {
        time_t now = time(nullptr);
        char   buf[64];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        timestamp = buf;
    }

    // Simulate speed sensor and plate recognition
    void detectViolation() {
        speed      = rand() % 120;
        identified = (rand() % 2 == 1);
        buildViolationStatus();
    }

    // Calculate fine from violation state
    void calculateTax() {
        bool speeding     = (speed > Terminal::getSpeedLimit());
        bool unidentified = !identified;
        if      (speeding && unidentified) taxAmount = 1100;
        else if (unidentified)             taxAmount = 1000;
        else if (speeding)                 taxAmount =  100;
        else                               taxAmount =    0;
    }

    // Print full vehicle details
    void displayInfo() const {
        Terminal::separator();
        if (hasViolation)
            cout << Terminal::red(violationStatus);
        else
            cout << Terminal::green("\n\t\t    Status  : OK - No violation detected");
        cout << Terminal::green("\n\n\t\t    Reg No  : ") << Terminal::yellow(regNo)
             << Terminal::green("\n\n\t\t    Time    : ") << Terminal::yellow(timestamp)
             << Terminal::green("\n\n\t\t    Speed   : ") << Terminal::yellow(to_string(speed) + " km/h")
             << Terminal::green("\n\n\t\t    Fine    : ") << Terminal::yellow("Rp " + to_string(taxAmount))
             << Terminal::green("\n\n\t\t    Color   : ") << Terminal::yellow(color)
             << "\n";
    }

    // Print compact one-line summary
    void displaySummary(int index) const {
        string status = hasViolation ? Terminal::red("[VIOLATION]") : Terminal::green("[OK]      ");
        cout << Terminal::cyan("\t\t  [" + to_string(index + 1) + "] ")
             << Terminal::yellow(regNo) << "  " << status
             << "  " << Terminal::yellow(to_string(speed) + " km/h")
             << "  Rp " << Terminal::yellow(to_string(taxAmount))
             << "  " << Terminal::magenta(timestamp) << "\n";
    }

    // Interactive keyboard input for all fields
    void inputManually() {
        regNo     = Terminal::readLine("Reg No (e.g. ABCD-1234)");
        timestamp = Terminal::readLine("Timestamp (e.g. 2025-01-01 12:00:00)");
        speed     = Terminal::readInt ("Speed (km/h)");
        color     = Terminal::readLine("Color");
        identified = true;
        buildViolationStatus();
        calculateTax();
    }

    // Reset all fields to defaults
    void reset() {
        regNo = "0000-0000"; color = "Unknown"; timestamp = "";
        taxAmount = 0; violationStatus = ""; speed = 0;
        identified = true; hasViolation = false;
    }

    // Serialize to pipe-delimited string for file storage
    string serialize() const {
        ostringstream oss;
        oss << regNo << "|" << color << "|" << timestamp << "|"
            << speed << "|" << taxAmount << "|" << identified << "|"
            << (hasViolation ? 1 : 0);
        return oss.str();
    }

    // Deserialize from pipe-delimited string
    bool deserialize(const string& line) {
        if (line.empty()) return false;
        istringstream iss(line);
        string fields[7]; int i = 0;
        string tok;
        while (getline(iss, tok, '|') && i < 7) fields[i++] = tok;
        if (i < 7) return false;
        regNo      = fields[0];
        color      = fields[1];
        timestamp  = fields[2];
        speed      = stoi(fields[3]);
        taxAmount  = stoi(fields[4]);
        identified = (fields[5] == "1");
        buildViolationStatus();
        return true;
    }

    // Getters
    string getRegNo()        const { return regNo;           }
    string getColor()        const { return color;           }
    string getTimestamp()    const { return timestamp;       }
    int    getSpeed()        const { return speed;           }
    int    getTaxAmount()    const { return taxAmount;       }
    string getViolation()    const { return violationStatus; }
    bool   isIdentified()    const { return identified;      }
    bool   getHasViolation() const { return hasViolation;    }

    // Setters (recalculate derived state when relevant fields change)
    void setRegNo    (const string& v) { regNo      = v; }
    void setColor    (const string& v) { color      = v; }
    void setTimestamp(const string& v) { timestamp  = v; }
    void setTaxAmount(int v)           { taxAmount  = v; }
    void setSpeed    (int v)           { speed      = v; buildViolationStatus(); calculateTax(); }
    void setIdentified(bool v)         { identified = v; buildViolationStatus(); calculateTax(); }
};

const string Vehicle::CHAR_POOL  = "QAZWSXEDCRFVTGBYHNUJMIKHEGCUWUBOLP";
const string Vehicle::DIGIT_POOL = "5647832910";

// ============================================================
//  CLASS: Database
//  Fixed-size array storage with file persistence,
//  CRUD operations, filtering, and statistics.
// ============================================================
class Database {
private:
    Vehicle records[100];
    int     total;

    void writeRecord(ofstream& file, int i) const {
        file << records[i].serialize() << "\n";
    }

public:
    Database() : total(0) {}

    // Add a vehicle; returns false if storage is full
    bool addVehicle(const Vehicle& v) {
        if (total >= Terminal::getMaxRecords()) {
            Terminal::printError("Storage full. Limit: " + to_string(Terminal::getMaxRecords()));
            return false;
        }
        records[total++] = v;
        return true;
    }

    // Linear search by plate number; returns index or -1
    int findByRegNo(const string& reg) const {
        for (int i = 0; i < total; i++)
            if (records[i].getRegNo() == reg) return i;
        return -1;
    }

    // Remove a record by plate; returns true on success
    bool removeByRegNo(const string& reg) {
        int idx = findByRegNo(reg);
        if (idx == -1) return false;
        for (int j = idx; j < total - 1; j++) records[j] = records[j + 1];
        total--;
        return true;
    }

    // Replace a record in-place
    bool updateByRegNo(const string& reg, const Vehicle& updated) {
        int idx = findByRegNo(reg);
        if (idx == -1) return false;
        records[idx] = updated;
        return true;
    }

    // Erase all records from memory
    void clearAll() { total = 0; }

    // Print every record in full detail
    void displayAll() const {
        if (total == 0) { Terminal::printInfo("No records found."); return; }
        for (int i = 0; i < total; i++) records[i].displayInfo();
    }

    // Print all records as compact one-line summaries
    void displaySummaryList() const {
        if (total == 0) { Terminal::printInfo("No records found."); return; }
        Terminal::separator();
        cout << Terminal::bold("\t\t  #   Reg No      Status        Speed     Fine        Time\n");
        Terminal::separator();
        for (int i = 0; i < total; i++) records[i].displaySummary(i);
    }

    // Print only records that have at least one violation
    void displayViolationsOnly() const {
        bool found = false;
        Terminal::separator();
        cout << Terminal::bold("\t\t  Vehicles with violations:\n");
        Terminal::separator();
        for (int i = 0; i < total; i++) {
            if (records[i].getHasViolation()) { records[i].displaySummary(i); found = true; }
        }
        if (!found) Terminal::printInfo("No violations on record.");
    }

    // Print only records with no violations
    void displayCleanOnly() const {
        bool found = false;
        Terminal::separator();
        cout << Terminal::bold("\t\t  Clean vehicles (no violations):\n");
        Terminal::separator();
        for (int i = 0; i < total; i++) {
            if (!records[i].getHasViolation()) { records[i].displaySummary(i); found = true; }
        }
        if (!found) Terminal::printInfo("No clean vehicles found.");
    }

    // Print a full statistics dashboard
    void displayStatistics() const {
        int totalFines = 0, violations = 0, unidentified = 0, speedingCount = 0;
        int maxSpeed   = 0, totalSpeed = 0;
        int minSpeed   = (total > 0) ? records[0].getSpeed() : 0;

        for (int i = 0; i < total; i++) {
            totalFines += records[i].getTaxAmount();
            totalSpeed += records[i].getSpeed();
            if (records[i].getSpeed() > maxSpeed) maxSpeed = records[i].getSpeed();
            if (records[i].getSpeed() < minSpeed) minSpeed = records[i].getSpeed();
            if (records[i].getHasViolation())               violations++;
            if (!records[i].isIdentified())                 unidentified++;
            if (records[i].getSpeed() > Terminal::getSpeedLimit()) speedingCount++;
        }
        int avgSpeed = (total > 0) ? totalSpeed / total : 0;

        Terminal::separator();
        cout << Terminal::bold("\t\t  TRAFFIC STATISTICS DASHBOARD\n");
        Terminal::separator();
        cout << Terminal::cyan("\t\t  Total vehicles scanned  : ") << Terminal::yellow(to_string(total))               << "\n"
             << Terminal::cyan("\t\t  Total violations        : ") << Terminal::red(to_string(violations))             << "\n"
             << Terminal::cyan("\t\t  Unidentified vehicles   : ") << Terminal::red(to_string(unidentified))           << "\n"
             << Terminal::cyan("\t\t  Speeding violations     : ") << Terminal::red(to_string(speedingCount))          << "\n"
             << Terminal::cyan("\t\t  Clean vehicles          : ") << Terminal::green(to_string(total - violations))   << "\n";
        Terminal::separator();
        cout << Terminal::cyan("\t\t  Max speed detected      : ") << Terminal::yellow(to_string(maxSpeed) + " km/h") << "\n"
             << Terminal::cyan("\t\t  Min speed detected      : ") << Terminal::yellow(to_string(minSpeed) + " km/h") << "\n"
             << Terminal::cyan("\t\t  Average speed           : ") << Terminal::yellow(to_string(avgSpeed) + " km/h") << "\n";
        Terminal::separator();
        cout << Terminal::cyan("\t\t  Total fines collected   : ") << Terminal::yellow("Rp " + to_string(totalFines)) << "\n";
        Terminal::separator();
    }

    // Count records with violations
    int countViolations() const {
        int n = 0;
        for (int i = 0; i < total; i++) if (records[i].getHasViolation()) n++;
        return n;
    }

    // Sum all fines
    int totalFinesCollected() const {
        int sum = 0;
        for (int i = 0; i < total; i++) sum += records[i].getTaxAmount();
        return sum;
    }

    // Persist all records to DATA_FILE
    bool saveToFile() const {
        ofstream file(Terminal::getDataFile().c_str(), ios::out);
        if (!file.is_open()) { Terminal::printError("Cannot write to: " + Terminal::getDataFile()); return false; }
        for (int i = 0; i < total; i++) writeRecord(file, i);
        file.close();
        return true;
    }

    // Read records from DATA_FILE; returns count loaded
    int loadFromFile() {
        ifstream file(Terminal::getDataFile().c_str(), ios::in);
        if (!file.is_open()) return 0;
        total = 0;
        string line;
        while (getline(file, line) && total < Terminal::getMaxRecords()) {
            Vehicle v;
            if (v.deserialize(line)) records[total++] = v;
        }
        file.close();
        return total;
    }

    // Export a plain-text human-readable report
    bool exportReport(const string& filename) const {
        ofstream file(filename.c_str(), ios::out);
        if (!file.is_open()) { Terminal::printError("Cannot create: " + filename); return false; }
        time_t now = time(nullptr);
        file << "==============================================\n"
             << "  TRAFFIC CONTROL SYSTEM - REPORT\n"
             << "  Generated : " << ctime(&now)
             << "==============================================\n\n"
             << "Total Records    : " << total               << "\n"
             << "Total Violations : " << countViolations()   << "\n"
             << "Total Fines      : Rp " << totalFinesCollected() << "\n\n"
             << "----------------------------------------------\n";
        for (int i = 0; i < total; i++) {
            file << "[" << (i + 1) << "] "
                 << records[i].getRegNo()     << " | "
                 << records[i].getTimestamp() << " | "
                 << records[i].getSpeed()     << " km/h | "
                 << "Rp " << records[i].getTaxAmount() << " | "
                 << (records[i].getHasViolation() ? "VIOLATION" : "OK") << "\n";
        }
        file << "----------------------------------------------\n";
        file.close();
        return true;
    }

    // Accessors
    int            getTotal()       const { return total;      }
    Vehicle&       getRecord(int i)       { return records[i]; }
    const Vehicle& getRecord(int i) const { return records[i]; }
    bool           isEmpty()        const { return total == 0; }
};

// ============================================================
//  CLASS: Auth
//  Admin session management with lockout after 3 failures.
// ============================================================
class Auth {
private:
    bool   authenticated;
    string currentUser;
    int    failedAttempts;
    static const int MAX_ATTEMPTS = 3;

public:
    Auth() : authenticated(false), currentUser(""), failedAttempts(0) {}

    // Prompt for credentials; returns true on success
    bool login() {
        if (failedAttempts >= MAX_ATTEMPTS) {
            Terminal::printError("Account locked after " + to_string(MAX_ATTEMPTS) + " failed attempts.");
            return false;
        }
        string username = Terminal::readLine("Username");
        string password = Terminal::readLine("Password");
        if (username == Terminal::getAdminUser() && password == Terminal::getAdminPass()) {
            authenticated  = true;
            currentUser    = username;
            failedAttempts = 0;
            Terminal::printSuccess("Logged in as: " + currentUser);
            return true;
        }
        failedAttempts++;
        Terminal::printError("Wrong credentials. Attempts left: " +
            to_string(MAX_ATTEMPTS - failedAttempts));
        return false;
    }

    // End current session
    void logout() { authenticated = false; currentUser = ""; }

    // Print admin operation menu
    void showAccessMenu() const {
        Terminal::separator();
        cout << Terminal::bold("\t\t  Admin Panel  [" + currentUser + "]\n\n");
        cout << Terminal::red("\t\t  1") << ") Show All Records (detailed)\n";
        cout << Terminal::red("\t\t  2") << ") Show Summary List\n";
        cout << Terminal::red("\t\t  3") << ") Show Violations Only\n";
        cout << Terminal::red("\t\t  4") << ") Show Clean Vehicles\n";
        cout << Terminal::red("\t\t  5") << ") Search by Plate Number\n";
        cout << Terminal::red("\t\t  6") << ") Update a Record\n";
        cout << Terminal::red("\t\t  7") << ") Delete a Record\n";
        cout << Terminal::red("\t\t  8") << ") Clear ALL Records\n";
        cout << Terminal::red("\t\t  9") << ") Statistics Dashboard\n";
        cout << Terminal::red("\t\t  R") << ") Export Report to File\n";
        cout << Terminal::red("\t\t  Q") << ") Logout & Exit Admin Panel\n";
        Terminal::separator();
        cout << Terminal::cyan("\t\t  Choice: ");
    }

    bool   isAuthenticated() const { return authenticated;  }
    string getCurrentUser()  const { return currentUser;    }
};

// ============================================================
//  CLASS: AdminPanel
//  Full CRUD + reports behind authentication.
// ============================================================
class AdminPanel {
private:
    Database& db;
    Auth      auth;

    void opShowAll() {
        Terminal::clearScreen(); Terminal::printHeader("All Records - Detailed");
        db.displayAll(); Terminal::pressEnter();
    }
    void opShowSummary() {
        Terminal::clearScreen(); Terminal::printHeader("All Records - Summary");
        db.displaySummaryList(); Terminal::pressEnter();
    }
    void opShowViolations() {
        Terminal::clearScreen(); Terminal::printHeader("Violation Records");
        db.displayViolationsOnly(); Terminal::pressEnter();
    }
    void opShowClean() {
        Terminal::clearScreen(); Terminal::printHeader("Clean Vehicle Records");
        db.displayCleanOnly(); Terminal::pressEnter();
    }
    void opSearch() {
        Terminal::clearScreen(); Terminal::printHeader("Search by Plate");
        string reg = Terminal::readLine("Plate number");
        int idx = db.findByRegNo(reg);
        if (idx == -1) Terminal::printError("Not found: " + reg);
        else { Terminal::printSuccess("Found:"); db.getRecord(idx).displayInfo(); }
        Terminal::pressEnter();
    }
    void opUpdate() {
        Terminal::clearScreen(); Terminal::printHeader("Update Record");
        string reg = Terminal::readLine("Plate number to update");
        int idx = db.findByRegNo(reg);
        if (idx == -1) { Terminal::printError("Not found: " + reg); Terminal::pressEnter(); return; }
        cout << Terminal::cyan("\n\t\t  Current data:"); db.getRecord(idx).displayInfo();
        cout << Terminal::cyan("\n\t\t  Enter new data:\n");
        Vehicle updated = db.getRecord(idx);
        updated.inputManually();
        db.updateByRegNo(reg, updated);
        db.saveToFile();
        Terminal::printSuccess("Record updated and saved."); Terminal::pressEnter();
    }
    void opDelete() {
        Terminal::clearScreen(); Terminal::printHeader("Delete a Record");
        string reg = Terminal::readLine("Plate number to delete");
        int idx = db.findByRegNo(reg);
        if (idx == -1) { Terminal::printError("Not found: " + reg); Terminal::pressEnter(); return; }
        cout << Terminal::yellow("\n\t\t  Record to delete:"); db.getRecord(idx).displaySummary(idx);
        cout << Terminal::red("\n\t\t  Confirm delete? (y/n): ");
        int c = Terminal::getKey(); cout << "\n";
        if (c == 'y' || c == 'Y') {
            db.removeByRegNo(reg); db.saveToFile();
            Terminal::printSuccess("Deleted: " + reg);
        } else { Terminal::printInfo("Cancelled."); }
        Terminal::pressEnter();
    }
    void opClearAll() {
        Terminal::clearScreen(); Terminal::printHeader("Clear ALL Records");
        cout << Terminal::red("\n\t\t  WARNING: This will erase all "
            + to_string(db.getTotal()) + " record(s)!\n");
        cout << Terminal::red("\t\t  Type CONFIRM to proceed: ");
        string input; cin >> input;
        if (input == "CONFIRM") {
            db.clearAll(); db.saveToFile();
            Terminal::printSuccess("All records cleared.");
        } else { Terminal::printInfo("Cancelled."); }
        Terminal::pressEnter();
    }
    void opStatistics() {
        Terminal::clearScreen(); Terminal::printHeader("Statistics Dashboard");
        db.displayStatistics(); Terminal::pressEnter();
    }
    void opExportReport() {
        Terminal::clearScreen(); Terminal::printHeader("Export Report");
        string filename = Terminal::readLine("Output filename (e.g. report.txt)");
        if (db.exportReport(filename))
            Terminal::printSuccess("Report saved to: " + filename);
        Terminal::pressEnter();
    }

    void runMenu() {
        bool running = true;
        while (running) {
            Terminal::clearScreen(); Terminal::printHeader("Admin Panel");
            auth.showAccessMenu();
            int choice = Terminal::getKey(); cout << "\n";
            switch (choice) {
                case '1': opShowAll();       break;
                case '2': opShowSummary();   break;
                case '3': opShowViolations();break;
                case '4': opShowClean();     break;
                case '5': opSearch();        break;
                case '6': opUpdate();        break;
                case '7': opDelete();        break;
                case '8': opClearAll();      break;
                case '9': opStatistics();    break;
                case 'r': case 'R': opExportReport(); break;
                case 'q': case 'Q': running = false; break;
                default: Terminal::printError("Invalid choice."); SLEEP_MS(600); break;
            }
        }
        auth.logout();
        Terminal::printInfo("Logged out.");
        SLEEP_MS(500);
    }

public:
    explicit AdminPanel(Database& database) : db(database) {}

    void open() {
        Terminal::clearScreen(); Terminal::printHeader("Admin Login");
        if (auth.login()) runMenu();
        else Terminal::pressEnter();
    }
};

// ============================================================
//  CLASS: Scanner
//  Automated vehicle scanning workflow.
// ============================================================
class Scanner {
private:
    Database&  db;
    AdminPanel panel;

    void showScanAnimation() const {
        cout << Terminal::cyan("\n\t\t    Incoming vehicle");
        Terminal::animateText("...", 300);
        cout << Terminal::cyan("\n\t\t    Scanning plate");
        Terminal::animateText("...", 300);
        cout << "\n";
    }

    void showStatusBar() const {
        Terminal::separator();
        cout << Terminal::cyan("\t\t  Scanned: ") << Terminal::yellow(to_string(db.getTotal()))
             << "   " << Terminal::cyan("Violations: ") << Terminal::red(to_string(db.countViolations()))
             << "   " << Terminal::cyan("Fines: ") << Terminal::yellow("Rp " + to_string(db.totalFinesCollected()))
             << "\n";
    }

    void showActionMenu() const {
        Terminal::separator();
        cout << Terminal::yellow("\t\t  After-scan actions:\n");
        cout << Terminal::red("\t\t  SPACE") << " - Scan next vehicle\n";
        cout << Terminal::red("\t\t  A    ") << " - Open Admin Panel\n";
        cout << Terminal::red("\t\t  M    ") << " - Back to Main Menu\n";
        Terminal::separator();
        cout << Terminal::cyan("\t\t  Choice: ");
    }

    Vehicle performScan() {
        int    colorIdx = rand() % 7;
        string carColor = Terminal::getColorCode(colorIdx);
        Vehicle v;
        v.generateRegNo();
        v.detectViolation();
        v.recordTimestamp();
        v.calculateTax();
        v.assignColor(colorIdx);
        cout << Terminal::green("\n\n\t\t    Plate : ");
        Terminal::animateText(v.getRegNo(), 50);
        Terminal::drawCar(carColor);
        v.displayInfo();
        db.addVehicle(v);
        db.saveToFile();
        return v;
    }

    void runScanCycle() {
        Terminal::clearScreen();
        showScanAnimation();
        performScan();
        showStatusBar();
        showActionMenu();
        int key = Terminal::getKey(); cout << "\n";
        if      (key == ' ')               runScanCycle();
        else if (key == 'a' || key == 'A') panel.open();
        // M or any other key: returns to launchScanMode loop
    }

public:
    explicit Scanner(Database& database) : db(database), panel(database) {}

    // Launch the full scanning mode with its own loop
    void launchScanMode() {
        Terminal::clearScreen(); Terminal::printHeader("AI Traffic Scanner");
        cout << Terminal::cyan(
            "\n\t\t  SPACE - Scan a vehicle\n"
            "\t\t  A     - Admin Panel\n"
            "\t\t  Q     - Back to Main Menu\n\n");
        bool running = true;
        while (running) {
            showStatusBar();
            cout << Terminal::cyan("\n\t\t  Command: ");
            int key = Terminal::getKey(); cout << "\n";
            if      (key == ' ')               runScanCycle();
            else if (key == 'a' || key == 'A') panel.open();
            else if (key == 'q' || key == 'Q' || key == 27) running = false;
            else { Terminal::printError("Invalid key."); SLEEP_MS(400); }
        }
    }
};

// ============================================================
//  CLASS: ManualEntry
//  Operator-driven vehicle data input (single or batch).
// ============================================================
class ManualEntry {
private:
    Database& db;

    bool isValidPlate(const string& plate) const { return plate.length() >= 4; }

    void showConfirmation(const Vehicle& v) const {
        Terminal::separator();
        cout << Terminal::yellow("\t\t  Preview of record to be saved:\n");
        v.displayInfo();
        Terminal::separator();
        cout << Terminal::cyan("\t\t  Save this record? (y/n): ");
    }

public:
    explicit ManualEntry(Database& database) : db(database) {}

    // Add a single vehicle via keyboard
    void addRecord() {
        Terminal::clearScreen(); Terminal::printHeader("Manual Vehicle Entry");
        cout << Terminal::cyan("\n\t\t  Enter vehicle details:\n\n");
        Vehicle v;
        v.inputManually();
        if (!isValidPlate(v.getRegNo())) {
            Terminal::printError("Invalid plate. Record not saved."); Terminal::pressEnter(); return;
        }
        if (db.findByRegNo(v.getRegNo()) != -1) {
            Terminal::printError("Plate already exists: " + v.getRegNo()); Terminal::pressEnter(); return;
        }
        showConfirmation(v);
        int c = Terminal::getKey(); cout << "\n";
        if (c == 'y' || c == 'Y') {
            db.addVehicle(v); db.saveToFile();
            Terminal::printSuccess("Saved: " + v.getRegNo());
        } else { Terminal::printInfo("Entry cancelled."); }
        Terminal::pressEnter();
    }

    // Add multiple vehicles in one session
    void addBatch() {
        Terminal::clearScreen(); Terminal::printHeader("Batch Manual Entry");
        int count = Terminal::readInt("Number of vehicles to enter");
        for (int i = 0; i < count; i++) {
            Terminal::separator();
            cout << Terminal::cyan("\t\t  Vehicle " + to_string(i + 1) + " of " + to_string(count) + ":\n");
            Vehicle v;
            v.inputManually();
            if (db.addVehicle(v)) Terminal::printSuccess("Added: " + v.getRegNo());
        }
        db.saveToFile();
        Terminal::printSuccess("Batch complete. " + to_string(count) + " vehicle(s) processed.");
        Terminal::pressEnter();
    }
};

// ============================================================
//  CLASS: TrafficApp
//  Top-level controller. main() only creates this and
//  calls run(). All logic lives in methods below.
// ============================================================
class TrafficApp {
private:
    Database    db;
    Scanner     scanner;
    ManualEntry manualEntry;
    AdminPanel  adminPanel;

    void loadData() {
        cout << Terminal::cyan("\n\t\t  Loading saved data");
        Terminal::animateText("...", 300);
        int loaded = db.loadFromFile();
        if (loaded > 0) Terminal::printSuccess(to_string(loaded) + " record(s) loaded.");
        else            Terminal::printInfo("No saved data found. Starting fresh.");
        SLEEP_MS(600);
    }

    void showWelcomeScreen() {
        Terminal::clearScreen();
        int colorIdx = rand() % 7;
        string carColor = Terminal::getColorCode(colorIdx);
        cout << Terminal::blue("\n\n\t\t    Welcome to the Traffic Control System");
        Terminal::animateText("...\n", 200);
        Terminal::drawCar(carColor);
        Terminal::animateText("\t\t   ......................................\n", 5);
        cout << Terminal::bold(Terminal::yellow("\n\t\t\t\tLet's Start!\n"));
        Terminal::animateText("\t\t   ......................................\n\n", 5);
        SLEEP_MS(400);
    }

    void printMainMenu() const {
        Terminal::separator();
        cout << Terminal::bold("\t\t  MAIN MENU\n\n");
        cout << Terminal::red("\t\t  1") << ") AI Automated Scanner\n";
        cout << Terminal::red("\t\t  2") << ") Manual Vehicle Entry\n";
        cout << Terminal::red("\t\t  3") << ") Batch Manual Entry\n";
        cout << Terminal::red("\t\t  4") << ") Admin Panel (History & Reports)\n";
        cout << Terminal::red("\t\t  Q") << ") Quit\n";
        Terminal::separator();
        cout << Terminal::cyan("\t\t  Choice: ");
    }

    void runMainMenu() {
        bool running = true;
        while (running) {
            Terminal::clearScreen(); Terminal::printHeader("Traffic Control System");
            if (!db.isEmpty()) {
                cout << Terminal::cyan("\t\t  Records: ")    << Terminal::yellow(to_string(db.getTotal()))
                     << "  " << Terminal::cyan("Violations: ") << Terminal::red(to_string(db.countViolations()))
                     << "  " << Terminal::cyan("Fines: ")
                     << Terminal::yellow("Rp " + to_string(db.totalFinesCollected())) << "\n";
            }
            printMainMenu();
            int choice = Terminal::getKey(); cout << "\n";
            switch (choice) {
                case '1': scanner.launchScanMode();  break;
                case '2': manualEntry.addRecord();   break;
                case '3': manualEntry.addBatch();    break;
                case '4': adminPanel.open();         break;
                case 'q': case 'Q':
                    Terminal::printSuccess("Thank you! Goodbye.");
                    SLEEP_MS(700); running = false; break;
                default: Terminal::printError("Invalid choice."); SLEEP_MS(500); break;
            }
        }
    }

public:
    TrafficApp() : scanner(db), manualEntry(db), adminPanel(db) {}

    // Only public method - called by main()
    void run() {
        srand(static_cast<unsigned>(time(nullptr)));
        loadData();
        showWelcomeScreen();
        runMainMenu();
    }
};

// ============================================================
//  MAIN  -  creates one object, calls one method. That's it.
// ============================================================
int main() {
    TrafficApp app;
    app.run();
    return 0;
}
