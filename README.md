# Event Bus

Event bus facility to help decouple communication between modules,class,etc.

## Getting Started

Since is header only,just include  `EventBus.hpp` your project.

#### 1) Define your events

```
struct DataReceivedEvent{/*Add context here*/};
struct DataTransmittedEvent{/*Add context here*/};
```
#### 2) Define your handlers

```
void CommIf::onReceived(DataReceivedEvent ev){/*Process event here*/}
void CommIf::onTransmitted(DataTransmittedEvent ev){/*Process event here*/}
```

#### 3) Register events to bus with handlers

```
auto& bus = event_bus::EventBus::getInstance();
CommIf comm{/* */};

bus.registerEvent<DataReceivedEvent>(std::bind(&CommIf::onReceived, &comm std::placeholders::_1));
bus.registerEvent<DataTransmittedEvent>(std::bind(&CommIf::onTransmitted, &comm std::placeholders::_1));
```

#### 4) Fire events whenever you want to signal listener.

```
auto& bus = event_bus::EventBus::getInstance();

bus.fireEvent(DataReceivedEvent{/**/});
bus.fireEvent(DataTransmittedEvent{/**/});
```

### Prerequisites

No dependecy.

### Installing

```
mkdir build
cd build
cmake ..
make
make install
```

## Running the tests

[GoogleTest](https://github.com/google/googletest) is required to run tests.

```
mkdir build
cd build
cmake .. -DBUILD_TEST=ON
make
./build/tests/run_tests
```

## Running the examples

```
mkdir build
cd build
cmake .. --DBUILD_EXAMPLE=ON
make
./build/tests/run_tests
```

## Authors

* **Ali ihsan Cengiz** - *Software Engineer* - [aliihsancengiz](https://github.com/aliihsancengiz)
