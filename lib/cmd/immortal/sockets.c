inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("sockets");
    set_help_text("The sockets command is used to view the entire list of sockets.");
}

void command (string input, mapping flags) {
    string *sockets = ({ }), *netStats = ({ });

    foreach (mixed *s in socket_status()) {
        sockets += ({ s[0], s[1], s[2], s[3], s[4] });
    }

    foreach (string key, int value in filter(network_stats(), (: regexp($1, "socket") :))) {
        key = replace_string(key, " sockets", "");
        netStats += ({ sprintf("%20s", key) + " : " + value });
    }
    netStats = sort_array(netStats, (: strcmp(ltrim($1), ltrim($2)) :));

    border(([
        "title": "SOCKETS",
        "subtitle": mud_name(),
        "body": ([
            "header": ({ "Socket", "State", "Mode", "Local Address", "Owner" }),
            "items": sockets,
            "columns": 5,
        ]),
        "footer": ([
            "items": netStats,
            "columns": 2,
        ]),
    ]));
}