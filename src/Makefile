CC:=gcc
CFLAGS:=-Wall -Werror -Wextra -std=c11 -pedantic

CAT_PATH:=./cat
GREP_PATH:=./grep
CAT:=s21_cat
GREP:=s21_grep

LDFLAGS:=-lm
ASAN:=-g -fsanitize=address

SRCS_CAT:=$(shell find $(CAT_PATH) -name "*.c")
OBJS_CAT:=$(SRCS_CAT:%.c=%.o)
SRCS_CAT_H:=$(shell find $(CAT_PATH) -name "*.h")

SRCS_GREP:=$(shell find $(GREP_PATH) -name "*.c")
OBJS_GREP:=$(SRCS_GREP:%.c=%.o)
SRCS_GREP_H:=$(shell find $(GREP_PATH) -name "*.h")

FORMAT_FILES_CAT := $(shell find $(CAT_PATH) -name "*.c" -o -name "*.h")
FORMAT_FILES_GREP := $(shell find $(GREP_PATH) -name "*.c" -o -name "*.h")


all:format $(CAT) $(GREP)

format: $(FORMAT_FILES_CAT) $(FORMAT_FILES_GREP)
	clang-format -style=file -n $(FORMAT_FILES_CAT) $(FORMAT_FILES_GREP)

$(CAT): $(OBJS_CAT) $(SRCS_CAT_H)
	$(CC) $(OBJS_CAT) $(CFLAGS) $(ASAN) -o $@ $(LDFLAGS)
	
$(CAT_PATH)/%.o: %.c $(SRCS_CAT_H)
	$(CC) $(CFLAGS) $(ASAN) -c $< -o $@

$(GREP): $(OBJS_GREP) $(SRCS_GREP_H)
	$(CC) $(OBJS_GREP) $(CFLAGS) $(ASAN) -o $@ $(LDFLAGS)

$(GREP_PATH)/%.o: %.c $(SRCS_GREP_H)
	$(CC) $(CFLAGS) $(ASAN) -c $< -o $@

clean:
	rm $(OBJS_CAT) $(CAT)
	rm $(OBJS_GREP) $(GREP)

rebuild: clean all

.PHONY: all clean format rebuild
