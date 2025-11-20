#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCAPACITY 256

    //define an enum for cmd types
    enum commandType {
            CMD_GET,
            CMD_SET,
            CMD_DEL,
            CMD_LIST,
            CMD_QUIT,
            CMD_UNKNOWN
    };

    //define a struct to hold a parsed cmd
    struct command {
        enum commandType type;
        char *key;  //NULL if not needed
        char *value;    //NULL if not needed
    };

    struct command cmd;
    
    //entry struct
    struct entry {
        char *key;
        char *value;
        int in_use;     //or a flag; 0 = empty, 1 = used
    };

    //database struct
    struct database {
        struct entry *entries;      //dynamically allocated array
        int count;      //how many entries are actually used
        int capacity;   //how big the entries array is
        //use malloc/realloc/etc to grow it later
    };

// for ONLY input reading
int getInput(char *line, int lineSize) {
    /* 1. read line until \n (from stdin into buffer)
    ensure its a valid C string, error if not
    2. delete the \n, ensure \0 is at the end
    3. return 1 for success, return 0 if EOF or error
    */
    
    if (fgets(line, lineSize, stdin) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        printf("Input string is valid. \n"); 
        return 1;
        }
    if(lineSize > (MAXCAPACITY - 1)) {
        printf("Error reading input.\n");
        return 0;
        } 
    if (fgets(line, lineSize, stdin) == NULL) {
        printf("EOF was reached.\n");
        return 0;
        } else {
        printf("Error reading input.\n");
        return 0;
    }
}

//out is a pointer to the struct command
//out->type means write into the type field inside that struct
int cmdParser(char *line, struct command *out) {
    /* 1. Take *line as input.
    2. Split into space seperated tokens using strtok()
    token 0 --> cmd
    token 1 --> key
    token 2 --> value (may include spaces but thats later)
    3. compare the cmd (token 0) to known commands and set out-->type
    4. set out-> key and out->value (or NULL)
    5. return 1 for success, return 0 if parsing failed.
    */
    char *token;
    char *token0;
    char *token1;
    char *token2;
    char *saveptr; // Pointer for strtok_r's internal state

    if (line == NULL) {
        perror("strdup failed");
        return 0;
    }
    // First call to strtok_r: pass the string to be tokenized
    token0 = strtok_r(line, " ", &saveptr);
    token1 = strtok_r(NULL, " ", &saveptr);
    token2 = strtok_r(NULL, " ", &saveptr);

    if (strcmp(token0, "get") == 0) {
        out->type = CMD_GET;
        out->key = token1;
        out->value = NULL;
    } else if (strcmp(token0, "set") == 0) {
        out->type = CMD_SET;
        out->key = token1;
        out->value = token2;
    } else if (strcmp(token0, "del") == 0) {
        out->type = CMD_DEL;
        out->key = token1;
        out->value = NULL;
    } else if (strcmp(token0, "list") == 0) {
        out->type = CMD_LIST;
        out->key = NULL;
        out->value = NULL;
    } else if (strcmp(token0, "quit") == 0) {
        out->type = CMD_QUIT;
        out->key = NULL;
        out->value = NULL;
    } else {
        out->type = CMD_UNKNOWN;
        return 0;   // parsing failed
    }

    return 1;
}

// allocate & initialize a DB with some initial capacity
int db_init(struct database *db, int dbCapacity) {
    return 1;
}

// free all memory owned by the DB (keys, values, entries array)
void db_free(struct database *db);

// set key -> value (insert or update)
int db_set(struct database *db, const char *key, const char *value);

// get value for key (returns pointer or NULL if not found)
const char *db_get(struct database *db, const char *key);

// delete key (returns 1 if found+deleted, 0 if not found)
int db_del(struct database *db, const char *key);

// list keys into an array you pass in, or return count
int db_list(const struct database *db, const char **keys_out, int max_keys);
//where does keys_out and max_keys live? stack array? dynamically allocated? 
//are they copies of keys or pointers into the db?

int main() {

    // call getInput()
    char line[MAXCAPACITY];
    printf("Enter input: \n");
    while(getInput(line, sizeof(line))) {
        
        //parsing error
        if (!cmdParser(line, &cmd)) {

        }
    }

    /* db_list(db): return a list/array of keys that you can then bubble sort */
    
    return 0;
}
