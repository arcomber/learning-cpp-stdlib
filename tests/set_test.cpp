#include "ordered_set.hpp"
#include <numeric>

//// debugging
#include <iostream>

#ifdef _WIN32
#include "detect_leaks.hpp"  // no valgrind on windows
#endif

#include "gtest/gtest.h"

using namespace wheel;

class set_test : public ::testing::Test {
protected:
	void SetUp() override {
#ifdef _WIN32
		start_detecting();
#endif
	}

	// void TearDown() override {}
};

TEST_F(set_test, size_zero_with_default_initialised_set) {

	ordered_set mylist;
	EXPECT_EQ(mylist.size(), 0);
}

TEST_F(set_test, size_incremented_by_one_after_insert) {
	ordered_set mylist;
	mylist.insert(1);
	EXPECT_EQ(mylist.size(), 1);
}

TEST_F(set_test, inserted_value_can_be_retrieved) {
	ordered_set mylist;
	mylist.insert(1);
	auto it = mylist.find(1);
	EXPECT_EQ(*it, 1);
}

TEST_F(set_test, inserted_value_iterator_returns_correctly) {
	ordered_set mylist;
	auto result = mylist.insert(1);
	EXPECT_EQ(*result.first, 1);
	EXPECT_EQ(result.second, true);
}

TEST_F(set_test, find_fails_when_set_is_empty) {
	ordered_set mylist;
	auto it = mylist.find(2);
	EXPECT_EQ(it, mylist.end());
}

TEST_F(set_test, find_fails_when_value_not_in_set) {
	ordered_set mylist;
	mylist.insert(1);
	auto it = mylist.find(2);
	EXPECT_EQ(it, mylist.end());
}

TEST_F(set_test, clear_causes_size_zero) {

	ordered_set mylist;
	mylist.insert(1);
	EXPECT_EQ(mylist.size(), 1);
	mylist.clear();
	EXPECT_EQ(mylist.size(), 0);
}

TEST_F(set_test, clear_and_start_again_succeeds) {

	ordered_set mylist;
	mylist.insert(1);

	mylist.clear();
	EXPECT_EQ(mylist.size(), 0);

    mylist.insert(3);
    EXPECT_EQ(mylist.size(), 1);
}
