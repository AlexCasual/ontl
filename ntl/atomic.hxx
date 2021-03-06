/**\file*********************************************************************
 *                                                                     \brief
 *  Atomic operations
 *
 ****************************************************************************
 */
#ifndef NTL__ATOMIC
#define NTL__ATOMIC
#pragma once

#ifndef NTL__NT_BASEDEF
# include "nt/basedef.hxx"
#endif

#include "cpu.hxx"

namespace ntl {

#if defined(_MSC_VER)

namespace intrinsic
{
// MSDN prototypes these as having long parameters, however
// it says: The sign is ignored.
  extern "C" {
    uint64_t __cdecl __rdtsc();
    
    // interlocked intrinsics

    int32_t __cdecl _InterlockedAnd(volatile uint32_t *, uint32_t);
    char    __cdecl _InterlockedAnd8(volatile uint8_t *, uint8_t);
    int16_t __cdecl _InterlockedAnd16(volatile unsigned short *, unsigned short);
    int64_t __cdecl _InterlockedAnd64(volatile uint64_t *, uint64_t);

    int32_t __cdecl _InterlockedOr(volatile uint32_t *, uint32_t);
    char    __cdecl _InterlockedOr8(volatile uint8_t *, uint8_t);
    int16_t __cdecl _InterlockedOr16(volatile uint16_t *, uint16_t);
    int64_t __cdecl _InterlockedOr64(volatile uint64_t *, uint64_t);

    int32_t __cdecl _InterlockedXor(volatile uint32_t *, uint32_t);
    char    __cdecl _InterlockedXor8(volatile uint8_t *, uint8_t);
    int16_t __cdecl _InterlockedXor16(volatile uint16_t *, uint16_t);
    int64_t __cdecl _InterlockedXor64(volatile uint64_t *, uint64_t);

    int32_t __cdecl _InterlockedIncrement(volatile uint32_t *);
    int16_t __cdecl _InterlockedIncrement16(volatile uint16_t *);
    int64_t __cdecl _InterlockedIncrement64(volatile uint64_t *);

    int32_t __cdecl _InterlockedDecrement(volatile uint32_t *);
    int16_t __cdecl _InterlockedDecrement16(volatile uint16_t *);
    int64_t __cdecl _InterlockedDecrement64(volatile uint64_t *);

    int32_t __cdecl _InterlockedExchange(volatile uint32_t *, uint32_t);
    int64_t __cdecl _InterlockedExchange64(volatile uint64_t *, uint64_t);

    int32_t __cdecl _InterlockedExchangeAdd(volatile uint32_t *, uint32_t);
    int64_t __cdecl _InterlockedExchangeAdd64(volatile uint64_t *, uint64_t);

    int32_t __cdecl _InterlockedCompareExchange(volatile uint32_t *, uint32_t, uint32_t);
    int16_t __cdecl _InterlockedCompareExchange16(volatile uint16_t *, uint16_t, uint16_t);
    int64_t __cdecl _InterlockedCompareExchange64(volatile uint64_t *, uint64_t, uint64_t);

    void*   __cdecl _InterlockedCompareExchangePointer(void* volatile* Destination, void* Exchange, void* Comperand);
    uint8_t __cdecl _InterlockedCompareExchange128(volatile uint64_t *, uint64_t, uint64_t, uint64_t*);
    int64_t __cdecl _InterlockedCompare64Exchange128(uint64_t volatile* Destination, uint64_t ExchangeHigh, uint64_t ExchangeLow, uint64_t Comperand);
    void*   __cdecl _InterlockedExchangePointer(void* volatile* Target, void* Value);
  } // extern "C"

