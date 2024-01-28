#include "human.h"

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village cross-street");
    set_long("A small street traveling west to east through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. An obelisk can be seen to the east. A framework of hexagons glimmer overhead.");
    set_exits(([
        "east": HUMAN_ROOM + "market_st_w2.c",
        "west": HUMAN_ROOM + "market_st_w4.c",
        "north": HUMAN_ROOM + "second_st_n1.c",
        "south": HUMAN_ROOM + "second_st_s1.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled W. Market St. and Second St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}