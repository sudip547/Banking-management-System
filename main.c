#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Function declarations
void createAccount(void);
void login(void);
void displayMenu(char* username);
void checkBalance(char* username);
void transferMoney(char* username);
void logout(void);

// Structure to store user data
struct User {
    char username[50];
    int date, month, year;
    char phoneNumber[15];
    char aadharNumber[20];
    char firstName[20];
    char lastName[20];
    char fatherName[20];
    char motherName[20];
    char address[50];
    char accountType[20];
};

// Structure to track money transfers
struct MoneyTransfer {
    char fromUsername[50];
    char toUsername[50];
    long int amount;
};

// Function to set cursor position
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Main function
int main() {
    int choice;

    gotoxy(20, 3);
    printf("WELCOME TO BANK ACCOUNT SYSTEM\n\n");
    gotoxy(18, 5);
    printf("**********************************");
    gotoxy(25, 7);
    printf("DEVELOPER: Sudip Das");
    gotoxy(20, 10);
    printf("1. CREATE A BANK ACCOUNT");
    gotoxy(20, 12);
    printf("2. ALREADY A USER? SIGN IN");
    gotoxy(20, 14);
    printf("3. EXIT\n\n");

    printf("ENTER YOUR CHOICE: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            system("cls");
            printf("\n\nUSERNAME: Max 50 characters\nPASSWORD: Max 50 characters\n");
            createAccount();
            break;
        case 2:
            login();
            break;
        case 3:
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
            break;
    }
    return 0;
}

// Function to create a new bank account
void createAccount() {
    FILE *file;
    struct User user;
    char password[50];
    int i;
    char ch;

    file = fopen("users.dat", "ab");
    if (file == NULL) {
        printf("Error opening file.");
        return;
    }

    system("cls");
    printf("CREATE ACCOUNT\n\n");
    printf("First Name: ");
    scanf("%s", user.firstName);
    printf("Last Name: ");
    scanf("%s", user.lastName);
    printf("Father's Name: ");
    scanf("%s", user.fatherName);
    printf("Mother's Name: ");
    scanf("%s", user.motherName);
    printf("Address: ");
    scanf("%s", user.address);
    printf("Account Type: ");
    scanf("%s", user.accountType);
    printf("Date of Birth (DD MM YYYY): ");
    scanf("%d %d %d", &user.date, &user.month, &user.year);
    printf("Aadhar Number: ");
    scanf("%s", user.aadharNumber);
    printf("Phone Number: ");
    scanf("%s", user.phoneNumber);
    printf("Username: ");
    scanf("%s", user.username);

    printf("Password: ");
    for (i = 0; i < 50; i++) {
        ch = getch();
        if (ch == 13) break;
        password[i] = ch;
        printf("*");
    }
    password[i] = '\0';

    fwrite(&user, sizeof(user), 1, file);
    fclose(file);

    printf("\n\nAccount created successfully! Press Enter to log in.");
    getch();
    login();
}

// Function to handle user login
void login() {
    FILE *file;
    struct User user;
    char username[50], password[50];
    int i;
    char ch;

    system("cls");
    file = fopen("users.dat", "rb");
    if (file == NULL) {
        printf("Error opening file.");
        return;
    }

    printf("ACCOUNT LOGIN\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    for (i = 0; i < 50; i++) {
        ch = getch();
        if (ch == 13) break;
        password[i] = ch;
        printf("*");
    }
    password[i] = '\0';

    while (fread(&user, sizeof(user), 1, file)) {
        if (strcmp(username, user.username) == 0) {
            fclose(file);
            printf("\n\nLogin successful! Press Enter to continue.");
            getch();
            displayMenu(username);
            return;
        }
    }

    fclose(file);
    printf("\n\nLogin failed! Invalid username or password.");
    getch();
}

// Function to display the user menu after login
void displayMenu(char* username) {
    int choice;

    do {
        system("cls");
        printf("WELCOME, %s\n\n", username);
        printf("1. CHECK BALANCE\n");
        printf("2. TRANSFER MONEY\n");
        printf("3. LOG OUT\n");
        printf("4. EXIT\n\n");
        printf("ENTER YOUR CHOICE: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                checkBalance(username);
                break;
            case 2:
                transferMoney(username);
                break;
            case 3:
                logout();
                login();
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice. Please try again.");
                break;
        }
    } while (choice != 3 && choice != 4);
}

// Function to check the balance in the user's account
void checkBalance(char* username) {
    FILE *file;
    struct MoneyTransfer transfer;
    int balance = 0;
    int transactionCount = 0;

    system("cls");
    file = fopen("transfers.dat", "rb");
    if (file == NULL) {
        printf("Error opening file.");
        return;
    }

    printf("BALANCE DASHBOARD\n\n");
    printf("S.No.  |  From User  |  Amount\n");
    printf("-----------------------------------\n");

    while (fread(&transfer, sizeof(transfer), 1, file)) {
        if (strcmp(transfer.toUsername, username) == 0) {
            transactionCount++;
            printf("%-6d | %-10s | %-8ld\n", transactionCount, transfer.fromUsername, transfer.amount);
            balance += transfer.amount;
        }
    }

    printf("\nTotal Balance: %d\n", balance);
    fclose(file);
    getch();
}

// Function to transfer money from one user to another
void transferMoney(char* username) {
    FILE *file, *userFile;
    struct MoneyTransfer transfer;
    struct User user;
    char toUsername[50];
    int found = 0;

    system("cls");
    userFile = fopen("users.dat", "rb");
    file = fopen("transfers.dat", "ab");

    if (userFile == NULL || file == NULL) {
        printf("Error opening file.");
        return;
    }

    printf("TRANSFER MONEY\n\n");
    printf("From (your username): %s\n", username);
    printf("To (recipient username): ");
    scanf("%s", toUsername);

    while (fread(&user, sizeof(user), 1, userFile)) {
        if (strcmp(toUsername, user.username) == 0) {
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Username not found! Transfer failed.");
        fclose(file);
        fclose(userFile);
        getch();
        return;
    }

    strcpy(transfer.fromUsername, username);
    strcpy(transfer.toUsername, toUsername);

    printf("Enter the amount to transfer: ");
    scanf("%ld", &transfer.amount);

    fwrite(&transfer, sizeof(transfer), 1, file);
    printf("\nAmount successfully transferred!");
    fclose(file);
    fclose(userFile);
    getch();
    displayMenu(username);
}

// Function to handle user logout
void logout() {
    system("cls");
    printf("Logging out...\n");
    Sleep(2000);
    printf("Logged out successfully! Press any key to continue.");
    getch();
}
