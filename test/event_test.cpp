/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2025 PeterBurgess
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


#include "event/event_dispatcher.h"
#include "event/event_subscriber.h"

#include "catch2/catch_test_macros.hpp"


struct EventWithData {
    int data;
};
struct SomethingHappenedEvent {};


class SingleSubscriber : public dispatch::EventSubscriber<SomethingHappenedEvent>
{

public:

    void handle_event(const SomethingHappenedEvent& event) override
    {
        event_handled = true;
    }

    bool event_handled = false;
};

class MultiSubscriber : public dispatch::EventSubscriber<EventWithData, SomethingHappenedEvent>
{

public:

    void handle_event(const EventWithData& event) override
    {
        event_with_data_handled = true;
        event_data = event.data;
    }

    void handle_event(const SomethingHappenedEvent& event) override
    {
        something_happened_event_handled = true;
    }

    bool event_with_data_handled = false;
    std::optional<int> event_data = std::nullopt;

    bool something_happened_event_handled = false;
};


/// Mock subscriber initialisation tests

TEST_CASE("Test mock SingleSubscriber public variable initialisation")
{
    using namespace dispatch;

    EventDispatcher event_dispatcher;
    SingleSubscriber subscriber;

    REQUIRE(subscriber.event_handled == false);
}

TEST_CASE("Test mock MultiSubscriber public variable initialisation")
{
    using namespace dispatch;

    EventDispatcher event_dispatcher;
    MultiSubscriber subscriber;

    REQUIRE(subscriber.event_with_data_handled == false);
    REQUIRE(subscriber.event_data.has_value() == false);
    REQUIRE(subscriber.something_happened_event_handled == false);
}


/// Single subscriber tests

TEST_CASE("Test event not handled before SingleSubscriber is subscribed to anything")
{
    using namespace dispatch;

    EventDispatcher event_dispatcher;
    SingleSubscriber subscriber;

    SomethingHappenedEvent event {};
    event_dispatcher.dispatch(event);

    REQUIRE(subscriber.event_handled == false);
}

TEST_CASE("Test event handled after SingleSubscriber is subscribed to events generally")
{
    using namespace dispatch;

    EventDispatcher event_dispatcher;
    SingleSubscriber subscriber;

    SomethingHappenedEvent event {};
    event_dispatcher.subscribe(&subscriber);
    event_dispatcher.dispatch(event);

    REQUIRE(subscriber.event_handled == true);
}

TEST_CASE("Test event not handled after SingleSubscriber is subscribed to and then unsubscribed from events generally")
{
    using namespace dispatch;

    EventDispatcher event_dispatcher;
    SingleSubscriber subscriber;

    SomethingHappenedEvent event {};
    event_dispatcher.subscribe(&subscriber);
    event_dispatcher.unsubscribe(&subscriber);
    event_dispatcher.dispatch(event);

    REQUIRE(subscriber.event_handled == false);
}

TEST_CASE("Test event handled after SingleSubscriber is subscribed to the specific event type it handles")
{
    using namespace dispatch;

    EventDispatcher event_dispatcher;
    SingleSubscriber subscriber;

    SomethingHappenedEvent event {};
    event_dispatcher.subscribe<SomethingHappenedEvent>(&subscriber);
    event_dispatcher.dispatch(event);

    REQUIRE(subscriber.event_handled == true);
}

TEST_CASE("Test event not handled after SingleSubscriber is subscribed to and then unsubscribed from the specific event type it handles")
{
    using namespace dispatch;

    EventDispatcher event_dispatcher;
    SingleSubscriber subscriber;

    SomethingHappenedEvent event {};
    event_dispatcher.subscribe<SomethingHappenedEvent>(&subscriber);
    event_dispatcher.unsubscribe<SomethingHappenedEvent>(&subscriber);
    event_dispatcher.dispatch(event);

    REQUIRE(subscriber.event_handled == false);
}

TEST_CASE("Test event not handled after SingleSubscriber is subscribed to events generally and then unsubscribed from the specific event type it handles")
{
    using namespace dispatch;

    EventDispatcher event_dispatcher;
    SingleSubscriber subscriber;

    SomethingHappenedEvent event {};
    event_dispatcher.subscribe(&subscriber);
    event_dispatcher.unsubscribe<SomethingHappenedEvent>(&subscriber);
    event_dispatcher.dispatch(event);

    REQUIRE(subscriber.event_handled == false);
}

