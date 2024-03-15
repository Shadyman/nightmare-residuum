inherit M_TEST;
inherit STD_STORAGE;

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/item.c");
    this_object()->handle_move("/domain/Nowhere/room/void.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_is_item () {
    expect_function("is_item", testOb);

    expect("is_item returns true", (: ({
        assert_equal(testOb->is_item(), 1),
        assert_equal(testOb->is_weapon(), 0),
        assert_equal(testOb->is_food(), 0),
    }) :));
}

void test_value () {
    expect_function("query_value", testOb);
    expect_function("set_value", testOb);

    expect("value is queryable and settable", (: ({
        assert_equal(testOb->query_value(), 0),
        testOb->set_value(123),
        assert_equal(testOb->query_value(), 123),
        testOb->set_value(54321),
        assert_equal(testOb->query_value(), 54321),
    }) :));
}

void test_verb_look_applies () {
    object r = new(STD_ROOM);

    expect_function("direct_look_at_obj", testOb);
    expect_function("direct_look_obj", testOb);

    expect("item handles verb apply direct_look_at_obj", (: ({
        // check mismatch environments
        assert_equal(environment(testOb), 0),
        assert_equal(environment(), find_object("/domain/Nowhere/room/void.c")),
        assert_equal(testOb->direct_look_at_obj(), 0),  // failure
        assert_equal(testOb->direct_look_obj(), 0),     // failure

        // check matching environments
        assert_equal(testOb->handle_move($(r)), 1),
        assert_equal(environment(testOb), $(r)),
        assert_equal(this_object()->handle_move($(r)), 1),
        assert_equal(environment(), $(r)),
        assert_equal(testOb->direct_look_at_obj(), 1),  // success
        assert_equal(testOb->direct_look_obj(), 1),     // success

        // cleanup
        assert_equal(testOb->handle_move("/domain/Nowhere/room/void.c"), 1),
        assert_equal(this_object()->handle_move("/domain/Nowhere/room/void.c"), 1),
    }) :));

    if (r) destruct(r);
}

void test_verb_drop_applies () {
    expect_function("direct_drop_obj", testOb);

    expect("item handles verb apply direct_drop_obj", (: ({
        assert_equal(environment(testOb), 0),
        assert_equal(environment(), find_object("/domain/Nowhere/room/void.c")),
        assert_equal(testOb->direct_drop_obj(), 0), // can't drop item
        assert_equal(testOb->handle_move(this_object()), 1),
        assert_equal(environment(testOb), this_object()),
        assert_equal(testOb->direct_drop_obj(), 1), // can drop
        assert_equal(testOb->handle_move("/domain/Nowhere/room/void.c"), 1),
    }) :));
}

void test_verb_give_applies () {
    expect_function("direct_give_obj_to_liv", testOb);

    expect("item handles verb apply direct_give_obj_to_liv", (: ({
        assert_equal(environment(testOb), 0),
        assert_equal(testOb->direct_give_obj_to_liv(), 0), // can't give item yet
        assert_equal(testOb->handle_move(this_object()), 1),
        assert_equal(environment(testOb), this_object()),
        assert_equal(testOb->direct_give_obj_to_liv(), 1), // can give item
    }) :));
}

void test_verb_get_obj_applies () {
    expect_function("direct_get_obj", testOb);

    expect("item handles verb apply direct_get_obj", (: ({
        assert_equal(environment(testOb), 0),
        assert_equal(environment(), find_object("/domain/Nowhere/room/void.c")),
        assert_equal(testOb->direct_get_obj(), "You can't get what isn't here."),
        assert_equal(testOb->handle_move("/domain/Nowhere/room/void.c"), 1),
        assert_equal(environment(testOb), find_object("/domain/Nowhere/room/void.c")),
        assert_equal(testOb->direct_get_obj(), 1), // can get item
        assert_equal(testOb->handle_move(this_object()), 1), // move item back
        assert_equal(testOb->direct_get_obj(), 0), // already has item get item
    }) :));
}

nosave private int GetCounter = 0;
void test_no_get () {
    expect_function("query_no_get", testOb);
    expect_function("set_no_get", testOb);

    testOb->set_name("test item");

    expect("set_no_get and direct_get_obj behave", (: ({
        assert_equal(environment(testOb), 0),
        assert_equal(environment(), find_object("/domain/Nowhere/room/void.c")),
        assert_equal(testOb->handle_move("/domain/Nowhere/room/void.c"), 1),
        assert_equal(testOb->query_no_get(), UNDEFINED),

        // test int
        testOb->set_no_get(1),
        assert_equal(testOb->query_no_get(), 1),
        assert_equal(testOb->direct_get_obj(), "You can't get the test item."),
        assert_equal(environment(testOb), find_object("/domain/Nowhere/room/void.c")),

        // test str
        testOb->set_no_get("Nope, no get."),
        assert_equal(testOb->query_no_get(), "Nope, no get."),
        assert_equal(testOb->direct_get_obj(), "Nope, no get."),
        assert_equal(environment(testOb), find_object("/domain/Nowhere/room/void.c")),

        // test fn
        testOb->set_no_get(function (object character) {
            GetCounter ++;
            return "Function says no.";
        }),
        assert_equal(!!functionp(testOb->query_no_get()), 1),
        assert_equal(testOb->direct_get_obj(), "Function says no."),
        assert_equal(environment(testOb), find_object("/domain/Nowhere/room/void.c")),
    }) :));
}

void test_verb_get_obj_from_obj_applies () {
    expect_function("direct_get_obj_from_obj", testOb);

    expect("item handles verb apply direct_get_obj_from_obj", (: ({
        // fails without env
        assert_equal(environment(testOb), 0),
        assert_equal(testOb->direct_get_obj_from_obj(), "You can't get what isn't here."),

        // passes with env
        assert_equal(environment(), find_object("/domain/Nowhere/room/void.c")),
        assert_equal(testOb->handle_move(this_object()), 1),
        assert_equal(testOb->direct_get_obj_from_obj(), 1),
    }) :));
}

void test_verb_put_obj_in_obj_applies () {
    expect_function("direct_put_obj_in_obj", testOb);

    expect("item handles verb apply direct_put_obj_in_obj", (: ({
        // fails without input
        assert_equal(testOb->direct_put_obj_in_obj(), 0),

        // fails without env
        assert_equal(environment(testOb), 0),
        assert_equal(testOb->direct_put_obj_in_obj(testOb), "You can't put what you don't have."),

        // passes with env
        assert_equal(environment(), find_object("/domain/Nowhere/room/void.c")),
        assert_equal(testOb->handle_move(this_object()), 1),
        assert_equal(testOb->direct_put_obj_in_obj(testOb), 1),
    }) :));
}

void test_verb_sell_obj_applies () {
    expect_function("direct_sell_obj", testOb);

    expect("item handles verb apply direct_sell_obj", (: ({
        // fails without input
        assert_equal(testOb->direct_sell_obj(), 0),

        // fails without env
        assert_equal(environment(testOb), 0),
        assert_equal(testOb->direct_sell_obj(testOb), "You can't sell what you don't have."),

        // passes with env
        assert_equal(environment(), find_object("/domain/Nowhere/room/void.c")),
        assert_equal(testOb->handle_move(this_object()), 1),
        assert_equal(testOb->direct_sell_obj(testOb), 1),
    }) :));
}