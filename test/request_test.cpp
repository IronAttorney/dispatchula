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


#include "request/request.h"
#include "request/request_dispatcher.h"
#include "request/request_subscriber.h"

#include "catch2/catch_test_macros.hpp"

#include <memory>
#include <optional>
#include <string>


struct Object {
    int data;
};

enum class ErrorMessage
{
    NoRequestSubscriberFound = -1,
};

using DoSomethingRequest = dispatch::Request<>;
struct GiveMeStuffRequest : public dispatch::Request<int> {};
struct GiveMeAnObjectRequest : public dispatch::Request<Object> {};
struct GiveMeStuffIfYouLikeRequest : public dispatch::Request<std::optional<int>> {};
struct GiveMeExpectedStuffOrErrorRequest : public dispatch::Request<std::expected<int, ErrorMessage>> {};
struct GiveMePointersRequest : public dispatch::Request<std::shared_ptr<int>> {};
struct GiveMeUniquePointersRequest : public dispatch::Request<std::unique_ptr<int>> {};
struct GiveMeRawPointersRequest : public dispatch::Request<int*> {};
struct ReadBackMyDataRequest : public dispatch::Request<std::string> {
    int data;
};


class SingleSubscriber : public dispatch::RequestSubscriber<ReadBackMyDataRequest>
{

public:

    std::optional<std::string> handle_request(const ReadBackMyDataRequest& request) override
    {
        return std::to_string(request.data);
    }
};

class MultiSubscriber : public dispatch::RequestSubscriber<DoSomethingRequest,
                                                           GiveMeStuffRequest,
                                                           GiveMeAnObjectRequest,
                                                           GiveMeStuffIfYouLikeRequest,
                                                           GiveMeExpectedStuffOrErrorRequest,
                                                           GiveMePointersRequest,
                                                           GiveMeUniquePointersRequest,
                                                           GiveMeRawPointersRequest,
                                                           ReadBackMyDataRequest>
{

public:

    void handle_request(const DoSomethingRequest& request) override
    {
        do_something_request_handled = true;
    }

    std::optional<int> handle_request(const GiveMeStuffRequest& request) override
    {
        return 12345;
    }

    std::optional<Object> handle_request(const GiveMeAnObjectRequest& request) override
    {
        return Object { .data = 12345 };
    }

    std::optional<int> handle_request(const GiveMeStuffIfYouLikeRequest& request) override
    {
        return 12345;
    }

    std::expected<int, ErrorMessage> handle_request(const GiveMeExpectedStuffOrErrorRequest& request) override
    {
        return 12345;
    }

    std::shared_ptr<int> handle_request(const GiveMePointersRequest& request) override
    {
        return std::make_shared<int> ( 12345 );
    }

    std::unique_ptr<int> handle_request(const GiveMeUniquePointersRequest& request) override
    {
        return std::make_unique<int> ( 12345 );
    }

    int* handle_request(const GiveMeRawPointersRequest& request) override
    {
        static int return_value = 12345;
        return &return_value;
    }

    std::optional<std::string> handle_request(const ReadBackMyDataRequest& request) override
    {
        return std::to_string(request.data);
    }

    bool do_something_request_handled = false;
};


/// Return type tests

TEST_CASE("Test request with no return type returns nothing")
{
    using namespace dispatch;

    STATIC_REQUIRE(std::is_same_v<
            decltype(std::declval<MultiSubscriber>().handle_request(std::declval<DoSomethingRequest>())),
            void
    >);

    STATIC_REQUIRE(std::is_same_v<
            decltype(std::declval<RequestDispatcher>().dispatch(std::declval<DoSomethingRequest>())),
            void
    >);
}

TEST_CASE("Test request with value return type returns value wrapped in `std::optional`")
{
    using namespace dispatch;

    STATIC_REQUIRE(std::is_same_v<
            decltype(std::declval<MultiSubscriber>().handle_request(std::declval<GiveMeStuffRequest>())),
            std::optional<int>
    >);

    STATIC_REQUIRE(std::is_same_v<
            decltype(std::declval<RequestDispatcher>().dispatch(std::declval<GiveMeStuffRequest>())),
            std::optional<int>
    >);
}

TEST_CASE("Test request with object return type returns object wrapped in `std::optional`")
{
    using namespace dispatch;

    STATIC_REQUIRE(std::is_same_v<
            decltype(std::declval<MultiSubscriber>().handle_request(std::declval<GiveMeAnObjectRequest>())),
            std::optional<Object>
    >);

    STATIC_REQUIRE(std::is_same_v<
            decltype(std::declval<RequestDispatcher>().dispatch(std::declval<GiveMeAnObjectRequest>())),
            std::optional<Object>
    >);
}

