inherit M_TEST;

private int nBeforeAll = 0;
private int nBeforeEach = 0;
private int nAfterEach = 0;
private int nTestOrder = 0;
private string *testOrder = ({
    "test_expects_passing",
    "test_expects_failing",
    "test_lifecycle_events",
});

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/std/module/test.c");
    nBeforeAll ++;
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}
void before_each_test () { nBeforeEach ++; }
void after_each_test () { nAfterEach ++; }
string *test_order () {
    nTestOrder ++;
    return testOrder;
}
string *test_ignore () { return ({ "test_should_be_ignored" }); }

void test_expects_passing () {
    expect_function("expect_function", testOb);
    expect_function("expect", testOb);
    expect_function("assert", testOb);

    expect("assert_equal condition passes", (: ({
        assert_equal(1, 1),
        assert_equal(1.0, 1.0),
        assert_equal("abc", "abc"),
        assert_equal(({ 1, 2, 3 }), ({ 1, 2, 3 })),
        assert_equal(([ 1: "a", ]), ([ 1: "a", ])),
    }) :));

    expect("assert condition 'regex' passes", (: ({
        assert("bat", "regex", "b[a-z]t"),
        assert("cat", "regex", "c[a-z]t"),
        assert("/std/item/corpse#5135", "regex", "^/std/item/corpse#[0-9]+$"),
    }) :));

    expect("assert condition 'catch' passes", (: ({
        assert((: error("Test catch") :), "catch", "*Test catch\n"),
        assert((: error("Test catch 2") :), "catch", "*Test catch 2\n"),
        assert((: error("Different error") :), "catch", "*Different error\n"),
    }) :));

}

void test_expects_failing () {
    expect_next_failure();
    expect_function("nonexistant_function", testOb);

    expect_next_failure();
    expect("expect without asserts should fail", (: ({
        // no asserts should fail
    }) :));

    expect_next_failure();
    expect("assert_equal should fail", (: ({
        assert_equal(0, 1),
        assert_equal(0.0, 1.0),
        assert_equal("xyz", "abc"),
        assert_equal(({ -1, -2, -3 }), ({ 1, 2, 3 })),
        assert_equal(([ "a": 1, ]), ([ 1: "a", ])),
    }) :));

    expect_next_failure();
    expect("assert condition 'regex' should fail", (: ({
        assert("bar", "regex", "b[a-z]t"),
        assert("car", "regex", "c[a-z]t"),
        assert("/std/item/coins#3155", "regex", "/std/item/corpse#[0-9]+"),
    }) :));

    expect_next_failure();
    expect("assert condition 'catch' should fail", (: ({
        assert((: sprintf("%s", "No error") :), "catch", "*Test Error\n"),
        assert((: sprintf("%s", "Not an error") :), "catch", "*Test Error\n"),
        assert((: sprintf("%s", "Success") :), "catch", "*Test Error\n"),
    }) :));
}

void test_async_test (function done) {
    call_out_walltime(function (function done) {
        expect("async function completes", (: ({
            assert_equal(1, 1),
        }) :));
        evaluate(done);
    }, 0.01, done);
}

void test_lifecycle_events () {
    expect_function("query_expect_catch", testOb);

    expect("lifecycle events execute in order", (: ({
        assert_equal(nBeforeAll, 1), // before_all_tests
        assert_equal(nBeforeEach, sizeof(testOrder)), // before_each_test

        assert_equal(nAfterEach, sizeof(testOrder) - 1), // after_each_test not called for this test

        assert_equal(nTestOrder, 1), // test_order
    }) :));

    expect("query_expect_catch is enabled during assert 'catch'", (: ({
        assert_equal(query_expect_catch(), 0),
        assert((: query_expect_catch() && error("Catch") :), "catch", "*Catch\n"),
        assert_equal(query_expect_catch(), 0),
    }) :));
}

void test_should_be_ignored () {
    expect("this test should be ignored", (: ({
        assert_equal(0, 1),
    }) :));
}