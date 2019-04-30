########################################################
# This Is A Compile Shell Script For Fire-Core Project #
########################################################

#####################################
# This is A Fire-Core-Based Project #
#####################################

rm -rf build && \
mkdir build && \
cd build && \
cmake ../ && \
make && \
cd ../bin && \
# Run Login Server
./LoginServer
# Run Router Server
# ./RouterServer