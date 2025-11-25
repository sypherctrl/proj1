#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCAPACITY 256

    //entry struct
    struct entry {
        char *key;
        char *value;
        int in_use;     //for a flag; 0 = empty, 1 = used
    };

    //database struct
    struct database {
        struct entry *ptr_to_arrEnt;      //dynamically allocated array
        int count;      //how many entries are actually used
        int capacity;   //how big the entries array is
        //use malloc/realloc/etc to grow it later
    };

    //define an enum for cmd types
    enum operationType {
            CMD_GET,
            CMD_SET,
            CMD_DEL,
            CMD_LIST,
            CMD_QUIT,
            CMD_UNKNOWN
    };

    //define a struct to hold a parsed cmd
    struct operation {
        enum operationType type;
        char *key;  //NULL if not needed
        char *value;    //NULL if not needed
    };

 struct operation cmd;


    

// for ONLY input reading
int getInput(char *buffer, int bufferSize) {
    /* 1. read line until \n (from stdin into buffer)
    ensure its a valid C string, error if not
    2. delete the \n, ensure \0 is at the end
    3. return 1 for success, return 0 if EOF or error
    */
    return 0;       //temp return value
}

int cmdParser(char *line, struct operation *out) {
    /* 1. Take *line as input.
    2. Split into space seperated tokens using strtok()
    token 0 --> cmd
    token 1 --> key
    token 2 --> value (may include spaces but thats later)
    3. compare the cmd (token 0) to known commands and set out-->type
    4. set out-> key and out->value (or NULL)
    5. return 1 for success, return 0 if parsing failed.
    */
    return 0;       //temp return value
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
    return 1;  

    //idk what its for??
    if (!db_init(&db, MAXCAPACITY)) {
    fprintf(stderr, "db_init failed\n");
    return 1;
    }
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
    //create database instance db
    struct database db;
  //hey db_init, heres the addr of my db database. plz go inside it & fill in the values.
    db_init(&db, MAXCAPACITY);
  
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
