FROM ubuntu:22.04

RUN apt update && apt install -y \
    build-essential \
    wget \
    git \
    vim \
    cmake

# Install SystemC
WORKDIR /opt
RUN wget https://www.accellera.org/images/downloads/standards/systemc/systemc-2.3.3.tar.gz \
    && tar -xzf systemc-2.3.3.tar.gz \
    && cd systemc-2.3.3 \
    && mkdir objdir \
    && cd objdir \
    && ../configure --prefix=/usr/local/systemc \
    && make -j4 \
    && make install

ENV SYSTEMC=/usr/local/systemc
ENV LD_LIBRARY_PATH=/usr/local/systemc/lib-linux64

