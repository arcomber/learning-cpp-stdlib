#include "list.hpp"
#include <numeric>

//// debugging
#include <iostream>

#ifdef _WIN32
#include "detect_leaks.hpp"  // no valgrind on windows
#endif

#include "gtest/gtest.h"

using namespace wheel;

template<typename T>
list<T> make_list() {
	list<T> mylist;
	int numbers[]{ 0, 1, 2, 3, 4 };
	for (auto& n : numbers) {
		mylist.push_back(n);
	}
	return mylist;
}

class list_test : public ::testing::Test {
protected:
	void SetUp() override {
#ifdef _WIN32
		start_detecting();
#endif
	}

	// void TearDown() override {}
};

// push_back increases size by 1 each time
TEST_F(list_test, push_back_increases_size_by_one) {

	list<int> mylist;
	mylist.push_back(77);
	EXPECT_EQ(mylist.size(), 1);
}

// push_front increases size by 1 each time
TEST_F(list_test, push_front_increases_size_by_one) {

	list<int> mylist;
	EXPECT_EQ(mylist.size(), 0);

	int numbers[]{ 0, 1, 2, 3, 4 };
	size_t entries = 0;
	for (auto& n : numbers) {
		mylist.push_front(n);
		++entries;
		EXPECT_EQ(mylist.size(), entries);
	}
}

// iterate elements
TEST_F(list_test, iterate_add_3_elements_able_to_iterate_each) {

	list<int> mylist;

	int numbers[]{ 1, 2, 3};
	size_t entries = 0;
	for (auto& n : numbers) {
		mylist.push_front(n);
		++entries;
		EXPECT_EQ(mylist.size(), entries);
	}

	// elements sum to 6
	int result = std::accumulate(mylist.begin(), mylist.end(), 0);

	ASSERT_EQ(result, 6);
	std::cout << "values in linked list sum to: " << result << std::endl;

	auto it = mylist.begin();
	std::cout << "1st value: " << *it << std::endl;
	ASSERT_EQ(*it, numbers[2]);
   
	it++;
	std::cout << "2nd value: " << *it << std::endl;
	ASSERT_EQ(*it, numbers[1]);

	++it;
	std::cout << "3rd value: " << *it << std::endl;
	ASSERT_EQ(*it, numbers[0]);
}

TEST_F(list_test, front_add_elements_always_get_front_element) {

	list<int> mylist;

	mylist.push_back(2);
	EXPECT_EQ(mylist.front(), 2);

	mylist.push_front(1);
	EXPECT_EQ(mylist.front(), 1);

	mylist.push_back(3);
	// should be now in sequence from front to back, 1,2,3
	EXPECT_EQ(mylist.front(), 1);
}

TEST_F(list_test, back_add_elements_always_get_back_element) {

	list<int> mylist;

	mylist.push_back(2);
	EXPECT_EQ(mylist.back(), 2);

	mylist.push_front(1);
	EXPECT_EQ(mylist.back(), 2);

	mylist.push_back(3);
	// should be now in sequence from front to back, 1,2,3
	EXPECT_EQ(mylist.back(), 3);
}

TEST_F(list_test, pop_back_elements_popped_in_correct_sequence) {

	list<int> mylist;
	EXPECT_EQ(mylist.size(), 0);

	int numbers[]{ 1, 2, 3 };
	size_t entries = 0;
	for (auto& n : numbers) {
		mylist.push_back(n);
		++entries;
		EXPECT_EQ(mylist.size(), entries);
	}

	while (!mylist.empty()) {
		std::cout << "back item: " << mylist.back() << std::endl;
		EXPECT_EQ(mylist.back(), numbers[entries-1]);
		mylist.pop_back();
		entries--;
		EXPECT_EQ(mylist.size(), entries);
	}
}

TEST_F(list_test, pop_front_elements_popped_in_correct_sequence) {

	list<int> mylist;
	EXPECT_EQ(mylist.size(), 0);

	int numbers[]{ 1, 2, 3 };
	size_t size = sizeof(numbers) / sizeof(numbers[0]);
	size_t entries = 0;
	for (auto& n : numbers) {
		mylist.push_back(n);
		++entries;
		EXPECT_EQ(mylist.size(), entries);
	}

	while (!mylist.empty()) {
		std::cout << "front item: " << mylist.front() << std::endl;
		EXPECT_EQ(mylist.front(), numbers[size - entries]);
		mylist.pop_front();
		entries--;
		EXPECT_EQ(mylist.size(), entries);
	}
}

