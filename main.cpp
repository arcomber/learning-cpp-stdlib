/*
Exercise resizing_array
to check for no memory leaks run this code on linux using valgrind:
$ valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./resizing_array_test
*/
#include "resizing_array.hpp"

#include <cassert>

using namespace play;

// to test move constructor
resizing_array<int> fill(std::istream& is) {
    std::cout << "Enter your list of integers, ctrl-D to finish: ";
    resizing_array<int> ra;
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


int main() {

    resizing_array<int> ra;

    // Can it grow?
    for (size_t i = 0; i < 100; ++i) {
        ra.push_back(i);
    }

    // can we read elements by [index]?
    std::cout << "ra[0]=" << ra[0] << std::endl;
    std::cout << "ra[1]=" << ra[1] << std::endl;
    std::cout << "ra[2]=" << ra[2] << std::endl;
    std::cout << "ra[3]=" << ra[3] << std::endl;
    std::cout << "ra[4]=" << ra[4] << std::endl;

    // check iteration
    int number{ 0 };
    for (auto element : ra) {
        assert(element == number++);
    }

    // can we write a value by [index]?
    ra[3] = 10;
    std::cout << "ra[3] is now =" << ra[3] << std::endl;

    // can we remove elements?
    std::cout << "current size of resizing array is: " << ra.size() << " and last element: " << ra[ra.size() - 1] << std::endl;
    ra.pop_back();
    std::cout << "size of resizing array after pop() is: " << ra.size() << " and last element: " << ra[ra.size() - 1] << std::endl;

    // can we erase an element?  - should erase the value at beginning, 0, and return a ptr to next value - 1
    size_t current_size = ra.size();
    int* it = ra.erase(ra.begin());
    std::cout << "ra.erase() returned iterator: " << it << " containing value: " << *it << std::endl;
    assert(*it = 1);
    assert(ra.size() == --current_size);
    it = ra.erase(ra.begin());
    assert(*it == 2);
    assert(ra.size() == --current_size);

    // can retrieve front element - which will be 2 [*it]
    assert(ra.front() == *it);

    // can retrieve back element - should be the same as ra[ra.size() - 1]
    int expected_last = ra[ra.size() - 1];
    assert(ra.back() == expected_last);


    // copy construction works?
    resizing_array<int> ra2(ra);
    std::cout << "current size of resizing array 2 is: " << ra2.size() << " and last element: " << ra2[ra2.size() - 1] << std::endl;

    // create a new resizing_array from begin and end iterator constructor
    const int raw_array[]{ 1,2,3,4,5};
    size_t size = sizeof(raw_array) / sizeof(raw_array[0]);
    resizing_array<int> ra3(raw_array, raw_array + size);

    assert(ra3.size() == 5);

    assert(ra3[0] == raw_array[0]);
    assert(ra3[1] == raw_array[1]);
    assert(ra3[2] == raw_array[2]);
    assert(ra3[3] == raw_array[3]);
    assert(ra3[4] == raw_array[4]);

    // create a new resizing array from an initialisation_list
    resizing_array<int> ra4{1,2,3,4,5};
    assert(ra4[0] == 1);
    assert(ra4[1] == 2);
    assert(ra4[2] == 3);
    assert(ra4[3] == 4);
    assert(ra4[4] == 5);


    // assignment operator test
    // will crash if haven't implemented operator=
    ra4 = ra3;

    assert(ra4[ra4.size() - 1] == ra3[ra3.size() - 1]);

    // move constructor and move assignment test
    resizing_array<int> ra5 = fill(std::cin);


    // construct from a single value
    resizing_array<int> ra7(10u, 7);
    for (auto element : ra7) {
        assert(element == 7);
    }

    // load up an object
    Person p1("Angus");
    Person p2("Lisa");

    resizing_array<Person> ra8{ p1, p2 };
    assert(ra8.front().name() == p1.name());
    assert(ra8.back().name() == p2.name());
}
