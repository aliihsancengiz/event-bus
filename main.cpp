#include "EventBus.hpp"

#include <iostream>

struct MyData
{
    int foo;
};

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

int main()
{
    /*
    MemberFunctionBinding x;
    event_bus::EventBus::getInstance().registerEvent<MyData>(fooHandler);
    event_bus::EventBus::getInstance().registerEvent<MyData>(MyDataHandler, 78);
    event_bus::EventBus::getInstance().registerEvent<MyData>([](MyData data) {
        std::cout << "onLambdaHandler foo : " << data.foo << std::endl;
    });

    event_bus::EventBus::getInstance().registerEvent<MyData>(FunctionObject{}, 42);
    event_bus::EventBus::getInstance().registerEvent<MyData>(
      std::bind(&MemberFunctionBinding::onMyDataEvent, &x, std::placeholders::_1));

    event_bus::EventBus::getInstance().fireEvent(MyData{45});

    event_bus::EventBus::getInstance().removeEvent<MyData>();
    */

    const std::string fooTopic{"/foo/bar/"};

    MemberFunctionBinding x;
    event_bus::NamedEventBus::getInstance().registerEvent<MyData>(fooTopic, fooHandler);
    event_bus::NamedEventBus::getInstance().registerEvent<MyData>(fooTopic, MyDataHandler, 78);
    event_bus::NamedEventBus::getInstance().registerEvent<MyData>(
      fooTopic, [](std::string topic, MyData data) {
          std::cout << "Topic : " << topic << "  onLambdaHandler foo : " << data.foo << std::endl;
      });

    event_bus::NamedEventBus::getInstance().registerEvent<MyData>(fooTopic, FunctionObject{}, 42);
    event_bus::NamedEventBus::getInstance().registerEvent<MyData>(
      fooTopic, std::bind(&MemberFunctionBinding::onMyDataEvent, &x, std::placeholders::_1,
                          std::placeholders::_2));

    event_bus::NamedEventBus::getInstance().fireEvent(fooTopic, MyData{45});

    event_bus::NamedEventBus::getInstance().removeEvent<MyData>(fooTopic);
}