TEST_F(list_test, clear_causes_size_zero) {

	list<int> mylist;
	EXPECT_EQ(mylist.size(), 0);

	int numbers[]{ 0, 1, 2, 3, 4 };
	size_t entries = 0;
	for (auto& n : numbers) {
		mylist.push_front(n);
		++entries;
		EXPECT_EQ(mylist.size(), entries);
	}

	EXPECT_NE(mylist.size(), 0);
	mylist.clear();
	EXPECT_EQ(mylist.size(), 0);
}

TEST_F(list_test, clear_and_start_again_succeeds) {

	list<int> mylist;
	EXPECT_EQ(mylist.size(), 0);

	int numbers[]{ 0, 1, 2, 3, 4 };
	size_t entries = 0;
	for (auto& n : numbers) {
		mylist.push_front(n);
		++entries;
		EXPECT_EQ(mylist.size(), entries);
	}

	EXPECT_NE(mylist.size(), 0);
	mylist.clear();
	entries = 0;
	EXPECT_EQ(mylist.size(), 0);

	for (auto& n : numbers) {
		mylist.push_front(n);
		++entries;
		EXPECT_EQ(mylist.size(), entries);
	}
}

TEST_F(list_test, copy_constructor_deep_copy_values) {

	list<int> mylist = make_list<int>();
	EXPECT_EQ(mylist.size(), 5);

	list<int> list2(mylist);
	EXPECT_EQ(list2.size(), 5);
	EXPECT_EQ(list2.front(), mylist.front());
	EXPECT_EQ(list2.back(), mylist.back());
}

TEST_F(list_test, assignment_constructor_deep_copy_values) {

	list<int> mylist = make_list<int>();
	EXPECT_EQ(mylist.size(), 5);

	list<int> list2;
	list2.push_back(3);
	list2 = mylist;
	EXPECT_EQ(list2.size(), 5);
	EXPECT_EQ(list2.front(), mylist.front());
	EXPECT_EQ(list2.back(), mylist.back());
}

TEST_F(list_test, assignment_elements_added_correctly) {

	// create a new resizing array from an initialisation_list
	list<int> lst1{ 1,2,3,4,5 };

	// assignment operator test
 // will crash if haven't implemented operator=
	list<int> list2 = lst1;

	EXPECT_EQ(lst1.size(), list2.size());

	for (auto it1 = lst1.begin(), it2 = list2.begin(); it1 != lst1.end(); ++it1, ++it2) {
		EXPECT_EQ(*it1, *it2);
	}
}

// to test move constructor
static list<int> fill(const std::vector<int>& input, int*& ptr) {
	list<int> list1;
	for (auto it = input.begin(); it != input.end(); ++it) {
		list1.push_back(*it);
	}

	ptr = &(*list1.begin());
	return list1;
}

TEST_F(list_test, move_constructor_elements_moved) {

	// move constructor and move assignment test
	std::vector<int> input{ 1,2,3,4,5,6,7,8,9,10 };
	list<int>::iterator hdptr = nullptr;
	int* ptr = nullptr;
	list<int> l1 = fill(input, ptr);

	EXPECT_EQ(l1.size(), input.size());

	// head ptr of l1 returned must be same as head ptr of list1 in fill function - if move ctor correct 
	int* ptrl1 = &(*l1.begin());
	EXPECT_EQ(ptr, ptrl1);

	// old list must be empty now - not sure how to check that - in fill function - maybe have to make list1 in there global?

	auto it2 = input.begin();
	for (auto it1 = l1.begin(); it1 != l1.end(); ++it1, ++it2) {
		EXPECT_EQ(*it1, *it2);
	}
}

TEST_F(list_test, insert_at_beginning_position_check) {

	list<int> mylist;
	mylist.push_back(1);
	mylist.push_back(2);

	list<int>::iterator it = mylist.begin();
	list<int>::iterator inserted = mylist.insert(it, 99);

	// insert returns iterator to inserted item
	EXPECT_EQ(*inserted, 99);

	// front element should be value 99
	EXPECT_EQ(mylist.front(), 99);

	// last element should remain as value 2
	EXPECT_EQ(mylist.back(), 2);
}

TEST_F(list_test, insert_middle_position_check) {

	list<int> mylist;
	mylist.push_back(1);
	mylist.push_back(2);

	list<int>::iterator it = mylist.begin();
	++it;  // now points to last element
	list<int>::iterator inserted = mylist.insert(it, 99);

	// insert returns iterator to inserted item
	EXPECT_EQ(*inserted, 99);

	// front element should be value 1
	EXPECT_EQ(mylist.front(), 1);

	// last element should be value 2
	EXPECT_EQ(mylist.back(), 2);
}

TEST_F(list_test, insert_end_position_check) {

	list<int> mylist;
	mylist.push_back(1);
	mylist.push_back(2);

	list<int>::iterator it = mylist.end();

	list<int>::iterator inserted = mylist.insert(it, 99);

	// insert returns iterator to inserted item
	EXPECT_EQ(*inserted, 99);

	// front element should be value 1
	EXPECT_EQ(mylist.front(), 1);

	// last element should be value 2
	EXPECT_EQ(mylist.back(), 99);
}

