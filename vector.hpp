#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <cstdlib>
#include <vector>

using namespace std;

namespace sjtu {
	
template<typename T>
class vector {
public:
        T* data;
	size_t max_size, cur_size;
	class const_iterator;
	
	class iterator {
	private:
		vector* cur;
		size_t pos;
	public:
	        iterator(vector* _cur, size_t _pos) : cur(_cur), pos(_pos) {}
		iterator(const vector* _cur, size_t _pos) : cur(const_cast<vector *>(_cur)), pos(_pos) {}
		iterator operator+(const int &n) const {
			return iterator(cur, pos + n);
		}
		iterator operator-(const int &n) const {
			return iterator(cur, pos - n);
		}
		int operator-(const iterator &rhs) const {
			if (rhs.cur != cur) {
				throw invalid_iterator();
			}
			else
				return abs((int)pos - (int)rhs.pos); 
		}
		int operator-(const const_iterator &rhs) const {
			if (rhs.cur != cur) {
				throw invalid_iterator();
			}
			else
				return abs((int)pos - (int)rhs.pos); 
		}
		iterator operator+=(const int &n) {
			return *this + n;
		}
		iterator operator-=(const int &n) {
			return *this - n;
		}
		iterator operator++(int) {
			return iterator(cur, pos + 1);
		}
		iterator& operator++() {
			++pos;
			return *this;
		}
		iterator operator--(int) {
			return iterator(cur, pos - 1);
		}
		iterator& operator--() {
			--pos;
			return *this;
		}
		T& operator*() const{
		        return (*cur)[pos];
		}
		bool operator==(const iterator &rhs) const {
			if (cur == rhs.cur && pos == rhs.pos)
				return true;
			else
				return false;
		}
		bool operator==(const const_iterator &rhs) const {
		        if (cur == rhs.cur && pos == rhs.pos)
				return true;
			else
				return false;
		}
		bool operator!=(const iterator &rhs) const {
			if (cur == rhs.cur && pos == rhs.pos)
				return false;
			else
				return true;
		}
		bool operator!=(const const_iterator &rhs) const {
			if (cur == rhs.cur && pos == rhs.pos)
				return false;
			else
				return true;
		}
	};
		
	class const_iterator {
	private:
		vector* cur;
		size_t pos;
	public:
	        const_iterator(vector* _cur, size_t _pos) : cur(_cur), pos(_pos) {}
		const_iterator(const vector* _cur, size_t _pos) : cur(const_cast<vector *>(_cur)), pos(_pos) {}
		
		const_iterator operator+(const int &n) const {
			return const_iterator(cur, pos + n);
		}
		const_iterator operator-(const int &n) const {
			return const_iterator(cur, pos - n);
		}
		int operator-(const iterator &rhs) const {
			if (rhs.cur != cur) {
				throw invalid_iterator();
			}
			else
				return abs((int)pos - (int)rhs.pos); 
		}
		const_iterator operator+=(const int &n) {
			return *this + n;
		}
		const_iterator operator-=(const int &n) {
			return *this - n;
		}
		const_iterator operator++(int) {
			return iterator(cur, pos + 1);
		}
		const_iterator& operator++() {
		        ++pos;
			return *this;
		}
		const_iterator operator--(int) {
			return iterator(cur, pos - 1);
		}
		const_iterator& operator--() {
			--pos;
			return *this;
		}
		T& operator*() const{
			return (*cur)[pos];
		}
		bool operator==(const iterator &rhs) const {
			if (cur == rhs.cur && pos == rhs.pos)
				return true;
			else
				return false;
		}
		bool operator==(const const_iterator &rhs) const {
		        if (cur == rhs.cur && pos == rhs.pos)
				return true;
			else
				return false;
		}
		bool operator!=(const iterator &rhs) const {
			if (cur == rhs.cur && pos == rhs.pos)
				return false;
			else
				return true;
		}
		bool operator!=(const const_iterator &rhs) const {
			if (cur == rhs.cur && pos == rhs.pos)
				return false;
			else
				return true;
		}
	};
	vector(int _max_size = 32) {
		max_size = _max_size;
		cur_size = 0;
		data = (T*) malloc(sizeof(T) * max_size);
		memset(data, 0, sizeof(T) * max_size);
	}
	vector(const vector<T> &other) {
		max_size = other.max_size;
		cur_size = other.cur_size;
		data = (T*) malloc(sizeof(T) * max_size);
		memset(data, 0, sizeof(T) * max_size);
		for (size_t i = 0; i != cur_size; ++i) {
			data[i] = other.data[i];
		}
	}
	vector(const std::vector<T> &other) {
		max_size = cur_size = other.size();
		data = (T*) malloc(sizeof(T) * max_size);
		memset(data, 0, sizeof(T) * max_size);
		for (size_t i = 0; i != cur_size; ++i) {
			data[i] = other[i];
		}
	}
	~vector() {
		for (size_t i = 0; i != cur_size; ++i) {
			data[i].~T();
		}
		free(data);
	}
	vector<T>& operator=(const vector<T> &other) {
		if (this == &other)
			return *this;
		for (size_t i = 0; i != cur_size; ++i) {
			data[i].~T();
		}
		free(data);
		
		cur_size = other.cur_size;
		max_size = other.max_size;
		data = (T*) malloc(sizeof(T) * max_size);
		memset(data, 0, sizeof(T) * max_size);
		for (size_t i = 0; i != cur_size; ++i) {
			data[i] = other.data[i];
		}
		return *this;
	}
	T & at(const size_t &pos) {
		if (pos < 0 || pos >= cur_size) {
			throw index_out_of_bound();
		}
		return data[pos];
	}
	const T & at(const size_t &pos) const {
		if (pos < 0 || pos >= cur_size) {
			throw index_out_of_bound();
		}
		return data[pos];
	}
	T & operator[](const size_t &pos) {
		if (pos < 0 || pos >= cur_size) {
			throw index_out_of_bound();
		}
		return data[pos];
	}
	const T & operator[](const size_t &pos) const {
		if (pos < 0 || pos >= cur_size) {
			throw index_out_of_bound();
		}
		return data[pos];
	}
	
