/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 PeterBurgess
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
#include "request/request.h"
#include "request/request_dispatcher.h"
#include "request/request_subscriber.h"

#include <iostream>


struct EventWithData {
    int data;
};
struct SomethingHappenedEvent {};

struct RequestWithData : public dispatch::Request<> {
    int data;
};
using DoSomethingRequest = dispatch::Request<>;
struct GiveMeStuffRequest : public dispatch::Request<int> {};


class MySubscriberClass :
        public dispatch::EventSubscriber<EventWithData, SomethingHappenedEvent>,
        public dispatch::RequestSubscriber<RequestWithData, DoSomethingRequest, GiveMeStuffRequest>
{

public:

    void handle_event(const EventWithData& event) override {
        std::cout << "Handling event with data: " << event.data << std::endl;
    }

    void handle_event(const SomethingHappenedEvent& event) override {
        std::cout << "Handling something happened event" << std::endl;
    }

    void handle_request(const RequestWithData& request) override {
        std::cout << "Handling request with data: " << request.data << std::endl;
    }

    void handle_request(const DoSomethingRequest& request) override {
        std::cout << "Handling do something request" << std::endl;
    }

    std::optional<int> handle_request(const GiveMeStuffRequest& request) override {
        std::cout << "Handling give me stuff request and returning `7`" << std::endl;
        return 7;
    }
};


int main()
{
    using namespace dispatch;

    EventDispatcher event_dispatcher;
    RequestDispatcher request_dispatcher;
    MySubscriberClass subscriber;

    EventWithData event { .data = 12345 };
    SomethingHappenedEvent something_happened_event;

    RequestWithData request { .data = 67890 };
    DoSomethingRequest do_something_request;
    GiveMeStuffRequest give_me_stuff_request;


    event_dispatcher.subscribe(&subscriber);
    std::cout << "--Event subscription--" << std::endl;
    event_dispatcher.dispatch(event);
    event_dispatcher.dispatch(something_happened_event);

    event_dispatcher.unsubscribe(&subscriber);
    std::cout << std::endl << "--Event unsubscription--" << std::endl;
    event_dispatcher.dispatch(event);
    event_dispatcher.dispatch(something_happened_event);

    request_dispatcher.subscribe(&subscriber);
    std::cout << std::endl << "--General request subscription--" << std::endl;
    request_dispatcher.dispatch(request);
    request_dispatcher.dispatch(do_something_request);
    request_dispatcher.dispatch(give_me_stuff_request);

    request_dispatcher.unsubscribe(&subscriber);
    std::cout << std::endl << "--General request unsubscription--" << std::endl;
    request_dispatcher.dispatch(request);
    request_dispatcher.dispatch(do_something_request);
    request_dispatcher.dispatch(give_me_stuff_request);

    request_dispatcher.subscribe<RequestWithData, GiveMeStuffRequest>(&subscriber);
    std::cout << std::endl << "--Specific request `MyRequest` and `GiveMeStuffRequest` subscription--" << std::endl;
    request_dispatcher.dispatch(request);
    request_dispatcher.dispatch(do_something_request);
    request_dispatcher.dispatch(give_me_stuff_request);

    request_dispatcher.subscribe<DoSomethingRequest>(&subscriber);
    std::cout << std::endl << "--Specific request `DoSomethingRequest`` subscription--" << std::endl;
    request_dispatcher.dispatch(request);
    request_dispatcher.dispatch(do_something_request);
    request_dispatcher.dispatch(give_me_stuff_request);

    request_dispatcher.unsubscribe<RequestWithData>(&subscriber);
    std::cout << std::endl << "--Specific request `Request` unsubscription--" << std::endl;
    request_dispatcher.dispatch(request);
    request_dispatcher.dispatch(do_something_request);
    request_dispatcher.dispatch(give_me_stuff_request);

    return 0;
}
