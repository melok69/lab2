#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

// Enumeration for job types
enum class JobType {
    ENGINEERING,
    CLEANING,
    ACCOUNTING,
    MANAGEMENT
};

// Class representing a type of job
class Job {
private:
    JobType type;
    double rate; // Hourly rate for the job

public:
    Job(JobType t, double r) : type(t), rate(r) {
        if (r <= 0 || r > 10000)
            throw std::invalid_argument("Rate must be between 1 and 10,000.");
    }

    JobType getType() const { return type; }
    double getRate() const { return rate; }

    ~Job() {
        std::cout << "job information.\n";
    }
};

// Class representing a performed job
class PerformedJob {
private:
    JobType type;
    int hours; // Number of hours worked

public:
    PerformedJob(JobType t, int h) : type(t), hours(h) {
        if (h <= 0 || h > 200)
            throw std::invalid_argument("Hours must be between 1 and 200.");
    }

    JobType getType() const { return type; }
    int getHours() const { return hours; }

    ~PerformedJob() {
        std::cout << "performed job information.\n";
    }
};

// Class representing an employee
class Employee {
private:
    std::string name;
    std::vector<std::shared_ptr<PerformedJob>> performedJobs;

public:
    Employee(const std::string& n) : name(n) {}

    void addJob(JobType type, int hours) {
        performedJobs.push_back(std::make_shared<PerformedJob>(type, hours));
    }

    double calculateSalary(const std::vector<Job>& jobs) const {
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
        std::cout << "Employee: " << name << std::endl;
        for (const auto& job : performedJobs) {
            std::cout << "  Job type: " << static_cast<int>(job->getType())
                << ", Hours: " << job->getHours() << std::endl;
        }
    }

    std::string getName() const { return name; }

    ~Employee() {
        std::cout << "employee information.\n";
    }
};

// Class representing the payroll system
class PayrollSystem {
private:
    std::vector<std::shared_ptr<Employee>> employees;
    std::vector<Job> jobs;

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

    void addEmployee(const std::string& name) {
        employees.push_back(std::make_shared<Employee>(name));
    }

    void addPerformedJob(const std::string& employeeName, JobType type, int hours) {
        for (auto& employee : employees) {
            if (employee->getName() == employeeName) {
                employee->addJob(type, hours);
                return;
            }
        }
        std::cout << "Employee not found." << std::endl;
    }

    double getEmployeeSalary(const std::string& name) const {
        for (const auto& employee : employees) {
            if (employee->getName() == name) {
                return employee->calculateSalary(jobs);
            }
        }
        std::cout << "Employee not found." << std::endl;
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
        std::cout << "payroll system.\n";
    }
};

// Menu for user interaction
void menu() {
    PayrollSystem& payroll = PayrollSystem::getInstance();
    int choice;
    std::string name;
    int type, hours;
    double rate;

    std::cout << "\nMenu:\n1. Add Job Type\n2. Add Employee\n3. Add Performed Job\n4. Calculate Employee Salary\n5. Calculate Total Salaries\n6. Display All Employees\n7. Exit\n";

    do {
        std::cout << "\nEnter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::cout << "Enter job type (0=Engineering, 1=Cleaning, 2=Accounting, 3=Management): ";
            std::cin >> type;
            if (type < 0 || type > 3) {
                std::cout << "Invalid job type. Please try again." << std::endl;
                break;
            }
            std::cout << "Enter hourly rate (1 to 10,000): ";
            std::cin >> rate;
            if (rate <= 0 || rate > 10000) {
                std::cout << "Invalid rate. Please try again." << std::endl;
                break;
            }
            payroll.addJob(static_cast<JobType>(type), rate);
            break;
        }
        case 2: {
            std::cout << "Enter employee name: ";
            std::cin >> name;
            payroll.addEmployee(name);
            break;
        }
        case 3: {
            std::cout << "Enter employee name, job type (0=Engineering, 1=Cleaning, 2=Accounting, 3=Management), and hours worked (1 to 200): ";
            std::cin >> name >> type >> hours;
            if (type < 0 || type > 3) {
                std::cout << "Invalid job type. Please try again." << std::endl;
                break;
            }
            if (hours <= 0 || hours > 200) {
                std::cout << "Invalid number of hours. Please try again." << std::endl;
                break;
            }
            payroll.addPerformedJob(name, static_cast<JobType>(type), hours);
            break;
        }
        case 4: {
            std::cout << "Enter employee name: ";
            std::cin >> name;
            double total = payroll.getEmployeeSalary(name);
            std::cout << "Total salary for employee " << name << ": " << total << " RUB." << std::endl;
            break;
        }
        case 5: {
            double total = payroll.getTotalSalaries();
            std::cout << "Total salaries for all employees: " << total << " RUB." << std::endl;
            break;
        }
        case 6: {
            payroll.displayEmployees();
            break;
        }
        case 7: {
            std::cout << "Exiting program." << std::endl;
            break;
        }
        default: {
            std::cout << "Invalid choice. Try again." << std::endl;
            break;
        }
        }
    } while (choice != 7);
}

int main() {
    menu();
    return 0;
}