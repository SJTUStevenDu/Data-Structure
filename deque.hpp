#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"
#include "vector.hpp"

#include <cstddef>
#include <iostream>
namespace sjtu { 

	template<class T>
	class deque {
	public:
		vector <T> v1, v2;
		size_t l1, r1, l2, r2;

		T& Get(size_t pos) {
			if (pos >= r1 - l1) 
				return v2[l2 + (pos - r1 + l1)];
			return v1[r1 - pos - 1];
		}

		const T& Get(size_t pos) const {
			if (pos >= r1 - l1) 
				return v2[l2 + (pos - r1 + l1)];
			return v1[r1 - pos - 1];
		}
		
		class const_iterator;
		class iterator {
			friend class deque;
			friend class const_iterator;
		private:
			size_t pos;
			deque *cur;
		public:
			iterator (size_t _pos = 0, const deque *_cur = NULL) {
				pos = _pos;
				cur = (deque *)_cur;
			}

			iterator operator+(const int &n) const {
				if (n + pos < 0 || n + pos > cur -> size())
					throw index_out_of_bound();
				return iterator(pos + n, cur);
			}
			iterator operator-(const int &n) const {
				if (pos - n < 0 || pos - n > cur -> size())
					throw index_out_of_bound();
				return iterator(pos - n, cur);
			}

			int operator-(const iterator &rhs) const {
				if (cur != rhs.cur)
					throw invalid_iterator();
				return (pos - rhs.pos);
			}
			iterator operator+=(const int &n) {
				if (n + pos < 0 || n + pos > cur -> size())
					throw index_out_of_bound();
				pos += n;
				return *this;
			}
			iterator operator-=(const int &n) {
				if (pos - n < 0 || pos - n > cur -> size())
					throw index_out_of_bound();
				pos -= n;
				return *this;
			}
			
			iterator operator++(int) {
				if (1 + pos > cur -> size() || pos + 1 < 0)
					throw index_out_of_bound();
				pos++;
				return iterator(pos - 1, cur);
			}
			iterator& operator++() {
				if (1 + pos < 0 || 1 + pos > cur -> size())
					throw index_out_of_bound();
				pos++;
				return *this;
			}
			iterator operator--(int) {
				if (pos - 1 > cur -> size() || pos - 1 < 0)
					throw index_out_of_bound();
				pos--;
				return iterator(pos + 1, cur);
			}
			iterator& operator--() {
				if (pos - 1 > cur -> size() || pos - 1 < 0)
					throw index_out_of_bound();
				pos--;
				return *this;
			}
		        
			T& operator*() const {
				return cur -> Get(pos);
			}
			T* operator->() const noexcept {
				return &(cur -> Get(pos));
			}
		        
			bool operator==(const iterator &rhs) const {
				return (pos == rhs.pos && cur == rhs.cur);
			}
			bool operator==(const const_iterator &rhs) const {
				return (pos == rhs.pos && cur == rhs.cur);
			}
		        
			bool operator!=(const iterator &rhs) const {
				return (pos != rhs.pos || cur != rhs.cur);
			}
			bool operator!=(const const_iterator &rhs) const {
				return (pos != rhs.pos || cur != rhs.cur);
			}
		};
		class const_iterator {
			friend class deque;
			friend class iterator;
		private:
			size_t pos;
			const deque *cur;
		public:
			const_iterator (size_t _pos = 0, const deque *_cur = NULL) {
				pos = _pos;
				cur = _cur;
			}
			const_iterator (const iterator &rhs) {
				pos = rhs.pos;
				cur = (const deque *)rhs.cur;
			}
			
			const_iterator operator+(const int &n) const {
				if (n + pos < 0 || n + pos > cur -> size())
					throw index_out_of_bound();
				return const_iterator(pos + n, cur);
			}
			const_iterator operator-(const int &n) const {
				if (pos - n < 0 || pos - n > cur -> size())
					throw index_out_of_bound();
				return const_iterator(pos - n, cur);
			}
			int operator-(const const_iterator &rhs) const {
				if (cur != rhs.cur)
					throw invalid_iterator();
				return (pos - rhs.pos);
			}
			const_iterator operator+=(const int &n) {
				if (n + pos < 0 || n + pos > cur -> size())
					throw index_out_of_bound();
				pos += n;
				return *this;
			}
			const_iterator operator-=(const int &n) {
				if (pos - n < 0 || pos - n > cur -> size())
					throw index_out_of_bound();
				pos -= n;
				return *this;
			}
			
