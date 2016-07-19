all: seq

seq: seq.cpp
	g++ -o $@ $< -I /home/ubuntu/739-wisckey/include -L /home/ubuntu/739-wisckey -lleveldb -lpthread -lsnappy

clean:
	rm -f seq

