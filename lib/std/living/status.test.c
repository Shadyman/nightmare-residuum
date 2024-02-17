inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/living/status.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_busy () {
    expect_function("set_busy", testOb);
    expect_function("query_busy", testOb);

    expect("busy should be settable and queryable", (: ({
        assert(testOb->query_busy(), "==", 0),

        testOb->set_busy(2),
        assert(testOb->query_busy(), "==", 2),
        testOb->heart_beat(),
        assert(testOb->query_busy(), "==", 1),
        testOb->heart_beat(),
        assert(testOb->query_busy(), "==", 0),
        testOb->heart_beat(),
        assert(testOb->query_busy(), "==", 0), // still zero
    }) :));
}

void test_disable () {
    expect_function("set_disable", testOb);
    expect_function("query_disable", testOb);

    expect("disable should be settable and queryable", (: ({
        assert(testOb->query_disable(), "==", 0),

        testOb->set_disable(2),
        assert(testOb->query_disable(), "==", 2),
        testOb->heart_beat(),
        assert(testOb->query_disable(), "==", 1),
        testOb->heart_beat(),
        assert(testOb->query_disable(), "==", 0),
        testOb->heart_beat(),
        assert(testOb->query_disable(), "==", 0), // still zero
    }) :));
}

void test_immobile () {
    expect_function("set_immobile", testOb);
    expect_function("query_immobile", testOb);

    expect("immobile should be settable and queryable", (: ({
        assert(testOb->query_immobile(), "==", 0),

        testOb->set_immobile(2),
        assert(testOb->query_immobile(), "==", 2),
        testOb->heart_beat(),
        assert(testOb->query_immobile(), "==", 1),
        testOb->heart_beat(),
        assert(testOb->query_immobile(), "==", 0),
        testOb->heart_beat(),
        assert(testOb->query_immobile(), "==", 0), // still zero
    }) :));
}

void test_posture () {
    expect_function("set_posture", testOb);
    expect_function("query_posture", testOb);

    expect("posture should be settable and queryable", (: ({
        assert(testOb->query_posture(), "==", "standing"),

        testOb->set_posture("sitting"),
        assert(testOb->query_posture(), "==", "sitting"),
        testOb->set_posture("laying"),
        assert(testOb->query_posture(), "==", "laying"),
        testOb->set_posture("standing"),
        assert(testOb->query_posture(), "==", "standing"),
    }) :));

    // @TODO
    // expect("postures should heal on heartbeat", (: ({
        // resting

        // sleeping

    // }) :));
}