INCLUDE_PATH=C:/HPGCC-2.0-native-win32-SP2/include
LIBS_PATH=C:/HPGCC-2.0-native-win32-SP2/lib
ELF2HP_PATH=C:/HPGCC-2.0-native-win32-SP2/elf2hp
export CUR_DIR= $(shell pwd)
export CC= arm-elf-gcc
export C_FLAGS= -mtune=arm920t -mcpu=arm920t \
	-mlittle-endian -fomit-frame-pointer -msoft-float -Wall \
	-Os -I$(INCLUDE_PATH) -L$(LIBS_PATH) -mthumb-interwork -mthumb
export LD= arm-elf-ld
export LD_FLAGS= -L$(LIBS_PATH) -T VCld.script $(LIBS_PATH)/crt0.o
export LIBS= -lhpg -lhplib -lgcc
export AR= arm-elf-ar
export ELF2HP= elf2hp

OBJ = lista.o elemento.o nodo.o paquete.o quine.o tabla.o main.o

EXE = quinehp.exe

HP = $(SRC:%.c=%.hp)

all: quine.hp

install: all

clean:
	-rm -f *.o
	-rm -f *.hp
	-rm -f *.exe

lista.o: lista.c
	$(CC) $(C_FLAGS) -c $<

elemento.o: elemento.c
	$(CC) $(C_FLAGS) -c $<

nodo.o: nodo.c
	$(CC) $(C_FLAGS) -c $<

paquete.o: paquete.c
	$(CC) $(C_FLAGS) -c $<

quine.o: quine.c
	$(CC) $(C_FLAGS) -c $<

tabla.o: tabla.c
	$(CC) $(C_FLAGS) -c $<

main.o: main.c
	$(CC) $(C_FLAGS) -c $<

main.exe: $(OBJ)
	$(LD) $(LD_FLAGS) $(OBJ) $(LIBS) -o $@

quine.hp: main.exe
	$(ELF2HP) main.exe $@
