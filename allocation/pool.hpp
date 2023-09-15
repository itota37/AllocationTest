// pool.hpp

#if !defined(___ALLOCATION_TEST_POOL_HPP)
#define ___ALLOCATION_TEST_POOL_HPP

#include "i_pool.hpp"
#include "buffer.hpp"

// メモリプールです。
template<size_t POOL_SIZE, size_t OFFSET>
struct Pool: public IPool
{
private:

  Buffer<POOL_SIZE, OFFSET> m_Buff;

public:

  Pool(char *pTop, size_t blockSize)
    : m_Buff(pTop, blockSize)
  void *Pop() override
  {
    return (void*)m_Buff.Pop();
  }
  void Push(void *pBlock) override
  {
    if (m_Buff.Push((char*)pBlock))
    {
      // ページにPush
    }
  }
};

#endif