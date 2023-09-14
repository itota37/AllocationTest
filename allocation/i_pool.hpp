// i_pool.hpp

// プールの位置を表現する構造体です。
struct PoolPosition
{
  size_t pageIndex;
  size_t poolIndex;
};


struct IPool
{
  virtual void *Pop() = 0;
  virtual void Push(void *) = 0;
  virtual PoolPosition GetPosition() const = 0;
};