TEST_CASE("Test request with `std::optional` return type returns the same type as defined")
{
    using namespace dispatch;

    STATIC_REQUIRE(std::is_same_v<
            decltype(std::declval<MultiSubscriber>().handle_request(std::declval<GiveMeStuffIfYouLikeRequest>())),
            std::optional<int>
    >);

    STATIC_REQUIRE(std::is_same_v<
            decltype(std::declval<RequestDispatcher>().dispatch(std::declval<GiveMeStuffIfYouLikeRequest>())),
            std::optional<int>
    >);
}

TEST_CASE("Test request with `std::expected` return type returns the same type as defined")
{
    using namespace dispatch;

    STATIC_REQUIRE(std::is_same_v<
            decltype(std::declval<MultiSubscriber>().handle_request(std::declval<GiveMeExpectedStuffOrErrorRequest>())),
            std::expected<int, ErrorMessage>
    >);

    STATIC_REQUIRE(std::is_same_v<
            decltype(std::declval<RequestDispatcher>().dispatch(std::declval<GiveMeExpectedStuffOrErrorRequest>())),
            std::expected<int, ErrorMessage>
    >);
}

TEST_CASE("Test request with `std::shared_ptr` return type returns the same type as defined")
{
    using namespace dispatch;

    STATIC_REQUIRE(std::is_same_v<
            decltype(std::declval<MultiSubscriber>().handle_request(std::declval<GiveMePointersRequest>())),
            std::shared_ptr<int>
    >);

    STATIC_REQUIRE(std::is_same_v<
            decltype(std::declval<RequestDispatcher>().dispatch(std::declval<GiveMePointersRequest>())),
            std::shared_ptr<int>
    >);
}

TEST_CASE("Test request with `std::unique_ptr` return type returns the same type as defined")
{
    using namespace dispatch;

    STATIC_REQUIRE(std::is_same_v<
            decltype(std::declval<MultiSubscriber>().handle_request(std::declval<GiveMeUniquePointersRequest>())),
            std::unique_ptr<int>
    >);

    STATIC_REQUIRE(std::is_same_v<
            decltype(std::declval<RequestDispatcher>().dispatch(std::declval<GiveMeUniquePointersRequest>())),
            std::unique_ptr<int>
    >);
}

TEST_CASE("Test request with raw pointer return type returns the same type as defined")
{
    using namespace dispatch;

    STATIC_REQUIRE(std::is_same_v<
            decltype(std::declval<MultiSubscriber>().handle_request(std::declval<GiveMeRawPointersRequest>())),
            int*
    >);

    STATIC_REQUIRE(std::is_same_v<
            decltype(std::declval<RequestDispatcher>().dispatch(std::declval<GiveMeRawPointersRequest>())),
            int*
    >);
}

TEST_CASE("Test request with string return type returns value wrapped in `std::optional`")
{
    using namespace dispatch;

    STATIC_REQUIRE(std::is_same_v<
            decltype(std::declval<MultiSubscriber>().handle_request(std::declval<ReadBackMyDataRequest>())),
            std::optional<std::string>
    >);

    STATIC_REQUIRE(std::is_same_v<
            decltype(std::declval<RequestDispatcher>().dispatch(std::declval<ReadBackMyDataRequest>())),
            std::optional<std::string>
    >);
}


/// Single subscriber tests

TEST_CASE("Test request not handled before SingleSubscriber is subscribed to anything")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;
    SingleSubscriber subscriber;

    ReadBackMyDataRequest request { .data = 12345 };
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result.has_value() == false);
}

TEST_CASE("Test request handled after SingleSubscriber is subscribed to requests generally")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;
    SingleSubscriber subscriber;

    ReadBackMyDataRequest request { .data = 12345 };
    request_dispatcher.subscribe(&subscriber);
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result.has_value() == true);
    REQUIRE(result.value() == "12345");
}

TEST_CASE("Test request not handled after SingleSubscriber is subscribed to and then unsubscribed from requests generally")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;
    SingleSubscriber subscriber;

    ReadBackMyDataRequest request { .data = 12345 };
    request_dispatcher.subscribe(&subscriber);
    request_dispatcher.unsubscribe(&subscriber);
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result.has_value() == false);
}

TEST_CASE("Test request handled after SingleSubscriber is subscribed to the specific request type it handles")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;
    SingleSubscriber subscriber;

    ReadBackMyDataRequest request { .data = 12345 };
    request_dispatcher.subscribe<ReadBackMyDataRequest>(&subscriber);
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result.has_value() == true);
    REQUIRE(result.value() == "12345");
}

TEST_CASE("Test request not handled after SingleSubscriber is subscribed to and then unsubscribed from the specific request type it handles")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;
    SingleSubscriber subscriber;

    ReadBackMyDataRequest request { .data = 12345 };
    request_dispatcher.subscribe<ReadBackMyDataRequest>(&subscriber);
    request_dispatcher.unsubscribe<ReadBackMyDataRequest>(&subscriber);
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result.has_value() == false);
}

