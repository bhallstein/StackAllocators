//
// StackAllocator_PopAndExpand.h
//
// - StackAllocator_Pop has a fixed size
// - StackAllocator_PopAndExpand manages a vector of PSAs, adding & removing them as
//   necessary, thus providing flexible storage
//

#ifndef __StackAllocator_PopAndExpand_h
#define __StackAllocator_PopAndExpand_h

#include <vector>
#include "StackAllocator_Pop.h"

class StackAllocator_PopAndExpand {
public:
  StackAllocator_PopAndExpand(int initial_size = -1)
  {
    if (initial_size != -1) {
      size = initial_size;
    }
    allocators.push_back(new StackAllocator_Pop(size));
  }
  ~StackAllocator_PopAndExpand()
  {
    for (auto sa : allocators) { delete sa; }
  }

  template<class T>
  T* allocate() {
    return (T*) allocate(sizeof(T));
  }
  void* allocate(int bytes_for_object) {
    void *x = currentAllocator()->allocate(bytes_for_object);
    if (!x) {
      expand();
      x = currentAllocator()->allocate(bytes_for_object);
    }
    return x;
  }

  void pop() {
    currentAllocator()->pop();
    contractIfLastEmpty();
  }

private:
  StackAllocator_PopAndExpand(const StackAllocator_PopAndExpand &) { }
  StackAllocator_PopAndExpand operator= (const StackAllocator_PopAndExpand &) { return *this; }

  int size = 128;
  std::vector<StackAllocator_Pop*> allocators;

  void expand() {
    allocators.push_back(new StackAllocator_Pop(size *= 2));
  }
  void contractIfLastEmpty() {
    if (allocators.size() < 2) return;
    StackAllocator_Pop *sa = currentAllocator();
    if (sa->isEmpty()) {
      delete sa;
      allocators.pop_back();
      size *= 0.5;
    }
  }
  StackAllocator_Pop* currentAllocator() const {
    return allocators.back();
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