TEST_CASE("Test event not handled after SingleSubscriber is subscribed to the specific event type it handles and then unsubscribed from events generally")
{
    using namespace dispatch;

    EventDispatcher event_dispatcher;
    SingleSubscriber subscriber;

    SomethingHappenedEvent event {};
    event_dispatcher.subscribe<SomethingHappenedEvent>(&subscriber);
    event_dispatcher.unsubscribe(&subscriber);
    event_dispatcher.dispatch(event);

    REQUIRE(subscriber.event_handled == false);
}


/// MultiSubscriber tests

TEST_CASE("Test EventWithData handled after MultiSubscriber is subscribed")
{
    using namespace dispatch;

    EventDispatcher event_dispatcher;
    MultiSubscriber subscriber;

    event_dispatcher.subscribe(&subscriber);

    EventWithData event { .data = 12345 };
    event_dispatcher.dispatch(event);

    REQUIRE(subscriber.event_with_data_handled == true);
    REQUIRE(subscriber.event_data.has_value() == true);
    REQUIRE(subscriber.event_data.value() == 12345);

    REQUIRE(subscriber.something_happened_event_handled == false);
}

TEST_CASE("Test SomethingHappenedEvent handled after MultiSubscriber is subscribed")
{
    using namespace dispatch;

    EventDispatcher event_dispatcher;
    MultiSubscriber subscriber;

    event_dispatcher.subscribe(&subscriber);

    SomethingHappenedEvent event {};
    event_dispatcher.dispatch(event);

    REQUIRE(subscriber.something_happened_event_handled == true);

    REQUIRE(subscriber.event_with_data_handled == false);
    REQUIRE(subscriber.event_data.has_value() == false);
}

TEST_CASE("Test only EventWithData handled after MultiSubscriber is subscribed to EventWithData specifically")
{
    using namespace dispatch;

    EventDispatcher event_dispatcher;
    MultiSubscriber subscriber;

    event_dispatcher.subscribe<EventWithData>(&subscriber);

    EventWithData event_0 { .data = 12345 };
    SomethingHappenedEvent event_1 {};
    event_dispatcher.dispatch(event_0);
    event_dispatcher.dispatch(event_1);

    REQUIRE(subscriber.event_with_data_handled == true);
    REQUIRE(subscriber.event_data.has_value() == true);
    REQUIRE(subscriber.event_data.value() == 12345);

    REQUIRE(subscriber.something_happened_event_handled == false);
}

TEST_CASE("Test only SomethingHappenedEvent handled after MultiSubscriber is subscribed to SomethingHappenedEvent specifically")
{
    using namespace dispatch;

    EventDispatcher event_dispatcher;
    MultiSubscriber subscriber;

    event_dispatcher.subscribe<SomethingHappenedEvent>(&subscriber);

    EventWithData event_0 { .data = 12345 };
    SomethingHappenedEvent event_1 {};
    event_dispatcher.dispatch(event_0);
    event_dispatcher.dispatch(event_1);

    REQUIRE(subscriber.something_happened_event_handled == true);

    REQUIRE(subscriber.event_with_data_handled == false);
    REQUIRE(subscriber.event_data.has_value() == false);
}

TEST_CASE("Test no events handled when MultiSubscriber is subscribed to EventWithData specifically but SomethingHappenedEvent is dispatched")
{
    using namespace dispatch;

    EventDispatcher event_dispatcher;
    MultiSubscriber subscriber;

    event_dispatcher.subscribe<EventWithData>(&subscriber);

    SomethingHappenedEvent event {};
    event_dispatcher.dispatch(event);

    REQUIRE(subscriber.event_with_data_handled == false);
    REQUIRE(subscriber.event_data.has_value() == false);
    REQUIRE(subscriber.something_happened_event_handled == false);
}

TEST_CASE("Test no events handled when MultiSubscriber is subscribed to SomethingHappenedEvent specifically but EventWithData is dispatched")
{
    using namespace dispatch;

    EventDispatcher event_dispatcher;
    MultiSubscriber subscriber;

    event_dispatcher.subscribe<SomethingHappenedEvent>(&subscriber);

    EventWithData event { .data = 12345 };
    event_dispatcher.dispatch(event);

    REQUIRE(subscriber.event_with_data_handled == false);
    REQUIRE(subscriber.event_data.has_value() == false);
    REQUIRE(subscriber.something_happened_event_handled == false);
}
