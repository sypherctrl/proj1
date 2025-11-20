# proj1
A tiny in-memory database that speaks a super simple text protocol over TCP.
Concept
- Server listens on a port (PORT from env var, or -p from argv).
- Clients can send commands like:
SET key value
GET key
DEL key
LIST (returns all keys, sorted with bubble sort)
Data is stored in a hash table in memory.
- Each client connection is handled by a separate thread.

How it hits your checklist
Multi-file:
main.c – argument parsing, env vars, main loop.
kvstore.c / kvstore.h – hash table, get/set/del.
net.c / net.h – socket setup, accept loop, per-client handler.

Arrays / dynamic memory:
Dynamic array of buckets or a dynamically resized hash table.
Store keys/values as char * allocated with malloc.

File I/O:
On exit, dump the entire key-value store to store.db.
On startup, read store.db and restore state.

Pointers & advanced pointers:
struct kv_entry * linked lists or open-addressing arrays; pointer to functions for different commands.
typedef int (*command_handler)(const char *args, client_t *c);

Function pointers:
Command dispatch table: an array of { "SET", handler_set }, etc.
Multithreading:
One thread per connection or a small worker pool that handles queued client requests.

Hashing:
Implement your own hash_string(const char *s) using bitwise ops (e.g. shift/xor).
argc/argv / env vars:
./kvstore -p 8080 -f store.db

Env vars: KVSTORE_PORT, KVSTORE_MAX_CLIENTS.

Bitwise ops:
Flags on entries: e.g. uint8_t flags with bits like KV_FLAG_DIRTY, KV_FLAG_TOMBSTONE.
State machine:
Per-client state: enum client_state { STATE_RECV, STATE_PARSE, STATE_RESPOND, STATE_CLOSED };
In the client thread, switch based on state.

Valgrind: On shutdown, free every allocated key/value and hash bucket; then run with valgrind --leak-check=full ./kvstore.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Design Doc:

Protocol (text over TCP)
Commands and syntax:
SET key value
GET key
DEL key
LIST
QUIT
How lines are terminated (e.g., newline).

Error handling:
What if the client sends an unknown command?
What if the wrong number of arguments?

Limits:
Maximum key length: 15
Maximum value length: 65
Maximum number of items in the database: 100
Maximum number of clients: 8

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

In what order should you implement things?

1. Implement and test getInput.
Make sure it reads a line from stdin, strips the newline, and handles EOF/error correctly.
Print what you read back out to confirm it behaves as expected.

2. Implement and test cmdParser.
Feed it simple test strings (like "set x 10", "get x", "list") and print out:
what command type it detected
what it thinks key and value are
Don’t touch the database yet. Just make parsing correct and predictable.

3. Implement the basic database structure and lifecycle (db_init, db_free).
Create the database instance in main.
Call the init function once, free it at the end.
Maybe print some debug info to ensure counts/capacity look right.

4. Implement db_set, db_get, db_del for a simple linear search.
For now: no hashing, just loop over entries and compare keys.
Test them with hard-coded calls before wiring to the parser.

5. Wire everything together in main.
In the input loop:
call getInput
call cmdParser
use a switch on the command type to call the right db_ function and print responses.

6. Finally, implement db_list and bubble sort on the returned keys.
Use it to implement the LIST command.
After that, you’ll have a functioning little in-memory key–value REPL. Then you can start thinking about:
file persistence
sockets / TCP
threads




























