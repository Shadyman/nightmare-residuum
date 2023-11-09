inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/secure/sefun/format.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

private nosave string __ANSI = "on";
private nosave int __Width = 80;
mixed query_setting (string setting) {
    if (setting == "ansi") {
        return __ANSI;
    }
    if (setting == "width") {
        return __Width;
    }
}

void test_format_page () {
    string row;

    expect_function("format_page", testOb);

    expect("format_page handled width=80", (: ({
        assert(__Width, "==", 80),
        assert(testOb->format_page(({ "a", "b" }), 2), "regex", "^a +b +$"),
        assert(testOb->format_page(({ "a", "b", "c" }), 2), "regex", "^a +b +\nc +$"),
        assert(testOb->format_page(({ "a", "b", "c" }), 1), "regex", "^a +\nb +\nc +$"),
        assert(testOb->format_page(({ "a", "b", "c" }), 2), "regex", "^a +b +\nc +$"),
        assert(testOb->format_page(({ "a", "b", "c" }), 3), "regex", "^a +b +c +$"),
        assert(strlen(explode(testOb->format_page(({ "a", }), 2), "\n")[0]), "==", 80),
        assert(strlen(explode(testOb->format_page(({ "a", }), 2, 1), "\n")[0]), "==", 78),
        assert(strlen(explode(testOb->format_page(({ "a", }), 2, -1), "\n")[0]), "==", 82),
        assert(strlen(explode(testOb->format_page(({ "a", "b", "c" }), 2), "\n")[0]), "==", 80),
        assert(strlen(explode(testOb->format_page(({ "a", "b", "c" }), 2, 1), "\n")[0]), "==", 78),
        assert(strlen(explode(testOb->format_page(({ "a", "b", "c" }), 2, -1), "\n")[0]), "==", 82),
    }) :));

    expect("format_page handled width=60", (: ({
        assert(__Width = 60, "==", 60),
        assert(testOb->format_page(({ "a", "b" }), 2), "regex", "^a +b +$"),
        assert(testOb->format_page(({ "a", "b", "c" }), 2), "regex", "^a +b +\nc +$"),
        assert(testOb->format_page(({ "a", "b", "c" }), 1), "regex", "^a +\nb +\nc +$"),
        assert(testOb->format_page(({ "a", "b", "c" }), 3), "regex", "^a +b +c +$"),
        assert(testOb->format_page(({ "a", "b", "c" }), 2), "regex", "^a +b +\nc +$"),
        assert(strlen(explode(testOb->format_page(({ "a", }), 2), "\n")[0]), "==", 60),
        assert(strlen(explode(testOb->format_page(({ "a", }), 2, 1), "\n")[0]), "==", 58),
        assert(strlen(explode(testOb->format_page(({ "a", }), 2, -1), "\n")[0]), "==", 62),
        assert(strlen(explode(testOb->format_page(({ "a", "b", "c" }), 2), "\n")[0]), "==", 60),
        assert(strlen(explode(testOb->format_page(({ "a", "b", "c" }), 2, 1), "\n")[0]), "==", 58),
        assert(strlen(explode(testOb->format_page(({ "a", "b", "c" }), 2, -1), "\n")[0]), "==", 62),
    }) :));

    expect("format_page handled remainder of width/columns", (: ({
        assert(__Width = 80, "==", 80),
        assert(strlen(testOb->format_page(({ "a", "b", "c", "d", "e", }), 5, 4)), "==", 72), // remainder = 2
        assert(strlen(testOb->format_page(({ "a", "b", "c" }), 3, 0)), "==", 80), // remainder = 2
        assert(strlen(testOb->format_page(({ "1", "2", "3", "4", "5", "6", "7" }), 7, 0)), "==", 80), // remainder = 3
    }) :));

    expect("format_page trimmed longer strings when more than 1 column", (: ({
        assert(strlen(testOb->format_page(({ "111111111111111111111111111111", "222222222222222222222222222222", "333333333333333333333333333333", "444444444444444444444444444444", }), 4, 0)), "==", 80), // 30 * 4 = 120
        assert(strlen(testOb->format_page(({ "111111111111111111111111111111", "2", "3", "4", }), 4, 0)), "==", 80), // 30 + 25 * 3 = 85
    }) :));

    row = testOb->format_page(({"123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789"}), 1, 0, 0, 0);
    expect("format_page wrapped longer strings when 1 column with no ansi flag", (: ({
        assert($(row), "==", "123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 \n123456789 123456789                                                             \n"),
        assert(strlen($(row)), "==", 162), // 80 + 1 + 80 + 1
    }) :));

    row = testOb->format_page(({"123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789"}), 1, 0, 0, "256");
    expect("format_page wrapped longer strings when 1 column with ansi flag", (: ({
        assert($(row), "==", "123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 \e[0m\n123456789 123456789                                                             \n"),
        assert(strlen($(row)), "==", 166), // 80 + 4 + 1 + 80 + 1
    }) :));

    expect("format_page left aligns strings", (: ({
        assert(__Width = 20, "==", 20),
        assert(testOb->format_page(({ "1", "2", "3", "4" }), 4, 0), "==", "1    2    3    4    "),
        assert(testOb->format_page(({ "1", "2", "3", "4" }), 4, 0, 0), "==", "1    2    3    4    "),
        assert(testOb->format_page(({ "1" }), 1, 0, 0), "==", "1                   "),
        assert(testOb->format_page(({ "1", "2" }), 1, 0, 0), "==", "1                   \n2                   "),
    }) :));
    expect("format_page center aligns strings", (: ({
        assert(testOb->format_page(({ "1", "2", "3", "4" }), 4, 0, 1), "==", "  1    2    3    4  "),
        assert(testOb->format_page(({ "1", "2", "3", "4" }), 4, 0, 1), "==", "  1    2    3    4  "),
        assert(testOb->format_page(({ "1" }), 1, 0, 1), "==", "          1         "),
        assert(testOb->format_page(({ "1", "2" }), 1, 0, 1), "==", "          1         \n          2         "),
    }) :));

    expect("format_page handles column ratios", (: ({
        assert(testOb->format_page(({ "1", "2", "3" }), ({ 2, 1, 1 }), 0, 0), "==", "1         2    3    "),
        assert(testOb->format_page(({ "1", "2", "3", "4" }), ({ 2, 1, 1, 1 }), 0, 0), "==", "1       2   3   4   "),
        assert(testOb->format_page(({ "1", "2", "3" }), ({ 1, 1, 1 }), 0, 0), "==", "1     2     3       "), // remainder of 2

        assert(testOb->format_page(({ "1", "2", "3" }), ({ 2, 1, 1 }), 0, 1), "==", "     1      2    3  "),
        assert(testOb->format_page(({ "1", "2", "3", "4" }), ({ 2, 1, 1, 1 }), 0, 1), "==", "    1     2   3   4 "),
        assert(testOb->format_page(({ "1", "2", "3" }), ({ 1, 1, 1 }), 0, 1), "==", "   1     2     3    "), // remainder of 2
    }) :));

    expect("format_page displays ANSI strings with proper string length", (: ({
        // Items display properly left aligned
        assert(testOb->format_page(({ "%^RED%^Red text%^RESET%^", "%^BLUE%^Blue text%^RESET%^", }), 2), "==", "%^RED%^Red text%^RESET%^  %^BLUE%^Blue text%^RESET%^ "),
        // First item has a RESET appended
        assert(testOb->format_page(({ "%^RED%^No reset", "%^BLUE%^Blue text%^RESET%^", }), 2), "==", "%^RED%^No reset  %^RESET%^%^BLUE%^Blue text%^RESET%^ "),
        // First item is stripped of color due to exceeding column width
        assert(testOb->format_page(({ "%^RED%^Trimmed Red text%^RESET%^", "%^BLUE%^Blue text%^RESET%^", }), 2), "==", "Trimmed Re%^BLUE%^Blue text%^RESET%^ "),

        // Items display properly centered
        assert(testOb->format_page(({ "%^RED%^Red text%^RESET%^", "%^BLUE%^Blue text%^RESET%^", }), 2, 0, 1), "==", " %^RED%^Red text%^RESET%^  %^BLUE%^Blue text%^RESET%^"),
        assert(testOb->format_page(({ "%^RED%^Red%^RESET%^", "%^BLUE%^Blue%^RESET%^", }), 2, 0, 1), "==", "    %^RED%^Red%^RESET%^      %^BLUE%^Blue%^RESET%^   "),
    }) :));
}

