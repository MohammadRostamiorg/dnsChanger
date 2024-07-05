CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lpthread
TARGET = dnsch
SRCS = dnsch.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

install: $(TARGET)
	install -Dm755 $(TARGET) $(DESTDIR)/usr/bin/$(TARGET)
	install -Dm644 dnsch.1 $(DESTDIR)/usr/share/man/man1/dnsch.1
	install -Dm644 LICENSE $(DESTDIR)/usr/share/licenses/dnsch/LICENSE

uninstall:
	rm -f $(DESTDIR)/usr/bin/$(TARGET)
	rm -f $(DESTDIR)/usr/share/man/man1/dnsch.1
	rm -rf $(DESTDIR)/usr/share/licenses/dnsch
