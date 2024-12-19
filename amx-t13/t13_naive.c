
#include <stdio.h>

int main() {
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    int count;
    // Read the first int from the file, which indicates the count
    if (fscanf(file, "%d", &count) != 1) {
        fprintf(stderr, "Failed to read the count from the file\n");
        fclose(file);
        return 1;
    }

    printf("Count: %d\n", count);

    // Read `count` ints from the file and print them
    for (int i = 0; i < count; i++) {
        int number;
        if (fscanf(file, "%d", &number) != 1) {
            fprintf(stderr, "Failed to read number %d\n", i);
            fclose(file);
            return 1;
        }

        printf("Number %d: %d\n", i, number);
    }

    fclose(file);
    return 0;
}
