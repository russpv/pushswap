NAME = push_swap
NAME2 = checker

OUTPUT = executable

SOURCES = main.c solver.c utils.c quicksort.c \
		  state_.c state_checks.c state_moves.c state_moves_2.c state_utils.c \
		  stack_.c stack_checks.c stack_getters.c stack_getters_2.c stack_moves.c \
		  	stack_peeks.c stack_pushpop.c stack_search.c stack_utils.c stack_setters.c \
		  partition_.c partition_getters.c partition_getters_2.c partition_setters.c \
		  greedysort_.c greedysort_abs.c greedysort_findalts.c greedysort_getmoves.c \
		  	greedysort_search.c          
BONUS_SOURCES = ./tester/checker.c

BONUS_OBJECTS = $(BONUS_SOURCES:.c=.o) $(filter-out main.o, $(SOURCES:.c=.o))
OBJECTS = $(SOURCES:.c=.o)

# Colors
YELLOW = \033[33m
GREEN = \033[32m
RESET = \033[0m
BOLD = \033[1m

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -L$(LIB_DIR) -lft
LDFLAGS_SO = -L$(LIB_DIR) -lft -Wl,-rpath,$(LIB_DIR)

LIB_CPDIR = .
LIB_DIR = libft
LIB_NAME = libft.a
LIB_NAME_SO = libft.so
LIB_PATH = $(LIB_DIR)/$(LIB_NAME)
LIB_PATH_SO = $(LIB_DIR)/$(LIB_NAME_SO)

# Targets
all: $(NAME)

$(NAME): $(LIB_PATH) $(OBJECTS)
	@echo "Creating $(NAME) $(OUTPUT)..."
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)
	chmod +x $@
	@echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"
	@echo "$(YELLOW)Created: $(words $(OBJECTS) ) object file(s)$(RESET)"
	@echo "$(YELLOW)Created: $(NAME)$(RESET)"

bonus: $(NAME2)

$(NAME2): $(LIB_PATH) $(BONUS_OBJECTS)

	@echo "Creating $(NAME2) $(OUTPUT)..."
	$(CC) $^ -o $@ $(CFLAGS)
	chmod +x $@
	-@touch .bonus_made
	@echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"
	@echo "$(YELLOW)Created: $(words $(BONUS_OBJECTS)) object file(s)$(RESET)"
	@echo "$(YELLOW)Created: $(NAME2)$(RESET)"

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ #Initial compilation of .c files

# make shared object lib
$(LIB_PATH):
	@$(MAKE) -C $(LIB_DIR)
	@echo "Copying $(LIB_NAME) to ../"
	@cp $(LIB_PATH) .

clean:
	rm -f $(OBJECTS) $(BONUS_OBJECTS) 
	@$(MAKE) -C $(LIB_DIR) clean
	rm -f $(LIB_NAME) # don't delete so!
	-@rm -f .bonus_made
	@echo "$(GREEN)$(BOLD)SUCCESS$(RESET)"
	@echo "$(YELLOW) Deleted: $(words $(OBJECTS) $(BONUS_OBJECTS)) object file(s)$(RESET)"

fclean: clean
	rm -f $(NAME)
	rm -f $(LIB_PATH)
	rm -f $(LIB_NAME)
	rm -f $(LIB_NAME_SO)
	@echo "$(GREEN)SUCCESS$(RESET)"
	@echo "$(YELLOW) Deleted $(words $(NAME)) object files(s)$(RESET)"
	@echo "$(YELLOW) Deleted: $(NAME)"

re: fclean all

so:

	$(CC) -fPIC $(CFLAGS) -c $(SOURCES) $(BONUS_SOURCES)
	$(CC) -nostartfiles -shared -o $(NAME).so $(OBJECTS) $(BONUS_OBJECTS)

.PHONY: all bonus clean fclean re so
