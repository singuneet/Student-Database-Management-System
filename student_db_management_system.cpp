#include <iostream>
#include <fstream>
#include <string>

class Student {
private:
    int rollNumber;
    std::string name;
    int age;
    std::string course;
    char grade;

public:
    // Constructor
    Student() : rollNumber(0), age(0), grade(' ') {}

    // Input Student Details
    void inputDetails() {
        std::cout << "Enter Roll Number: "; std::cin >> rollNumber;
        std::cin.ignore();  // To consume leftover newline
        std::cout << "Enter Name: "; getline(std::cin, name);
        std::cout << "Enter Age: "; std::cin >> age;
        std::cout << "Enter Course: "; std::cin.ignore(); getline(std::cin, course);
        std::cout << "Enter Grade: "; std::cin >> grade;
    }

    // Display Student Details
    void displayDetails() const {
        std::cout << "Roll Number: " << rollNumber << "\n"
                  << "Name: " << name << "\n"
                  << "Age: " << age << "\n"
                  << "Course: " << course << "\n"
                  << "Grade: " << grade << "\n";
    }

    // Getters for attributes
    int getRollNumber() const { return rollNumber; }

    // Function to update details
    void updateDetails() {
        std::cout << "Update student details:\n";
        inputDetails();
    }
};

// Function to add a student record to the file
void addStudent() {
    std::ofstream file("students.dat", std::ios::binary | std::ios::app);
    Student student;
    student.inputDetails();
    file.write(reinterpret_cast<char*>(&student), sizeof(Student));
    file.close();
    std::cout << "Student record added successfully!\n";
}

// Function to view all student records from the file
void viewAllStudents() {
    std::ifstream file("students.dat", std::ios::binary);
    Student student;
    while (file.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
        student.displayDetails();
        std::cout << "-----------------------------\n";
    }
    file.close();
}

// Function to search a student by roll number
void searchStudent(int rollNumber) {
    std::ifstream file("students.dat", std::ios::binary);
    Student student;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
        if (student.getRollNumber() == rollNumber) {
            student.displayDetails();
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "Student with roll number " << rollNumber << " not found.\n";
    }
    file.close();
}

// Function to delete a student record by roll number
void deleteStudent(int rollNumber) {
    std::ifstream file("students.dat", std::ios::binary);
    std::ofstream tempFile("temp.dat", std::ios::binary);
    Student student;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
        if (student.getRollNumber() != rollNumber) {
            tempFile.write(reinterpret_cast<char*>(&student), sizeof(Student));
        } else {
            found = true;
        }
    }
    file.close();
    tempFile.close();

    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found) {
        std::cout << "Student record deleted successfully.\n";
    } else {
        std::cout << "Student with roll number " << rollNumber << " not found.\n";
    }
}

// Function to update a student record by roll number
void updateStudent(int rollNumber) {
    std::fstream file("students.dat", std::ios::binary | std::ios::in | std::ios::out);
    Student student;
    bool found = false;

    while (!found && file.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
        if (student.getRollNumber() == rollNumber) {
            std::cout << "Current details:\n";
            student.displayDetails();
            std::cout << "Enter new details:\n";
            student.updateDetails();
            file.seekp(-static_cast<int>(sizeof(Student)), std::ios::cur);
            file.write(reinterpret_cast<char*>(&student), sizeof(Student));
            found = true;
        }
    }
    if (!found) {
        std::cout << "Student with roll number " << rollNumber << " not found.\n";
    }
    file.close();
}

// Function to display the menu and perform operations
void menu() {
    int choice, rollNumber;
    do {
        std::cout << "1. Add Student\n";
        std::cout << "2. View All Students\n";
        std::cout << "3. Search Student\n";
        std::cout << "4. Delete Student\n";
        std::cout << "5. Update Student\n";
        std::cout << "6. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewAllStudents(); break;
            case 3:
                std::cout << "Enter roll number: ";
                std::cin >> rollNumber;
                searchStudent(rollNumber);
                break;
            case 4:
                std::cout << "Enter roll number to delete: ";
                std::cin >> rollNumber;
                deleteStudent(rollNumber);
                break;
            case 5:
                std::cout << "Enter roll number to update: ";
                std::cin >> rollNumber;
                updateStudent(rollNumber);
                break;
            case 6: std::cout << "Exiting...\n"; break;
            default: std::cout << "Invalid choice!\n"; break;
        }
    } while (choice != 6);
}

int main() {
    menu();
    return 0;
}
