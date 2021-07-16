########################################################
# This Is A Compile Shell Script For Fire-Core Project #
########################################################

#####################################
# This is A Fire-Core-Based Project #
#####################################

SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)
docker run --rm -v "${SHELL_FOLDER}/workdir:/home/workdir" server_builder
docker run --rm -it -v "${SHELL_FOLDER}/workdir:/home/workdir" server_runner