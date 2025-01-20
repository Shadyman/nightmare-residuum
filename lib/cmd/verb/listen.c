inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "STR", }));
    set_syntax("listen ([sound])");
    set_help_text("Your character will attempt to listen to the provided sound.");
    set_help_similar(({ "look", "smell", }));
}

/* ----- rule: "" ----- */

mixed can_listen () {
    if (!environment(previous_object())) return "There is nothing.";
    else return 1;
}
mixed do_listen () {
    object po = previous_object();
    /** @type {STD_ROOM} env */
    object env = environment(po);

    if (env->query_listen("default")) {
        previous_object()->describe_environment_senses("listen");
    } else {
        message("action", "There is nothing to listen to.", po);
    }
    return 1;
}

/* ----- rule: "STR" and "at STR" ----- */

mixed can_listen_str (string str, string verb) {
    return 1;
}
void do_listen_str (mixed args...) {
    object po = previous_object();
    /** @type {STD_ROOM} env */
    object env = environment(po);
    string str;

    if (sizeof(args)) str = args[0];

    if (!str || !env->query_listen(str)) {
        message("action", "Listen to what?", po);
        return;
    }
    previous_object()->describe_environment_senses("listen", str);
}
