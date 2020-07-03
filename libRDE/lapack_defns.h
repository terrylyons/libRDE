#ifndef lapack_defns_h__
#define lapack_defns_h__
#include <map>
#include <vector>
#define MKL
#ifdef MKL
/*
compiler & linker (for large array indexes - drop the I if all calls to lapack use (32 bit) int)

sequential
DMKL_ILP64 -I%MKLROOT%/include
mkl_intel_ilp64.lib mkl_core.lib mkl_sequential.lib

multi-threaded
DMKL_ILP64 /Qopenmp -I%MKLROOT%/include
mkl_intel_ilp64.lib mkl_core.lib mkl_intel_thread.lib (and for VS compiler libiomp5md.lib -ldl)

and for 32 bit simply don't define MKL_ILP64 and change mkl_intel_ilp64.lib to mkl_intel_c.lib
*/

//#include "mkl_types.h"
//typedef MKL_INT integer;

// Check windows
#if _WIN32 || _WIN64

#if _WIN64
#ifndef MKL_LP64
#ifndef MKL_ILP64
#define MKL_ILP64
#endif
#endif
#endif
//  mkl_intel_ilp64_dll.lib mkl_core_dll.lib mkl_intel_thread_dll.lib
//  /DMKL_ILP64 /Qopenmp -I"%MKLROOT%"/include

//  mkl_intel_lp64_dll.lib mkl_core_dll.lib mkl_intel_thread_dll.lib
//  /Qopenmp -I"%MKLROOT%"/include

//  mkl_intel_ilp64_dll.lib mkl_core_dll.lib mkl_sequential_dll.lib
//  /DMKL_ILP64 -I"%MKLROOT%"/include

//  mkl_intel_lp64_dll.lib mkl_core_dll.lib mkl_sequential_dll.lib
//   -I"%MKLROOT%"/include

// mkl_intel_c_dll.lib mkl_core_dll.lib mkl_intel_thread_dll.lib
// /Qopenmp -I"%MKLROOT%"/include

//  mkl_intel_c_dll.lib mkl_core_dll.lib mkl_sequential_dll.lib
//  -I"%MKLROOT%"/include

#ifdef MKL_ILP64
#pragma comment (lib, "mkl_intel_ilp64.lib")
typedef ptrdiff_t integer;
#else
#ifdef _WIN64
#pragma comment (lib, "mkl_intel_lp64.lib")
typedef int integer;
#else
#pragma comment (lib, "mkl_intel_c.lib")
typedef ptrdiff_t integer;
#endif

#endif
#endif

#pragma comment(lib, "mkl_core.lib")           // always
//#pragma comment(lib, "mkl_sequential.lib")     // if single-threaded
#pragma comment(lib, "mkl_intel_thread.lib") // if multi-threaded
#pragma comment(lib, "libiomp5md.lib")       // if multithreaded and using microsoft compiler
//#pragma comment(lib, "libmmdd.lib") // Intel Math Library for dynamic runtime
typedef double doublereal;
#if 1
#include "aligned_allocator.h"
typedef std::vector<doublereal, TJL_alloc::aligned_allocator<doublereal, 16> > dVECTOR;
typedef std::vector<integer, TJL_alloc::aligned_allocator<integer, 16> > iVECTOR;
#else 
typedef std::vector<doublereal> dVECTOR;
typedef std::vector<integer> iVECTOR;
#endif

#define daxpy_ DAXPY
#define ddot_ DDOT
#define dgels_ DGELS
#define dgelsd_ DGELSD
#define dgelss_ DGELSS
#define dnrm2_ DNRM2
#define dgesv_ DGESV
#define dgesvd_ DGESVD
#define dcopy_ DCOPY
#define dgemm_ DGEMM

#else

#pragma comment(lib,  "liblapack.a")
#pragma comment(lib,  "libf77blas.a")
#pragma comment(lib,  "libcblas.a")
#pragma comment(lib,  "libatlas.a")

#pragma comment(lib,  "libg2c.a")
#pragma comment(lib,  "libgcc.a")

