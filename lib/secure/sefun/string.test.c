inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/secure/sefun/string.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

private nosave string __ANSI = "on";
mixed query_setting (string setting) {
    if (setting == "ansi") {
        return __ANSI;
    }
}

class TestClass {
    string  str;
    string *strArr;
    int     i;
    float   f;
    mapping m;
    object  o;
}
class TestClass tc = new(class TestClass);

void test_identify () {
    int tInt = 123, undefInt;
    float tFloat = 123.0, undefFloat;
    object tOb, undefOb;
    string tString = "Here it is: \"abc123\".", undefStr;
    mapping tMap = ([ "test1": "abc", "test2": 123 ]), undefMap;
    function tFn = function(int a, int b) { return a + b; }, undefFn;

    expect_function("identify", testOb);

    expect("identify handles undefined", (: ({
        assert(testOb->identify(), "==", "UNDEFINED"),
        assert(testOb->identify($(undefInt)), "==", "UNDEFINED"),
        assert(testOb->identify($(undefFloat)), "==", "UNDEFINED"),
        assert(testOb->identify($(undefOb)), "==", "UNDEFINED"),
        assert(testOb->identify($(undefStr)), "==", "UNDEFINED"),
        assert(testOb->identify($(undefMap)), "==", "UNDEFINED"),
        assert(testOb->identify($(undefFn)), "==", "UNDEFINED"),
    }) :));

    expect("identify handles int", (: ({
        assert(testOb->identify(1), "==", "1"),
        assert(testOb->identify(0), "==", "0"),
        assert(testOb->identify(-1), "==", "-1"),
        assert(testOb->identify(MAX_INT), "==", "9223372036854775807"),
        assert(testOb->identify(MIN_INT), "==", "-9223372036854775807"),
        assert(testOb->identify($(tInt)), "==", "123"),
    }) :));

    expect("identify handles float", (: ({
        assert(testOb->identify(1.0), "==", "1.000000"),
        assert(testOb->identify(0.0), "==", "0.000000"),
        assert(testOb->identify(-1.0), "==", "-1.000000"),
        assert(testOb->identify(MAX_FLOAT), "==", "179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000"),
        assert(testOb->identify(MIN_FLOAT), "==", "0.000000"),
        assert(testOb->identify($(tFloat)), "==", "123.000000"),
    }) :));

    tOb = new(STD_OBJECT);
    expect("identify handles object", (: ({
        assert(testOb->identify($(tOb)), "regex", "OBJ\\("+replace_string(STD_OBJECT[0..<3], "/", "\\/")+"#(.+)\\)"),
    }) :));
    destruct(tOb);

    expect("identify handles string", (: ({
        assert(testOb->identify("abc"), "==", "\"abc\""),
        assert(testOb->identify("123"), "==", "\"123\""),
        assert(testOb->identify("!@#"), "==", "\"!@#\""),
        assert(testOb->identify($(tString)), "==", "\"Here it is: \\\"abc123\\\".\""),
    }) :));

    expect("identify handles map", (: ({
        assert(testOb->identify(([ ])), "==", "([ ])"),
        assert(testOb->identify((["key1":"value1","key2":"value2",])), "==", "([ \"key1\": \"value1\", \"key2\": \"value2\" ])"),
        assert(testOb->identify(([0:1,1:2,])), "==", "([ 0: 1, 1: 2 ])"),
        assert(testOb->identify($(tMap)), "==", "([ \"test1\": \"abc\", \"test2\": 123 ])"),
    }) :));

    expect("identify handles function", (: ({
        assert(testOb->identify(function() {}), "==", "(: <code>() :)"),
        assert(testOb->identify((: uptime :)), "==", "(: uptime :)"),
        assert(testOb->identify((: $1 + $2 :)), "==", "(: <code>($1, $2) :)"),
        assert(testOb->identify((: call_other, this_object(), "query_name" :)), "==", "(: call_other, " + file_name() + ", \"query_name\" :)"),
        assert(testOb->identify($(tFn)), "==", "(: <code>($1, $2) :)"),
    }) :));

    expect("identify handles class", (: ({
        assert(testOb->identify(tc), "==", "CLASS( 6 elements  0,  0,  0,  0,  0,  0 )"),
        tc->str = "test string",
        tc->strArr = ({ "test string 1", "test string 2", }),
        tc->i = 123,
        tc->f = 1.0,
        tc->m = ([ "test": 123, ]),
        tc->o = this_object(),
        assert(testOb->identify(tc), "==", "CLASS( 6 elements  \"test string\",  ({ /* sizeof() == 2 */    \"test string 1\",    \"test string 2\"  }),  123,  1.000000,  ([ /* sizeof() == 1 */    \"test\" : 123,  ]),  " + file_name() + " )"),
    }) :));

    expect("identify handles array", (: ({
        assert(testOb->identify(({ 1, 2, 3})), "==", "({ 1, 2, 3 })"),
    }) :));

    expect("identify handles buffer", (: ({
        assert(testOb->identify(read_buffer("/secure/sefun/string.test.c", 0, 8)), "==", "BUFFER (0x69, 0x6E, 0x68, 0x65, 0x72, 0x69, 0x74, 0x20)"),
    }) :));
}

