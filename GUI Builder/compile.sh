# temporary command to compile the LostArt application.

g++ -std=c++11 -I /usr/include/eigen3/ -Wno-deprecated-register -Wno-parentheses -Wno-logical-op-parentheses geometry.cpp graphene.cpp "gui model.cpp" "linear system solving.cpp" main.cpp "symbolic computation.cpp" "symbol table.cpp" -lGL -lGLU -lGLEW -lglut -L/usr/lib/nvidia-331
