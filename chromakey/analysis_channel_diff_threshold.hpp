#define PY_MAJOR_VERSION 3
#undef ENABLE_PYTHON_MODULE
#include <pythonic/core.hpp>
#include <pythonic/python/core.hpp>
#include <pythonic/types/bool.hpp>
#include <pythonic/types/int.hpp>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <pythonic\include\builtins\False.hpp>
#include <pythonic\include\builtins\True.hpp>
#include <pythonic\include\builtins\enumerate.hpp>
#include <pythonic\include\builtins\getattr.hpp>
#include <pythonic\include\builtins\int_.hpp>
#include <pythonic\include\builtins\max.hpp>
#include <pythonic\include\builtins\print.hpp>
#include <pythonic\include\builtins\pythran\and_.hpp>
#include <pythonic\include\builtins\pythran\static_list.hpp>
#include <pythonic\include\builtins\range.hpp>
#include <pythonic\include\builtins\tuple.hpp>
#include <pythonic\include\numpy\array.hpp>
#include <pythonic\include\numpy\convolve.hpp>
#include <pythonic\include\numpy\int32.hpp>
#include <pythonic\include\numpy\ndarray\astype.hpp>
#include <pythonic\include\numpy\sum.hpp>
#include <pythonic\include\numpy\zeros.hpp>
#include <pythonic\include\operator_\add.hpp>
#include <pythonic\include\operator_\div.hpp>
#include <pythonic\include\operator_\eq.hpp>
#include <pythonic\include\operator_\ge.hpp>
#include <pythonic\include\operator_\gt.hpp>
#include <pythonic\include\operator_\iadd.hpp>
#include <pythonic\include\operator_\idiv.hpp>
#include <pythonic\include\operator_\le.hpp>
#include <pythonic\include\operator_\lt.hpp>
#include <pythonic\include\operator_\mul.hpp>
#include <pythonic\include\operator_\ne.hpp>
#include <pythonic\include\operator_\not_.hpp>
#include <pythonic\include\operator_\pow.hpp>
#include <pythonic\include\operator_\sub.hpp>
#include <pythonic\include\types\slice.hpp>
#include <pythonic\include\types\str.hpp>
#include <pythonic\builtins\False.hpp>
#include <pythonic\builtins\True.hpp>
#include <pythonic\builtins\enumerate.hpp>
#include <pythonic\builtins\getattr.hpp>
#include <pythonic\builtins\int_.hpp>
#include <pythonic\builtins\max.hpp>
#include <pythonic\builtins\print.hpp>
#include <pythonic\builtins\pythran\and_.hpp>
#include <pythonic\builtins\pythran\static_list.hpp>
#include <pythonic\builtins\range.hpp>
#include <pythonic\builtins\tuple.hpp>
#include <pythonic\numpy\array.hpp>
#include <pythonic\numpy\convolve.hpp>
#include <pythonic\numpy\int32.hpp>
#include <pythonic\numpy\ndarray\astype.hpp>
#include <pythonic\numpy\sum.hpp>
#include <pythonic\numpy\zeros.hpp>
#include <pythonic\operator_\add.hpp>
#include <pythonic\operator_\div.hpp>
#include <pythonic\operator_\eq.hpp>
#include <pythonic\operator_\ge.hpp>
#include <pythonic\operator_\gt.hpp>
#include <pythonic\operator_\iadd.hpp>
#include <pythonic\operator_\idiv.hpp>
#include <pythonic\operator_\le.hpp>
#include <pythonic\operator_\lt.hpp>
#include <pythonic\operator_\mul.hpp>
#include <pythonic\operator_\ne.hpp>
#include <pythonic\operator_\not_.hpp>
#include <pythonic\operator_\pow.hpp>
#include <pythonic\operator_\sub.hpp>
#include <pythonic\types\slice.hpp>
#include <pythonic\types\str.hpp>
namespace __pythran_analysis_channel_diff_threshold
{
  struct analysis_channel_diff_threshold_compare1
  {
    typedef void callable;
    typedef void pure;
    template <typename argument_type0 >
    struct type
    {
      typedef typename pythonic::returnable<bool>::type result_type;
    }  
    ;
    template <typename argument_type0 >
    typename type<argument_type0>::result_type operator()(argument_type0&& diff) const
    ;
  }  ;
  struct analysis_channel_diff_threshold_compare0
  {
    typedef void callable;
    typedef void pure;
    template <typename argument_type0 >
    struct type
    {
      typedef typename pythonic::returnable<bool>::type result_type;
    }  
    ;
    template <typename argument_type0 >
    typename type<argument_type0>::result_type operator()(argument_type0&& diff) const
    ;
  }  ;
  struct find_next_continus_range
  {
    typedef void callable;
    typedef void pure;
    template <typename argument_type0 , typename argument_type1 , typename argument_type2 , typename argument_type3 >
    struct type
    {
      typedef typename pythonic::assignable<long>::type __type0;
      typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::builtins::functor::range{})>::type>::type __type1;
      typedef typename std::remove_cv<typename std::remove_reference<argument_type2>::type>::type __type2;
      typedef typename std::remove_cv<typename std::remove_reference<argument_type3>::type>::type __type3;
      typedef decltype(std::declval<__type1>()(std::declval<__type2>(), std::declval<__type3>())) __type4;
      typedef typename pythonic::assignable<typename std::remove_cv<typename std::iterator_traits<typename std::remove_reference<__type4>::type::iterator>::value_type>::type>::type __type5;
      typedef typename __combined<__type0,__type5>::type __type6;
      typedef typename pythonic::returnable<decltype(pythonic::types::make_tuple(std::declval<__type6>(), std::declval<__type6>()))>::type result_type;
    }  
    ;
    template <typename argument_type0 , typename argument_type1 , typename argument_type2 , typename argument_type3 >
    typename type<argument_type0, argument_type1, argument_type2, argument_type3>::result_type operator()(argument_type0&& values, argument_type1&& low_threshold, argument_type2&& from_index, argument_type3&& to_index) const
    ;
  }  ;
  struct find_max_length_continus_range
  {
    typedef void callable;
    typedef void pure;
    template <typename argument_type0 , typename argument_type1 , typename argument_type2 , typename argument_type3 >
    struct type
    {
      typedef typename std::remove_cv<typename std::remove_reference<argument_type2>::type>::type __type0;
      typedef typename pythonic::assignable<long>::type __type1;
      typedef find_next_continus_range __type2;
      typedef typename std::remove_cv<typename std::remove_reference<argument_type0>::type>::type __type3;
      typedef typename std::remove_cv<typename std::remove_reference<argument_type1>::type>::type __type4;
      typedef typename pythonic::assignable<typename std::remove_cv<typename std::remove_reference<argument_type2>::type>::type>::type __type5;
      typedef typename std::remove_cv<typename std::remove_reference<argument_type3>::type>::type __type6;
      typedef typename pythonic::assignable<decltype(std::declval<__type2>()(std::declval<__type3>(), std::declval<__type4>(), std::declval<__type5>(), std::declval<__type6>()))>::type __type7;
      typedef typename pythonic::assignable<typename std::tuple_element<0,typename std::remove_reference<__type7>::type>::type>::type __type8;
      typedef typename pythonic::assignable<typename std::tuple_element<1,typename std::remove_reference<__type7>::type>::type>::type __type9;
      typedef typename pythonic::assignable<decltype(pythonic::types::make_tuple(std::declval<__type8>(), std::declval<__type9>()))>::type __type10;
      typedef typename pythonic::assignable<typename std::tuple_element<0,typename std::remove_reference<__type10>::type>::type>::type __type11;
      typedef typename __combined<__type1,__type11>::type __type12;
      typedef typename pythonic::assignable<typename std::tuple_element<1,typename std::remove_reference<__type10>::type>::type>::type __type13;
      typedef typename __combined<__type1,__type13>::type __type14;
      typedef __type0 __ptype0;
      typedef typename pythonic::returnable<decltype(pythonic::types::make_tuple(std::declval<__type12>(), std::declval<__type14>()))>::type result_type;
    }  
    ;
    template <typename argument_type0 , typename argument_type1 , typename argument_type2 , typename argument_type3 >
    typename type<argument_type0, argument_type1, argument_type2, argument_type3>::result_type operator()(argument_type0&& values, argument_type1&& low_threshold, argument_type2&& from_index, argument_type3&& to_index) const
    ;
  }  ;
  struct get_one_channel_diff_threshold
  {
    typedef void callable;
    typedef void pure;
    template <typename argument_type0 >
    struct type
    {
      typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::numpy::functor::convolve{})>::type>::type __type0;
      typedef typename std::remove_cv<typename std::remove_reference<argument_type0>::type>::type __type1;
      typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::builtins::pythran::functor::static_list{})>::type>::type __type2;
      typedef double __type3;
      typedef decltype(pythonic::types::make_tuple(std::declval<__type3>(), std::declval<__type3>(), std::declval<__type3>(), std::declval<__type3>(), std::declval<__type3>(), std::declval<__type3>(), std::declval<__type3>(), std::declval<__type3>(), std::declval<__type3>())) __type4;
      typedef decltype(std::declval<__type2>()(std::declval<__type4>())) __type5;
      typedef decltype(std::declval<__type0>()(std::declval<__type1>(), std::declval<__type5>())) __type6;
      typedef pythonic::types::contiguous_slice __type7;
      typedef typename pythonic::assignable<decltype(std::declval<__type6>()[std::declval<__type7>()])>::type __type8;
      typedef typename pythonic::assignable<long>::type __type9;
      typedef find_max_length_continus_range __type10;
      typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::numpy::functor::sum{})>::type>::type __type11;
      typedef decltype(std::declval<__type11>()(std::declval<__type8>())) __type12;
      typedef decltype(pythonic::builtins::getattr(pythonic::types::attr::SHAPE{}, std::declval<__type8>())) __type13;
      typedef typename std::tuple_element<0,typename std::remove_reference<__type13>::type>::type __type14;
      typedef typename pythonic::assignable<decltype(pythonic::operator_::div(std::declval<__type12>(), std::declval<__type14>()))>::type __type15;
      typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::builtins::functor::range{})>::type>::type __type16;
      typedef long __type17;
      typedef decltype(std::declval<__type16>()(std::declval<__type17>())) __type18;
      typedef typename std::remove_cv<typename std::iterator_traits<typename std::remove_reference<__type18>::type::iterator>::value_type>::type __type19;
      typedef decltype(pythonic::operator_::sub(std::declval<__type19>(), std::declval<__type17>())) __type20;
      typedef decltype(pythonic::builtins::pow(std::declval<__type3>(), std::declval<__type20>())) __type21;
      typedef decltype(pythonic::operator_::mul(std::declval<__type15>(), std::declval<__type21>())) __type22;
      typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::builtins::functor::int_{})>::type>::type __type23;
      typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::builtins::functor::enumerate{})>::type>::type __type24;
      typedef decltype(std::declval<__type24>()(std::declval<__type8>())) __type25;
      typedef typename std::remove_cv<typename std::iterator_traits<typename std::remove_reference<__type25>::type::iterator>::value_type>::type __type26;
      typedef typename std::tuple_element<0,typename std::remove_reference<__type26>::type>::type __type27;
      typedef typename std::tuple_element<1,typename std::remove_reference<__type26>::type>::type __type28;
      typedef decltype(pythonic::operator_::mul(std::declval<__type27>(), std::declval<__type28>())) __type29;
      typedef decltype(pythonic::operator_::add(std::declval<__type9>(), std::declval<__type29>())) __type30;
      typedef typename __combined<__type9,__type30>::type __type31;
      typedef typename __combined<__type31,__type29>::type __type32;
      typedef decltype(pythonic::operator_::div(std::declval<__type32>(), std::declval<__type12>())) __type33;
      typedef typename pythonic::assignable<decltype(std::declval<__type23>()(std::declval<__type33>()))>::type __type34;
      typedef typename find_max_length_continus_range::type<__type8, __type22, __type34, __type14>::__ptype0 __type35;
      typedef typename __combined<__type34,__type35>::type __type36;
      typedef typename pythonic::assignable<decltype(std::declval<__type10>()(std::declval<__type8>(), std::declval<__type22>(), std::declval<__type36>(), std::declval<__type14>()))>::type __type37;
      typedef typename pythonic::assignable<typename std::tuple_element<0,typename std::remove_reference<__type37>::type>::type>::type __type38;
      typedef typename pythonic::assignable<typename std::tuple_element<1,typename std::remove_reference<__type37>::type>::type>::type __type39;
      typedef typename pythonic::assignable<decltype(pythonic::types::make_tuple(std::declval<__type38>(), std::declval<__type39>()))>::type __type40;
      typedef typename pythonic::assignable<typename std::tuple_element<0,typename std::remove_reference<__type40>::type>::type>::type __type41;
      typedef typename __combined<__type9,__type41>::type __type42;
      typedef decltype(pythonic::operator_::mul(std::declval<__type42>(), std::declval<__type3>())) __type43;
      typedef typename pythonic::assignable<typename std::tuple_element<1,typename std::remove_reference<__type40>::type>::type>::type __type44;
      typedef typename __combined<__type9,__type44>::type __type45;
      typedef decltype(pythonic::operator_::mul(std::declval<__type45>(), std::declval<__type3>())) __type46;
      typedef decltype(pythonic::operator_::add(std::declval<__type43>(), std::declval<__type46>())) __type47;
      typedef decltype(pythonic::operator_::sub(std::declval<__type45>(), std::declval<__type42>())) __type48;
      typedef decltype(pythonic::operator_::div(std::declval<__type48>(), std::declval<__type17>())) __type49;
      typedef typename pythonic::returnable<decltype(pythonic::types::make_tuple(std::declval<__type8>(), std::declval<__type47>(), std::declval<__type49>()))>::type result_type;
    }  
    ;
    template <typename argument_type0 >
    typename type<argument_type0>::result_type operator()(argument_type0&& hist) const
    ;
  }  ;
  struct analysis_channel_diff_threshold
  {
    typedef void callable;
    ;
    template <typename argument_type0 , typename argument_type1 = bool>
    struct type
    {
      typedef get_one_channel_diff_threshold __type0;
      typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::numpy::functor::zeros{})>::type>::type __type1;
      typedef long __type2;
      typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::numpy::functor::int32{})>::type>::type __type3;
      typedef typename pythonic::assignable<decltype(std::declval<__type1>()(std::declval<__type2>(), std::declval<__type3>()))>::type __type4;
      typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::builtins::functor::int_{})>::type>::type __type5;
      typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::numpy::ndarray::functor::astype{})>::type>::type __type6;
      typedef typename std::remove_cv<typename std::remove_reference<argument_type0>::type>::type __type7;
      typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::builtins::functor::range{})>::type>::type __type8;
      typedef decltype(pythonic::builtins::getattr(pythonic::types::attr::SHAPE{}, std::declval<__type7>())) __type9;
      typedef pythonic::types::fast_contiguous_slice __type10;
      typedef typename pythonic::assignable<decltype(std::declval<__type9>()[std::declval<__type10>()])>::type __type11;
      typedef typename std::tuple_element<0,typename std::remove_reference<__type11>::type>::type __type12;
      typedef typename pythonic::lazy<__type12>::type __type13;
      typedef decltype(std::declval<__type8>()(std::declval<__type2>(), std::declval<__type13>(), std::declval<__type2>())) __type14;
      typedef typename std::remove_cv<typename std::iterator_traits<typename std::remove_reference<__type14>::type::iterator>::value_type>::type __type15;
      typedef typename std::tuple_element<1,typename std::remove_reference<__type11>::type>::type __type16;
      typedef typename pythonic::lazy<__type16>::type __type17;
      typedef decltype(std::declval<__type8>()(std::declval<__type2>(), std::declval<__type17>(), std::declval<__type2>())) __type18;
      typedef typename std::remove_cv<typename std::iterator_traits<typename std::remove_reference<__type18>::type::iterator>::value_type>::type __type19;
      typedef decltype(pythonic::types::make_tuple(std::declval<__type15>(), std::declval<__type19>())) __type20;
      typedef decltype(std::declval<__type7>()[std::declval<__type20>()]) __type21;
      typedef typename pythonic::assignable<decltype(std::declval<__type6>()(std::declval<__type21>(), std::declval<__type3>()))>::type __type22;
      typedef typename pythonic::assignable<typename std::tuple_element<1,typename std::remove_reference<__type22>::type>::type>::type __type23;
      typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::builtins::functor::max{})>::type>::type __type24;
      typedef typename pythonic::assignable<typename std::tuple_element<0,typename std::remove_reference<__type22>::type>::type>::type __type25;
      typedef typename pythonic::assignable<typename std::tuple_element<2,typename std::remove_reference<__type22>::type>::type>::type __type26;
      typedef typename __combined<__type25,__type26>::type __type27;
      typedef decltype(std::declval<__type24>()(std::declval<__type27>(), std::declval<__type26>())) __type28;
      typedef decltype(pythonic::operator_::sub(std::declval<__type23>(), std::declval<__type28>())) __type29;
      typedef decltype(pythonic::operator_::mul(std::declval<__type2>(), std::declval<__type29>())) __type30;
      typedef decltype(std::declval<__type5>()(std::declval<__type30>())) __type31;
      typedef decltype(pythonic::operator_::sub(std::declval<__type2>(), std::declval<__type31>())) __type32;
      typedef decltype(pythonic::operator_::sub(std::declval<__type32>(), std::declval<__type2>())) __type33;
      typedef decltype(pythonic::operator_::mul(std::declval<__type30>(), std::declval<__type2>())) __type34;
      typedef decltype(pythonic::operator_::sub(std::declval<__type29>(), std::declval<__type23>())) __type35;
      typedef decltype(pythonic::operator_::sub(std::declval<__type35>(), std::declval<__type28>())) __type36;
      typedef decltype(pythonic::operator_::mul(std::declval<__type34>(), std::declval<__type36>())) __type37;
      typedef decltype(std::declval<__type5>()(std::declval<__type37>())) __type38;
      typedef decltype(pythonic::operator_::sub(std::declval<__type33>(), std::declval<__type38>())) __type39;
      typedef indexable<__type39> __type40;
      typedef typename __combined<__type4,__type40>::type __type41;
      typedef typename __combined<__type27,__type23>::type __type42;
      typedef decltype(std::declval<__type24>()(std::declval<__type42>(), std::declval<__type23>())) __type43;
      typedef decltype(pythonic::operator_::sub(std::declval<__type26>(), std::declval<__type43>())) __type44;
      typedef decltype(pythonic::operator_::mul(std::declval<__type2>(), std::declval<__type44>())) __type45;
      typedef decltype(std::declval<__type5>()(std::declval<__type45>())) __type46;
      typedef decltype(pythonic::operator_::sub(std::declval<__type2>(), std::declval<__type46>())) __type47;
      typedef decltype(pythonic::operator_::sub(std::declval<__type47>(), std::declval<__type2>())) __type48;
      typedef decltype(pythonic::operator_::mul(std::declval<__type45>(), std::declval<__type2>())) __type49;
      typedef decltype(pythonic::operator_::sub(std::declval<__type44>(), std::declval<__type26>())) __type50;
      typedef decltype(pythonic::operator_::sub(std::declval<__type50>(), std::declval<__type43>())) __type51;
      typedef decltype(pythonic::operator_::mul(std::declval<__type49>(), std::declval<__type51>())) __type52;
      typedef decltype(std::declval<__type5>()(std::declval<__type52>())) __type53;
      typedef decltype(pythonic::operator_::sub(std::declval<__type48>(), std::declval<__type53>())) __type54;
      typedef indexable<__type54> __type55;
      typedef typename __combined<__type41,__type55>::type __type56;
      typedef container<typename std::remove_reference<__type2>::type> __type57;
      typedef typename __combined<__type56,__type57>::type __type58;
      typedef typename pythonic::assignable<decltype(std::declval<__type0>()(std::declval<__type58>()))>::type __type59;
      typedef typename pythonic::assignable<typename std::tuple_element<1,typename std::remove_reference<__type59>::type>::type>::type __type60;
      typedef decltype(pythonic::operator_::div(std::declval<__type60>(), std::declval<__type2>())) __type61;
      typedef typename __combined<__type60,__type61>::type __type62;
      typedef typename __combined<__type62,__type2>::type __type63;
      typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::numpy::functor::array{})>::type>::type __type64;
      typedef decltype(pythonic::types::make_tuple(std::declval<__type2>(), std::declval<__type2>(), std::declval<__type2>())) __type65;
      typedef typename pythonic::assignable<decltype(std::declval<__type64>()(std::declval<__type65>(), std::declval<__type3>()))>::type __type66;
      typedef decltype(pythonic::types::make_tuple(std::declval<__type27>(), std::declval<__type23>(), std::declval<__type26>())) __type67;
      typedef decltype(pythonic::operator_::add(std::declval<__type66>(), std::declval<__type67>())) __type68;
      typedef typename __combined<__type66,__type68>::type __type69;
      typedef decltype(pythonic::types::make_tuple(std::declval<__type42>(), std::declval<__type23>(), std::declval<__type26>())) __type70;
      typedef decltype(pythonic::operator_::add(std::declval<__type69>(), std::declval<__type70>())) __type71;
      typedef typename __combined<__type69,__type71>::type __type72;
      typedef typename __combined<__type72,__type67>::type __type73;
      typedef typename __combined<__type73,__type70>::type __type74;
      typedef typename pythonic::assignable<long>::type __type75;
      typedef decltype(pythonic::operator_::add(std::declval<__type75>(), std::declval<__type2>())) __type76;
      typedef typename __combined<__type75,__type76>::type __type77;
      typedef decltype(pythonic::operator_::add(std::declval<__type77>(), std::declval<__type2>())) __type78;
      typedef typename __combined<__type77,__type78>::type __type79;
      typedef typename __combined<__type79,__type2>::type __type80;
      typedef decltype(pythonic::operator_::div(std::declval<__type74>(), std::declval<__type80>())) __type81;
      typedef typename pythonic::returnable<decltype(pythonic::types::make_tuple(std::declval<__type63>(), std::declval<__type81>()))>::type result_type;
    }  
    ;
    template <typename argument_type0 , typename argument_type1 = bool>
    typename type<argument_type0, argument_type1>::result_type operator()(argument_type0&& image, argument_type1 is_blue_screen= false) const
    ;
  }  ;
  template <typename argument_type0 >
  typename analysis_channel_diff_threshold_compare1::type<argument_type0>::result_type analysis_channel_diff_threshold_compare1::operator()(argument_type0&& diff) const
  {
    if (pythonic::operator_::not_(pythonic::operator_::le(0L, diff)))
    {
      return pythonic::builtins::False;
    }
    if (pythonic::operator_::not_(pythonic::operator_::lt(diff, 256L)))
    {
      return pythonic::builtins::False;
    }
    return pythonic::builtins::True;
  }
  template <typename argument_type0 >
  typename analysis_channel_diff_threshold_compare0::type<argument_type0>::result_type analysis_channel_diff_threshold_compare0::operator()(argument_type0&& diff) const
  {
    if (pythonic::operator_::not_(pythonic::operator_::le(0L, diff)))
    {
      return pythonic::builtins::False;
    }
    if (pythonic::operator_::not_(pythonic::operator_::lt(diff, 256L)))
    {
      return pythonic::builtins::False;
    }
    return pythonic::builtins::True;
  }
  template <typename argument_type0 , typename argument_type1 , typename argument_type2 , typename argument_type3 >
  typename find_next_continus_range::type<argument_type0, argument_type1, argument_type2, argument_type3>::result_type find_next_continus_range::operator()(argument_type0&& values, argument_type1&& low_threshold, argument_type2&& from_index, argument_type3&& to_index) const
  {
    typedef typename pythonic::assignable<long>::type __type0;
    typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::builtins::functor::range{})>::type>::type __type1;
    typedef typename std::remove_cv<typename std::remove_reference<argument_type2>::type>::type __type2;
    typedef typename std::remove_cv<typename std::remove_reference<argument_type3>::type>::type __type3;
    typedef decltype(std::declval<__type1>()(std::declval<__type2>(), std::declval<__type3>())) __type4;
    typedef typename pythonic::assignable<typename std::remove_cv<typename std::iterator_traits<typename std::remove_reference<__type4>::type::iterator>::value_type>::type>::type __type5;
    typename pythonic::assignable<typename std::remove_cv<typename std::iterator_traits<typename std::remove_reference<__type4>::type::iterator>::value_type>::type>::type i;
    typename pythonic::assignable<typename __combined<__type0,__type5>::type>::type start = -1L;
    typename pythonic::assignable<typename __combined<__type0,__type5>::type>::type end = -1L;
    {
      long  __target1566531027464 = to_index;
      for (long  i=from_index; i < __target1566531027464; i += 1L)
      {
        if (pythonic::operator_::le(values[i], low_threshold))
        {
          if (pythonic::operator_::eq(start, -1L))
          {
            start = i;
            end = i;
          }
          else
          {
            end = i;
          }
        }
        else
        {
          if (pythonic::operator_::ne(start, -1L))
          {
            break;
          }
        }
      }
    }
    return pythonic::types::make_tuple(start, end);
  }
  template <typename argument_type0 , typename argument_type1 , typename argument_type2 , typename argument_type3 >
  typename find_max_length_continus_range::type<argument_type0, argument_type1, argument_type2, argument_type3>::result_type find_max_length_continus_range::operator()(argument_type0&& values, argument_type1&& low_threshold, argument_type2&& from_index, argument_type3&& to_index) const
  {
    typedef typename pythonic::assignable<typename std::remove_cv<typename std::remove_reference<argument_type2>::type>::type>::type __type0;
    typedef find_next_continus_range __type1;
    typedef typename std::remove_cv<typename std::remove_reference<argument_type0>::type>::type __type2;
    typedef typename std::remove_cv<typename std::remove_reference<argument_type1>::type>::type __type3;
    typedef typename std::remove_cv<typename std::remove_reference<argument_type3>::type>::type __type4;
    typedef typename pythonic::assignable<decltype(std::declval<__type1>()(std::declval<__type2>(), std::declval<__type3>(), std::declval<__type0>(), std::declval<__type4>()))>::type __type5;
    typedef typename pythonic::assignable<typename std::tuple_element<1,typename std::remove_reference<__type5>::type>::type>::type __type6;
    typedef typename pythonic::assignable<typename std::tuple_element<0,typename std::remove_reference<__type5>::type>::type>::type __type7;
    typedef typename pythonic::assignable<decltype(pythonic::types::make_tuple(std::declval<__type7>(), std::declval<__type6>()))>::type __type8;
    typedef typename std::tuple_element<1,typename std::remove_reference<__type8>::type>::type __type9;
    typedef typename __combined<__type6,__type9>::type __type10;
    typedef long __type11;
    typedef typename pythonic::assignable<decltype(pythonic::operator_::add(std::declval<__type10>(), std::declval<__type11>()))>::type __type12;
    typedef typename pythonic::assignable<long>::type __type13;
    typedef typename pythonic::assignable<typename std::tuple_element<0,typename std::remove_reference<__type8>::type>::type>::type __type14;
    typedef typename pythonic::assignable<typename std::tuple_element<1,typename std::remove_reference<__type8>::type>::type>::type __type15;
    typedef typename std::tuple_element<0,typename std::remove_reference<__type8>::type>::type __type16;
    typename pythonic::assignable<typename __combined<__type0,__type12>::type>::type from_index_ = from_index;
    typename pythonic::assignable<typename __combined<__type13,__type14>::type>::type start = -1L;
    typename pythonic::assignable<typename __combined<__type13,__type15>::type>::type end = -1L;
    while (true)
    {
      typename pythonic::assignable_noescape<decltype(find_next_continus_range()(values, low_threshold, from_index_, to_index))>::type __tuple0 = find_next_continus_range()(values, low_threshold, from_index_, to_index);
      typename pythonic::assignable<typename __combined<__type7,__type16>::type>::type next_start = std::get<0>(__tuple0);
      typename pythonic::assignable<typename __combined<__type6,__type9>::type>::type next_end = std::get<1>(__tuple0);
      if (pythonic::operator_::eq(next_start, -1L))
      {
        break;
      }
      else
      {
        if (pythonic::operator_::gt(pythonic::operator_::sub(next_end, next_start), pythonic::operator_::sub(end, start)))
        {
          typename pythonic::assignable_noescape<decltype(pythonic::types::make_tuple(next_start, next_end))>::type __tuple1 = pythonic::types::make_tuple(next_start, next_end);
          start = std::get<0>(__tuple1);
          end = std::get<1>(__tuple1);
        }
        from_index_ = pythonic::operator_::add(next_end, 1L);
      }
    }
    return pythonic::types::make_tuple(start, end);
  }
  template <typename argument_type0 >
  typename get_one_channel_diff_threshold::type<argument_type0>::result_type get_one_channel_diff_threshold::operator()(argument_type0&& hist) const
  {
    typedef typename pythonic::assignable<long>::type __type0;
    typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::builtins::functor::enumerate{})>::type>::type __type1;
    typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::numpy::functor::convolve{})>::type>::type __type2;
    typedef typename std::remove_cv<typename std::remove_reference<argument_type0>::type>::type __type3;
    typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::builtins::pythran::functor::static_list{})>::type>::type __type4;
    typedef double __type5;
    typedef decltype(pythonic::types::make_tuple(std::declval<__type5>(), std::declval<__type5>(), std::declval<__type5>(), std::declval<__type5>(), std::declval<__type5>(), std::declval<__type5>(), std::declval<__type5>(), std::declval<__type5>(), std::declval<__type5>())) __type6;
    typedef decltype(std::declval<__type4>()(std::declval<__type6>())) __type7;
    typedef decltype(std::declval<__type2>()(std::declval<__type3>(), std::declval<__type7>())) __type8;
    typedef pythonic::types::contiguous_slice __type9;
    typedef typename pythonic::assignable<decltype(std::declval<__type8>()[std::declval<__type9>()])>::type __type10;
    typedef decltype(std::declval<__type1>()(std::declval<__type10>())) __type11;
    typedef typename std::remove_cv<typename std::iterator_traits<typename std::remove_reference<__type11>::type::iterator>::value_type>::type __type12;
    typedef typename std::tuple_element<0,typename std::remove_reference<__type12>::type>::type __type13;
    typedef typename std::tuple_element<1,typename std::remove_reference<__type12>::type>::type __type14;
    typedef decltype(pythonic::operator_::mul(std::declval<__type13>(), std::declval<__type14>())) __type15;
    typedef decltype(pythonic::operator_::add(std::declval<__type0>(), std::declval<__type15>())) __type16;
    typedef typename __combined<__type0,__type16>::type __type17;
    typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::builtins::functor::int_{})>::type>::type __type18;
    typedef typename __combined<__type17,__type15>::type __type19;
    typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::numpy::functor::sum{})>::type>::type __type20;
    typedef decltype(std::declval<__type20>()(std::declval<__type10>())) __type21;
    typedef decltype(pythonic::operator_::div(std::declval<__type19>(), std::declval<__type21>())) __type22;
    typedef typename pythonic::assignable<decltype(std::declval<__type18>()(std::declval<__type22>()))>::type __type23;
    typedef decltype(pythonic::builtins::getattr(pythonic::types::attr::SHAPE{}, std::declval<__type10>())) __type24;
    typedef typename std::tuple_element<0,typename std::remove_reference<__type24>::type>::type __type25;
    typedef typename pythonic::assignable<decltype(pythonic::operator_::div(std::declval<__type21>(), std::declval<__type25>()))>::type __type26;
    typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::builtins::functor::range{})>::type>::type __type27;
    typedef long __type28;
    typedef decltype(std::declval<__type27>()(std::declval<__type28>())) __type29;
    typedef typename std::remove_cv<typename std::iterator_traits<typename std::remove_reference<__type29>::type::iterator>::value_type>::type __type30;
    typedef decltype(pythonic::operator_::sub(std::declval<__type30>(), std::declval<__type28>())) __type31;
    typedef decltype(pythonic::builtins::pow(std::declval<__type5>(), std::declval<__type31>())) __type32;
    typedef decltype(pythonic::operator_::mul(std::declval<__type26>(), std::declval<__type32>())) __type33;
    typedef typename find_max_length_continus_range::type<__type10, __type33, __type23, __type25>::__ptype0 __type34;
    typedef find_max_length_continus_range __type35;
    typedef typename __combined<__type23,__type34>::type __type36;
    typedef typename pythonic::assignable<decltype(std::declval<__type35>()(std::declval<__type10>(), std::declval<__type33>(), std::declval<__type36>(), std::declval<__type25>()))>::type __type37;
    typedef typename pythonic::assignable<typename std::tuple_element<0,typename std::remove_reference<__type37>::type>::type>::type __type38;
    typedef typename pythonic::assignable<typename std::tuple_element<1,typename std::remove_reference<__type37>::type>::type>::type __type39;
    typedef typename pythonic::assignable<decltype(pythonic::types::make_tuple(std::declval<__type38>(), std::declval<__type39>()))>::type __type40;
    typedef typename pythonic::assignable<typename std::tuple_element<0,typename std::remove_reference<__type40>::type>::type>::type __type41;
    typedef typename pythonic::assignable<typename std::tuple_element<1,typename std::remove_reference<__type40>::type>::type>::type __type42;
    typedef typename std::tuple_element<0,typename std::remove_reference<__type40>::type>::type __type43;
    typedef typename std::tuple_element<1,typename std::remove_reference<__type40>::type>::type __type44;
    typename pythonic::assignable<typename std::remove_cv<typename std::iterator_traits<typename std::remove_reference<__type29>::type::iterator>::value_type>::type>::type t;
    typename pythonic::assignable_noescape<decltype(pythonic::numpy::functor::convolve{}(hist, pythonic::builtins::pythran::functor::static_list{}(pythonic::types::make_tuple(0.1111111111111111, 0.1111111111111111, 0.1111111111111111, 0.1111111111111111, 0.1111111111111111, 0.1111111111111111, 0.1111111111111111, 0.1111111111111111, 0.1111111111111111)))[pythonic::types::contiguous_slice(4L,-4L)])>::type hist__ = pythonic::numpy::functor::convolve{}(hist, pythonic::builtins::pythran::functor::static_list{}(pythonic::types::make_tuple(0.1111111111111111, 0.1111111111111111, 0.1111111111111111, 0.1111111111111111, 0.1111111111111111, 0.1111111111111111, 0.1111111111111111, 0.1111111111111111, 0.1111111111111111)))[pythonic::types::contiguous_slice(4L,-4L)];
    typename pythonic::assignable<typename __combined<__type17,__type15>::type>::type count_integral = 0L;
    {
      for (auto&& __tuple0: pythonic::builtins::functor::enumerate{}(hist__))
      {
        count_integral += pythonic::operator_::mul(std::get<0>(__tuple0), std::get<1>(__tuple0));
      }
    }
    typename pythonic::assignable<typename __combined<__type23,__type34>::type>::type center = pythonic::builtins::functor::int_{}(pythonic::operator_::div(count_integral, pythonic::numpy::functor::sum{}(hist__)));
    typename pythonic::assignable_noescape<decltype(pythonic::operator_::div(pythonic::numpy::functor::sum{}(hist__), std::get<0>(pythonic::builtins::getattr(pythonic::types::attr::SHAPE{}, hist__))))>::type average_value = pythonic::operator_::div(pythonic::numpy::functor::sum{}(hist__), std::get<0>(pythonic::builtins::getattr(pythonic::types::attr::SHAPE{}, hist__)));
    typename pythonic::assignable<typename __combined<__type0,__type41>::type>::type start = -1L;
    typename pythonic::assignable<typename __combined<__type0,__type42>::type>::type end = -1L;
    {
      long  __target1566531219784 = 11L;
      for (long  t=0L; t < __target1566531219784; t += 1L)
      {
        typename pythonic::assignable_noescape<decltype(find_max_length_continus_range()(hist__, pythonic::operator_::mul(average_value, pythonic::builtins::pow(1.5, pythonic::operator_::sub(t, 5L))), center, std::get<0>(pythonic::builtins::getattr(pythonic::types::attr::SHAPE{}, hist__))))>::type __tuple1 = find_max_length_continus_range()(hist__, pythonic::operator_::mul(average_value, pythonic::builtins::pow(1.5, pythonic::operator_::sub(t, 5L))), center, std::get<0>(pythonic::builtins::getattr(pythonic::types::attr::SHAPE{}, hist__)));
        typename pythonic::assignable<typename __combined<__type38,__type43>::type>::type next_start = std::get<0>(__tuple1);
        typename pythonic::assignable<typename __combined<__type39,__type44>::type>::type next_end = std::get<1>(__tuple1);
        {
          typename pythonic::assignable<typename pythonic::assignable<decltype(pythonic::types::make_tuple(std::declval<__type38>(), std::declval<__type39>()))>::type>::type __tuple2;
          if (pythonic::builtins::pythran::and_([&] () { return pythonic::operator_::ge(pythonic::operator_::sub(next_end, next_start), 16L); }, [&] () { return pythonic::operator_::lt(pythonic::operator_::sub(next_end, next_start), pythonic::operator_::mul(pythonic::operator_::sub(end, start), 1.125)); }))
          {
            break;
          }
          else
          {
            __tuple2 = pythonic::types::make_tuple(next_start, next_end);
            start = std::get<0>(__tuple2);
            end = std::get<1>(__tuple2);
          }
        }
      }
    }
    return pythonic::types::make_tuple(hist__, pythonic::operator_::add(pythonic::operator_::mul(start, 0.5), pythonic::operator_::mul(end, 0.5)), pythonic::operator_::div(pythonic::operator_::sub(end, start), 2L));
  }
  template <typename argument_type0 , typename argument_type1 >
  typename analysis_channel_diff_threshold::type<argument_type0, argument_type1>::result_type analysis_channel_diff_threshold::operator()(argument_type0&& image, argument_type1 is_blue_screen) const
  {
    typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::numpy::functor::zeros{})>::type>::type __type0;
    typedef long __type1;
    typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::numpy::functor::int32{})>::type>::type __type2;
    typedef typename pythonic::assignable<decltype(std::declval<__type0>()(std::declval<__type1>(), std::declval<__type2>()))>::type __type3;
    typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::builtins::functor::int_{})>::type>::type __type4;
    typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::numpy::ndarray::functor::astype{})>::type>::type __type5;
    typedef typename std::remove_cv<typename std::remove_reference<argument_type0>::type>::type __type6;
    typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::builtins::functor::range{})>::type>::type __type7;
    typedef decltype(pythonic::builtins::getattr(pythonic::types::attr::SHAPE{}, std::declval<__type6>())) __type8;
    typedef pythonic::types::fast_contiguous_slice __type9;
    typedef typename pythonic::assignable<decltype(std::declval<__type8>()[std::declval<__type9>()])>::type __type10;
    typedef typename std::tuple_element<0,typename std::remove_reference<__type10>::type>::type __type11;
    typedef typename pythonic::lazy<__type11>::type __type12;
    typedef decltype(std::declval<__type7>()(std::declval<__type1>(), std::declval<__type12>(), std::declval<__type1>())) __type13;
    typedef typename std::remove_cv<typename std::iterator_traits<typename std::remove_reference<__type13>::type::iterator>::value_type>::type __type14;
    typedef typename std::tuple_element<1,typename std::remove_reference<__type10>::type>::type __type15;
    typedef typename pythonic::lazy<__type15>::type __type16;
    typedef decltype(std::declval<__type7>()(std::declval<__type1>(), std::declval<__type16>(), std::declval<__type1>())) __type17;
    typedef typename std::remove_cv<typename std::iterator_traits<typename std::remove_reference<__type17>::type::iterator>::value_type>::type __type18;
    typedef decltype(pythonic::types::make_tuple(std::declval<__type14>(), std::declval<__type18>())) __type19;
    typedef decltype(std::declval<__type6>()[std::declval<__type19>()]) __type20;
    typedef typename pythonic::assignable<decltype(std::declval<__type5>()(std::declval<__type20>(), std::declval<__type2>()))>::type __type21;
    typedef typename pythonic::assignable<typename std::tuple_element<1,typename std::remove_reference<__type21>::type>::type>::type __type22;
    typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::builtins::functor::max{})>::type>::type __type23;
    typedef typename pythonic::assignable<typename std::tuple_element<0,typename std::remove_reference<__type21>::type>::type>::type __type24;
    typedef typename pythonic::assignable<typename std::tuple_element<2,typename std::remove_reference<__type21>::type>::type>::type __type25;
    typedef typename __combined<__type24,__type25>::type __type26;
    typedef decltype(std::declval<__type23>()(std::declval<__type26>(), std::declval<__type25>())) __type27;
    typedef decltype(pythonic::operator_::sub(std::declval<__type22>(), std::declval<__type27>())) __type28;
    typedef decltype(pythonic::operator_::mul(std::declval<__type1>(), std::declval<__type28>())) __type29;
    typedef decltype(std::declval<__type4>()(std::declval<__type29>())) __type30;
    typedef decltype(pythonic::operator_::sub(std::declval<__type1>(), std::declval<__type30>())) __type31;
    typedef decltype(pythonic::operator_::sub(std::declval<__type31>(), std::declval<__type1>())) __type32;
    typedef decltype(pythonic::operator_::mul(std::declval<__type29>(), std::declval<__type1>())) __type33;
    typedef decltype(pythonic::operator_::sub(std::declval<__type28>(), std::declval<__type22>())) __type34;
    typedef decltype(pythonic::operator_::sub(std::declval<__type34>(), std::declval<__type27>())) __type35;
    typedef decltype(pythonic::operator_::mul(std::declval<__type33>(), std::declval<__type35>())) __type36;
    typedef decltype(std::declval<__type4>()(std::declval<__type36>())) __type37;
    typedef decltype(pythonic::operator_::sub(std::declval<__type32>(), std::declval<__type37>())) __type38;
    typedef indexable<__type38> __type39;
    typedef typename __combined<__type3,__type39>::type __type40;
    typedef typename __combined<__type26,__type22>::type __type41;
    typedef decltype(std::declval<__type23>()(std::declval<__type41>(), std::declval<__type22>())) __type42;
    typedef decltype(pythonic::operator_::sub(std::declval<__type25>(), std::declval<__type42>())) __type43;
    typedef decltype(pythonic::operator_::mul(std::declval<__type1>(), std::declval<__type43>())) __type44;
    typedef decltype(std::declval<__type4>()(std::declval<__type44>())) __type45;
    typedef decltype(pythonic::operator_::sub(std::declval<__type1>(), std::declval<__type45>())) __type46;
    typedef decltype(pythonic::operator_::sub(std::declval<__type46>(), std::declval<__type1>())) __type47;
    typedef decltype(pythonic::operator_::mul(std::declval<__type44>(), std::declval<__type1>())) __type48;
    typedef decltype(pythonic::operator_::sub(std::declval<__type43>(), std::declval<__type25>())) __type49;
    typedef decltype(pythonic::operator_::sub(std::declval<__type49>(), std::declval<__type42>())) __type50;
    typedef decltype(pythonic::operator_::mul(std::declval<__type48>(), std::declval<__type50>())) __type51;
    typedef decltype(std::declval<__type4>()(std::declval<__type51>())) __type52;
    typedef decltype(pythonic::operator_::sub(std::declval<__type47>(), std::declval<__type52>())) __type53;
    typedef indexable<__type53> __type54;
    typedef typename __combined<__type40,__type54>::type __type55;
    typedef typename __combined<__type55,__type39>::type __type56;
    typedef container<typename std::remove_reference<__type1>::type> __type57;
    typedef typename __combined<__type56,__type57>::type __type58;
    typedef typename std::remove_cv<typename std::remove_reference<decltype(pythonic::numpy::functor::array{})>::type>::type __type59;
    typedef decltype(pythonic::types::make_tuple(std::declval<__type1>(), std::declval<__type1>(), std::declval<__type1>())) __type60;
    typedef typename pythonic::assignable<decltype(std::declval<__type59>()(std::declval<__type60>(), std::declval<__type2>()))>::type __type61;
    typedef decltype(pythonic::types::make_tuple(std::declval<__type26>(), std::declval<__type22>(), std::declval<__type25>())) __type62;
    typedef decltype(pythonic::operator_::add(std::declval<__type61>(), std::declval<__type62>())) __type63;
    typedef typename __combined<__type61,__type63>::type __type64;
    typedef decltype(pythonic::types::make_tuple(std::declval<__type41>(), std::declval<__type22>(), std::declval<__type25>())) __type65;
    typedef decltype(pythonic::operator_::add(std::declval<__type64>(), std::declval<__type65>())) __type66;
    typedef typename __combined<__type64,__type66>::type __type67;
    typedef typename __combined<__type67,__type62>::type __type68;
    typedef typename pythonic::assignable<long>::type __type69;
    typedef decltype(pythonic::operator_::add(std::declval<__type69>(), std::declval<__type1>())) __type70;
    typedef typename __combined<__type69,__type70>::type __type71;
    typedef decltype(pythonic::operator_::add(std::declval<__type71>(), std::declval<__type1>())) __type72;
    typedef typename __combined<__type71,__type72>::type __type73;
    typedef get_one_channel_diff_threshold __type74;
    typedef typename __combined<__type55,__type57>::type __type75;
    typedef typename pythonic::assignable<decltype(std::declval<__type74>()(std::declval<__type75>()))>::type __type76;
    typedef typename pythonic::assignable<typename std::tuple_element<1,typename std::remove_reference<__type76>::type>::type>::type __type77;
    typedef decltype(pythonic::operator_::div(std::declval<__type77>(), std::declval<__type1>())) __type78;
    typedef typename __combined<__type77,__type78>::type __type79;
    typedef typename pythonic::assignable<typename std::tuple_element<2,typename std::remove_reference<__type76>::type>::type>::type __type80;
    typedef decltype(pythonic::operator_::div(std::declval<__type80>(), std::declval<__type1>())) __type81;
    typedef typename __combined<__type80,__type81>::type __type82;
    typename pythonic::assignable<typename std::remove_cv<typename std::iterator_traits<typename std::remove_reference<__type13>::type::iterator>::value_type>::type>::type y;
    typename pythonic::assignable<typename std::remove_cv<typename std::iterator_traits<typename std::remove_reference<__type17>::type::iterator>::value_type>::type>::type i;
    typename pythonic::assignable<typename __combined<__type58,__type54>::type>::type hist = pythonic::numpy::functor::zeros{}(256L, pythonic::numpy::functor::int32{});
    typename pythonic::assignable<typename __combined<__type68,__type65>::type>::type sum = pythonic::numpy::functor::array{}(pythonic::types::make_tuple(0L, 0L, 0L), pythonic::numpy::functor::int32{});
    typename pythonic::assignable<typename __combined<__type73,__type1>::type>::type total_pixels = 0L;
    typename pythonic::assignable_noescape<decltype(pythonic::builtins::getattr(pythonic::types::attr::SHAPE{}, image)[pythonic::types::fast_contiguous_slice(0L,2L)])>::type __tuple0 = pythonic::builtins::getattr(pythonic::types::attr::SHAPE{}, image)[pythonic::types::fast_contiguous_slice(0L,2L)];
    typename pythonic::lazy<decltype(std::get<0>(__tuple0))>::type h = std::get<0>(__tuple0);
    typename pythonic::lazy<decltype(std::get<1>(__tuple0))>::type w = std::get<1>(__tuple0);
    {
      long  __target1566531677064 = h;
      for (long  y=0L; y < __target1566531677064; y += 10L)
      {
        {
          long  __target1566531678024 = w;
          for (long  i=0L; i < __target1566531678024; i += 10L)
          {
            typename pythonic::assignable_noescape<decltype(pythonic::numpy::ndarray::functor::astype{}(image.fast(pythonic::types::make_tuple(y, i)), pythonic::numpy::functor::int32{}))>::type __tuple1 = pythonic::numpy::ndarray::functor::astype{}(image.fast(pythonic::types::make_tuple(y, i)), pythonic::numpy::functor::int32{});
            typename pythonic::assignable<typename __combined<__type41,__type26>::type>::type r = std::get<0>(__tuple1);
            typename pythonic::assignable_noescape<decltype(std::get<1>(__tuple1))>::type g = std::get<1>(__tuple1);
            typename pythonic::assignable_noescape<decltype(std::get<2>(__tuple1))>::type b = std::get<2>(__tuple1);
            if (pythonic::operator_::not_(is_blue_screen))
            {
              if (analysis_channel_diff_threshold_compare0()(pythonic::operator_::sub(256L, pythonic::builtins::functor::int_{}(pythonic::operator_::mul(2L, pythonic::operator_::sub(g, pythonic::builtins::functor::max{}(r, b)))))))
              {
                hist[pythonic::operator_::sub(256L, pythonic::builtins::functor::int_{}(pythonic::operator_::mul(2L, pythonic::operator_::sub(g, pythonic::builtins::functor::max{}(r, b)))))] += 1L;
                sum += pythonic::types::make_tuple(r, g, b);
                total_pixels += 1L;
              }
            }
            else
            {
              if (analysis_channel_diff_threshold_compare1()(pythonic::operator_::sub(256L, pythonic::builtins::functor::int_{}(pythonic::operator_::mul(2L, pythonic::operator_::sub(b, pythonic::builtins::functor::max{}(r, g)))))))
              {
                hist[pythonic::operator_::sub(256L, pythonic::builtins::functor::int_{}(pythonic::operator_::mul(2L, pythonic::operator_::sub(b, pythonic::builtins::functor::max{}(r, g)))))] += 1L;
                sum += pythonic::types::make_tuple(r, g, b);
                total_pixels += 1L;
              }
            }
          }
        }
      }
    }
    typename pythonic::assignable_noescape<decltype(get_one_channel_diff_threshold()(hist))>::type __tuple2 = get_one_channel_diff_threshold()(hist);
    typename pythonic::assignable<typename __combined<__type79,__type1>::type>::type threshold = std::get<1>(__tuple2);
    typename pythonic::assignable<typename __combined<__type82,__type1>::type>::type smooth_radius = std::get<2>(__tuple2);
    pythonic::operator_::idiv(threshold, 255L);
    pythonic::operator_::idiv(smooth_radius, 255L);
    pythonic::builtins::functor::print{}(pythonic::types::str("色差分布:"), pythonic::numpy::ndarray::functor::astype{}(std::get<0>(__tuple2), pythonic::numpy::functor::int32{}));
    pythonic::builtins::functor::print{}(pythonic::types::str("色差阈值:"), threshold);
    return pythonic::types::make_tuple(threshold, pythonic::operator_::div(sum, total_pixels));
  }
}
#include <pythonic/python/exception_handler.hpp>
#ifdef ENABLE_PYTHON_MODULE

