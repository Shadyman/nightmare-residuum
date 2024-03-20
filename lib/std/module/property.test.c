inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/module/property.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({
        "test_property_single",
        "test_properties_multiple",
     });
}

void test_property_single () {
    expect_function("query_property", testOb);
    expect_function("set_property", testOb);
    expect_function("add_property", testOb);
    expect_function("remove_property", testOb);

    expect("add, set, remove, and query property are handled", (: ({
        assert_equal(testOb->query_property("test_key"), UNDEFINED),

        assert_equal(testOb->set_property("test_key1", "test_value"), "test_value"),
        assert_equal(testOb->query_property("test_key1"), "test_value"),

        assert_equal(testOb->set_property("test_key2", to_float(123)), to_float(123)),
        assert_equal(testOb->query_property("test_key2"), to_float(123)),

        assert_equal(testOb->set_property("test_key4", ""), ""),
        assert_equal(testOb->query_property("test_key4"), ""),

        assert_equal(testOb->add_property("test_key3", 10), 10),
        assert_equal(testOb->query_property("test_key3"), 10),
        assert_equal(testOb->add_property("test_key3", 1), 11),
        assert_equal(testOb->query_property("test_key3"), 11),

        assert_equal(testOb->remove_property("test_key1"), 1),
        assert_equal(testOb->query_property("test_key1"), UNDEFINED),
        assert_equal(testOb->remove_property("test_key2"), 1),
        assert_equal(testOb->query_property("test_key2"), UNDEFINED),
    }) :));
}

private mapping m1 = ([ "test_key1": "test_value" ]);
private mapping m2 = ([ "test_key2": to_float(123) ]);

void test_properties_multiple () {
    expect_function("query_properties", testOb);
    expect_function("set_properties", testOb);
    expect_function("remove_properties", testOb);

    expect("set and query properties are handled", (: ({
        assert_equal(testOb->query_properties(), ([ ])),
        assert_equal(testOb->set_properties(m1), identify(m1)),
        assert_equal(testOb->query_properties(), identify(m1)),
        assert_equal(testOb->set_properties(m2), identify(m1+m2)),
        assert_equal(testOb->query_properties(), identify(m1+m2)),
        assert_equal(testOb->remove_property("test_key2"), 1),
        assert_equal(testOb->query_properties(), identify(m1)),
        assert_equal(testOb->remove_property("test_key1"), 1),
        assert_equal(testOb->query_properties(), ([ ])),
    }) :));

    expect("remove properties is handled", (: ({
        assert_equal(testOb->remove_properties(), ([ ])),
        assert_equal(testOb->query_properties(), ([ ])),
        assert_equal(testOb->set_properties(m1), identify(m1)),
        assert_equal(testOb->set_properties(m2), identify(m1+m2)),
        assert_equal(testOb->remove_properties(), ([ ])),
    }) :));
}