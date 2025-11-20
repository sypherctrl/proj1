#include <stdio.h>
#include <string.h>     //for stringcmp()
#define MAXINPUTSIZE 100

void myGetString(char *buffer, int bufferSize) {
    printf("Enter a command: ");
    fgets(buffer, bufferSize, stdin); 
    
    //if input exceeds MAXINPUTSIZE - 1, exit function
    if (bufferSize >= MAXINPUTSIZE - 1) {
        printf("Input exceeds max length. \n");
        return;
    }

    for (int i = 0; i < bufferSize; i++) {
        //if \n, end input line
        if (buffer[i] == '\n') {
            buffer[i] = '\0';
            break;
        } else if (strcmp(buffer, "get") == 0) {
        
        } else if (strcmp(buffer, "set") == 0) {
        
        } else if (strcmp(buffer, "del") == 0) {
        
        } else if (strcmp(buffer, "list") == 0) {

        } else {
        printf("Unknown command.\n");
    }

    }

        //if buffer == 'get' 
        //if buffer == 'set'
        //if buffer == 'del'
        //if buffer == 'list'
        //if buffer == 'quit'
}

int main() {
    char myString[MAXINPUTSIZE]; 
    myGetString(myString, sizeof(myString));

    return 0;
}
