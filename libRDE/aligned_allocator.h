/* Copyright -  All Rights Reserved - Terry Lyons 2008 - derived from the Microsoft Version of Aloc */
#pragma once
#ifndef aligned_allocator_h__
#define aligned_allocator_h__

// usage  typedef std::vector<X, aligned_allocator < X , required_alignment> > VecAligned;

#include <malloc.h>

#pragma pack(push,8)
#pragma warning(push,3)

#pragma warning(disable: 4100)

#ifndef _FARQ	/* specify standard memory model */
#define _FARQ
#define _PDFT	ptrdiff_t
#define _SIZT	size_t
#endif

#define _CPOINTER_X(T, A)		\
typename A::template rebind<T>::other::const_pointer
#define _CREFERENCE_X(T, A)	\
typename A::template rebind<T>::other::const_reference
#define _POINTER_X(T, A)	\
typename A::template rebind<T>::other::pointer
#define _REFERENCE_X(T, A)	\
typename A::template rebind<T>::other::reference
namespace TJL_alloc
{
// TEMPLATE FUNCTION _Allocate


template<class _Ty>
inline
	_Ty _FARQ* _Allocate(_SIZT _Count, _Ty _FARQ*, _SIZT alignment)
{	// allocate storage for _Count elements of type _Ty
	_Ty* t = (_Ty*)_aligned_malloc(_Count * sizeof(_Ty), alignment); //void *_aligned_malloc(size_t size, size_t alignment)
#ifdef SHOWALLOC
		std::cout
			<< "  used my_allocator to allocate at address "
			<< t << " (+)" << std::endl;
#endif	
	return t;
	//return ((_Ty _FARQ *)operator new(_Count * sizeof (_Ty)));
}

// TEMPLATE FUNCTION _Construct
template<class _T1,
		 class _T2>
inline
	void _Construct(_T1 _FARQ* _Ptr, const _T2& _Val)
{	// construct object at _Ptr with value _Val
	new ((void _FARQ*)_Ptr) _T1(_Val);
}

// TEMPLATE FUNCTION _Destroy
template<class _Ty>
inline
	void _Destroy(_Ty _FARQ* _Ptr)
{	
		(_Ptr)->~_Ty();
		// destroy object at _Ptr
	//_DESTRUCTOR(_Ty, _Ptr); 	
}

template<>
inline
	void _Destroy(char _FARQ*)
{	// destroy a char (do nothing)
}

template<>
inline
	void _Destroy(wchar_t _FARQ*)
{	// destroy a wchar_t (do nothing)
}


// TEMPLATE CLASS _Allocator_base
template<class _Ty>
struct _Allocator_base
{	// base class for generic allocators
	typedef _Ty value_type;
};

// TEMPLATE CLASS _Allocator_base<const _Ty>
template<class _Ty>
struct _Allocator_base<const _Ty>
{	// base class for generic allocators for const _Ty
	typedef _Ty value_type;
};

// TEMPLATE CLASS aligned_allocator
template<class _Ty, _SIZT alignment>
class aligned_allocator : public _Allocator_base<_Ty>
{	// generic aligned_allocator for objects of class _Ty
public:
	typedef _Allocator_base<_Ty> _Mybase;
	typedef typename _Mybase::value_type value_type;


	typedef value_type _FARQ* pointer;
	typedef value_type _FARQ& reference;
	typedef const value_type _FARQ* const_pointer;
	typedef const value_type _FARQ& const_reference;

	typedef _SIZT index_integerype;
	typedef _PDFT difference_type;

	template<class _Other>
			 struct rebind
	{	// convert an aligned_allocator<_Ty> to an aligned_allocator <_Other>
		typedef aligned_allocator<_Other, alignment> other;
	};

	pointer address(reference _Val) const
	{	// return address of mutable _Val
		return (&_Val);
	}

	const_pointer address(const_reference _Val) const
	{	// return address of nonmutable _Val
		return (&_Val);
	}

	aligned_allocator()
	{	// construct default aligned_allocator (do nothing)
	}

	aligned_allocator(const aligned_allocator<_Ty, alignment>&)
	{	// construct by copying (do nothing)
	}

	template<class _Other>
aligned_allocator(const aligned_allocator<_Other, alignment>&)
{	// construct from a related aligned_allocator (do nothing)
}

	template<class _Other>
aligned_allocator<_Ty, alignment>& operator=(const aligned_allocator<_Other, alignment>&)
{	// assign from a related aligned_allocator (do nothing)
	return (*this);
}

