#ifndef EMM_VEC_TESTS // only include once.
#define EMM_VEC_TESTS 

#include <typeinfo>
#include "emm_vec.hxx"
#include <string>


template <typename vecptrtype, typename T> struct wrapptr {
  static vecptrtype get( T * ptr , int numtests, int numel ) {
    return vecptrtype( ptr  );
  };
};

template <typename T, int vlen>  struct wrapptr< vecptr< typename guard_pointer_type< T* >::type , vlen> , T > { 
  typedef typename guard_pointer_type< T* >::type gptrT;
  typedef vecptr< gptrT, vlen>  vecptrtype;
  static vecptrtype get( T * ptr, int numtests , int numel ) {
    gptrT gptr( ptr  ,  ptr+numel);
    return vecptrtype(  gptr );
  };
};

template <typename T, int vlen>  struct wrapptr< vecptr_step< T* , vlen> , T > { 
  typedef vecptr_step< T* , vlen>  vecptrtype;
  static vecptrtype get( T * ptr, int numtests , int numel ) {
    return vecptrtype(  ptr , numtests );
  };
};
template <typename T, int vlen>  struct wrapptr< vecptr_step< typename guard_pointer_type< T* >::type, vlen> , T > { 
  typedef typename guard_pointer_type< T* >::type gptrT;
  typedef vecptr_step< gptrT , vlen>  vecptrtype;
  static vecptrtype get( T * ptr, int numtests , int numel ) {
    gptrT gptr( ptr  ,  ptr+numel);
    return vecptrtype(  gptr , numtests );
  };
};

// structure with usefull values for testing. The defaults can be used for normal testing of floating point values:
template< typename T=int, int nr=0 > struct testLims {
  static T scale1()   {return 1;   };
  static T scale2()   {return 1;   };

  static T plus()     {return 123; };
  static T minus()    {return 134; };
  static T multiply() {return 12;  };
  static T divide()   {return 3;   };

  static T is()       {return 123; };
  static T plusis()   {return 15435;};
  static T minusis()  {return 12344; };
  static T multiplyis(){return 73; };
  static T divideis() {return 5; };
private: testLims(){};
};

// Integer types should behave normally when wrapping around
template< > struct testLims< int32_t, 0> {
  typedef int32_t T;
  static T scale1()   {return 429496751;   };
  static T scale2()   {return 45007;   };

  static const T plus () {return  2147483646;};
  static const T minus () {return  2147483645;};
  static const T multiply () {return  5052911;};
  static const T divide () {return  3;};

  static T is()       {return 123; };
  static const T plusis () {return  -2147483646;};
  static const T minusis () {return  -2147483643;};
  static const T multiplyis () {return  -715827883;};
  static const T divideis () {return  5;};
private: testLims(){};
};

// Integer types should behave normally when wrapping around
template< > struct testLims< int64_t, 0> {
  typedef int64_t T;
  static T scale1()   {return 429496751;   };
  static T scale2()   {return 613566757;   };

  static const T plus () {return  9223372036854775806;};
  static const T minus () {return  9223372036854775805;};
  static const T multiply () {return  3074457345618258603;};
  static const T divide () {return  3;};

  static T is()       {return 123; };
  static const T plusis () {return  -9223372036854775806;};
  static const T minusis () {return  -9223372036854775805;};
  static const T multiplyis () {return  -3074457345618258603;};
  static const T divideis () {return  5;};
private: testLims(){};
};

