#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

using namespace std;

enum class JobType {
    ENGINEERING,
    CLEANING,
    ACCOUNTING,
    MANAGEMENT
};

class Job {
private:
    JobType type;
    double rate;

public:
    Job(JobType t, double r) : type(t), rate(r) {
        if (r <= 0 || r > 10000)
            throw invalid_argument("Rate must be between 1 and 10,000.");
    }

    JobType getType() const { return type; }
    double getRate() const { return rate; }

    ~Job() {
        cout << "job information.\n";
    }
};

class PerformedJob {
private:
    JobType type;
    int hours;

public:
    PerformedJob(JobType t, int h) : type(t), hours(h) {
        if (h <= 0 || h > 200)
            throw invalid_argument("Hours must be between 1 and 200.");
    }

    JobType getType() const { return type; }
    int getHours() const { return hours; }

    ~PerformedJob() {
        cout << "performed job information.\n";
    }
};

class Employee {
private:
    string name;
    vector<shared_ptr<PerformedJob>> performedJobs;

public:
    Employee(const string& n) : name(n) {}

    void addJob(JobType type, int hours) {
        performedJobs.push_back(make_shared<PerformedJob>(type, hours));
    }

    double calculateSalary(const vector<Job>& jobs) const {
        double total = 0;
        for (const auto& job : performedJobs) {
            for (const auto& definedJob : jobs) {
                if (definedJob.getType() == job->getType()) {
                    total += job->getHours() * definedJob.getRate();
                }
            }
        }
        return total;
    }

    void displayInfo() const {
        cout << "Employee: " << name << endl;
        for (const auto& job : performedJobs) {
            cout << "  Job type: " << static_cast<int>(job->getType())
                << ", Hours: " << job->getHours() << endl;
        }
    }

    string getName() const { return name; }

    ~Employee() {
        cout << "employee information.\n";
    }
};

class PayrollSystem {
private:
    vector<shared_ptr<Employee>> employees;
    vector<Job> jobs;

    PayrollSystem() {}

public:
    PayrollSystem(const PayrollSystem&) = delete;
    PayrollSystem& operator=(const PayrollSystem&) = delete;

    static PayrollSystem& getInstance() {
        static PayrollSystem instance;
        return instance;
    }

    void addJob(JobType type, double rate) {
        jobs.push_back(Job(type, rate));
    }

    void addEmployee(const string& name) {
        employees.push_back(make_shared<Employee>(name));
    }

    void addPerformedJob(const string& employeeName, JobType type, int hours) {
        for (auto& employee : employees) {
            if (employee->getName() == employeeName) {
                employee->addJob(type, hours);
                return;
            }
        }
        cout << "Employee not found." << endl;
    }

    double getEmployeeSalary(const string& name) const {
        for (const auto& employee : employees) {
            if (employee->getName() == name) {
                return employee->calculateSalary(jobs);
            }
        }
        cout << "Employee not found." << endl;
        return 0;
    }

    double getTotalSalaries() const {
        double total = 0;
        for (const auto& employee : employees) {
            total += employee->calculateSalary(jobs);
        }
        return total;
    }

    void displayEmployees() const {
        for (const auto& employee : employees) {
            employee->displayInfo();
        }
    }

    ~PayrollSystem() {
        cout << "payroll system.\n";
    }
};

void menu() {
    PayrollSystem& payroll = PayrollSystem::getInstance();
    int choice;
    string name;
    int type, hours;
    double rate;

    cout << "\nMenu:\n1. Add Job Type\n2. Add Employee\n3. Add Performed Job\n4. Calculate Employee Salary\n5. Calculate Total Salaries\n6. Display All Employees\n7. Exit\n";

    do {
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "Enter job type (0=Engineering, 1=Cleaning, 2=Accounting, 3=Management): ";
            cin >> type;
            if (type < 0 || type > 3) {
                cout << "Invalid job type. Please try again." << endl;
                break;
            }
            cout << "Enter hourly rate (1 to 10,000): ";
            cin >> rate;
            if (rate <= 0 || rate > 10000) {
                cout << "Invalid rate. Please try again." << endl;
                break;
            }
            payroll.addJob(static_cast<JobType>(type), rate);
            break;
        }
        case 2: {
            cout << "Enter employee name: ";
            cin >> name;
            payroll.addEmployee(name);
            break;
        }
        case 3: {
            cout << "Enter employee name, job type (0=Engineering, 1=Cleaning, 2=Accounting, 3=Management), and hours worked (1 to 200): ";
            cin >> name >> type >> hours;
            if (type < 0 || type > 3) {
                cout << "Invalid job type. Please try again." << endl;
                break;
            }
            if (hours <= 0 || hours > 200) {
                cout << "Invalid number of hours. Please try again." << endl;
                break;
            }
            payroll.addPerformedJob(name, static_cast<JobType>(type), hours);
            break;
        }
        case 4: {
            cout << "Enter employee name: ";
            cin >> name;
            double total = payroll.getEmployeeSalary(name);
            cout << "Total salary for employee " << name << ": " << total << " RUB." << endl;
            break;
        }
        case 5: {
            double total = payroll.getTotalSalaries();
            cout << "Total salaries for all employees: " << total << " RUB." << endl;
            break;
        }
        case 6: {
            payroll.displayEmployees();
            break;
        }
        case 7: {
            cout << "Exiting program." << endl;
            break;
        }
        default: {
            cout << "Invalid choice. Try again." << endl;
            break;
        }
        }
    } while (choice != 7);
}

int main() {
    menu();
    return 0;
}