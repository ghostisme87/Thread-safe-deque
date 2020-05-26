#if !defined(_TS_DEQUE_H_)
#define _TS_DEQUE_H_

#include <deque>
#include <thread>
#include <condition_variable>


namespace Utils {
	namespace Containers {
		template<typename T>
		class Deque {

			/*
			 *Method:
			 *	Push(T t) - pushes an item in front
			 *					of the internal deque.
			 *
			 *		@parameters:
			 *			T - an element of unknown type.
			 */
			void Push(T t) {
				std::lock_guard<std::mutex> lk(m_mutex);
				m_deque.front(t);
			}

			/*
			 *Method:
			 *	PushBack(T t) - pushes an item to the back
			 *					of the internal deque.
			 *
			 *		@parameters:
			 *			T - an element of unknown type.
			 */
			void PushBack(T t) {
				std::lock_guard<std::mutex> lk(m_mutex);
				m_deque.back(t);
			}

			/*
			 *Method:
			 *	TryPop() - returns element from the internal deque
			 *			   and pop it.
			 *
			 *		@returns:
			 *			T& - reference to the item in the internal deque
			 */
			T& TryPop() {
				std::unique_lock<std::mutex> lk(m_mutex);
				m_cv.wait(lk, [this]() { return !m_deque.empty(); });

				T& t = m_deque.front();
				m_deque.pop_front();

				return t;
			}

			/*
			 *Method:
			 *	Empty()
			 *		Returns whether the internal deque is empty.
			 *
			 *		@returns:
			 *			bool
			 */
			bool Empty() {
				std::lock_guard<std::mutex> lk(m_mutex);
				return m_deque.empty();
			}


		private:

			/*
			 *Internal deque
			 */
			std::deque<T> m_deque;

			/*
			 *Sync mutex
			 */
			std::mutex m_mutex;

			/*
			 *Contition variable
			 */
			std::condition_variable m_cv;
		};
	}//namespace Conteiners
}//namespace Utils

#endif //_TS_DEQUE_H_