TEST_F(list_test, insert_no_existing_nodes) {

	list<int> mylist;

	list<int>::iterator it = mylist.begin();

	list<int>::iterator inserted = mylist.insert(it, 99);

	// insert returns iterator to inserted item
	EXPECT_EQ(*inserted, 99);

	// front element should be value 1
	EXPECT_EQ(mylist.front(), 99);

	// last element should be value 2
	EXPECT_EQ(mylist.back(), 99);
}

TEST_F(list_test, erase_at_beginning_position_check) {

	list<int> mylist;
	mylist.push_back(1);
	mylist.push_back(2);

	list<int>::iterator it = mylist.begin();

	list<int>::iterator after_erased = mylist.erase(it);

	// erase returns iterator to element after element erased
	EXPECT_EQ(*after_erased, 2);

	// front element should be value 2
	EXPECT_EQ(mylist.front(), 2);

	// last element should remain as value 2
	EXPECT_EQ(mylist.back(), 2);

	// should be 1 element remaining
	EXPECT_EQ(mylist.size(), 1);
}

TEST_F(list_test, erase_end_position) {

	list<int> mylist;
	mylist.push_back(1);
	mylist.push_back(2);

	list<int>::iterator it = mylist.begin();
	++it;

	// erase 2 - after erased will be end
	list<int>::iterator after_erased = mylist.erase(it);

	// front element should be value 1
	EXPECT_EQ(mylist.front(), 1);

	// last element should remain as value 1
	EXPECT_EQ(mylist.back(), 1);

	// should be 1 element remaining
	EXPECT_EQ(mylist.size(), 1);
}

TEST_F(list_test, erase_middle_position) {

	list<int> mylist;
	mylist.push_back(1);
	mylist.push_back(2);
	mylist.push_back(3);

	list<int>::iterator it = mylist.begin();
	++it;

	// erase 2 - after erased will be 3
	list<int>::iterator after_erased = mylist.erase(it);

	// erase returns iterator to element after element erased
	EXPECT_EQ(*after_erased, 3);

	// front element should be value 1
	EXPECT_EQ(mylist.front(), 1);

	// last element should remain as value 1
	EXPECT_EQ(mylist.back(), 3);

	// should be 1 element remaining
	EXPECT_EQ(mylist.size(), 2);
}

TEST_F(list_test, reverse_start_mid_end_reversed) {

	list<int> mylist;
	mylist.push_back(1);
	mylist.push_back(2);
	mylist.push_back(3);

	mylist.reverse();

	list<int>::iterator it = mylist.begin();
	EXPECT_EQ(*it, 3);

	++it;

	// middle element still 2
	EXPECT_EQ(*it, 2);

	++it;

	// last element 1
	EXPECT_EQ(*it, 1);

	// should still be 3 elements in list
	EXPECT_EQ(mylist.size(), 3);
}

TEST_F(list_test, reverse_start_end_reversed) {

	list<int> mylist;
	mylist.push_back(1);
	mylist.push_back(2);

	mylist.reverse();

	// front element should be value 1
	EXPECT_EQ(mylist.front(), 2);

	// last element should remain as value 1
	EXPECT_EQ(mylist.back(), 1);
}

TEST_F(list_test, reverse_single_item_no_change) {

	list<int> mylist;
	mylist.push_back(1);

	mylist.reverse();

	// front element should be value 1
	EXPECT_EQ(mylist.front(), 1);

	// last element should remain as value 1
	EXPECT_EQ(mylist.back(), 1);
}

TEST_F(list_test, reverse_no_elements_no_change) {

	list<int> mylist;
	mylist.reverse();

	EXPECT_EQ(mylist.size(), 0);
}


TEST_F(list_test, splice_beginning_first_list) {

	list<int> list1;
	list1.push_front(2);
	list1.push_front(1);

	list<int> list2;
	list2.push_front(4);
	list2.push_front(3);

	auto it = list1.begin();

	list1.splice(it, list2);

	EXPECT_EQ(list1.size(), 4);
	EXPECT_EQ(list2.size(), 0);

	it = list1.begin();
	EXPECT_EQ(*it, 3);
	it++;
	EXPECT_EQ(*it, 4);
	it++;
	EXPECT_EQ(*it, 1);
	it++;
	EXPECT_EQ(*it, 2);
}

