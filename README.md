# CPUScheduling

This project is a CPU scheduling simulator.
Simulated scheduling algorithms are:
FCFS (First Come First Serve)
SJF (Shortest Job First)
SRTF (Shortest Remaining Time First)
RR(q) (Round Robin)

For each burst, data set includes burst number, the arrival time and burst length. For example:
1 0 60

2 10 120

3 15 50

4 35 100

...

How to run:
./schedule <inputfile><quantum>

<quantum> is the time quantum used in RR scheduling.

Output includes Arrival time, Burst lenght, Finish, TurnAround time and Waiting time for each process and for each scheduling algorithm and average turnaround of the scheduling algorithms.