  __forceinline uint64_t rdtsc() { return __rdtsc(); }

#ifndef __ICL
#pragma intrinsic(__rdtsc)
#pragma intrinsic(_InterlockedAnd, _InterlockedAnd8, _InterlockedAnd16, _InterlockedOr, _InterlockedOr8, _InterlockedOr16, _InterlockedXor, _InterlockedXor8, _InterlockedXor16)
#pragma intrinsic(_InterlockedIncrement, _InterlockedIncrement16, _InterlockedDecrement, _InterlockedDecrement16, _InterlockedExchange, _InterlockedExchangeAdd)
#pragma intrinsic(_InterlockedCompareExchange, _InterlockedCompareExchange16, _InterlockedCompareExchange64)
#ifdef _M_X64
#pragma intrinsic(_InterlockedAnd64, _InterlockedOr64, _InterlockedXor64, _InterlockedIncrement64, _InterlockedDecrement64, _InterlockedExchange64, _InterlockedExchangeAdd64)
#pragma intrinsic(_InterlockedCompareExchangePointer, _InterlockedExchangePointer)
//#if _MSC_VER >= 1600
//#pragma intrinsic(_InterlockedCompareExchange128, _InterlockedCompare64Exchange128)
//#endif // vc10
#endif // x64
#endif // icl

  extern "C" {
  // compiler intrinsics
  void __cdecl _ReadBarrier(void);
  void __cdecl _WriteBarrier(void);
  void __cdecl _ReadWriteBarrier(void);

  // cache intrinsics

  /** Guarantees that every load instruction that precedes, in program order, the load fence instruction is globally visible 
    before any load instruction that follows the fence in program order. */
  void __cdecl _mm_lfence(void);

  /** Guarantees that every memory access that precedes, in program order, the memory fence instruction is globally visible 
    before any memory instruction that follows the fence in program order. */
  void __cdecl _mm_mfence(void);
  
  /** Guarantees that every preceding store is globally visible before any subsequent store. */
  void __cdecl _mm_sfence(void);
  
  /** The execution of the next instruction is delayed an implementation specific amount of time (spin-lock hint). */
  void __cdecl _mm_pause(void);

#ifdef _M_X64
  /** Guarantees that every preceding store is globally visible before any subsequent store. */
  void __cdecl __faststorefence();
#endif

  } // extern "C"

#ifndef __ICL
#pragma intrinsic(_ReadBarrier, _WriteBarrier, _ReadWriteBarrier, _mm_lfence, _mm_mfence, _mm_sfence, _mm_pause)

#ifdef _M_X64
# pragma intrinsic(__faststorefence)
#endif
#endif // icl

}//namespace intrinsic


namespace atomic {

#pragma warning(push)
#pragma warning(disable:4365)// signed/unsigned mismatch

  static inline
    uint16_t increment(volatile uint16_t & val)
  {
    return intrinsic::_InterlockedIncrement16(&val);
  }

  static inline
    uint32_t increment(volatile uint32_t & val)
  {
    return intrinsic::_InterlockedIncrement(&val);
  }

  static inline
    uint64_t increment(volatile uint64_t & val)
  {
    return intrinsic::_InterlockedIncrement64(reinterpret_cast<volatile uint64_t*>(&val));
  }
#ifndef _M_X64
  static inline
    size_t increment(volatile size_t & val)
  {
    return intrinsic::_InterlockedIncrement(reinterpret_cast<volatile uint32_t*>(&val));
  }
#endif
  static inline
    uint16_t
    decrement(volatile uint16_t & val)
  {
    return intrinsic::_InterlockedDecrement16(&val);
  }
  static inline
    uint32_t
    decrement(volatile uint32_t & val)
  {
    return intrinsic::_InterlockedDecrement(&val);
  }

  static inline
    uint64_t
    decrement(volatile uint64_t & val)
  {
    return intrinsic::_InterlockedDecrement64(&val);
  }
#ifndef _M_X64
  static inline
    size_t decrement(volatile size_t & val)
  {
    return intrinsic::_InterlockedDecrement(reinterpret_cast<volatile uint32_t*>(&val));
  }
#endif
  static inline
    bool bit_and(volatile bool & val, bool mask)
  {
    return intrinsic::_InterlockedAnd8(reinterpret_cast<volatile uint8_t*>(&val), mask) == 1;
  }

  static inline
    uint8_t bit_and(volatile uint8_t & val, uint8_t mask)
  {
    return intrinsic::_InterlockedAnd8(&val, mask);
  }

  static inline
    uint16_t bit_and(volatile uint16_t & val, uint16_t mask)
  {
    return intrinsic::_InterlockedAnd16(&val, mask);
  }

  static inline
    uint32_t bit_and(volatile uint32_t & val, uint32_t mask)
  {
    return intrinsic::_InterlockedAnd(&val, mask);
  }

