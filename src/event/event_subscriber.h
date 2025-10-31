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


#include "shared/dispatchula_concepts.h"

#include <typeindex>
#include <vector>


namespace dispatch {


/**
 * All classes wishing to subscribe to events need to derive from this class.
 *
 * Clients should use this class only to derive from when subscribing to events,
 *
 * Refer to `examples/event_demo.cpp` for example usage.
 *
 * @tparam EVENT_TYPE_LIST - is a list of all event types the derived subscribing class
 *                           will ever wish to subscribe to.
 */
template<class ... EVENT_TYPE_LIST> requires _are_unique_types_<EVENT_TYPE_LIST...>
class EventSubscriber;

/**
 * A base class used only by the EventDispatcher to allow subscribing and unsubscribing
 * to all events that a subscribing class is subscribed to.
 *
 * Clients should not use this class.
 */
class _EventSubscriberBase_;

/**
 * An intermediate base class used only by EventSubscriber to help with generating the
 * event handler functions through template meta-programming.
 *
 * Clients should not use this class.
 *
 * @tparam EVENT_TYPE - is a single event type
 */
template<class EVENT_TYPE>
class _SingleEventSubscriber_;


class EventDispatcher;


class _EventSubscriberBase_
{
    friend EventDispatcher;

    virtual const std::vector<std::type_index>& _get_event_type_id_list() = 0;
};


template<class EVENT_TYPE>
class _SingleEventSubscriber_ : virtual public _EventSubscriberBase_
{
    friend EventDispatcher;

    virtual void handle_event(const EVENT_TYPE& dispatch) = 0;
};


template<class ... EVENT_TYPE_LIST> requires _are_unique_types_<EVENT_TYPE_LIST...>
class EventSubscriber : public _SingleEventSubscriber_<EVENT_TYPE_LIST>...
{
    const std::vector<std::type_index>& _get_event_type_id_list() override {
        return _event_type_id_list;
    }

    static inline const std::vector<std::type_index> _event_type_id_list = { typeid(EVENT_TYPE_LIST)... };
};


template<class SINGLE_EVENT_SUBSCRIBER_TYPE, class EVENT_TYPE>
concept _is_subscriber_for_event_type_ = std::is_base_of_v<_SingleEventSubscriber_<EVENT_TYPE>, SINGLE_EVENT_SUBSCRIBER_TYPE>;


} // namespace dispatch