TEST_CASE("Test request not handled after SingleSubscriber is subscribed to requests generally and then unsubscribed from the specific request type it handles")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;
    SingleSubscriber subscriber;

    ReadBackMyDataRequest request { .data = 12345 };
    request_dispatcher.subscribe(&subscriber);
    request_dispatcher.unsubscribe<ReadBackMyDataRequest>(&subscriber);
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result.has_value() == false);
}

TEST_CASE("Test request not handled after SingleSubscriber is subscribed to the specific request type it handles and then unsubscribed from requests generally")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;
    SingleSubscriber subscriber;

    ReadBackMyDataRequest request { .data = 12345 };
    request_dispatcher.subscribe<ReadBackMyDataRequest>(&subscriber);
    request_dispatcher.unsubscribe(&subscriber);
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result.has_value() == false);
}


/// MultiSubscriber tests for when subscriber exists

TEST_CASE("Test request with no return type handled")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;
    MultiSubscriber subscriber;

    DoSomethingRequest request {};
    request_dispatcher.subscribe(&subscriber);
    request_dispatcher.dispatch(request);

    REQUIRE(subscriber.do_something_request_handled == true);
}

TEST_CASE("Test request with value return type handled")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;
    MultiSubscriber subscriber;

    GiveMeStuffRequest request {};
    request_dispatcher.subscribe(&subscriber);
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result.has_value() == true);
    REQUIRE(result.value() == 12345);
}

TEST_CASE("Test request with object return type handled")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;
    MultiSubscriber subscriber;

    GiveMeAnObjectRequest request {};
    request_dispatcher.subscribe(&subscriber);
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result.has_value() == true);
    REQUIRE(result.value().data == 12345);
}

TEST_CASE("Test request with `std::optional` return type handled")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;
    MultiSubscriber subscriber;

    GiveMeStuffIfYouLikeRequest request {};
    request_dispatcher.subscribe(&subscriber);
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result.has_value() == true);
    REQUIRE(result.value() == 12345);
}

TEST_CASE("Test request with `std::expected` return type handled")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;
    MultiSubscriber subscriber;

    GiveMeExpectedStuffOrErrorRequest request {};
    request_dispatcher.subscribe(&subscriber);
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result.has_value() == true);
    REQUIRE(result.value() == 12345);
}

TEST_CASE("Test request with `std::shared_ptr` return type handled")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;
    MultiSubscriber subscriber;

    GiveMePointersRequest request {};
    request_dispatcher.subscribe(&subscriber);
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result != nullptr);
    REQUIRE(*result == 12345);
}

TEST_CASE("Test request with `std::unique_ptr` return type handled")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;
    MultiSubscriber subscriber;

    GiveMeUniquePointersRequest request {};
    request_dispatcher.subscribe(&subscriber);
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result != nullptr);
    REQUIRE(*result == 12345);
}

TEST_CASE("Test request with raw pointer return type handled")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;
    MultiSubscriber subscriber;

    GiveMeRawPointersRequest request {};
    request_dispatcher.subscribe(&subscriber);
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result != nullptr);
    REQUIRE(*result == 12345);
}

TEST_CASE("Test request with string return type handled")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;
    MultiSubscriber subscriber;

    ReadBackMyDataRequest request { .data = 12345 };
    request_dispatcher.subscribe(&subscriber);
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result.has_value() == true);
    REQUIRE(result.value() == "12345");
}


/// MultiSubscriber tests for when subscriber does not exist

TEST_CASE("Test request with value return type returns `std:::nullopt` when no subscriber exists")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;

    GiveMeStuffRequest request {};
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result.has_value() == false);
}

TEST_CASE("Test request with object return type returns `std:::nullopt` when no subscriber exists")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;

    GiveMeAnObjectRequest request {};
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result.has_value() == false);
}

TEST_CASE("Test request with `std::optional` return type returns `std:::nullopt` when no subscriber exists")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;

    GiveMeStuffIfYouLikeRequest request {};
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result.has_value() == false);
}

TEST_CASE("Test request with `std::expected` return type returns error when no subscriber exists")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;

    GiveMeExpectedStuffOrErrorRequest request {};
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result.has_value() == false);
    REQUIRE(result.error() == ErrorMessage::NoRequestSubscriberFound);
}

TEST_CASE("Test request with `std::shared_ptr` return type returns `nullptr` when no subscriber exists")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;

    GiveMePointersRequest request {};
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result == nullptr);
}

TEST_CASE("Test request with `std::unique_ptr` return type returns `nullptr` when no subscriber exists")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;

    GiveMeUniquePointersRequest request {};
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result == nullptr);
}

TEST_CASE("Test request with raw pointer return type returns `nullptr` when no subscriber exists")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;

    GiveMeRawPointersRequest request {};
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result == nullptr);
}

TEST_CASE("Test request with string return type returns `std::nullopt` when no subscriber exists")
{
    using namespace dispatch;

    RequestDispatcher request_dispatcher;

    ReadBackMyDataRequest request { .data = 12345 };
    auto result = request_dispatcher.dispatch(request);

    REQUIRE(result.has_value() == false);
}