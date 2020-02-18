#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <time.h>

#define BUFFERSIZE 10
typedef struct Points {																// All the dots on the plane
	int x;
	int y;
} Points;

typedef struct Result {																// To compare and send the results across functions index of two points, coordinates of two points and the dstance between them
	int index_a;
	Points *a;
	int index_b;
	Points *b;
	double distance;
} Result;

//********************************************************************************************************************************//
//																																  //
// Starts with the number of points, then you choose the input method.  Our options is the manual input and random array.		  //
// In order to see the time that required by the recursive function i used the bruteforce approach to compare two distances.	  //
// Recursive function calls itself for the left and right of the median. It recursively gets smaller until the last 3 points.     //
// If it is less then 3 points, it uses bruteforce for the rest of the points.													  //
// We have to check the strip with the findClosestInStrip() function. It sortes the points by looking their y coordinates.		  //
// Then it returns minimum of left, right and the strip between left and right.													  //
//																																  //
//********************************************************************************************************************************//





Points** getArray(int n);															// Get input array from user
Points** randArray(int n, int size);												// Get random array
void printArray(Points** arr, int n);												// Print array to the screen
void swap(Points* a, Points* b);													// Swap two points
int partition(Points** arr, int low, int high,char type);							// Partition the array to two parts
void quickSort(Points** arr, int low, int high,char type);							// Sort the points in quicksort
double distance(Points* a, Points* b);												// Manhatttan distance between two points
Result* bruteForceFindDistance(Points** arr, int low, int high);					// Bruteforce function to find minimum distance between two points
Result* findClosestTwoPoints(Points** arr, int left, int right);					// To find the smallest distance in two parts recursively
Result* findClosestInStrip(Points** arr, int left, int right, Result* sigma);		// To find the smallest distance in the strip where two parts collide

