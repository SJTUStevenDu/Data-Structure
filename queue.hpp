#ifndef SJTU_QUEUE_HPP
#define SJTU_QUEUE_HPP

#include "list.hpp"
#include "exceptions.hpp"

namespace sjtu {

/**
 * a container whose behavior is similar to a queue.
 * It should be based on the list you had written before.
 * It should be based on the list you had written before.
 * It should be based on the list you had written before.
 * 重要的事情要说三遍：请调用自己写的list来实现
 * 重要的事情要说三遍：请调用自己写的list来实现
 * 重要的事情要说三遍：请调用自己写的list来实现
 */
template<typename T>
class queue {
	// hint: you can use you own name to store the data.
	list<T> data;
public:
	/**
	 * TODO constructors
	 */
	queue() {}
	queue(const queue<T> &other) {
		data = other.data;
	}
	/**
	 * TODO deconstructor
	 */
	~queue() {}
	/**
	 * TODO Assignment operator
	 */
	queue<T> &operator=(const queue<T> &other) {
		data = other.data;
	}
	
	/**
	 * get the first of the queue.
	 * @return a reference of the first element.
	 * throw container_is_empty if empty() returns true;
	 */
	const T & front() const {
		if (data.empty())
			throw container_is_empty();
		return data.front();
	}
	/**
	 * TODO
	 * push new element to the queue.
	 */
	void push(const T &e) {
		data.push_back(e);
	}
	/**
	 * TODO
	 * delete the first element.
	 * throw container_is_empty if empty() returns true;
	 */
	void pop() {
		if (data.empty())
			throw container_is_empty();
		data.pop_front();
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
