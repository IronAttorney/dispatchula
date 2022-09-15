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


#include "request_subscriber.h"

#include <algorithm>
#include <concepts>
#include <map>
#include <optional>
#include <typeindex>
#include <vector>


namespace dispatch {


class RequestDispatcher {

public:

    bool subscribe(_RequestSubscriberBase_* subscriber)
    {
        const auto& type_id_list = subscriber->_d_get_request_type_id_list();

        bool subscribe_success = true;

        for (auto type_id : type_id_list) {
            subscribe_success &= _try_subscribe_to_type_id(subscriber, type_id);
        }

        return subscribe_success;
    }

    void unsubscribe(_RequestSubscriberBase_* subscriber)
    {
        const auto& type_id_list = subscriber->_d_get_request_type_id_list();

        for (auto type_id : type_id_list) {
            _unsubscribe_from_type_id(subscriber, type_id);
        }
    }

    template<class REQUEST_TYPE, class SUBSCRIBER_TYPE> requires _convertable_to_subscriber_of_<SUBSCRIBER_TYPE, REQUEST_TYPE>
    bool subscribe(SUBSCRIBER_TYPE* subscriber)
    {
        return _try_subscribe_to_type_id(subscriber, typeid(REQUEST_TYPE));
    }

    template<class REQUEST_TYPE, class SUBSCRIBER_TYPE> requires  _convertable_to_subscriber_of_<SUBSCRIBER_TYPE, REQUEST_TYPE>
    void unsubscribe(SUBSCRIBER_TYPE* subscriber)
    {
        return _unsubscribe_from_type_id(subscriber, typeid(REQUEST_TYPE));
    }

    template<class ... REQUEST_TYPE_LIST, class SUBSCRIBER_TYPE> requires _convertable_to_subscribers_of_<SUBSCRIBER_TYPE, REQUEST_TYPE_LIST...>
    bool subscribe(SUBSCRIBER_TYPE* subscriber)
    {
        bool subscribe_success = true;

        const std::vector<std::type_index> _d_request_type_id_list = { typeid(REQUEST_TYPE_LIST)... };

        for (auto type_id : _d_request_type_id_list) {
            subscribe_success &= _try_subscribe_to_type_id(subscriber, type_id);
        }

        return subscribe_success;
    }

    template<class ... REQUEST_TYPE_LIST, class SUBSCRIBER_TYPE> requires _convertable_to_subscribers_of_<SUBSCRIBER_TYPE, REQUEST_TYPE_LIST...>
    void unsubscribe(SUBSCRIBER_TYPE* subscriber)
    {
        const std::vector<std::type_index> _d_request_type_id_list = { typeid(REQUEST_TYPE_LIST)... };

        for (auto type_id : _d_request_type_id_list) {
            _try_subscribe_to_type_id(subscriber, type_id);
        }
    }

    template<class REQUEST_TYPE> requires std::is_void<typename REQUEST_TYPE::_RETURN_TYPE_>::value
    void dispatch(const REQUEST_TYPE& request)
    {
        const auto request_subscriber_iter = _subscriber_map.find(typeid(request));

        if (request_subscriber_iter == _subscriber_map.end()) {
            return;
        }

        const auto& subscriber = request_subscriber_iter->second;

        auto sub_subscriber = dynamic_cast<_SingleRequestSubscriber_<REQUEST_TYPE>*>(subscriber);
        sub_subscriber->handle_request(request);
    }

    template<class REQUEST_TYPE>
    auto dispatch(const REQUEST_TYPE& request) -> std::optional<typename REQUEST_TYPE::_RETURN_TYPE_>
    {
        const auto request_subscriber_iter = _subscriber_map.find(typeid(request));

        if (request_subscriber_iter == _subscriber_map.end()) {
            return std::nullopt;
        }

        const auto& subscriber = request_subscriber_iter->second;

        auto sub_subscriber = dynamic_cast<_SingleRequestSubscriber_<REQUEST_TYPE>*>(subscriber);
        return sub_subscriber->handle_request(request);
    }

private:

    bool _try_subscribe_to_type_id(_RequestSubscriberBase_* subscriber, std::type_index type_id)
    {
        auto [_, insert_success] = _subscriber_map.try_emplace(type_id, subscriber);
        return insert_success;
    }

    void _unsubscribe_from_type_id(_RequestSubscriberBase_* subscriber, std::type_index type_id)
    {
        _subscriber_map.erase(type_id);
    }

    std::map<std::type_index, _RequestSubscriberBase_*> _subscriber_map;
};


} // namespace dispatch
