inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("defeats");
    set_help_text("The defeats command is used to view the list of defeats your character has recorded.");
}

void command (string input, mapping flags) {
    object tc = this_character();
    object target = tc;
    string *items = ({ });

    if (input && tc->query_immortal()) {
        target = determine_immortal_target(tc, input);
    }

    foreach (mixed *defeat in target->query_defeat()) {
        items += ({ (defeat[0] ? defeat[0] : "Unknown"), ctime(defeat[1]) });
    }

    border(([
        "title": "DEFEATS",
        "subtitle": target->query_cap_name(),
        "body": ([
            "items": sizeof(items) ? items : ({ "No defeats." }),
            "columns": sizeof(items) ? 2 : 1,
            "align": sizeof(items) ? "left" : "center",
        ]),
    ]));
}