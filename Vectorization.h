#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH2 170
#define MAX_EMAILS 2200
#define MAX_VOCAB_SIZE 430

        // Structure to store email details
typedef struct {
    int label2;       // Spam (1) or Ham (0)
    char email2[MAX_LINE_LENGTH2];
} Email2;

        // Structure to store vocabulary
typedef struct {
    char word[MAX_LINE_LENGTH2];
} Vocabulary;

        // Function to tokenize an email into words
void tokenize(const char *str, char words[MAX_VOCAB_SIZE][MAX_LINE_LENGTH2], int *word_count) {
    char temp[MAX_LINE_LENGTH2];
    strcpy(temp, str); // Copy the input string to a temporary buffer
    char *token = strtok(temp, " ");
    while (token != NULL) {
                 // Remove leading and trailing spaces, and convert to lowercase
        for (int i = 0; token[i]; i++) {
            token[i] = tolower((unsigned char)token[i]);
        }
        
              // Add token to words array if it's not empty
        if (strlen(token) > 0) {
            strcpy(words[*word_count], token);
            (*word_count)++;
        }
        token = strtok(NULL, " ");
    }
}

        // Function to check if a word exists in the vocabulary
int find_word_in_vocab(const char *word, Vocabulary vocab[], int vocab_size) {
    for (int i = 0; i < vocab_size; i++) {
        if (strcmp(vocab[i].word, word) == 0) {
            return i; // Word found
        }
    }
    return -1; // Word not found
}

        // Function to add a word to the vocabulary
int add_word_to_vocab(const char *word, Vocabulary vocab[], int vocab_size) {
    if (vocab_size < MAX_VOCAB_SIZE) {
        strcpy(vocab[vocab_size].word, word);
        return vocab_size + 1; // Increase vocabulary size
    }
    return vocab_size; // Return unchanged size if vocab is full
}

        // Function to build the vocabulary from the dataset
int build_vocabulary(Email2 *emails, int email_count, Vocabulary vocab[]) {
    int vocab_size = 0;
    for (int i = 0; i < email_count; i++) {
        char words[MAX_VOCAB_SIZE][MAX_LINE_LENGTH2];
        int word_count = 0;
        tokenize(emails[i].email2, words, &word_count);

        for (int j = 0; j < word_count; j++) {
            if (find_word_in_vocab(words[j], vocab, vocab_size) == -1) {
                vocab_size = add_word_to_vocab(words[j], vocab, vocab_size);
            }
        }
    }
    return vocab_size;
}

        // Function to save vocabulary to a text file
void save_vocabulary_to_file(Vocabulary vocab[], int vocab_size, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: Unable to create vocabulary file.\n");
        return;
    }

    for (int i = 0; i < vocab_size; i++) {
        fprintf(file, "%s\n", vocab[i].word);
    }

    fclose(file);
    //printf("Vocabulary saved to %s\n", filename);
}

        // Function to vectorize emails based on the vocabulary
void vectorize_emails(Email2 *emails, int email_count, Vocabulary vocab[], int vocab_size) {
    FILE *output_file = fopen("Vectorized_Dataset.csv", "w");
    if (!output_file) {
        printf("Error: Unable to create output file.\n");
        return;
    }

         // Write CSV header
    fprintf(output_file, "label");
    for (int i = 0; i < vocab_size; i++) {
        fprintf(output_file, ",%s", vocab[i].word);
    }
    fprintf(output_file, "\n");

        // Write vectorized data for each email
    for (int i = 0; i < email_count; i++) {
        int word_count_vector[vocab_size];
        memset(word_count_vector, 0, sizeof(word_count_vector));

        char words[MAX_VOCAB_SIZE][MAX_LINE_LENGTH2];
        int word_count = 0;
        tokenize(emails[i].email2, words, &word_count);

        for (int j = 0; j < word_count; j++) {
            int index = find_word_in_vocab(words[j], vocab, vocab_size);
            if (index != -1) {
                word_count_vector[index]++;
            }
        }

            // Write label
        fprintf(output_file, "%d", emails[i].label2);

            // Write word counts for each word in the vocabulary
        for (int j = 0; j < vocab_size; j++) {
            fprintf(output_file, ",%d", word_count_vector[j]);
        }
        fprintf(output_file, "\n");
    }

    fclose(output_file);
}

        // Main function to handle vectorization
int vectorization(FILE *file3) {
    if (!file3) {
        printf("Error: Unable to open dataset file.\n");
        return 1;
    }

    Email2 emails2[MAX_EMAILS];
    Vocabulary vocab[MAX_VOCAB_SIZE];
    int email_count = 0;

    char line[MAX_LINE_LENGTH2];

    // Skip header
    fgets(line, sizeof(line), file3);

    // Read the dataset
    while (fgets(line, sizeof(line), file3) && email_count < MAX_EMAILS) {
        char *token = strtok(line, ",");
        if (!token) continue;

        // Extract label (spam or ham)
        emails2[email_count].label2 = atoi(token);

        // Extract email message
        token = strtok(NULL, "\n");
        if (!token) continue;
        strncpy(emails2[email_count].email2, token, MAX_LINE_LENGTH2);

        email_count++;
    }

    fclose(file3);

            // Build vocabulary
    int vocab_size = build_vocabulary(emails2, email_count, vocab);
    //printf("Vocabulary built with %d unique words.\n", vocab_size);

    save_vocabulary_to_file(vocab, vocab_size, "vocabulary.txt"); 

    // Vectorize emails
    vectorize_emails(emails2, email_count, vocab, vocab_size);

    return 0;
}
