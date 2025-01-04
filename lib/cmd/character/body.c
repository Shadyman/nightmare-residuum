inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("body");
    set_help_text("The body command is used to view the limbs of your character's body.");
}

void command (string input, mapping flags) {
    object tc = this_character(), target = tc;
    string *items = ({ }), *limbs, *severedLimbs;
    mapping footer;

    if (input && tc->query_immortal()) {
        target = determine_immortal_target(tc, input);
    }

    limbs = target->query_limbs();
    severedLimbs = target->query_severed_limbs();
    foreach (string type in ({
        "head", "torso", "thorax", "abdomen",
        "arm", "hand", "leg", "foot",
        "paw", "wing", "tail",
    })) {
        foreach (string l in sort_array(filter(limbs, (: regexp($1, $(type)) :)), 1)) {
            mapping limb = target->query_limb(l);
            items += ({
                l,
                limb["damage"]+"/"+limb["maxdamage"] + " " + (limb["damage"]*100/limb["maxdamage"])+"%",
                limb["type"] ? limb["type"] : "",
                limb["attach"] && member_array(limb["attach"], severedLimbs) == -1 ? limb["attach"] : "",
            });
            limbs -= ({ l });
        }
    }
    foreach (string l in limbs) {
        mapping limb = target->query_limb(l);
        items += ({
            l,
            limb["damage"]+"/"+limb["maxdamage"] + " " + (limb["damage"]*100/limb["maxdamage"])+"%",
            limb["type"] ? limb["type"] : "",
            limb["attach"] ? limb["attach"] : "",
        });
    }

    if (sizeof(severedLimbs)) {
        footer = ([
            "items": ({
                "You are missing your " + conjunction(severedLimbs) + ".",
            }),
            "columns": 1,
            "align": "center",
        ]);
    }

    border(([
        "title": "BODY",
        "subtitle": target->query_cap_name(),
        "header": ([
            "items": ({
                "You have the body of a " + target->query_species(),
            }),
            "columns": 1,
            "align": "center",
        ]),
        "body": ([
            "header": ({ "Limb", "Damage", "Type", "Attached" }),
            "items": items,
            "columns": 4,
        ]),
        "footer": footer,
    ]));
}