  static inline
    uint64_t bit_and(volatile uint64_t & val, uint64_t mask)
  {
    return intrinsic::_InterlockedAnd64(&val, mask);
  }

  static inline
    bool bit_or(volatile bool & val, bool mask)
  {
    return intrinsic::_InterlockedOr8(reinterpret_cast<volatile uint8_t*>(&val), mask) == 1;
  }

  static inline
    uint8_t bit_or(volatile uint8_t & val, uint8_t mask)
  {
    return intrinsic::_InterlockedOr8(&val, mask);
  }

  static inline
    uint16_t bit_or(volatile uint16_t & val, uint16_t mask)
  {
    return intrinsic::_InterlockedOr16(&val, mask);
  }

  static inline
    uint32_t bit_or(volatile uint32_t & val, uint32_t mask)
  {
    return intrinsic::_InterlockedOr(&val, mask);
  }

  static inline
    uint64_t bit_or(volatile uint64_t & val, uint64_t mask)
  {
    return intrinsic::_InterlockedOr64(&val, mask);
  }

  static inline
    bool bit_xor(volatile bool & val, bool mask)
  {
    return intrinsic::_InterlockedXor8(reinterpret_cast<volatile uint8_t*>(&val), mask) == 1;
  }

  static inline
    uint8_t bit_xor(volatile uint8_t & val, uint8_t mask)
  {
    return intrinsic::_InterlockedXor8(&val, mask);
  }

  static inline
    uint16_t bit_xor(volatile uint16_t & val, uint16_t mask)
  {
    return intrinsic::_InterlockedXor16(&val, mask);
  }

  static inline
    uint32_t bit_xor(volatile uint32_t & val, uint32_t mask)
  {
    return intrinsic::_InterlockedXor(&val, mask);
  }

  static inline
    uint64_t bit_xor(volatile uint64_t & val, uint64_t mask)
  {
    return intrinsic::_InterlockedXor64(&val, mask);
  }


  static inline
    uint32_t
    exchange(volatile uint32_t & dest, uint32_t val)
  {
    return intrinsic::_InterlockedExchange(&dest, val);
  }

  static inline
    uint64_t
    exchange(volatile uint64_t & dest, uint64_t val)
  {
    return intrinsic::_InterlockedExchange64(&dest, val);
  }

  static inline
    uint32_t
    exchange_add(volatile uint32_t & dest, uint32_t val)
  {
    return intrinsic::_InterlockedExchangeAdd(&dest, val);
  }

  static inline
    uint64_t
    exchange_add(volatile uint64_t & dest, uint64_t val)
  {
    return intrinsic::_InterlockedExchangeAdd64(&dest, val);
  }

  static inline
    uint16_t
    compare_exchange(volatile uint16_t & dest, uint16_t exchange, uint16_t comparand)
  {
    return intrinsic::_InterlockedCompareExchange16(&dest, exchange, comparand);
  }

  static inline
    uint32_t
    compare_exchange(volatile uint32_t & dest, uint32_t exchange, uint32_t comparand)
  {
    return intrinsic::_InterlockedCompareExchange(&dest, exchange, comparand);
  }

  static inline
    uint64_t
    compare_exchange(volatile uint64_t & dest, uint64_t exchange, uint64_t comparand)
  {
    return intrinsic::_InterlockedCompareExchange64(&dest, exchange, comparand);
  }

  static inline
    uint8_t
    compare_exchange(volatile uint64_t & dest, uint64_t exchange_high, uint64_t exchange_low, uint64_t* comparand)
  {
    return intrinsic::_InterlockedCompareExchange128(&dest, exchange_high, exchange_low, comparand);
  }

  static inline
    uint64_t
    compare_exchange(volatile uint64_t & dest, uint64_t exchange_high, uint64_t exchange_low, uint64_t comparand)
  {
    return intrinsic::_InterlockedCompare64Exchange128(&dest, exchange_high, exchange_low, comparand);
  }

  //////////////////////////////////////////////////////////////////////////

  static inline
    int16_t increment(volatile int16_t & val)
  {
    return intrinsic::_InterlockedIncrement16(reinterpret_cast<volatile uint16_t*>(&val));
  }

  static inline
    int32_t increment(volatile int32_t & val)
  {
    return intrinsic::_InterlockedIncrement(reinterpret_cast<volatile uint32_t*>(&val));
  }

