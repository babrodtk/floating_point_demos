SRCS=$(wildcard src/test_*.cpp)
OUTPUTS=$(patsubst src/%.cpp, bin/%, $(SRCS))


ALL: $(OUTPUTS)

clean:
	rm -f $(OUTPUTS)

$(OUTPUTS): bin/%: src/%.cpp
	mkdir -p bin
	g++ -O3 -o $@ -lrt $<
