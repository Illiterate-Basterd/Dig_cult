#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#define N 31 // Matrix size N * N
#define INF 1e6

typedef struct edge
{
	int coord[2];
	int weight;
} Edge;

typedef struct Ostov
{
	int component;
} Ostov;

void quicksort(Edge *Arr, int first, int last);
void PrintMatr(int *Matr);
int DefineEdges(int edges, Ostov *OstMatr, int *OstovMatrix, Edge *ResultMatr, int *MadeEdges, int *weight_sum);

int main(void)
{
	int InpMatr[N * N];
	size_t edges = 0;
	Edge SortMatr[N * N];
	Ostov OstMatr[N];

	for (int i = 0; i < N * N; i++)
	{
		SortMatr[i].weight = INF;
	}

	for (size_t i = 0; i < N; i++)
	{
		OstMatr[i].component = i;
	}

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			scanf("%d", &InpMatr[i * N + j]);
			if (InpMatr[i * N + j] == 0)
				InpMatr[i * N + j] = INF;
			if (j > i)
			{
				SortMatr[i * N + j].coord[0] = i < j ? i : j;
				SortMatr[i * N + j].coord[1] = j > i ? j : i;
				SortMatr[i * N + j].weight = InpMatr[i * N + j];
				edges = InpMatr[i * N + j] == INF ? edges : edges + 1;
			}
		}
	}
	quicksort(SortMatr, 0, N * N - 1);
	printf("\n\n");

	Edge *ResultMatr = (Edge *)malloc(edges * sizeof(Edge));

	for (size_t i = 0; i < N * N; i++)
	{
		static size_t j = 0;
		if (SortMatr[i].weight != INF)
		{
			ResultMatr[j].weight = SortMatr[i].weight;
			ResultMatr[j].coord[0] = SortMatr[i].coord[0];
			ResultMatr[j].coord[1] = SortMatr[i].coord[1];
			j++;
		}
	}

	int OstovMatrix[N * N] = {0}, weight_sum = 0;
	int *MadeEdges = (int *)calloc(0, sizeof(int));

	int OstEdges = DefineEdges(edges, OstMatr, OstovMatrix, ResultMatr, MadeEdges, &weight_sum);

	printf("\n%d\n", weight_sum);
	PrintMatr(OstovMatrix);

	for (size_t i = 0; i < OstEdges; i += 2)
	{
		printf("(%d, %d) ", MadeEdges[i], MadeEdges[i + 1]);
	}

	free(MadeEdges);
	free(ResultMatr);
	return 0;
}

void quicksort(Edge *Arr, int first, int last)
{
	int i, j, pivot;
	int temp_w, temp_cd1, temp_cd2;
	if (first < last)
	{
		pivot = first;
		i = first;
		j = last;

		while (i < j)
		{
			while (Arr[i].weight <= Arr[pivot].weight && i < last)
				i++;
			while (Arr[j].weight > Arr[pivot].weight)
				j--;
			if (i < j)
			{
				temp_w = Arr[i].weight;
				temp_cd1 = Arr[i].coord[0];
				temp_cd2 = Arr[i].coord[1];
				Arr[i].weight = Arr[j].weight;
				Arr[i].coord[0] = Arr[j].coord[0];
				Arr[i].coord[1] = Arr[j].coord[1];
				Arr[j].weight = temp_w;
				Arr[j].coord[0] = temp_cd1;
				Arr[j].coord[1] = temp_cd2;
			}
		}
		temp_w = Arr[pivot].weight;
		temp_cd1 = Arr[pivot].coord[0];
		temp_cd2 = Arr[pivot].coord[1];
		Arr[pivot].weight = Arr[j].weight;
		Arr[pivot].coord[0] = Arr[j].coord[0];
		Arr[pivot].coord[1] = Arr[j].coord[1];
		Arr[j].weight = temp_w;
		Arr[j].coord[0] = temp_cd1;
		Arr[j].coord[1] = temp_cd2;
		quicksort(Arr, first, j - 1);
		quicksort(Arr, j + 1, last);
	}
}

void PrintMatr(int *Matr)
{
	for (size_t i = 0; i < N; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			if(j == N - 1)
				printf("%d", Matr[i * N + j]);
			else
				printf("%d, ", Matr[i * N + j]);
		}
		printf("\n");
	}
}

int DefineEdges(int edges, Ostov *OstMatr, int *OstovMatrix, Edge *ResultMatr, int *MadeEdges, int *weight_sum)
{
	int OstEdges = 0;
	for (size_t i = 0; i < edges; i++)
	{
		if (OstMatr[ResultMatr[i].coord[0]].component != OstMatr[ResultMatr[i].coord[1]].component)
		{
			OstovMatrix[ResultMatr[i].coord[0] * N + ResultMatr[i].coord[1]] = OstovMatrix[ResultMatr[i].coord[1] * N + ResultMatr[i].coord[0]] = 1;
			*weight_sum += ResultMatr[i].weight;
			OstEdges += 2;
			MadeEdges = (int*)realloc(MadeEdges, OstEdges * sizeof(int));
			MadeEdges[OstEdges - 2] = ResultMatr[i].coord[0] + 1;
			MadeEdges[OstEdges - 1] = ResultMatr[i].coord[1] + 1;
			unsigned short comp = OstMatr[ResultMatr[i].coord[0]].component;
			for (size_t j = 0; j < N; j++)
			{
				if (comp == OstMatr[j].component)
					OstMatr[j].component = OstMatr[ResultMatr[i].coord[1]].component;
			}
		}
	}
	return OstEdges;
}