  static inline
    int64_t increment(volatile int64_t & val)
  {
    return intrinsic::_InterlockedIncrement64(reinterpret_cast<volatile uint64_t*>(&val));
  }

  static inline
    int16_t
    decrement(volatile int16_t & val)
  {
    return intrinsic::_InterlockedDecrement16(reinterpret_cast<volatile uint16_t*>(&val));
  }
  static inline
    int32_t
    decrement(volatile int32_t & val)
  {
    return intrinsic::_InterlockedDecrement(reinterpret_cast<volatile uint32_t*>(&val));
  }

  static inline
    int64_t
    decrement(volatile int64_t & val)
  {
    return intrinsic::_InterlockedDecrement64(reinterpret_cast<volatile uint64_t*>(&val));
  }

  static inline
    int8_t bit_and(volatile int8_t & val, int8_t mask)
  {
    return intrinsic::_InterlockedAnd8(reinterpret_cast<volatile uint8_t*>(&val), mask);
  }

  static inline
    int16_t bit_and(volatile int16_t & val, int16_t mask)
  {
    return intrinsic::_InterlockedAnd16(reinterpret_cast<volatile uint16_t*>(&val), mask);
  }

  static inline
    int32_t bit_and(volatile int32_t & val, int32_t mask)
  {
    return intrinsic::_InterlockedAnd(reinterpret_cast<volatile uint32_t*>(&val), mask);
  }

  static inline
    int64_t bit_and(volatile int64_t & val, int64_t mask)
  {
    return intrinsic::_InterlockedAnd64(reinterpret_cast<volatile uint64_t*>(&val), mask);
  }

  static inline
    int8_t bit_or(volatile int8_t & val, int8_t mask)
  {
    return intrinsic::_InterlockedOr8(reinterpret_cast<volatile uint8_t*>(&val), mask);
  }

  static inline
    int16_t bit_or(volatile int16_t & val, int16_t mask)
  {
    return intrinsic::_InterlockedOr16(reinterpret_cast<volatile uint16_t*>(&val), mask);
  }

  static inline
    int32_t bit_or(volatile int32_t & val, int32_t mask)
  {
    return intrinsic::_InterlockedOr(reinterpret_cast<volatile uint32_t*>(&val), mask);
  }

  static inline
    int64_t bit_or(volatile int64_t & val, int64_t mask)
  {
    return intrinsic::_InterlockedOr64(reinterpret_cast<volatile uint64_t*>(&val), mask);
  }

  static inline
    int8_t bit_xor(volatile int8_t & val, int8_t mask)
  {
    return intrinsic::_InterlockedXor8(reinterpret_cast<volatile uint8_t*>(&val), mask);
  }

  static inline
    int16_t bit_xor(volatile int16_t & val, int16_t mask)
  {
    return intrinsic::_InterlockedXor16(reinterpret_cast<volatile uint16_t*>(&val), mask);
  }

  static inline
    int32_t bit_xor(volatile int32_t & val, int32_t mask)
  {
    return intrinsic::_InterlockedXor(reinterpret_cast<volatile uint32_t*>(&val), mask);
  }

  static inline
    int64_t bit_xor(volatile int64_t & val, int64_t mask)
  {
    return intrinsic::_InterlockedXor64(reinterpret_cast<volatile uint64_t*>(&val), mask);
  }


  static inline
    int32_t
    exchange(volatile int32_t & dest, int32_t val)
  {
    return intrinsic::_InterlockedExchange(reinterpret_cast<volatile uint32_t*>(&dest), val);
  }

  static inline
    int64_t
    exchange(volatile int64_t & dest, int64_t val)
  {
    return intrinsic::_InterlockedExchange64(reinterpret_cast<volatile uint64_t*>(&dest), val);
  }

  static inline
    int32_t
    exchange_add(volatile int32_t & dest, int32_t val)
  {
    return intrinsic::_InterlockedExchangeAdd(reinterpret_cast<volatile uint32_t*>(&dest), val);
  }

  static inline
    int64_t
    exchange_add(volatile int64_t & dest, int64_t val)
  {
    return intrinsic::_InterlockedExchangeAdd64(reinterpret_cast<volatile uint64_t*>(&dest), val);
  }

