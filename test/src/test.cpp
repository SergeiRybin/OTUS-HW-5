#define BOOST_TEST_MODULE static library test // Defines the name of the program which is used in the messages
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(MainSuite)

BOOST_AUTO_TEST_CASE(copyTest)
{
    BOOST_TEST(true);
}

BOOST_AUTO_TEST_SUITE_END()