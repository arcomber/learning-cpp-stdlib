#ifndef VECTOR_
#define VECTOR_

/*
Useful resources:
https://en.cppreference.com/w/cpp/container/vector
https://www.cs.odu.edu/~zeil/cs361/sum18/Public/vectorImpl/index.html
https://www.cs.odu.edu/~zeil/references/cpp_ref_draft_nov97/lib-containers.html

Operation       Speed
vector()        O(1)
vector(n, x)    O(n)
size()          O(1)
v[ i ]          O(1)
push_back(x)    O(1)
pop_back        O(1)
insert          O(size())  // TODO
erase           O(size())  // implemented single element only
front, back     O(1)
*/

#include <iterator>
#include <algorithm>
#include <initializer_list>
#include <iostream>  // debug output

namespace wheel {  // as in re-inventing the wheel

    template< typename T >
    class vector {
    public:

        template< typename input_iterator >
        vector(input_iterator first, input_iterator last) {

            size_ = std::distance(first, last);
            array_ = nullptr;
            resize_array(size_);
            std::copy(first, last, array_);
        }

        vector(std::initializer_list<T> init) : size_(init.size()), array_(nullptr) {
            size_ = init.size();
            resize_array(size_);
            std::copy(init.begin(), init.end(), array_);
        }

        vector() : size_(0), array_(nullptr) {
            resize_array(8);
            std::cout << "allocating array at address: " << array_ << std::endl;
        }

        vector(size_t count, const T& value) : size_(count), array_(nullptr) {
            resize_array(count);
            std::fill(array_, array_+size_, value);
        }

        vector(const vector& other) : size_(other.size()), capacity_(other.capacity()), array_(nullptr) {
            resize_array(capacity_);
            std::cout << "copy ctor allocated array at address: " << array_ << std::endl;
            std::copy(other.begin(), other.end(), array_);
        }

        vector& operator=(const vector& other) {
            if (this != &other) {
                size_ = other.size();
                delete [] array_;
                array_ = nullptr;
                resize_array(other.capacity());
                std::cout << "op= allocated array at address: " << array_ << std::endl;
                std::copy(other.begin(), other.end(), array_);
            }
            return *this;
        }

        vector(vector&& other) : size_(other.size()), capacity_(other.capacity()), array_(other.begin()) {
            other.size_ = 0;
            other.capacity_ = 0;
            other.array_ = nullptr;
        }

        vector& operator=(vector&& other) {
            //free(array_);
            delete [] array_;
            array_ = other.array_;
            size_ = other.size();
            capacity_ = other.capacity();
            other.array_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }

        ~vector() {
            clear();
        }


        // O(n) - implementation below will leak - I think, because have to call destructor on each element?
        void clear() {
            std::cout << "deallocating array at address: " << array_ << std::endl;
            delete [] array_;
            array_ = nullptr;
            size_ = 0u;
            capacity_ = 0u;
        }

        void push_back(const T& v) {
            if (size_ == capacity_) {
                resize_array(size_ * 2);
            }
            array_[size_++] = v;
        }

        void pop_back() {
            --size_;
        }

        size_t size() const { return size_; }

        bool empty() const { return size_ == 0u; }

        T& operator[](size_t index) {
            return array_[index];
        }

        const T& operator[](size_t index) const {
            return array_[index];
        }

        T* begin() {
            return array_;
        }
        T* end() {
            return array_ + size_;
        }

        const T* begin() const {
            return array_;
        }
        const T* end() const {
            return array_ + size_;
        }

        T front() {
            return array_[0];
        }

        const T front() const {
            return array_[0];
        }

        T back() {
            return array_[size_ - 1];
        }

        const T back() const {
            return array_[size_ - 1];
        }

        size_t capacity() const {
            return capacity_;
        }

        T* erase(T* pos) {
            T* next = pos+1;
            std::copy(next, end(), pos);
            --size_;
            return pos;
        }

    private:
        void resize_array(size_t new_size) {

            T* temp = nullptr;
            if (array_) {
                temp = new T[new_size];
                std::copy(array_, array_ + size_, temp);
                std::swap(array_, temp);
                delete[] temp;
            }
            else {
                array_ = new T[new_size];
            }

            capacity_ = new_size;

        }

        size_t size_ = 0;
        size_t capacity_ = 0;
        T* array_ = nullptr;
    };

} // end of namespace wheel

#endif // VECTOR_
