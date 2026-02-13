#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Group 10: Employee Payroll System 
// Members: 
// Ailov Krizian S. Anquillano
// Joshua Gabriel Gerong
// Abigail Blanche O. Antipuesto
// Christian kenth Caseres


// Nested structure definition
struct Payroll {
    float basicSalary;   // base salary for the employee
    float allowance;     // additional allowances
    float deductions;    // deductions (taxes, etc.)
    float grossPay;      // computed gross pay (basic + allowance)
    float netPay;        // computed net pay (gross - deductions)
};

struct Employee {
    int id;              // unique employee ID
    char name[50];       // employee full name (uses fgets for input)
    char position[30];   // job position
    struct Payroll pay;  // nested payroll structure
};

// Function prototypes
void displayMenu();
void addEmployee(struct Employee employees[], int *count);
void displayAllEmployees(struct Employee employees[], int count);
void searchEmployee(struct Employee employees[], int count);
void updateEmployee(struct Employee employees[], int count);
void deleteEmployee(struct Employee employees[], int *count);
void generateReport(struct Employee employees[], int count);
void saveToFile(struct Employee employees[], int count);
void loadFromFile(struct Employee employees[], int *count);
float computeGross(float basic, float allowance);
float computeNet(float gross, float deductions);

// Global constants
#define MAX_EMPLOYEES 100
// Use an absolute path for predictable file location (escape backslashes)
#define FILENAME "c:\\Users\\Anquillano\\Desktop\\Coding folders\\employee.txt"

int main() {
    struct Employee employees[MAX_EMPLOYEES];
    int count = 0;
    int choice;
    
    printf("=== EMPLOYEE PAYROLL SYSTEM ===\n");
    
    // Load existing records from file (if present)
    loadFromFile(employees, &count);
    printf("Loaded %d employee records.\n\n", count);
    
    do {
        displayMenu();
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            int c;
            // Clear invalid input from stdin to avoid infinite loop
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input! Please enter a number 1-7.\n\n");
            continue;
        }

        switch(choice) {
            case 1:
                addEmployee(employees, &count);
                break;
            case 2:
                displayAllEmployees(employees, count);
                break;
            case 3:
                searchEmployee(employees, count);
                break;
            case 4:
                updateEmployee(employees, count);
                break;
            case 5:
                deleteEmployee(employees, &count);
                break;
            case 6:
                generateReport(employees, count);
                break;
            case 7:
                // Save on exit to persist the current state
                saveToFile(employees, count);
                printf("Thank you for using our Employee Payroll System have a nice day!\n");
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
        printf("\n");
    } while(choice != 7);
    
    return 0;
}

void displayMenu() {
    // Simple menu display; called each loop iteration
    printf("=== EMPLOYEE PAYROLL SYSTEM ===\n");
    printf("1. Add Employee Record\n");
    printf("2. Display All Employees\n");
    printf("3. Search Employee by ID\n");
    printf("4. Update Employee Record\n");
    printf("5. Delete Employee Record\n");
    printf("6. Generate Payroll Report\n");
    printf("7. Exit\n");
    printf("--------------------------------\n");
}

void addEmployee(struct Employee employees[], int *count) {
    if(*count >= MAX_EMPLOYEES) {
        printf("Error: Maximum employee limit reached!\n");
        return;
    }
    
    struct Employee newEmp;
    
    printf("\n--- Add New Employee ---\n");
    
    // Read numeric ID using scanf; this leaves a newline in the input buffer
    printf("Enter Employee ID: ");
    scanf("%d", &newEmp.id);
    
    // Flush the rest of the line so subsequent fgets reads the name correctly.
    // Use a loop to discard everything until newline or EOF.
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    // Check if ID already exists (prevent duplicates)
    for(int i = 0; i < *count; i++) {
        if(employees[i].id == newEmp.id) {
            printf("Error: Employee ID already exists!\n");
            return;
        }
    }
    
    // Use fgets to accept names with spaces; remove trailing newline.
    printf("Enter Employee Name: ");
    fgets(newEmp.name, 50, stdin);
    newEmp.name[strcspn(newEmp.name, "\n")] = 0; // Remove newline
    
    printf("Enter Position: ");
    fgets(newEmp.position, 30, stdin);
    newEmp.position[strcspn(newEmp.position, "\n")] = 0;
    
    // Read floating values for payroll (missing in your version)
    printf("Enter Basic Salary: ");
    scanf("%f", &newEmp.pay.basicSalary);
    printf("Enter Allowance: ");
    scanf("%f", &newEmp.pay.allowance);
    printf("Enter Deductions: ");
    scanf("%f", &newEmp.pay.deductions);
    
    // Calculate payroll
    newEmp.pay.grossPay = computeGross(newEmp.pay.basicSalary, newEmp.pay.allowance);
    newEmp.pay.netPay = computeNet(newEmp.pay.grossPay, newEmp.pay.deductions);
    
    // Add to array
    employees[*count] = newEmp;
    (*count)++;
    
    // Save the updated list immediately to preserve data on crash/exit
    saveToFile(employees, *count);
    
    printf("Employee added successfully!\n");
}

