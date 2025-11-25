#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCAPACITY 256

// Structures
struct entry {
    char *key;
    char *value;
    int in_use;   // 0 = empty, 1 = used
};

struct database {
    struct entry *ptr_to_arrEnt;
    int count;
    int capacity;
};

enum operationType { CMD_GET, CMD_SET, CMD_DEL, CMD_LIST, CMD_QUIT, CMD_UNKNOWN };

struct operation {
    enum operationType type;
    char *key;      // or NULL
    char *value;    // or NULL
};

// Input reader
int getInput(char *buffer, int bufferSize) {
    if (!buffer || bufferSize <= 1) return 0;

    if (!fgets(buffer, bufferSize, stdin))
        return 0; // EOF or error

    size_t len = strlen(buffer);

    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        return 1;
    }

    // flush overflow
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
    return 0;
}

// Command parser
int cmdParser(char *line, struct operation *out) {
    out->type = CMD_UNKNOWN;
    out->key = NULL;
    out->value = NULL;

    char *token0 = strtok(line, " ");
    if (!token0) return 0;

    char *token1 = strtok(NULL, " ");
    char *token2 = strtok(NULL, ""); // remainder

    // Identify command
    if (strcmp(token0, "get") == 0) {
        out->type = CMD_GET;
        out->key = token1;
    }
    else if (strcmp(token0, "set") == 0) {
        out->type = CMD_SET;
        out->key = token1;
        out->value = token2;
    }
    else if (strcmp(token0, "del") == 0) {
        out->type = CMD_DEL;
        out->key = token1;
    }
    else if (strcmp(token0, "list") == 0) {
        out->type = CMD_LIST;
    }
    else if (strcmp(token0, "quit") == 0) {
        out->type = CMD_QUIT;
    }
    else {
        out->type = CMD_UNKNOWN;
    }

    return 1;
}

// DB init + free
int db_init(struct database *db, int capacity) {
    db->count = 0;
    db->capacity = capacity;

    db->ptr_to_arrEnt = calloc(capacity, sizeof(struct entry));
    if (!db->ptr_to_arrEnt) {
        fprintf(stderr, "calloc failed\n");
        return 0;
    }

    return 1;
}

void db_free(struct database *db) {
    if (!db || !db->ptr_to_arrEnt) return;

    for (int i = 0; i < db->capacity; i++) {
        struct entry *e = &db->ptr_to_arrEnt[i];
        if (e->in_use) {
            free(e->key);
            free(e->value);
        }
    }

    free(db->ptr_to_arrEnt);
    db->ptr_to_arrEnt = NULL;
    db->count = 0;
    db->capacity = 0;
}

// db_set (insert/update)
int db_set(struct database *db, const char *key, const char *value) {
    if (!db || !key || !value) return 0;

    struct entry *found = NULL;

    // Search existing key
    for (int i = 0; i < db->capacity; i++) {
        struct entry *e = &db->ptr_to_arrEnt[i];
        if (e->in_use && strcmp(e->key, key) == 0) {
            found = e;
            break;
        }
    }

    // UPDATE PATH 
    if (found) {
        char *newVal = malloc(strlen(value) + 1);
        if (!newVal) return 0;

        strcpy(newVal, value);

        free(found->value);
        found->value = newVal;

        return 1;
    }

    // INSERT PATH 

    if (db->count == db->capacity)
        return 0;

    struct entry *slot = NULL;

    for (int i = 0; i < db->capacity; i++) {
        if (!db->ptr_to_arrEnt[i].in_use) {
            slot = &db->ptr_to_arrEnt[i];
            break;
        }
    }

    if (!slot) return 0;

    char *key_copy = malloc(strlen(key) + 1);
    char *val_copy = malloc(strlen(value) + 1);
    if (!key_copy || !val_copy) {
        free(key_copy);
        free(val_copy);
        return 0;
    }

    strcpy(key_copy, key);
    strcpy(val_copy, value);

    slot->key = key_copy;
    slot->value = val_copy;
    slot->in_use = 1;

    db->count++;
    return 1;
}

// db_get
const char *db_get(struct database *db, const char *key) {
    if (!db || !key) return NULL;

    for (int i = 0; i < db->capacity; i++) {
        struct entry *e = &db->ptr_to_arrEnt[i];
        if (e->in_use && strcmp(e->key, key) == 0)
            return e->value;
    }
    return NULL;
}

// db_del
int db_del(struct database *db, const char *key) {
    if (!db || !key) return 0;

    for (int i = 0; i < db->capacity; i++) {
        struct entry *e = &db->ptr_to_arrEnt[i];
        if (e->in_use && strcmp(e->key, key) == 0) {

            free(e->key);
            free(e->value);

            e->key = NULL;
            e->value = NULL;
            e->in_use = 0;

            db->count--;
            return 1;
        }
    }
    return 0;
}

// db_list
int db_list(const struct database *db, const char **keys_out, int max_keys) {
    int count = 0;

    for (int i = 0; i < db->capacity && count < max_keys; i++) {
        if (db->ptr_to_arrEnt[i].in_use) {
            keys_out[count++] = db->ptr_to_arrEnt[i].key;
        }
    }

    return count;
}

// MAIN LOOP
int main() {
    struct database db;
    struct operation cmd;

    if (!db_init(&db, MAXCAPACITY)) {
        fprintf(stderr, "db_init failed\n");
        return 1;
    }

    char line[MAXCAPACITY];

    printf("MiniDB ready. Commands: set/get/del/list/quit\n");

    while (getInput(line, sizeof(line))) {
        if (!cmdParser(line, &cmd)) {
            printf("Parse error\n");
            continue;
        }

        if (cmd.type == CMD_QUIT)
            break;

        switch (cmd.type) {
        case CMD_SET:
            if (!cmd.key || !cmd.value) {
                printf("Usage: set <key> <value>\n");
                break;
            }
            if (db_set(&db, cmd.key, cmd.value))
                printf("OK\n");
            else
                printf("ERR\n");
            break;

        case CMD_GET: {
            if (!cmd.key) {
                printf("Usage: get <key>\n");
                break;
            }
            const char *val = db_get(&db, cmd.key);
            if (val) printf("%s\n", val);
            else     printf("(null)\n");
            break;
        }

        case CMD_DEL:
            if (!cmd.key) {
                printf("Usage: del <key>\n");
                break;
            }
            if (db_del(&db, cmd.key))
                printf("Deleted\n");
            else
                printf("Not found\n");
            break;

        case CMD_LIST: {
            const char *keys[256];
            int n = db_list(&db, keys, 256);
            for (int i = 0; i < n; i++)
                printf("%s\n", keys[i]);
            break;
        }

        default:
            printf("Unknown command\n");
            break;
        }
    }

    db_free(&db);
    return 0;
}
