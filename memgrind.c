#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include "mymalloc.h"

struct timeval begin; //start time
struct timeval end; //end time

int main(int argc, char** argv){

/*Test Case A. */
int i; //for loop counters
int j;
double totalTime1=0; //total time it takes to run the loop


for (j=0;j<100;j++) {
  gettimeofday(&begin,NULL); //start counting time

  for (i=0;i<150;i++) {
  char* p=malloc(1); //malloc 1 byte
  free(p); //free the 1 byte
  }

  gettimeofday(&end,NULL); //end time
  totalTime1=totalTime1+(double)(end.tv_usec-begin.tv_usec); //increment the total time by the amount of time it took to run this iteration of the loop
}
totalTime1=totalTime1/(double)100; //to take average, divide by 100

/*Test Case B. */
char* arr[50]; //array of pointers
double totalTime2=0; //total time for this test case
int k; //loop counter

for (k=0;k<100;k++) {
  gettimeofday(&begin,NULL); //start counting

  for (i=0;i<3;i++) { //run 3 times
    for (j=0;j<50;j++) { //run 50 times

      arr[j]=malloc(1); //malloc one byte until 50 have been allocated
    }

    for (j=0;j<50;j++) { //run 50 times
      free(arr[j]); //free 50 bytes, one by one
    }
  }

  gettimeofday(&end,NULL); //end time
  totalTime2=totalTime2+(double)(end.tv_usec-begin.tv_usec); //increment the total time by the amount of time it took to run this iteration of the loop
}
totalTime2=totalTime2/(double)100; //find average

/*Test Case C. */
double totalTime3=0;
char* arr2[50];
for (i=0;i<100;i++) {
  gettimeofday(&begin,NULL);
  int mcount=0;
  int fcount=0;
  while (mcount!=50||fcount!=50) { //run until both malloc and free count are 50
    int random=rand(); //get a random number
    //printf("%d\n",random);
    if (random%2==0 && mcount!=50) { //if even number, malloc
      arr2[mcount]=malloc(1);
      mcount++;
    }
    else if ((random%2==1 && mcount>fcount)||mcount==50) { //if odd number, free
      free(arr2[fcount]);
      fcount++;
    }
  }
  gettimeofday(&end,NULL);
  totalTime3=totalTime3+(double)(end.tv_usec-begin.tv_usec);
}
totalTime3=totalTime3/(double)100;

/*Test Case D. */
double totalTime4=0;
for (i=0;i<100;i++) {
  char* arr3[50];
  int mcount=0;
  int count=0;
  int mem=4096; //remaining memory
  int sizes[50]; //array to keep track of what sizes you malloced so you can add or subtract from the current memory space
  gettimeofday(&begin,NULL);
  while (mcount!=50 || count>0) {
    int random=rand(); //generate random number
    int num=(random%64)+1; //mod by 64 and add 1 to get a random malloc size from 1-64 bytes
    if (random%2==0&&mcount!=50) { //if even number and malloc is not 50 yet
      if ((num+sizeof(Node))<=mem) { //if memory to be added can actually fit
        mem=mem-(num+sizeof(Node)); //decrease amount of remaining memory
        arr3[count]=malloc(num); //malloc
        sizes[count]=num+sizeof(Node); //add size just added to array of sizes
        count++;  //increase count (this is used to make sure you malloc more times than you free so that no errors appear)
        mcount++; //increase malloc count by 1;
      }
    }
    else if (random%2==1||mcount==50){ //if odd number of malloc count has reached 50
      if (count>0) { //if count is greater than 0 (essentially you've malloced more times than you've freed)
        mem=mem+sizes[count-1]; // //increase amount of memory remaining
        free(arr3[count-1]); //free
        count--; //decrease count
      }
    }
  }

  gettimeofday(&end,NULL);
  totalTime4=totalTime4+(double)(end.tv_usec-begin.tv_usec);
}
totalTime4=totalTime4/(double)100;

/*Test Case E: testing divide and combine. */
double totalTime5=0;

for (i=0; i<100; i++) {
  gettimeofday(&begin,NULL);
  char* arr4[33]; //array of 33
  for (j=0;j<32;j++) { //chose these numbers randomly because they were just factors of 4096 (essentially mallocs equal blocks of memory into the array)
    arr4[j]=malloc((128-sizeof(Node))); //malloc
  }
  for (j=0;j<32;j++) {
    free(arr4[j]); //free each of the malloced blocks
  }
    arr4[j]=malloc(4096-sizeof(Node)); //now you test to see if you can completely fill the memory to see if divide and combine worked properly
    free(arr4[j]); //free
    gettimeofday(&end,NULL);
    totalTime5=totalTime5+(double)(end.tv_usec-begin.tv_usec);
  }
totalTime5=totalTime5/(double)100; //average total time

/*Test Case F: Mallocing equal sizes throughout and then freeing every other; after this, test to see if a size thats bigger than any free block left returns a mymalloc error. */
double totalTime6=0;

for (i=0; i<100; i++) {
  gettimeofday(&begin,NULL);
  char* arr5[33];
  for (j=0;j<32;j++) { //chose these numbers randomly because they were just factors of 4096 (essentially mallocs equal blocks of memory into the array)
    arr5[j]=malloc((128-sizeof(Node))); //malloc
  }
  for (j=0;j<32;j+=2) { //free every other block
    free(arr5[j]);
  }
char* m=malloc(129); //should print a malloc error because not enough space to allocate memory of this size

for (j=1;j<32;j+=2) {
    free(arr5[j]); //free everything else
  }
    gettimeofday(&end,NULL);
    totalTime6=totalTime6+(double)(end.tv_usec-begin.tv_usec);
  }
totalTime6=totalTime6/(double)100; //take average of total time

/* Print all the average times. */
printf("%f is the average time in microseconds it takes to run Test Case A.\n",totalTime1);
printf("%f is the average time in microseconds it takes to run Test Case B.\n",totalTime2);
printf("%f is the average time in microseconds it takes to run Test Case C.\n",totalTime3);
printf("%f is the average time in microseconds it takes to run Test Case D.\n",totalTime4);
printf("%f is the average time in microseconds it takes to run Test Case E.\n",totalTime5);
printf("%f is the average time in microseconds it takes to run Test Case F.\n",totalTime6);
}
