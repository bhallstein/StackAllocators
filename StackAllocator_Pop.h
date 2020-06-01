//
// StackAllocator_Pop.h
//
// - items can be popped of the end of the stack
// - in order to do this, the size of the object is stored alongside it
//

#ifndef __StackAllocator_Pop_h
#define __StackAllocator_Pop_h

#include <cstdint>

#ifdef __PSA_PSIZE_TYPE
  typedef __PSA_PSIZE_TYPE _psize;
#else
  typedef uint16_t _psize;
#endif

class StackAllocator_Pop {
public:
  StackAllocator_Pop(int _size) : size(_size), arr(new uint8_t[_size]) {  }
  ~StackAllocator_Pop() { delete [] arr; }

  template<class T>
  T* allocate() {
    return (T*) allocate(sizeof(T));
  }

  void* allocate(int bytes_for_object) {
    size_t bytes_required = bytes_for_object + sizeof(_psize);

    if (bytes_required > bytes_remaining()) {
      return NULL;
    }

    // The layout is { object, object_size_in_bytes }
    void *p_obj  = (void*) (arr + offset);
    _psize *p_size = (_psize*) (arr + offset + bytes_for_object);

    *p_size = bytes_for_object;

    offset += bytes_required;
    return p_obj;
  }

  void pop() {
    if (isEmpty()) return;

    _psize *p_size = (_psize*) (arr + offset - sizeof(_psize));
    offset -= (sizeof(_psize) + *p_size);
  }

  bool isEmpty() const {
    return offset == 0;
  }

private:
  StackAllocator_Pop(const StackAllocator_Pop &) { }
  StackAllocator_Pop operator= (const StackAllocator_Pop &) { return *this; }

  uint8_t *arr;
  int offset = 0;
  int size;

  int bytes_remaining() const {
    return size - offset;
  }
};

#endif


// LICENSE
// Copyright (c) 2017 Ben Hallstein
// -------------------------------------------
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
//   The above copyright notice and this permission notice shall be included in all
//   copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
