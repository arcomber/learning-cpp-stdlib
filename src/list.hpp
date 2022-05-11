#ifndef LIST_HPP_
#define LIST_HPP_

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <utility>

namespace wheel {  // as in re-inventing the wheel

	template< typename T >
	class list {
	public:

		struct node {
			T value;
			node* next = nullptr;
			node* prior = nullptr;
		};

		struct iterator {

			using value_type = T;
			using difference_type = std::ptrdiff_t;
			using pointer = T*;
			using reference = T&;
			using iterator_category = std::bidirectional_iterator_tag;

			constexpr iterator(node* p) noexcept : ptr_{ p } {}

			iterator& operator++() {
				if (ptr_) {
					ptr_ = ptr_->next;
				}
				return *this;
			}

			iterator operator++(int) {
				auto old = *this;
				if (ptr_) {
					ptr_ = ptr_->next;
				}
				return old;
			}

			// iterator_category bidirectional, so have to implement --
			iterator& operator--() {
				if (ptr_) {
					ptr_ = ptr_->prior;
				}
				return *this;
			}

			iterator operator--(int) {
				auto old = *this;
				if (ptr_) {
					ptr_ = ptr_->prior;
				}
				return old;
			}

			T& operator*() const { return ptr_->value; }
			T* operator->() { return &ptr_->value; }

			bool operator==(const iterator& other) const { return ptr_ == other.ptr_; }
			bool operator!=(const iterator& other) const { return ptr_ != other.ptr_; }

			node* ptr_ = nullptr;
		};

		struct const_iterator {

			using value_type = const T;
			using difference_type = std::ptrdiff_t;
			using pointer = const T*;
			using reference = const T&;
			using iterator_category = std::bidirectional_iterator_tag;

			constexpr const_iterator() noexcept = default;

			constexpr const_iterator(node* p) noexcept : ptr_{ p } {}

			// Implicit conversion from iterator:
			constexpr const_iterator(iterator const& it) noexcept {
				this->ptr_ = it.ptr_;
			}

			const_iterator& operator++() {
				if (ptr_) {
					ptr_ = ptr_->next;
				}
				return *this;
			}

			const_iterator operator++(int) {
				auto old = *this;
				if (ptr_) {
					ptr_ = ptr_->next;
				}
				return old;
			}

			// iterator_category bidirectional, so have to implement --
			const_iterator& operator--() {
				if (ptr_) {
					ptr_ = ptr_->prior;
				}
				return *this;
			}

			const_iterator operator--(int) {
				auto old = *this;
				if (ptr_) {
					ptr_ = ptr_->prior;
				}
				return old;
			}

			const T& operator*() const { return ptr_->value; }
			T* operator->() { return &ptr_->value; }

			bool operator==(const const_iterator& other) const { return ptr_ == other.ptr_; }
			bool operator!=(const const_iterator& other) const { return ptr_ != other.ptr_; }

			node const* ptr_ = nullptr;
		};

		// O(1)
		list() = default;

		// O(n)
		template <typename InputIterator>
		constexpr list(InputIterator first, InputIterator last)
			: list{}    // delegate to default constuctor
		{
			// By the time you get here, the default constructor has completed,
			// so if an exception is thrown below, the destructor will be called.
			std::for_each(first, last, [this](auto&& item) { push_back(item); });
		}

		// O(n)
		list(std::initializer_list<T> init) : list<T>(init.begin(), init.end()) {}

		// O(n) - copy constructor
		constexpr list(list const& other)
			: list{ other.begin(), other.end() }
		{}

		// O(n) copy assignment 
		// - because although we just swap pointers, note list is copied - first arg is copied
		// note also that we don't need to define a copy assignment move operator - this one does it!
		// If called with an rvalue reference will use move constructor to create other and then swap with the current state.
		list& operator=(list other)
		{
			swap(*this, other);
			return *this;
		}

		// O(1) move constructor
		list(list&& other) noexcept : list() {
			swap(*this, other);
		}

		// O(n)
		~list() {
			clear();
		}

		// O(1) - just 3 swaps
		friend void swap(list& first, list& second) // nothrow
		{
			// by swapping the members of list,
			// first and second are effectively swapped
			std::swap(first.size_, second.size_);
			std::swap(first.head_, second.head_);
			std::swap(first.tail_, second.tail_);
		}

		// O(n)
		void clear() {
			node* current = head_;
			while (current) {
				node* next = current->next;
				delete current;
				current = next;
			}
			head_ = nullptr;
			tail_ = nullptr;
			size_ = 0;
		}

		// O(1)
		bool empty() const {
			return head_ == nullptr;
		}

		// O(n)
		bool operator==(const list<T>& other) const {

			if (size_ != other.size()) {
				return false;
			}

			for (const_iterator it = begin(), it2 = other.begin(); it != end(); ++it, ++it2) {
				if (*it != *it2) {
					return false;
				}
			}

			return true;
		}

