#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <limits>
using namespace std;

class Student {
public:
    string rollNumber;
    string name;
    string department;
    Student* next;

    Student(const string& studentRollNumber, const string& studentName, const string& studentDepartment)
        : rollNumber(studentRollNumber), name(studentName), department(studentDepartment), next(nullptr) {}
};

class StudentDatabase {
private:
    Student* head;

public:
    StudentDatabase() : head(nullptr) {}

    void addStudent(const string& rollNumber, const string& name, const string& department) {
        Student* newStudent = new Student(rollNumber, name, department);
        newStudent->next = head;
        head = newStudent;
        cout << "Student added successfully." << endl;
    }

    void displayAllStudents() const {
        if (head == nullptr) {
            cout << "\n\n\t\tNo students in the database." << endl;
            return;
        }

        Student* current = head;
        cout << "\n\n\tStudent Database:" << endl;

        // Table header
        cout << "\t--------------------------------------------------------------\n";
        cout << "\t| Roll Number  |      Name        |         Department        |\n";
        cout << "\t--------------------------------------------------------------\n";

        while (current != nullptr) {
            // Table row
            cout << "\t| " << setw(12) << current->rollNumber << " | " << setw(16) << current->name << " | " << setw(25) << current->department << " |\n";
            current = current->next;
        }

        // Table footer
        cout << "\t--------------------------------------------------------------\n";
    }

    Student* findStudentByRollNumber(const string& rollNumber) const {
        Student* current = head;
        while (current != nullptr) {
            if (current->rollNumber == rollNumber) {
                return current;
            }
            current = current->next;
        }
        return nullptr; // Student not found
    }

    void deleteStudentByRollNumber(const string& rollNumber) {
        Student* current = head;
        Student* previous = nullptr;

        while (current != nullptr && current->rollNumber != rollNumber) {
            previous = current;
            current = current->next;
        }

        if (current == nullptr) {
            cout << "Student with Roll Number " << rollNumber << " not found. Cannot delete." << endl;
            return;
        }

        // If the student to be deleted is the head
        if (previous == nullptr) {
            head = current->next;
        } else {
            // If the student to be deleted is not the head
            previous->next = current->next;
        }

        delete current;
        cout << "Student with Roll Number " << rollNumber << " deleted successfully." << endl;
    }

    void loadFromFile(const string& filename) {
        cout << "Loading data from file: " << filename << endl;

        ifstream inFile(filename);
        if (inFile.is_open()) {
            string line;

            // Check if the file is empty
            if (inFile.peek() == ifstream::traits_type::eof()) {
                cout << "File is empty. No data loaded." << endl;
                inFile.close();
                return;
            }

            while (getline(inFile, line)) {
                istringstream iss(line);
                string rollNumber, name, department;

                if (getline(iss, rollNumber, '|') && getline(iss, name, '|') && getline(iss, department)) {
                    // Trim leading and trailing whitespaces
                    rollNumber = trim(rollNumber);
                    name = trim(name);
                    department = trim(department);

                    addStudent(rollNumber, name, department);
                }
            }

            inFile.close();
            cout << "Data loaded from file successfully." << endl;
        } else {
            cout << "Error: Unable to open the file for reading." << endl;
        }
    }

    void saveToFile(const string& filename) const {
        ofstream outFile(filename, ios::app);  // Open the file in append mode
        if (outFile.is_open()) {
            Student* current = head;
            while (current != nullptr) {
                outFile << current->rollNumber << " | " << current->name << " | " << current->department << endl;
                current = current->next;
            }
            outFile.close();
            cout << "Data saved to file successfully." << endl;
        } else {
            cout << "Error: Unable to open the file for writing." << endl;
        }
    }

private:
    // Helper function to trim whitespaces from the beginning and end of a string
    string trim(const string& str) {
        size_t first = str.find_first_not_of(' ');
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }
};


int main() {
    StudentDatabase studentDB;
    int choice;
    string filename = "students.txt";

    cout << "\t\t _______________________________________________________________________________\n";
    cout << "\t\t|                                                                               |\n";
    cout << "\t\t|                    STUDENTS DATABASE MANAGEMENT SYSTEM                        |\n";
    cout << "\t\t|_______________________________________________________________________________|\n\n\n";

    // Load data from file at the beginning
    studentDB.loadFromFile(filename);

    do {
        cout << "\n1. Add Student\n"
             << "2. Display All Students\n"
             << "3. Search Student by Roll Number\n"
             << "4. Delete Student by Roll Number\n"
             << "5. Save Data to File\n"
             << "6. Exit\n"
             << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string rollNumber, department, name;

                cout << "Enter student Roll Number: ";
                cin >> rollNumber;

                // Clear the input buffer
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Enter student name: ";
                getline(cin, name);

                cout << "Enter student department: ";
                getline(cin, department);

                studentDB.addStudent(rollNumber, name, department);
                break;
            }
            case 2:
                studentDB.displayAllStudents();
                break;
            case 3: {
                string searchRollNumber;
                cout << "Enter student Roll Number to search: ";
                cin >> searchRollNumber;

                Student* foundStudent = studentDB.findStudentByRollNumber(searchRollNumber);
                if (foundStudent != nullptr) {
                    cout << "\n\n\t\tStudent found - Roll Number: " << foundStudent->rollNumber << "\t Name: " << foundStudent->name
                         << "\t Department: " << foundStudent->department << endl;
                } else {
                    cout << "\n\n\t\tStudent not found." << endl;
                }
                break;
            }
            case 4: {
                string deleteRollNumber;
                cout << "Enter student Roll Number to delete: ";
                cin >> deleteRollNumber;

                studentDB.deleteStudentByRollNumber(deleteRollNumber);
                break;
            }
            case 5: 
                studentDB.saveToFile(filename);
                break;
            case 6:
                cout << "Exiting the program." << endl;
                break;

            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
        }

    } while (choice != 6);

    return 0;
}
