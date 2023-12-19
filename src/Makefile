CC = g++ -g
CFLAGS = -Wall -Wextra -Werror -std=c++17 -pedantic-errors #-fsanitize=address
TEST_FLAGS = -lgtest_main -lgtest
GTEST_DIR = tests
SRC_DIR = src
TEST_DIR = tests
SRCS=$(wildcard s21_*.h)
OBJS=$(SRCS:.cc=.o)
SRCS2=$(wildcard tests/*.cc)
OBJS2=$(SRCS2:.cc=.o)
LIB_NAME=s21_matrix_oop.a
FORTEST =

# Check for Alpine Linux
ifeq ($(shell cat /etc/alpine-release 2>/dev/null),)
	# Not Alpine (assuming macOS or other Linux)
	# macOS
	ifeq ($(shell uname), Darwin)
		FORTEST += -DMACOS
	# Other Linux
	else
		FORTEST += -DLINUX
	endif
else
	# Alpine Linux
	FORTEST += -DALPINE
endif



all: test

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@

# $(LIB_NAME): $(OBJS)
# 	ar rcs $(LIB_NAME) $^
# 	rm -rf *.o

test: clean tests/*.cc *.h
	$(CC) $(TEST_FLAGS) $(CFLAGS) $(FORTEST) -DS21TEST tests/*.cc $(TST_LIBS) -o test -lpthread
	./$@

add_coverage_flag:
	$(eval CFLAGS += --coverage -fprofile-arcs -ftest-coverage)


gcov_report: clean add_coverage_flag test
	./test && \
	gcov */*.cc && \
	lcov --directory . --capture --output-file gcov_report.info --no-external --exclude '*/tests/*' --rc lcov_branch_coverage=1 && \
	genhtml gcov_report.info --output-directory report && \
	open report/index.html


clean:
	rm -rf *.a *.o test *.gcda *.gcno *.info report test.dSYM

leak: test
	leaks -atExit -- ./test

style:
	clang-format -n -style=Google *.cc *.h tests/*.cc

set-style-google:
	clang-format -i -style=Google *.cc *.h tests/*.cc

# Проверка памяти для Linux
valgrind: clean test
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --error-exitcode=1 --show-leak-kinds=all ./test
