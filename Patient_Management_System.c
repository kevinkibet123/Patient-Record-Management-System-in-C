#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for a patient record
typedef struct Patient {
    int id;
    char name[50];
    char *medical_history;
    struct Patient *next;
} Patient;

// Function to create a new patient
Patient* createPatient(int id, const char *name, const char *history) {
    Patient *newPatient = (Patient*)malloc(sizeof(Patient));
    if (!newPatient) { //Here, we test if the allocation passed, same as newPatient==NULL
        printf("Memory allocation failed!\n");
        return NULL;
    }
    newPatient->id = id; //If it were a non-pointer structure, we would use,newPatient.id
    strncpy(newPatient->name, name, 49);
    newPatient->name[49] = '\0';
    
    if (history && strlen(history) > 0) {
        newPatient->medical_history = (char*)malloc(strlen(history) + 1);
        if (!newPatient->medical_history) {
            free(newPatient);
            printf("Memory allocation for medical history failed!\n");
            return NULL;
        }
        strcpy(newPatient->medical_history, history);
    } else {
        newPatient->medical_history = NULL;
    }
    
    newPatient->next = NULL;
    return newPatient;
}

// Function to add a patient to the system
void addPatient(Patient **head, int id, const char *name, const char *history) {
    Patient *newPatient = createPatient(id, name, history);
    if (!newPatient) return;
    
    newPatient->next = *head;
    *head = newPatient;
    printf("Patient %s (ID: %d) added successfully!\n", name, id);
}

// Function to update a patient's medical history
void updateMedicalHistory(Patient *head, int id, const char *history) {
    Patient *current = head;
    while (current && current->id != id) {
        current = current->next;
    }
    if (!current) {
        printf("Patient not found!\n");
        return;
    }
    
    char *newHistory = (char*)realloc(current->medical_history, strlen(history) + 1);
    if (!newHistory) {
        printf("Memory reallocation failed!\n");
        return;
    }
    current->medical_history = newHistory;
    strcpy(current->medical_history, history);
    printf("Medical history for patient ID %d updated!\n", id);
}

// Function to remove a patient from the system
void removePatient(Patient **head, int id) {
    Patient *current = *head, *prev = NULL;
    while (current && current->id != id) {
        prev = current;
        current = current->next;
    }
    if (!current) {
        printf("Patient not found!\n");
        return;
    }
    
    if (prev) {
        prev->next = current->next;
    } else {
        *head = current->next;
    }
    
    free(current->medical_history);
    free(current);
    printf("Patient ID %d removed successfully!\n", id);
}

// Function to display all patients
void displayPatients(Patient *head) {
    if (!head) {
        printf("No patients found!\n");
        return;
    }
    
    Patient *current = head;
    while (current) {
        printf("ID: %d, Name: %s, Medical History: %s\n", current->id, current->name, current->medical_history ? current->medical_history : "N/A");// Here, we use the tenary operator ?, which evaluates>> condition ? value if true: value if false
        current = current->next;
    }
}

// Function to free allocated memory
void freePatients(Patient *head) {
    Patient *current;
    while (head) {
        current = head;
        head = head->next;
        free(current->medical_history);
        free(current);
    }
}

// Main function
int main() {
    Patient *head = NULL;
    int choice, id;
    char name[50], history[200];
    
    do {
        printf("\n1. Add Patient\n2. Update Medical History\n3. Remove Patient\n4. Display Patients\n5. Exit\nEnter choice: ");
        scanf("%d", &choice);
        getchar(); // Clear input buffer
        
        switch (choice) {
            case 1: 
                printf("Enter Patient ID: ");
                scanf("%d", &id);
                getchar();
                printf("Enter Name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                printf("Enter Medical History: ");
                fgets(history, sizeof(history), stdin);
                history[strcspn(history, "\n")] = 0;
                addPatient(&head, id, name, history);
                break;
            case 2:
                printf("Enter Patient ID: ");
                scanf("%d", &id);
                getchar();
                printf("Enter New Medical History: ");
                fgets(history, sizeof(history), stdin);
                history[strcspn(history, "\n")] = 0;
                updateMedicalHistory(head, id, history);
                break;
            case 3:
                printf("Enter Patient ID to remove: ");
                scanf("%d", &id);
                removePatient(&head, id);
                break;
            case 4:
                displayPatients(head);
                break;
            case 5:
                freePatients(head);
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 5);
    
    return 0;
}
