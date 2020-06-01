//
// StackAllocator_Expand.h
//
// - StackAllocator has a fixed size
// - StackAllocator_Expand manages a vector of StackAllocators, adding
//   them as necessary, thus providing somewhat flexible storage
// - Each added allocator is twice the size of the previous one
//

#ifndef __StackAllocator_Expand_h
#define __StackAllocator_Expand_h

#include <vector>
#include "StackAllocator.h"

class StackAllocator_Expand {
public:
  StackAllocator_Expand(int initial_size = -1)
  {
    if (initial_size != -1) {
      size = initial_size;
    }
    allocators.push_back(new StackAllocator(size));
  }
  ~StackAllocator_Expand()
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

private:
  StackAllocator_Expand(const StackAllocator_Expand &) { }
  StackAllocator_Expand operator= (const StackAllocator_Expand &) { return StackAllocator_Expand(); }

  int size = 128;
  std::vector<StackAllocator*> allocators;

  void expand() {
    allocators.push_back(new StackAllocator(size *= 2));
  }
  void contractIfLastEmpty() {
    if (allocators.size() < 2) return;
    StackAllocator *sa = currentAllocator();
    if (sa->isEmpty()) {
      delete sa;
      allocators.pop_back();
      size *= 0.5;
    }
  }
  StackAllocator* currentAllocator() const {
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
