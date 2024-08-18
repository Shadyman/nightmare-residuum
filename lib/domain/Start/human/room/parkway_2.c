#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a wide village street");
    set_long("A wide street traveling northwest to southeast with green spaces instead of buildings. Birch trees branch over the street and open green spaces. Flower beds are prepared near street intersections. A cafe can be seen on the west side of the street. An obelisk can be seen to the southeast. A framework of hexagons glimmer far overhead.");
    set_listen("default", "Birds chirp from the trees.");
    set_smell("default", "Fresh air smelling of grass.");
    set_exits(([
        "northwest": HUMAN_ROOM + "park_nw.c",
        "southeast": HUMAN_ROOM + "parkway_1.c",
        "enter west": HUMAN_ROOM + "store_cafe.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled Parkway.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
        "cafe": "A brick dwelling serving as a cafe, as seen through the windows.",
    ]));
    set_reset(([
        HUMAN_ITEM + "artwork_2.c": 1,
        HUMAN_ITEM + "bench.c": 1,
        HUMAN_NPC + "ant.c": ([
            "number": !random(3),
        ]),
        HUMAN_NPC + "bee.c": ([
            "number": !random(3),
        ]),
        HUMAN_NPC + "beetle.c": ([
            "number": !random(3),
        ]),
        HUMAN_NPC + "squirrel.c": ([
            "number": !random(3),
        ]),
        HUMAN_NPC + "wasp.c": ([
            "number": !random(3),
        ]),
        HUMAN_NPC + "pigeon.c": ([
            "number": !random(2),
        ]),
        HUMAN_NPC + "sparrow.c": ([
            "number": !random(2),
        ]),
    ]));
}