TEST_F(list_test, splice_middle_first_list) {

	list<int> list1;
	list1.push_front(2);
	list1.push_front(1);

	list<int> list2;
	list2.push_front(4);
	list2.push_front(3);

	auto it = list1.begin();
	it++;

	list1.splice(it, list2);

	EXPECT_EQ(list1.size(), 4);
	EXPECT_EQ(list2.size(), 0);

	it = list1.begin();
	EXPECT_EQ(*it, 1);
	it++;
	EXPECT_EQ(*it, 3);
	it++;
	EXPECT_EQ(*it, 4);
	it++;
	EXPECT_EQ(*it, 2);
}

TEST_F(list_test, remove_elements_removed_correctly) {

	list<int> mylist;
	EXPECT_EQ(mylist.size(), 0);

	int numbers[]{ 1,2,3,1,2,3 };
	size_t entries = 0;
	for (auto& n : numbers) {
		mylist.push_back(n);
		++entries;
		EXPECT_EQ(mylist.size(), entries);
	}

	size_t removed = mylist.remove(2);
	EXPECT_EQ(removed, 2);
	EXPECT_EQ(mylist.size(), 4);

	for (auto it = mylist.begin(); it != mylist.end(); ++it) {
		EXPECT_NE(*it, 2);
	}
}

TEST_F(list_test, iterator_constructor_elements_added_correctly) {

	std::vector<int> vec{1,2,3,4,5,6,7,8,9,10};

	list<int> mylist{vec.begin(), vec.end()};
	EXPECT_EQ(mylist.size(), 10);

	int index = 0;
	for (auto it = mylist.begin(); it != mylist.end(); ++it) {
		EXPECT_EQ(*it, vec[index++]);
	}
}

TEST_F(list_test, initialiser_list_constructor_elements_added_correctly) {

	list<int> mylist{ 1,2,3,4,5,6,7,8,9,10 };
	EXPECT_EQ(mylist.size(), 10);

	int number = 1;
	for (auto it = mylist.begin(); it != mylist.end(); ++it) {
		EXPECT_EQ(*it, number++);
	}
}

TEST_F(list_test, operatorequals_list_comparison) {

	list<int> mylist{ 1,2,3,4,5,6,7,8,9,10 };
	EXPECT_EQ(mylist.size(), 10);

	list<int> mylist2(mylist);

	EXPECT_TRUE(mylist == mylist2);
}


struct President
{
	std::string name;
	std::string country;
	int year;

	President(std::string p_name, std::string p_country, int p_year)
		: name(std::move(p_name)), country(std::move(p_country)), year(p_year)
	{
		std::cout << "I am being constructed.\n";
	}
	President(President&& other)
		: name(std::move(other.name)), country(std::move(other.country)), year(other.year)
	{
		std::cout << "I am being moved.\n";
	}
	President& operator=(const President& other) = default;
};

TEST_F(list_test, emplace_back_push_back_emplace) {

	list<President> elections;
	elections.emplace_back("Nelson Mandela", "South Africa", 1994);
	President& p = elections.front();

	EXPECT_EQ(p.year, 1994);
	EXPECT_EQ(p.name, "Nelson Mandela");
	EXPECT_EQ(p.country, "South Africa");
}

class Person {
public:
	Person(const std::string& name, const int age, const int height_cm)
		: name_(name), age_(age), height_cm_(height_cm) {}

	// just used for checking - maybe we don't need?
	~Person() {
		std::cout << name_ << " Person destructor called\n";
	}

	std::string how_tall() const {
	if (height_cm_ < 154)
		return "short";
	else if (height_cm_ < 183)
		return "medium";
	else
		return "tall";
	}

	std::string how_old() const {
		if (age_ < 40)
			return "young";
		else if (age_ < 60)
			return "don't ask";
		else
			return "ancient";
	}

private:
	std::string name_;
	int age_;
	int height_cm_;
};

TEST_F(list_test, push_back_non_trivial_object) {

	list<Person> persons;
	Person Robert("Robert De Niro", 78, 171);
	Person Julia("Julia Roberts", 54, 175);

	persons.push_back(Robert);
	persons.push_back(Julia);

	Person& pref = persons.front();

	EXPECT_EQ(pref.how_tall(), "medium");
	EXPECT_EQ(pref.how_old(), "ancient");

	Person& prefb = persons.back();

	EXPECT_EQ(prefb.how_tall(), "medium");
	EXPECT_EQ(prefb.how_old(), "don't ask");
}

TEST_F(list_test, pop_back_non_trivial_object) {

	list<Person> persons;
	Person Robert("Robert De Niro", 78, 171);
	Person Julia("Julia Roberts", 54, 175);

	persons.push_back(Robert);
	persons.push_back(Julia);

	EXPECT_EQ(persons.size(), 2);
	persons.pop_back();
	EXPECT_EQ(persons.size(), 1);
	persons.pop_back();
	EXPECT_EQ(persons.size(), 0);
}
