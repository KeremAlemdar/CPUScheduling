#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct processData
{
	int arrivalTime;
	int burstLength;
	int finish;
	int turnAround;
	int waiting;
	int done;
	int summed;
	int index;
};
int seePrints = 0;
/* 
0 60
10 120
15 50
35 100
45 60
46 60 */
int FCFS(struct processData arr[1000], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (i == 0)
		{
			arr[i].waiting = 0;
			arr[i].turnAround = arr[i].burstLength + arr[i].waiting;
			arr[i].finish = arr[i].arrivalTime + arr[i].turnAround;
		}
		else
		{
			/* 
			arrival burst   finish 	turnaround waiting
			0 		60		60		60	  	   0
			10 		120 	180		170		   50
			*/
			if (arr[i].arrivalTime >= arr[i - 1].finish)
				arr[i].waiting = 0;
			else
				arr[i].waiting = arr[i - 1].finish - arr[i].arrivalTime;
			arr[i].turnAround = arr[i].burstLength + arr[i].waiting;
			arr[i].finish = arr[i].arrivalTime + arr[i].turnAround;
		}
	}
	int sumTurnAround = 0;
	if (seePrints == 1)
		printf("Arrival, Burst, Finish, TurnAround, Waiting\n");
	for (int i = 0; i < size; i++)
	{
		if (seePrints == 1)
			printf("%7d %5d %6d %10d %7d\n", arr[i].arrivalTime, arr[i].burstLength, arr[i].finish, arr[i].turnAround, arr[i].waiting);
		sumTurnAround = sumTurnAround + arr[i].turnAround;
		arr[i].waiting = 0;
		arr[i].finish = 0;
		arr[i].turnAround = 0;
	}
	int avg = sumTurnAround / size;
	if (sumTurnAround % size > (size / 2))
		avg = avg + 1;
	return avg;
}
int SJF(struct processData arr[1000], int size)
{
	int count = 0;
	int previousProcess;
	while (count < size)
	{
		if (count == 0)
		{
			arr[0].waiting = 0;
			arr[0].turnAround = arr[0].burstLength + arr[0].waiting;
			arr[0].finish = arr[0].arrivalTime + arr[0].turnAround;
			count++;
			arr[0].done = 1;
			previousProcess = 0;
		}
		else
		{
			// find min
			int m = 1;
			int minIndex = -1;
			int minBurst = INT_MAX;
			while ((arr[previousProcess].finish > arr[m].arrivalTime || arr[m].done == 1) && m < size)
			{
				if (minBurst > arr[m].burstLength && arr[m].done != 1)
				{
					minBurst = arr[m].burstLength;
					minIndex = m;
				}
				m++;
			}
			if (minIndex == -1)
			{
				arr[m].waiting = 0;
				arr[m].turnAround = arr[m].burstLength + arr[m].waiting;
				arr[m].finish = arr[m].arrivalTime + arr[m].turnAround;
				arr[m].done = 1;
				previousProcess = m;
				count++;
			}
			else
			{
				arr[minIndex].waiting = arr[previousProcess].finish - arr[minIndex].arrivalTime;
				arr[minIndex].turnAround = arr[minIndex].burstLength + arr[minIndex].waiting;
				arr[minIndex].finish = arr[minIndex].arrivalTime + arr[minIndex].turnAround;
				arr[minIndex].done = 1;
				previousProcess = minIndex;
				count++;
			}
			// find min end
		}
	}
	if (seePrints == 1)
		printf("Arrival, Burst, Finish, TurnAround, Waiting\n");
	int sumTurnAround = 0;
	for (int i = 0; i < size; i++)
	{
		if (seePrints == 1)
			printf("%7d %5d %6d %10d %7d\n", arr[i].arrivalTime, arr[i].burstLength, arr[i].finish, arr[i].turnAround, arr[i].waiting);
		sumTurnAround = sumTurnAround + arr[i].turnAround;
		arr[i].waiting = 0;
		arr[i].finish = 0;
		arr[i].turnAround = 0;
		arr[i].done = 0;
	}
	int avg = sumTurnAround / size;
	if (sumTurnAround % size > (size / 2))
		avg = avg + 1;
	return avg;
}
int SRTF(struct processData arr[1000], int size)
{
	int state[1000];
	/*
	state:
	1 = waiting
	2 = running
	*/
	int remainingTime = 0;
	int indexCounter = 0;
	int totalTime = 0;
	int currentRunningProcess = 0;
	// put first one
	remainingTime = arr[0].burstLength;
	totalTime = arr[0].burstLength;
	indexCounter++;
	state[0] = 2;
	for (int i = 0; i < totalTime; i++)
	{
		if (arr[indexCounter].arrivalTime > totalTime && i == totalTime - 1)
		{
			totalTime = arr[indexCounter].burstLength + arr[indexCounter].arrivalTime;
		}
		while (arr[indexCounter].arrivalTime == i)
		{
			totalTime = arr[indexCounter].burstLength + totalTime;
			indexCounter++;
		}
		// find min
		int m = 0;
		int minIndex = -1;
		int minBurst = INT_MAX;
		for (int m = 0; m < indexCounter; m++)
		{
			if (arr[m].burstLength - arr[m].turnAround < minBurst && arr[m].done == 0)
			{
				minBurst = arr[m].burstLength - arr[m].turnAround;
				minIndex = m;
			}
		}
		if (minIndex != currentRunningProcess)
		{
			if (arr[currentRunningProcess].done == 0)
			{
				state[currentRunningProcess] = 1;
			}
			state[minIndex] = 2;
			remainingTime = minBurst;
			currentRunningProcess = minIndex;
		}
		// end of find min
		arr[currentRunningProcess].turnAround++;
		if (arr[currentRunningProcess].turnAround == arr[currentRunningProcess].burstLength)
		{
			arr[currentRunningProcess].finish = i + 1;
			arr[currentRunningProcess].turnAround = arr[currentRunningProcess].finish - arr[currentRunningProcess].arrivalTime;
			arr[currentRunningProcess].waiting = arr[currentRunningProcess].turnAround - arr[currentRunningProcess].burstLength;
			arr[currentRunningProcess].done = 1;
			state[currentRunningProcess] = -1;
		}
	}
	if (seePrints == 1)
		printf("Arrival, Burst, Finish, TurnAround, Waiting\n");
	int sumTurnAround = 0;
	for (int i = 0; i < size; i++)
	{
		if (seePrints == 1)
			printf("%7d %5d %6d %10d %7d\n", arr[i].arrivalTime, arr[i].burstLength, arr[i].finish, arr[i].turnAround, arr[i].waiting);
		sumTurnAround = sumTurnAround + arr[i].turnAround;
		arr[i].waiting = 0;
		arr[i].finish = 0;
		arr[i].turnAround = 0;
		arr[i].done = 0;
	}
	int avg = sumTurnAround / size;
	if (sumTurnAround % size > (size / 2))
		avg = avg + 1;
	return avg;
}
int RR(struct processData arr[1000], int size, int quantum)
{
	int remaining[1000], queue[1000], prevQueue[1000];
	int count, totalTime, prevQueueSize, queueSize, delay = 0;
	int delayedInput[1000];
	prevQueue[0] = 0;
	prevQueueSize++;
	arr[0].done = 1;
	// copy burstLengths
	for (int i = 0; i < size; i++)
	{
		remaining[i] = arr[i].burstLength;
	}
	while (count < size && prevQueueSize > 0)
	{
		// printf("prev all queue start\n");
		// for (int o = prevQueueSize; o > 0; o--)
		// {
		// 	printf("prev queue all index = %d, proId = %d \n", o - 1, prevQueue[o - 1]);
		// }
		// printf("Prev queue START\n ");
		for (int i = prevQueueSize; i > 0; i--)
		{
			// printf("prev queue index = %d, proId = %d \n", i - 1, prevQueue[i - 1]);
			int newCycleLength = remaining[prevQueue[i - 1]] > quantum ? quantum : remaining[prevQueue[i - 1]];
			// printf("newCycle = %d\n", newCycleLength);
			for (int k = totalTime + 1; k <= totalTime + newCycleLength; k++)
			{
				// printf("k = %d\n", k);
				for (int m = 0; m < size; m++)
				{
					if ((k == arr[m].arrivalTime && arr[m].done == 0) && remaining[m] != 0)
					{
						if (k == totalTime + newCycleLength && (arr[m].index > arr[prevQueue[i - 1]].index))
						{
							// printf("process %d is delayed\n",m);
							delayedInput[delay++] = m;
						}
						else
						{
							// printf("process %d is added\n", m);
							queue[queueSize++] = m;
							arr[m].done == 1;
						}
					}
				}
			}
			totalTime = totalTime + newCycleLength;
			remaining[prevQueue[i - 1]] = remaining[prevQueue[i - 1]] - newCycleLength;
			if (remaining[prevQueue[i - 1]] == 0)
			{
				// printf("ENDEDDDDDDDDDDDDDDDDDDDDDDDDDDD\n");
				arr[prevQueue[i - 1]].finish = totalTime;
				arr[prevQueue[i - 1]].turnAround = arr[prevQueue[i - 1]].finish - arr[prevQueue[i - 1]].arrivalTime;
				arr[prevQueue[i - 1]].waiting = arr[prevQueue[i - 1]].turnAround - arr[prevQueue[i - 1]].burstLength;
				count++;
			}
			else
			{
				// printf("process %d is added\n", prevQueue[i - 1]);
				queue[queueSize++] = prevQueue[i - 1];
			}
			if (delay > 0)
			{
				for (int o = 0; o < delay; o++)
				{
					queue[queueSize++] = delayedInput[o];
				}
				delay = 0;
			}
		}
		// printf("end of queue \n");
		for (int l = 0; l < queueSize; l++)
		{
			// printf("index i = %d , value = %d\n", l, queue[l]);
			prevQueue[queueSize - l - 1] = queue[l];
		}
		prevQueueSize = queueSize;
		queueSize = 0;
		// printf("\n");
	}
	// find avg
	int sumTurnAround = 0;
	if (seePrints == 1)
		printf("RR Arrival, Burst, Finish, TurnAround, Waiting\n");
	for (int i = 0; i < size; i++)
	{
		if (seePrints == 1)
			printf("%9d %5d %6d %10d %7d\n", arr[i].arrivalTime, arr[i].burstLength, arr[i].finish, arr[i].turnAround, arr[i].waiting);
		sumTurnAround = sumTurnAround + arr[i].turnAround;
		arr[i].waiting = 0;
		arr[i].finish = 0;
		arr[i].turnAround = 0;
		arr[i].done = 0;
	}
	int avg = sumTurnAround / size;
	if (sumTurnAround % size > (size / 2))
		avg = avg + 1;
	return avg;
}
int RRFINAL(struct processData arr[1000], int size, int quantum)
{

	int remaining[1000];
	int count = 0;
	int totalTime = 0;

	int queue[1000];
	int queueSize = 0;

	int prevQueue[1000];
	int prevQueueSize = 0;
	prevQueue[0] = 0;
	prevQueueSize++;
	arr[0].done = 1;
	int delay = 0;
	int delayedInput = 0;

	for (int i = 0; i < size; i++)
	{
		remaining[i] = arr[i].burstLength;
	}
	//////////////////////////////

	while (count < size && prevQueueSize > 0)
	{
		printf("prev all queue start\n");
		for (int o = prevQueueSize; o > 0; o--)
		{
			printf("prev queue all index = %d, proId = %d \n", o - 1, prevQueue[o - 1]);
		}
		printf("Prev queue START\n ");
		for (int i = prevQueueSize; i > 0; i--)
		{
			printf("prev queue index = %d, proId = %d \n", i - 1, prevQueue[i - 1]);
			int newCycleLength = remaining[prevQueue[i - 1]] > quantum ? quantum : remaining[prevQueue[i - 1]];
			printf("newCycle = %d\n", newCycleLength);
			for (int k = totalTime + 1; k <= totalTime + newCycleLength; k++)
			{
				printf("k = %d\n", k);
				for (int m = 0; m < size; m++)
				{
					if ((k == arr[m].arrivalTime && arr[m].done == 0) && remaining[m] != 0)
					{
						if (k == totalTime + newCycleLength && (arr[m].index > arr[prevQueue[i - 1]].index))
						{
							delay = 1;
							delayedInput = m;
						}
						else
						{
							printf("process %d is added\n", m);
							queue[queueSize++] = m;
							arr[m].done == 1;
						}
					}
				}
			}
			totalTime = totalTime + newCycleLength;
			remaining[prevQueue[i - 1]] = remaining[prevQueue[i - 1]] - newCycleLength;
			if (remaining[prevQueue[i - 1]] == 0)
			{
				printf("ENDEDDDDDDDDDDDDDDDDDDDDDDDDDDD\n");
				arr[prevQueue[i - 1]].finish = totalTime;
				arr[prevQueue[i - 1]].turnAround = arr[prevQueue[i - 1]].finish - arr[prevQueue[i - 1]].arrivalTime;
				arr[prevQueue[i - 1]].waiting = arr[prevQueue[i - 1]].turnAround - arr[prevQueue[i - 1]].burstLength;
				count++;
			}
			else
			{
				printf("process %d is added\n", prevQueue[i - 1]);
				queue[queueSize++] = prevQueue[i - 1];
			}
			if (delay == 1)
			{
				queue[queueSize++] = delayedInput;
				delay = 0;
			}
		}

		printf("end of queue \n");
		for (int l = 0; l < queueSize; l++)
		{
			printf("index i = %d , value = %d\n", l, queue[l]);
			prevQueue[queueSize - l - 1] = queue[l];
		}
		prevQueueSize = queueSize;
		queueSize = 0;
		printf("\n");
	}

	printf("RR Arrival, Burst, Finish, TurnAround, Waiting\n");
	int sumTurnAround = 0;
	for (int i = 0; i < size; i++)
	{
		printf("%9d %5d %6d %10d %7d\n", arr[i].arrivalTime, arr[i].burstLength, arr[i].finish, arr[i].turnAround, arr[i].waiting);
		sumTurnAround = sumTurnAround + arr[i].turnAround;
		arr[i].waiting = 0;
		arr[i].finish = 0;
		arr[i].turnAround = 0;
		arr[i].done = 0;
	}
	int avg = sumTurnAround / size;
	if (sumTurnAround % size > (size / 2))
		avg = avg + 1;
	return avg;
}
int main(int argc, char *argv[])
{
	// argc = 2
	// argv[0] = ./schedule
	// argv[1] = input file name
	// argv[2] = quantum
	int burstLength;
	int arrivalTime;
	int index;
	char *filename = argv[1];
	int quantum = atoi(argv[2]);
	int answer = -1;

	struct processData arr[1000];
	int i = 0;
	int size = -1;

	FILE *file;
	file = fopen(filename, "r");
	if (file == NULL)
	{
		perror("Error in file opening\n");
		exit(1);
	}

	while (!feof(file))
	{
		fscanf(file, "%d\n%d\n%d\n", &index, &arrivalTime, &burstLength);
		arr[i].index = index;
		arr[i].arrivalTime = arrivalTime;
		arr[i].burstLength = burstLength;
		i++;
	}
	fclose(file);
	size = i; // size is correct here
	int averageTurnAroundFCFS = FCFS(arr, size);
	printf("FCFS = %d\n", averageTurnAroundFCFS);

	int averageTurnAroundSJF = SJF(arr, size);
	printf("SJF = %d\n", averageTurnAroundSJF);

	int averageTurnAroundSRTF = SRTF(arr, size);
	printf("SRTF = %d\n", averageTurnAroundSRTF);

	int averageTurnAroundRR = RR(arr, size, quantum);
	printf("RR = %d\n", averageTurnAroundRR);

	return 0;
}
