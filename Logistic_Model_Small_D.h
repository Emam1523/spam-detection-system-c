#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_EMAILS 2200
#define VOCAB_SIZE 430
#define LEARNING_RATE 0.1
#define EPOCHS 2000

        // Dataset: Emails and their labels (1 = spam, 0 = ham)
double emails[MAX_EMAILS][VOCAB_SIZE];
int labels[MAX_EMAILS];

        // Model parameters
double weights[VOCAB_SIZE] = {0};
double bias = 0;

        // Sigmoid function
double sigmoid(double z) {
    return 1.0 / (1.0 + exp(-z));
}

        // Predict the probability of an email being spam
double predict(double *email) {
    double z = bias;
    for (int i = 0; i < VOCAB_SIZE; i++) {
        z += weights[i] * email[i];
    }
    return sigmoid(z);
}

        // Train the logistic regression model
void train() {
    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        double dw[VOCAB_SIZE] = {0};
        double db = 0;

        // Compute gradients
        for (int i = 0; i < MAX_EMAILS; i++) {
            double y_pred = predict(emails[i]);
            double error = y_pred - labels[i];
            for (int j = 0; j < VOCAB_SIZE; j++) {
                dw[j] += error * emails[i][j];
            }
            db += error;
        }

        // Update weights and bias
        for (int j = 0; j < VOCAB_SIZE; j++) {
            weights[j] -= LEARNING_RATE * dw[j] / MAX_EMAILS;
        }
        bias -= LEARNING_RATE * db / MAX_EMAILS;

        // Print loss and gradients every 100 epochs
        if (epoch % 100 == 0) {
            double loss = 0;
            for (int i = 0; i < MAX_EMAILS; i++) {
                double y_pred = predict(emails[i]);
                loss += -labels[i] * log(y_pred) - (1 - labels[i]) * log(1 - y_pred);
            }
            loss /= MAX_EMAILS;
            //printf("Epoch %d, Loss: %f, dw[0]: %f, db: %f\n", epoch, loss, dw[0], db);
        }
    }
}

        // This is for mini dataset. Load dataset from CSV file (first column = label, rest = features)
void load_dataset(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    // Skip header
    char line[4096];   //4096 for mini dataset
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Error: File is empty or missing header\n");
        fclose(file);
        exit(1);
    }

    int row_count = 0;
    while (fgets(line, sizeof(line), file) && row_count < MAX_EMAILS) {
        char *token = strtok(line, ",\n");
        if (!token) {
            printf("Error: No label at row %d\n", row_count);
            continue;
        }

        // Parse label
        int label = atoi(token);
        labels[row_count] = label;

        // Parse features
        int col = 0;
        while ((token = strtok(NULL, ",\n")) != NULL && col < VOCAB_SIZE) {
            emails[row_count][col] = atof(token);
            col++;
        }

        if (col != VOCAB_SIZE) {
            //printf("Error: Row %d has %d features (expected %d)\n", row_count, col, VOCAB_SIZE);
            continue;
        }

        row_count++;
    }

    fclose(file);
    //printf("Dataset loaded successfully. Total rows: %d\n", row_count);
}

        // Save trained model to a file
void save_model(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        exit(1);
    }

    for (int i = 0; i < VOCAB_SIZE; i++) {
        fprintf(file, "%lf,", weights[i]);
    }
    printf("\n");
    fprintf(file, "%lf\n", bias);

    fclose(file);
}

        // This is for mini dataset. Evaluate the model on a test dataset
void evaluate_model(const char *test_file) {
    double (*test_emails)[VOCAB_SIZE] = (double (*)[VOCAB_SIZE])malloc(MAX_EMAILS * sizeof(*test_emails));
    int *test_labels = (int *)malloc(MAX_EMAILS * sizeof(int));
    if (!test_emails || !test_labels) {
        perror("Failed to allocate memory for test data");
        free(test_emails);
        free(test_labels);
        exit(1);
    }

    int test_row_count = 0;
    //printf("Loading test data from %s\n", test_file);
    FILE *file = fopen(test_file, "r");
    if (!file) {
        free(test_emails);
        free(test_labels);
        perror("Error opening test file");
        exit(1);
    }

    // Skip header
    char line[8192];   //8192 for mini dataset
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Error: Test file is empty or missing header\n");
        fclose(file);
        free(test_emails);
        free(test_labels);
        exit(1);
    }

    // Read test data
    while (fgets(line, sizeof(line), file) && test_row_count < MAX_EMAILS) {
        char *token = strtok(line, ",\n");
        if (!token) {
            printf("Error: No label at test row %d\n", test_row_count);
            continue;
        }

        // Parse label
        test_labels[test_row_count] = atoi(token);

        // Parse features
        int col = 0;
        while ((token = strtok(NULL, ",\n")) != NULL && col < VOCAB_SIZE) {
            test_emails[test_row_count][col] = atof(token);
            col++;
        }

        if (col != VOCAB_SIZE) {
            printf("Error: Test row %d has %d features (expected %d)\n", test_row_count, col, VOCAB_SIZE);
            continue;
        }

        test_row_count++;
    }

    fclose(file);

    // Check if any test data was loaded
    if (test_row_count == 0) {
        printf("Error: No test data loaded\n");
        free(test_emails);
        free(test_labels);
        exit(1);
    }

    // Evaluate the model
    int correct = 0;
    for (int i = 0; i < test_row_count; i++) {
        double y_pred = predict(test_emails[i]);
        int prediction = (y_pred >= 0.5) ? 1 : 0;
        if (prediction == test_labels[i]) {
            correct++;
        }
    }

    double accuracy = (double)correct / test_row_count;
    //printf("Test Accuracy: %.2f%%\n", accuracy * 100);

            // Free allocated memory
    free(test_emails);
    free(test_labels);
}

void logistic_model_small(const char *tr_file, const char *te_file) {
    if(tr_file == NULL || te_file == NULL) {
        printf("Error: Invalid file paths.\n");
        return;
    }
    // Load dataset
    load_dataset(tr_file);

        // Train the model
    train();
    //printf("Model trained successfully!\n");

    // Save the trained model
    save_model("logistic_model.txt");

        // Evaluate the model on the test dataset
    evaluate_model(te_file);

    //printf("Model trained, saved, and evaluated successfully!\n");
}