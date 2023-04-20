CC = gcc
CFLAGS = -w
OBJFILES = distance_vector.o node0.o node1.o node2.o node3.o
TARGET = exec

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~