#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm> 
#include <fstream>   
using namespace std;


const int MAX_STUDENTS_PER_GROUP = 50;
const int MAX_STUDENTS_PER_SPORT = 20;
const int MAX_MEMBERS_PER_CLUB = 60;
const double MAX_PERCENTAGE_MALE_SPORT = 0.75;
const double MAX_PERCENTAGE_MALE_CLUB = 0.5;


struct Student {
    string firstname;
    string surname;
    string gender;
    int age;
    int bbitGroup;
    int sportIndex;           
    vector<int> clubIndices;  
};

struct Activity {
    string name;
    int currentCapacity;
    int maxCapacity;
    vector<int> members;  
};

// Global vectors to store data
vector<Student> students;
vector<Activity> sports;
vector<Activity> clubsSocieties;

// Function prototypes
void displayMenu();
void addStudent();
void viewStudents();
void viewClubsSocieties();
void viewSports();
void viewGroupedStudents();
void saveData();
void assignActivity(Student& student);
bool isMaleExceedPercentage(const Activity& activity, const Student& student, double maxPercentage);
void displayActivities(const vector<Activity>& activities);
void displayStudentsByGroup(int group);
void loadData();  

int main() {
    loadData();  

    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                viewStudents();
                break;
            case 3:
                viewClubsSocieties();
                break;
            case 4:
                viewSports();
                break;
            case 5:
                viewGroupedStudents();
                break;
            case 6:
                saveData();
                break;
            case 7:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
        }
    } while (choice != 7);

    return 0;
}

void displayMenu() {
    cout << "===== Co-curricular Activities Management =====" << endl;
    cout << "1. Add Student" << endl;
    cout << "2. View Students" << endl;
    cout << "3. View Clubs/Societies" << endl;
    cout << "4. View Sports" << endl;
    cout << "5. View Grouped Students" << endl;
    cout << "6. Save all Files" << endl;
    cout << "7. Exit" << endl;
    cout << "Enter your choice: ";
}

void addStudent() {
    Student newStudent;
    cout << "Enter firstname: ";
    cin >> newStudent.firstname;
    cout << "Enter surname: ";
    cin >> newStudent.surname;
    cout << "Enter gender (Male/Female): ";
    cin >> newStudent.gender;
    cout << "Enter age: ";
    cin >> newStudent.age;
    cout << "Enter BBIT group (1, 2, or 3): ";
    cin >> newStudent.bbitGroup;

    // Check if student already exists
    auto it = find_if(students.begin(), students.end(), [&](const Student& s) {
        return s.firstname == newStudent.firstname && s.surname == newStudent.surname;
    });
    if (it != students.end()) {
        cout << "Student already exists!" << endl;
        return;
    }

    // Assign activities to the student
    assignActivity(newStudent);

    // Add the student to the students vector
    students.push_back(newStudent);

    cout << "Student added successfully!" << endl;
}

void viewStudents() {
    if (students.empty()) {
        cout << "No students to display." << endl;
        return;
    }

    cout << "===== Students =====" << endl;
    cout << setw(20) << "Name" << setw(10) << "Gender" << setw(5) << "Age" << setw(15) << "Sport" << setw(20) << "Clubs/Societies" << endl;

    for (const auto& student : students) {
        cout << setw(20) << student.firstname + " " + student.surname << setw(10) << student.gender << setw(5) << student.age;

        if (student.sportIndex != -1) {
            cout << setw(15) << sports[student.sportIndex].name;
        } else {
            cout << setw(15) << "-";
        }

        if (!student.clubIndices.empty()) {
            string clubs;
            for (int clubIndex : student.clubIndices) {
                clubs += clubsSocieties[clubIndex].name + ", ";
            }
            clubs = clubs.substr(0, clubs.size() - 2);  
            cout << setw(20) << clubs;
        } else {
            cout << setw(20) << "-";
        }

        cout << endl;
    }
}

void viewClubsSocieties() {
    if (clubsSocieties.empty()) {
        cout << "No clubs/societies available." << endl;
        return;
    }

    cout << "===== Clubs/Societies =====" << endl;
    displayActivities(clubsSocieties);
}

void viewSports() {
    if (sports.empty()) {
        cout << "No sports available." << endl;
        return;
    }

    cout << "===== Sports =====" << endl;
    displayActivities(sports);
}

void viewGroupedStudents() {
    if (students.empty()) {
        cout << "No students to display." << endl;
        return;
    }

    cout << "===== Grouped Students =====" << endl;
    cout << "BBIT Group 1:" << endl;
    displayStudentsByGroup(1);
    cout << endl << "BBIT Group 2:" << endl;
    displayStudentsByGroup(2);
    cout << endl << "BBIT Group 3:" << endl;
    displayStudentsByGroup(3);
}

void saveData() {
    ofstream outFile("students_data.csv");
    if (!outFile) {
        cerr << "Error opening file for writing." << endl;
        return;
    }

    outFile << "Name,Gender,Age,BBIT Group,Sport,Clubs/Societies" << endl;
    for (const auto& student : students) {
        outFile << student.firstname + " " + student.surname << ","
                << student.gender << ","
                << student.age << ","
                << student.bbitGroup << ",";

        if (student.sportIndex != -1) {
            outFile << sports[student.sportIndex].name << ",";
        } else {
            outFile << ",";
        }

        if (!student.clubIndices.empty()) {
            string clubs;
            for (int clubIndex : student.clubIndices) {
                clubs += clubsSocieties[clubIndex].name + ", ";
            }
            clubs = clubs.substr(0, clubs.size() - 2);  
            outFile << clubs;
        } else {
            outFile << "-";
        }

        outFile << endl;
    }

    cout << "Data saved successfully to students_data.csv" << endl;
}

