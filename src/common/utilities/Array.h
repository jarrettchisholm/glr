/*
 * Array.h
 *
 *  Created on: 2011-05-01
 *      Author: jarrett
 */

#ifndef ARRAY_H_
#define ARRAY_H_

#include "../compatibility/Types.h"
#include "../math/Math.h"

namespace utilities {

using namespace compatibility;

// todo: make sure it works!

template<class T> class Array {
public:
	/**
	 * Default constructor.
	 */
	Array() {
		arraySize_ = 1;
		numElements_ = 0;
		array_ = new T*[arraySize_];
	}

	/**
	 * Constructor, allowing a custom size for the array to start with.
	 *
	 * @param size The size the array should start with.  This value must be >=0 and <= MAX_SIZE.
	 */
	Array(uint32 size) {
		arraySize_ = size;
		if (size < 0 || size > MAX_SIZE)
			arraySize_ = 1;
		numElements_ = 0;
		array_ = new T*[arraySize_];
	}

	/**
	 * Destructor.  Note that each non-null element in the array has its destructor called.
	 */
	virtual ~Array() {
		if (array_ != 0) {
			for (int i = 0; i < arraySize_; i++) {
				if (array_[i] != 0)
					delete array_[i];
			}

			delete[] array_;
		}
	}

	/**
	 * Returns the element at 'index'.
	 *
	 *@param index
	 *
	 * @return Reference to the element at 'index'.
	 */
	T& get(uint32 index) {
		return *array_[index];
	}

	/**
	 * Returns the first element in the array.
	 *
	 * @return Reference to the first element in the array.
	 */
	T& first() {
		if (numElements_ == 0)
			return 0;

		for (int i = 0; i < arraySize_; i++) {
			if (array_[i] != 0)
				return *array_[i];
		}

		return 0;
	}

	/**
	 * Returns the last element in the array.
	 *
	 * @return Reference to the last element in the array.
	 */
	T& last() {
		if (numElements_ == 0)
			return 0;

		for (int i = arraySize_ - 1; i >= 0; i--) {
			if (array_[i] != 0)
				return *array_[i];
		}

		return 0;
	}

	/**
	 *	Returns the index of the first element in the array.
	 *
	 *	@return The index of the first element in the array, or -1 if not found.
	 */
	uint32 firstPos() {
		for (int i = 0; i < arraySize_; i++) {
			if (array_[i] != 0)
				return i;
		}

		return -1;
	}

	/**
	 *	Returns the index of the last element in the array.
	 *
	 *	@return The index of the last element in the array, or -1 if not found.
	 */
	uint32 lastPos() {
		for (int i = arraySize_ - 1; i >= 0; i--) {
			if (array_[i] != 0)
				return i;
		}

		return -1;
	}

	/**
	 * Adds element to the first available slot in the array.
	 * Note that the runtime for this method is O(1) best case and O(n) worst case.
	 *
	 * @param element
	 *
	 * @return The index where the element was added, or -1 on error.
	 */
	uint32 put(T& element) {
		// error check
		if (&element == 0)
			return -1;

		// if array is full
		if (arraySize_ == numElements_) {
			// error check
			if (arraySize_ == MAX_SIZE)
				return -1;
			// otherwise, increase array size
			increaseArraySize();
		}

		uint32 index = 0;
		// find the first empty slot and put the new element there
		for (int index = 0; index < arraySize_; index++) {
			if (array_[index] == 0) {
				array_[index] = &element;
				break;
			}
		}

		numElements_++;

		return index;
	}

	/**
	 * Adds element at the array index 'index'.
	 * Note that the runtime for this method is O(1) best case and O(n) worst case.  Also,
	 * if index is greater than the current capacity of the array, the array will grow to accommodate
	 * the index (unless index > MAX_SIZE).  This method will replace any element that is currently at
	 * 'index' without deletion, which may lead to memory leaks.  Be sure to check the location is
	 * empty before adding an element with this method.
	 *
	 * @param element The element to add
	 * @param index The index in the array that the reference to element is to be put
	 *
	 * @return The index where the element was added, or -1 on error.
	 */
	uint32 put(T& element, uint32 index) {
		// error check
		if (&element == 0 || index < 0 || index > MAX_SIZE)
			return -1;

		// if array is full
		if (arraySize_ == numElements_) {
			// error check
			if (arraySize_ == MAX_SIZE)
				return -1;
			// otherwise, increase array size
			increaseArraySize();
		}

		// check if array is not large enough for the given index
		if (index >= arraySize_) {
			// resize array and check for error
			if (resize(index) != 0)
				return -1;
		}

		array_[index] = &element;
		numElements_++;

		return 0;
	}

	/**
	 * Adds element to the back of the array (element is added after the last element in the array).
	 * Note that the runtime for this method is O(1) best case and O(n) worst case (array resizing).
	 *
	 * @param element
	 *
	 * @return The index where the element was added, or -1 on error.
	 */
	uint32 push_back(T& element) {
		// error check
		if (&element == 0)
			return -1;

		// if array is full OR last array slot is not empty
		if (arraySize_ == numElements_ || array_[arraySize_ - 1] != 0) {
			// error check
			if (arraySize_ == MAX_SIZE)
				return -1;
			// otherwise, increase array size
			increaseArraySize();
		}

		uint32 count = 0;
		uint32 index = 0;
		// find index of last element in the array
		for (; index < arraySize_ && count < numElements_; index++) {
			if (&array_[index] != 0)
				count++;
		}

		// must increment 'index' so that we point to the slot AFTER the last element in array
		array_[++index] = &element;

		numElements_++;

		return index;
	}

	/**
	 * Adds element to the front of the array.  Note that the runtime for this method is always O(n).
	 *
	 * @param element
	 *
	 * @return The index where the element was added, -1 on error.
	 */
	uint32 push_front(T& element) {
		// error check
		if (&element == 0)
			return -1;

		// if array is full OR last array slot is not empty
		if (arraySize_ == numElements_ || array_[arraySize_ - 1] != 0) {
			// error check
			if (arraySize_ == MAX_SIZE)
				return -1;
			// otherwise, increase array size
			increaseArraySize();
		}

		// if first slot in the array is not empty
		if (array_[0] != 0) {
			// shift all elements down the array by 1
			for (int i = arraySize_ - 1; i > 0; i--) {
				array_[i] = array_[i - 1];
			}
		}

		uint32 index = 0;
		// find the first element in the array
		for (int index = 0; index < arraySize_; index++) {
			if (&array_[index] != 0) {
				break;
			}
		}

		// must decrement 'index' so that we point to the slot BEFORE the first element in array
		array_[--index] = &element;

		numElements_++;

		return index;
	}

	/**
	 *	Remove the element at index and return it.
	 *
	 *	@param index
	 *
	 *	@return A pointer to the element.
	 */
	T* remove(uint32 index) {
		// error check
		if (index < 0 || index >= arraySize_)
			return 0; // null

		T* element = array_[index];
		// only decrement numElements_ if there WAS an element at position 'index'
		if (element != 0)
			numElements_--;
		array_[index] = 0;

		return element;
	}

	const static uint32 MAX_SIZE = 4096;
	/**
	 *	Resize the array to the newly specified size.  This function does not move any of the elements, it only increases
	 *	the capacity of the array.  Also, the array cannot be made larger than MAX_SIZE, and cannot be made smaller than
	 *	the number of elements already in the array (including the empty space between elements in the array).  If newSize is
	 *	already the same size as the array, this function makes no changes to the array.
	 *
	 *	@param newSize The new size the array will be after the function executes.
	 *
	 *	@return 0 on success, or -1 on error.
	 */
	uint32 resize(uint32 newSize) {
		// error check
		if (newSize > MAX_SIZE || newSize < numElements_)
			return -1;

		uint32 count = 0;
		// make sure new size doesn't cut out any elements
		for (int i = 0; i < newSize && count < numElements_; i++) {
			if (&array_[i] != 0)
				count++;
		}
		if (count < numElements_)
			return -1;

		// if the array is already the size we want
		if (newSize == arraySize_)
			return 0;

		// resize the array
		T** temp = array_;
		array_ = new T*[newSize];
		for (int i = 0; i < arraySize_; i++)
			array_[i] = temp[i];
		delete[] temp;

		arraySize_ = newSize;

		return 0;
	}

	/**
	 *	Shift all elements in the array by 'amount'.  Note that this function will increase the array size as necessary.
	 *	If the array would exceed its maximum size after the shift, it returns -1 and does not perform the shift.
	 *
	 *	@param amount The amount to shift elements to the right by.  Expects a value >= 0.
	 *
	 *	@return 0 on success, or -1 on error.
	 */
	uint32 shift(uint32 amount) {
		// error check
		if (amount < 0 || (arraySize_ + amount) > MAX_SIZE)
			return -1;

		// if there are no elements to shift
		if (numElements_ == 0)
			return 0;

		uint32 last = lastPos();
		uint32 space = arraySize_ - last - 1;
		// check if we need to increase array size to make space for shift
		if (space < amount) {
			// error check
			if (arraySize_ == MAX_SIZE)
				return -1;
			// otherwise, increase array size
			increaseArraySize();
		}

		// shift elements to the right by 'amount'
		for (int i = last; i >= 0; i--) {
			if (array_[i] != 0) {
				array_[i + amount] = array_[i];
				array_[i] = 0;
			}
		}

		return 0;
	}

	/**
	 *	Packs all elements together and shifts them to the front of the array.  This eliminates all empty space between
	 *	elements and ensures the first element starts at index '0'.  This function also ensures that the order of the elements
	 *	remains intact.
	 */
	void pack() {
		// if there are no elements in the array
		if (numElements_ == 0)
			return;

		// search through the array, shifting elements to the left to fill in empty spaces
		for (int i = 0; i < arraySize_; i++) {
			// found an empty space
			if (&array_[i] == 0) {
				// find an element
				for (int j = i + 1; j < arraySize_; j++) {
					if (&array_[j] != 0) {
						// swap
						array_[i] = array_[j];
						array_[j] = 0;
						// skip i ahead to j (we subtract 1 because the 'i++' will increment i ahead of this new empty space)
						i = j - 1;
						break;
					}
				} // end loop
			}
		} // end loop

	}

	/**
	 * Return the total number of elements in the array.
	 *
	 * @return The number of elements in the array.
	 */
	uint32 numElements() {
		return numElements_;
	}

	/**
	 * Return the total size of the array (i.e. the maximum number of
	 * elements that can be stored in the array at the current size).
	 *
	 * @return The total size of the array.
	 */
	uint32 size() {
		return arraySize_;
	}

private:
	uint32 arraySize_, numElements_;
	T** array_;

	/**
	 * Increase the size of our array, ensuring all of our current elements remain.
	 */
	void increaseArraySize() {
		T** temp = array_;
		array_ = new T*[math::Math::min(arraySize_ * 2, MAX_SIZE)];
		for (int i = 0; i < arraySize_; i++)
			array_[i] = temp[i];
		delete[] temp;

		arraySize_ = math::Math::min(arraySize_ * 2, MAX_SIZE);
	}

};

}

#endif /* ARRAY_H_ */
