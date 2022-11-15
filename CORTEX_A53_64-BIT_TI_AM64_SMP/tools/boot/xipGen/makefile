
ifeq ($(OS),Windows_NT)
  EXE_FILE = xipGen.exe 
  RM=del
else
  EXE_FILE = xipGen.out
  RM=rm -f
endif

%.exe %.out: %.c
	gcc $< -o $@

all: $(EXE_FILE)

clean:
	$(RM) $(EXE_FILE)
	
	
