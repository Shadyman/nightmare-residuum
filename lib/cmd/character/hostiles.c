inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("hostiles");
    set_help_text("The hostiles command is used to view the list of things that are in combat with your character, even if they are in another location.");
}

void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    string *items = ({ });

    if (input && tc->query_immortal()) {
        target = determine_immortal_target(tc, input);
    }

    foreach (object ob in target->query_hostiles()) {
        items += ({ ob->query_cap_name() });
    }

    border(([
        "title": "HOSTILES",
        "subtitle": target->query_cap_name(),
        "body": ([
            "items": sizeof(items) ? items : ({ "No current hostiles." }),
            "columns": sizeof(items) ? 2 : 1,
            "align": sizeof(items) ? "left" : "center",
        ]),
    ]));
}