void test_wrap () {
    string resetANSI = "\e[0m", linewrap = "\n";

    expect_function("wrap", testOb);

    expect("wrap handles wrapping text", (: ({
        assert(this_object()->query_setting("ansi"), "==", "on"),

        assert(testOb->wrap("test", 80), "==", "test"),
        assert(testOb->wrap("testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttest"), "==", "testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttest" + $(resetANSI) + $(linewrap) + "test"),
        assert(testOb->wrap("testtest", 4), "==", "test" + $(resetANSI) + $(linewrap) + "test"),
        assert(testOb->wrap("testtesttest", 10), "==", "testtestte" + $(resetANSI) + $(linewrap) + "st"),
        assert(testOb->wrap("testtesttest", 10, 2), "==", "testtestte" + $(resetANSI) + $(linewrap) + "  st"),
        assert(testOb->wrap("", 80), "==", ""),
        assert(testOb->wrap("test", -10), "==", "test"),
        assert(testOb->wrap("%^BOLD_OFF%^test", 80), "==", "\e[22mtest"),
        assert(testOb->wrap("%^RED%^test%^RESET%^", 80), "==", "\e[31mtest" + $(resetANSI)),

        assert(__ANSI = "off", "==", "off"),

        assert(testOb->wrap("test", 80), "==", "test"),
        assert(testOb->wrap("testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttest"), "==", "testtesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttesttest" + $(linewrap) + "test"),
        assert(testOb->wrap("testtest", 4), "==", "test" + $(linewrap) + "test"),
        assert(testOb->wrap("testtesttest", 10), "==", "testtestte" + $(linewrap) + "st"),
        assert(testOb->wrap("testtesttest", 10, 2), "==", "testtestte" + $(linewrap) + "  st"),
        assert(testOb->wrap("", 80), "==", ""),
        assert(testOb->wrap("test", -10), "==", "test"),
        assert(testOb->wrap("%^BOLD_OFF%^test", 80), "==", "test"),
        assert(testOb->wrap("%^RED%^test%^RESET%^", 80), "==", "test"),
    }) :));
}

void test_string_compare_same_until () {
    expect_function("string_compare_same_until", testOb);

    expect("string_compare_same_until handles comparing strings", (: ({
        assert(testOb->string_compare_same_until("abc", "abc"), "==", 3),
        assert(testOb->string_compare_same_until("abc", "ab"), "==", 2),
        assert(testOb->string_compare_same_until("ab", "abc"), "==", 2),
        assert(testOb->string_compare_same_until("a", "abc"), "==", 1),
        assert(testOb->string_compare_same_until("abc", "a"), "==", 1),
        assert(testOb->string_compare_same_until("f", "abc"), "==", 0),
        assert(testOb->string_compare_same_until("abc", "f"), "==", 0),
        assert(testOb->string_compare_same_until("staff", "staves"), "==", 3),
        assert(testOb->string_compare_same_until("staffs", "staves"), "==", 3),
    }) :));
}

void test_sanitize_name () {
    expect_function("sanitize_name", testOb);

    expect("sanitize_name handles names", (: ({
        assert(testOb->sanitize_name("test"), "==", "test"),
        assert(testOb->sanitize_name("t'e's't"), "==", "test"),
        assert(testOb->sanitize_name("t e s t"), "==", "test"),
        assert(testOb->sanitize_name("t-e-s-t"), "==", "test"),
        assert(testOb->sanitize_name("TEST"), "==", "test"),
        assert(testOb->sanitize_name("T'- E'- S'- T"), "==", "test"),
    }) :));
}

void test_parse_command_flags () {
    expect_function("parse_command_flags", testOb);

    expect("parse_command_flags parses valid flags", (: ({
        // check no flags
        assert((: testOb->parse_command_flags("something") :), "==", ({ "something", ([ ]) })),
        assert((: testOb->parse_command_flags(0) :), "==", ({ 0, ([ ]) })),

        // check flags at front
        assert((: testOb->parse_command_flags("-a something") :), "==", ({ "something", ([ "a": 1, ]) })),
        assert((: testOb->parse_command_flags("-abc something") :), "==", ({ "something", ([ "abc": 1, ]) })),
        assert((: testOb->parse_command_flags("-a -b -c something") :), "==", ({ "something", ([ "a": 1, "b": 1, "c": 1, ]) })),
        assert((: testOb->parse_command_flags("-a=1 -b=2 -c=3 something") :), "==", ({ "something", ([ "a": "1", "b": "2", "c": "3", ]) })),
        assert((: testOb->parse_command_flags("-a=x -b=y -c=z something") :), "==", ({ "something", ([ "a": "x", "b": "y", "c": "z", ]) })),
        assert((: testOb->parse_command_flags("-path=/path/with/special_characters-in.it something else") :), "==", ({ "something else", ([ "path": "/path/with/special_characters-in.it", ]) })),
    }) :));

    expect("parse_command_flags parses invalid flags", (: ({
        assert((: testOb->parse_command_flags("- something") :), "==", ({ "something", ([ ]) })),
        assert((: testOb->parse_command_flags("something -") :), "==", ({ "something -", ([ ]) })),
        assert((: testOb->parse_command_flags("something -invalidflag") :), "==", ({ "something -invalidflag", ([ ]) })),
        assert((: testOb->parse_command_flags("x y z -a") :), "==", ({ "x y z -a", ([ ]) })),
        assert((: testOb->parse_command_flags("x y z -abc") :), "==", ({ "x y z -abc", ([ ]) })),
    }) :));
}