  static inline
    int16_t
    compare_exchange(volatile int16_t & dest, int16_t exchange, int16_t comparand)
  {
    return intrinsic::_InterlockedCompareExchange16(reinterpret_cast<volatile uint16_t*>(&dest), exchange, comparand);
  }

  static inline
    int32_t
    compare_exchange(volatile int32_t & dest, int32_t exchange, int32_t comparand)
  {
    return intrinsic::_InterlockedCompareExchange(reinterpret_cast<volatile uint32_t*>(&dest), exchange, comparand);
  }

  static inline
    int64_t
    compare_exchange(volatile int64_t & dest, int64_t exchange, int64_t comparand)
  {
    return intrinsic::_InterlockedCompareExchange64(reinterpret_cast<volatile uint64_t*>(&dest), exchange, comparand);
  }

  static inline
    int8_t
    compare_exchange(volatile int64_t & dest, int64_t exchange_high, int64_t exchange_low, int64_t* comparand)
  {
    return intrinsic::_InterlockedCompareExchange128(reinterpret_cast<volatile uint64_t*>(&dest), exchange_high, exchange_low, reinterpret_cast<uint64_t*>(comparand));
  }

  static inline
    int64_t
    compare_exchange(volatile int64_t & dest, int64_t exchange_high, int64_t exchange_low, int64_t comparand)
  {
    return intrinsic::_InterlockedCompare64Exchange128(reinterpret_cast<volatile uint64_t*>(&dest), exchange_high, exchange_low, comparand);
  }


  //////////////////////////////////////////////////////////////////////////
  struct generic_op
  {
  private:
    template<size_t v> struct int2type { enum { value = v }; };
    template<typename T> 
    struct tmap
    {
    private:
      template<size_t> struct map;
      template<> struct map<1> { typedef uint8_t type; };
      template<> struct map<2> { typedef uint16_t type; };
      template<> struct map<4> { typedef uint32_t type; };
      template<> struct map<8> { typedef uint64_t type; };
    public:
      typedef typename map<sizeof(T)>::type type;
    };
  public:
    template<typename T>
    static inline T increment(volatile T& val)
    {
      typedef typename tmap<T>::type U;
      return reinterpret_cast<T>(atomic::increment(reinterpret_cast<volatile U&>(val)));
    }
    template<typename T>
    static inline T decrement(volatile T& val)
    {
      typedef typename tmap<T>::type U;
      return reinterpret_cast<T>(atomic::decrement(reinterpret_cast<volatile U&>(val)));
    }
    template<typename T>
    static inline T bit_or(volatile T& val, T mask)
    {
      typedef typename tmap<T>::type U;
      return reinterpret_cast<T>(atomic::bit_or(reinterpret_cast<volatile U&>(val), reinterpret_cast<U>(mask)));
    }
    template<typename T>
    static inline T bit_xor(volatile T& val, T mask)
    {
      typedef typename tmap<T>::type U;
      return reinterpret_cast<T>(atomic::bit_xor(reinterpret_cast<volatile U&>(val), reinterpret_cast<U>(mask)));
    }
    template<typename T>
    static inline T bit_and(volatile T& val, T mask)
    {
      typedef typename tmap<T>::type U;
      return reinterpret_cast<T>(atomic::bit_and(reinterpret_cast<volatile U&>(val), reinterpret_cast<U>(mask)));
    }
    template<typename T>
    static inline T exchange(volatile T& dest, T val)
    {
      typedef typename tmap<T>::type U;
      return reinterpret_cast<T>(atomic::exchange(reinterpret_cast<volatile U&>(dest), reinterpret_cast<U>(val)));
    }
    template<typename T>
    static inline T exchange_add(volatile T& dest, T val)
    {
      typedef typename tmap<T>::type U;
      return reinterpret_cast<T>(atomic::exchange_add(reinterpret_cast<volatile U&>(dest), reinterpret_cast<U>(val)));
    }
    template<typename T>
    static inline T compare_exchange(volatile T& dest, T exchange, T comparand)
    {
      typedef typename tmap<T>::type U;
      return reinterpret_cast<T>(atomic::compare_exchange(reinterpret_cast<volatile U&>(dest), reinterpret_cast<U>(exchange), reinterpret_cast<U>(comparand)));
    }
  };

#pragma warning(pop)
}//namespace atomic

#elif defined(__GNUC__)

namespace atomic {

