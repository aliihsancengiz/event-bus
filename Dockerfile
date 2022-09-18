FROM ubuntu:latest

RUN apt-get -y update && apt-get install -y

RUN apt-get -y install clang cmake

RUN apt-get -y install libgtest-dev

COPY . /usr/src/EventBus

WORKDIR /usr/src/EventBus

RUN scripts/build.sh -e -t 

CMD ["./build/tests/run_tests"]