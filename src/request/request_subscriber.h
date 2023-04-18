/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Peter Burgess
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE
 */

#pragma once


#include "concepts/subscriber_concepts.h"
#include "request.h"


#include <concepts>
#include <optional>
#include <typeindex>
#include <vector>


namespace dispatch {


class RequestDispatcher;


class _RequestSubscriberBase_ {
    friend RequestDispatcher;

    virtual const std::vector<std::type_index> &_d_get_request_type_id_list() = 0;
};


template<class REQUEST_TYPE>
class _SingleRequestSubscriber_ : virtual public _RequestSubscriberBase_
{
    friend RequestDispatcher;

    using RETURN_TYPE = typename std::conditional<std::is_void<typename REQUEST_TYPE::_RETURN_TYPE_>::value,
                                                  void, std::optional<typename REQUEST_TYPE::_RETURN_TYPE_>>::type;

    virtual RETURN_TYPE handle_request(const REQUEST_TYPE& dispatch) = 0;
};


template<_are_unique_types_ ... REQUEST_TYPE_LIST>
class RequestSubscriber : public _SingleRequestSubscriber_<REQUEST_TYPE_LIST>...
{
    friend RequestDispatcher;

    const std::vector<std::type_index>& _d_get_request_type_id_list() override {
        return _d_request_type_id_list;
    }

    static inline const std::vector<std::type_index> _d_request_type_id_list = {typeid(REQUEST_TYPE_LIST)... };
};


template <class SUBSCRIBER_TYPE, class REQUEST_TYPE>
concept _convertable_to_subscriber_of_ = std::convertible_to<SUBSCRIBER_TYPE*, _SingleRequestSubscriber_<REQUEST_TYPE>*>;


template <class SUBSCRIBER_TYPE, class ... REQUEST_TYPE_LIST>
concept _convertable_to_subscribers_of_ = (std::convertible_to<SUBSCRIBER_TYPE*, _SingleRequestSubscriber_<REQUEST_TYPE_LIST>*> && ...) && sizeof...(REQUEST_TYPE_LIST) > 1;


} // namespace dispatch
