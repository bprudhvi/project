#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
int nr,np; //in the sense nr means no:of resources and np means no of processes
int *res; //in the sense we cant say the size of resources so we are going to use dma
int **alloc,**max, **need; //in the sense allocated memory should be in two-d array so using dma with 2pointers
//similarly max and need also should be in twod array so using 2pointers in its allocation using dma
int *seq;//for reference of safesequence
int npr = 0;
pthread_mutex_t t;
pthread_cond_t pp;
int safestate();
void* banker(void* );
int main() {
        printf("\nENTER NO OF PROCESSES\n ");
        scanf("%d", &np);
        printf("\nENTER NO OF RESOURSES\n");
        scanf("%d", &nr);
        system("cls");
        if(np>0 && nr >0)
        {
        res = (int *)malloc(nr * sizeof(*res));
        printf("ENTER AVAILABLE RESOURSES ");
        for(int i=0; i<nr; i++)
        scanf("%d", &res[i]);
        alloc = (int **)malloc(np * sizeof(*alloc));
        for(int i=0; i<np; i++)
        alloc[i] = (int *)malloc(nr * sizeof(**alloc));
		max = (int **)malloc(np * sizeof(*max));
        for(int i=0; i<np; i++)
        max[i] = (int *)malloc(nr * sizeof(**max));
        printf("\n");
        printf("\n<<<<<<<<<<<<<RESOURCE ALLOCATION<<<<<<<<<<<<<<<<<<\n");
        for(int i=0; i<np; i++) {
        printf("\nRESOURSE ALLOCATED TO P %d\n",i+1);
        for(int j=0; j<nr; j++)
        scanf("%d", &alloc[i][j]);
        }
        printf("\n");
        for(int i=0; i<np; i++) {
                printf("\nMaximum resource required by P %d  ", i+1);
                for(int j=0; j<nr; j++)
                        scanf("%d", &max[i][j]);
        }
        printf("\n");
        need = (int **)malloc(np * sizeof(*need));
        for(int i=0; i<np; i++)
        need[i] = (int *)malloc(nr * sizeof(**need));
        for(int i=0; i<np; i++)
        for(int j=0; j<nr; j++)
        {
        	if(max[i][j]>=alloc[i][j])
        need[i][j] = max[i][j] - alloc[i][j];
        else
        printf("\n<<<<<<<<<<<<<<<<<<MAXIMUM SHOULD BE ALWAYS GREATER THAN ALLOCATION<<<<<<<<<<\n");}
		seq = (int *)malloc(np * sizeof(*seq));
        for(int i=0; i<np; i++) seq[i] = -1;
		int x=safestate();
		system("cls");
        if(x!=1) {
            printf("\n\n<<<<<<<<<<<<<<<<<<<IT IS IN UNSAFE STATE<<<<<<<<<<<<<<<<\n\n");
            exit(-1);}
        printf("\n\n<<<<<<<<<<<<<<<<<<<<<<SAFE STATE<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
        printf("\n\nSafe Sequence Found : ");
        for(int i=0; i<np; i++) {
        printf("%-3d", seq[i]+1);}
        printf("\nExecuting Processes...\n\n");
        sleep(1); 
	pthread_t processes[np];
	int pno[np];
	for(int i=0; i<np; i++) pno[i] = i;
    for(int i=0; i<np; i++)
    pthread_create(&processes[i], NULL, banker, (void *)(&pno[i]));
        for(int i=0; i<np; i++)
        pthread_join(processes[i], NULL);
		printf("\n<<<<<<<<<<<<<<<<<<<<<<All Processes TERMINATED<<<<<<<<<<<<<<<<<<<<\n");	
	// free resources
        free(res);
        for(int i=0; i<np; i++) {
                free(alloc[i]);
                free(max[i]);
		free(need[i]);
		}
        free(alloc);
        free(max);
	free(need);
        free(seq);
        printf("<<<<<<<<<<<<<<<<ALL RESOURCES ARE FREE<<<<<<<<<<<<<<<<\n");}
        else{
printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
printf("\n\n NO:OF PROCESSES AND NO:OF RESOURSES SHOULD BE GREATER THAN ZERO\n\n");}
}
int safestate() {
        int tempRes[nr];
        for(int i=0; i<nr; i++) tempRes[i] = res[i];
        bool finished[np];
        for(int i=0; i<np; i++) finished[i] = false;
        int nfinished=0;
        while(nfinished < np) {
                bool safe = false;
                for(int i=0; i<np; i++) {
                if(!finished[i]) {
                bool possible = true;
					for(int j=0; j<nr; j++)
                    if(need[i][j] > tempRes[j]) {
                    possible = false;
                    break;}
					if(possible) {
                    for(int j=0; j<nr; j++)
                    tempRes[j] += alloc[i][j];
                    seq[nfinished] = i;
                    finished[i] = true;
                    ++nfinished;
                    safe = true;}}}
                if(!safe) {
                for(int k=0; k<np; k++) seq[k] = -1;
                return 0; }// no safe sequence found and returning value as 0
        } return 1; // safe sequence found and returning value as 1
}
void* banker(void *arg) {
        int p = *((int *) arg);
        pthread_mutex_lock(&t);
        while(p != seq[npr])
        pthread_cond_wait(&pp, &t);
        printf("\n--> Process %d", p+1);
        printf("\n\tAllocated : ");
        for(int i=0; i<nr; i++)
        printf("%3d", alloc[p][i]);
        printf("\n\tNeeded    : ");
        for(int i=0; i<nr; i++)
        printf("%3d", need[p][i]);
        printf("\n\tAvailable : ");
        for(int i=0; i<nr; i++)
        printf("%3d", res[i]);
        printf("\n"); sleep(1);
        printf("\tResource Allocated!");
        printf("\n"); sleep(1);
        printf("\tResource Released!");
	    for(int i=0; i<nr; i++)
        res[i] += alloc[p][i];
        printf("\n\tNow Available : ");
        for(int i=0; i<nr; i++)
        printf("%3d", res[i]);
        printf("\n\n");
        sleep(1);
        npr++;
        pthread_cond_broadcast(&pp);
        pthread_mutex_unlock(&t);
		pthread_exit(NULL);
}
