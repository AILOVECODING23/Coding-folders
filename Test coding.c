#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ACCOUNTS 100

// ATM account structure
typedef struct {
    char name[100];
    int pin;
    float balance;
} ATM;

// Generate Current Date and Time
void current_datetime(char *buffer, size_t len) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, len, "%Y-%m-%d %H:%M:%S", tm_info);
}

// Generate Unique Transaction ID
void generate_transaction_id(char *buffer, size_t len) {
    int id = rand() % 1000000; // Random 6-digit number
    snprintf(buffer, len, "TXN-%06d", id);
}

// Print receipt to console the file

void print_receipt(int pin, const char *type, float amount, float prev_balance, float new_balance) {
    char datetime[50], txn_id[20], filename[40];
    current_datetime(datetime, sizeof(datetime));
    generate_transaction_id(txn_id, sizeof(txn_id));
    
    
// File name per account
    snprintf(filename, sizeof(filename), "receipt_%d.txt", pin);
    

// to display on screen
    printf("\n============ ATM RECEIPT ============\n");
    printf("Date/Time       : %s\n", datetime);
    printf("Transaction ID  : %s\n", txn_id);
    printf("Transaction Type: %s\n", type);
    printf("Amount          : ₱%.2f\n", amount);
    printf("Previous Balance: ₱%.2f\n", prev_balance);
    printf("New Balance     : ₱%.2f\n", new_balance);
    printf("=====================================\n");

// Save to file
    FILE *f = fopen(filename, "a");
    if (f != NULL) {
        fprintf(f, "\n============ ATM RECEIPT ============\n");
        fprintf(f, "Date/Time       : %s\n", datetime);
        fprintf(f, "Transaction ID  : %s\n", txn_id);
        fprintf(f, "Transaction Type: %s\n", type);
        fprintf(f, "Amount          : ₱%.2f\n", amount);
        fprintf(f, "Previous Balance: ₱%.2f\n", prev_balance);
        fprintf(f, "New Balance     : ₱%.2f\n", new_balance);
        fprintf(f, "=====================================\n");
        fclose(f);
    }
}


// for File Handling
void saveAccounts(ATM accounts[], int count) {
    FILE *f = fopen("accounts.txt", "w");
    if (!f) return;

    for (int i = 0; i < count; i++) {
        fprintf(f, "%s %d %.2f\n", accounts[i].name, accounts[i].pin, accounts[i].balance);
    }
    fclose(f);
}


// 
void loadAccounts(ATM accounts[], int *count) {
    FILE *f = fopen("accounts.txt", "r");
    if (!f) {
        *count = 0;
        return;
    }

    *count = 0;
    while (fscanf(f, "%s %d %f",
                  accounts[*count].name,
                  &accounts[*count].pin,
                  &accounts[*count].balance) != EOF) {
        (*count)++;
    }
    fclose(f);
}


// To Create Account
void createAccount(ATM accounts[], int *count) {
    printf("\nEnter Full Name: ");
    getchar();
    fgets(accounts[*count].name, 100, stdin);
    accounts[*count].name[strcspn(accounts[*count].name, "\n")] = '\0';

    printf("Set 4-digit PIN: ");
    scanf("%d", &accounts[*count].pin);

    printf("Initial Deposit: ₱");
    scanf("%f", &accounts[*count].balance);

    printf("\nAccount created successfully!\n");

    (*count)++;
    saveAccounts(accounts, *count);
}


// ATM menu loop
void ATM_loop(ATM *acct) {
    int choice;
    float amount, prev;

    while (1) {
        printf("\n===== ATM MENU =====\n");
        printf("WELCOME, %s!\n", acct->name);
        printf("1. Check Balance\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Change PIN\n");
        printf("5. View Past Receipts\n");
        printf("6. Logout\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                prev = acct->balance;
                print_receipt(acct->pin, "Balance Inquiry", 0.0f, prev, acct->balance);
                break;

            case 2:
                printf("Enter amount to deposit: ₱");
                scanf("%f", &amount);
                prev = acct->balance;
                acct->balance += amount;
                print_receipt(acct->pin, "Deposit", amount, prev, acct->balance);
                break;

            case 3:
                printf("Enter amount to withdraw: ₱");
                scanf("%f", &amount);
                if (amount > acct->balance) {
                    printf("Insufficient balance!\n");
                } else {
                    prev = acct->balance;
                    acct->balance -= amount;
                    print_receipt(acct->pin, "Withdrawal", amount, prev, acct->balance);
                }
                break;

            case 4: {
                int newPin;
                printf("Enter new 4-digit PIN: ");
                scanf("%d", &newPin);
                acct->pin = newPin;
                printf("PIN changed successfully!\n");
                break;
            }

            case 5: {
                char filename[40];
                snprintf(filename, sizeof(filename), "receipt_%d.txt", acct->pin);
                FILE *f = fopen(filename, "r");
                if (!f) {
                    printf("No receipts found.\n");
                } else {
                    char line[256];
                    printf("\n--- Past Receipts ---\n");
                    while (fgets(line, sizeof(line), f))
                        printf("%s", line);
                    fclose(f);
                }
                break;
            }

            case 6:
                return;

            default:
                printf("Invalid option.\n");
        }
    }
}


// Main program for the accounts

// for log ins/Create account to work properly

int main() {
    srand(time(NULL)); // for main accounts to be created

    ATM accounts[MAX_ACCOUNTS];
    int count = 0;

    loadAccounts(accounts, &count);

    int choice;

    while (1) {
        printf("\n===== ATM BANKING SYSTEM =====\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            createAccount(accounts, &count);
            saveAccounts(accounts, count);
        }
        else if (choice == 2) {
            int pin, found = -1;
            printf("Enter PIN: ");
            scanf("%d", &pin);

            for (int i = 0; i < count; i++) {
                if (accounts[i].pin == pin) {
                    found = i;
                    break;
                }
            }
            if (found == -1)
                printf("Invalid PIN.\n");
            else
                ATM_loop(&accounts[found]);
        }
        else if (choice == 3) {
            printf("Thank you for using the ATM system!\n");
            saveAccounts(accounts, count);
            break;
        }
        else {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}