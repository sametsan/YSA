SRC		= $(PWD)
BD		= $(PWD)
CXX		= gcc

MAIN_FILES	= training.c ysa.c image.c
RUN_FILES	= run.c ysa.c image.c
CONVERT_FILES =  convert.c image.c

LINKS		= -lm

MAIN_OUTPUT	= training
RUN_OUTPUT	= run
CONVERT_OUTPUT	= convert


all:    

	$(CXX)  --debug -Wall $(CONVERT_FILES) -o $(CONVERT_OUTPUT) $(LINKS) 
	$(CXX)  --debug -Wall $(MAIN_FILES) -o $(MAIN_OUTPUT) $(LINKS) 
	$(CXX)  --debug -Wall  $(RUN_FILES) -o $(RUN_OUTPUT)  $(LINKS)

clean:	
	rm $(MAIN_OUTPUT) 
	rm $(RUN_OUTPUT) 
install:
