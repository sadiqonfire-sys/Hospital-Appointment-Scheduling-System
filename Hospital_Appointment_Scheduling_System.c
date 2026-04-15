#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------- PATIENT ----------------
struct Patient
{
    char id[20];
    char name[50];
    int priority;

    int doctorId;
    char doctorName[50];

    struct Patient *next;
};

struct Patient *front = NULL;
struct Patient *rear = NULL;
struct Patient *emergencyHead = NULL;

int patientCounter = 1100;
int emergencyCounter = 100;

// ---------------- DOCTOR ----------------
struct Doctor
{
    int id;
    char name[50];
    char department[50];
    int available;

    struct Doctor *left;
    struct Doctor *right;
};

// ---------------- DOCTOR FUNCTIONS ----------------
struct Doctor *createDoctor(int id, char name[], char dept[], int available)
{
    struct Doctor *n = (struct Doctor *)malloc(sizeof(struct Doctor));

    n->id = id;
    strcpy(n->name, name);
    strcpy(n->department, dept);
    n->available = available;

    n->left = n->right = NULL;
    return n;
}

struct Doctor *insertDoctor(struct Doctor *root, int id, char name[], char dept[], int available)
{
    if (!root)
        return createDoctor(id, name, dept, available);

    if (id < root->id)
        root->left = insertDoctor(root->left, id, name, dept, available);
    else
        root->right = insertDoctor(root->right, id, name, dept, available);

    return root;
}

struct Doctor *searchDoctor(struct Doctor *root, int id)
{
    if (!root || root->id == id)
        return root;

    if (id < root->id)
        return searchDoctor(root->left, id);

    return searchDoctor(root->right, id);
}

void displayDoctors(struct Doctor *root)
{
    if (!root)
        return;

    displayDoctors(root->left);

    printf("ID: %d | Name: %s | Dept: %s | %s\n",
           root->id,
           root->name,
           root->department,
           root->available ? "Available" : "On Leave");

    displayDoctors(root->right);
}

void displayDoctorsByDepartment(struct Doctor *root, char dept[])
{
    if (!root)
        return;

    displayDoctorsByDepartment(root->left, dept);

    if (strcmp(root->department, dept) == 0)
    {
        printf("ID: %d | Name: %s | %s\n",
               root->id,
               root->name,
               root->available ? "Available" : "On Leave");
    }

    displayDoctorsByDepartment(root->right, dept);
}

void showAvailableDoctors(struct Doctor *root, char dept[])
{
    if (!root)
        return;

    showAvailableDoctors(root->left, dept);

    if (strcmp(root->department, dept) == 0 && root->available == 1)
    {
        printf("ID: %d | Name: %s\n", root->id, root->name);
    }

    showAvailableDoctors(root->right, dept);
}

// ---------------- NORMAL QUEUE (FIFO) ----------------
void enqueue(char id[], char name[], int did, char dname[])
{
    struct Patient *n = (struct Patient *)malloc(sizeof(struct Patient));

    strcpy(n->id, id);
    strcpy(n->name, name);
    n->priority = 0;

    n->doctorId = did;
    strcpy(n->doctorName, dname);
    n->next = NULL;

    if (!rear)
        front = rear = n;
    else
    {
        rear->next = n;
        rear = n;
    }
}

struct Patient *dequeue()
{
    if (!front)
        return NULL;

    struct Patient *t = front;
    front = front->next;

    if (!front)
        rear = NULL;

    return t;
}

// ---------------- EMERGENCY QUEUE ----------------
void insertEmergency(char id[], char name[], int priority)
{
    struct Patient *n = (struct Patient *)malloc(sizeof(struct Patient));

    strcpy(n->id, id);
    strcpy(n->name, name);
    n->priority = priority;

    n->doctorId = -1;
    strcpy(n->doctorName, "Emergency");
    n->next = NULL;

    if (!emergencyHead)
    {
        emergencyHead = n;
        return;
    }

    if (priority < emergencyHead->priority)
    {
        n->next = emergencyHead;
        emergencyHead = n;
        return;
    }

    struct Patient *temp = emergencyHead;
    struct Patient *prev = NULL;

    while (temp && temp->priority < priority)
    {
        prev = temp;
        temp = temp->next;
    }

    if (prev == NULL)
    {
        n->next = emergencyHead;
        emergencyHead = n;
    }
    else
    {
        n->next = temp;
        prev->next = n;
    }
}

