/* skeleton v1:

#include <stdio.h>

// for ONLY input reading
void getInput() {
    /* 1. read line until \n 
    2. delete the \n
    ensure its a valid C string, error if not
    3. return string to main
    */
}

void cmdParser() {
    /* 1. Take output of getInput() as input.
    2. Split into space seperated tokens
    first word --> cmd
    second word --> key
    third word --> value
    3. decide which cmd it is, return the cmd to main()*/
}

int main() {
    
    /* define a dynamically resized db object (multidimentional array? struct?).
    count how many entries are currently used
    count a max capacity
    */

    // call getInput()
    //input returned string as input for cmdParser()
    
    /* db_set(db, key, value): if key exists, replace its value. if key dosent exist && theyres room, add a new entry
    db_get(db, key): print key value. else error.
    db_del(db, key): mark key as empty ""
    db_list(db): return a list/array of keys that you can then bubble sort 
    
    */
    
    return 0;
}

----------------------------------------------------------------------------------------------------------------------------------

skeleton v2:
    
int main(void) {
    struct Database db;
    char line[256];
    struct Command cmd;

    // 1. initialize DB
    db_init(&db, 16); // for example

    // 2. main loop
    for (;;) {
        if (!getInput(line, sizeof(line))) {
            break; // EOF or error
        }

        if (!cmdParser(line, &cmd)) {
            // print parse error
            continue;
        }

        switch (cmd.type) {
            case CMD_GET:
                // use cmd.key -> db_get -> print result
                break;
            case CMD_SET:
                // use cmd.key, cmd.value -> db_set
                break;
            case CMD_DEL:
                // db_del(cmd.key)
                break;
            case CMD_LIST:
                // db_list -> bubble sort -> print keys
                break;
            case CMD_QUIT:
                // clean up and break out of loop
                break;
            case CMD_UNKNOWN:
            default:
                // "Unknown command"
                break;
        }
    }

    // 3. free DB
    db_free(&db);
    return 0;
}

----------------------------------------------------------------------------------------------------------------------------------
skeleton v3:
*/

    
#include <stdio.h>
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
int getInput(char *buffer, int bufferSize) {
    /* 1. read line until \n (from stdin into buffer)
    ensure its a valid C string, error if not
    2. delete the \n, ensure \0 is at the end
    3. return 1 for success, return 0 if EOF or error
    */
}

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
}

// allocate & initialize a DB with some initial capacity
int db_init(struct database *db, int MAXCAPACITY);

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

    while(getInput(line, sizeof(line))) {
        //parsing error
        if (!cmdParser(line, &cmd)) {

    }
    }

    /* db_list(db): return a list/array of keys that you can then bubble sort */
    
    return 0;
}













