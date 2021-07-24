########################################################
# This Is A Compile Shell Script For Fire-Core Project #
########################################################

#####################################
# This is A Fire-Core-Based Project #
#####################################

SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)
docker run --rm -v "${SHELL_FOLDER}/../:/home/workdir" server_builder
docker run --rm -it -v "${SHELL_FOLDER}/../:/home/workdir" -p 19999:19999 -p 82:80 server_runner