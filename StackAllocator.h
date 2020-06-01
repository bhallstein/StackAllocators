//
// StackAllocator.h
//
// - a bog-standard stack allocator
//

#ifndef __StackAllocator_h
#define __StackAllocator_h

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

class StackAllocator {
public:
  StackAllocator(int _size) : size(_size), arr(new uint8_t[_size]) {  }
  ~StackAllocator() { delete [] arr; }

  template<class T>
  T* allocate() {
    return (T*) allocate(sizeof(T));
  }

  void* allocate(int bytes_for_object) {
    if (bytes_for_object > bytes_remaining())
      return NULL;

    void *p_obj  = (void*) (arr + offset);
    offset += bytes_for_object;

    return p_obj;
  }

  inline bool isEmpty() const {
    return offset == 0;
  }

private:
  StackAllocator(const StackAllocator &) { }
  StackAllocator operator= (const StackAllocator &) { return StackAllocator(32); }

  uint8_t *arr;
  int offset = 0;
  int size;

  inline int bytes_remaining() {
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
