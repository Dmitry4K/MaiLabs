#pragma once
#include <iterator>
#include <memory>


namespace containers {
	
	template<class T, class Allocator = std::allocator<T>>
	class list {
	private:
		struct element;//объявление типа хранящегося в list, для того, чтобы он был виден forward_iterator
		size_t size = 0;//размер списка
	public:
		list() = default;//коструктор по умолчанию

		class forward_iterator {
		public:
			using value_type = T;
			using reference = value_type& ;
			using pointer = value_type* ;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::forward_iterator_tag;
			explicit forward_iterator(element* ptr);
			T& operator*();
			forward_iterator& operator++();
			forward_iterator operator++(int);
			bool operator== (const forward_iterator& other) const;
			bool operator!= (const forward_iterator& other) const;
		private:
			element* it_ptr;
			friend list;
		};

		forward_iterator begin();
		forward_iterator end();
		void push_back(const T& value);
		void push_front(const T& value);
		T& front();
		T& back();
		void pop_back();
		void pop_front();
		size_t length();
		bool empty();
		void delete_by_it(forward_iterator d_it);
		void delete_by_number(size_t N);
		void insert_by_it(forward_iterator ins_it, T& value);
		void insert_by_number(size_t N, T& value);
		list& operator=(list& other);
		T& operator[](size_t index);
	private:
		using allocator_type = typename Allocator::template rebind<element>::other;
		
		struct deleter {
		private:
			allocator_type* allocator_;
		public:
			deleter(allocator_type* allocator) : allocator_(allocator) {}

			void operator() (element* ptr) {
				if (ptr != nullptr) {
					std::allocator_traits<allocator_type>::destroy(*allocator_, ptr);
					allocator_->deallocate(ptr, 1);
				}
			}

		};

		using unique_ptr = std::unique_ptr<element, deleter>;
		struct element {
			T value;
			unique_ptr next_element = { nullptr, deleter{nullptr} };
			element* prev_element = nullptr;
			element(const T& value_) : value(value_) {}
			forward_iterator next();
		};

		allocator_type allocator_{};
		unique_ptr first{ nullptr, deleter{nullptr} };
		element* tail = nullptr;
	};

	template<class T, class Allocator>
	typename list<T, Allocator>::forward_iterator list<T, Allocator>::begin() {//+
		return forward_iterator(first.get());
	}

	template<class T, class Allocator>
	typename list<T, Allocator>::forward_iterator list<T, Allocator>::end() {//+
		return forward_iterator(nullptr);
	}
	template<class T, class Allocator>
	size_t list<T, Allocator>::length() {//+
		return size;
	}
	template<class T, class Allocator>
	bool list<T, Allocator>::empty() {
		return length() == 0;
	}
	
	template<class T, class Allocator>
	void list<T, Allocator>::push_back(const T& value) {
		element* result = this->allocator_.allocate(1); 
		std::allocator_traits<allocator_type>::construct(this->allocator_, result, value);
		if (!size) {
			first = unique_ptr(result, deleter{ &this->allocator_ });
			tail = first.get();
			size++;
			return;
		}
		tail->next_element = unique_ptr(result, deleter{ &this->allocator_ });
		element* temp =  tail;//?
		tail = tail->next_element.get();
		tail->prev_element = temp;//?
		size++;
		//first = push_impl(std::move(first),nullptr, value);
		//size++;
	}

	template<class T, class Allocator>
	void list<T, Allocator>::push_front(const T& value) {
		size++;
		element* result = this->allocator_.allocate(1);
		std::allocator_traits<allocator_type>::construct(this->allocator_, result, value);
		unique_ptr tmp = std::move(first);
		first = unique_ptr(result, deleter{ &this->allocator_ });
		first->next_element = std::move(tmp);
		if(first->next_element != nullptr)
			first->next_element->prev_element = first.get();
		if (size == 1) {
			tail = first.get();
		}
		if (size == 2) {
			tail = first->next_element.get();
		}
	}

	template<class T, class Allocator>
	void list<T, Allocator>::pop_front() {
		if (size == 0) {
			throw std::logic_error("can`t pop from empty list");
		}
		if (size == 1) {
			first = nullptr;
			tail = nullptr;
			size--;
			return;
		}
		unique_ptr tmp = std::move(first->next_element);
		first = std::move(tmp);
		//if (first->prev_element)
		first->prev_element = nullptr;
		size--;
		//if (size == 1)
			//tail = first.get();
	}

	template<class T, class Allocator>
	void list<T, Allocator>::pop_back() {
		if (size == 0) {
			throw std::logic_error("can`t pop from empty list");
		}
		 //tmp = nullptr;
		if (tail->prev_element){
			element* tmp = tail->prev_element;
			//unique_ptr dump = std::move(tail->prev_element->next_element);
			tail->prev_element->next_element = nullptr;
			tail = tmp;
			//size--;
		}
		else{
			first = nullptr;
			tail = nullptr;
			//tmp = first.get();
			//unique_ptr dump = std::move(first);
		}
		//tail = tmp;
		//first = pop_impl(std::move(first));
		size--;
	}
	

	template<class T, class Allocator>
	T& list<T, Allocator>::front() {
		if (size == 0) {
			throw std::logic_error("list is empty");
		}
		return first->value;
	}

