#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#define FILE_NAME "students.dat"
#define TEMP_FILE "temp.dat"

// Color codes for Windows console
#define COLOR_GREEN 10
#define COLOR_RED 12
#define COLOR_YELLOW 14
#define COLOR_WHITE 15
#define COLOR_CYAN 11

struct Student {
    int id;
    char name[50];
    char dept[30];
    char phone[15];
    float cgpa;
    int age;
    char email[50];
};

// Function prototypes
void setColor(int color);
void printHeader();
void printFooter();
void printMenu();
void addStudent();
void modifyStudent();
void showAllStudents();
void individualView();
void removeStudent();
void logout();
int getNextId();
void printStudent(struct Student s);
int studentExists(int id);
void pauseScreen();

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void printHeader() {
    system("cls");
    setColor(COLOR_CYAN);
    printf("\n\t================== Student Management System ==================\n");
    setColor(COLOR_YELLOW);
    printf("\t***************************************************************\n");
    setColor(COLOR_WHITE);
}

void printFooter() {
    setColor(COLOR_YELLOW);
    printf("\t***************************************************************\n\n");
    setColor(COLOR_WHITE);
}

void printMenu() {
    printHeader();
    printf("\n");
    setColor(COLOR_GREEN);
    printf("\t\t\t1. Add Student\n\n");
    printf("\t\t\t2. Modify Student\n\n");
    printf("\t\t\t3. Show All Student\n\n");
    printf("\t\t\t4. Individual View\n\n");
    printf("\t\t\t5. Remove Student\n\n");
    printf("\t\t\t6. Logout\n\n");
    printFooter();
    printf("\t\t\tEnter Your Option :--> ");
}

void pauseScreen() {
    printf("\n\t\t\tPress any key to continue...");
    getch();
}

void addStudent() {
    FILE *fp;
    struct Student s;
    
    printHeader();
    setColor(COLOR_GREEN);
    printf("\n\t\t\t\tADD NEW STUDENT\n\n");
    setColor(COLOR_WHITE);
    
    fp = fopen(FILE_NAME, "ab");
    if (fp == NULL) {
        setColor(COLOR_RED);
        printf("\n\t\t\tError opening file!\n");
        pauseScreen();
        return;
    }
    
    s.id = getNextId();
    
    printf("\t\t\tAuto-generated ID: %d\n", s.id);
    printf("\t\t\tEnter Name        : ");
    getchar();                 
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = 0;
    
    printf("\t\t\tEnter Department  : ");
    fgets(s.dept, sizeof(s.dept), stdin);
    s.dept[strcspn(s.dept, "\n")] = 0;
    
    printf("\t\t\tEnter Phone       : ");
    fgets(s.phone, sizeof(s.phone), stdin);
    s.phone[strcspn(s.phone, "\n")] = 0;
    
    printf("\t\t\tEnter Email       : ");
    fgets(s.email, sizeof(s.email), stdin);
    s.email[strcspn(s.email, "\n")] = 0;
    
    printf("\t\t\tEnter Age         : ");
    scanf("%d", &s.age);
    
    printf("\t\t\tEnter CGPA        : ");
    scanf("%f", &s.cgpa);
    
    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);
    
    setColor(COLOR_GREEN);
    printf("\n\t\t\tStudent added successfully! (ID: %d)\n", s.id);
    setColor(COLOR_WHITE);
    pauseScreen();
}

void modifyStudent() {
    FILE *fp;
    struct Student s;
    int id, found = 0;
    
    printHeader();
    setColor(COLOR_YELLOW);
    printf("\n\t\t\t\tMODIFY STUDENT\n\n");
    setColor(COLOR_WHITE);
    
    printf("\t\t\tEnter Student ID to Modify: ");
    scanf("%d", &id);
    
    if (!studentExists(id)) {
        setColor(COLOR_RED);
        printf("\n\t\t\tStudent with ID %d not found!\n", id);
        setColor(COLOR_WHITE);
        pauseScreen();
        return;
    }
    
    fp = fopen(FILE_NAME, "rb+");
    if (fp == NULL) {
        setColor(COLOR_RED);
        printf("\n\t\t\tError opening file!\n");
        pauseScreen();
        return;
    }
    
    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.id == id) {
            found = 1;
            printf("\n\t\t\tCurrent Name: %s\n", s.name);
            printf("\t\t\tEnter New Name        : ");
            fflush(stdin);
            fgets(s.name, sizeof(s.name), stdin);
            s.name[strcspn(s.name, "\n")] = 0;
            
            printf("\t\t\tEnter New Department  : ");
            fgets(s.dept, sizeof(s.dept), stdin);
            s.dept[strcspn(s.dept, "\n")] = 0;
            
            printf("\t\t\tEnter New Phone       : ");
            fgets(s.phone, sizeof(s.phone), stdin);
            s.phone[strcspn(s.phone, "\n")] = 0;
            
            printf("\t\t\tEnter New Email       : ");
            fgets(s.email, sizeof(s.email), stdin);
            s.email[strcspn(s.email, "\n")] = 0;
            
            printf("\t\t\tEnter New Age         : ");
            scanf("%d", &s.age);
            
            printf("\t\t\tEnter New CGPA        : ");
            scanf("%f", &s.cgpa);
            
            fseek(fp, -sizeof(s), SEEK_CUR);
            fwrite(&s, sizeof(s), 1, fp);
            
            setColor(COLOR_GREEN);
            printf("\n\t\t\tStudent modified successfully!\n");
            setColor(COLOR_WHITE);
            break;
        }
    }
    
    fclose(fp);
    pauseScreen();
}

