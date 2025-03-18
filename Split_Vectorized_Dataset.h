#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH3 1000
#define MAX_EMAILS 2350

void split_dataset(const char *input_file, const char *train_file, const char *test_file, float train_ratio) {
    FILE *input = fopen(input_file, "r");
    FILE *train_output = fopen(train_file, "w");
    FILE *test_output = fopen(test_file, "w");

    if (!input || !train_output || !test_output) {
        printf("Error: Unable to open one or more files.\n");
        return;
    }

    char line[MAX_LINE_LENGTH3];
    int total_lines = 0;

            // Count the total number of lines (excluding the header)
    fgets(line, sizeof(line), input);  // Skip header
    while (fgets(line, sizeof(line), input)) {
        total_lines++;
    }
    rewind(input);  // Reset file pointer to the beginning

            // Calculate the split point
    int train_count = (int)(total_lines * train_ratio);
    int test_count = total_lines - train_count;

            // Copy header to both files
    fgets(line, sizeof(line), input);
    fprintf(train_output, "%s", line);
    fprintf(test_output, "%s", line);

            // Write to training and testing files
    int line_num = 0;
    while (fgets(line, sizeof(line), input)) {
        if (line_num < train_count) {
            fprintf(train_output, "%s", line);
        } else {
            fprintf(test_output, "%s", line);
        }
        line_num++;
    }

    fclose(input);
    fclose(train_output);
    fclose(test_output);

    // printf("Dataset split completed.\n");
    // printf("Training dataset: %s (%d emails)\n", train_file, train_count);
    // printf("Testing dataset: %s (%d emails)\n", test_file, test_count);
}
