########################################################
# This Is A Compile Shell Script For Fire-Core Project #
########################################################

#####################################
# This is A Fire-Core-Based Project #
#####################################

docker build -t server_builder -f server_builder.DockerFile .
docker build -t server_runner -f server_runner.DockerFile .