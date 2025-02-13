#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

class Employee {
private:
    int empID;
    string name;
    string position;
    double salary;

public:
    Employee(int id, string n, string pos, double sal) {
        empID = id;
        name = n;
        position = pos;
        salary = sal;
    }

    int getEmpID() const { return empID; }
    string getName() const { return name; }
    string getPosition() const { return position; }
    double getSalary() const { return salary; }

    void displayEmployee() const {
        cout << "ID: " << empID << " | Name: " << name << " | Position: " << position
             << " | Salary: ?" << fixed << setprecision(2) << salary << endl;
    }

    void saveToFile(ofstream &outfile) const {
        outfile << empID << "," << name << "," << position << "," << salary << endl;
    }

    static Employee loadFromFile(ifstream &infile) {
        int id;
        string name, position;
        double salary;
        infile >> id;
        infile.ignore();
        getline(infile, name, ',');
        getline(infile, position, ',');
        infile >> salary;
        return Employee(id, name, position, salary);
    }
};

bool isEmpIDUnique(vector<Employee> &employees, int id) {
    for (const auto &e : employees) {
        if (e.getEmpID() == id) {
            return false;
        }
    }
    return true;
}

Employee* findEmployee(vector<Employee> &employees, int id) {
    for (auto &e : employees) {
        if (e.getEmpID() == id) {
            return &e;
        }
    }
    return nullptr;
}

void saveEmployeesToFile(vector<Employee> &employees) {
    ofstream outfile("employees.txt", ios::trunc);
    for (const auto &e : employees) {
        e.saveToFile(outfile);
    }
    outfile.close();
}

void loadEmployeesFromFile(vector<Employee> &employees) {
    ifstream infile("employees.txt");
    if (!infile) return;

    while (!infile.eof()) {
        int id;
        string name, position;
        double salary;

        infile >> id;
        infile.ignore();
        if (getline(infile, name, ',') && getline(infile, position, ',') && infile >> salary) {
            employees.push_back(Employee(id, name, position, salary));
        }
    }
    infile.close();
}

int main() {
    vector<Employee> employees;
    loadEmployeesFromFile(employees);

    int choice;
    while (true) {
        cout << "\n===== Employee Management System =====\n";
        cout << "1. Add Employee\n";
        cout << "2. View All Employees\n";
        cout << "3. Search Employee by ID\n";
        cout << "4. Update Employee Details\n";
        cout << "5. Delete Employee\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int id;
            string name, position;
            double salary;
            cout << "Enter Employee ID: ";
            cin >> id;
            cin.ignore();

            if (!isEmpIDUnique(employees, id)) {
                cout << "Error! Employee ID already exists.\n";
                continue;
            }

            cout << "Enter Name: ";
            getline(cin, name);
            cout << "Enter Position: ";
            getline(cin, position);
            cout << "Enter Salary: ?";
            cin >> salary;

            employees.push_back(Employee(id, name, position, salary));
            saveEmployeesToFile(employees);
            cout << "Employee added successfully!\n";

        } else if (choice == 2) {
            cout << "\n===== List of Employees =====\n";
            for (const auto &e : employees) {
                e.displayEmployee();
            }

        } else if (choice == 3) {
            int id;
            cout << "Enter Employee ID to search: ";
            cin >> id;
            Employee* emp = findEmployee(employees, id);

            if (emp) {
                emp->displayEmployee();
            } else {
                cout << "Employee not found!\n";
            }

        } else if (choice == 4) {
            int id;
            cout << "Enter Employee ID to update: ";
            cin >> id;
            Employee* emp = findEmployee(employees, id);

            if (emp) {
                string newName, newPosition;
                double newSalary;
                cout << "Enter New Name: ";
                cin.ignore();
                getline(cin, newName);
                cout << "Enter New Position: ";
                getline(cin, newPosition);
                cout << "Enter New Salary: ?";
                cin >> newSalary;

                *emp = Employee(id, newName, newPosition, newSalary);
                saveEmployeesToFile(employees);
                cout << "Employee details updated successfully!\n";
            } else {
                cout << "Employee not found!\n";
            }

        } else if (choice == 5) {
            int id;
            cout << "Enter Employee ID to delete: ";
            cin >> id;

            for (auto it = employees.begin(); it != employees.end(); ++it) {
                if (it->getEmpID() == id) {
                    employees.erase(it);
                    cout << "Employee record deleted successfully.\n";
                    saveEmployeesToFile(employees);
                    break;
                }
            }

        } else if (choice == 6) {
            cout << "Exiting Employee Management System. Thank you!\n";
            break;
        } else {
            cout << "Invalid choice! Try again.\n";
        }
    }
    return 0;
}