static PyMethodDef Methods[] = {

    {NULL, NULL, 0, NULL}
};


#if PY_MAJOR_VERSION >= 3
  static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "analysis_channel_diff_threshold",            /* m_name */
    "",         /* m_doc */
    -1,                  /* m_size */
    Methods,             /* m_methods */
    NULL,                /* m_reload */
    NULL,                /* m_traverse */
    NULL,                /* m_clear */
    NULL,                /* m_free */
  };
#define PYTHRAN_RETURN return theModule
#define PYTHRAN_MODULE_INIT(s) PyInit_##s
#else
#define PYTHRAN_RETURN return
#define PYTHRAN_MODULE_INIT(s) init##s
#endif
PyMODINIT_FUNC
PYTHRAN_MODULE_INIT(analysis_channel_diff_threshold)(void)
#ifndef _WIN32
__attribute__ ((visibility("default")))
__attribute__ ((externally_visible))
#endif
;
PyMODINIT_FUNC
PYTHRAN_MODULE_INIT(analysis_channel_diff_threshold)(void) {
    import_array()
    #if PY_MAJOR_VERSION >= 3
    PyObject* theModule = PyModule_Create(&moduledef);
    #else
    PyObject* theModule = Py_InitModule3("analysis_channel_diff_threshold",
                                         Methods,
                                         ""
    );
    #endif
    if(! theModule)
        PYTHRAN_RETURN;
    PyObject * theDoc = Py_BuildValue("(sss)",
                                      "0.9.9",
                                      "2021-05-07 14:15:47.192162",
                                      "b03c04689b53a6d4b7da0ba62f4fa5d98159f7a7f0c8449a6d099e6c391f80d9");
    if(! theDoc)
        PYTHRAN_RETURN;
    PyModule_AddObject(theModule,
                       "__pythran__",
                       theDoc);


    PYTHRAN_RETURN;
}

#endif