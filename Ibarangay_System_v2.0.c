#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

// SYSTEM CONFIGURATION & MACROS
#define C_BLUE 9
#define C_GREEN 10
#define C_CYAN 11
#define C_RED 12
#define C_YELLOW 14
#define C_WHITE 15
#define C_GRAY 8

HANDLE hConsole;

/* 
 * DATA STRUCTURE 1: SINGLY LINKED LIST
 * Purpose: Acts as the main resident database. Grows dynamically without wasting memory.
 * Algorithm: Head Insertion (for adding), Linear Search (for finding), Bubble Sort (for sorting).
 */
struct Resident {
    int id;
    char name[50];
    int age;
    char address[100];
    struct Resident* next; 
};

struct Receipt {
    int resident_id;
    char name[50];
    char category[50];
    float amount;
    char date[20];
    struct Receipt* next;
};

struct QueueNode {
    int resident_id;
    struct QueueNode* next;
};

// --- GLOBAL POINTERS ---
struct Resident* head = NULL;              // Head of the Resident Linked List
struct QueueNode* processFront = NULL;     // Front of the Waiting Queue
struct QueueNode* processRear = NULL;      // Rear of the Waiting Queue
struct Receipt* top = NULL;                // Top of the Receipt History (LIFO Stack)


// 2. UI & ANIMATION ENGINE

void setColor(int colorCode) {
    SetConsoleTextAttribute(hConsole, colorCode);
}

void typeWrite(const char* text, int speed_ms) {
    for (int i = 0; text[i] != '\0'; i++) {
        printf("%c", text[i]);
        Sleep(speed_ms);
    }
}

void showSpinner(const char* message, int duration_ms) {
    char spinner[] = {'|', '/', '-', '\\'};
    int loops = duration_ms / 100;
    
    printf("\n  ");
    setColor(C_CYAN);
    typeWrite(message, 15);
    printf(" [ ");
    
    setColor(C_YELLOW);
    for (int i = 0; i < loops; i++) {
        printf("\b%c", spinner[i % 4]);
        Sleep(100);
    }
    
    setColor(C_GREEN);
    printf("\bOK");
    setColor(C_CYAN);
    printf("]\n");
    setColor(C_WHITE);
    Sleep(300);
}

void printHeader(const char* title) {
    system("cls");
    setColor(C_BLUE);
    printf("\n %c", 201); for(int i=0; i<70; i++) printf("%c", 205); printf("%c\n", 187);
    printf(" %c  ", 186);
    setColor(C_CYAN);
    printf("%-66s", title);
    setColor(C_BLUE);
    printf("  %c\n", 186);
    printf(" %c", 200); for(int i=0; i<70; i++) printf("%c", 205); printf("%c\n\n", 188);
    setColor(C_WHITE);
}
    
void animateBoot() {
    system("cls");
    setColor(C_CYAN);
    printf("\n\n ");
    typeWrite("INITIALIZING IBARANGAY DATABASE SYSTEM...", 30);
    printf("\n");
    
    showSpinner("Mounting Storage Engine", 800);
    showSpinner("Building Index Maps", 600);
    showSpinner("Connecting UI Overlay", 500);
    
    setColor(C_GREEN);
    printf("\n  >> SYSTEM ONLINE. PRESS ANY KEY TO CONTINUE <<");
    _getch();
}


/*
 * ALGORITHM: HEAD INSERTION (Time Complexity: O(1))
 * Instantly attaches a new node to the front of the Linked List.
 */
void executeInsert() {
    printHeader(" ADD NEW RESIDENT");
    
    struct Resident* newNode = (struct Resident*)malloc(sizeof(struct Resident));
    
    printf("  [ INPUT RESIDENT DATA ]\n");
    printf("  > Resident ID : "); scanf("%d", &newNode->id);
    printf("  > Full Name   : "); scanf(" %[^\n]s", newNode->name);
    printf("  > Age         : "); scanf("%d", &newNode->age);
    printf("  > Address     : "); scanf(" %[^\n]s", newNode->address);
    
    showSpinner("Recording to Database", 1000);
    
    newNode->next = head;
    head = newNode;

    setColor(C_GREEN);
    printf("\n  [+] TRANSACTION SUCCESS: Record successfully saved.\n");
    setColor(C_GRAY);
    printf("\n  Press any key to return...");
    _getch();
}