void test_format_syntax () {
    expect_function("format_syntax", testOb);

    expect("format_syntax handles syntaxes with ANSI off", (: ({
        assert(__ANSI = "off", "==", "off"),
        assert(testOb->format_syntax("syntax"), "==", "<syntax>"),
        assert(testOb->format_syntax("verb [target] ([limb]) (with [thing])"), "==", "<verb [target] ([limb]) (with [thing])>"),
    }) :));

    expect("format_syntax handles syntaxes with ANSI on", (: ({
        assert(__ANSI = "on", "==", "on"),
        assert(testOb->format_syntax("syntax"), "==", "%^CYAN%^<syntax>%^RESET%^"),
        assert(strip_colour(testOb->format_syntax("syntax target")), "==", "<syntax target>"),
    }) :));
}

void test_format_exit_brief () {
    expect_function("format_exit_brief", testOb);

    expect("format_exit_brief handled exits", (: ({
        assert(testOb->format_exit_brief("north"), "==", "n"),
        assert(testOb->format_exit_brief("northeast"), "==", "ne"),
        assert(testOb->format_exit_brief("east"), "==", "e"),
        assert(testOb->format_exit_brief("southeast"), "==", "se"),
        assert(testOb->format_exit_brief("south"), "==", "s"),
        assert(testOb->format_exit_brief("southwest"), "==", "sw"),
        assert(testOb->format_exit_brief("west"), "==", "w"),
        assert(testOb->format_exit_brief("northwest"), "==", "nw"),
        assert(testOb->format_exit_brief("enter"), "==", "ent"),
        assert(testOb->format_exit_brief("out"), "==", "out"),
        assert(testOb->format_exit_brief("enter north"), "==", "ent n"),
        assert(testOb->format_exit_brief("up"), "==", "u"),
        assert(testOb->format_exit_brief("down"), "==", "d"),
        assert(testOb->format_exit_brief("n"), "==", "n"),
        assert(testOb->format_exit_brief("exit"), "==", "exit"),
    }) :));
}
void test_format_exit_verbose () {
    expect_function("format_exit_verbose", testOb);

    expect("format_exit_verbose handled exits", (: ({
        assert(testOb->format_exit_verbose("n"), "==", "north"),
        assert(testOb->format_exit_verbose("ne"), "==", "northeast"),
        assert(testOb->format_exit_verbose("e"), "==", "east"),
        assert(testOb->format_exit_verbose("se"), "==", "southeast"),
        assert(testOb->format_exit_verbose("s"), "==", "south"),
        assert(testOb->format_exit_verbose("sw"), "==", "southwest"),
        assert(testOb->format_exit_verbose("w"), "==", "west"),
        assert(testOb->format_exit_verbose("nw"), "==", "northwest"),
        assert(testOb->format_exit_verbose("ent"), "==", "enter"),
        assert(testOb->format_exit_verbose("out"), "==", "out"),
        assert(testOb->format_exit_verbose("ent n"), "==", "enter north"),
        assert(testOb->format_exit_verbose("u"), "==", "up"),
        assert(testOb->format_exit_verbose("d"), "==", "down"),
        assert(testOb->format_exit_verbose("north"), "==", "north"),
        assert(testOb->format_exit_verbose("exit"), "==", "exit"),
    }) :));
}
void test_format_exit_reverse () {
    expect_function("format_exit_reverse", testOb);

    expect("format_exit_reverse handled exits", (: ({
        assert(testOb->format_exit_reverse("south"), "==", "north"),
        assert(testOb->format_exit_reverse("southwest"), "==", "northeast"),
        assert(testOb->format_exit_reverse("west"), "==", "east"),
        assert(testOb->format_exit_reverse("northwest"), "==", "southeast"),
        assert(testOb->format_exit_reverse("north"), "==", "south"),
        assert(testOb->format_exit_reverse("northeast"), "==", "southwest"),
        assert(testOb->format_exit_reverse("east"), "==", "west"),
        assert(testOb->format_exit_reverse("southeast"), "==", "northwest"),
        assert(testOb->format_exit_reverse("out"), "==", "enter"),
        assert(testOb->format_exit_reverse("enter"), "==", "out"),
        assert(testOb->format_exit_reverse("enter north"), "==", "out south"),
        assert(testOb->format_exit_reverse("out north"), "==", "enter south"),
        assert(testOb->format_exit_reverse("down"), "==", "up"),
        assert(testOb->format_exit_reverse("up"), "==", "down"),
        assert(testOb->format_exit_reverse("exit"), "==", "exit"),
    }) :));
}