  template<typename T>
  static inline
    T increment(volatile T & val)
  {
    return __sync_fetch_and_add(&val, 1);
  }

  template<typename T>
  static inline
    T decrement(volatile T & val)
  {
    return __sync_fetch_and_sub(&val);
  }

  template<typename T>
  static inline
    T bit_and(volatile T & val, T mask)
  {
    return __sync_fetch_and_and(&val, mask);
  }
  static inline
    bool bit_and(volatile bool & val, bool mask)
  {
    return bit_and<uint8_t>(reinterpret_cast<volatile uint8_t&>(val), mask) == 1;
  }

  template<typename T>
  static inline
    T bit_or(volatile T & val, T mask)
  {
    return __sync_fetch_and_or(&val, mask);
  }
  static inline
    bool bit_or(volatile bool & val, bool mask)
  {
    return bit_or<uint8_t>(reinterpret_cast<volatile uint8_t&>(val), mask) == 1;
  }

  template<typename T>
  static inline
    T bit_xor(volatile T & val, T mask)
  {
    return __sync_fetch_and_xor(&val, mask);
  }
  static inline
    bool bit_xor(volatile bool & val, bool mask)
  {
    return bit_xor<uint8_t>(reinterpret_cast<volatile uint8_t&>(val), mask) == 1;
  }

  template<typename T>
  static inline
    T exchange(volatile T & dest, T val)
  {
    return __sync_lock_test_and_set(&dest, val);
  }

  template<typename T>
  static inline
    T exchange_add(volatile T & dest, T val)
  {
    return __sync_fetch_and_add(&dest, val);
  }

  template<typename T>
  static inline
    T compare_exchange(volatile T & dest, T exchange, T comparand)
  {
    return __sync_val_compare_and_swap(&dest, comparand, exchange);
  }

}//namespace atomic


#else
 // unimplemented stubs
namespace atomic {

static inline
unsigned long
  increment(volatile unsigned long & val)
{
  return 0;
}

static inline
unsigned long
  decrement(volatile unsigned long & val)
{
  return 0;
}

static inline
unsigned long
  exchange(volatile unsigned long & dest, unsigned long val)
{
  return 0;
}

static inline
unsigned long
  exchange_add(volatile unsigned long & dest, unsigned long val)
{
  return 0;
}

static inline
unsigned long
  compare_exchange(volatile unsigned long & dest, unsigned long exchange,
                   unsigned long comparand)
{
  return 0;
}

}//namespace atomic


#endif


/// @warning not to be allocated on memory with the PAGE_NOCACHE attribute.
struct atomic_t
{
	typedef unsigned long type;

public:
	operator const volatile type & () const { return value; }

	type compare(type val) const
	{ return atomic::compare_exchange(value, value, val); }

	type operator++() { return atomic::increment(value); }
	type operator--() { return atomic::decrement(value); }
	void operator+=(type val) { exchange_add(val); }
	void operator-=(type val) { exchange_add(0 - val); }
	type exchange(type val)
	{ return atomic::exchange(value, val); }
	type exchange_add(type val)
	{ return atomic::exchange_add(value, val); }
	type exchange_if_equal(type val, type comparand)
	{ return atomic::compare_exchange(value, val, comparand); }

public:
	explicit atomic_t(type val = 0)
		:value(val)
	{}

private:
	mutable volatile type value;
};//struct atomic_t

namespace atomic
{
  /**
   *	@brief    Atomic backoff helper
   *  @details  Class that implements exponential backoff.
   *  @author   Intel Corp.
   *  @note     Idea taken from T.B.B.
   **/
  class backoff
  {
    // values in \c pause units
    static const unsigned maximum = 16;
    unsigned current;
  public:
    constexpr backoff()
      :current(1)
    {}

    /** Pause or yield execution */
    void pause()
    {
      if(current < maximum){
        cpu::pause(current);
        current *= 2;
      }else{
        cpu::yield();
      }
    }

    /** Resets the counter */
    void reset()
    {
      current = 1;
    }
  };


	/** Integral atomic value */
	typedef ntl::atomic_t value_t;


