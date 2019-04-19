#include<stdio.h>
#include<stdlib.h>
struct process
{
    int process_name;
    int priority,arrival_time,burst_time,completed,completed_part;
}processq[10];
int nop;
int roundrobin();
int rr[100],rr1[100];
int ii=0,kk=0;
int flag=0,p[100],w[100],total=0,temp[100];
int g=0;
int rr3[100],h=0;
int gethighpriority()
{
    int i,high=1000,no;
    for(i=0;i<nop;i++)
    {
        if(processq[i].completed==0 && processq[i].priority<high)
        {
			high=processq[i].priority;
            no=i;
        }
    }
    return no;
}
int higharrivaltime()
{
    int i,high=-1;
    for(i = 0; i < nop; i++)
    {
        if(processq[i].arrival_time>high)
        {
            high=processq[i].arrival_time;
        }
    }
    return high;
}
int main()
{
    printf("Enter the number of process:");
    scanf("%d",&nop);
    if(nop>0)
    {
        int i,j,time=0,totaltime,highest_priority=1000,current_process=0,lowest=9999,prio_order[10],highat;
        for(i = 0; i < nop; i++)
        {
            printf("Enter the process name in integers:");
            scanf("%d",&processq[i].process_name);
            printf("Enter the priority and arrival time and burst time:");
            scanf("%d%d%d",&processq[i].priority,&processq[i].arrival_time,&processq[i].burst_time);
            if(processq[i].burst_time>0 && processq[i].arrival_time>=0)
            {
                totaltime+=processq[i].burst_time;
                processq[i].completed_part=processq[i].completed=0;
            }
            else
                printf("\nSORRY INVALID INPUT\n");
        }
        highat=higharrivaltime();
        for(i=0;i<nop;i++)
        {
            if(processq[i].arrival_time<lowest)
            {
                current_process=i;
                lowest=processq[i].arrival_time;
            }
        }
        printf("Process:%d runs from 0 to",processq[current_process].process_name);
        while(time<totaltime)
        {
            for(i=0;i<nop;i++)
            {
                if(i==current_process)
                    continue;
                if(processq[i].priority==processq[current_process].priority && processq[i].completed==0 && processq[i].arrival_time>=processq[current_process].arrival_time && processq[current_process].completed==0)
            	{
            	    rr[g]=processq[i].burst_time-processq[i].completed_part;
            	    rr1[g]=processq[i].process_name;
            	    processq[i].completed=2;
            	    g++;
				}
				if(processq[i].arrival_time<=time && processq[i].priority<processq[current_process].priority && processq[i].completed==0 )
                {
					rr[g]=processq[current_process].burst_time-processq[current_process].completed_part;
            	    rr1[g]=processq[current_process].process_name;
            	    g++;
            	    processq[current_process].completed=2;
            	    current_process=i;
                    printf("%d",time);
                    printf("\nProcess:%d runs from %d to",processq[current_process].process_name,time);
                }
            }
            processq[current_process].completed_part++;
            if(processq[current_process].completed_part==processq[current_process].burst_time)
            {
                processq[current_process].completed=1;
            	printf("%d",time+1);
				printf("\nProcess:%d completed",processq[current_process].process_name);
				current_process=gethighpriority();
                if(time>highat)
                    break;
                printf("\nProcess:%d runs from %d to",processq[current_process].process_name,time+1);
            }
            time++;        
        }
        int count[10],h=0;
        for(i=0;i<nop;i++)
        {
            for(j=0;j<g;j++)
            {
 	            if(processq[i].process_name==rr1[j])
 	                break;
 		    }
        }
	    printf("\n No.of processes in roundrobbin array ");
        printf("%d\n",g);
        fflush(stdin);
        for(ii=0;ii<h;i++)
        printf("\n%d",rr3[h]);
        for(j=0;j<g;j++)
        {
    	    printf("\n process p%d remaining of burst time %d\n",rr1[j],rr[j]);
            w[j]=0;
            p[j]=0;
        }
        roundrobin();
    }
    else
        printf("\nENTER THE NO OF PROCESSES GREATER THEN 0\n");
}
int roundrobin()
{
    int quan;
    printf("enter quan");
    scanf("%d",&quan);
    if(quan%2==0)
    {
        while(1)
        {
            if(ii==g)                 
            ii=0;
            if(flag==g)
                break;
            if(rr[ii]<=quan && rr[ii]>0)
            {
                w[ii]+=total-p[ii];
                flag++;
                total+=rr[ii];     
                rr[ii]=0;
                temp[kk]=rr1[ii];
                kk++;	
            }
            if(rr[ii]>quan)
            {
                w[ii]+=total-p[ii];
                rr[ii]=rr[ii]-quan;
                total+=quan;
                temp[kk]=rr1[ii];
                kk++;	
            }
            p[ii]=total;
            ii++;
        }
        printf("order of execution:\n");
        for(ii=0;ii<kk;ii++)
        {
            printf("P%d ",temp[ii]);	
        }
        printf("\nTHE PROCESSES OTHER THAN IN ROUND ROBIN PROCESS IS COMPLETED IN PRIORITY QUEQE\n ");
    }
    else
        printf("\nTIME QUANTUM SHOULD BE MULTIPLESOF 2\n");
}
