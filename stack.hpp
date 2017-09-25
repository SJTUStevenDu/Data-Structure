#ifndef SJTU_STACK_HPP
#define SJTU_STACK_HPP

#include "vector.hpp"
#include "list.hpp"

namespace sjtu {

/**
 * a container whose behavior is similar to a stack.
 * It should be based on the vector and list you had written before.
 * The default ListType is vector, but you should make sure that 
 * the stack you design is also correct when ListType is sjtu::list.
 */
template<typename T, typename ListType = vector<T>>
class stack {
public:
	ListType data;
	/**
	 * TODO constructors
	 */
	stack() {}
	stack(const stack<T, ListType> &other) {
		data = other.data;
	}
	/**
	 * TODO deconstructor
	 */
	~stack() {}
	/**
	 * TODO Assignment operator
	 */
	stack<T, ListType> &operator=(const stack<T, ListType> &other) {
		if (this == &other)
			return *this;
		data = other.data;
	}
	
	/**
	 * get the top of the stack.
	 * @return a reference of the top element.
	 * throw container_is_empty when empty() returns true.
	 */
	const T & top() const {
		if (data.empty())
			throw container_is_empty();
		return data.back();
	}
	/**
	 * TODO
	 * push new element to the stack.
	 */
	void push(const T &e) {
		data.push_back(e);
	}
	/**
	 * TODO
	 * delete the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	void pop() {
		if (data.empty())
			throw container_is_empty();
		data.pop_back();
	}
	/**
	 * return the number of the elements.
	 */
	size_t size() const {
		return data.size();
	}
	/**
	 * check if the container has at least an element.
	 * @return true if it is empty, false if it has at least an element.
	 */
	bool empty() const {
		return data.empty();
	}
};

}

#endif
