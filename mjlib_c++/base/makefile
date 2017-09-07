all: gen_table test_hu table

hulib.o: hulib.h hulib.cpp
	g++ -g -c hulib.cpp

test_hu.o: hulib.h test_hu.cpp
	g++ -g -c test_hu.cpp

gen_table.o: hulib.h gen_table.cpp
	g++ -g -c gen_table.cpp

wave_table.o: wave_table.h wave_table.cpp
	g++ -g -c wave_table.cpp

wave_eye_table.o: wave_eye_table.h wave_eye_table.cpp
	g++ -g -c wave_eye_table.cpp

gen_table: gen_table.o hulib.o wave_table.o wave_eye_table.o
	g++ gen_table.o hulib.o wave_table.o wave_eye_table.o -o gen_table

test_hu: test_hu.o hulib.o wave_table.o wave_eye_table.o
	g++ test_hu.o hulib.o wave_table.o wave_eye_table.o -o test_hu

table: gen_table
	./gen_table

clean:
	rm *.o
	rm gen_table
	rm test_hu
	rm *.data
