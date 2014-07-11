LIBNICE_H = -I third_party/libnice/agent/
LIBNICE_LIBS = dist/deps/libnice/*.o
GLIB_H = $(shell pkg-config --cflags gobject-2.0 gio-2.0 gthread-2.0)
GLIB_LIBS = $(shell pkg-config --libs gobject-2.0 gio-2.0 gthread-2.0)
GTEST_FLAGS = -I third_party/google-test/include/ third_party/google-test/make/gtest-all.o
JSON_H = -I third_party/parson/
JSON_LIB = dist/deps/parson.o

CPP_FILES := $(wildcard src/*.cc)
OBJ_FILES := $(addprefix dist/, $(notdir $(CPP_FILES:.cc=.o)))

all: update libnice parson libpeerconnection.a 

update:
	git submodule update --init

libnice:
ifeq ($(CI), true)
	cd third_party/libnice/; ./autogen.sh; make
else
	cd third_party/libnice/; ./configure; make
endif
	rm -rf dist/deps/libnice; mkdir -p dist/deps/libnice; cp third_party/libnice/agent/.libs/libagent.a dist/deps/libnice; cd dist/deps/libnice; ar x libagent.a

parson:
	mkdir -p dist/deps
	$(CC) -c -o dist/deps/parson.o third_party/parson/parson.c

dist/%.o: src/%.cc
	mkdir -p dist
	$(CXX) -c -o $@ $< $(JSON_H) $(LIBNICE_H) $(GLIB_H)

libpeerconnection.a: $(OBJ_FILES)
	ar rvs dist/$@ $^ $(LIBNICE_LIBS) $(JSON_LIB)

test_suite:
	git clone https://github.com/alandipert/fswatch.git third_party/fswatch
	cd third_party/fswatch/; ./autogen.sh; ./configure; make
	svn checkout http://googletest.googlecode.com/svn/trunk/ third_party/google-test; cd third_party/google-test; svn up -r690
	cd third_party/google-test/make/; make;

test: libpeerconnection.a
	$(CXX) -std=c++11 -o dist/run_tests test/main.cc dist/libpeerconnection.a $(GTEST_FLAGS) $(JSON_H) $(LIBNICE_H) $(GLIB_H) $(GLIB_LIBS)
	./dist/run_tests

develop:
	third_party/fswatch/fswatch -0 src test |  while read -d "" event; do clear; echo "Changed file: $${event}"; make test; done

clean:
	rm -rvf dist

.PHONY: update libnice parson libpeerconnection.a test_suite test develop clean
