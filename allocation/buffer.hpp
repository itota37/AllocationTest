// buffer.hpp

#if !defined(___ALLOCATION_TEST_BUFFER_HPP)
#define ___ALLOCATION_TEST_BUFFER_HPP

#include <mutex>

// バッファテンプレートです。
template<size_t BUFF_SIZE, size_t OFFSET>
struct Buffer
{
  using This = Buffer<BUFF_SIZE, OFFSET>;

private:

  char *m_pTop;
  size_t m_ElemSize;
  size_t m_ElemCapacity;
  char *(*m_PopFuncptr)(This *);
  size_t m_ElemCount;
  char *m_pElemStack;
  std::mutex m_FreeMutex;
  size_t m_FreeCount;
  char *m_pFreeStack;
  bool m_IsClosed;

  static char *_Pop(This *pBuff)
  {
    if (pBuff->m_ElemCount == 0)
    {
      std::lock_guard<std::mutex> _lock(pBuff->m_FreeMutex);
      if (pBuff->m_FreeCount == 0)
      {
        pBuff->m_IsClosed = true;
        return nullptr;
      }
      pBuff->m_ElemCount = pBuff->m_FreeCount;
      pBuff->m_pElemStack = pBuff->m_pFreeStack;
      pBuff->m_FreeCount = 0;
      pBuff->m_pFreeStack = nullptr;
    }
    auto pElem = pBuff->m_pElemStack;
    pBuff->m_pElemStack = *(char**)pElem;
    pBuff->m_ElemCount -= 1;
    return pElem;
  }

  static char *_Increment(This *pBuff)
  {
    auto pElem = pBuff->m_pTop[pBuff->m_ElemSize * pBuff->m_ElemCount];
    pBuff->m_ElemCount += 1;
    if (pBuff->m_ElemCount == pBuff->m_ElemCapacity)
    {
      pBuff->m_PopFuncptr = _Pop;
      pBuff->m_ElemCount = 0;
    }
    return pElem;
  }

public:

  Buffer(char *pTop, size_t elemSize)
    : m_pTop(pTop)
    , m_ElemSize(elemSize)
    , m_ElemCapacity((BUFF_SIZE - OFFSET) / m_ElemSize)
    , m_PopFuncptr(_Increment)
    , m_ElemCount(0)
    , m_pElemStack(nullptr)
    , m_FreeMutex()
    , m_FreeCount(0)
    , m_pFreeStack(nullptr)
    , m_IsClosed(false)
  {}

  char *Pop() 
  {
    return (*m_PopFuncptr)(this);
  }

  bool Push(char *pElem)
  {
    std::lock_guard<std::mutex> _lock(m_FreeMutex);
    (*(char**)pElem) = m_pFreeStack;
    m_pFreeStack = pElem;
    m_FreeCount += 1;
    return m_IsClosed ? m_FreeCount == m_ElemCapacity : false;
  }
};

#endif