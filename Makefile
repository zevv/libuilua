
# Set this to your lua version

LUAVER	:= 5.2

CFLAGS	+= -g
CFLAGS	+= -Wall
CFLAGS	+= -shared -fPIC
CFLAGS	+= $(shell pkg-config --cflags --libs lua-$(LUAVER))

LDFLAGS	+= -lui

libuilua.so: libuilua.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -f libuilua.so
