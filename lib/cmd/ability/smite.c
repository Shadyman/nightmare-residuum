#include <verb.h>

inherit STD_ABILITY;

void create () {
    ::create();
    set_type("attack");
    set_ability_requirements(([
        "paladin": ([
            "level": 1,
        ])
    ]));
    set_skill_powers(([
        "brawl": 15,
        "psionic": 5,
    ]));
    set_help_text("Smite your opponent.");
}