// ---------------- SERVE ----------------
struct Patient *serveEmergency()
{
    if (!emergencyHead)
        return NULL;

    struct Patient *t = emergencyHead;
    emergencyHead = emergencyHead->next;
    return t;
}

// ---------------- DISPLAY ----------------
void displayQueues()
{
    struct Patient *t;

    printf("\n--- Normal Patients ---\n");
    t = front;
    while (t)
    {
        printf("%s %s %s\n", t->id, t->name, t->doctorName);
        t = t->next;
    }

    printf("\n--- Emergency Patients ---\n");
    t = emergencyHead;
    while (t)
    {
        printf("%s %s Priority:%d\n", t->id, t->name, t->priority);
        t = t->next;
    }
}

// ---------------- MAIN ----------------
int main()
{
    int choice, priority;
    char id[20], name[50], dept[50];

    struct Doctor *root = NULL;

    // PRELOADED DOCTORS
    root = insertDoctor(root, 101, "Dr. A", "Cardiology", 1);
    root = insertDoctor(root, 102, "Dr. B", "Cardiology", 1);
    root = insertDoctor(root, 103, "Dr. C", "Neurology", 1);
    root = insertDoctor(root, 104, "Dr. D", "Orthopedic", 0);
    root = insertDoctor(root, 105, "Dr. E", "Orthopedic", 1);

    while (1)
    {
        printf("\n===== Hospital System =====\n");
        printf("1. Add Normal Patient\n");
        printf("2. Add Emergency Patient\n");
        printf("3. Serve Patient\n");
        printf("4. Display All\n");
        printf("5. Display Doctors\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            sprintf(id, "P-%d", patientCounter++);
            printf("ID: %s\n", id);

            printf("Name: ");
            scanf(" %[^\n]", name);

            printf("Department: ");
            scanf(" %[^\n]", dept);

            printf("\nDoctors in %s:\n", dept);
            displayDoctorsByDepartment(root, dept);

            int did;
            printf("Doctor ID: ");
            scanf("%d", &did);

            struct Doctor *d = searchDoctor(root, did);

            if (!d || strcmp(d->department, dept) != 0)
            {
                printf("Invalid doctor!\n");
                continue;
            }

                        if (d->available == 0)
            {
                int opt;
                printf("Doctor is on leave.\n");
                printf("1. Show other available doctors\n");
                printf("2. Leave\n");
                printf("Enter choice: ");
                scanf("%d", &opt);

                if (opt == 1)
                {
                    printf("\nAvailable doctors in %s:\n", dept);
                    showAvailableDoctors(root, dept);

                    printf("Select Doctor ID: ");
                    scanf("%d", &did);

                    d = searchDoctor(root, did);

                    if (!d || strcmp(d->department, dept) != 0 || d->available == 0)
                    {
                        printf("Invalid doctor selection. Patient leaves.\n");
                        continue;
                    }

                    enqueue(id, name, d->id, d->name);
                }
                else
                {
                    printf("Patient leaves.\n");
                    continue;
                }
            }
            else
            {
                enqueue(id, name, d->id, d->name);
            }
        }

        else if (choice == 2)
        {
            sprintf(id, "E-%d", emergencyCounter++);
            printf("ID: %s\n", id);

            printf("Name: ");
            scanf(" %[^\n]", name);

            printf("Priority: ");
            scanf("%d", &priority);

            insertEmergency(id, name, priority);
        }

        else if (choice == 3)
        {
            struct Patient *p = serveEmergency();

            if (p)
            {
                printf("Emergency: %s\n", p->name);
                free(p);
            }
            else
            {
                p = dequeue();
                if (p)
                {
                    printf("Normal: %s (Doctor: %s)\n", p->name, p->doctorName);
                    free(p);
                }
                else
                    printf("No patients\n");
            }
        }

        else if (choice == 4)
        {
            displayQueues();
        }

        else if (choice == 5)
        {
            printf("\n--- Doctors ---\n");
            displayDoctors(root);
        }

        else if (choice == 6)
            break;

        else
            printf("Invalid\n");
    }

    return 0;
}