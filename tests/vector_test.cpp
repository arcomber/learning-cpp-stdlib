#include "vector.hpp"
#include <numeric>

//// debugging
#include <iostream>
#include <cassert>

#ifdef _WIN32
#include "detect_leaks.hpp"  // no valgrind on windows
#endif

#include "gtest/gtest.h"


using namespace wheel;

template<typename T>
vector<T> make_list() {
	vector<T> mylist;
	int numbers[]{ 0, 1, 2, 3, 4 };
	for (auto& n : numbers) {
		mylist.push_back(n);
	}
	return mylist;
}


class vector_test : public ::testing::Test {
protected:
	void SetUp() override {
#ifdef _WIN32
		start_detecting();
#endif
	}

	// void TearDown() override {}
};

// to test move constructor
vector<int> fill(std::istream& is) {
    std::cout << "Enter your list of integers, ctrl-D to finish: ";
    vector<int> ra;
    for (int v; is >> v; )
        ra.push_back(v);

    return ra;
}

// test object to insert into resizing_array
class Person {
public:
    Person() :name_() {}
    Person(const std::string& name) : name_(name) {}
    Person(const Person& other) {
        this->name_ = other.name();
    }
    const std::string name() const { return name_; }
private:
    std::string name_;
};


// push_back increases size by 1 each time
TEST_F(vector_test, push_back_increases_size_by_one) {

    vector<int> mylist;
    EXPECT_EQ(mylist.size(), 0u);

    int numbers[]{ 0, 1, 2, 3, 4 };
    size_t entries = 0;
    for (auto& n : numbers) {
        mylist.push_back(n);
        ++entries;
        EXPECT_EQ(mylist.size(), entries);
    }
}
// iterate elements
TEST_F(vector_test, iterate_add_3_elements_able_to_iterate_each) {

	vector<int> mylist;
	EXPECT_EQ(mylist.size(), 0u);

	int numbers[]{ 1, 2, 3 };
	size_t entries = 0;
	for (auto& n : numbers) {
		mylist.push_back(n);
		++entries;
		EXPECT_EQ(mylist.size(), entries);
	}

	// elements sum to 6
	int result = std::accumulate(mylist.begin(), mylist.end(), 0);

	ASSERT_EQ(result, 6);
	std::cout << "values in linked list sum to: " << result << std::endl;

	auto it = mylist.begin();
	std::cout << "1st value: " << *it << std::endl;
	ASSERT_EQ(*it, numbers[0]);

	it++;
	std::cout << "2nd value: " << *it << std::endl;
	ASSERT_EQ(*it, numbers[1]);

	++it;
	std::cout << "3rd value: " << *it << std::endl;
	ASSERT_EQ(*it, numbers[2]);
}

TEST_F(vector_test, back_add_elements_always_get_back_element) {

	vector<int> mylist;

	mylist.push_back(2);
	EXPECT_EQ(mylist.back(), 2);

	mylist.push_back(1);
	EXPECT_EQ(mylist.back(), 1);

	mylist.push_back(3);
	// should be now in sequence from front to back, 1,2,3
	EXPECT_EQ(mylist.back(), 3);
}

TEST_F(vector_test, pop_back_elements_popped_in_correct_sequence) {

	vector<int> mylist;
	EXPECT_EQ(mylist.size(), 0u);

	int numbers[]{ 1, 2, 3 };
	size_t entries = 0;
	for (auto& n : numbers) {
		mylist.push_back(n);
		++entries;
		EXPECT_EQ(mylist.size(), entries);
	}

	while (!mylist.empty()) {
		std::cout << "back item: " << mylist.back() << std::endl;
		EXPECT_EQ(mylist.back(), numbers[entries - 1]);
		mylist.pop_back();
		entries--;
		EXPECT_EQ(mylist.size(), entries);
	}
}

TEST_F(vector_test, clear_causes_size_zero) {

	vector<int> mylist;
	EXPECT_EQ(mylist.size(), 0u);

	int numbers[]{ 0, 1, 2, 3, 4 };
	size_t entries = 0;
	for (auto& n : numbers) {
		mylist.push_back(n);
		++entries;
		EXPECT_EQ(mylist.size(), entries);
	}

	EXPECT_EQ(mylist.size(), 5u);
	mylist.clear();
	EXPECT_EQ(mylist.size(), 0u);
}

TEST_F(vector_test, clear_and_start_again_succeeds) {

	vector<int> mylist;
	EXPECT_EQ(mylist.size(), 0u);

	int numbers[]{ 0, 1, 2, 3, 4 };
	size_t entries = 0;
	for (auto& n : numbers) {
		mylist.push_back(n);
		++entries;
		EXPECT_EQ(mylist.size(), entries);
	}

	EXPECT_NE(mylist.size(), 0u);
	mylist.clear();
	entries = 0;
	EXPECT_EQ(mylist.size(), 0u);

	for (auto& n : numbers) {
		mylist.push_back(n);
		++entries;
		EXPECT_EQ(mylist.size(), entries);
	}
}

TEST_F(vector_test, copy_constructor_deep_copy_values) {

	vector<int> mylist = make_list<int>();
	EXPECT_EQ(mylist.size(), 5u);

	vector<int> list2(mylist);
	EXPECT_EQ(list2.size(), 5u);
	EXPECT_EQ(list2.front(), mylist.front());
	EXPECT_EQ(list2.back(), mylist.back());
}

TEST_F(vector_test, assignment_constructor_deep_copy_values) {

	vector<int> mylist = make_list<int>();
	EXPECT_EQ(mylist.size(), 5u);

	vector<int> list2;
	list2.push_back(3);
	list2 = mylist;
	EXPECT_EQ(list2.size(), 5u);
	EXPECT_EQ(list2.front(), mylist.front());
	EXPECT_EQ(list2.back(), mylist.back());
}

TEST_F(vector_test, assignment_elements_added_correctly) {

	// create a new resizing array from an initialisation_list
	vector<int> lst1{ 1,2,3,4,5 };

	// assignment operator test
 // will crash if haven't implemented operator=
	vector<int> list2 = lst1;

	EXPECT_EQ(lst1.size(), list2.size());

	for (auto it1 = lst1.begin(), it2 = list2.begin(); it1 != lst1.end(); ++it1, ++it2) {
		EXPECT_EQ(*it1, *it2);
	}
}

// to test move constructor
vector<int> fill(const std::vector<int>& input, int*& ptr) {
	vector<int> list1;
	for (auto it = input.begin(); it != input.end(); ++it) {
		list1.push_back(*it);
	}

	ptr = &(*list1.begin());
	return list1;
}

// we don't expose an iterator in our vector - so line 247 below wont' compule

//TEST_F(vector_test, move_constructor_elements_moved) {
//
//	// move constructor and move assignment test
//	std::vector<int> input{ 1,2,3,4,5,6,7,8,9,10 };
//	play::vector<int>::iterator hdptr = nullptr;
//	int* ptr = nullptr;
//	play::vector<int> l1 = fill(input, ptr);
//
//	EXPECT_EQ(l1.size(), input.size());
//
//	// head ptr of l1 returned must be same as head ptr of list1 in fill function - if move ctor correct 
//	int* ptrl1 = &(*l1.begin());
//	EXPECT_EQ(ptr, ptrl1);
//
//	// old list must be empty now - not sure how to check that - in fill function - maybe have to make list1 in there global?
//
//	auto it2 = input.begin();
//	for (auto it1 = l1.begin(); it1 != l1.end(); ++it1, ++it2) {
//		EXPECT_EQ(*it1, *it2);
//	}
//}
