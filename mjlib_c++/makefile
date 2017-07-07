all: gen_table gen_feng_table

hulib.o: hulib.h hulib.cpp
	g++ -g -c hulib.cpp

set_table.o: set_table.h set_table.cpp
	g++ -g -c set_table.cpp

table_mgr.o: table_mgr.h table_mgr.cpp
	g++ -g -c table_mgr.cpp

gen_table.o: hulib.h gen_table.cpp
	g++ -g -c gen_table.cpp

gen_table: gen_table.o hulib.o set_table.o table_mgr.o
	g++ gen_table.o hulib.o set_table.o table_mgr.o -o gen_table

gen_feng_table.o: hulib.h gen_feng_table.cpp
	g++ -g -c gen_feng_table.cpp

gen_feng_table: gen_feng_table.o hulib.o set_table.o table_mgr.o
	g++ gen_feng_table.o hulib.o set_table.o table_mgr.o -o gen_feng_table

test_hu.o: hulib.h test_hu.cpp
	g++ -g -c test_hu.cpp

test_hu: test_hu.o hulib.o set_table.o table_mgr.o
	g++ test_hu.o hulib.o set_table.o table_mgr.o -o test_hu

table: gen_table
	./gen_table

clean:
	rm *.o
	rm gen_table
	rm gen_feng_table
	rm test_hu
	rm *.data
	rm tbl/*
