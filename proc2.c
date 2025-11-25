#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCAPACITY 256

    //entry struct
    struct entry { char *key; char *value; int in_use; };    //for a flag; 0 = empty, 1 = used 

    //database struct
    struct database {
        struct entry *ptr_to_arrEnt;      //dynamically allocated array
        int count;      //how many entries are actually used
        int capacity;   //how big the entries array is
        //use malloc/realloc/etc to grow it later
    };

    //define an enum for cmd types
    enum operationType { CMD_GET, CMD_SET, CMD_DEL, CMD_LIST, CMD_QUIT, CMD_UNKNOWN };

    //define a struct to hold a parsed cmd
    struct operation {
        enum operationType type;
        char *key;  //NULL if not needed
        char *value;    //NULL if not needed
    };


// for ONLY input reading
int getInput(char *buffer, int bufferSize) {
    /* 1. read line until \n (from stdin into buffer)
    ensure its a valid C string, error if not
    2. delete the \n, ensure \0 is at the end
    3. return 1 for success, return 0 if EOF or error */
    // can't store anything useful if buffer is too small
    if (buffer == NULL || bufferSize <= 1) {
        return 0;
    }
    if (fgets(buffer, bufferSize, stdin) == NULL) {     // NULL being eof or error
        return 0;
    }

    // at this point buffer is a valid C string
    size_t len = strlen(buffer);
    
    // if we got a newline, strip it
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        return 1;
    }
    // if there's no newline, the line was longer than bufferSize-1.
    // flush the rest of the line so it doesn't mess up the next read.
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    // treat this as an error (input too long for buffer)
    return 0;
}

int cmdParser(char *line, struct operation *out) {
    /* 1. Take *line as input.
    2. Split into space seperated tokens using strtok()
    token 0 --> cmd
    token 1 --> key
    token 2 --> value (may include spaces but thats later)
    3. compare the cmd (token 0) to known commands and set out-->type
    4. set out-> key and out->value (or NULL)
    5. return 1 for success, return 0 if parsing failed   */
    // initialize output
    out->key = NULL;
    out->value = NULL;
    out->type = CMD_UNKNOWN;

    // tokenize in-place
    char *token0 = strtok(line, " ");
    char *token1 = strtok(NULL, " ");
    char *token2 = strtok(NULL, ""); // rest of line (may be NULL)

    if (token0 == NULL) {
        return 0; // no command
    }

    // determine type
    if (strcmp(token0, "get") == 0) {
        out->type = CMD_GET;
        out->key = token1;
    } else if (strcmp(token0, "set") == 0) {
        out->type = CMD_SET;
        out->key = token1;
        out->value = token2;
    } else if (strcmp(token0, "del") == 0) {
        out->type = CMD_DEL;
        out->key = token1;
    } else if (strcmp(token0, "list") == 0) {
        out->type = CMD_LIST;
    } else if (strcmp(token0, "quit") == 0) {
        out->type = CMD_QUIT;
    } else {
        out->type = CMD_UNKNOWN;
    }

    // basic validation (tighten later)
    return 1;
}

// allocate & initialize a DB with some initial capacity
int db_init(struct database *db, int capacity) {
    db->count = 0;
    db->capacity = capacity;

  //create an instance of entry structure called arrEnt (array of Entries) on heap
   struct entry *arrEnt = (struct entry *)calloc(capacity, sizeof(struct entry));

    //if calloc fails, arrEnt will be NULL
    if (arrEnt == NULL) {
        printf("calloc failed, arrEnt is NULL\n");
        return 0;
    }
   
  // if calloc succeeds, then:
  //store the addr of arrEnt in db->ptr_to_arrEnt. THIS IS THE SAME MEMORY AS THE DB IN MAIN
   db->ptr_to_arrEnt = arrEnt;
  
  // iterate through each element in arrEnt
  for (int i = 0; i < capacity; i++) {
    // set every slot as empty
    db->ptr_to_arrEnt[i].in_use = 0;
    db->ptr_to_arrEnt[i].key = NULL;
    db->ptr_to_arrEnt[i].value = NULL;
    }
    fprintf(stderr, "db_init: capacity=%d\n", capacity);   //for debug
    return 1;  
}

// free all memory owned by the DB (keys, values, entries array)
void db_free(struct database *db) {
    (void)db;
    //free any per-entry strings you allocate, and then free db->ptr_to_arrEnt
}

/* 
Option A: Design db_set logically (insert/update behavior, key search, capacity handling),
Option B: Make getInput actually read from stdin,
Option C: Flesh out cmdParser so you can type set foo bar and see it parsed,
*/

