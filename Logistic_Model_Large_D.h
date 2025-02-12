// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>
// #include <string.h>

// #define MAX_EMAILS 5200
// #define VOCAB_SIZE 3300
// #define LEARNING_RATE 0.1
// #define EPOCHS 500 
// #define LINE_BUFFER_SIZE (64 * 1024)  // 64 KB buffer for large datasets
// int training_size;

// // Dataset: Emails and their labels (1 = spam, 0 = ham)
// double emails_L[MAX_EMAILS][VOCAB_SIZE];
// int labels_L[MAX_EMAILS];

// // Model parameters
// double weights_L[VOCAB_SIZE] = {0};
// double bias_L = 0;

// // Sigmoid function
// double sigmoid_L(double z) {
//     return 1.0 / (1.0 + exp(-z));
// }

// // Predict the probability of an email being spam
// double predict_L(double *email) {
//     double z = bias_L;
//     for (int i = 0; i < VOCAB_SIZE; i++) {
//         z += weights_L[i] * email[i];
//     }
//     return sigmoid_L(z);
// }

// // Train the logistic regression model
// void train_L() {
//     for (int epoch = 0; epoch < EPOCHS; epoch++) {
//         double dw[VOCAB_SIZE] = {0};
//         double db = 0;

//         // Compute gradients
//         for (int i = 0; i < MAX_EMAILS; i++) {
//             double y_pred = predict_L(emails_L[i]);
//             double error = y_pred - labels_L[i];
//             for (int j = 0; j < VOCAB_SIZE; j++) {
//                 dw[j] += error * emails_L[i][j];
//             }
//             db += error;
//         }

//         // Update weights and bias
//         for (int j = 0; j < VOCAB_SIZE; j++) {
//             weights_L[j] -= LEARNING_RATE * dw[j] / MAX_EMAILS;
//         }
//         bias_L -= LEARNING_RATE * db / MAX_EMAILS;

//         // Print loss and gradients every 100 epochs
//         if (epoch % 100 == 0) {
//             double loss = 0;
//             for (int i = 0; i < MAX_EMAILS; i++) {
//                 double y_pred = predict_L(emails_L[i]);
//                 loss += -labels_L[i] * log(y_pred) - (1 - labels_L[i]) * log(1 - y_pred);
//             }
//             loss /= MAX_EMAILS;
//             printf("Epoch %d, Loss: %f, dw[0]: %f, db: %f\n", epoch, loss, dw[0], db);
//         }
//     }
// }

// // Load dataset from CSV file (first column = label, rest = features)
// int load_dataset_L(const char *filename) {
//     FILE *file = fopen(filename, "r");
//     if (!file) {
//         perror("Error opening file");
//         exit(1);
//     }

//     char *line = (char*)malloc(LINE_BUFFER_SIZE);
//     if (!line) {
//         perror("Failed to allocate line buffer");
//         fclose(file);
//         exit(1);
//     }

//     // Skip header
//     if (fgets(line, LINE_BUFFER_SIZE, file) == NULL) {
//         printf("Error: File is empty or missing header\n");
//         free(line);
//         fclose(file);
//         exit(1);
//     }

//     int row_count = 0;
//     while (row_count < MAX_EMAILS && fgets(line, LINE_BUFFER_SIZE, file)) {
//         if (strchr(line, '\n') == NULL) {
//             printf("Error: Line %d truncated (buffer too small)\n", row_count);
//             continue;
//         }

//         char *token = strtok(line, ",\n");
//         if (!token) {
//             printf("Error: No label at row %d\n", row_count);
//             continue;
//         }

//         int label = atoi(token);
//         labels_L[row_count] = label;

//         int col = 0;
//         while ((token = strtok(NULL, ",\n")) != NULL && col < VOCAB_SIZE) {
//             emails_L[row_count][col] = atof(token);
//             col++;
//         }

//         if (col != VOCAB_SIZE) {
//             printf("Error: Row %d has %d features (expected %d)\n", row_count, col, VOCAB_SIZE);
//             continue;
//         }

//         row_count++;
//     }

//     free(line);  // Free the buffer
//     fclose(file);
//     printf("Loaded %d rows\n", row_count);
//     return row_count;
// }

// // Save trained model to a file
// void save_model_L(const char *filename) {
//     FILE *file = fopen(filename, "w");
//     if (!file) {
//         printf("Error: Could not open file %s\n", filename);
//         exit(1);
//     }

//     for (int i = 0; i < VOCAB_SIZE; i++) {
//         fprintf(file, "%lf,", weights_L[i]);
//     }
//     fprintf(file, "%lf\n", bias_L);

//     fclose(file);
// }

// // New function to evaluate training set
// void evaluate_training_set() {
//     int TP = 0, FP = 0, FN = 0, TN = 0;

//     for (int i = 0; i < training_size; i++) {
//         double y_pred = predict_L(emails_L[i]);
//         int prediction = (y_pred >= 0.5) ? 1 : 0;
//         int actual = labels_L[i];

