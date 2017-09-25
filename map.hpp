/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP
#include <functional>
#include <cstddef>
#include <iostream>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

	template<
		class Key,
		class T,
		class Compare = std::less<Key>
		> class map {

	public:
		typedef pair<const Key, T> value_type;

	private:

		Compare cmp;
		struct Node {
			value_type *data;
			int level;
			Node *left, *right;
			Key getKey() const {
				return data -> first;
			}
			T& getT() {
				return data -> second;
			}
			Node() {
				data = NULL;
				left = right = NULL;
				level = 0;
			}
			Node(const Key key, T data_T) {
				data = new value_type(key, data_T);		
				level = 1;
				left = right = NULL;
			}
			Node(const Node &other) {
				level = other.level;
				data = new value_type(*other.data);
				left = other.left;
				right = other.right;
			}
			~Node() {
				delete data;
			}
		};
		Node *root, *End;
		size_t totsize;

		void LL(Node *&p) {
			if (p -> left != NULL && p -> left -> level == p -> level) {
				Node *q = p -> left;
				p -> left = q -> right;
				q -> right = p;
				p = q;
			}
		}
		void RR(Node *&p) {
			if (p -> right != NULL && p -> right -> right != NULL && p -> right -> right -> level == p -> level){
				Node *q = p -> right;
				p -> right = q -> left;
				q -> left = p;
				p = q;
				(p -> level)++;
			}
		}
		Node* First() const {
			if (totsize == 0)
				return End;
			Node *p = root;
			while (p -> left != NULL)
				p = p -> left;
			return p;
		}
		Node* Last() const {
			if (totsize == 0)
				return End;
			Node *p = root;
			while (p -> right != NULL)
				p = p -> right;
			return p;
		}
		Node* GetNext(const Node* const &now) const {
			if (now == End) return End;
			if (now -> right != NULL) {
				Node *tmp = now -> right;
				while (tmp -> left != NULL)
					tmp = tmp -> left;
				return tmp;
			}
			else {
				Node *last = NULL;
				Node *p = root;
				while (p != now) {
					if (cmp(p -> getKey(), now -> getKey()))
						p = p -> right;
					else {
						last = p;
						p = p -> left;				
					}
				}
				if (last != NULL)
					return last;
				else
					return End;
			}
		}
		Node* GetPrev(const Node* const &now) const {
			if (now == End)
				return Last();
			if (now -> left != NULL) {
				Node *tmp = now -> left;
				while (tmp -> right != NULL)
					tmp = tmp -> right;
				return tmp;
			}
			else {
				Node *last = NULL;
				Node *p = root;
				while (p != now) {
					if (cmp(now -> getKey(), p -> getKey()))
						p = p -> left;
					else {
						last = p;
						p = p -> right;				
					}
				}
				return last;
			}
		}
		/*int Find2(Node* now, const Key &key) const {
			if (now == NULL)
				return 0;
			int ret = 0;
			if ((!cmp(key, now -> getKey()) && !cmp(now -> getKey(), key)))
				ret = 1;
			ret += Find2(now -> left, key);
			ret += Find2(now -> right, key);
			return ret;
			}*/
		Node* Find(Node* const now, const Key &key) const {
			if (now == NULL) 
				return now;
			if (cmp(key, now -> getKey()))
				return Find(now -> left, key);
			else if (cmp(now -> getKey(), key))
				return Find(now -> right, key);
			else
				return now;
		}
		Node* Insert(Node *&now, const Key &key, const T &data_T) {
			Node *ret;
			if (now == NULL) {
				now = new Node(key, data_T);
				ret = now;
			}
			else {
				if(cmp(key, now -> getKey()))
					ret = Insert(now -> left, key, data_T);
				else
					ret = Insert(now -> right, key, data_T);
			}
			LL(now);
			RR(now);
			return ret;
		}
		void Remove(Node *&now, const Key &key) {
			if (now == NULL) return;
			if (cmp(key, now -> getKey()))
				Remove(now -> left, key);
			else if (cmp(now -> getKey(), key))
				Remove(now -> right, key);
			else {
				if (now -> left != NULL && now -> right != NULL) {
					Node *tmp = now -> right;
					while (tmp -> left != NULL)
						tmp = tmp -> left;
					value_type *x = now -> data;
					now -> data = tmp -> data;
					tmp -> data = x;
					
					Remove(now -> right, key);
				}
				else {
					Node *tmp = now;
					if (now -> left != NULL)
						now = now -> left;
					else
						now = now -> right;
					delete tmp;
					return;
				}
			}
			if (now -> left == NULL || now -> right == NULL) 
				now -> level = 1;
			else if (now -> left -> level < now -> right -> level) 
				now -> level = now -> left -> level + 1;
			else
				now -> level = now -> right -> level + 1;
			if (now -> right != NULL && now -> right -> level > now -> level)
				now -> right -> level = now -> level;
			LL(now);
			if (now -> right != NULL)
				LL(now -> right);
			if (now -> right != NULL && now -> right -> right != NULL)
				LL(now -> right -> right);
			RR(now);
			if (now -> right != NULL)
				RR(now -> right);
		}
		Node* Copy(Node* now) {
			if (now == NULL)
				return NULL;
			Node *tmp = new Node(*now);
			tmp -> left = Copy(now -> left);
			tmp -> right = Copy(now -> right);
			return tmp;
		}
		void Clear(Node *now) {
			if (now == NULL)
				return;
			Clear(now -> left);
			Clear(now -> right);
			delete now;
		}
	
	public:
		class iterator;
		class const_iterator;
		class iterator {
		public:
			map <Key, T, Compare> *cur;
			Node *pos;
			iterator(map <Key, T, Compare> *ccur = NULL, Node *ppos = NULL) {
				cur = ccur;
				pos = ppos;
			}
			iterator(const iterator &other) {
				cur = other.cur;
				pos = other.pos;
			}

			iterator operator++(int) {
				if (pos == cur -> End)
					throw invalid_iterator();
				iterator ret(*this);
				pos = cur -> GetNext(pos);
				return ret;
			}
			iterator & operator++() {
				if (pos == cur -> End)
					throw invalid_iterator();
				pos = cur -> GetNext(pos);
				return *this;
			}
			iterator operator--(int) {
				if (pos == cur -> First())
					throw invalid_iterator();
				iterator ret(*this);
				pos = cur -> GetPrev(pos);
				return ret;
			}
			iterator & operator--() {
				if (pos == cur -> First())
					throw invalid_iterator();
				pos = cur -> GetPrev(pos);
				return *this;
			}

			value_type & operator*() const {
				return *(pos -> data);
			}
			bool operator==(const iterator &rhs) const {
				return (cur == rhs.cur && pos == rhs.pos);
			}
			bool operator==(const const_iterator &rhs) const {
				return (cur == rhs.cur && pos == rhs.pos);
			}
			bool operator!=(const iterator &rhs) const {
				return (pos != rhs.pos || cur != rhs.cur);
			}
			bool operator!=(const const_iterator &rhs) const {
				return (pos != rhs.pos || cur != rhs.cur);
			}
			value_type* operator -> () const noexcept {
				return pos -> data;
			}
		};
		class const_iterator {
		public:
			const map <Key, T, Compare>* cur;
			const Node *pos;
		        const_iterator(const map <Key, T, Compare> *ccur = NULL, Node *ppos = NULL) {
				cur = ccur;
				pos = ppos;
			}
			const_iterator(const const_iterator &other) {
				cur = other.cur;
				pos = other.pos;
			}
			const_iterator(const iterator &other) {
				cur = other.cur;
				pos = other.pos;
			}
			const_iterator operator++(int) {
				if (pos == cur -> End)
					throw invalid_iterator();
				const_iterator ret(*this);
				pos = cur -> GetNext(pos);
				return ret;
			}
			const_iterator & operator++() {
				if (pos == cur -> End)
					throw invalid_iterator();
				pos = cur -> GetNext(pos);
				return *this;
			}
			const_iterator operator--(int) {
				if (pos == cur -> First())
					throw invalid_iterator();
				const_iterator ret(*this);
				pos = cur -> GetPrev(pos);
				return ret;
			}
			const_iterator & operator--() {
				if (pos == cur -> First())
					throw invalid_iterator();
				pos = cur -> GetPrev(pos);
				return *this;
			}

			value_type & operator*() const {
				return *(pos -> data);
			}
			bool operator==(const iterator &rhs) const {
				return (cur == rhs.cur && pos == rhs.pos);
			}
			bool operator==(const const_iterator &rhs) const {
				return (cur == rhs.cur && pos == rhs.pos);
			}
			bool operator!=(const iterator &rhs) const {
				return (pos != rhs.pos || cur != rhs.cur);
			}
			bool operator!=(const const_iterator &rhs) const {
				return (pos != rhs.pos || cur != rhs.cur);
			}
			value_type* operator -> () const noexcept {
				return pos -> data;
			}
		};

		map() {
			totsize = 0;
			End = new Node;
			root = NULL;
		}
		map(const map &other) {
			totsize = other.totsize;
			End = new Node;
			root = Copy(other.root);
		}
		map & operator=(const map &other) {
			if (this == &other)
				return *this;
			totsize = other.totsize;
			root = Copy(other.root);
			return *this;
		}
		~map() {
			clear();
			delete End;
		}

		T & at(const Key &key) {
			Node *ret = Find(root, key);
			if (ret == NULL)
				throw index_out_of_bound();
			return ret -> getT();
		}
		const T & at(const Key &key) const {
			Node *ret = Find(root, key);
			if (ret == NULL)
				throw index_out_of_bound();
			return ret -> getT();
		}

		T & operator[](const Key &key) {
			Node *ret = Find(root, key);
			if (ret == NULL) {
				T tmp;
				++totsize;
				ret = Insert(root, key, tmp);
			}
			return ret -> getT();
		}
		const T & operator[](const Key &key) const {
			Node *ret = Find(root, key);
			if (ret == NULL)
				throw index_out_of_bound();
			return ret -> getT();
		}

		iterator begin() {
			return iterator(this, First());
		}
		const_iterator cbegin() const {
			return const_iterator(this, First());
		}
		iterator end() {
			return iterator(this, End);
		}
		const_iterator cend() const {
			return const_iterator(this, End);
		}
		
		bool empty() const {
			return (totsize == 0);
		}
		size_t size() const {
			return totsize;
		}
		void clear() {
			Clear(root);
			totsize = 0;
			root = NULL;
		}
		pair<iterator, bool> insert(const value_type &value) {		
			Node *ret = Find(root, value.first);		
			if (ret != NULL)
				return pair<iterator, bool>(iterator(this, ret), false);
			++totsize;
			
			return pair<iterator, bool>(iterator(this, Insert(root, value.first, value.second)), true);
		}
		void erase(iterator ps) {
			if (ps.pos == End || ps.cur != this)
				throw invalid_iterator();
			--totsize;
			Remove(root, (*ps).first);
		}
		size_t count(const Key &key) const {
			if (Find(root, key) == NULL) 
				return 0;
			else
				return 1;
		}
		iterator find(const Key &key) {
			Node *ret = Find(root, key);
			if (ret == NULL)
				return iterator(this, End);
			else
				return iterator(this, ret);
		}
		const_iterator find(const Key &key) const {
			Node *ret = Find(root, key);
			if (ret == NULL)
				return const_iterator(this, End);
			else
				return const_iterator(this, ret);
		}
	};

}

#endif