typedef double doublereal;
typedef std::vector<doublereal> dVECTOR;
typedef std::vector<integer> iVECTOR;
typedef ptrdiff_t integer;
#endif
typedef std::vector<doublereal>  VECTORD;
typedef std::vector<integer> VECTORI;
typedef VECTORD::const_iterator CONST_D_IT;
typedef VECTORD::iterator D_IT;
typedef VECTORI::const_iterator CONST_I_IT;
typedef VECTORI::iterator I_IT;
typedef size_t index_integer;



// Y=a*X+Y
extern "C"
void daxpy_(integer* N, doublereal* A, doublereal* X, integer* INCX, doublereal* Y, integer* INCY);
// returns X.Y
extern "C"
doublereal ddot_(integer* N, doublereal* X, integer* INCX, doublereal* Y, integer* INCY);
//minimize 2-norm(| b - A*x |)
extern "C"
void dgelsd_(integer* m, integer* n, integer* nrhs, doublereal* a, integer* lda, doublereal* b, integer* ldb,
			 doublereal* s, doublereal* rcond, integer* rank, doublereal* work, integer* lwork, integer* iwork, integer* info);
extern "C"
void dgels_(char* transa, integer* m, integer* n, integer* nrhs, doublereal* a, integer* lda, doublereal* b, integer* ldb,
doublereal* work, integer* lwork, integer* info);
extern "C"
void dgelss_(integer* m, integer* n, integer* nrhs, doublereal* a, integer* lda, doublereal* b, integer* ldb,
			 doublereal* s, doublereal* rcond, integer* rank, doublereal* work, integer* lwork, integer* info);
extern "C"
doublereal dnrm2_(integer* n, doublereal* x, integer* incx);
extern "C"
void dgesv_(integer* n, integer* nrhs, doublereal* a, integer* lda, integer* ipiv, doublereal* b, integer* ldb, integer* info);
extern "C"
void dgesvd_(char* jobu, char* jobvt, integer* m, integer* n, doublereal* a, integer* lda, doublereal* s, doublereal* u, integer* ldu, doublereal* vt, integer* ldvt, doublereal* work, integer* lwork, integer* info);
extern "C"
void dcopy_(integer* n, doublereal* x, integer* incx, doublereal* y, integer* incy);
extern "C"
void dgemm_(char* transa, char* transb, integer* m, integer* n, integer* k, doublereal* alpha, doublereal* a, integer
			* lda, doublereal* b, integer* ldb, doublereal* beta, doublereal* c, integer* ldc);



////#include "mkl_types.h"
////typedef MKL_INT integer;
//
//// Check windows
//#if _WIN32 || _WIN64
//
//#if _WIN64
//#ifndef MKL_ILP64
//#define MKL_ILP64
//#endif
//#endif
//#ifdef MKL_ILP64
//#pragma comment (lib, "mkl_intel_ilp64.lib")
//typedef ptrdiff_t integer;
//#else
//#ifdef _WIN64
//#pragma comment (lib, "mkl_intel_lp64.lib")
//typedef int integer;
//#else
//#pragma comment (lib, "mkl_intel_c.lib")
//typedef ptrdiff_t integer;
//#endif
//#endif
//#endif
//typedef double doublereal;
//
//// Y=a*X+Y
//extern "C"
//void DAXPY(integer* N, doublereal* A, doublereal* X, integer* INCX, doublereal* Y, integer* INCY);
//
//extern "C"
//void DGEMM(char* transa, char* transb, integer* m, integer* n, integer* k, doublereal* alpha, doublereal* a, integer
//* lda, doublereal* b, integer* ldb, doublereal* beta, doublereal* c, integer* ldc);
//
//extern "C"
//void DGELS(char* transa, integer* m, integer* n, integer* nrhs, doublereal* a, integer* lda, doublereal* b, integer* ldb,
//doublereal* work, integer* lwork, integer* info);
//
//extern "C"
//void DGELSS(integer* m, integer* n, integer* nrhs, doublereal* a, integer* lda, doublereal* b, integer* ldb,
//doublereal*
//s, doublereal* rcond, integer* rank, doublereal* work, integer* lwork, integer* info);
//
//extern "C"
//void DGESVD(char* jobu, char* jobvt, integer* m, integer* n, doublereal* a, integer* lda, doublereal* s, doublereal* u, integer* ldu, doublereal* vt, integer* ldvt, doublereal* work, integer* lwork, integer* info);
//

#endif // lapack_defns_h__
