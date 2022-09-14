#include "EventBus/EventBus.hpp"

#include <iostream>

struct MyData
{
    int foo;
};

namespace named_eventbus_example {

void MyDataHandler(std::string topic, MyData data, int x)
{
    std::cout << "Topic : " << topic << "  onMyDataHandler : foo " << data.foo << " X : " << x
              << std::endl;
}

void fooHandler(std::string topic, MyData data)
{
    std::cout << "Topic : " << topic << "  onfooHandler : foo: " << data.foo << std::endl;
}

struct FunctionObject
{
    FunctionObject() {}
    void operator()(std::string topic, MyData data, int k) const
    {
        std::cout << "Topic : " << topic << "  onFunctionObjectHandler : foo: " << data.foo
                  << " k : " << k << std::endl;
    }
};

struct MemberFunctionBinding
{
    void onMyDataEvent(std::string topic, MyData data)
    {
        std::cout << "Topic : " << topic << "  MemberFunctionBinding::onMyDataEvent " << data.foo
                  << std::endl;
    }
};
}  // namespace named_eventbus_example

namespace eventbus_example {

void MyDataHandler(MyData data, int x)
{
    std::cout << "onMyDataHandler : foo " << data.foo << " X : " << x << std::endl;
}

void fooHandler(MyData data)
{
    std::cout << "onfooHandler : foo: " << data.foo << std::endl;
}

struct FunctionObject
{
    FunctionObject() {}
    void operator()(MyData data, int k) const
    {
        std::cout << "onFunctionObjectHandler : foo: " << data.foo << " k : " << k << std::endl;
    }
};

struct MemberFunctionBinding
{
    void onMyDataEvent(MyData data)
    {
        std::cout << "MemberFunctionBinding::onMyDataEvent " << data.foo << std::endl;
    }
};
}  // namespace eventbus_example

void eventbusUseCase()
{
    eventbus_example::MemberFunctionBinding m1;
    event_bus::EventBus::getInstance().registerEvent<MyData>(eventbus_example::fooHandler);
    event_bus::EventBus::getInstance().registerEvent<MyData>(eventbus_example::MyDataHandler, 78);
    event_bus::EventBus::getInstance().registerEvent<MyData>([](MyData data) {
        std::cout << "onLambdaHandler foo : " << data.foo << std::endl;
    });

    event_bus::EventBus::getInstance().registerEvent<MyData>(eventbus_example::FunctionObject{},
                                                             42);
    event_bus::EventBus::getInstance().registerEvent<MyData>(std::bind(
      &eventbus_example::MemberFunctionBinding::onMyDataEvent, &m1, std::placeholders::_1));

    event_bus::EventBus::getInstance().fireEvent(MyData{45});

    event_bus::EventBus::getInstance().removeEvent<MyData>();
}

void namedEventBusUseCase()
{
    const std::string fooTopic{"/foo/bar/"};

    named_eventbus_example::MemberFunctionBinding m2;
    event_bus::NamedEventBus::getInstance().registerEvent<MyData>(
      fooTopic, named_eventbus_example::fooHandler);
    event_bus::NamedEventBus::getInstance().registerEvent<MyData>(
      fooTopic, named_eventbus_example::MyDataHandler, 78);
    event_bus::NamedEventBus::getInstance().registerEvent<MyData>(
      fooTopic, [](std::string topic, MyData data) {
          std::cout << "Topic : " << topic << "  onLambdaHandler foo : " << data.foo << std::endl;
      });

    event_bus::NamedEventBus::getInstance().registerEvent<MyData>(
      fooTopic, named_eventbus_example::FunctionObject{}, 42);
    event_bus::NamedEventBus::getInstance().registerEvent<MyData>(
      fooTopic, std::bind(&named_eventbus_example::MemberFunctionBinding::onMyDataEvent, &m2,
                          std::placeholders::_1, std::placeholders::_2));

    event_bus::NamedEventBus::getInstance().fireEvent(fooTopic, MyData{45});

    event_bus::NamedEventBus::getInstance().removeEvent<MyData>(fooTopic);
}

int main()
{
    eventbusUseCase();
    namedEventBusUseCase();
}
