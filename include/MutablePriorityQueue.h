//
// Created by Henrique Silva on 05/04/2023.
//

#ifndef DA_PROJ1_MUTABLEPRIORITYQUEUE_H
#define DA_PROJ1_MUTABLEPRIORITYQUEUE_H

#include <vector>

/**
 * class T must have: (i) accessible field int queueIndex; (ii) operator< defined.
 */

/**
 * @brief A simple implementation of a priority queue.
 *
 * @note This priority queue was implemented by João Pascoal Faria.
 * @tparam T A class for the template.
 */
template <class T>
class MutablePriorityQueue {
    /**
     * @brief A vector that stores the element of the priority queue.
     */
    std::vector<T *> H;

    /**
     * @brief Used to insert an element in the heap.
     *
     * @param i The position of the element that will be inserted.
     */
    void heapifyUp(unsigned i);

    /**
     * @brief Used to remove an element from the heap.
     *
     * @param i The position of the element that will be removed.
     */
    void heapifyDown(unsigned i);

    /**
     * @brief Sets the element in the heap.
     *
     * @param i The postion of the element.
     * @param x The element.
     */
    inline void set(unsigned i, T * x);
public:
    /**
     * @brief Creates an empty heap.
     */
    MutablePriorityQueue();

    /**
     * @brief Insert an element in the heap.
     *
     * @param x The element that will be inserted.
     */
    void insert(T * x);

    /**
     * @brief Extracts the minimal element of the heap.
     *
     * @return The minimal element.
     */
    T * extractMin();

    /**
     * @brief Updates the priority of an element in the heap.
     *
     * @param x The element that will be updated.
     */
    void decreaseKey(T * x);

    /**
     * @brief Returns if the heap is empty.
     *
     * @return True if the heap is empty.
     * @return False otherwise.
     */
    bool empty();
};

// Index calculations
#define parent(i) ((i) / 2)
#define leftChild(i) ((i) * 2)

template <class T>
MutablePriorityQueue<T>::MutablePriorityQueue() {
    H.push_back(nullptr);
    // indices will be used starting in 1
    // to facilitate parent/child calculations
}

template <class T>
bool MutablePriorityQueue<T>::empty() {
    return H.size() == 1;
}

template <class T>
T* MutablePriorityQueue<T>::extractMin() {
    auto x = H[1];
    H[1] = H.back();
    H.pop_back();
    if(H.size() > 1) heapifyDown(1);
    x->queueIndex = 0;
    return x;
}

template <class T>
void MutablePriorityQueue<T>::insert(T *x) {
    H.push_back(x);
    heapifyUp(H.size()-1);
}

template <class T>
void MutablePriorityQueue<T>::decreaseKey(T *x) {
    heapifyUp(x->queueIndex);
}

template <class T>
void MutablePriorityQueue<T>::heapifyUp(unsigned i) {
    auto x = H[i];
    while (i > 1 && *x < *H[parent(i)]) {
        set(i, H[parent(i)]);
        i = parent(i);
    }
    set(i, x);
}

template <class T>
void MutablePriorityQueue<T>::heapifyDown(unsigned i) {
    auto x = H[i];
    while (true) {
        unsigned k = leftChild(i);
        if (k >= H.size())
            break;
        if (k+1 < H.size() && *H[k+1] < *H[k])
            ++k; // right child of i
        if ( ! (*H[k] < *x) )
            break;
        set(i, H[k]);
        i = k;
    }
    set(i, x);
}

template <class T>
void MutablePriorityQueue<T>::set(unsigned i, T * x) {
    H[i] = x;
    x->queueIndex = i;
}

#endif //DA_PROJ1_MUTABLEPRIORITYQUEUE_H
