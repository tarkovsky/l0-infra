#include <magellan/magellan.hpp>
#include <l0-infra/std/Algorithm.h>
#include <l0-infra/std/StaticAssert.h>

USING_HAMCREST_NS

namespace
{
    int arr[] = {-1, -2, 3, 4};
    std::vector<int> v1 {-1, -2, 3, 4};
    const std::vector<int> v2 {-1, -2, 3, 4};
}

FIXTURE(AlgorithmTest)
{
    TEST("find support non const container")
    {
        auto found = stdext::find(v1, 3);

        STATIC_ASSERT_TYPE(std::vector<int>::iterator, found);
        ASSERT_THAT(*found, is(3));
    }

    TEST("find support const container")
    {
        auto found = stdext::find(v2, 3);

        STATIC_ASSERT_TYPE(std::vector<int>::const_iterator, found);
        ASSERT_THAT(*found, is(3));
    }

    TEST("find_if support non const container")
    {
        auto found = stdext::find_if(v1, [](auto e) { return e > 0; });

        STATIC_ASSERT_TYPE(std::vector<int>::iterator, found);
        ASSERT_THAT(*found, is(3));
    }

    TEST("find_if support const container")
    {
        auto found = stdext::find_if(v2, [](auto e) { return e > 0; });

        STATIC_ASSERT_TYPE(std::vector<int>::const_iterator, found);
        ASSERT_THAT(*found, is(3));
    }

    TEST("map")
    {
        std::vector<int> v;
        stdext::map(v1, std::back_inserter(v), [](auto e) { return e > 0 ? e : -e; });

        ASSERT_THAT(v[0], is(1));
        ASSERT_THAT(v[1], is(2));
        ASSERT_THAT(v[2], is(3));
        ASSERT_THAT(v[3], is(4));
    }

    TEST("reduce")
    {
        auto sum = stdext::reduce(arr, 0, [](auto& sum, auto e) { sum += e; });

        ASSERT_THAT(sum, is(4));
    }

    TEST("filter")
    {
        std::vector<int> v;
        stdext::filter(arr, std::back_inserter(v), [](auto e) { return e > 0; });

        ASSERT_THAT(v[0], is(3));
        ASSERT_THAT(v[1], is(4));
    }

    TEST("each")
    {
        auto sum = 0;
        stdext::each(arr, [&sum](auto e) { sum += e; });

        ASSERT_THAT(sum, is(4));
    }
};
