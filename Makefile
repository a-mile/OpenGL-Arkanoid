LIBS=-framework OpenGL -lglfw3 -lglew
FILES=shader.cpp main_file.cpp

main_file: $(FILES)
	g++ -o arkanoid $(FILES)  $(LIBS)
