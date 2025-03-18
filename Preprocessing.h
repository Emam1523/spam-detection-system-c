#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_LINE_LENGTH1 200
#define MAX_EMAILS 2350
#define STOP_WORDS_COUNT 115

        // Sample stop words (You can add more to this list)
const char *stop_words[STOP_WORDS_COUNT] = {"i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your","he","she","it","they","them","their","him","her","his","am","is","are","was","were","be","been","have","has","would","should","will","shall","had","do","does","did","can","could","may","might","the","a","an","and","or","but","if","of","for","with","at","by","from","up","down","in","out","on","then","once","here","there","when","where","why","how","all","any","both","each","few","more","most","other","some","such","no","nor","not","only","own","same","so","than","too","very","s","t","can","will","just","don","should","now","ve","y","ain","aren","couldn","didn","doesn","hadn","hasn","haven","isn","ma","mightn","mustn","needn","shan","shouldn","wasn","weren","won","wouldn"};

        // Structure to store email details
typedef struct {
    int label1;       // Spam (1) or Ham (0)
    char email1[MAX_LINE_LENGTH1];
} Email1;

        // Function to convert string to lowercase
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

        // Function to check if a word is a stop word
bool is_stop_word(const char *word) {
    for (int i = 0; i < STOP_WORDS_COUNT; i++) {
        if (strcmp(word, stop_words[i]) == 0) {
            return true;
        }
    }
    return false;
}

        // Function to remove punctuation from a string
void remove_punctuation(char *str) {
    int i = 0;
    while (str[i]) {
        if (ispunct((unsigned char)str[i])) {
                    // Shift the characters to the left to remove punctuation
            memmove(&str[i], &str[i + 1], strlen(&str[i]));
        } else {
            i++;
        }
    }
}

        // Function to remove special characters and numeric digits from a string
void remove_special_characters_and_numbers(char *str) {
    int i = 0;
    while (str[i]) {
        if (!isalnum((unsigned char)str[i]) && !isspace((unsigned char)str[i])) {
                    // Shift the characters to the left to remove special characters
            memmove(&str[i], &str[i + 1], strlen(&str[i]));
        } else if (isdigit((unsigned char)str[i])) {
                    // Remove numeric digits by shifting the remaining characters
            memmove(&str[i], &str[i + 1], strlen(&str[i]));
        } else {
            i++;
        }
    }
}


        // Function to stem a word (very simple stemmer based on common suffixes)
void stem_word(char *word) {
    int length1 = strlen(word);

            // Simple stemming logic: remove common suffixes
    if (length1 > 3) {
        if (strcmp(&word[length1 - 3], "ing") == 0) {
            word[length1 - 3] = '\0';  // Remove "ing"
        } else if (strcmp(&word[length1 - 2], "ed") == 0) {
            word[length1 - 2] = '\0';  // Remove "ed"
        }
    }
}

        // Function to preprocess and tokenize the email message
void preprocess_email(char *email, char *processed_email) {
    char temp_email[MAX_LINE_LENGTH];
    strcpy(temp_email, email);  // Copy the original email

            // Step 1: Convert to lowercase
    to_lowercase(temp_email);

            // Step 2: Remove punctuation and numbers
    remove_special_characters_and_numbers(temp_email);

            // Step 3: Tokenize the email and process each word
    char *token = strtok(temp_email, " ");
    bool first_word = true;

    while (token != NULL) {
                // Step 4: Remove stop words
        if (!is_stop_word(token)) {
                    // Step 5: Stem the word
            stem_word(token);

                 // Append the word to processed_email
            if (!first_word) {
                strcat(processed_email, " ");
            }
            strcat(processed_email, token);
            first_word = false;
        }
        token = strtok(NULL, " ");
    }
}

        // Function to write the processed data to a new CSV file
void write_processed_data_to_csv(Email1 *emails, int email_count) {
    FILE *output = fopen("Preprocessed_Dataset.csv", "w");
    if (!output) {
        printf("Error: Could not create output file.\n");
        return;
    }

         // Write header
    fprintf(output, "label,Message\n");

            // Write processed emails
    for (int i = 0; i < email_count; i++) {
        char processed_email[MAX_LINE_LENGTH] = "";
        preprocess_email(emails[i].email1, processed_email);
        fprintf(output, "%d,%s\n", emails[i].label1, processed_email);
    }

    fclose(output);
}

        // Function to preprocess the dataset from the input file
int preprocessing(FILE *file2) {
    Email1 emails1[MAX_EMAILS];
    int email_count = 0;
    char line[MAX_LINE_LENGTH];
    
             // Skip header
    fgets(line, sizeof(line), file2);

            // Read the dataset
    while (fgets(line, sizeof(line), file2) && email_count < MAX_EMAILS) {
        char *token = strtok(line, ",");
        if (!token) continue;

                // Extract label (spam or ham)
        emails1[email_count].label1 = atoi(token);

                // Extract email message
        token = strtok(NULL, "\n");
        if (!token) continue;
        strncpy(emails1[email_count].email1, token, MAX_LINE_LENGTH);

        email_count++;
    }

    fclose(file2);

            // Process and save the preprocessed emails to a new CSV file
    write_processed_data_to_csv(emails1, email_count);
    return email_count;
}

