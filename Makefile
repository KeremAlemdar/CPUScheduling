all: schedule

scheduler: schedule.c
	gcc -Wall -o schedule schedule.c -lrt

clean:
	rm -fr *~ schedule
