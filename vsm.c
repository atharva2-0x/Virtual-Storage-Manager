#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STORAGE_FILE "storage.dat"
#define MAX_FILES 100
#define MAX_NAME 32
#define MAX_SIZE 1024

typedef struct {
    char name[MAX_NAME];
    size_t size;
    time_t created_at;
    char data[MAX_SIZE];
} VirtualFile;

VirtualFile files[MAX_FILES];
int file_count = 0;

void init_storage() {
    FILE *fp = fopen(STORAGE_FILE, "rb");
    if (fp) {
        fread(files, sizeof(VirtualFile), MAX_FILES, fp);
        fclose(fp);
    }
}

void save_storage() {
    FILE *fp = fopen(STORAGE_FILE, "wb");
    fwrite(files, sizeof(VirtualFile), MAX_FILES, fp);
    fclose(fp);
}

void create_file(const char *name, const char *content) {
    if (file_count >= MAX_FILES) {
        printf("Storage is full!\n");
        return;
    }

    strcpy(files[file_count].name, name);
    strcpy(files[file_count].data, content);
    files[file_count].size = strlen(content);
    files[file_count].created_at = time(NULL);
    file_count++;

    save_storage();
    printf("File '%s' created successfully!\n", name);
}

void list_files() {
    printf("Files in storage:\n");
    for (int i = 0; i < file_count; i++) {
        printf("%d. %s (%zu bytes)\n", i + 1, files[i].name, files[i].size);
    }
}

void read_file(const char *name) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].name, name) == 0) {
            printf("Content of %s: %s\n", name, files[i].data);
            return;
        }
    }
    printf("File not found!\n");
}

void delete_file(const char *name) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].name, name) == 0) {
            for (int j = i; j < file_count - 1; j++) {
                files[j] = files[j + 1];
            }
            file_count--;
            save_storage();
            printf("File '%s' deleted.\n", name);
            return;
        }
    }
    printf("File not found!\n");
}

int main() {
    init_storage();
    int choice;
    char name[MAX_NAME];
    char content[MAX_SIZE];

    while (1) {
        printf("\nVirtual Storage Manager:\n");
        printf("1. Create File\n2. List Files\n3. Read File\n4. Delete File\n5. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Enter filename: ");
                fgets(name, MAX_NAME, stdin);
                name[strcspn(name, "\n")] = 0;
                printf("Enter content: ");
                fgets(content, MAX_SIZE, stdin);
                content[strcspn(content, "\n")] = 0;
                create_file(name, content);
                break;
            case 2:
                list_files();
                break;
            case 3:
                printf("Enter filename to read: ");
                fgets(name, MAX_NAME, stdin);
                name[strcspn(name, "\n")] = 0;
                read_file(name);
                break;
            case 4:
                printf("Enter filename to delete: ");
                fgets(name, MAX_NAME, stdin);
                name[strcspn(name, "\n")] = 0;
                delete_file(name);
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}
