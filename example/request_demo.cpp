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


#include "request/request.h"
#include "request/request_dispatcher.h"
#include "request/request_subscriber.h"

#include <iostream>


struct RequestWithData : public dispatch::Request<> {
    int data;
};
using DoSomethingRequest = dispatch::Request<>;
struct GiveMeStuffRequest : public dispatch::Request<int> {};
struct GiveMeStuffIfYouLikeRequest : public dispatch::Request<std::optional<int>> {};
struct GiveMePointersRequest : public dispatch::Request<std::shared_ptr<int>> {};
struct GiveMeRawPointersRequest : public dispatch::Request<float*> {};


class MySubscriberClass : public dispatch::RequestSubscriber<RequestWithData, DoSomethingRequest, GiveMeStuffRequest, GiveMeStuffIfYouLikeRequest, GiveMePointersRequest, GiveMeRawPointersRequest>
{

public:

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

    std::optional<int> handle_request(const GiveMeStuffIfYouLikeRequest& request) override {
        std::cout << "Didn't feel like handling give me stuff if you like request and returning `std::nullptr`" << std::endl;
        return std::nullopt;
    }

    std::shared_ptr<int> handle_request(const GiveMePointersRequest& request) override {
        std::cout << "Didn't feel like handling give me pointers request and returning `nullptr`" << std::endl;
        return nullptr;
    }

    float* handle_request(const GiveMeRawPointersRequest& request) override {
        std::cout << "Didn't feel like handling give me raw pointers request and returning `nullptr`" << std::endl;
        return nullptr;
    }
};


int main()
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;
    MySubscriberClass subscriber;

    RequestWithData request { .data = 67890 };
    DoSomethingRequest do_something_request;
    GiveMeStuffRequest give_me_stuff_request;
    GiveMeStuffIfYouLikeRequest give_me_stuff_if_you_like_request;
    GiveMePointersRequest give_me_pointers_request;
    GiveMeRawPointersRequest give_me_raw_pointers_request;

    request_dispatcher.subscribe(&subscriber);
    std::cout << std::endl << "--General request subscription--" << std::endl;
    request_dispatcher.dispatch(request);
    request_dispatcher.dispatch(do_something_request);
    request_dispatcher.dispatch(give_me_stuff_request);
    request_dispatcher.dispatch(give_me_stuff_if_you_like_request);
    request_dispatcher.dispatch(give_me_pointers_request);
    request_dispatcher.dispatch(give_me_raw_pointers_request);

    request_dispatcher.unsubscribe(&subscriber);
    std::cout << std::endl << "--General request unsubscription--" << std::endl;
    request_dispatcher.dispatch(request);
    request_dispatcher.dispatch(do_something_request);
    request_dispatcher.dispatch(give_me_stuff_request);
    request_dispatcher.dispatch(give_me_stuff_if_you_like_request);
    request_dispatcher.dispatch(give_me_pointers_request);
    request_dispatcher.dispatch(give_me_raw_pointers_request);

    request_dispatcher.subscribe<RequestWithData, GiveMeStuffRequest, GiveMePointersRequest>(&subscriber);
    std::cout << std::endl << "--Specific request `MyRequest`, `GiveMeStuffRequest` and `GiveMePointersRequest` subscription--" << std::endl;
    request_dispatcher.dispatch(request);
    request_dispatcher.dispatch(do_something_request);
    request_dispatcher.dispatch(give_me_stuff_request);
    request_dispatcher.dispatch(give_me_stuff_if_you_like_request);
    request_dispatcher.dispatch(give_me_pointers_request);
    request_dispatcher.dispatch(give_me_raw_pointers_request);

    request_dispatcher.subscribe<DoSomethingRequest>(&subscriber);
    std::cout << std::endl << "--Specific request `DoSomethingRequest`` subscription--" << std::endl;
    request_dispatcher.dispatch(request);
    request_dispatcher.dispatch(do_something_request);
    request_dispatcher.dispatch(give_me_stuff_request);
    request_dispatcher.dispatch(give_me_stuff_if_you_like_request);
    request_dispatcher.dispatch(give_me_pointers_request);
    request_dispatcher.dispatch(give_me_raw_pointers_request);

    request_dispatcher.unsubscribe<RequestWithData>(&subscriber);
    std::cout << std::endl << "--Specific request `Request` unsubscription--" << std::endl;
    request_dispatcher.dispatch(request);
    request_dispatcher.dispatch(do_something_request);
    request_dispatcher.dispatch(give_me_stuff_request);
    request_dispatcher.dispatch(give_me_stuff_if_you_like_request);
    request_dispatcher.dispatch(give_me_pointers_request);
    request_dispatcher.dispatch(give_me_raw_pointers_request);

    return 0;
}
