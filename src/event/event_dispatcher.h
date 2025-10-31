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


#include "event_subscriber.h"

#include <algorithm>
#include <map>
#include <typeindex>
#include <vector>


namespace dispatch {


class EventDispatcher {

public:

    void subscribe(_EventSubscriberBase_* subscriber);

    template<class EVENT_TYPE, class SINGLE_EVENT_SUBSCRIBER_TYPE> requires _is_subscriber_for_event_type_<SINGLE_EVENT_SUBSCRIBER_TYPE, EVENT_TYPE>
    void subscribe(SINGLE_EVENT_SUBSCRIBER_TYPE* subscriber);

    void unsubscribe(_EventSubscriberBase_* subscriber);

    template<class EVENT_TYPE, class SINGLE_EVENT_SUBSCRIBER_TYPE> requires _is_subscriber_for_event_type_<SINGLE_EVENT_SUBSCRIBER_TYPE, EVENT_TYPE>
    void unsubscribe(SINGLE_EVENT_SUBSCRIBER_TYPE* subscriber);

    template<class EVENT_TYPE>
    void dispatch(const EVENT_TYPE& event) const;

private:

    void _subscribe_to_type_id(_EventSubscriberBase_* subscriber, std::type_index type_id);
    void _unsubscribe_from_type_id(_EventSubscriberBase_* subscriber, std::type_index type_id);

    std::map<std::type_index, std::vector<_EventSubscriberBase_*>> _subscriber_map {};
};


void EventDispatcher::subscribe(_EventSubscriberBase_* subscriber)
{
    const std::vector<std::type_index>& type_id_list = subscriber->_get_event_type_id_list();

    for (auto type_id : type_id_list) {
        _subscribe_to_type_id(subscriber, type_id);
    }
}

template<class EVENT_TYPE, class SINGLE_EVENT_SUBSCRIBER_TYPE> requires _is_subscriber_for_event_type_<SINGLE_EVENT_SUBSCRIBER_TYPE, EVENT_TYPE>
void EventDispatcher::subscribe(SINGLE_EVENT_SUBSCRIBER_TYPE* subscriber)
{
    _subscribe_to_type_id(subscriber, typeid(EVENT_TYPE));
}

void EventDispatcher::unsubscribe(_EventSubscriberBase_* subscriber)
{
    const std::vector<std::type_index>& type_id_list = subscriber->_get_event_type_id_list();

    for (auto type_id : type_id_list) {
        _unsubscribe_from_type_id(subscriber, type_id);
    }
}

template<class EVENT_TYPE, class SINGLE_EVENT_SUBSCRIBER_TYPE> requires _is_subscriber_for_event_type_<SINGLE_EVENT_SUBSCRIBER_TYPE, EVENT_TYPE>
void EventDispatcher::unsubscribe(SINGLE_EVENT_SUBSCRIBER_TYPE* subscriber)
{
    _unsubscribe_from_type_id(subscriber, typeid(EVENT_TYPE));
}

template<class EVENT_TYPE>
void EventDispatcher::dispatch(const EVENT_TYPE& event) const
{
    const auto event_subscribers_iter = _subscriber_map.find(typeid(event));

    if (event_subscribers_iter == _subscriber_map.end()) {
        return;
    }

    const auto& subscriber_list = event_subscribers_iter->second;

    for (auto& subscriber : subscriber_list) {
        auto sub_subscriber = dynamic_cast<_SingleEventSubscriber_<EVENT_TYPE>*>(subscriber);
        sub_subscriber->handle_event(event);
    }
}

void EventDispatcher::_subscribe_to_type_id(_EventSubscriberBase_* subscriber, std::type_index type_id)
{
    auto event_subscribers_iter = _subscriber_map.find(type_id);

    if (event_subscribers_iter == _subscriber_map.end()) {
        _subscriber_map.insert({type_id, {subscriber}});
    }

    else {
        auto& subscriber_list = event_subscribers_iter->second;
        subscriber_list.push_back(subscriber);
    }
}

void EventDispatcher::_unsubscribe_from_type_id(_EventSubscriberBase_* subscriber, std::type_index type_id)
{
    auto event_subscribers_iter = _subscriber_map.find(type_id);

    if (event_subscribers_iter == _subscriber_map.end()) {
        return;
    }

    else {
        auto& subscriber_list = event_subscribers_iter->second;
        subscriber_list.erase(std::remove(subscriber_list.begin(), subscriber_list.end(), subscriber), subscriber_list.end());
    }
}


} // namespace dispatch

