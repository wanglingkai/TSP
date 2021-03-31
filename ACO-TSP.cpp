#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define city 32
#define m 100//sum of ant
#define Tmax 10000
#define alpha 2
#define beta 5
#define rho 0.99
#define tau0 1 
#define Q 1

struct route
{
	int route[city];
	double distance;
}indivisual[m];
struct position
{
	int name;
	double x, y;
}p[city];
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

//get the distance of two point
double distance(position a, position b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

//get the total distance
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
/*double GetFit(double dis)
{
	return 1 / dis ;
}*/


/*void DownSort(route *in)
{
	int i, j;
	route tmp;
	for (i = 0; i < m; i++)
	{
		for (j = i + 1; j <m ; j++)
		{
			if (in[i].fitness < in[j].fitness)
			{
				tmp = in[i];
				in[i] = in[j];
				in[j] = tmp;
			}
		}
	}

}*/
double GetProbility(int i, int j, double tau[city][city], int* list)
{
	int l;
	double x, y=0;
	x = pow(tau[i][j], alpha) / pow(distance(p[i], p[j]),beta);
	for (l = 0; l < city; l++)
	{
		if (list[l] == 1)
			y += pow(tau[i][l], alpha) / pow(distance(p[i], p[l]), beta);
	}
	return x/y;
}


/*int Roulette(double P[city])
{
	double roulette,sum;
	int   y, rand;
	roulette = 0; sum = 0;
	//for (x = 0; x < city; x++)
	//	sum = sum + P[x];
	
	rand = GetRandom(0, 100);
	for(y=0;y<city;y++)
	{
		roulette +=P[y];
		if (roulette*100 > rand)
			break;
	}		
		return y;
	
*/
double GetDeltatau(route r[m],int i,int j)
{
	int a,b;
	double sum=0;
	for (a = 0; a < m; a++)
	{
		for (b = 0; b < city - 1; b++)
		{
			if ((r[a].route[b] == i && r[a].route[b + 1] == j) || (r[a].route[b] == j && r[a].route[b + 1] == i))
			{
				sum += Q / r[a].distance;
				break;
			}
		}
		if ((r[a].route[0] == i && r[a].route[city - 1] == j) || (r[a].route[city - 1] == i && r[a].route[0] == j))
		sum += Q / r[a].distance;
	}
	return sum;
}
int main()
{
	FILE* fp;
	int n,h, i, j,k, rand1;
	char s[50];
	double tau[city][city],SumP;
	double BestDis=600000;
	int J[city] ;//list of the city have not choiced
	double P[city];//probility of city choice
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
	//initialize start city

	for (i = 0; i < m; i++)
	{
		rand1 = GetRandom(0, city-1);
		indivisual[i].route[0] = rand1;	
	}
//initialize the pheromone
	for (i = 0; i < city; i++)
		for (j = 0; j < city; j++)
			tau[i][j] = tau0;
	for (n = 0; n < Tmax; n++)
	{
		for (i = 0; i < m; i++)
		{
			//for (j = 1; j < city; j++)
			//	indivisual[i].route[j] = 0;
			for (j = 0; j < city; j++)
				J[j] = 1;

			J[indivisual[i].route[0]] = 0;

			for (h = 1; h < city ; h++)//creat  route
			{
				rand1 = GetRandom(1, 100);
				SumP = 0;
				for (k = 0; k < city; k++)
				{
					
					if (J[k] != 0)
						P[k] = GetProbility(indivisual[i].route[h - 1], k, tau, J);
					else
						P[k] = 0;
					SumP += P[k];
					if (SumP * 100 > rand1)
						break;
				}
				indivisual[i].route[h] = k;
				J[k] = 0;
			}
			indivisual[i].distance = GetDis(p, indivisual[i]);
			//indivisual[i].fitness = GetFit(indivisual[i].distance);
			if (indivisual[i].distance < BestDis)
				BestDis = indivisual[i].distance;
		}
		for (i = 0; i < city; i++)//update tau
		{
			for (j = i+1; j < city; j++)
			{
				tau[i][j] = rho * tau[i][j] + Q/BestDis;
				tau[j][i] = tau[i][j];
			}
		}
		//DownSort(indivisual);
		printf("%d,%f\n",n ,BestDis);
		
	}			

	return 0;
}