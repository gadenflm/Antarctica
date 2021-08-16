#include <stdio.h>

int main(int argc, char const *argv[])
{
	long t,index,i,j,n,cur_ind,cur_time,time;
	long a[102];
	long b[102];
	long tmi[102];

	scanf ("%ld",&t);
	for (index=0;index<t;index++)
	{
		scanf ("%ld",&n);
		a[0]=0;b[0]=0;tmi[0]=0;
		for (i=1;i<n+1;i++) scanf ("%ld %ld",&a[i],&b[i]);

		for (i=1;i<n+1;i++) scanf ("%ld",&tmi[i]); 

		cur_ind=0;
		cur_time=0;
		while (cur_ind<n-1 )
		{
			time=0;
			time += a[cur_ind+1]-b[cur_ind]+tmi[cur_ind+1];
			time += ((int)(b[cur_ind+1]-a[cur_ind+1])/2+(b[cur_ind+1]-a[cur_ind+1])%2) ;
			cur_time+=time;
			if (cur_time<b[cur_ind+1]) cur_time=b[cur_ind+1]; 
			cur_ind++;
		}
		cur_time +=a[cur_ind+1]-b[cur_ind]+tmi[cur_ind+1];
		printf("%ld\n",cur_time );
	}

	return 0;
}