	/** Boolean atomic flag */
	struct flag_t
	{
		operator const bool() const { return value != false; }

		void set()		{ exchange(true); }
		void clear()	{ exchange(false); }

		bool test() const
		{ return atomic::exchange_add(value, 0) != false; }

		bool test(bool val) const
		{ return atomic::exchange_add(value, 0) == static_cast<unsigned long>(val); }

		bool test_and_set()
		{ return atomic::exchange(value, true) != false; }

		bool exchange(bool val)
		{ return atomic::exchange(value, val) != false; }

		bool exchange_if_equal(bool val, bool comparand)
		{ return atomic::compare_exchange(value, val, comparand) != false; }

		explicit flag_t(bool val = false)
			:value(val)
		{}

		flag_t& operator= (bool val)
		{ return exchange(val), *this; }

	private: mutable volatile unsigned long value;
	};
	
} // atomic

template <class Lock, class T = void>
class atomic_exec;

template <class Lock, class T>
class atomic_exec
{
    atomic_exec();
    atomic_exec(const atomic_exec &);
    atomic_exec & operator=(const atomic_exec &);

  public:

    explicit
    atomic_exec(Lock & lock)
    : lock(lock), r(lock.acquire())
    { }

    explicit
    atomic_exec(Lock * lock)
    : lock(*lock), r(lock->acquire())
    { }

    ~atomic_exec()
    {
      lock.release(r);
    }

  private:

    Lock & lock;
    T      r;
};


template <class Lock>
class atomic_exec<Lock, void>
{
    atomic_exec();
    atomic_exec(const atomic_exec &);
    atomic_exec & operator=(const atomic_exec &);

  public:

    explicit
    atomic_exec(Lock & lock)
    : lock(lock)
    {
      lock.acquire();
    }

    template<class AnyLock>
    explicit
    atomic_exec(AnyLock & lock)
    : lock(*static_cast<Lock*>(&lock))
    {
      this->lock.acquire();
    }

    explicit
    atomic_exec(Lock * lock)
    : lock(*lock)
    {
      lock.acquire();
    }

    template<class AnyLock>
    explicit
    atomic_exec(AnyLock * lock)
    : lock(*static_cast<Lock*>(lock))
    {
      this->lock.acquire();
    }

    ~atomic_exec()
    {
      lock.release();
    }

  private:

    Lock & lock;

};

#if !defined(_M_X64) && !defined(__ICL)
namespace intrinsic
{
  extern "C"
  {
    inline int64_t __cdecl _InterlockedAnd64(volatile uint64_t * dest, uint64_t mask)
    {
      const int64_t old = static_cast<int64_t>(*dest);
      while(_InterlockedExchange64(dest, old&mask) == old);
      return *dest;
    }
    inline int64_t __cdecl _InterlockedOr64(volatile uint64_t * dest, uint64_t mask)
    {
      const int64_t old = static_cast<int64_t>(*dest);
      while(_InterlockedExchange64(dest, old|mask) == old);
      return *dest;
    }
    inline int64_t __cdecl _InterlockedXor64(volatile uint64_t * dest, uint64_t mask)
    {
      const int64_t old = static_cast<int64_t>(*dest);
      while(_InterlockedExchange64(dest, old^mask) == old);
      return *dest;
    }

    inline int64_t __cdecl _InterlockedExchange64(volatile uint64_t * dest, uint64_t val)
    {
      __asm 
      {
        mov esi,[dest];
        mov ecx,dword ptr [val+4];
        mov ebx,dword ptr [val];
cas_loop:
        lock cmpxchg8b [esi];
        jnz cas_loop
      }
      return *dest;
    }

    inline int64_t __cdecl _InterlockedExchangeAdd64(volatile uint64_t * dest, uint64_t val)
    {
      __asm 
      {
        mov esi,[dest];
        mov eax,[esi];
        mov edx,[esi+4];
cas_loop:
        mov ebx,dword ptr [val];
        mov ecx,dword ptr [val+4];
        add ebx,eax;
        adc ecx,edx;
        lock cmpxchg8b [esi];
        jne cas_loop;
      }
      return *dest;
    }
  }
}
#endif // x86

}//namespace ntl

#endif//#ifndef NTL__ATOMIC