void showAllStudents() {
    FILE *fp;
    struct Student s;
    int count = 0;
    
    printHeader();
    setColor(COLOR_CYAN);
    printf("\n\t\t\t\tALL STUDENTS RECORD\n\n");
    setColor(COLOR_WHITE);
    
    fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        setColor(COLOR_RED);
        printf("\n\t\t\tNo records found!\n");
        setColor(COLOR_WHITE);
        pauseScreen();
        return;
    }
    
    printf("\t+------+----------------------+------------+----------+-----+-------+\n");
    printf("\t|  ID  |         Name         | Department |   Phone  | Age |  CGPA |\n");
    printf("\t+------+----------------------+------------+----------+-----+-------+\n");
    
    while (fread(&s, sizeof(s), 1, fp)) {
        printf("\t| %-4d | %-20s | %-10s | %-8s | %-3d | %-5.2f |\n",
               s.id, s.name, s.dept, s.phone, s.age, s.cgpa);
        count++;
    }
    
    printf("\t+------+----------------------+------------+----------+-----+-------+\n");
    setColor(COLOR_YELLOW);
    printf("\n\t\t\tTotal Students: %d\n", count);
    setColor(COLOR_WHITE);
    
    fclose(fp);
    pauseScreen();
}

void individualView() {
    FILE *fp;
    struct Student s;
    int id, found = 0;
    
    printHeader();
    setColor(COLOR_CYAN);
    printf("\n\t\t\t\tINDIVIDUAL STUDENT VIEW\n\n");
    setColor(COLOR_WHITE);
    
    printf("\t\t\tEnter Student ID: ");
    scanf("%d", &id);
    
    fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) {
        setColor(COLOR_RED);
        printf("\n\t\t\tNo records found!\n");
        pauseScreen();
        return;
    }
    
    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.id == id) {
            found = 1;
            printf("\n");
            printStudent(s);
            break;
        }
    }
    
    if (!found) {
        setColor(COLOR_RED);
        printf("\n\t\t\tStudent with ID %d not found!\n", id);
        setColor(COLOR_WHITE);
    }
    
    fclose(fp);
    pauseScreen();
}

void removeStudent() {
    FILE *fp, *temp;
    struct Student s;
    int id, found = 0;
    
    printHeader();
    setColor(COLOR_RED);
    printf("\n\t\t\t\tREMOVE STUDENT\n\n");
    setColor(COLOR_WHITE);
    
    printf("\t\t\tEnter Student ID to Remove: ");
    scanf("%d", &id);
    
    if (!studentExists(id)) {
        setColor(COLOR_RED);
        printf("\n\t\t\tStudent with ID %d not found!\n", id);
        setColor(COLOR_WHITE);
        pauseScreen();
        return;
    }
    
    fp = fopen(FILE_NAME, "rb");
    temp = fopen(TEMP_FILE, "wb");
    
    if (fp == NULL || temp == NULL) {
        setColor(COLOR_RED);
        printf("\n\t\t\tError opening file!\n");
        pauseScreen();
        return;
    }
    
    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.id != id) {
            fwrite(&s, sizeof(s), 1, temp);
        } else {
            found = 1;
        }
    }
    
    fclose(fp);
    fclose(temp);
    
    remove(FILE_NAME);
    rename(TEMP_FILE, FILE_NAME);
    
    if (found) {
        setColor(COLOR_GREEN);
        printf("\n\t\t\tStudent removed successfully!\n");
    }
    setColor(COLOR_WHITE);
    pauseScreen();
}

void logout() {
    printHeader();
    setColor(COLOR_GREEN);
    printf("\n\n\t\t\tThank you for using Student Management System!\n");
    printf("\t\t\t\tDeveloped by: CHANDRA NATH\n\n");
    setColor(COLOR_WHITE);
    Sleep(1500);
    exit(0);
}

int getNextId() {
    FILE *fp;
    struct Student s;
    int maxId = 1000;
    
    fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) return 1001;
    
    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.id > maxId) maxId = s.id;
    }
    fclose(fp);
    return maxId + 1;
}

int studentExists(int id) {
    FILE *fp;
    struct Student s;
    
    fp = fopen(FILE_NAME, "rb");
    if (fp == NULL) return 0;
    
    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.id == id) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void printStudent(struct Student s) {
    printf("\t+------------------------------------------+\n");
    printf("\t|            STUDENT DETAILS               |\n");
    printf("\t+------------------------------------------+\n");
    printf("\t| Student ID    : %-24d |\n", s.id);
    printf("\t| Name          : %-24s |\n", s.name);
    printf("\t| Department    : %-24s |\n", s.dept);
    printf("\t| Phone         : %-24s |\n", s.phone);
    printf("\t| Email         : %-24s |\n", s.email);
    printf("\t| Age           : %-24d |\n", s.age);
    printf("\t| CGPA          : %-24.2f |\n", s.cgpa);
    printf("\t+------------------------------------------+\n");
}

int main() {
    int choice;
    
    // Set console title
    SetConsoleTitle("Student Management System | CHANDRA NATH");
    
    while (1) {
        printMenu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: addStudent(); break;
            case 2: modifyStudent(); break;
            case 3: showAllStudents(); break;
            case 4: individualView(); break;
            case 5: removeStudent(); break;
            case 6: logout(); break;
            default:
                setColor(COLOR_RED);
                printf("\n\t\t\tInvalid option! Please try again.\n");
                setColor(COLOR_WHITE);
                Sleep(1000);
        }
    }
    
    return 0;
}