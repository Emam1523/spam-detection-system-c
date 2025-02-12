#include "Header.h"

int main() {
           // Step 1: Explore Data Analysis (EDA)
    FILE *file = fopen("DATASET.txt", "r");
    if (!file) {
        printf("Error: Could not open file 'DATASET.txt'.\n");
        return 1;
    }
    eda(file);  // Perform EDA
    fclose(file);  // Close the file after EDA
   // printf("EDA results have been saved to EDA.txt\n");

           // Step 2: Data Preprocessing
    FILE *file2 = fopen("DATASET.txt", "r");
    if (!file2) {
        printf("Error: Could not open file 'DATASET.txt' for preprocessing.\n");
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

    logistic_model_small("Training_Dataset.csv", "Testing_Dataset.csv");

    //logistic_model_large("Training_Dataset.csv", "Testing_Dataset.csv");

    classify_messages("Test_Message.txt");
}
