#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define STORAGE_FILE "storage.dat"
#define USERS_FILE "users.dat"
#define MAX_USERS 10
#define MAX_FILES 100
#define MAX_NAME 32
#define MAX_PASS 32
#define MAX_SIZE 1024

typedef struct {
    char username[MAX_NAME];
    char password[MAX_PASS];
} User;

typedef struct {
    char owner[MAX_NAME];
    char name[MAX_NAME];
    size_t size;
    time_t created_at;
    char data[MAX_SIZE];
} VirtualFile;

User users[MAX_USERS];
VirtualFile files[MAX_FILES];
int user_count = 0, file_count = 0;
pthread_mutex_t lock;

// Load users from file
void load_users() {
    FILE *fp = fopen(USERS_FILE, "rb");
    if (fp) {
        fread(users, sizeof(User), MAX_USERS, fp);
        fclose(fp);
    }
}

// Save users to file
void save_users() {
    FILE *fp = fopen(USERS_FILE, "wb");
    fwrite(users, sizeof(User), MAX_USERS, fp);
    fclose(fp);
}

// Register a new user
void register_user() {
    if (user_count >= MAX_USERS) {
        printf("User limit reached!\n");
        return;
    }
    printf("Enter username: ");
    scanf("%s", users[user_count].username);
    printf("Enter password: ");
    scanf("%s", users[user_count].password);
    
    user_count++;
    save_users();
    printf("User registered successfully!\n");
}

// Authenticate user
int login(char *username) {
    char password[MAX_PASS];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            printf("Login successful!\n");
            return 1;
        }
    }
    printf("Invalid credentials!\n");
    return 0;
}

// Load storage data
void load_storage() {
    FILE *fp = fopen(STORAGE_FILE, "rb");
    if (fp) {
        fread(files, sizeof(VirtualFile), MAX_FILES, fp);
        fclose(fp);
    }
}

// Save storage data
void save_storage() {
    FILE *fp = fopen(STORAGE_FILE, "wb");
    fwrite(files, sizeof(VirtualFile), MAX_FILES, fp);
    fclose(fp);
}

// Create a new file
void *create_file(void *arg) {
    char *owner = (char *)arg;
    pthread_mutex_lock(&lock);

    if (file_count >= MAX_FILES) {
        printf("Storage full!\n");
        pthread_mutex_unlock(&lock);
        return NULL;
    }

    printf("Enter filename: ");
    scanf("%s", files[file_count].name);
    printf("Enter content: ");
    getchar();  
    fgets(files[file_count].data, MAX_SIZE, stdin);

    strcpy(files[file_count].owner, owner);
    files[file_count].size = strlen(files[file_count].data);
    files[file_count].created_at = time(NULL);
    file_count++;

    save_storage();
    pthread_mutex_unlock(&lock);
    printf("File created successfully!\n");
    return NULL;
}

// List files owned by a user
void list_files(char *owner) {
    printf("Your stored files:\n");
    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].owner, owner) == 0) {
            printf("%d. %s (%zu bytes)\n", i + 1, files[i].name, files[i].size);
        }
    }
}

// Read a file
void read_file(char *owner) {
    char name[MAX_NAME];
    printf("Enter filename to read: ");
    scanf("%s", name);

    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].name, name) == 0 && strcmp(files[i].owner, owner) == 0) {
            printf("Content: %s\n", files[i].data);
            return;
        }
    }
    printf("File not found!\n");
}

// Delete a file
void delete_file(char *owner) {
    char name[MAX_NAME];
    printf("Enter filename to delete: ");
    scanf("%s", name);

    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].name, name) == 0 && strcmp(files[i].owner, owner) == 0) {
            for (int j = i; j < file_count - 1; j++) {
                files[j] = files[j + 1];
            }
            file_count--;
            save_storage();
            printf("File deleted!\n");
            return;
        }
    }
    printf("File not found!\n");
}

// Main function
int main() {
    pthread_mutex_init(&lock, NULL);
    load_users();
    load_storage();

    int choice;
    char username[MAX_NAME];

    while (1) {
        printf("\n1. Register\n2. Login\n3. Exit\n");
        printf("Select an option: ");
        scanf("%d", &choice);

        if (choice == 1) register_user();
        else if (choice == 2 && login(username)) {
            pthread_t thread;
            while (1) {
                printf("\n1. Create File\n2. List Files\n3. Read File\n4. Delete File\n5. Logout\n");
                printf("Select an option: ");
                scanf("%d", &choice);

                if (choice == 1) pthread_create(&thread, NULL, create_file, username);
                else if (choice == 2) list_files(username);
                else if (choice == 3) read_file(username);
                else if (choice == 4) delete_file(username);
                else break;
            }
            pthread_join(thread, NULL);
        } else if (choice == 3) break;
    }

    pthread_mutex_destroy(&lock);
    return 0;
}