	void deallocate(pointer _Ptr, index_integerype)
	{	// deallocate object at _Ptr, ignore size
		if (_Ptr)
		{
			_aligned_free(_Ptr);
			// operator delete (_Ptr);
#if 0
				std::cout
					<< "  used my_allocator to deallocate at address "
					<< _Ptr << " (-)" << std::endl;
#endif			
		}
	}

	pointer allocate(index_integerype _Count)
	{	
		// allocate array of _Count elements
		return TJL_alloc::_Allocate(_Count, (pointer)0, alignment);
	}

	pointer allocate(index_integerype _Count, const void _FARQ*)
	{	// allocate array of _Count elements, ignore hint
		return (allocate(_Count));
	}

	void construct(pointer _Ptr, const _Ty& _Val)
	{	// construct object at _Ptr with value _Val
		TJL_alloc::_Construct(_Ptr, _Val);
	}

	void destroy(pointer _Ptr)
	{	// destroy object at _Ptr
		TJL_alloc::_Destroy(_Ptr);
	}

	_SIZT max_size() const
	{	// estimate maximum array size
		_SIZT _Count = (_SIZT)(-1) / sizeof (_Ty);
		return (0 < _Count ? _Count : 1);
	}
};

// aligned_allocator TEMPLATE OPERATORS
template<class _Ty, size_t alignment,
		 class _Other>
inline
	bool operator==(const aligned_allocator<_Ty, alignment>&, const aligned_allocator<_Other, alignment>&)
{	// test for aligned_allocator equality (always true)
	return (true);
}

template<class _Ty, size_t alignment,
		 class _Other>
inline
	bool operator!=(const aligned_allocator<_Ty, alignment>&, const aligned_allocator<_Other, alignment>&)
{	// test for aligned_allocator inequality (always false)
	return (false);
}

// CLASS aligned_allocator<void>
template<size_t alignment>
class _CRTIMP2 aligned_allocator<void, alignment>
{	// generic aligned_allocator for type void
	public:
	typedef void _Ty;
	typedef _Ty _FARQ* pointer;
	typedef const _Ty _FARQ* const_pointer;
	typedef _Ty value_type;

		template<class _Other>
	struct rebind
	{	// convert an aligned_allocator<void> to an aligned_allocator <_Other>
		typedef aligned_allocator<_Other, alignment> other;
	};

		aligned_allocator()
	{	// construct default aligned_allocator (do nothing)
	}

		aligned_allocator(const aligned_allocator<_Ty, alignment>&)
	{	// construct by copying (do nothing)
	}

		template<class _Other>
			aligned_allocator(const aligned_allocator<_Other, alignment>&)
	{	// construct from related aligned_allocator (do nothing)
	}

		template<class _Other>
			aligned_allocator<_Ty, alignment >& operator=(const aligned_allocator<_Other, alignment>&)
	{	// assign from a related aligned_allocator (do nothing)
		return (*this);
	}
};

//// TEMPLATE FUNCTION _Destroy_range
//template<class _Ty,
//class _Alloc> inline
//	void _Destroy_range(_Ty *_First, _Ty *_Last, _Alloc& _Al)
//{	// destroy [_First, _Last)
//	_Destroy_range(_First, _Last, _Al, _Ptr_cat(_First, _Last));
//}

//template<class _Ty,
//class _Alloc> inline
//	void _Destroy_range(_Ty *_First, _Ty *_Last, _Alloc& _Al,
//	_Nonscalar_ptr_iterator_tag)
//{	// destroy [_First, _Last), arbitrary type
//	for (; _First != _Last; ++_First)
//		_Al.destroy(_First);
//}
//
}
;//TJL_alloc
#pragma warning(default: 4100)

#pragma warning(pop)
#pragma pack(pop)


/*
* Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
* Consult your license regarding permissions and restrictions.
*/

/*
* This file is derived from software bearing the following
* restrictions:
*
* Copyright (c) 1994
* Hewlett-Packard Company
*
* Permission to use, copy, modify, distribute and sell this
* software and its documentation for any purpose is hereby
* granted without fee, provided that the above copyright notice
* appear in all copies and that both that copyright notice and
* this permission notice appear in supporting documentation.
* Hewlett-Packard Company makes no representations about the
* suitability of this software for any purpose. It is provided
* "as is" without express or implied warranty.
V3.13:0009 */



#endif // aligned_allocator_h__