		// O(1)
		// pos - iterator before which the content will be inserted. pos may be the end() iterator
		// returns iterator pointing to the inserted value
		iterator insert(iterator pos, const T& value) {
			node* inserted = new node;
			inserted->value = value;
			inserted->next = pos.ptr_;

			// if pos.ptr_ is null means inserting at end of list
			if (pos.ptr_ == nullptr) {
				inserted->prior = tail_;
				if (tail_ != nullptr) {
					tail_->next = inserted;
				}
				else {
					// if tail_ is null, head_ must also be null
					head_ = tail_ = inserted;
				}
				tail_ = inserted;
			}
			else {
				inserted->prior = pos.ptr_->prior;
				if (pos.ptr_->prior) {
					pos.ptr_->prior->next = inserted;
				}
			}

			// if inserted is now at head_, update head_
			if (inserted->prior == nullptr) {
				head_ = inserted;
			}

			return inserted;
		}

		// O(1)
		void push_back(const T& value) {

			node* newnode = new node{value};

			if (tail_) {
				node* oldtail = tail_;
				oldtail->next = newnode;
				newnode->prior = oldtail;
				tail_ = newnode;
			}
			else {
				head_ = tail_ = newnode;
			}
			++size_;
		}

		// O(1)
		void push_front(const T& value) {

			node* newnode = new node{ value };

			if (head_) {

				node* oldhead = head_;
				oldhead->prior = newnode;
				newnode->next = oldhead;
				head_ = newnode;
			}
			else {
				head_ = tail_ = newnode;
			}
			++size_;
		}

		// O(1)
		size_t size() const {
			return size_;
		}

		// O(1)
		iterator begin() {
			return iterator(head_);
		}
		const_iterator begin() const {
			return const_iterator(head_);
		}

		// O(1)
		iterator end() {
			return nullptr;
		}
		const_iterator end() const {
			return nullptr;
		}

		// O(1)
		T& front() { return *iterator(head_); }
		const T& front() const { return *iterator(head_); }

		// O(1)
		T& back() { return *iterator(tail_); }
		const T& back() const { return *iterator(tail_); }

		// O(1)
		void pop_back() {
			if (tail_) {
				node* newtail = tail_->prior;
				if (newtail) {
					newtail->next = nullptr;
				}

				delete tail_;
				tail_ = newtail;
				--size_;
				// TODO INVESTIGATE better way to handle this
				if (size_ == 0) {
					head_ = nullptr;
				}
			}
		}

		// O(1)
		void pop_front() {
			if (head_) {
				node* newhead = head_->next;
				if (newhead) {
					newhead->prior = nullptr;
				}

				delete head_;
				head_ = newhead;
				--size_;
			}
		}

		// O(n)
		size_t remove(const T& value) {
			size_t count{ 0 };
			node* current = head_;
			while (current) {
				node* next = current->next;

				// if value exists, remove item
				if (current->value == value) {
					/*iterator next_it =*/ erase(current);
					//next = next_it.ptr_;
					++count;
				}

				current = next;
			}
			return count;
		}

		// O(1)
		// pos must be dereferenceable - ie cannot pass in end
		// return iterator following the last removed element
		iterator erase(iterator pos) {
			node* before = pos.ptr_->prior;
			node* after = pos.ptr_->next;

			//  remove pos
			if (before) {
				before->next = after;
			}
			else {
				head_ = after;
			}
			if (after) {
				after->prior = before;
			}
			else {
				// if there is no element after, means that head_ must be set to before
				tail_ = before;
			}

			--size_;

			delete pos.ptr_;
			pos.ptr_ = nullptr;

			return iterator(after);
		}

		// O(1)
		// pos - element before which the content will be inserted
		void splice(iterator pos, list& other) {
			// this size becomes += other.size()
			this->size_ += other.size();

			// other size becomes zero
			other.size_ = 0;

			// pos->prior->next point to other.begin()
			node* priornode = pos.ptr_->prior;
			if (priornode) {
				iterator beg(other.begin());
				priornode->next = beg.ptr_;
			}
			else {
				// reset head to be start of other list
				head_ = other.head_;
			}

			// pos->prior point to last node in other
			pos.ptr_->prior = other.tail_;

			// last in other point to pos
			other.tail_->next = pos.ptr_;

			// other head_ point to node prior to pos
			other.head_->prior = priornode;
			other.head_ = other.tail_ = nullptr;
		}

		// O(n)
		void reverse() {
			node* current = head_;
			while (current) {
				node* next = current->next;

				// swap prior & next
				node* tmp = current->next;
				current->next = current->prior;
				current->prior = tmp;

				current = next;
			}

			// finally we swap head_ & tail_
			node* oldhead = head_;
			head_ = tail_;
			tail_ = oldhead;
		}

		// O(1)
		template<typename... Args>
		void emplace_back(Args&&... v)
		{
			node* newnode = new node{ T(std::forward<Args>(v)...) };
			if (tail_) {
				node* oldtail = tail_;
				oldtail->next = newnode;
				newnode->prior = oldtail;
				tail_ = newnode;
			}
			else {
				head_ = tail_ = newnode;
			}
			++size_;
		}

	private:
		node* head_ = nullptr;
		node* tail_ = nullptr;
		size_t size_ = 0;
	};

}  // namespace wheel

#endif // LIST_HPP_
