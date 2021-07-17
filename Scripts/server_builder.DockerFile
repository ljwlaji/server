FROM ubuntu:bionic

RUN apt-get update && apt-get install -y git gcc g++ clang mysql-server libmysqld-dev cmake

ENTRYPOINT cd /home/workdir && rm -rf build && \
rm -rf bin && \
mkdir build && \
cd build && \
cmake ../ && \
make