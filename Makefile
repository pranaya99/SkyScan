sighting_search: sighting_search.o
	g++ -Wall -Werror -std=c++11 -o sighting_search sighting_search.o

sighting_search.o: sighting_search.cc
	g++ -Wall -Werror -std=c++11 -c sighting_search.cc

clean:
	rm -f sighting_search sighting_search.o *.dat

lint:
	/home/cs36cjp/public/cpplint/cpplint sighting_search.cc
