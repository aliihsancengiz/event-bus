#include "EventBus/EventBus.hpp"

#include <gtest/gtest.h>

namespace 
{
	struct TestEvent
	{};

	struct TestEventWithData
	{
		int data;
	};

	auto& namedBus = event_bus::NamedEventBus::getInstance();

	bool onTestEventFreeFunctionCalled{false};

	void onTestEvent(std::string topic,TestEvent ev)
	{
		onTestEventFreeFunctionCalled = true;
	};

} // namespace 

TEST(NamedEventBus, canRegisterDeregisterEvent)
{
	const std::string testTopic{"/test/canRegisterDeregisterEvent"};
	auto onTestEvent = [&testTopic](std::string topic,TestEvent ev) {
		EXPECT_TRUE(topic == testTopic);
	};
	namedBus.registerEvent<TestEvent>(testTopic,onTestEvent);
	namedBus.removeEvent<TestEvent>(testTopic);
}

TEST(NamedEventBus,cannotHandleAfterDeregister)
{
	const std::string testTopic{"cannotHandleAfterDeregister"};
	bool isHandled{false};
	
	auto handler = [&isHandled](std::string topic,TestEvent ev){
		isHandled = true;
	};

	namedBus.registerEvent<TestEvent>(testTopic,handler);
	namedBus.removeEvent<TestEvent>(testTopic);

	namedBus.fireEvent<TestEvent>(testTopic,TestEvent{});

	EXPECT_FALSE(isHandled);
}

TEST(NamedEventBus, canUseLambdaAsEventHandler)
{
	const std::string testTopic{"/test/canUseLambdaAsEventHandler"};
	
	bool isHandled{false};
	auto onTestEvent = [&isHandled,&testTopic](std::string topic,TestEvent ev) {
		isHandled = true;
		EXPECT_TRUE(topic == testTopic);
	};
   namedBus.registerEvent<TestEvent>(testTopic,onTestEvent);
   namedBus.fireEvent(testTopic,TestEvent{});

	EXPECT_TRUE(isHandled);
	
	namedBus.removeEvent<TestEvent>(testTopic);
}

TEST(NamedEventBus, canUseFreeFunctionAsEventHandler)
{
	const std::string testTopic{"/test/canUseFreeFunctionAsEventHandler"};

	namedBus.registerEvent<TestEvent>(testTopic,onTestEvent);
	namedBus.fireEvent(testTopic,TestEvent{});

	EXPECT_TRUE(onTestEventFreeFunctionCalled);

	namedBus.removeEvent<TestEvent>(testTopic);
}

TEST(NamedEventBus, canUseMemberFunctionAsEventHandler)
{
	const std::string testTopic{"/test/canUseMemberFunctionAsEventHandler"};

	struct TestEventHandler
	{
		void onTestEvent(std::string topic,TestEvent ev)
		{
			isHandlerCalled = true;
			EXPECT_TRUE(topic == testTopic);

		}
		bool isHandlerCalled{false};
		const std::string testTopic{"/test/canUseMemberFunctionAsEventHandler"};

	};
	TestEventHandler handler;

	namedBus.registerEvent<TestEvent>(handler.testTopic,std::bind(&TestEventHandler::onTestEvent, &handler, std::placeholders::_1,std::placeholders::_2));
	namedBus.fireEvent(handler.testTopic,TestEvent{});

	EXPECT_TRUE(handler.isHandlerCalled);

	namedBus.removeEvent<TestEvent>(handler.testTopic);

}


TEST(NamedEventBus, doesNotMissAnyEventHandling)
{
	const std::string testTopic{"/test/doesNotMissAnyEventHandling"};
	int counter = 0;
	auto testEventHandler = [&counter,&testTopic](std::string topic,TestEvent ev)
	{
		EXPECT_TRUE(topic == testTopic);
		counter++;
	};

	namedBus.registerEvent<TestEvent>(testTopic,testEventHandler);
	
	// Fire 5 times
	namedBus.fireEvent(testTopic,TestEvent{});
	namedBus.fireEvent(testTopic,TestEvent{});
	namedBus.fireEvent(testTopic,TestEvent{});
	namedBus.fireEvent(testTopic,TestEvent{});
	namedBus.fireEvent(testTopic,TestEvent{});

	EXPECT_EQ(counter,5);

	namedBus.removeEvent<TestEvent>(testTopic);
}

TEST(NamedEventBus, canEventStoreState)
{
	const std::string testTopic{"/test/canEventStoreState"};
	
	int valueFromHandler{0}; 
	auto onTestEventWithData = [&valueFromHandler,&testTopic](std::string topic,TestEventWithData ev)
	{	
		EXPECT_TRUE(topic == testTopic);
		valueFromHandler = ev.data;
	};

	namedBus.registerEvent<TestEventWithData>(testTopic,onTestEventWithData);

	EXPECT_EQ(valueFromHandler,0);

	namedBus.fireEvent(testTopic,TestEventWithData{45});

	EXPECT_EQ(valueFromHandler,45);

	namedBus.removeEvent<TestEvent>(testTopic);
}

TEST(NamedEventBus, HandlerWithMultipleArgs)
{
	const std::string testTopic{"/test/HandlerWithMultipleArgs"};

	int intValueFromHandler{0};
	bool boolValueFromHandler{false};

	auto handlerWithArgs = [&intValueFromHandler,&boolValueFromHandler,&testTopic](std::string topic,TestEvent ev,int integer_arg,bool boolean_arg)
	{
		EXPECT_TRUE(topic == testTopic);
		intValueFromHandler = integer_arg;
		boolValueFromHandler = boolean_arg;
	};

	namedBus.registerEvent<TestEvent>(testTopic,handlerWithArgs,12,true);

	namedBus.fireEvent<TestEvent>(testTopic,TestEvent{});

	EXPECT_TRUE(boolValueFromHandler);
	EXPECT_EQ(intValueFromHandler,12);

	namedBus.removeEvent<TestEvent>(testTopic);

}