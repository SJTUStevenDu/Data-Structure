#ifndef SJTU_UNORDERED_MAP_HPP
#define SJTU_UNORDERED_MAP_HPP

#include <functional>
#include <cstddef>
#include "exceptions.hpp"
#include "utility.hpp"

namespace sjtu {

	template<
		class Key, 
		class T,
		class Hash = std::hash<Key>,
		class KeyEqual = std::equal_to<Key>
		> class unordered_map {
	public:
		typedef pair<const Key, T> value_type;

		struct Node {
			value_type *data;
			Node *nxt;
			Node () {
				data = NULL;
				nxt = NULL;
			}
			Node (value_type _data, Node *_nxt) {
				data = new value_type(_data);
				nxt = _nxt;
			}
			~Node () {
				delete data;
			}
			inline Key getK() {
				return data -> first;
			}
			inline T & getT() {
				return data -> second;
			}
		} **storage;
		Node *End;

		size_t cur_size, max_size;

		Hash hash;
		KeyEqual equal;

		const int size_num[15] = {101, 211, 431, 863, 1733, 3467, 6947, 13901, 27803, 55609, 111227, 222461, 444929, 889871, 1779761};

		inline int NextSize(int max_size) {
			for (int i = 1; i < 15; ++i)
				if (size_num[i] > max_size)
					return size_num[i];
		}

		inline Node* Find(value_type v) {
			int h = hash(v.first) % max_size;
			Node *cur = storage[h];
			while (cur != NULL) {
				if (equal(cur -> getK(), v.first))
					return cur;
				cur = cur -> nxt;
			}
			return End;
		}

		inline Node* Find(Key key) {
			size_t h = hash(key);
			for (Node *p = storage[h % max_size]; p != NULL; p = p -> nxt)
				if (equal(p -> getK(), key))
					return p;
			return End;
		}

		inline Node* Insert(value_type v) {
			++cur_size;
			if (cur_size == max_size) {
				DoubleSpace();
				++cur_size;
			}
			int h = hash(v.first) % max_size;
			if (storage[h] == NULL) {
				storage[h] = new Node(v, NULL);
				return storage[h];
			}
			Node *cur = storage[h];
			storage[h] = new Node(v, cur);
			return storage[h];
		}

		inline void Erase(Key key) {
			--cur_size;
			int h = hash(key) % max_size;
			Node *last = storage[h];
			if (equal(last -> getK(), key)) {
				storage[h] = last -> nxt;
				delete last;
				return;
			}
			for (Node *p = storage[h] -> nxt; p != NULL; p = p -> nxt) {
				if (equal(p -> getK(), key)) {
					last -> nxt = p -> nxt;
					delete p;
					return;
				}
				last = p;
			}
			return;
		}

		void DoubleSpace() {
			Node **old_storage = storage;
			max_size = NextSize(max_size);
			size_t tmp_size = cur_size;
			cur_size = 0;
			storage = new Node* [max_size];
			for (size_t i = 0; i < max_size; ++i)
				storage[i] = NULL;
			for (size_t i = 0; i < tmp_size; ++i) {
				for (Node *j = old_storage[i]; j != NULL; j = j -> nxt) 
					Insert(*(j -> data));
				for (Node *j = old_storage[i]; j != NULL; ) {
					Node *tmp = j -> nxt;
					delete j;
					j = tmp;
				}
				old_storage[i] = NULL;
			}
			delete [] old_storage;
		}

		inline Node* FindNext(Node *p) {
			if (p -> nxt != NULL)
				return p -> nxt;
			int h = hash(p -> getK()) % max_size;
			for (int i = h + 1; i < max_size; ++i) {
				if (storage[i] != NULL)
					return storage[i];
			}
			return End;
		}

		inline Node* FindNext1(Node *p) {
			size_t h = hash(p -> getK());
			for (Node *now = p -> nxt; now != NULL; now = now -> nxt)
				if (hash(now -> getK()) == h)
					return now;
			return End;
		}

		inline Node* FindBegin() {
			for (int i = 0; i < max_size; ++i)
				if (storage[i] != NULL)
					return storage[i];
			return End;
		}

		inline Node* FindBegin1(size_t h) {
			for (Node *p = storage[h % max_size]; p != NULL; p = p -> nxt)
				if (hash(p -> getK()) == h)
					return p;
			return End;
		}

		void clear() {
			for (int i = 0; i < max_size; ++i) {
				Node *now = storage[i];
				Node *tmp;
				while (now != NULL) {
					tmp = now -> nxt;
					delete now;
					now = tmp;
				}
				storage[i] = NULL;
			}
			cur_size = 0;
		}
        
		class const_iterator;
		class iterator {
		public:
			unordered_map <Key, T, Hash, KeyEqual> *it;
			Node *pos;
			iterator(unordered_map <Key, T, Hash, KeyEqual> *_it = NULL, Node *_pos = NULL) {
				it = _it;
				pos = _pos;
			}
			iterator operator++(int) {
				iterator ret = *this;
				pos = it -> FindNext(pos);
				return ret;
			}
			iterator& operator++() {
				pos = it -> FindNext(pos);
				return *this;
			}
			value_type& operator*() const {
				return *(pos -> data);
			}
			bool operator==(const iterator &rhs) const {
				return (pos == rhs.pos && it == rhs.it);
			}
			bool operator==(const const_iterator &rhs) const {
				return (pos == rhs.pos && it == rhs.it);
			}
			bool operator!=(const iterator &rhs) const {
				return (pos != rhs.pos || it != rhs.it);
			}
			bool operator!=(const const_iterator &rhs) const {
				return (pos != rhs.pos || it != rhs.it);
			}
			value_type* operator->() const noexcept {
				return pos -> data;
			}
		};
		
		class const_iterator {
		public:
			unordered_map <Key, T, Hash, KeyEqual> *it;
			Node *pos;
			const_iterator(unordered_map <Key, T, Hash, KeyEqual> *_it, Node *_pos) {
				it = _it;
				pos = _pos;
			}
			const_iterator() {
				it = NULL;
				pos = NULL;
			}
			const_iterator(const const_iterator &other) {
				it = other.it;
				pos = other.pos;
			}
			const_iterator(const iterator &other) {
				it = other.it;
				pos = other.pos;
			}
			const_iterator operator++(int) {
				iterator ret = *this;
				pos = it -> FindNext(pos);
				return ret;
			}
			const_iterator& operator++() {
				pos = it -> FindNext(pos);
				return *this;
			}
			value_type& operator*() const {
				return *(pos -> data);
			}
			bool operator==(const iterator &rhs) const {
				return (pos == rhs.pos && it == rhs.it);
			}
			bool operator==(const const_iterator &rhs) const {
				return (pos == rhs.pos && it == rhs.it);
			}
			bool operator!=(const iterator &rhs) const {
				return (pos != rhs.pos || it != rhs.it);
			}
			bool operator!=(const const_iterator &rhs) const {
				return (pos != rhs.pos || it != rhs.it);
			}
			value_type* operator->() const noexcept {
				return pos -> data;
			}
		};

		class const_local_iterator;
		class local_iterator {
		public:
			unordered_map <Key, T, Hash, KeyEqual> *it;
			Node *pos;
		        local_iterator(unordered_map <Key, T, Hash, KeyEqual> *_it = NULL, Node *_pos = NULL) {
				it = _it;
				pos = _pos;
			}
			local_iterator operator++(int) {
				local_iterator ret = *this;
				pos = it -> FindNext1(pos);
				return ret;
			}
			local_iterator& operator++() {
				pos = it -> FindNext1(pos);
				return *this;
			}
			value_type& operator*() const{
				return *(pos -> data);
			}
			bool operator==(const iterator &rhs) const {
				return (pos == rhs.pos && it == rhs.it);
			}
			bool operator==(const const_iterator &rhs) const {
				return (pos == rhs.pos && it == rhs.it);
			}
			bool operator!=(const iterator &rhs) const {
				return (pos != rhs.pos || it != rhs.it);
			}
			bool operator!=(const const_iterator &rhs) const {
				return (pos != rhs.pos || it != rhs.it);
			}
			value_type* operator->() const noexcept {
				return pos -> data;
			}
		};
		
		class const_local_iterator {
		public:
			unordered_map <Key, T, Hash, KeyEqual> *it;
			Node *pos;
			const_local_iterator(unordered_map <Key, T, Hash, KeyEqual> *_it, Node *_pos) {
				it = _it;
				pos = _pos;
			}
			const_local_iterator() {
				it = NULL;
				pos = NULL;
			}
			const_local_iterator(const const_local_iterator &other) {
			        it = other.it;
				pos = other.pos;
			}
			const_local_iterator(const local_iterator &other) {
				it = other.it;
				pos = other.pos;
			}
		        const_local_iterator operator++(int) {
				iterator ret = *this;
				pos = it -> FindNext1(pos);
				return ret;
			}
			const_local_iterator& operator++() {
				pos = it -> FindNext1(pos);
				return *this;
			}
			value_type& operator*() const {
				return *(pos -> data);
			}
			bool operator==(const iterator &rhs) const {
				return (pos == rhs.pos && it == rhs.it);
			}
			bool operator==(const const_iterator &rhs) const {
				return (pos == rhs.pos && it == rhs.it);
			}
			bool operator!=(const iterator &rhs) const {
				return (pos != rhs.pos || it != rhs.it);
			}
			bool operator!=(const const_iterator &rhs) const {
				return (pos != rhs.pos || it != rhs.it);
			}
			value_type* operator->() const noexcept {
				return pos -> data;
			}
		};
		
		unordered_map() {
			max_size = size_num[0];
			cur_size = 0;
			storage = new Node* [max_size];
			for (int i = 0; i < max_size; ++i)
				storage[i] = NULL;
			End = new Node;
		}
		
		unordered_map(const unordered_map &other) {
			max_size = other.max_size;
			cur_size = other.cur_size;
			storage = new Node* [max_size];
			for (int i = 0; i < max_size; ++i) {
				storage[i] = NULL;
			        if (other.storage[i] != NULL) {
					storage[i] = new Node(*(other.storage[i] -> data), NULL);
					Node *now = storage[i];
					for (Node *p = other.storage[i] -> nxt; p != NULL; p = p -> nxt) {
						now -> nxt = new Node(*(p -> data), NULL);
						now = now -> nxt;
					}
				}
			}
			End = new Node;
		}
	        
		~unordered_map() {
			clear();
			delete [] storage;
			delete End;
		}
	        
		unordered_map &operator=(const unordered_map &other) {
			if (this == &other)
				return *this;
			clear();
			delete [] storage;
			max_size = other.max_size;
			cur_size = other.cur_size;
			storage = new Node* [max_size];
			for (int i = 0; i < max_size; ++i) {
				storage[i] = NULL;
			        if (other.storage[i] != NULL) {
					storage[i] = new Node(*(other.storage[i] -> data), NULL);
					Node *now = storage[i];
					for (Node *p = storage[i] -> nxt; p != NULL; p = p -> nxt) {
						now -> nxt = new Node(*(p -> data), NULL);
						now = now -> nxt;
					}
				}
			}
		}
		
		iterator begin() {
			return iterator(this, FindBegin());
		}
		const_iterator cbegin() {
			return const_iterator(this, FindBegin());
		}
	        
		iterator end() {
			return iterator(this, End);
		}
		const_iterator cend() {
			return const_iterator(this, End);
		}
		
		bool empty() const {
			return (cur_size == 0);
		}
		
		size_t size() const {
			return cur_size;
		}
		
		pair<iterator, bool> insert(const value_type &value) {
			Node *ret = Find(value);
			if (ret != End)
				return pair<iterator, bool>(iterator(this, ret), false);
			ret = Insert(value);
			return pair<iterator, bool>(iterator(this, ret), true);
		}
		
		void erase(iterator ps) {
			Erase(ps.pos -> getK());
		}

		size_t count(const Key &key) const {
			if (Find(key) != End)
				return 1;
			else
				return 0;
		}
		
		iterator find(const Key &key) {
			return iterator(this, Find(key));
		}
		const_iterator find(const Key &key) const {
			return const_iterator(this, Find(key));
		}
		
		T & at(const Key &key) {
			Node *ret = Find(key);
			if (ret == End)
				throw index_out_of_bound();
			return ret -> data -> second;
		}
		const T & at(const Key &key) const {
			Node *ret = Find(key);
			if (ret == End)
				throw index_out_of_bound();
			return ret -> data -> second;
		}
		T & operator[](const Key &key) {
			Node *ret = Find(key);
			if (ret == End) {
				ret = Insert(value_type(key, T()));
				return ret -> data -> second; 
			}
			return ret -> data -> second;
		}
		const T & operator[](const Key &key) const {
			Node *ret = Find(key);
			if (ret == End)
				throw index_out_of_bound();
			return ret -> data -> second;
		}

		local_iterator begin(const size_t &n) {
			return local_iterator(this, FindBegin1(n));
		}
		const_local_iterator cbegin(const size_t &n) const {
			return const_local_iterator(this, FindBegin1(n));
		}
		local_iterator end(const size_t &n) {
			return local_iterator(this, End);
		}
		const_local_iterator cend(const size_t &n) const {
			return const_local_iterator(this, End);
		}

		size_t bucket_count() const {
			return max_size;
		}
		size_t bucket_size(const size_t &n) const {
			size_t ret = 0;
			for (Node *p = storage[n % max_size]; p != NULL; p = p -> nxt)
				++ret;
			return ret;
		}
		double load_factor() const {
			return 1.0 * cur_size / max_size;
		}
	};
}

#endif
