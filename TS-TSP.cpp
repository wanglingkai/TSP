#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#define city 32
#define listsize 10
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
int CheckList(int a,int b,route indivisual,int list[listsize][4])
{
	int i,q,w,e,r;
	q = indivisual.route[a - 1];
	w = indivisual.route[b];
	e = indivisual.route[a];
	r = indivisual.route[b + 1];
	if (q > w)
		swap(&q, &w);
	if (e > r)
		swap(&e, &r);
	for (i = 0; i < listsize; i++)
	{
		if (list[i][0] == q && list[i][1] == w)
			return 1;
		if (list[i][2] == q && list[i][3] == w)
			return 1;
		if (list[i][0] == e && list[i][1] == r)
			return 1;
		if (list[i][2] == e && list[i][3] == r)
			return 1;
	}
	return 0;
}
void GetList(int a, int b, route indivisual, int list[listsize][4])
{
	int i;
	for (i = listsize - 1; i > 0; i--)
	{
		list[i][0] = list[i - 1][0];
		list[i][1] = list[i - 1][1];
		list[i][2] = list[i - 1][2];
		list[i][3] = list[i - 1][3];
	}
	if (indivisual.route[a - 1] > indivisual.route[a])
	{
		list[0][1] = indivisual.route[a - 1];
		list[0][0] = indivisual.route[a];
	}
	else
	{
		list[0][0] = indivisual.route[a - 1];
		list[0][1] = indivisual.route[a];
	}
	if (indivisual.route[b] > indivisual.route[b + 1])
	{
	list[0][3] = indivisual.route[b ];
	list[0][2] = indivisual.route[b+1 ];
	}
	else
	{
		list[0][2] = indivisual.route[b];
		list[0][3] = indivisual.route[b + 1];
	}
}
int main()
{
	FILE* fp;
	int n,i,j,in,rand1,rand2;
	int check;
	int list[listsize][4];
	char s[50];
	double sum = 0, BestDis = 60000;
	int Best[32],Temp[32];
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
	//initialize the list
	for(i=0;i<listsize;i++)
	{
		for (j = 0; j < 4; j++)
		{
			list[i][j] = 0;
		}
	}
	in = 0;
	while ( in < 1000)
	{
		for (i = 0; i < city; i++)
			Temp[i] = indivisual.route[i];
		rand1 = GetRandom(1,31);
		rand2 = GetRandom(1,31);
		if (rand1 > rand2)
			swap(&rand1, &rand2);
		check = CheckList(rand1,rand2,indivisual, list);
		if (check == 0)
		{
			GetList(rand1, rand2, indivisual, list);
			while (rand1 < rand2)
			{
				swap(&indivisual.route[rand1], &indivisual.route[rand2]);
				rand1++; rand2--;
			}
			indivisual.distance = GetDis(p, indivisual);

			if (indivisual.distance < BestDis)
			{
				BestDis = indivisual.distance;
				for (n = 0; n < 32; n++)
					Best[n] = indivisual.route[n];
			}
			else
				for (i = 0; i < city; i++)
					indivisual.route[i] = Temp[i];
			in++;
		}
	}
	printf("The best route is\n");
	for (int j = 0; j < city; j++)
	{
		printf("%d ", Best[j] + 1);
	}
	printf("\n");
	printf("Distance is%f\n", BestDis);

	return 0;
}