void assignActivity(Student& student) {
    char choice;

    // Assign sport
    cout << "Do you want to participate in a sport? (y/n): ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        if (sports.empty()) {
            cout << "No sports available." << endl;
        } else {
            displayActivities(sports);
            int sportIndex;
            cout << "Enter the index of the sport you want to join: ";
            cin >> sportIndex;
            if (sportIndex >= 0 && sportIndex < sports.size()) {
                if (sports[sportIndex].currentCapacity < MAX_STUDENTS_PER_SPORT) {
                    student.sportIndex = sportIndex;
                    sports[sportIndex].members.push_back(students.size() - 1);  
                    sports[sportIndex].currentCapacity++;
                    cout << "Sport assigned successfully!" << endl;
                } else {
                    cout << "Sport is full. Cannot assign." << endl;
                }
            } else {
                cout << "Invalid sport index." << endl;
            }
        }
    }

    // Assign clubs/societies
    if (student.sportIndex == -1 || student.clubIndices.size() < 3) {
        cout << "Do you want to join a club/society? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            if (clubsSocieties.empty()) {
                cout << "No clubs/societies available." << endl;
            } else {
                displayActivities(clubsSocieties);
                int clubIndex;
                cout << "Enter the index of the club/society you want to join: ";
                cin >> clubIndex;
                if (clubIndex >= 0 && clubIndex < clubsSocieties.size()) {
                    if (clubsSocieties[clubIndex].currentCapacity < MAX_MEMBERS_PER_CLUB) {
                        // Check if joining this club exceeds maximum male percentage
                        if (!isMaleExceedPercentage(clubsSocieties[clubIndex], student, MAX_PERCENTAGE_MALE_CLUB)) {
                            student.clubIndices.push_back(clubIndex);
                            clubsSocieties[clubIndex].members.push_back(students.size() - 1);  // Index of the newly added student
                            clubsSocieties[clubIndex].currentCapacity++;
                            cout << "Club/Society assigned successfully!" << endl;
                        } else {
                            cout << "Joining this club exceeds maximum male percentage." << endl;
                        }
                    } else {
                        cout << "Club/Society is full. Cannot assign." << endl;
                    }
                } else {
                    cout << "Invalid club/society index." << endl;
                }
            }
        }
    }
}

bool isMaleExceedPercentage(const Activity& activity, const Student& student, double maxPercentage) {
    if (student.gender == "Male") {
        int currentMaleCount = count_if(activity.members.begin(), activity.members.end(), [&](int index) {
            return students[index].gender == "Male";
        });
        double currentMalePercentage = static_cast<double>(currentMaleCount + 1) / (activity.currentCapacity + 1);  // +1 for the new student
        return currentMalePercentage > maxPercentage;
    }
    return false;
}

void displayActivities(const vector<Activity>& activities) {
    cout << setw(5) << "Index" << setw(20) << "Name" << setw(10) << "Capacity" << setw(15) << "Current Count" << endl;
    for (int i = 0; i < activities.size(); ++i) {
        cout << setw(5) << i << setw(20) << activities[i].name << setw(10) << activities[i].maxCapacity << setw(15) << activities[i].currentCapacity << endl;
    }
}

void displayStudentsByGroup(int group) {
    for (const auto& student : students) {
        if (student.bbitGroup == group) {
            cout << student.firstname + " " + student.surname << endl;
        }
    }
}

void loadData() {
    // Initialize sports
    sports = {
            {"Rugby", 0, MAX_STUDENTS_PER_SPORT, {}},
            {"Athletics", 0, MAX_STUDENTS_PER_SPORT, {}},
            {"Swimming", 0, MAX_STUDENTS_PER_SPORT, {}},
            {"Soccer", 0, MAX_STUDENTS_PER_SPORT, {}}
    };

    // Initialize clubs/societies
    clubsSocieties = {
            {"Journalism", 0, MAX_MEMBERS_PER_CLUB, {}},
            {"Red Cross Society", 0, MAX_MEMBERS_PER_CLUB, {}},
            {"AISEC", 0, MAX_MEMBERS_PER_CLUB, {}},
            {"Business Club", 0, MAX_MEMBERS_PER_CLUB, {}},
            {"Computer Club", 0, MAX_MEMBERS_PER_CLUB, {}}
    };

    // Sample student data
    students = {
            {"John", "Doe", "Male", 20, 1, 0, {0, 1, 2}},
            {"Jane", "Smith", "Female", 19, 2, 1, {}},
            {"Michael", "Brown", "Male", 21, 1, 2, {3}},
            {"Emily", "Johnson", "Female", 20, 3, -1, {}}
    };

    // Assign members to initial clubs/societies and sports
    for (auto& student : students) {
        if (student.sportIndex != -1) {
            sports[student.sportIndex].members.push_back(&student - &students[0]);  
            sports[student.sportIndex].currentCapacity++;
        }
        for (int clubIndex : student.clubIndices) {
            clubsSocieties[clubIndex].members.push_back(&student - &students[0]);  
            clubsSocieties[clubIndex].currentCapacity++;
        }
    }

    cout << "Initial data loaded successfully." << endl;
}