void displayAllEmployees(struct Employee employees[], int count) {
    if(count == 0) {
        printf("No employee records found!\n");
        return;
    }
    // Print a nicely formatted table of all employees
    printf("\n--- All Employee Records ---\n");
    printf("------------------------------------------------------------"
           "-----------------------------\n");
    printf("ID    Name                 Position          Basic     Allowance  "
           "Deductions  Gross      Net\n");
    printf("------------------------------------------------------------"
           "-----------------------------\n");
    
    for(int i = 0; i < count; i++) {
        printf("%-5d %-20s %-15s %-9.2f %-9.2f %-9.2f %-9.2f %-9.2f\n",
               employees[i].id,
               employees[i].name,
               employees[i].position,
               employees[i].pay.basicSalary,
               employees[i].pay.allowance,
               employees[i].pay.deductions,
               employees[i].pay.grossPay,
               employees[i].pay.netPay);
    }
}

void searchEmployee(struct Employee employees[], int count) {
    if(count == 0) {
        printf("No employee records found!\n");
        return;
    }
    
    int searchId;
    printf("Enter Employee ID to search: ");
    scanf("%d", &searchId);
    
    // Linear search by ID
    for(int i = 0; i < count; i++) {
        if(employees[i].id == searchId) {
            printf("\n--- Employee Found ---\n");
            printf("ID: %d\n", employees[i].id);
            printf("Name: %s\n", employees[i].name);
            printf("Position: %s\n", employees[i].position);
            printf("Basic Salary: %.2f\n", employees[i].pay.basicSalary);
            printf("Allowance: %.2f\n", employees[i].pay.allowance);
            printf("Deductions: %.2f\n", employees[i].pay.deductions);
            printf("Gross Pay: %.2f\n", employees[i].pay.grossPay);
            printf("Net Pay: %.2f\n", employees[i].pay.netPay);
            return;
        }
    }
    
    printf("Employee with ID %d not found!\n", searchId);
}

void updateEmployee(struct Employee employees[], int count) {
    if(count == 0) {
        printf("No employee records found!\n");
        return;
    }
    
    int updateId;
    printf("Enter Employee ID to update: ");
    scanf("%d", &updateId);
    
    // Clear input buffer before using fgets for name/position updates
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    for(int i = 0; i < count; i++) {
        if(employees[i].id == updateId) {
            printf("\n--- Updating Employee ID: %d ---\n", updateId);
            
            // Allow updating name and position with spaces
            printf("Enter new Name (current: %s): ", employees[i].name);
            fgets(employees[i].name, 50, stdin);
            employees[i].name[strcspn(employees[i].name, "\n")] = 0;
            
            // Update numeric fields (scanf is fine here)
            printf("Enter new Position (current: %s): ", employees[i].position);
            fgets(employees[i].position, 30, stdin);
            employees[i].position[strcspn(employees[i].position, "\n")] = 0;
            
            printf("Enter new Basic Salary (current: %.2f): ", employees[i].pay.basicSalary);
            scanf("%f", &employees[i].pay.basicSalary);
            
            printf("Enter new Allowance (current: %.2f): ", employees[i].pay.allowance);
            scanf("%f", &employees[i].pay.allowance);
            
            printf("Enter new Deductions (current: %.2f): ", employees[i].pay.deductions);
            scanf("%f", &employees[i].pay.deductions);
            
            // Recalculate payroll
            employees[i].pay.grossPay = computeGross(employees[i].pay.basicSalary, employees[i].pay.allowance);
            employees[i].pay.netPay = computeNet(employees[i].pay.grossPay, employees[i].pay.deductions);
            
            // Save to file
            saveToFile(employees, count);
            
            printf("Employee record updated successfully!\n");
            return;
        }
    }
    
    printf("Employee with ID %d not found!\n", updateId);
}

