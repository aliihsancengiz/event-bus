#include "EventBus/EventBus.hpp"

#include <gtest/gtest.h>

struct TestEvent
{};

struct TestEventWithData
{
	int data;
};

auto& bus = event_bus::EventBus::getInstance();

bool onTestEventFreeFunctionCalled{false};

void onTestEvent(TestEvent ev)
{
    onTestEventFreeFunctionCalled = true;
};


TEST(EventBus, canRegisterDeregisterEvent)
{
	auto onTestEvent = [](TestEvent ev) {};
	

    bus.registerEvent<TestEvent>(onTestEvent);
	bus.removeEvent<TestEvent>();
}

TEST(EventBus,cannotHandleAfterDeregister)
{
	bool isHandled{false};
	
	auto handler = [&isHandled](TestEvent ev){
		isHandled = true;
	};

	bus.registerEvent<TestEvent>(handler);
	bus.removeEvent<TestEvent>();

	bus.fireEvent<TestEvent>(TestEvent{});

	EXPECT_FALSE(isHandled);
}

TEST(EventBus, canUseLambdaAsEventHandler)
{
    bool isHandled{false};
    auto onTestEvent = [&isHandled](TestEvent ev) {
        isHandled = true;
    };
    bus.registerEvent<TestEvent>(onTestEvent);
    bus.fireEvent(TestEvent{});

    EXPECT_TRUE(isHandled);
	
	bus.removeEvent<TestEvent>();
}

TEST(EventBus, canUseFreeFunctionAsEventHandler)
{
    bus.registerEvent<TestEvent>(onTestEvent);
    bus.fireEvent(TestEvent{});

    EXPECT_TRUE(onTestEventFreeFunctionCalled);

	bus.removeEvent<TestEvent>();
}

TEST(EventBus, canUseMemberFunctionAsEventHandler)
{

    struct TestEventHandler
    {
        void onTestEvent(TestEvent ev)
        {
            isHandlerCalled = true;
        }
        bool isHandlerCalled{false};
    };
    TestEventHandler handler;

    bus.registerEvent<TestEvent>(std::bind(&TestEventHandler::onTestEvent, &handler, std::placeholders::_1));
    bus.fireEvent(TestEvent{});

    EXPECT_TRUE(handler.isHandlerCalled);

	bus.removeEvent<TestEvent>();

}


TEST(EventBus, doesNotMissAnyEventHandling)
{
	int counter = 0;
	auto testEventHandler = [&counter](TestEvent ev)
	{
		counter++;
	};

	bus.registerEvent<TestEvent>(testEventHandler);
	
	// Fire 5 times
	bus.fireEvent(TestEvent{});
	bus.fireEvent(TestEvent{});
	bus.fireEvent(TestEvent{});
	bus.fireEvent(TestEvent{});
	bus.fireEvent(TestEvent{});

	EXPECT_EQ(counter,5);

	bus.removeEvent<TestEvent>();
}

TEST(EventBus, canEventStoreState)
{
	int valueFromHandler{0}; 
	auto onTestEventWithData = [&valueFromHandler](TestEventWithData ev)
	{	
		valueFromHandler = ev.data;
	};

	bus.registerEvent<TestEventWithData>(onTestEventWithData);

	EXPECT_EQ(valueFromHandler,0);

	bus.fireEvent(TestEventWithData{45});

	EXPECT_EQ(valueFromHandler,45);

	bus.removeEvent<TestEvent>();
}

TEST(EventBus, HandlerWithMultipleArgs)
{
	int intValueFromHandler{0};
	bool boolValueFromHandler{false};

	auto handlerWithArgs = [&intValueFromHandler,&boolValueFromHandler](TestEvent ev,int integer_arg,bool boolean_arg)
	{
		intValueFromHandler = integer_arg;
		boolValueFromHandler = boolean_arg;
	};

	bus.registerEvent<TestEvent>(handlerWithArgs,12,true);

	bus.fireEvent<TestEvent>(TestEvent{});

	EXPECT_TRUE(boolValueFromHandler);
	EXPECT_EQ(intValueFromHandler,12);

	bus.removeEvent<TestEvent>();

}