int main(int argc, char** argv)
{
	int n, size, choice;															// n stands for number of points in the plane
	clock_t start, end;																// start and end times for calculating the cpu time that functions took
	double cpu_time_recursive, cpu_time_bruteforce;									// to compare two methods
	Result *resRec, *resBrute;														// result od two different methods that find the smallest distance between two points in 2d plane
	Points** pointarray;															// a is the point
	
	printf("\n# of points : ");
	scanf_s("%d", &n);
	printf("\nChoose one from the below. (1/2)\n1-) I will give the points.\n2-) I want the random numbers. \nchoice: ");
	scanf_s("%d", &choice);
	if (choice == 1) {
		pointarray = getArray(n);
	}
	else if (choice == 2) {
		printf("\nGive the size of point space.\nsize: ");
		scanf_s("%d",&size);
		pointarray =randArray(n, size);
	}
	else {
		printf("You haven't enter a valid choice");
		return 0;
	}
	
	printArray(pointarray, n);
	quickSort(pointarray, 0, n - 1, 'x');
	printArray(pointarray, n);
	start = clock();
	resBrute = bruteForceFindDistance(pointarray, 0, n-1);
	end = clock();
	cpu_time_bruteforce = ((double)(end - start)) / CLOCKS_PER_SEC;


	printf("\n\t------ recursive ------\n");
 	start = clock();
	resRec = findClosestTwoPoints(pointarray, 0, n - 1);
	end = clock();
	cpu_time_recursive = ((double)(end - start)) / CLOCKS_PER_SEC;
	/*2, 3
	12, 30
	40, 50
	5, 1
	12, 10
	3, 4*/
	printf("\n\t\t name       \t\t time\t\t         points      \t         distance \t");
	printf("\n\t\t bruteforce \t\t %lf \t\t a(%d,%d) - b(%d,%d) \t %lf \t", cpu_time_bruteforce, resBrute->a->x, resBrute->a->y, resBrute->b->x, resBrute->b->y, resBrute->distance);
	printf("\n\t\t recursive  \t\t %lf \t\t a(%d,%d) - b(%d,%d) \t %lf \t", cpu_time_recursive, resRec->a->x, resRec->a->y, resRec->b->x, resRec->b->y, resRec->distance);

	 
	return 0;
}
void printArray(Points** a, int n) {
	
	for (int i = 0; i < n; i++) {
		printf("\t\npoints[%d] = (%d,%d)", i, a[i]->x, a[i]->y);
	
	}

}
Points** getArray(int n){
	int i = 0;
	
	Points** pointArray = (Points **)malloc(n * sizeof(Points*));						// pointArray to get input points in x,y format

	printf("\npoint are supposed to be formatted as (x,y)\n");
	while (i<n) 
	{
		
		pointArray[i] = (Points*)malloc(sizeof(Points));
		printf("\npointArray[%d] = ",i);
		scanf_s(" %d,%d", &pointArray[i]->x, &pointArray[i]->y);		
		i++;
	}
	
	getchar();
	return pointArray;

}
Points** randArray(int n, int size) {
	int i = 0;
	Points** pointArray = (Points * *)malloc(n * sizeof(Points*));						// pointArray to create random array in given max size and given number of points
	srand(time(NULL));
	while (i < n)
	{
		pointArray[i] = (Points*)malloc(sizeof(Points));
		pointArray[i]->x = rand() % size;
		pointArray[i]->y = rand() % size;
		printf("\npointArray[%d] = (%d,%d)", i, pointArray[i]->x, pointArray[i]->y);
		i++;
	}
	
	getchar();
	return pointArray;


}	
void swap(Points* a, Points* b){														
	Points* tmpPoint = (Points*)malloc(sizeof(Points));
	tmpPoint->x = a->x; 
	tmpPoint->y = a->y;

	a->x = b->x;
	a->y = b->y;
	
	b->x = tmpPoint->x;
	b->y = tmpPoint->y;
}
int partition(Points **arr, int low, int high, char type){
	int pivot, i, j;



	if (type == 'x')																	//type x is to sort the points by looking their x values
	{
		pivot = arr[high]->x;    // pivot 
		i = (low - 1);  // Index of smaller element 

		for (j = low; j <= high - 1; j++)
		{
			// If current element is smaller than the pivot 
			if (arr[j]->x < pivot)
			{
				i++;    // increment index of smaller element 
				swap(arr[i], arr[j]);
			}
		}
		swap(arr[i + 1], arr[high]);
		return (i + 1);
	}
	else if (type == 'y')																// type y is to sort the points by looking their y values
	{
		
		pivot = arr[high]->y;    // pivot 
		i = (low - 1);  // Index of smaller element 

		for (j = low; j <= high - 1; j++)
		{
			// If current element is smaller than the pivot 
			if (arr[j]->y < pivot)
			{
				i++;    // increment index of smaller element 
				swap(arr[i], arr[j]);
			}
		}
		swap(arr[i + 1], arr[high]);
		return (i + 1);
	}
	return 0;
	
}
void quickSort(Points** arr, int low, int high, char type){								// sort the pointArrays by looking their type
	if (low < high)
	{
																						// pi is partitioning index, arr[p] is now at right place 
		int pi = partition(arr, low, high, type);
																						// Separately sort elements partition and after partition 
		quickSort(arr, low, pi - 1, type);
		quickSort(arr, pi + 1, high, type);
	}
}
double distance(Points* a, Points* b) {
	double res = 0.0;																	// result of manhatten distance between two given points
	res = sqrt((pow((double)(abs((a->x)-(b->x))),2) + pow((double)(abs((a->y) - (b->y))), 2)));
	return res;
}
Result* bruteForceFindDistance(Points** arr, int low, int high) {
	int i, j;
	Result *tempDist = (Result*)malloc(sizeof(Result));									// Result struct tempDist is for keeping the temp values to compare with the minimum
	Result *min = (Result*)malloc(sizeof(Result));										// to keep the minimum points, their indexes and the distance between them
	min->distance = DBL_MAX;															// initializing minimum distance with the max value to best compare 
	for (i = low; i < high; i++) {
		for (j = i + 1; j <= high;j++) {
			tempDist->a = arr[i];
			tempDist->b = arr[j];
			tempDist->index_a = i;
			tempDist->index_b = j;
			tempDist->distance = distance(arr[i], arr[j]);
			//printf("\na(%d,%d) - b(%d,%d) \t %lf \t", tempDist->a->x, tempDist->a->y, tempDist->b->x, tempDist->b->y, tempDist->distance);

			if (tempDist->distance < min->distance) {
				//printf("\n%lf < %lf", tempDist->distance, min->distance);
				min->a = tempDist->a;
				min->b = tempDist->b;
				min->index_a = tempDist->index_a;
				min->index_b = tempDist->index_a;
				min->distance = tempDist->distance;
			}
		}
		
	}
	//printf("\nmin: %lf",min->distance);
	return min;
}
Result* findClosestTwoPoints(Points** arr, int left, int right) {
	//printf("\n\t%d  -  %d ",left,right);
	/*printf("\n\n\n");
	for (int q = left; q <= right; q++) { 
		printf("\t%d,%d", arr[q]->x, arr[q]->y);
	}*/
	int len = (right - left + 1);
	int mid,left_of_strip, right_of_strip;
	Result* resLeft, * resRight, * sigma, * minDistanceStrip;							// result of left and right parts and sigma is the smallest of the two
	if (len <= 3) {
		return bruteForceFindDistance(arr, left, right);
	}
	mid = (right + left) / 2;

	resLeft  = findClosestTwoPoints(arr, left, mid);
	resRight = findClosestTwoPoints(arr, mid+1, right);
	
	sigma = (Result*)malloc(sizeof(Result));
	sigma->a = resLeft->a;
	sigma->b = resLeft->b;
	sigma->index_a = resLeft->index_a;
	sigma->index_b = resLeft->index_b;
	sigma->distance = resLeft->distance;


	if (resRight->distance < resLeft->distance) sigma = resRight;
	if (((mid - (int)sigma->distance) <= left) || (right_of_strip = (mid + (int)sigma->distance) >= right)) {
		left_of_strip = left;
		right_of_strip = right;
	}else {
		left_of_strip = (mid - (int)sigma->distance);
		right_of_strip= (mid + (int)sigma->distance);
	}


	//printf("\n\tstrip\t\n\t%d-%d\n", left_of_strip, right_of_strip);
	minDistanceStrip = findClosestInStrip(arr, left_of_strip, right_of_strip, sigma);

	//printf("\n\n minimum of left: %lf \t minimum of right: %lf\n\n", resLeft->distance, resRight->distance);

	if (minDistanceStrip->distance < sigma->distance) {
		sigma = minDistanceStrip;
	}
	return sigma;
}
Result* findClosestInStrip(Points** arr, int left, int right, Result* sigma) {
	//printf("\n\t -- strip -- \t\n");
	int i = 0, j = 0;

	Points** strip = (Points**)malloc(sizeof(Points*) * (right - left + 1));
	Result* minDist = (Result*)malloc(sizeof(Result));
	
	
	minDist = (Result*)malloc(sizeof(Result));
	minDist->a = sigma->a;
	minDist->b = sigma->b;
	minDist->index_a = sigma->index_a;
	minDist->index_b = sigma->index_b;
	minDist->distance = sigma->distance;





	for (i = 0; i < (right - left + 1); i++) {
		strip[i] = arr[left + i];
		//printf("(%d,%d)", strip[i]->x, strip[i]->y);
	}
	quickSort(strip, 0, (right - left), 'y');
	//printf("\n-- sorted strip --\n");
	//printArray(strip, (right - left+1));


	for (i = 0; i < (right - left + 1); i++) {
		for (j = i + 1; j < (right - left + 1); j++) {
			//printf("\n--\n");
			//printf("\nsigma: %lf\n difference of y's = %d", sigma->distance, (strip[j]->y - strip[i]->y));
			if ((strip[j]->y - strip[i]->y) < sigma->distance) {
				//printf("\ndistance: %lf", distance(strip[i], strip[j]));
				if (distance(strip[i], strip[j]) < sigma->distance) {
					
					minDist->a = strip[i];
					minDist->b = strip[j];
					minDist->index_a = i + left;
					minDist->index_b = j + left;
					minDist->distance = distance(strip[i], strip[j]);


					//printf("\na(%d,%d) - b(%d,%d) \t %lf \t", minDist->a->x, minDist->a->y, minDist->b->x, minDist->b->y, minDist->distance);
					//printf("\n--\n");
				}
			}
		}
	}
	//printf("\n\t min_strip: %lf",minDist->distance);
	return minDist;
}