void deleteEmployee(struct Employee employees[], int *count) {
    if(*count == 0) {
        printf("No employee records found!\n");
        return;
    }
    
    int deleteId;
    printf("Enter Employee ID to delete: ");
    scanf("%d", &deleteId);
    
    for(int i = 0; i < *count; i++) {
        if(employees[i].id == deleteId) {
            // Shift all elements after the deleted one
            for(int j = i; j < *count - 1; j++) {
                employees[j] = employees[j + 1];
            }
            (*count)--;
            
            // Save new state to file
            saveToFile(employees, *count);
            
            printf("Employee with ID %d deleted successfully!\n", deleteId);
            return;
        }
    }
    
    printf("Employee with ID %d not found!\n", deleteId);
}

void generateReport(struct Employee employees[], int count) {
    if(count == 0) {
        printf("No employee records found!\n");
        return;
    }
    
    float totalGross = 0, totalNet = 0;
    
    printf("\n--- PAYROLL REPORT ---\n");
    printf("-------------------------------------------------\n");
    printf("ID    Name                 Position     Gross      Net\n");
    printf("-------------------------------------------------\n");
    
    // Print each employee and accumulate totals
    for(int i = 0; i < count; i++) {
        printf("%-5d %-20s %-12s %-9.2f %-9.2f\n",
               employees[i].id,
               employees[i].name,
               employees[i].position,
               employees[i].pay.grossPay,
               employees[i].pay.netPay);
        
        totalGross += employees[i].pay.grossPay;
        totalNet += employees[i].pay.netPay;
    }
    
    printf("-------------------------------------------------\n");
    printf("Total Employees: %d\n", count);
    printf("Total Gross Pay: %.2f\n", totalGross);
    printf("Total Net Pay: %.2f\n", totalNet);
}

void saveToFile(struct Employee employees[], int count) {
    // Opens file in write mode which truncates file; this writes the current snapshot.
    FILE *file = fopen(FILENAME, "w");
    if(file == NULL) {
        printf("Error: Cannot open file for writing!\n");
        return;
    }

    // Store each employee on a single line using '|' as a field separator.
    // Format must match loadFromFile's fscanf format.
    for(int i = 0; i < count; i++) {
        fprintf(file, "%d|%s|%s|%.2f|%.2f|%.2f|%.2f|%.2f\n",
                employees[i].id,
                employees[i].name,
                employees[i].position,
                employees[i].pay.basicSalary,
                employees[i].pay.allowance,
                employees[i].pay.deductions,
                employees[i].pay.grossPay,
                employees[i].pay.netPay);
    }
    
    fclose(file);
}

void loadFromFile(struct Employee employees[], int *count) {
    // Attempt to open existing file; if missing, start with zero records.
    FILE *file = fopen(FILENAME, "r");
    if(file == NULL) {
        // File doesn't exist, start with empty records
        *count = 0;
        return;
    }
    
    *count = 0;
    // Read lines using the same format used in saveToFile.
    // %49[^|] and %29[^|] ensure we don't overflow name/position buffers.

    while(*count < MAX_EMPLOYEES && 
          fscanf(file, "%d|%49[^|]|%29[^|]|%f|%f|%f|%f|%f\n",
                 &employees[*count].id,
                 employees[*count].name,
                 employees[*count].position,
                 &employees[*count].pay.basicSalary,
                 &employees[*count].pay.allowance,
                 &employees[*count].pay.deductions,
                 &employees[*count].pay.grossPay,
                 &employees[*count].pay.netPay) == 8) {
        (*count)++;
    }
    
    fclose(file);
}

float computeGross(float basic, float allowance) {
    // Simple gross calculation: basic salary + allowance
    return basic + allowance;
}

float computeNet(float gross, float deductions) {
    // Net pay after deductions
    return gross - deductions;
}