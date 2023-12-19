#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// linked list
typedef struct Node {
    char word[30];
    struct Node* next;
}Node ;

// function to open the file
FILE* openFile(){
    FILE* fp;
    fp = fopen( "words.txt" , "r");
    if ( fp != NULL ){
        printf("the file was opened successfully  ! \n");
    }else{
        printf("Error File could not be opened");
    }

    return fp;
}

// function to add elements to our linked list
void addelement( Node** head, const char* word) {
     Node* newNode = ( Node*)malloc(sizeof( Node));
    if (newNode == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->word, word);
    newNode->next = *head;
    *head = newNode;
}

//function to print the linked list
void printList( Node* head) {
    while (head != NULL) {
        printf("%s ", head->word);
        head = head->next;
    }
    printf("\n");
}

// Function to free the memory allocated for the linked list
void freeList(struct Node* head) {
    while (head != NULL) {
        struct Node* temp = head;
        head = head->next;
        free(temp);
    }
}

// Function to choose a random word with a specified length from the linked list

char* randomWordWithChosenLength(int length, struct Node* head) {
    char** wordsOfLengthRandomLength = NULL;
    int wordsWithRandomLengthCount = 0;

    // Traverse the linked list and store words of the specified length
    while (head != NULL) {
        if (strlen(head->word) == length) {
            wordsOfLengthRandomLength = realloc(wordsOfLengthRandomLength, (wordsWithRandomLengthCount + 1) * sizeof(char*));
            wordsOfLengthRandomLength[wordsWithRandomLengthCount] = strdup(head->word);
            wordsWithRandomLengthCount++;
        }
        head = head->next;
    }

    if (wordsWithRandomLengthCount == 0) {
        fprintf(stderr, "No words found with the specified length\n");
        return NULL;
    }

    srand(time(NULL));  // Seed the random number generator with the current time
    // Generate a random index to select a word

    int random_word_index = rand() % wordsWithRandomLengthCount;
        // Copy the randomly selected word

    char* random_word = strdup(wordsOfLengthRandomLength[random_word_index]);

    // Free memory allocated for the temporary array
    for (int i = 0; i < wordsWithRandomLengthCount; i++) {
        free(wordsOfLengthRandomLength[i]);
    }
    free(wordsOfLengthRandomLength);

    return random_word;
}