/*
 * ALGORITHM:                        BUBBLE SORT (Time Complexity: O(n^2))
 * Compares adjacent nodes and swaps their internal data if they are out of alphabetical order.
 */
void executeSort() {
    if (head == NULL) return;
    int swapped;
    struct Resident *ptr1, *lptr = NULL;

    do {
        swapped = 0;
        ptr1 = head;
        while (ptr1->next != lptr) {
            if (strcmp(ptr1->name, ptr1->next->name) > 0) {
                // Swap integer data
                int tempId = ptr1->id; ptr1->id = ptr1->next->id; ptr1->next->id = tempId;
                int tempAge = ptr1->age; ptr1->age = ptr1->next->age; ptr1->next->age = tempAge;
                
                // Swap string data
                char tempName[50]; strcpy(tempName, ptr1->name);
                strcpy(ptr1->name, ptr1->next->name); strcpy(ptr1->next->name, tempName);
                
                char tempAddr[100]; strcpy(tempAddr, ptr1->address);
                strcpy(ptr1->address, ptr1->next->address); strcpy(ptr1->next->address, tempAddr);
                
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

/*
 * ALGORITHM: LINKED LIST TRAVERSAL (Time Complexity: O(n))
 * Walks through the list from head to tail to display records.
 */
void viewDatabase() {
    printHeader(" RESIDENT DIRECTORY (INDEXED)");
    showSpinner("Sorting Index by Name", 800);
    
    executeSort(); 
    
    if(head == NULL) {
        setColor(C_RED);
        printf("\n  [!] EXCEPTION: Storage is empty.\n");
    } else {
        setColor(C_CYAN);
        printf("\n  %-10s %-25s %-5s %-25s\n", "ID", "NAME", "AGE", "ADDRESS");
        setColor(C_BLUE);
        for(int i=0; i<70; i++) printf("-"); printf("\n");
        
        setColor(C_WHITE);
        struct Resident* temp = head;
        while (temp != NULL) {
            printf("  %-10d %-25s %-5d %-25s\n", temp->id, temp->name, temp->age, temp->address);
            temp = temp->next;
            Sleep(50);
        }
    }
    setColor(C_GRAY);
    printf("\n  Press any key to return...");
    _getch();
}

/*
 * ALGORITHM:          LINEAR SEARCH (Time Complexity: O(n))
 * Checks every node sequentially until the target ID is found.
 */
void executeQuery() {
    printHeader(" SEARCH DATABASE");
    int searchId;
    printf("  > Enter Resident ID: ");
    scanf("%d", &searchId);
    
    showSpinner("Scanning data", 1200);
    
    struct Resident* temp = head;
    int found = 0;
    
    while(temp != NULL) {
        if(temp->id == searchId) {
            setColor(C_YELLOW);
            printf("\n  %c 1 MATCH FOUND IN STORAGE %c\n", 201, 187);
            printf("  %c ID      : %-22d %c\n", 186, temp->id, 186);
            printf("  %c Name    : %-22s %c\n", 186, temp->name, 186);
            printf("  %c Age     : %-22d %c\n", 186, temp->age, 186);
            printf("  %c Address : %-22s %c\n", 186, temp->address, 186);
            printf("  %c", 200); for(int i=0; i<34; i++) printf("%c", 205); printf("%c\n", 188);
            setColor(C_WHITE);
            found = 1;
            break;
        }
        temp = temp->next;
    }
    
    if(!found) {
        setColor(C_RED);
        printf("\n  [!] QUERY FAILED: 0 Records matched ID %d.\n", searchId);
        setColor(C_WHITE);
    }
    setColor(C_GRAY);
    printf("\n  Press any key to return...");
    _getch();
}

/*
 * ALGORITHM: NODE DELETION / POINTER BYPASS (Time Complexity: O(n))
 * Locates a node, routes the previous pointer to bypass it, and frees the memory.
 */
void executeDelete() {
    printHeader("MODULE: DELETE RECORD");
    int delId;
    printf("  > Enter Resident ID to Terminate: ");
    scanf("%d", &delId);
    
    showSpinner("Locating Record", 1000);
    
    struct Resident *temp = head, *prev = NULL;
    
    if (temp != NULL && temp->id == delId) {
        head = temp->next; 
        free(temp);        
        setColor(C_RED); printf("\n  [-] RECORD PURGED FROM DATABASE.\n");
    } else {
        while (temp != NULL && temp->id != delId) {
            prev = temp;
            temp = temp->next;
        }
        
        if (temp == NULL) {
            setColor(C_YELLOW); printf("\n  [!] RECORD NOT FOUND.\n");
        } else {
            prev->next = temp->next;
            free(temp);
            setColor(C_RED); printf("\n  [-] RECORD PURGED FROM DATABASE.\n");
        }
    }
    setColor(C_GRAY);
    printf("\n  Press any key to return...");
    _getch();
}


//  TRANSACTION ALGORITHMS (QUEUE & STACK LOGIC)


void processTransaction() {
    printHeader(" GENERATE RECEIPT ");
    
    int id;
    printf("  > Insert Resident ID: ");
    scanf("%d", &id);
    
    struct Resident* temp = head;
    int found = 0;
    while(temp != NULL) { if(temp->id == id) { found = 1; break; } temp = temp->next; }
    
    if(!found) {
        setColor(C_RED); printf("\n  [!] DENIED: ID NOT IN DATABASE.\n");
        setColor(C_GRAY); printf("  Press any key to return..."); _getch(); return;
    }
    
    /* 
     * ALGORITHM: ENQUEUE (Waiting Line - FIFO)
     * Pushes the resident to the back of the processing line.
     */
    struct QueueNode* qNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    qNode->resident_id = id; qNode->next = NULL;
    if (processRear == NULL) { 
        processFront = processRear = qNode; 
    } else { 
        processRear->next = qNode; 
        processRear = qNode; 
    }
    
    /* 
     * ALGORITHM: DEQUEUE (Waiting Line - FIFO)
     * Immediately pulls the resident from the front of the line to process them.
     */
    struct QueueNode* processNode = processFront;
    processFront = processFront->next;
    if (processFront == NULL) processRear = NULL;
    int processedId = processNode->resident_id;
    free(processNode); 
    
    printf("\n  [ PAYMENT DETAILS ]\n");
    char cat[50], date[20]; float amt;
    printf("  > Document Type : "); scanf(" %[^\n]s", cat);
    printf("  > Amount (PHP)  : "); scanf("%f", &amt);
    printf("  > Date (MM/DD/YY): "); scanf(" %[^\n]s", date);
    
    showSpinner("Authorizing Transaction", 1500);
    showSpinner("Generating Secure Hash", 800);
    
    /* 
     * ALGORITHM: PUSH (Receipt History - LIFO STACK)
     * Pushes the generated receipt onto the TOP of the history stack.
     * This guarantees the Last transaction processed is the First one viewed.
     */
    struct Receipt* newReceipt = (struct Receipt*)malloc(sizeof(struct Receipt));
    newReceipt->resident_id = processedId;
    strcpy(newReceipt->name, temp->name); strcpy(newReceipt->category, cat);
    newReceipt->amount = amt; strcpy(newReceipt->date, date);
    
    // Point the new receipt's next to the current top, then make it the new top
    newReceipt->next = top;
    top = newReceipt;
    
    system("cls");
    setColor(C_GREEN); printf("\n  >> PRINTING OFFICIAL RECEIPT...\n\n"); Sleep(800);
    
    setColor(C_WHITE);
    printf("       .-------------------------------------------.\n");
    printf("       |   IBARANGAY OFFICIAL TRANSACTION SLIP     |\n");
    printf("       |-------------------------------------------|\n");
    printf("       | ID: %-13d   | DATE: %-13s |\n", processedId, date);
    printf("       | NAME: %-35s |\n", temp->name);
    printf("       | TYPE: %-35s |\n", cat);
    printf("       |-------------------------------------------|\n");
    setColor(C_YELLOW);
    printf("       | TOTAL AMOUNT:            PHP %-10.2f   |\n", amt);
    setColor(C_WHITE);
    printf("       | STATUS: [PAID]                            |\n");
    printf("       '-------------------------------------------'\n\n");
    
    setColor(C_GRAY); printf("  Press any key to close receipt..."); 
    _getch();
}

/*
 * MODULE: VIEW RECEIPTS (LIFO PRINCIPLE)
 * ALGORITHM: Stack Traversal (Time Complexity: O(n))
 * Traverses the Receipt Stack starting from Top (Newest) down to Bottom (Oldest).
 */
void viewReceipts() {
    printHeader(" TRANSACTION HISTORY ");
    showSpinner("Fetching Records", 800);
    
    if (top == NULL) {
        setColor(C_RED);
        printf("\n  [!] EXCEPTION: No transactions recorded yet.\n");
    } else {
        setColor(C_CYAN);
        // Header for the receipt table
        printf("\n  %-10s %-20s %-15s %-10s %-10s\n", "RES ID", "NAME", "DOC TYPE", "AMOUNT", "DATE");
        setColor(C_BLUE);
        for(int i=0; i<70; i++) printf("-"); printf("\n");
        
        setColor(C_WHITE);
        struct Receipt* temp = top; // Start traversing from the TOP of the stack
        
        // Loop from the Top (last created) to the Bottom (first created)
        while (temp != NULL) {
            printf("  %-10d %-20s %-15s PHP %-6.2f %-10s\n", 
                   temp->resident_id, temp->name, temp->category, temp->amount, temp->date);
            temp = temp->next;
            Sleep(50); // Small animation effect
        }
        
        setColor(C_YELLOW);
        printf("\n  [INFO] Records are displayed (Newest to Oldest).\n");
    }
    
    setColor(C_GRAY);
    printf("\n  Press any key to return...");
    _getch();
}

// MAIN LOOP

void drawDashboard() {
    system("cls");
    setColor(C_BLUE);
    printf("\n %c", 201); for(int i=0; i<70; i++) printf("%c", 205); printf("%c\n", 187);
    printf(" %c", 186); setColor(C_GREEN); printf("  [ SYSTEM: ONLINE ]"); setColor(C_CYAN); printf("   BARANGAY RECEIPT AND RECORDING SYSTEM          "); setColor(C_BLUE); printf("%c\n", 186);
    printf(" %c", 204); for(int i=0; i<70; i++) printf("%c", 205); printf("%c\n", 185);
    
    setColor(C_WHITE);
    printf(" %c  COMMANDS:                                                           %c\n", 186, 186);
    printf(" %c  > [1] Add New Resident             > [4] Delete Record              %c\n", 186, 186);
    printf(" %c  > [2] View Resident                > [5] Process Transaction        %c\n", 186, 186);
    printf(" %c  > [3] Search Resident              > [6] View Receipts              %c\n", 186, 186);
    printf(" %c                                     > [0] EXIT                       %c\n", 186, 186);
    
    setColor(C_BLUE);
    printf(" %c", 200); for(int i=0; i<70; i++) printf("%c", 205); printf("%c\n", 188);
    setColor(C_YELLOW);
    printf("\n  Selection >> ");
    setColor(C_WHITE);
}

int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    animateBoot();
    
    int choice = -1;
    while(choice != 0) {
        drawDashboard();
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); 
            choice = -1; 
        }
        
        switch(choice) {
            case 1: executeInsert(); break;
            case 2: viewDatabase(); break;
            case 3: executeQuery(); break;
            case 4: executeDelete(); break;
            case 5: processTransaction(); break;
            case 6: viewReceipts(); break;
            case 0: 
                printHeader("SYSTEM SHUTDOWN");
                showSpinner("Freeing Dynamic Memory", 1000);
                showSpinner("Closing Connections", 500);
                setColor(C_RED);
                printf("\n  [ SYSTEM OFFLINE ] Goodbye.\n");
                Sleep(1000);
                break;
            default:
                setColor(C_RED);
                printf("  [!] INVALID SYNTAX. Try again.\n");
                Sleep(800);
        }
    }
    return 0;
}
