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
