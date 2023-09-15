// i_pool.hpp

#if !defined(___ALLOCATION_TEST_I_POOL_HPP)
#define ___ALLOCATION_TEST_I_POOL_HPP

// メモリプールインタフェースです。
struct IPool
{
  virtual void *Pop() = 0;
  virtual void Push(void *) = 0;
};

#endif