# Time-stamp: <Wed Jun 02 2021 22:39:39 hwloidl>

# -----------------------------------------------------------------------------
# MAIN part of the makefile

# C compiler (either native or as cross compiler)
CC=gcc
AS=as

PRG=crack_rot
FILEIN=filein.txt

all: $(PRG)

os:
	@echo "You are running on this operating system: $(OS)"

$(PRG): $(PRG).c
	$(CROSS)$(CC) -o $@ $^

# $(PRG)_asm: $(PRG).c rotN.s showIntCConv.s
# 	$(CROSS)$(CC) -c -DASM -o $(PRG)_asm.o $(PRG).c
# 	$(CROSS)$(AS) -o showIntCConv.o showIntCConv.s
# 	$(CROSS)$(AS) -o rotN.o rotN.s
# 	$(CROSS)$(CC) -o $@ $(PRG)_asm.o rotN.o showIntCConv.o

run:
	$(EXEC)$(PRG)

test:
	$(EXEC)$(PRG) -f $(FILEIN)

clean:
	-rm $(PRG) *.o

