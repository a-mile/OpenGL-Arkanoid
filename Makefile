LIBS=-framework OpenGL -lglfw3 -lglew
FILES=main_file.cpp shaderprogram.cpp block.cpp

main_file: $(FILES)
	g++ -o arkanoid $(FILES)  $(LIBS)
