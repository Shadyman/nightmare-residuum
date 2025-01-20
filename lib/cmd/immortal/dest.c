inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("dest [object id]|[object instance]");
    set_help_text("The dest command is used to destroy a cloned copy of an object by id or using the object instance.");
}

void command (string input, mapping flags) {
    object ob;
    string name;

    if (!input) {
        message("action", "Syntax: dest [ob]", this_character());
        return;
    }

    ob = present(input, this_character());
    if (!ob) {
        ob = present(input, environment(this_character()));
    }
    if (!ob) {
        ob = find_object(input);
    }
    if (ob) {
        if (livingp(ob)) {
            name = ob->query_cap_name();
        } else if (ob->is_room()) {
            name = ob->query_short();
        } else {
            name = ob->query_name();
        }
        message("action", "You dest " + name + ".", this_character());
        message("action", this_character()->query_cap_name() + " dests you.", ob);
        message("action", this_character()->query_cap_name() + " dests " + name + ".", environment(this_character()), ({ this_character(), ob }));
        if (!ob->handle_remove()) {
            message("action", "dest: cannot dest " + ob->query_name() + ".", this_character());
        }
    } else {
        message("action", "dest: cannot find " + input + ".", this_character());
    }
}
