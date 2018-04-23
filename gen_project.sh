#!/bin/sh

#Get current path
current_path=${pwd}

# Pass the project name from command line
project_name=$1

# The src directory. For contains source files
src="src"

# The include directory for placing header files
include="include"

# The lib directory for placing external libraries
lib="lib"

# The obj directory for place object files
obj="obj"

# The makefile
makefile="Makefile"

echo "Generate project ${project_name}"
mkdir ${project_name} && cd ${project_name} && touch ${makefile}

# Fill the makefile
cat > ${makefile} << EOF
EXE = ${project_name}

CC = mpicc

SRC_DIR = ${src}

OBJ_DIR = ${obj}

INC_DIR = ${include}

SRC = \$(wildcard \$(SRC_DIR)/*.c)

OBJ = \$(SRC:\$(SRC_DIR)/%.c=\$(OBJ_DIR)/%.o)

CPPFLAGS += -I\$(INC_DIR)

CFLAGS += -Wall -std=c99

LDFLAGS += -Llib

LDLIBS += -lm

# Default behavior of make
all: \$(EXE)

\$(EXE): \$(OBJ)
	\$(CC) \$(LDFLAGS) \$^ \$(LDLIBS) -o \$@

\$(OBJ_DIR)/%.o: \$(SRC_DIR)/%.c
	\$(CC) \$(CPPFLAGS) \$(CFLAGS) -c \$< -o \$@

clean:
	\$(RM) \$(OBJ)

.PHONY: all clean
EOF
mkdir ${src} ${include} ${lib} ${obj} && touch ./lib/.gitkeep && touch ./obj/.gitkeep
cd ./src && touch ${project_name}.c
echo "Inflating main"
cat > ${project_name}.c << EOF
#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char **argv){
  MPI_Init(&argc, &argv);
  // Insert code here
  MPI_Finalize();
  exit(EXIT_SUCCESS);
}
EOF
cd ../${include} && touch ${project_name}.h
echo "Finish Generation"
