#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define VOCAB_SIZE 550
#define MAX_WORD_LEN 50

char vocabulary[VOCAB_SIZE][MAX_WORD_LEN];
double weights[VOCAB_SIZE];
double bias;

void load_vocabulary_and_weights(const char* vocab_file, const char* weights_file) {
    FILE *f_vocab = fopen(vocab_file, "r");
    FILE *f_weights = fopen(weights_file, "r");

    if (!f_vocab || !f_weights) {
        printf("Error: Could not open vocabulary or weights file\n");
        exit(1);
    }

    // Load vocabulary (one word per line)
    for (int i = 0; i < VOCAB_SIZE; i++) {
        fscanf(f_vocab, "%s", vocabulary[i]);
    }

    // Load weights and bias
    for (int i = 0; i < VOCAB_SIZE; i++) {
        fscanf(f_weights, "%lf", &weights[i]);
    }
    fscanf(f_weights, "%lf", &bias);

    fclose(f_vocab);
    fclose(f_weights);
}

// Convert string to lowercase
void to_lowercase_T(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Remove special characters
void remove_special_chars(char* str) {
    int j = 0;
    for (int i = 0; str[i]; i++) {
        if (isalnum(str[i]) || str[i] == ' ') {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

// Simple tokenization function (splits by spaces)
int tokenize_T(const char *message, char words[][MAX_WORD_LEN]) {
    int count = 0;
    char temp_msg[1000];
    strncpy(temp_msg, message, sizeof(temp_msg));
    temp_msg[sizeof(temp_msg) - 1] = '\0';

    char *token = strtok(temp_msg, " ");
    while (token != NULL && count < 100) {
        strncpy(words[count], token, MAX_WORD_LEN);
        words[count][MAX_WORD_LEN - 1] = '\0';
        count++;
        token = strtok(NULL, " ");
    }
    return count;
}

// Vectorize the message based on the vocabulary
void vectorize_message(const char* message, int* feature_vector) {
    char words[100][MAX_WORD_LEN];  // Temporary storage for tokenized words
    int num_words = tokenize_T(message, words);

    // Initialize feature vector to 0
    for (int i = 0; i < VOCAB_SIZE; i++) {
        feature_vector[i] = 0;
    }

    // Check if each vocabulary word exists in the message
    for (int i = 0; i < VOCAB_SIZE; i++) {
        for (int j = 0; j < num_words; j++) {
            if (strcmp(words[j], vocabulary[i]) == 0) {
                feature_vector[i] = 1;  // Binary occurrence
                break;
            }
        }
    }
}

double sigmoid_T(double z) {
    return 1.0 / (1.0 + exp(-z));
}

// Predict the probability of spam
double predict_spam_probability(int* feature_vector) {
    double z = bias;
    for (int i = 0; i < VOCAB_SIZE; i++) {
        z += weights[i] * feature_vector[i];
    }
    return sigmoid_T(z);
}

int is_spam(double probability) {
    return (probability >= 0.5) ? 1 : 0;
}

/*Classify message*/
int classify_messages(const char* filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Error: Could not open file %s\n", filename);
        return 1;
    }

    // Load vocabulary and weights
    load_vocabulary_and_weights("vocabulary.txt", "logistic_model.txt");

    char message[1000];
    int message_count = 0;

    // Read and classify each line as a separate message
    while (fgets(message, sizeof(message), f)) {

        printf("Message %d: %.500s\n", ++message_count, message); // Truncate long messages for display

        // Remove trailing newline if present
        message[strcspn(message, "\n")] = 0;

        // Preprocess the message
        to_lowercase(message);
        remove_special_chars(message);

        // Vectorize the message
        int feature_vector[VOCAB_SIZE] = {0};
        vectorize_message(message, feature_vector);

        // Predict spam probability
        double prob = predict_spam_probability(feature_vector);

        // Print result
        printf("Spam Probability: %.2f%%\n", prob * 100);
        printf("Prediction: %s\n\n", is_spam(prob) ? "SPAM" : "HAM");
    }

    fclose(f);
    return 0;
}
