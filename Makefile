CC = clang
CFLAGS = -Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic
LFLAGS = 
EXEC1 = huff
EXEC2 = dehuff
T1 = nodetest
T2 = pqtest
T3 = brtest
T4 = bwtest
HEADERS = bitreader.h bitwriter.h node.h pq.h

all: $(T1) $(T2) $(T3) $(T4) $(EXEC1) $(EXEC2) 

$(T1): $(T1).o bitreader.o bitwriter.o node.o pq.o
	$(CC) $^ $(LFLAGS) -o $@ $(LDLIBS)

$(T2): $(T2).o bitreader.o bitwriter.o node.o pq.o
	$(CC) $^ $(LFLAGS) -o $@ $(LDLIBS)

$(T3): $(T3).o bitreader.o bitwriter.o node.o pq.o
	$(CC) $^ $(LFLAGS) -o $@ $(LDLIBS)

$(T4): $(T4).o bitreader.o bitwriter.o node.o pq.o
	$(CC) $^ $(LFLAGS) -o $@ $(LDLIBS)

$(EXEC1): $(EXEC1).o bitreader.o bitwriter.o node.o pq.o
	$(CC) $^ $(LFLAGS) -o $@ $(LDLIBS)

$(EXEC2): $(EXEC2).o bitreader.o bitwriter.o node.o pq.o
	$(CC) $^ $(LFLAGS) -o $@ $(LDLIBS)

clean:
	rm -rf $(EXEC1) $(EXEC2) $(T1) $(T2) $(T3) $(T4) *.o *.gch

format:
	clang-format -i -style=file *.[ch]

.PHONY: all clean format
