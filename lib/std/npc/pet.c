inherit STD_NPC;
inherit M_SAVE;

private int __Following;

nosave private object __Owner;

void set_owner (object owner) {
    if (!objectp(owner)) {
        error("Bad argument 1 to follower->set_owner");
    }

    __Owner = owner;
    set_heart_beat(1);
}
object query_owner () {
    return __Owner;
}
string query_owner_name () {
    if (__Owner) {
        return __Owner->query_key_name();
    } else {
        return 0;
    }
}

void set_following (int i) {
    if (!intp(i)) {
        error("Bad argument 1 to follower->set_following");
    }
    __Following = i;
}
int query_following () {
    return __Following;
}

/* ----- applies ----- */

void heart_beat () {
    ::heart_beat();

    if (__Owner) {
        if (__Following && environment() != environment(__Owner)) {
            message("action", query_cap_name() + " leaves to follow " + __Owner->query_cap_name() + ".", environment(), this_object());
            this_object()->handle_move(environment(__Owner));
            message("action", query_cap_name() + " arrives to follow you.", __Owner);
            message("action", query_cap_name() + " arrives to follow " + __Owner->query_cap_name() + ".", environment(), ({ this_object(), __Owner }));
        }
        if (present_hostile(__Owner) && !present_hostile(this_object())) {
            handle_command("aid " + __Owner->query_key_name());
        }
    }
}

mixed direct_lead_liv (mixed args...) {
    object po = previous_object();
    return environment() == environment(po) && po != this_object();
}
mixed direct_ditch_liv (mixed args...) {
    object po = previous_object();
    return environment() == environment(po) && po != this_object();
}