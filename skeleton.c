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
