#include "EventBus.hpp"

#include <iostream>

struct MyData
{
    int foo;
};

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

int main()
{
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
}