			const_iterator operator++(int) {
				if (1 + pos > cur -> size() || pos + 1 < 0)
					throw index_out_of_bound();
				pos++;
				return const_iterator(pos - 1, cur);
			}
			const_iterator& operator++() {
				if (1 + pos < 0 || 1 + pos > cur -> size())
					throw index_out_of_bound();
				pos++;
				return *this;
			}
			const_iterator operator--(int) {
				if (pos - 1 > cur -> size() || pos - 1 < 0)
					throw index_out_of_bound();
				pos--;
				return const_iterator(pos + 1, cur);
			}

			const_iterator& operator--() {
				if (pos - 1 > cur -> size() || pos - 1 < 0)
					throw index_out_of_bound();
				pos--;
				return *this;
			}

			const T& operator*() const {
				return cur -> Get(pos);
			}
			const T* operator->() const noexcept {
				return &(cur -> Get(pos));
			}

			bool operator==(const iterator &rhs) const {
				return (pos == rhs.pos && cur == rhs.cur);
			}
			bool operator==(const const_iterator &rhs) const {
				return (pos == rhs.pos && cur == rhs.cur);
			}
			bool operator!=(const iterator &rhs) const {
				return (pos != rhs.pos || cur != rhs.cur);
			}
			bool operator!=(const const_iterator &rhs) const {
				return (pos != rhs.pos || cur != rhs.cur);
			}
		};
		
		deque() {
			l1 = r1 = l2 = r2 = 0;
		}
		deque(const deque &other) : v1(other.v1), v2(other.v2) {
		        l1 = other.l1;
			l2 = other.l2;
			r1 = other.r1;
			r2 = other.r2;
		}

		~deque() {}
		deque &operator=(const deque &other) {
			if (this == &other)
				return *this;
			v1 = other.v1;
			v2 = other.v2;
			l1 = other.l1;
			l2 = other.l2;
			r1 = other.r1;
			r2 = other.r2;
			return *this;
		}

		T & at(const size_t &pos) {
			if (pos < 0 || pos >= size())
				throw index_out_of_bound();
			return Get(pos);
		}
		const T & at(const size_t &pos) const {
			if (pos < 0 || pos >= size())
				throw index_out_of_bound();
			return Get(pos);
		}
		T & operator[](const size_t &pos) {
			if (pos < 0 || pos >= size())
				throw index_out_of_bound();
			return Get(pos);
		}
		const T & operator[](const size_t &pos) const {
			if (pos < 0 || pos >= size())
				throw index_out_of_bound();
			return Get(pos);
		}

		const T & front() const {
			return operator[](0);
		}

		const T & back() const {
			return operator[](size() - 1);
		}
	        
		iterator begin() {
			return iterator(0, this);
		}
		const_iterator cbegin() const {
			return const_iterator(0, this);
		}
	        
		iterator end() {
			return iterator(size(), this);
		}
		const_iterator cend() const {
			return const_iterator(size(), this);
		}
	        
		bool empty() const {
			return size() == 0;
		}
	        
		size_t size() const {
			return r1 + r2 - l1 - l2;
		}
	        
		void clear() {
			v1.clear(); v2.clear();
			l1 = r1 = l2 = r2 = 0;
		}
	        
		iterator insert(iterator ps, const T &value) {
			if (ps.cur != this)
				throw invalid_iterator();
			if (ps.pos < 0 || ps.pos > size())
				throw index_out_of_bound();
			size_t pos = ps.pos;
			if (ps.pos >= r1 - l1) {
				size_t now = l2 + (pos - (r1 - l1));
				v2.insert(now, value);
				++r2;
			}
			else {
				size_t now = r1 - pos - 1;
				v1.insert(now + 1, value);
				++r1;
			}
			return ps;
		}
	        
		iterator erase(iterator ps) {
			if (ps.cur != this)
				throw invalid_iterator();
			if (ps.pos < 0 || ps.pos > size())
				throw index_out_of_bound();
			size_t pos = ps.pos;
			if (ps.pos >= r1 - l1) {
				size_t now = l2 + (pos - (r1 - l1));
				v2.erase(now);
				--r2;
			}
			else {
				size_t now = r1 - pos - 1;
				v1.erase(now);
				--r1;
			}
			return ps;
		}

		void push_back(const T &value) {
			v2.push_back(value);
			++r2;
		}
		void pop_back() {
			if (empty())
				throw container_is_empty();
			if (v2.empty()) 
				++l1;
			else {
				v2.pop_back();
				--r2;
			}
		}
	        
		void push_front(const T &value) {
			v1.push_back(value);
			++r1;
		}

		void pop_front() {
			if (empty())
				throw container_is_empty();
			if (v1.empty())
				++l2;
			else {
				v1.pop_back();
				--r1;
			}
		}
	};

}

#endif
