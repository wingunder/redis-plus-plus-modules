FROM debian:stable-slim

RUN apt-get -y update
RUN apt-get -y install git autoconf make g++ cmake

WORKDIR /usr/src
RUN git clone https://github.com/wingunder/redis-plus-plus-modules.git
WORKDIR /usr/src/redis-plus-plus-modules
RUN ./bootstrap.sh
RUN ./configure
RUN make -j8
CMD true
