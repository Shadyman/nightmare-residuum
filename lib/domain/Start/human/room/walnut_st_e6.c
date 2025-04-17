#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling west through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead. A concrete wall at the base of the hexagon dome blocks further passage east.");
    set_exits(([
        "west": HUMAN_ROOM + "walnut_st_e5.c",
        "north": HUMAN_ROOM + "fifth_st_s2.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled E. Walnut St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
    set_reset(([
        HUMAN_NPC + "teen_suspicious.c": 1,
    ]));
}