// set key -> value (update or insert)
int db_set(struct database *db, const char *key, const char *value) {
    struct entry *found = NULL;

    if (db == NULL) {
        printf("db pointer is NULL, failed db_set\n");
        return 0;
    } else if (key == NULL) {
        printf("key is NULL, failed db_set\n");
        return 0;
    } else {
        // 1. search for existing key. 
        // loop over all entries
        for (int i = 0; i < db->capacity; i++) {
        // e points to actual entry in database array
        struct entry *e = &db->ptr_to_arrEnt[i];   
            
        if(e->in_use == 0) continue;    //find existing key 
        if (e->in_use == 1 && strcmp(e->key, key) == 0) { found = e; break; }
        } 
    }

    if (found != NULL) {
        printf("UPDATE PATH\n");
        if (value == NULL) { printf("Value is NULL in Update Path\n"); return 0; }
        else {  // validate value
            int value_len = strlen(value);
            char *value_copy = malloc(value_len + 1);    // allocate new heap buffer for the new value
                    // if allocation fails, return 0 (do not change the entry)
                    if (!value_copy == NULL) {
                    printf("Update Path memory allocation failed.\n");
                    return 0;
                    } 
                    memcpy(value_copy, value, value_len + 1);
                
                    // free the old found->value if it exists
                    if ((found->value) != NULL) {
                        free(found->value);
                    } found->value = value_copy;
                    return 1;   // success if update succeeds
                }
            }
        } else {
        printf("INSERT PATH\n");
        if (db->count == db->capacity) {
            printf("db_set insert failed: database full\n");
            return 0;
        } else {
            // find a free spot
            struct entry *free_slot = NULL;
            for (int i = 0; i < db->capacity; i++) {
                // e points to actual entry in database array
                struct entry *e = &db->ptr_to_arrEnt[i];
                if (e->in_use == 0) {
                    free_slot = e;
                    break;
                }
            }
            if (free_slot == NULL) {
                printf("db_set insert: no free slot found but db-> count < capacity\n");
                return 0;
            }
            if (value == NULL) { printf("Insert path: value is NULL.\n"); return 0; }

            // allocate heap copy of key
            int key_len = strlen(key);
            char *key_copy;
            key_copy = (char *)malloc(key_len + 1);    // allocate new heap buffer for the new key
            // if allocation fails, return 0 (do not change the entry)
            if (key_copy == NULL) {
            printf("Insert path: key allocation failed\n");
            return 0;
            } else {
                // copy string from key into key_copy (no '\0')
                for (int j = 0; j < key_len; j++) {
                    key_copy[j] = key[j];
                }
                key_copy[key_len] = '\0';
            }

            //allocate heap copy of value
            int value_len = strlen(value);
            char *value_copy;
            value_copy = (char *)malloc(value_len + 1);    // allocate new heap buffer for the new key
            // if allocation fails, return 0 (do not change the entry)
            if (value_copy == NULL) {
                printf("Insert path: value allocation failed\n");
                // free key_copy to avoid a leak, then return 0
                free(key_copy);
                return 0;
            } else { 
                // copy string from value into value_copy
                for (int j = 0; j < value_len; j++) {
                    value_copy[j] = value[j];
                }
            value_copy[value_len] = '\0';

            free_slot->key = key_copy;
            free_slot->value = value_copy;
            free_slot->in_use = 1;
            db->count++;
            return 1;  //return success
        }
    }
}


// get value for key (returns pointer or NULL if not found)
const char *db_get(struct database *db, const char *key){
    (void)db; (void)key;
    return NULL;
}

// delete key (returns 1 if found+deleted, 0 if not found)
int db_del(struct database *db, const char *key){
    (void)db; (void)key;
    return 0;
}

// list keys into an array you pass in, or return count
int db_list(const struct database *db, const char **keys_out, int max_keys) {
    (void)db; (void)keys_out; (void)max_keys;
    return 0;
//where does keys_out and max_keys live? stack array? dynamically allocated? 
//are they copies of keys or pointers into the db?
}

int main() {
    struct operation cmd; //in main bc only main uses cmd
    struct database db;
  //hey db_init, heres the addr of my db database. plz go inside it & fill in the values.
    if (db_init(&db, MAXCAPACITY) == 0) {       //initializes the database
        fprintf(stderr, "db_init failed\n");
        return 1;
    } 
    //if we reach here, db_init was successful
    // call getInput() to enter input loop
    char line[MAXCAPACITY];

    while(getInput(line, sizeof(line))) {
        //parsing error
        if (!cmdParser(line, &cmd)) {
        printf("Parse error\n");
        continue;
        }
        printf("Parsed cmd type=%d, key=\"%s\", value=\"%s\"\n",
           cmd.type,
           cmd.key ? cmd.key : "(null)",
           cmd.value ? cmd.value : "(null)");
    }

    /* db_list(db): return a list/array of keys that you can then bubble sort */
    
    return 0;       //success
}
