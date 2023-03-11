TESTS=\
	slide_binary_util-test

all: $(TESTS)

%: %.cpp
	$(CXX) -Wall $< `pkg-config --cflags --libs gtest` -o $@
	./$@

clean:
	rm -rf *-test
