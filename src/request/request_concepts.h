//
// Created by PeterBurcello on 05/08/2025.
//

#pragma once


#include "shared/dispatchula_concepts.h"


namespace dispatch {


/** Type Concepts */

template <class T>
concept _is_expected_ = requires {
    typename T::value_type;
    typename T::error_type;
    typename T::unexpected_type;
    requires std::same_as<T, std::expected<typename T::value_type, typename T::error_type>>;
};

//template <class T>
//struct _is_expected_helper_ : std::false_type {};
//
//template <class T, class E>
//struct _is_expected_helper_<std::expected<T, E>> : std::true_type {};
//
//template <class T>
//concept _is_expected_ = _is_expected_helper_<T>::value;

//template<class T>
//concept _is_expected_ = requires {
//    []<class U, class E>(const std::expected<U, E>&){}(std::declval<T>());
//};

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
concept _is_any_pointer_ = (_is_raw_pointer_<T> || _is_smart_pointer_<T>);


/** Return type concepts **/

template <class T>
concept _is_value_request_return_type_ = ( !_is_expected_<T> && !_is_any_pointer_<T> && !_is_optional_<T> && !std::is_void_v<T> && !_is_raw_reference_<T> );

template<class T>
concept _is_non_value_request_return_type_ = ( _is_expected_<T> || _is_any_pointer_<T> || _is_optional_<T> || std::is_void<T>::value );


/** Request return type concepts **/

template <class REQUEST_TYPE>
concept _has_expected_return_type_without_string_error_ = (_is_expected_<typename REQUEST_TYPE::_RETURN_TYPE_> && !std::is_same_v<typename REQUEST_TYPE::_RETURN_TYPE_::error_type, std::string>);

template <class REQUEST_TYPE>
concept _has_optional_return_type_ = _is_optional_<typename REQUEST_TYPE::_RETURN_TYPE_>;

template <class REQUEST_TYPE>
concept _has_pointer_return_type_ = _is_any_pointer_<typename REQUEST_TYPE::_RETURN_TYPE_>;

template <class REQUEST_TYPE>
concept _has_value_return_type_ = _is_value_request_return_type_<typename REQUEST_TYPE::_RETURN_TYPE_>;

template <class REQUEST_TYPE>
concept _has_void_return_type_ = std::is_void<typename REQUEST_TYPE::_RETURN_TYPE_>::value;


} // dispatch