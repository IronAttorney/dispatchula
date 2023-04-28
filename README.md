# Dispatchula

**A header only library defining two similar classes designed that can
underpin large scale projects and allow for a flat hierarchy structure.**

## Events

### Examples

_Example usage of the concepts described below can be found in
`./example/event_demo.cpp` and `./example/combined_demo.cpp`._

### Defining Events

User defined event types can be either empty implementations or as simple as
plain old data structures. The only members of an event (if any) are those
required to describe the event of which you wish to inform subscribers.

### Event Subscriber

Multiple subscriber objects (irrespective of type) can subscribe to the same
event type at any given time.

`EventSubscriber` takes event type(s), as described above, as template arguments.
Any class wishing to handle dispatched events must be derived from this class
and must include all event types it wishes to handle in the template parameter
list for `EventSubscriber`.

An `EventSubscriber` cannot repeat template argument types as this would equate to
subscribing to the same event multiple times. Aliased types are treated as repeated
types.

The derived subscriber class will then need to implement one handler function called
`handle_event` per event type in the template parameter list `EVENT_TYPE_LIST`
for the `EventSubscriber` base class.

Each handler function takes one argument of the given event type, and has a return
type of `void`.

### Event Dispatcher

_(The below instructions assume a `EventDispatcher` named `event_dispatcher` has been constructed)_

##### Subscribe

Call `event_dispatcher.subscribe(&subscriber);` to subscribe to all event types
listed in the subscriber's template parameters.

##### Unsubscribe

Call `event_dispatcher.unsubscribe(&subscriber);` to unsubscribe from all event types
listed in the subscriber's template parameters.

##### Dispatch

It is valid for there to be no subscribers to  a give event type when an instance of that event
is dispatched.

Call `event_dispatcher.dispatch(event);` to dispatch an event to be handled by all objects
currently subscribed to the given event type.

## Requests

### Examples

_Example usage of the concepts described below can be found in
`./example/request_demo.cpp` and `./example/combined_demo.cpp`._

### Defining Requests

User defined request follow the same requirements as user defined request types
described above. Additionally, they must either derive from the templated `Request`
class or be an alias of `Request` with the template parameter defined  if they
don't require any member data to describe the request.

The `Request` template parameter called `RESULT_TYPE` defines a return type for
the request handler function that must be implemented for any class that
subscribes to the given request type.

### Request Subscriber

Only one subscriber object (irrespective of type) can subscribe to the same
request type at a given time. If the current subscriber to a given request
unsubscribes, then a new object of any type can subscribe to that request
type.

`RequestSubscriber` takes request type(s), as described above, as template arguments.
Any class wishing to handle dispatched request must be derived from this class
and must include all request types it wishes to handle in the template parameter
list for `RequestSubscriber`.

A `RequestSubscriber` cannot repeat template argument types as this would equate to
subscribing to the same event multiple times. Aliased types are treated as repeated
types.

The derived subscriber class will then need to implement one handler function called
`handle_request` per request type in the template parameter list `REQUEST_TYPE_LIST`
for the `RequestSubscriber` base class.

Each handler function takes one argument of the given request type, and has a return
type of either `void` or the type defined in the given request's template parameter
`RESULT_TYPE`.

### Request Dispatcher

_(The below instructions assume a `RequestDispatcher` named `request_dispatcher` has been constructed)_

##### Subscribe

Call `request_dispatcher.subscribe(&subscriber);` to subscribe to all request types
listed in the subscriber's template parameters.

Call `request_dispatcher.unsubscribe(&subscriber);` to unsubscribe from all request types
listed in the subscriber's template parameters.

##### Unsubscribe

Call `request_dispatcher.subscribe<RequestType1, RequestType2, ...>(subscriber)` to
subscribe to specific request types.

Call `request_dispatcher.unsubscribe<RequestType1, RequestType2, ...>(subscriber)` to
unsubscribe from specific request types.

##### Dispatch

It is valid for there to be no subscribers to a give request type when an instance of that request
is dispatched.

Call `request_dispatcher.dispatch(request);` to dispatch a request to be handled by the object
currently subscribed to the given request type.

Call `auto result = request_dispatcher.dispatch(request);` or similar if you expect this request
handler function to result in a return type. `auto` == `std::optional<RESULT_TYPE>` where
`RESULT_TYPE` is the template parameter of the given request type as detailed above. If no object
is subscribed to a given request type with a non-void return type at the time that the request is
dispatched, `std::nullopt` is returned.