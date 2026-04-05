//
// Created by PeterBurcello on 18/04/2023.
//

#pragma once


#include <expected>
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


} // dispatch