	const T & front() const {
		if (cur_size == 0)
			throw container_is_empty();
		return data[0];
	}
	const T & back() const {
		if (cur_size == 0)
			throw container_is_empty();
		return data[cur_size - 1];
	}

	iterator begin() {
		return iterator(this, 0);
	}
	const_iterator cbegin() const {
		return const_iterator(this, 0);
	}
	iterator end() {
		return iterator(this, cur_size);
	}
	const_iterator cend() const {
		return const_iterator(this, cur_size);
	}
	bool empty() const {
		if (cur_size == 0)
			return true;
		return false;
	}
	size_t size() const {return cur_size;}
	size_t capacity() const {return max_size;}

	void clear() {
		cur_size = 0;
		memset(data, 0, sizeof(T) * max_size);
	}

	void DoubleSpace() {
		T* newdata;
		newdata =(T*)malloc(sizeof(T) * max_size * 2);
		max_size = max_size * 2;
		memset(newdata, 0, sizeof(T) * max_size);
		for (size_t i = 0; i != cur_size; ++i)
			newdata[i] = data[i];
		for (size_t i = 0; i != cur_size; ++i) {
			data[i].~T();
		}
		free(data);
		data = newdata;
	}
	
	iterator insert(iterator pos, const T &value) {
		if (cur_size == max_size)
			DoubleSpace();
		cur_size++;
		for (iterator it = end() - 1; it != pos; --it)
			*it = *(it - 1);
		*pos = value;
		return pos;
	}
	iterator insert(const size_t &ind, const T &value) {
		if (ind < 0 || ind > cur_size) {
			throw index_out_of_bound();
		}
		if (cur_size == max_size)
			DoubleSpace();
		cur_size++;
		for (size_t i = cur_size - 1; i != ind; --i) {
			data[i] = data[i - 1];
		}
		data[ind] = value;
		return iterator(this, ind);
	}
	
	iterator erase(iterator pos) {
	        for (iterator it = pos; it != end() - 1; ++it) {
			*it = *(it + 1);
		}
		data[cur_size - 1].~T();
		--cur_size;
		return pos;
	}
	iterator erase(const size_t &ind) {
		if (ind < 0 || ind >= cur_size) {
			throw index_out_of_bound();
		}
		for (size_t i = ind; i != cur_size; ++i) 
			data[i] = data[i + 1];
		data[cur_size - 1].~T();
		--cur_size;
		return iterator(this, ind);
	}

	void push_back(const T &value) {
		if (cur_size == max_size)
			DoubleSpace();
		data[cur_size++] = value;
	}
	void pop_back() {
		if (cur_size == 0)
			throw container_is_empty();
		data[cur_size - 1].~T();
		--cur_size;
	}
};


}

#endif
