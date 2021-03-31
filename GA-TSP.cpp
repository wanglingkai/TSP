#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define city 32
#define mutation 1
#define population 100
#define generation 10000

struct route
{
	int route[city];
	double distance, fitness;
}indivisual[population], MatingPool[population];
struct position
{
	int name;
	double x, y;
}p[32];
int GetRandom(int min, int max)
{
	int range = max - min;

	int random = rand() % range + min;
	return random;
}
//swap the position
void swap(int* a, int* b)
{
	int x;
	x = *a;
	*a = *b;
	*b = x;
}
//get the distance
double GetDis(position* p, route q)
{
	double a, b, sum = 0;
	for (int k = 0; k < city - 1; k++)
	{
		a = (p[q.route[k]].x - p[q.route[k + 1]].x) * (p[q.route[k]].x - p[q.route[k + 1]].x);
		b = (p[q.route[k]].y - p[q.route[k + 1]].y) * (p[q.route[k]].y - p[q.route[k + 1]].y);
		sum += sqrt(a + b);
	}
	a = (p[q.route[city - 1]].x - p[q.route[0]].x) * (p[q.route[city - 1]].x - p[q.route[0]].x);
	b = (p[q.route[city - 1]].y - p[q.route[0]].y) * (p[q.route[city - 1]].y - p[q.route[0]].y);
	sum += sqrt(a + b);
	return sum;
}
double GetFit(double dis)
{
	return 1 / dis * 1000000;
}

void GetMatingPool(route * indivisual)
{
	//roulette wheel
	//make the MatingPool
	double roulette, z;
	int a, b, x, y, rand;
	roulette = 0;
	//Get the sum of fitness
	for (x = 0; x < population / 2; x++)
		roulette = roulette + indivisual[x].fitness;

	for (a = 0; a < population; a++)
	{
		rand = GetRandom(0, roulette);
		z = indivisual[0].fitness; y = 0;
		while (z < rand)
		{
			y++;
			z = z + indivisual[y].fitness;
		}
		//for (y = 0, z = 0; z < rand; y++)
		//{
		//	z = z + indivisual[y].fitness ;
		//}
		MatingPool[a] = indivisual[y];
	}
}
void Crossover(route Mating[population])
{
	//PMX
	int n, cross, a, i;
	for (n = 0; n < population; n += 2)
	{
		cross = GetRandom(1, 31);
		for (a = 0; a < city; a++)
		{
			if (Mating[n].route[a] == Mating[n + 1].route[cross])
			{
				swap(&Mating[n].route[cross], &Mating[n].route[a]);
				break;
			}
		}
		for (a = 0; a < city; a++)
		{
			if (Mating[n + 1].route[a] == Mating[n].route[cross])
			{
				swap(&Mating[n + 1].route[cross], &Mating[n + 1].route[a]);
				break;
			}
		}
		cross = GetRandom(1, 31);
		for (a = 0; a < city; a++)
		{
			if (Mating[n].route[a] == Mating[n + 1].route[cross])
			{
				swap(&Mating[n].route[cross], &Mating[n].route[a]);
				break;
			}
		}
		for (a = 0; a < city; a++)
		{
			if (Mating[n + 1].route[a] == Mating[n].route[cross])
			{
				swap(&Mating[n + 1].route[cross], &Mating[n + 1].route[a]);
				break;
			}
		}
		for (i = 0; i < city; i++)
			indivisual[n].route[i] = Mating[n].route[i];
	}
	cross = GetRandom(1, 31);
	for (a = 0; a < city; a++)
	{
		if (Mating[population - 1].route[a] == Mating[0].route[cross])
		{
			swap(&Mating[population - 1].route[cross], &Mating[population - 1].route[a]);
			break;
		}
	}
	cross = GetRandom(1, 31);
	for (a = 0; a < city; a++)
	{
		if (Mating[population - 1].route[a] == Mating[0].route[cross])
		{
			swap(&Mating[population - 1].route[cross], &Mating[population - 1].route[a]);
			break;
		}
	}
	for (i = 0; i < city; i++)
		indivisual[population - 1].route[i] = Mating[population - 1].route[i];
}
void Mutation(route * MatingPool)
{
	int rand, rand1, rand2, i, j;
	for (j = 0; j < population * mutation / 100; j++)
	{
		rand = GetRandom(0, population - 1);
		rand1 = GetRandom(1, city - 1);
		rand2 = GetRandom(1, city - 1);
		swap(&MatingPool[rand].route[rand1], &MatingPool[rand].route[rand2]);
		for (i = 0; i < city; i++)
			indivisual[rand].route[i] = MatingPool[rand].route[i];
		rand1 = GetRandom(1, city - 1);
		rand2 = GetRandom(1, city - 1);
		swap(&MatingPool[rand].route[rand1], &MatingPool[rand].route[rand2]);
		for (i = 0; i < city; i++)
			indivisual[rand].route[i] = MatingPool[rand].route[i];
	}
}
void DownSort(route in[population])
{
	int i, j;
	route tmp;
	for (i = 0; i < population; i++)
	{
		for (j = i + 1; j < population; j++)
		{
			if (in[i].fitness < in[j].fitness)
			{
				tmp = in[i];
				in[i] = in[j];
				in[j] = tmp;
			}
		}
	}

}

int main()
{
	FILE* fp;
	int n, i, j, rand1;
	char s[50];
	double sum = 0;
	srand((unsigned)time(NULL));
	//open the file
	fp = fopen("city_position.csv", "r");
	if (fp == NULL)
	{
		printf("cant open\n");
		exit(0);
	}
	fscanf(fp, "%s\n", s);

	for (n = 0; n < city; n++)
	{
		fscanf(fp, "%d, %lf, %lf\n", &p[n].name, &p[n].x, &p[n].y);
	}
	fclose(fp);
	//initialize the population



	for (i = 0; i < population; i++)
	{
		rand1 = GetRandom(1, 31);
		indivisual[i].route[0] = 0;
		for (j = 1; j < city; j++)
		{
			indivisual[i].route[j] = rand1;
			rand1++;
			if (rand1 == 32)
				rand1 = 1;
		}
		indivisual[i].distance = GetDis(p, indivisual[i]);
		indivisual[i].fitness = GetFit(indivisual[i].distance);
	}
	//GA
	for (n = 0; n < generation; n++)
	{
		GetMatingPool(indivisual);
		Crossover(MatingPool);
		Mutation(MatingPool);
		for (i = 0; i < population; i++)
		{
			indivisual[i].distance = GetDis(p, indivisual[i]);
			indivisual[i].fitness = GetFit(indivisual[i].distance);
		}
		DownSort(indivisual);
	}
	printf("Best Distance is %f\n", indivisual[0].distance);
	printf("Best Route is ");
	for (n = 0; n < city; n++)
		printf("%d ", indivisual[0].route[n] + 1);



	return 0;
}