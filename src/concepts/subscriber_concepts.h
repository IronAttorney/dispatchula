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

template<class ... TYPE_LIST>
concept _are_unique_types_ = (_are_unique_types_v_<TYPE_LIST...>);


/** Request Return Types Traits */

template<class T>
constexpr bool _is_non_value_request_return_type_v_ = ( std::is_void<T>::value ||
                                                        std::is_pointer<T>::value ||
                                                        std::same_as<T, std::shared_ptr<typename T::element_type>> ||
                                                        std::same_as<T, std::unique_ptr<typename T::element_type>> ||
                                                        std::same_as<T, std::optional<typename T::element_type>> );

/** Type Concepts */

template <class T>
concept _is_raw_pointer_ = std::is_pointer<T>::value;

template <class T>
concept _is_raw_reference_ = std::is_reference<T>::value;

template <class T>
concept _is_shared_pointer_ = std::same_as<T, std::shared_ptr<typename T::element_type>>;

template <class T>
concept _is_unique_pointer_ = std::same_as<T, std::unique_ptr<typename T::element_type>>;

template <class T>
concept _is_optional_ = std::same_as<T, std::optional<typename T::value_type>>;

template <class T>
concept _is_smart_pointer_ = (_is_shared_pointer_<T> || _is_unique_pointer_<T>);

template <class T>
concept _is_smart_wrapper_ = (_is_smart_pointer_<T> || _is_optional_<T>);

template <class T>
concept _is_non_unique_pointer_ = (_is_raw_pointer_<T> || _is_shared_pointer_<T>);

template <class T>
concept _is_any_pointer_ = (_is_raw_pointer_<T> || _is_smart_pointer_<T>);

template <class T>
concept _is_value_ = (!_is_smart_wrapper_<T> && !_is_raw_pointer_<T> && !_is_raw_reference_<T>);

template<class T>
concept _is_request_return_type_ = ( ( _is_smart_wrapper_<T> || _is_raw_pointer_<T> ) && !_is_raw_reference_<T> );


} // dispatch