	template<class T, class Allocator>
	T& list<T, Allocator>::back() {
		if (size == 0) {
			throw std::logic_error("list is empty");
		}
		forward_iterator i = this->begin();
		while ( i.it_ptr->next() != this->end()) {
			i++;
		}
		return *i;
	}
	template<class T, class Allocator>
	list<T,Allocator>& list<T, Allocator>::operator=(list<T, Allocator>& other) {
		size = other.size;
		first = std::move(other.first);
	}

	template<class T, class Allocator>
	void list<T, Allocator>::delete_by_it(containers::list<T, Allocator>::forward_iterator d_it) {
		forward_iterator i = this->begin(), end = this->end();
		if (d_it == end) throw std::logic_error("out of borders");
		if (d_it == this->begin()) {
			this->pop_front();
			return;
		}
		if (d_it.it_ptr == tail) {
			this->pop_back();
			return;
		}
		/*
		while ((i.it_ptr != nullptr) && (i.it_ptr->next() != d_it)) {
			++i;
		}
		*/
		if (d_it.it_ptr == nullptr) throw std::logic_error("out of broders");
		auto temp = d_it.it_ptr->prev_element;
		unique_ptr temp1 = std::move(d_it.it_ptr->next_element);
		d_it.it_ptr->prev_element->next_element = std::move(temp1);
		d_it.it_ptr = d_it.it_ptr->prev_element;
		d_it.it_ptr->next_element->prev_element = temp;
		/*
		if (i.it_ptr == nullptr) throw std::logic_error("out of borders");
		i.it_ptr->next_element = std::move(d_it.it_ptr->next_element);
		*/
		size--;
	}

	template<class T, class Allocator>
	void list<T, Allocator>::delete_by_number(size_t N) {
		forward_iterator it = this->begin();
		for (size_t i = 0; i < N; ++i) {
			++it;
		}
		this->delete_by_it(it);
	}

	template<class T, class Allocator>
	void list<T, Allocator>::insert_by_it(containers::list<T, Allocator>::forward_iterator ins_it, T& value) {
		//auto tmp = unique_ptr(new element{ value });
		element* tmp = this->allocator_.allocate(1);
		std::allocator_traits<allocator_type>::construct(this->allocator_, tmp, value);

		forward_iterator i = this->begin();
		if (ins_it == this->begin()) {
			this->push_front(value);
		/*	tmp->next_element = std::move(first);
			first = unique_ptr(tmp, deleter{ &this->allocator_ });
			if(first->next_element)
			first->next_element->prev_element = first.get();
			//??????
			*/
			//size++;
			return;
		}
		if(/*ins_it.it_ptr == tail || */ins_it.it_ptr == nullptr){
			this->push_back(value);
			return;
		}
		/*
		while ((i.it_ptr != nullptr) && (i.it_ptr->next() != ins_it)) {
			++i;
		}
		/*
		if(ins_it.it_ptr->prev_element == nullptr) throw std::logic_error("out of borders");
		auto tmp1 = ins_it.it_ptr->prev_element;
		tmp->next_element = std::move(ins_it.it_ptr->prev_element->next_element);
		tmp->prev_element = tmp1;
		tmp->next_element->prev_element = tmp.get();
		ins_it.it_ptr->prev_element->next_element = std::move(tmp);
		*/
		tmp->prev_element = ins_it.it_ptr->prev_element;
		ins_it.it_ptr->prev_element = tmp;
		//unique_ptr(result, deleter{ &this->allocator_ });
		tmp->next_element = unique_ptr(ins_it.it_ptr, deleter{ &this->allocator_ });
		tmp->prev_element->next_element = unique_ptr(tmp, deleter{ &this->allocator_ });

		size++;
	}

	template<class T, class Allocator>
	void list<T, Allocator>::insert_by_number(size_t N, T& value) {
		forward_iterator it = this->begin();
		if (N >= this->length())
			it = this->end();
		else
		for (size_t i = 0; i < N; ++i) {
			++it;
		} 
		this->insert_by_it(it, value);
	}
	template<class T, class Allocator>
	typename list<T,Allocator>::forward_iterator list<T, Allocator>::element::next() {
		return forward_iterator(this->next_element.get());
	}

	template<class T, class Allocator>
	list<T, Allocator>::forward_iterator::forward_iterator(containers::list<T, Allocator>::element *ptr) {
		it_ptr = ptr;
	}

	template<class T, class Allocator>
	T& list<T, Allocator>::forward_iterator::operator*() {
		return this->it_ptr->value;
	}
	template<class T, class Allocator>
	T& list<T, Allocator>::operator[](size_t index) {
		if (index < 0 || index >= size) {
			throw std::out_of_range("out of list's borders");
		}
		forward_iterator it = this->begin();
		for (size_t i = 0; i < index; i++) {
			it++;
		}
		return *it;
	}

	template<class T, class Allocator>
	typename list<T, Allocator>::forward_iterator& list<T, Allocator>::forward_iterator::operator++() {
		if (it_ptr == nullptr) throw std::logic_error("out of list borders");
		*this = it_ptr->next();
		return *this;
	}

	template<class T, class Allocator>
	typename list<T, Allocator>::forward_iterator list<T, Allocator>::forward_iterator::operator++(int) {
		forward_iterator old = *this;
		++*this;
		return old;
	}

	template<class T, class Allocator>
	bool list<T, Allocator>::forward_iterator::operator==(const forward_iterator& other) const {
		return it_ptr == other.it_ptr;
	}

	template<class T, class Allocator>
	bool list<T, Allocator>::forward_iterator::operator!=(const forward_iterator& other) const {
		return it_ptr != other.it_ptr;
	}
}