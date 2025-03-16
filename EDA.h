#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 200
#define MAX_EMAILS 2350

// Structure to store email details
typedef struct {
    int label;       // Spam (1) or Ham (0)
    char email[MAX_LINE_LENGTH];
    int length;      // Length of the email (total characters, including spaces)
    int char_count;  // Number of visible (non-whitespace) characters
    int word_count;  // Number of words in the email
} Email;

// Function to count the number of visible (non-whitespace) characters
int count_visible_chars(const char *str) {
    int count = 0;
    while (*str) {
        if (!isspace(*str)) {
            count++;
        }
        str++;
    }
    return count;
}

// Function to count the number of words in a string
int count_words(const char *str) {
    int count = 0;
    int in_word = 0;

    while (*str) {
        if (isspace(*str)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            count++;
        }
        str++;
    }
    return count;
}
int eda(FILE *file){
    Email emails[MAX_EMAILS];
    int email_count = 0;
    char line[MAX_LINE_LENGTH];
    
    // Skip header
    fgets(line, sizeof(line), file);

    // Read the dataset
    while (fgets(line, sizeof(line), file) && email_count < MAX_EMAILS) {
        char *token = strtok(line, ",");
        if (!token) continue;

        // Extract label (spam or ham)
        emails[email_count].label = atoi(token);

        // Extract email message
        token = strtok(NULL, "\n");
        if (!token) continue;
        strncpy(emails[email_count].email, token, MAX_LINE_LENGTH);

        // Calculate EDA metrics
        emails[email_count].length = strlen(emails[email_count].email);              // Total characters (including spaces)
        emails[email_count].char_count = count_visible_chars(emails[email_count].email);  // Visible characters
        emails[email_count].word_count = count_words(emails[email_count].email);

        email_count++;
    }
    fclose(file);

    // Calculate spam and ham statistics
    int spam_count = 0, ham_count = 0;
    for (int i = 0; i < email_count; i++) {
        if (emails[i].label == 1) {
            spam_count++;
        } else {
            ham_count++;
        }
    }
    double spam_percentage = (double)spam_count / email_count * 100.0;
    double ham_percentage = (double)ham_count / email_count * 100.0;

    // Write EDA results to a file
    FILE *output = fopen("EDA.txt", "w");
    if (!output) {
        printf("Error: Could not create output file.\n");
        return 1;
    }

    fprintf(output, "Exploratory Data Analysis (EDA) Results:\n\n");
    fprintf(output, "Total Emails: %d\n\n", email_count);
    fprintf(output, "Columns:\n");
    fprintf(output, "Label | Email | Length | Characters | Words\n");
    fprintf(output, "--------------------------------------------------\n");

    for (int i = 0; i < (email_count < 5 ? email_count : 5); i++) {
        fprintf(output, "%d | %s | %d | %d | %d\n", 
            emails[i].label, emails[i].email, emails[i].length, emails[i].char_count, emails[i].word_count);
    }

    fprintf(output, ".... .... .... .... .... .... .... .... .... ....\n");
    fprintf(output, ".... .... .... .... .... .... .... .... .... ....\n");
    fprintf(output, ".... .... .... .... .... .... .... .... .... ....\n");

    // Write the last five emails
    for (int i = (email_count >= 5 ? email_count - 5 : 0); i < email_count; i++) {
        fprintf(output, "%d | %s | %d | %d | %d\n", 
            emails[i].label, emails[i].email, emails[i].length, emails[i].char_count, emails[i].word_count);
    }

    // Write spam and ham statistics
    fprintf(output, "\nSpam and Ham Statistics:\n");
    fprintf(output, "Spam Emails: %d (%.2f%%)\n", spam_count, spam_percentage);
    fprintf(output, "Ham Emails: %d (%.2f%%)\n", ham_count, ham_percentage);

    fclose(output);
}
