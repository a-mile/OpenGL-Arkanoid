LIBS=-framework OpenGL -lglfw3 -lglew
FILES=main_file.cpp shaderprogram.cpp lodepng.cpp objloader.cpp tools.cpp gameobject.cpp gameobjectvertices.cpp text2D.cpp

main_file: $(FILES)
	g++ -o arkanoid $(FILES)  $(LIBS)
