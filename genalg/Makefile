# Redefine the global variables
EXENAME	= genalg

# Compiler
CXX 		= gcc

# Preprocessor flags
CPPFLAGS 	=

# Compiler flags
CXXFLAGS 	= -std=gnu99 -Wall -fmessage-length=0

# Linker flags
LDFLAGS 	=

## FOLDERS ##

# Src folder
SRCFOLDER	:= src

# Obj folder
OBJFOLDER	:= obj

# Bin folder
BINFOLDER	:= bin

# Read sources list
SOURCES 	:= $(wildcard $(SRCFOLDER)/*.c)

# Generate objects list
#OBJS 		:= $(patsubst %.cpp, %.o, $(SOURCES))
OBJS		:= $(addprefix $(OBJFOLDER)/,$(notdir $(SOURCES:.c=.o)))


# A list of all needed special libraries
LIBS 		:= m
LIBS 		:= $(addprefix -l,$(LIBS))

# A list of paths to special libraries
INCLIBS		:= /home/pawel/.local/lib/
INCLIBS		:= $(addprefix -L,$(INCLIBS))
LDFLAGS 	+= $(INCLIBS)

# A list of all directories for including
#INCDIRS 	:= /usr/local/cuda/include
INCDIRS 	:=
INCDIRS 	:= $(addprefix -I,$(INCDIRS))

# A list of all macros
MACROS		:=
MACROS		:= $(addprefix -D,$(MACROS))

all: release

# Rule for compiling .cpp source files to .o object files
#.cpp.o:
$(OBJFOLDER)/%.o : $(SRCFOLDER)/%.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(MACROS) $(INCDIRS) -c $< -o $@

# Cleanup
clean:
	rm -fv $(OBJFOLDER)/*.o
	rm -fv Makefile.deps

cleanest: clean
	rm -fv $(BINFOLDER)/*

# Generate dependencies for all source files using -MM switch (-M lists all deps, including a lot of the system headers)
deps:
	$(CXX) -MM $(INCDIRS) $(SOURCES)  > Makefile.deps
	sed 's/\(^.*\.o:\)/$(echo $(OBJFOLDER) | sed -e 's/[]\/()$*.^|[]/\\&/g')\/\1/' < Makefile.deps > temp.deps
	mv temp.deps Makefile.deps


# Building depends on generated dependencies and all .o (object) files
build: deps $(OBJS)
	$(CXX) $(CXXFLAGS) $(MACROS) $(LDFLAGS) $(OBJS) $(LIBS) -o $(BINFOLDER)/$(EXENAME)

# Effectively, the default build target ('all' redirects here)
# Append compiler code optimalization
release: CXXFLAGS 	+= -O2
release: EXENAME 	:= release
release: build

# Debugging target, append max possible level (3rd) of adding
# debugging symbols to the output program
debug: CXXFLAGS += -g3
debug: MACROS	+= -DGENALG_DEBUG=1
debug: EXENAME 	:= debug
debug: build


ompdebug: CXXFLAGS 	+= -g3
ompdebug: CXXFLAGS	+= -fopenmp
ompdebug: MACROS	+= -DGENALG_DEBUG=1
ompdebug: EXENAME 	:= ompdebug
ompdebug: build

omprelease: CXXFLAGS 	+= -O2
omprelease: CXXFLAGS	+= -fopenmp
omprelease: EXENAME 	:= omprelease
omprelease: build

mpidebug: CXX		:= mpicc
mpidebug: CXXFLAGS 	+= -g3
mpidebug: MACROS	+= -DGENALG_DEBUG=1
mpidebug: EXENAME 	:= mpidebug
mpidebug: build

mpirelease: CXX			:= mpicc
omprelease: CXXFLAGS	+= -O2
mpirelease: EXENAME		:= mpirelease
mpirelease: build

echo:
	@echo CXX: $(CXX)
	@echo CPPFLAGS: $(CPPFLAGS)
	@echo CXXFLAGS: $(CXXFLAGS)
	@echo MACROS:	$(MACROS)
	@echo LDFLAGS: $(LDFLAGS)
	@echo SOURCES: $(SOURCES)
	@echo OBJS: $(OBJS)
	@echo LIBS: $(LIBS)

# Force all targets
.PHONY: all clean deps release debug omprelease ompdebug echo cleanest

# Include (ie. insert here literally) all the generated dependencies
# '-' prevents make from showing errors if the file doesn't exist (or is not readable)
-include Makefile.deps
