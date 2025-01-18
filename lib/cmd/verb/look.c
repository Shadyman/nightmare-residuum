inherit STD_VERB;

varargs mixed do_look_at_obj (object ob, mixed arg);

void create () {
    verb::create();
    add_rules(({ "", "STR", "at STR", "OBJ", "at OBJ", "LIV", "at LIV", }));
    set_syntax("look (at) ([item|living])");
    set_help_text("Your character will attempt to look at the provided item or target.");
    set_help_similar(({ "listen", "smell", }));
}

/* ----- rule: "" ----- */

mixed can_look () {
    if (!environment(previous_object())) return "There is nothing.";
    else return 1;
}
mixed do_look () {
    previous_object()->describe_environment();
    return 1;
}

/* ----- rule: "STR" and "at STR" ----- */

mixed can_look_at_str (string str, string verb) {
    return 1;
}
mixed can_look_str (string str, string verb) {
    return can_look_at_str(str, verb);
}
void do_look_at_str (mixed args...) {
    object po = previous_object(), ob;
    /** @type {STD_ROOM} env */
    object env = environment(po);
    string str, desc;

    if (sizeof(args)) str = args[0];
    else return;

    if (!env->query_property("indoors") && member_array(str, ({"sky", "sun"})) > -1) {
        desc = D_ASTRONOMY->query_localsky(D_ASTRONOMY->query_astronomy_from_room(env), str);
    } else if (ob = present(str, env)) {
        // work around for parser not recognizing 'object 2' syntax
        return do_look_at_obj(ob);
    } else {
        desc = env->handle_look(str);
    }

    if (!desc) {
        message("action", "You do not see that here.", po);
    } else {
        message("action", desc, po);
        message("action", po->query_cap_name() + " looks over the " + str + ".", env, po);
    }
}
void do_look_str (mixed args...) {
    string str;

    if (sizeof(args)) str = args[0];

    do_look_at_str(str, str);
}

/* ----- rule: "OBJ" and "at OBJ" ----- */

mixed can_look_at_obj (string str, string verb) {
    return 1;
}
mixed can_look_obj (string str, string verb) {
    return can_look_at_obj(str, verb);
}
/**
 * Look at and describe a non-living item.
 *
 * @param {STD_ITEM} ob the non-living item to be described
 * @param arg any extra arguments provided by the driver parser (unused)
 */
varargs mixed do_look_at_obj (object ob, mixed arg) {
    object po = previous_object();
    string long = replace_string(ob->query_long(), "%^DEFAULT%^", "%^RESET%^");
    string type = sizeof(long) > __LARGEST_PRINTABLE_STRING__ / 2 ? "pager" : "action";

    // force pager mode
    if (strsrch(long, "#!PAGER!#") == 0) {
        long = long[9..];
        type = "pager";
    }

    message("action", "You look over " + ob->query_short() + "...", po);
    message(type, long, po);
    if (long = ob->query_long_footer()) {
        message("action", "\n" + long, po);
    }
    message("action", po->query_cap_name() + " looks over " + ob->query_name() + ".", environment(ob), po);

    return 1;
}
varargs mixed do_look_obj (object ob, mixed args...) {
    return do_look_at_obj(ob, args...);
}

/* ----- rule: "LIV" and "at LIV" ----- */

mixed can_look_at_liv (string str, string verb) {
    return 1;
}
mixed can_look_liv (string str, string verb) {
    return can_look_at_liv(str, verb);
}
/**
 * Look at and describe a living item.
 *
 * @param {STD_LIVING} ob the living item to be described
 * @param arg any extra arguments provided by the driver parser (unused)
 */
mixed do_look_at_liv (object ob, mixed arg...) {
    object po = previous_object();
    /** @type {STD_WEAPON*} wielded */
    object *wielded;
    /** @type {STD_ARMOR*} worn */
    object *worn;
    string str;

    message("action", "You look over " + ob->query_cap_name() + "...", po);
    message("action", po->query_cap_name() + " looks you over.", ob, po);
    message("action", po->query_cap_name() + " looks over " + ob->query_cap_name() + ".", environment(ob), ({ po, ob }));

    if (str = ob->query_long()) {
        if (sizeof(str) && str[<1] != '\n') str += "\n";
        str = replace_string(str, "%^DEFAULT%^", "%^RESET%^");
        message("action", str, po);
    }
    if (po->query_immortal()) {
        message("action", "Level: "+ob->query_level(), po);
        message("action", "Vitals: "+ob->query_hp()+"/"+ob->query_max_hp()+" "+ob->query_sp()+"/"+ob->query_max_sp()+" "+ob->query_mp()+"/"+ob->query_max_mp(), po);
    }

    if (sizeof(wielded = ob->query_wielded_weapons())) {
        message("action", capitalize(subjective(ob)) + " is wielding:", po);
        foreach (string weapon in wielded) {
            message("action", "  " + weapon->query_short(), po);
        }
    }
    if (sizeof(worn = ob->query_all_armor())) {
        message("action", capitalize(subjective(ob)) + " is wearing:", po);
        foreach (string armor in worn) {
            message("action", "  " + armor->query_short(), po);
        }
    }
    return 1;
}
mixed do_look_liv (object ob, mixed args...) {
    return do_look_at_liv(ob, args...);
}