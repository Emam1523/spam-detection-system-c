#include <stdio.h>
#include <stdlib.h>

#include "Header.h"

#define MAX_LEN 200

int main() {
    FILE *file_D1 = fopen("DATASET.txt", "r");
    FILE *file_D2 = fopen("New_DATASET.txt", "r");
    FILE *merge_D = fopen("merged_dataset.txt", "w");
    if (file_D1 == NULL || file_D2 == NULL || merge_D == NULL) {
        printf("Error opening files\n");
        return 1;
    }

    char label1[2], message1[MAX_LEN];
    char label2[2], message2[MAX_LEN];

    // Read and merge from the first dataset
    
    // Read and merge from the first dataset
    while (fscanf(file_D1, "%1s, \"%[^\"]\"", label1, message1) != EOF) {
        fprintf(merge_D, "%s, \"%s\"\n", label1, message1);
    }

    // Read and merge from the second dataset
    while (fscanf(file_D2, "%1s, \"%[^\"]\"", label2, message2) != EOF) {
        fprintf(merge_D, "%s, \"%s\"\n", label2, message2);
    }

    fclose(file_D1);
    fclose(file_D2);
    fclose(merge_D);

           // Step 1: Explore Data Analysis (EDA)
    FILE *file = fopen("merged_dataset.txt", "r");
    if (!file) {
        printf("Error: Could not open file 'merged_dataset.txt'.\n");
        return 1;
    }
    eda(file);  // Perform EDA
    fclose(file);  // Close the file after EDA
   // printf("EDA results have been saved to EDA.txt\n");

            // Step 2: Data Preprocessing
    FILE *file2 = fopen("merged_dataset.txt", "r");
    if (!file2) {
        printf("Error: Could not open file 'merged_dataset.txt' for preprocessing.\n");
        return 1;
    }
    preprocessing(file2);  // Perform preprocessing
    fclose(file2);  // Close the file after preprocessing
   // printf("Preprocessed data has been saved to Preprocessed_Dataset.csv\n");

           // Step 3: Preprocessed Data Vectorization
    FILE *file3 = fopen("Preprocessed_Dataset.csv", "r");
    if (!file3) {
        printf("Error: Could not open file 'Preprocessed_Dataset.csv' for vectorization.\n");
        return 1;
    }
    vectorization(file3);  // Vectorize the preprocessed data
    fclose(file3);  // Close the file after vectorization
   // printf("Vectorization completed and saved to Vectorized_Dataset.csv\n");

          // Step 4: Splitting the vectorized dataset
    const char *input_file = "Vectorized_Dataset.csv";  // Path to the vectorized dataset
    const char *train_file = "Training_Dataset.csv";    // Output file for training data
    const char *test_file = "Testing_Dataset.csv";      // Output file for testing data
    float train_ratio = 0.8;                             // 80% for training
    split_dataset(input_file, train_file, test_file, train_ratio);  // Split the dataset

    logistic_model("Training_Dataset.csv", "Testing_Dataset.csv");

    classify_messages("Test_Message.txt");
}
