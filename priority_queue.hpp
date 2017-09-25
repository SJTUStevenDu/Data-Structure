#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"
#include "vector.hpp"

namespace sjtu {
	template<typename T, class Compare = std::less<T>>
		class priority_queue {
		public:
			Compare cmp;
			struct Node {
				Node *father, *child, *nxt;
				T* data;
				size_t degree;
				Node () {
					father = NULL;
					child = NULL;
					nxt = NULL;
					degree = 0;
				}
			} *head;
			
			Node* dfs(Node* p) {
				if (p == NULL)
					return NULL;
				Node* v = new Node;
				v -> data = new T(*p -> data);
				v -> degree = p -> degree;
				Node* q = dfs(p -> child);
				v -> child = q;
				if (q != NULL)
					q -> father = v;
				Node* w = dfs(p -> nxt);
				v -> nxt = w;
				return v;
			}
			
			Node* Top() const {
				Node* ret = NULL;
				Node* now = head;
				while (now != NULL) {
					if (ret == NULL || (!cmp(*now -> data, *ret -> data)))
						ret = now;
					now = now -> nxt;
				}
			        return ret;
			}
			
			void Link(Node* p1, Node *p2) {
				p1 -> father = p2;
				p1 -> nxt = p2 -> child;
				p2 -> child = p1;
				p2 -> degree++;
			}
			
			Node* Merge(Node* p1, Node* p2) {
				Node* ret;
				if (p1 == NULL)
					return p2;
				if (p2 == NULL)
				        return p1;
				if (p1 -> degree < p2 -> degree) {
					ret = p1;
					p1 = p1 -> nxt;
				}
				else {
					ret = p2;
					p2 = p2 -> nxt;
				}
				Node* now = ret;
				while (p1 != NULL && p2 != NULL) {
					if (p1 -> degree < p2 -> degree) {
						now -> nxt = p1;
						p1 = p1 -> nxt;
					}
					else {
						now -> nxt = p2;
						p2 = p2 -> nxt;
					}
					now = now -> nxt;
				}
				while (p1 != NULL) {
					now -> nxt = p1;
					now = now -> nxt;
					p1 = p1 -> nxt;
				}
				while (p2 != NULL) {
					now -> nxt = p2;
					now = now -> nxt;
					p2 = p2 -> nxt;
				}
				return ret;
			}
			
			Node* Union(Node* p1, Node* p2) {
				Node *ret;
				ret = Merge(p1, p2);
				Node *tmp = ret;
				if (ret == NULL)
					return ret;
				Node *prv, *nxt, *now;
				now = ret;
				prv = NULL;
				nxt = now -> nxt;
				while (nxt != NULL) {
					if (now -> degree != now -> nxt -> degree || (nxt -> nxt != NULL && nxt -> nxt -> degree == now -> degree)) {
						prv = now;
						now = nxt;
					}
					else if (!cmp(*now -> data, *nxt -> data)) {
						now -> nxt = nxt -> nxt;
						Link(nxt, now);
					}
					else {
						if (prv == NULL)
							ret = nxt;
						else
							prv -> nxt = nxt;
						Link(now, nxt);
						now = nxt;
					}
					nxt = now -> nxt;
				}
				return ret;
			}
			
			priority_queue() {
				head = NULL;
			}
			priority_queue(const priority_queue &other) {
				head = dfs(other.head);
			}
			~priority_queue() {
				if (head == NULL)
					return;
				vector<Node*> tmp;
				tmp.push_back(head);
				for (int i = 0; i < tmp.size(); ++i) {
					Node *now = tmp[i];
					if (now -> nxt != NULL)
						tmp.push_back(now -> nxt);
					if (now -> child != NULL)
						tmp.push_back(now -> child);
					delete now;
				}
			}
			
			priority_queue &operator=(const priority_queue &other) {
				if (this == &other)
					return *this;
				if (head != NULL) {
					vector<Node*> tmp;
					tmp.push_back(head);
					for (int i = 0; i < tmp.size(); ++i) {
						Node *now = tmp[i];
						if (now -> nxt != NULL)
							tmp.push_back(now -> nxt);
						if (now -> child != NULL)
							tmp.push_back(now -> child);
						delete now;
					}
				}
				head = dfs(other.head);
				return *this;
			}
			
			const T & top() const {
				if (empty())
					throw container_is_empty();
				return *(Top() -> data);
			}
				
			void push(const T &e) {
				Node *cur = new Node;
				cur -> data = new T(e);
				head = Union(head, cur);
			}
			
			void pop() {
				if (empty())
					throw container_is_empty();
				Node *ret = Top();
				Node *prv = head;
				if (ret == prv) 
					head = head -> nxt;
				else {
					for (;prv -> nxt != ret && prv -> nxt != NULL; prv = prv -> nxt);
					prv -> nxt = ret -> nxt;
				}
				Node *tmp = head;
				Node *p = ret -> child;
				delete ret;
				Node* nxt = NULL;
				if (p != NULL) {
					nxt = p -> nxt;
					prv = NULL;
					for (;;p = nxt) {
						nxt = p -> nxt;
						p -> nxt = prv;
						prv = p;
						if (nxt == NULL)
							break;
					}
				}
				head = Union(head, p);
			}
			
			size_t size() const {
				Node* now = head;
				size_t ret = 0;
				while (now != NULL) {
					ret += (1 << now -> degree);
					now = now -> nxt;
				}
				return ret;
			}
			
			bool empty() const {
				return head == NULL;
			}
		};

}

#endif