template < typename T, int vlen, typename vecptrtype, typename lims = testLims<float,0>, bool hasDivision = true> struct test_vec { 
  enum {numvecs = 10};
  static const int numel = numvecs * vlen;
  typedef vec<T, vlen> vecType;

  // first some internal macros:
// Test if out1 and out2 have the same content:
#define TESTEQ( OP , TESTNM)\
  {bool didnotyetwarn = true;\
  for ( int i = 0; i < numel; ++i ) {\
    if ( (out1[i] != out2[i])  && (didnotyetwarn) ) {\
	WARNMSG( (std::string("Operator "  #OP  " has a difference in " #TESTNM " in ") + typeid(vecType).name() ).c_str() );\
      didnotyetwarn = false;\
    };\
    out1[i] = 12376;\
  };}

// Define test macro for binary op
#define TEST_BINARY_OP( OP , CVAL) \
  /* standard c reference run:*/\
  for ( int i = 0; i < numel; ++i ) {\
    out2[i] = in1[ i ] OP in2[ i ] ;\
  }\
  /* vec OP vec */\
  for ( int i = 0; i < numvecs; ++i ) {\
    vecType v1 = vIn1[ i ] ;\
    vecType v2(vIn2[ i ]) ;  /* use different constructors*/\
    vecType o1 = v1 OP v2;\
    *(vOut1+i) = o1; \
  }\
  TESTEQ( #OP , 1 );\
  /* vecptr OP vecptr */\
  for ( int i = 0; i < numvecs; ++i ) {\
    vOut1[i] = vIn1[ i ] OP vIn2[ i ] ;\
  }\
  \
  TESTEQ( #OP , 2);\
  {T c = CVAL;\
  /* standard c reference run:*/\
  for ( int i = 0; i < numel; ++i ) {\
    out2[i] = in2[ i ] OP c;\
  }\
  /* vecptr OP scalar */\
  for ( int i = 0; i < numvecs; ++i ) {\
    vOut1[i] = vIn2[ i ] OP c;\
  }\
  TESTEQ( #OP , 4)\
  /* vec OP scalar */\
  for ( int i = 0; i < numvecs; ++i ) {\
    vecType v2( *(vIn2+ i) ) ;\
    vecType vo1 = v2 OP c;\
    vOut1[i] = vo1;\
  }}\
  TESTEQ( #OP , 5) 
  /*
  for ( int i = 0; i < numel; ++i ) {\
    out2[i] = c OP in2[ i ];\
  }
  for ( int i = 0; i < numvecs; ++i ) {\
    vOut1[i] = c OP vIn2[ i ] ;\
  }\
  TESTEQ( "+" );*/\

// Define macro for assignments operations
#define TEST_ASGN_OP( OP , CVAL) \
  /* initialize out1 and out2 and perform OP on out2*/\
  for ( int i = 0; i < numel; ++i ) {\
    out1[i] = in1[ i ];\
	out2[i] = in1[ i ];\
    out2[i] OP in2[ i ] ;\
  }\
  /* perform OP on out1; vec_store_helper OP vec */\
  for ( int i = 0; i < numvecs; ++i ) {\
    vecType v2(vIn2[ i ]) ;  \
    *(vOut1+i) OP v2; \
  }\
  TESTEQ( #OP , 11 );\
  /* re-initialize out1*/\
  for ( int i = 0; i < numel; ++i ) {\
    out1[i] = in1[ i ];\
  }\
  /* perform OP on out1; vec_store_helper OP vec_store_helper */\
  for ( int i = 0; i < numvecs; ++i ) {\
    vOut1[i] OP vIn2[ i ] ;\
  }\
  TESTEQ( #OP , 12);\
  /* re-initialize out1*/\
  for ( int i = 0; i < numel; ++i ) {\
    out1[i] = in1[ i ];\
  }\
  /* perform OP on out1; vec OP vec*/\
  for ( int i = 0; i < numvecs; ++i ) {\
	vecType v2(vIn2[ i ]) ;  \
	vecType o1 = *(vOut1+ i) ;  \
    o1 OP v2 ;\
	vOut1[i] = o1;\
  }\
  TESTEQ( #OP , 13);\
  /* re-initialize out1 and out2 and perform OP on out2 with scalar 2nd argument*/\
  {T c = CVAL;\
  for ( int i = 0; i < numel; ++i ) {\
    out1[i] = in1[ i ];\
	out2[i] = in1[ i ];\
    out2[i] OP c;\
  }\
  /*for ( int i = 0; i < numvecs; ++i ) {\
    vOut1[i] OP c;\
  }\
  TESTEQ( #OP , 14)\
  /* re-initialize out1*/\
  for ( int i = 0; i < numel; ++i ) {\
    out1[i] = in1[ i ];\
  }\
  /* perform OP on out1; vec OP scalar*/\
  for ( int i = 0; i < numvecs; ++i ) {\
	vecType o1 = *(vOut1+ i) ;  \
    o1 OP c ;\
	vOut1[i] = o1;\
  }\
  TESTEQ( #OP , 15)}

// Define and fill some arrays used for the tests:
#define INIT_TESTS( scale1, scale2 ) \
  T  in1[ numel ];\
  T  in2[ numel ];\
  T  out1[ numel ];\
  T  out2[ numel ];\
\
  for ( int i = 0; i < numel; ++i ) {\
    in1[ i ] = (i-3)*scale1;\
    in2[ i ] = (numel*(numel-1)/2 - i*i)*scale2;\
  }\
  \
  vecptrtype vIn1 = wrapptr<vecptrtype, T>::get( &in1[0] , numvecs,  numel );\
  vecptrtype vIn2 = wrapptr<vecptrtype, T>::get( &in2[0] , numvecs, numel );\
  vecptrtype vOut1= wrapptr<vecptrtype, T>::get( &out1[0] , numvecs, numel )


// Main test routine:
  static void test() {
    DISPMSG( (std::string("Testing ") + typeid(vecType).name() + " with pointer type " + typeid(vecptrtype).name() + "\n").c_str() );

    INIT_TESTS( lims::scale1(), lims::scale2() );
    // Perform binary tests:
    TEST_BINARY_OP( + , lims::plus() ) ;
    TEST_BINARY_OP( - , lims::minus() ) ;
    TEST_BINARY_OP( * , lims::multiply()  ) ;


    // Perform assignement operation tests:
    TEST_ASGN_OP( = , lims::is());
    TEST_ASGN_OP( += , lims::plusis());
    TEST_ASGN_OP( -= , lims::minusis());
    TEST_ASGN_OP( *= , lims::multiplyis());
    /*if (hasDivision) {
      test_division();
    };*/
  };
  static void test_division() {
	  INIT_TESTS( lims::scale1(), lims::scale2() );

    TEST_BINARY_OP( / , lims::divide() ) ;
    TEST_ASGN_OP( /= , lims::divideis() );
  };
};

#endif