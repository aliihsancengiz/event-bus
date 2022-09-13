#include "EventBus/EventBus.hpp"

#include <gtest/gtest.h>

struct TestEvent
{};

struct TestEventWithData
{
	int data;
};

bool onTestEventFreeFunctionCalled{false};

void onTestEvent(TestEvent ev)
{
    onTestEventFreeFunctionCalled = true;
};


TEST(EventBus, canRegisterDeregisterEvent)
{
	auto onTestEvent = [](TestEvent ev) {};
    event_bus::EventBus::getInstance().registerEvent<TestEvent>(onTestEvent);
	event_bus::EventBus::getInstance().removeEvent<TestEvent>();
}


TEST(EventBus, canUseLambdaAsEventHandler)
{
    bool isHandled{false};
    auto onTestEvent = [&isHandled](TestEvent ev) {
        isHandled = true;
    };
    event_bus::EventBus::getInstance().registerEvent<TestEvent>(onTestEvent);
    event_bus::EventBus::getInstance().fireEvent(TestEvent{});

    EXPECT_TRUE(isHandled);
	
	event_bus::EventBus::getInstance().removeEvent<TestEvent>();
}

TEST(EventBus, canUseFreeFunctionAsEventHandler)
{
    event_bus::EventBus::getInstance().registerEvent<TestEvent>(onTestEvent);
    event_bus::EventBus::getInstance().fireEvent(TestEvent{});

    EXPECT_TRUE(onTestEventFreeFunctionCalled);

	event_bus::EventBus::getInstance().removeEvent<TestEvent>();
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

    event_bus::EventBus::getInstance().registerEvent<TestEvent>(std::bind(&TestEventHandler::onTestEvent, &handler, std::placeholders::_1));
    event_bus::EventBus::getInstance().fireEvent(TestEvent{});

    EXPECT_TRUE(handler.isHandlerCalled);

	event_bus::EventBus::getInstance().removeEvent<TestEvent>();

}


TEST(EventBus, doesNotMissAnyEventHandling)
{
	int counter = 0;
	auto testEventHandler = [&counter](TestEvent ev)
	{
		counter++;
	};

	event_bus::EventBus::getInstance().registerEvent<TestEvent>(testEventHandler);
	
	// Fire 5 times
	event_bus::EventBus::getInstance().fireEvent(TestEvent{});
	event_bus::EventBus::getInstance().fireEvent(TestEvent{});
	event_bus::EventBus::getInstance().fireEvent(TestEvent{});
	event_bus::EventBus::getInstance().fireEvent(TestEvent{});
	event_bus::EventBus::getInstance().fireEvent(TestEvent{});

	EXPECT_EQ(counter,5);

	event_bus::EventBus::getInstance().removeEvent<TestEvent>();
}

TEST(EventBus, canEventStoreState)
{
	int valueFromHandler{0}; 
	auto onTestEventWithData = [&valueFromHandler](TestEventWithData ev)
	{	
		valueFromHandler = ev.data;
	};

	event_bus::EventBus::getInstance().registerEvent<TestEventWithData>(onTestEventWithData);

	EXPECT_EQ(valueFromHandler,0);

	event_bus::EventBus::getInstance().fireEvent(TestEventWithData{45});

	EXPECT_EQ(valueFromHandler,45);

	event_bus::EventBus::getInstance().removeEvent<TestEvent>();
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

	event_bus::EventBus::getInstance().registerEvent<TestEvent>(handlerWithArgs,12,true);

	event_bus::EventBus::getInstance().fireEvent<TestEvent>(TestEvent{});

	EXPECT_TRUE(boolValueFromHandler);
	EXPECT_EQ(intValueFromHandler,12);

	event_bus::EventBus::getInstance().removeEvent<TestEvent>();

}