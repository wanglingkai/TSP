
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define city 32
#define eta 0.997
#define maxT 10000
#define minT 50

struct route
{
	int route[city];
	double distance;
}indivisual;
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
int SimulatedAnnealing(double a, route b, int T)
{
	double compare, Q, r;

	compare = (a - b.distance)*10000;//compare the distance
	if (compare < 0)
	{
		Q = 1;
		return 1;
	}
	else
	{
		Q = exp((-compare) / T);
		r = GetRandom(0, 100);
		if (r/100 < Q) 
			return 1;
		else
			return 0;		
	}
}

int main()
{
	FILE* fp;
	int n, i, j, rand1, rand2,check;
	char s[50];
	double sum = 0,TemDis, T = maxT;
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
	//initialize the route
	for (i = 0; i < city; i++) {
		indivisual.route[i] = i;
	}
	indivisual.distance = GetDis(p, indivisual);
	
	while (T > minT)
	{
		rand1 = GetRandom(1, 31);
		rand2 = GetRandom(1, 31);
		swap(&indivisual.route[rand1], &indivisual.route[rand2]);
		TemDis = GetDis(p, indivisual);
		check=SimulatedAnnealing(TemDis, indivisual, T);//SA
		if(check==0)
			swap(&indivisual.route[rand1], &indivisual.route[rand2]);
		else
			indivisual.distance = TemDis;
		T = eta * T;//update temp
	}

	
	printf("The best route is\n");
	for (int j = 0; j < city; j++)
	{
		printf("%d ", indivisual.route[j] + 1);
	}
	printf("\n");
	printf("Distance is%f\n", indivisual.distance);
	return 0;
}