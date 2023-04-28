//
// Created by PeterBurcello on 18/04/2023.
//

#pragma once


#include <memory>
#include <optional>
#include <type_traits>


namespace dispatch {


/** Unique Types **/

template<class FIRST_TYPE_IN_LIST, class... REST_OF_TYPE_LIST>
constexpr bool _are_unique_types_v_ = (!std::is_same_v<FIRST_TYPE_IN_LIST, REST_OF_TYPE_LIST> && ...) && _are_unique_types_v_<REST_OF_TYPE_LIST...>;

template<class T>
constexpr bool _are_unique_types_v_<T> = true;

template<class TYPE_LIST>
concept _are_unique_types_ = (_are_unique_types_v_<TYPE_LIST>);


/** Request Return Types Traits */

template<class T>
struct _is_optional_t_ : std::false_type {};
template<class T>
struct _is_optional_t_<std::optional<T>> : std::true_type {};

template<class T>
struct _is_smart_pointer_t_ : std::false_type {};
template<class T>
struct _is_smart_pointer_t_<std::shared_ptr<T>> : std::true_type {};
template<class T>
struct _is_smart_pointer_t_<std::unique_ptr<T>> : std::true_type {};

template<class T>
struct _is_smart_wrapper_t_ : std::false_type {};
template<class T>
struct _is_smart_wrapper_t_<std::optional<T>> : std::true_type {};
template<class T>
struct _is_smart_wrapper_t_<std::shared_ptr<T>> : std::true_type {};
template<class T>
struct _is_smart_wrapper_t_<std::unique_ptr<T>> : std::true_type {};

template<class T>
constexpr bool _is_unwrapped_request_return_type_v_ = (std::is_void<T>::value || std::is_pointer<T>::value || _is_smart_wrapper_t_<T>::value);


/** Type Concepts */

template<class T>
concept _is_optional_ = _is_optional_t_<T>::value;

template<class T>
concept _is_raw_or_smart_pointer_ = std::is_pointer<T>::value || _is_smart_pointer_t_<T>::value;


} // dispatch