// Function to get word length from user input
int getWordLengthFromUser() {
    int wordLength, validInput;
    printf("Please Enter Word Length: ");

    // Loop until valid input is entered
    do {
        validInput = scanf("%d", &wordLength);

        if (validInput != 1 || wordLength < 1 || wordLength > 16) {
            printf("Invalid Word Length. Please enter a length between 2 and 16.\n");
            printf("Please Enter Word Length: ");

            // Clear the input buffer to remove non-integer input
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    } while (validInput != 1 || wordLength < 1 || wordLength > 16);

    return wordLength;
}

// Function to remove a letter from a given word
char* removeLetter(const char* randomWord, char c) {
    int len = strlen(randomWord);

        // Allocate memory for the modified string
    char* modifiedWord = malloc(len + 1);  // Allocate memory for the modified string

    if (modifiedWord == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    int i, j;
    int found = 0; // Flag to indicate whether the character 'c' has been found

    for (i = 0, j = 0; i < len; i++) {
    // Check if the current character matches 'c' and it's the first occurrence
        if (randomWord[i] == c && !found) {
            found = 1; // Set found flag to indicate the first occurrence of 'c'
        } else {
    // Copy the character to the modified word
            modifiedWord[j] = randomWord[i];
            j++;
        }
    }

    modifiedWord[j] = '\0'; // Null-terminate the modified string

    free((char*)randomWord);  // Free the original randomWord string

    return modifiedWord;
}

// Function to convert a string to lowercase
void convertToLowercase(char* str) {
    int length = strlen(str);
    for (int i = 0; i < length; ++i) {
        str[i] = tolower(str[i]);
    }
}

// Function to check if a word contains a specific letter
int containsLetter(const char* word, char letter) {
    while (*word) {
        if (tolower(*word) == tolower(letter)) {
            return 1; // Letter found
        }
        word++;
    }
    return 0; // Letter not found
}

// Function to perform the guessing game
char* Guessing(char* correctGuess, char* misses, int wordLength, char* randomWord, int* missCounter, int* correctcounter) {
    int difficultyOption = getDifficultyOption();
    int correctGuessCounter = 0;
    int missesCounter = 0;
    int maxMisses, validInput;


    // Set the maximum number of misses based on the difficulty option
    if (difficultyOption == 1) {
        maxMisses = 6;
    } else {
        printf("Choose Difficulty: ");
        do {
            validInput = scanf("%d", &maxMisses);
            if (validInput != 1) {
                printf("Please choose an integer\n");
                printf("Choose Difficulty: ");
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }
        } while (validInput != 1);
    }

        // Initialize the misses and correctGuess arrays
    for (int i = 0; i < wordLength; i++) {
        misses[i] = '\0';
    }

    for (int i = 0; i < wordLength; i++) {
        correctGuess[i] = '\0';
    }

        // Create a copy of the original word for comparison
    char* originalWord = strdup(randomWord);

    // Keep track of guessed letters to avoid counting the same letter as a correct guess multiple times
    int guessedLetters[26] = {0};

    while (correctGuessCounter < wordLength && missesCounter < maxMisses) {
        printf("Guess a letter for the word:\n");

        char c;
        scanf(" %c", &c);

        // Remove the guessed letter from the word
        char* modifiedWord = removeLetter(randomWord, tolower(c));
        randomWord = modifiedWord;

        // Check if the letter is in the original word
        if (containsLetter(originalWord, c)) {
            int index = tolower(c) - 'a';

        // Check if the letter has already been guessed
            if (guessedLetters[index]) {
                printf("You already guessed the letter '%c'. Try again.\n", c);
                continue;
            }

            guessedLetters[index] = 1;

        // Update the correctGuess array and counter
            for (int i = 0; i < wordLength; i++) {
                if (tolower(originalWord[i]) == tolower(c)) {
                    correctGuess[i] = originalWord[i];
                    correctGuessCounter++;
                }
            }
            printf("Correct guess! The letter '%c' is in the word: \n", c);

        // Display the positions of the correct guesses
            for (int i = 0; i < wordLength; i++) {
                if (correctGuess[i] != '\0') {
                    printf("%c", correctGuess[i]);
                } else {
                    printf("*");
                }
            }
            printf("\n");
        } else {

        // Update the misses array and counter
            misses[missesCounter] = c;
            missesCounter++;
            printf("Incorrect guess! The letter '%c' is not in the word.\n", c);
        }
    }

        // Display game results
    if (missesCounter == maxMisses) {
        printf("Game over\n");
        printf("The word is: %s \n", originalWord);
    } else if (correctGuessCounter == wordLength) {
        printf("Victory\n");
    }

    *missCounter = missesCounter;
    *correctcounter = correctGuessCounter;

    // Free the memory allocated for the original word
    free(originalWord);

    // Return the correctGuess array
    return correctGuess;
}

// Function to display the word in **
void showgame( int wl )
{
    if(wl <= 16){
        for (int i = 0 ; i<wl ; i++){
            printf("*");
        }
        printf("\n");
        printf("\n");
    }else{
        printf("There is no Word with the specified length\n\n");
    }

}

// Function to play the game
void playGame(Node* head){
    // Get the desired word length from the user
    int chosenWordLength = getWordLengthFromUser();
    // Get a random word of the specified length from the linked list
   char* randomWord = randomWordWithChosenLength(chosenWordLength, head);
   if (randomWord != NULL) {
 //printf("Random word with length %d: %s\n", chosenWordLength, randomWord);
       showgame(chosenWordLength);

  // free(randomWord);
    }

    char correctGuess[chosenWordLength ];
    char misses[chosenWordLength] ;

    int missCounter = 0;
    int correctGuessCounter = 0 ;

    char* result = Guessing(correctGuess, misses, chosenWordLength, randomWord, &missCounter , &correctGuessCounter);

        // Print correct guesses if any
     if ( correctGuessCounter != 0) {
        printf("correct guesses are : \n");

        for(int i = 0; i < correctGuessCounter; i++){
            printf("%c", result[i]);
        }
        printf("\n");

    } else {
        printf("No correct guess!\n");
    }

    // Print the word if guessed correctly
     if (correctGuessCounter == chosenWordLength )
    {
        printf("the word is %s \n",randomWord);

    }

     // Print the misses if any
    if ( missCounter != 0) {
        printf("misses : \n");

        for(int i = 0; i < missCounter; i++){
            printf("%c", misses[i]);
        }

    } else {
        printf("No misses!\n");
    }
}

// Function to get the first menu option
int getFirstMenueOption(){
    printf("1 - Play\n");
    printf("2 - Exit\n");
    printf("Choose Option (1 or 2): ");

    int option, validInput;

    // Loop until valid input is entered
    do {
        validInput = scanf("%d", &option);

        if (validInput != 1 || (option != 1 && option != 2)) {
            printf("You need to enter a number 1 or 2.\n");
            printf("Choose Option (1 or 2): ");

            // Clear the input buffer to remove non-integer input
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    } while (validInput != 1 || (option != 1 && option != 2));

    if(option == 1 || option == 2){
        return option;
    }else{
        getFirstMenueOption();
    }

}

// Function to get the difficulty option
int getDifficultyOption(){
    printf("Choose Difficulty Level\n\n");
    printf("1 - Default\n");
    printf("2 - Special Level\n");
    int option, validInput;

    do {
        validInput = scanf("%d", &option);
        if (validInput != 1) {
            printf("Please choose an integer 1 or 2\n");
            printf("Choose Difficulty: ");
            // Clear the input buffer to remove non-integer input
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    } while (validInput != 1 || (option != 1 && option != 2));

    if(option == 1 || option == 2){
        return option;
    }else{
        getDifficultyOption();
    }
}

// Function to launch the game
void launcher(Node* head) {
    int option;

    do {
        option = getFirstMenueOption();
        if (option == 1) {
            playGame(head);
            printf("\n\n");
        }
    } while (option != 2);

    exit(1);
}

int main ()
{
    FILE *fp = openFile();
    Node* head = NULL;
    char word[30];

        // Read words from the file to be added to the linked list
    while (fscanf(fp, "%49s", word) == 1) {




        addelement(&head, word);
    }
    // close the file
    fclose(fp);

        // Start the game launcher
    launcher(head);

    // Free memory allocated for the linked list
    freeList(head);

    return 0 ;
}
