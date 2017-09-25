#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu {

	template<typename T>
	class list {
	private:
		struct Node {
			T *data;
			Node *pre, *nxt;
			bool avl;
			
			Node (const T &x, const Node *p = NULL, const Node *n = NULL) {
				pre = (const_cast<Node *> (p));
				nxt = (const_cast<Node *> (n));
				data = (T*) operator new(sizeof(T));
				avl = true;
				new (data)T(x);
			}
			Node () {
				data = (T*) operator new(sizeof(T));
				avl = false;
				nxt = NULL;
				pre = NULL;
			}
			~Node () {
				if (avl) (*data).~T();
				operator delete(data);
			}
		};
	public:
		Node *head, *tail;
		size_t sz;
		
		class const_iterator;
		
		class iterator {
			friend const_iterator;
		private:
			Node *cur;
			const list *bel;
		public:
			iterator (Node *p = NULL, const list* l = NULL) {cur = p; bel = l;}
			//iterator (const Node *p = NULL) {cur = const_cast<Node *>(p);}

			Node* getcur() {
				return cur;
			}
			const list* getbel() {
				return bel;
			}
			
			iterator operator++(int) {
				if (*this == bel -> cend())
				        throw invalid_iterator();
			        Node *ret = cur;
				ret = ret -> nxt;
				return iterator(ret, bel);
			}
			iterator& operator++() {
				if (*this == bel -> cend())
				        throw invalid_iterator();
				cur = cur -> nxt;
				return *this;
			}
			iterator operator--(int) {
				if (*this == bel -> cbegin())
				        throw invalid_iterator();
				Node *ret = cur;
				ret = ret -> pre;
				return iterator(ret, bel);
			}
			iterator& operator--() {
				if (*this == bel -> cbegin())
				        throw invalid_iterator();
				cur = cur -> pre;
				return *this;
			}
			T& operator*() const{
				return *(cur -> data);
			}
			bool operator==(const iterator &rhs) const {
				return (cur == rhs.cur);
			}
			bool operator==(const const_iterator &rhs) const {
				return (cur == rhs.cur);
			}
			bool operator!=(const iterator &rhs) const {
				return (cur != rhs.cur);
			}
			bool operator!=(const const_iterator &rhs) const {
				return (cur != rhs.cur);
			}
			T* operator->() const {
				return cur -> data;
			}
		};
		class const_iterator {
			friend iterator;
		private:
			const Node *cur;
			const list *bel;
		public:
			const_iterator (const Node *p = NULL, const list *l = NULL) {cur = p; bel = l;}
			const_iterator (Node *p = NULL, const list *l = NULL) {cur = p; bel = l;}

			const Node* getcur() {
				return cur;
			}
			const list* getbel() {
				return bel;
			}

			const_iterator operator++(int) {
				if (*this == bel -> cend())
				        throw invalid_iterator();
			        const Node *ret = cur;
				ret = ret -> nxt;
				return const_iterator(ret, bel);
			}
			const_iterator& operator++() {
				if (*this == bel -> cend())
				        throw invalid_iterator();
				cur = cur -> nxt;
				return *this;
			}
			const_iterator operator--(int) {
				if (*this == bel -> cbegin())
				        throw invalid_iterator();
				const Node *ret = cur;
				ret = ret -> pre;
				return const_iterator(ret);
			}
			const_iterator& operator--() {
				if (*this == bel -> cbegin())
				        throw invalid_iterator();
				cur = cur -> pre;
				return *this;
			}
			const T& operator*() const{
				return *(cur -> data);
			}
			bool operator==(const iterator &rhs) const {
				return (cur == rhs.cur);
			}
			bool operator==(const const_iterator &rhs) const {
				return (cur == rhs.cur);
			}
			bool operator!=(const iterator &rhs) const {
				return (cur != rhs.cur);
			}
			bool operator!=(const const_iterator &rhs) const {
				return (cur != rhs.cur);
			}
			const T* operator->() const {
				return cur -> data;
			}
		};
		
		list() {
			head = new Node;
			head -> nxt = tail = new Node;
			tail -> pre = head;
			sz = 0;
		}
		list(const list<T> &other)
		{
			sz = other.size();
			head = new Node;
			tail = new Node;
			head -> nxt = tail;
			tail -> pre = head;
			Node *p = head, *q = NULL;
			for (const_iterator it = other.cbegin(); it != other.cend(); ++it) {
				q = new Node;
				p -> nxt = q;
				q -> pre = p;
				new (q -> data) T (*it);
				q -> avl = 1;
				p = q;
			}
			if (q != NULL) q -> nxt = tail;
		}
		
		~list() {
			clear();
			delete head;
			delete tail;
		}

		list<T> & operator=(const list<T> &other)
		{
			if (this == &other) return *this;
			clear();	
			sz = other.size();
			head -> nxt = tail;
			tail -> pre = head;
			Node *p = head, *q = NULL;
			for (const_iterator it = other.cbegin(); it != other.cend(); ++it) {
				q = new Node;
				p -> nxt = q;
				q -> pre = p;
				new (q -> data) T (*it);
				q -> avl = 1;
				p = q;
			}
			if (q != NULL) {
				q -> nxt = tail;
				tail -> pre = q;
			}
			return *this;
		}
		
		const T & front() const {
			if (sz == 0)
				throw container_is_empty();
			return *(head -> nxt -> data);
		}
		const T & back() const {
			if (sz == 0)
				throw container_is_empty();
			return *(tail -> pre -> data);
		}
		
		iterator begin() {
			return iterator(head -> nxt, this);
		}
		const_iterator cbegin() const {
			return const_iterator(head -> nxt, this);
		}

		iterator end() {
			return iterator(tail, this);
		}
		const_iterator cend() const {
			return const_iterator(tail, this);
		}
		bool empty() const {
			return (sz == 0);
		}
		size_t size() const {
			return sz;
		}
		void clear() {
			Node *p = head -> nxt, *q;
			head -> nxt = tail;
			tail -> pre = head;
			while (p != tail) {
				q = p -> nxt;
				delete p;
				p = q;
			}
			sz = 0;
		}

		iterator insert(iterator pos, const T &value) {
			if (pos.getbel() != this)
			        throw invalid_iterator();
			Node *p = pos.getcur();
			Node *ret = new Node(value, p -> pre, p);
			p -> pre -> nxt = ret;
			p -> pre = ret;
			sz++;
			return iterator(ret, this);
		}
		iterator erase(iterator pos) {
			if (pos.getbel() != this)
			        throw invalid_iterator();
			Node *p = pos.getcur();
			p -> nxt -> pre = p -> pre;
			p -> pre -> nxt = p -> nxt;
			sz--;
			iterator ret(p -> nxt, this);
			delete p;
			return ret;
		}

	        iterator erase(iterator first, iterator last) {
			if (first.getbel() != this || last.getbel() != this)
			        throw invalid_iterator();
			Node *fr = first.getcur(), *lr = last.getcur();
			Node *nfr = fr -> pre;
			iterator now(first);
			Node *q;
			while (fr != last.getcur()) {
				if (now == end())
					throw invalid_iterator();
				q = fr -> nxt;
				--sz;
				delete fr;
				fr = q;
				++now;
			}
			nfr -> nxt = lr;
			lr -> pre = nfr;
			return iterator(lr, this);
		}

		void push_back(const T &value) {
			sz++;
			Node *p = new Node(value, tail -> pre, tail);
			p -> pre -> nxt = p;
			tail -> pre = p;
			return;
		}

		void pop_back() {
			sz--;
			Node *del = tail -> pre;
			tail -> pre = del -> pre;
			del -> pre -> nxt = tail;
			delete del;
			return;
		}

		void push_front(const T &value) {
			sz++;
			Node *p = new Node(value, head, head -> nxt);
		        p -> nxt -> pre = p;
			head -> nxt = p;
			return;
		}

		void pop_front() {
			sz--;
			Node *del = head -> nxt;
			head -> nxt = del -> nxt;
			del -> nxt -> pre = head;
			delete del;
			return;
		}
	};

}

#endif
