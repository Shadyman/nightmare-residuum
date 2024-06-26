#include <human.h>

inherit STD_ROOM;

void create () {
    ::create();
    set_short("a village street");
    set_long("A small street traveling north to south through brick dwellings covered in dark ivy. Several birch trees branch over the street and dwellings. A framework of hexagons glimmer overhead. A concrete wall supporting the hexagon dome stands at the edge of town.");
    set_exits(([
        "north": HUMAN_ROOM + "broad_st_s6.c",
        "enter south": HUMAN_ROOM + "broad_st_s8.c",
    ]));
    set_looks(([
        "street": "A street sign is labeled N. Broad St.",
        ({ "framework", "hexagons"}): "Far above, the hexagons form a geodesic dome.",
    ]));
}