//         if (actual == 1 && prediction == 1) TP++;
//         else if (actual == 0 && prediction == 1) FP++;
//         else if (actual == 1 && prediction == 0) FN++;
//         else if (actual == 0 && prediction == 0) TN++;
//     }

//     double precision = (TP + FP) ? (double)TP / (TP + FP) : 0;
//     double recall = (TP + FN) ? (double)TP / (TP + FN) : 0;
//     double f1 = (precision + recall) ? 2 * precision * recall / (precision + recall) : 0;
//     double accuracy = (double)(TP + TN) / training_size;

//     printf("\nTraining Set Metrics:\n");
//     printf("Accuracy: %.2f%%\n", accuracy * 100);
//     printf("Precision: %.2f%%\n", precision * 100);
//     printf("Recall: %.2f%%\n", recall * 100);
//     printf("F1 Score: %.2f%%\n\n", f1 * 100);
// }

// // Evaluate the model on a test dataset
// void evaluate_model_L(const char *test_file) {
//     double (*test_emails)[VOCAB_SIZE] = (double (*)[VOCAB_SIZE])malloc(MAX_EMAILS * sizeof(*test_emails));
//     int *test_labels = (int *)malloc(MAX_EMAILS * sizeof(int));
//     if (!test_emails || !test_labels) {
//         perror("Failed to allocate test data");
//         free(test_emails);
//         free(test_labels);
//         exit(1);
//     }

//     // Dynamically allocate line buffer
//     char *line = (char*)malloc(LINE_BUFFER_SIZE);
//     if (!line) {
//         perror("Failed to allocate line buffer");
//         free(test_emails);
//         free(test_labels);
//         exit(1);
//     }

//     FILE *file = fopen(test_file, "r");
//     if (!file) {
//         perror("Error opening test file");
//         free(test_emails);
//         free(test_labels);
//         free(line);
//         exit(1);
//     }

//     // Skip header
//     if (fgets(line, LINE_BUFFER_SIZE, file) == NULL) {
//         printf("Error: Test file is empty or missing header\n");
//         free(test_emails);
//         free(test_labels);
//         free(line);
//         fclose(file);
//         exit(1);
//     }

//     int test_row_count = 0;
//     while (test_row_count < MAX_EMAILS && fgets(line, LINE_BUFFER_SIZE, file)) {
//         if (strchr(line, '\n') == NULL) {
//             printf("Error: Test line %d truncated\n", test_row_count);
//             continue;
//         }

//         char *token = strtok(line, ",\n");
//         if (!token) {
//             printf("Error: No label at test row %d\n", test_row_count);
//             continue;
//         }
//         test_labels[test_row_count] = atoi(token);
//         int col = 0;
//         while ((token = strtok(NULL, ",\n")) != NULL && col < VOCAB_SIZE) {
//             test_emails[test_row_count][col] = atof(token);
//             col++;
//         }

//         if (col != VOCAB_SIZE) {
//             printf("Error: Test row %d has %d features (expected %d)\n", test_row_count, col, VOCAB_SIZE);
//             continue;
//         }

//         test_row_count++;
//     }

//     free(line);          // Free line buffer
//     fclose(file);

//     // Evaluate model and free test data
//     int correct = 0, TP = 0, FP = 0, FN = 0, TN = 0;;
//     for (int i = 0; i < test_row_count; i++) {
//         double y_pred = predict_L(test_emails[i]);
//         int prediction = (y_pred >= 0.5) ? 1 : 0;
//         int actual = test_labels[i];

//         if (actual == 1 && prediction == 1) TP++;
//         else if (actual == 0 && prediction == 1) FP++;
//         else if (actual == 1 && prediction == 0) FN++;
//         else if (actual == 0 && prediction == 0) TN++;
//     }

//     double precision = (TP + FP) ? (double)TP / (TP + FP) : 0;
//     double recall = (TP + FN) ? (double)TP / (TP + FN) : 0;
//     double f1 = (precision + recall) ? 2 * precision * recall / (precision + recall) : 0;
//     double accuracy = (double)(TP + TN) / test_row_count;

//     printf("\nTest Set Metrics:\n");
//     printf("Accuracy: %.2f%%\n", accuracy * 100);
//     printf("Precision: %.2f%%\n", precision * 100);
//     printf("Recall: %.2f%%\n", recall * 100);
//     printf("F1 Score: %.2f%%\n\n", f1 * 100);

//     free(test_emails);
//     free(test_labels);
// }

// void logistic_model_large(const char *tr_file, const char *te_file) {
//     if(tr_file == NULL || te_file == NULL) {
//         printf("Error: Invalid file paths.\n");
//         return;
//     }
//     // Load dataset
//     training_size = load_dataset_L(tr_file);

//     // Train the model
//     train_L();
//     printf("Model trained successfully!\n");

//     // Evaluate on training set
//     evaluate_training_set();

//     // Save the trained model
//     save_model_L("logistic_model_L.txt");

//     // Evaluate the model on the test dataset
//     evaluate_model_L(te_file);

//         printf("Model trained, saved, and evaluated successfully!\n");
// }