void test_format_stat_brief () {
    expect_function("format_stat_brief", testOb);

    expect("format_stat_brief handled exits", (: ({
        assert(testOb->format_stat_brief("strength"), "==", "str"),
        assert(testOb->format_stat_brief("perception"), "==", "per"),
        assert(testOb->format_stat_brief("endurance"), "==", "end"),
        assert(testOb->format_stat_brief("charisma"), "==", "cha"),
        assert(testOb->format_stat_brief("intelligence"), "==", "int"),
        assert(testOb->format_stat_brief("agility"), "==", "agi"),
        assert(testOb->format_stat_brief("luck"), "==", "lck"),
        assert(testOb->format_stat_brief("unknown"), "==", ""),
        assert(testOb->format_stat_brief("str"), "==", "str"),
        assert(testOb->format_stat_brief("per"), "==", "per"),
        assert(testOb->format_stat_brief("end"), "==", "end"),
        assert(testOb->format_stat_brief("cha"), "==", "cha"),
        assert(testOb->format_stat_brief("int"), "==", "int"),
        assert(testOb->format_stat_brief("agi"), "==", "agi"),
        assert(testOb->format_stat_brief("lck"), "==", "lck"),
    }) :));
}
void test_format_stat_verbose () {
    expect_function("format_stat_verbose", testOb);

    expect("format_stat_brief handled exits", (: ({
        assert(testOb->format_stat_verbose("str"), "==", "strength"),
        assert(testOb->format_stat_verbose("per"), "==", "perception"),
        assert(testOb->format_stat_verbose("end"), "==", "endurance"),
        assert(testOb->format_stat_verbose("cha"), "==", "charisma"),
        assert(testOb->format_stat_verbose("int"), "==", "intelligence"),
        assert(testOb->format_stat_verbose("agi"), "==", "agility"),
        assert(testOb->format_stat_verbose("lck"), "==", "luck"),
        assert(testOb->format_stat_verbose("unknown"), "==", ""),
        assert(testOb->format_stat_verbose("strength"), "==", "strength"),
        assert(testOb->format_stat_verbose("perception"), "==", "perception"),
        assert(testOb->format_stat_verbose("endurance"), "==", "endurance"),
        assert(testOb->format_stat_verbose("charisma"), "==", "charisma"),
        assert(testOb->format_stat_verbose("intelligence"), "==", "intelligence"),
        assert(testOb->format_stat_verbose("agility"), "==", "agility"),
        assert(testOb->format_stat_verbose("luck"), "==", "luck"),
    }) :));
}

void test_format_integer () {
    expect_function("format_integer", testOb);

    expect("format_integer handled integers", (: ({
        // positives
        assert(testOb->format_integer(0), "==", "0"),
        assert(testOb->format_integer(1), "==", "1"),
        assert(testOb->format_integer(123), "==", "123"),
        assert(testOb->format_integer(1234), "==", "1,234"),
        assert(testOb->format_integer(123456), "==", "123,456"),
        assert(testOb->format_integer(1234567), "==", "1,234,567"),
        assert(testOb->format_integer(1234567890), "==", "1,234,567,890"),

        // negatives
        assert(testOb->format_integer(-0), "==", "0"),
        assert(testOb->format_integer(-1), "==", "-1"),
        assert(testOb->format_integer(-1234), "==", "-1,234"),
        assert(testOb->format_integer(-123456789), "==", "-123,456,789"),
        assert(testOb->format_integer(-1234567890), "==", "-1,234,567,890"),
    }) :));
}