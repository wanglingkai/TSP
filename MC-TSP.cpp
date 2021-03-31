#define city 32
#include <math.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
struct route
{
	int route[city];
	double distance;
};
 struct position
{
	int name;
	double x, y;
};
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
	 for (int k = 0; k < city-1; k++)
	 {
		 a = (p[q.route[k]].x - p[q.route[k + 1]].x) * (p[q.route[k]].x - p[q.route[k + 1]].x);
		 b = (p[q.route[k]].y - p[q.route[k + 1]].y) * (p[q.route[k]].y - p[q.route[k + 1]].y);
		 sum += sqrt(a + b);
	 }
	 a = (p[q.route[city-1]].x - p[q.route[0]].x) * (p[q.route[city-1]].x - p[q.route[0]].x);
	 b = (p[q.route[city-1]].y - p[q.route[0]].y) * (p[q.route[city-1]].y - p[q.route[0]].y);
	 sum += sqrt(a + b);
	 return sum;
 }

 int main()
 {
	 FILE* fp;
	 struct route indivisual;
	 struct position p[city];
	 char s[50];
	 int n, i, rand1, rand2;
	 double sum = 0, BestDis=60000;
	 int Best[city] = { 0 };
	 srand((unsigned)time(NULL));
	 //open the file
	 fp = fopen("city_position.csv", "r");
	 if (fp == NULL)
	 {
		 printf("cant oprn\n");
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
	 indivisual.distance = GetDis(p,indivisual);
	 //monte carlo method
	 for (int in = 0; in < 1000; in++) 
	 {
		 rand1 = GetRandom(1, 31);
		 rand2 = GetRandom(1, 31);
         swap(&indivisual.route[rand1], &indivisual.route[rand2]);
		 indivisual.distance = GetDis(p, indivisual);

		 if (indivisual.distance < BestDis)
		 {
			 BestDis = indivisual.distance;
			 for (n = 0; n < city; n++)
				 Best[n] = indivisual.route[n];
		 }
	 }
	 printf("The best route is\n");
	 for (int j = 0; j < city; j++) 
	 {
		 printf("%d ", Best[j]+1);
	 }
	 printf("\n");
	 printf("Distance is%f\n", BestDis);

	return 0;
}