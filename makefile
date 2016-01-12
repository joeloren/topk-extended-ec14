all: topk topk-mallows topk-copeland

CFLAGS=-O2

topk: topk.o rules.o rand.o
topk-mallows: topk-mallows.o rules.o rand.o
topk-copeland: topk-copeland.o rand.o
topk.o: topk.c rules.h rand.h
topk-mallows.o: topk-mallows.c rules.h rand.h
topk-copeland.o: topk-copeland.c rand.h
rules.o: rules.c rules.h
rand.o: rand.c rand.h

clean:
	rm -f topk topk-mallows topk-copeland topk.o topk-mallows.o topk-copeland.o rules.o rand.o