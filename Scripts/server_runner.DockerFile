FROM ubuntu:bionic

RUN apt-get update && apt-get install -y git gcc g++ clang mysql-server libmysqld-dev cmake

ENTRYPOINT cd /home/workdir/bin && \
cp ../Src/LoginServer/LoginServer.conf ./ && \
cp ../Src/RouterServer/RouterServer.conf ./ && \
cp ../Src/WorldServer/WorldServer.conf ./ && \
./LoginServer