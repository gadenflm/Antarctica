#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	int t,index,flag,i,num_a,num_b,num_c;
	char str[52];

	scanf("%d",&t);
	for (index=0;index<t;index++)
	{
		scanf("%s",str);
		flag=0;
		num_a=0;
		num_b=0;
		num_c=0;

	 	for (i=0;i<strlen(str);i++)
		{

			if (str[i]=='A')
				num_a++;
			if (str[i]=='B')
				num_b++;
			if (str[i]=='C')
				num_c++;
				
		}


		if (num_a==strlen(str)/2)
			flag=1;
	
		if (num_b==strlen(str)/2)
			flag=1;

		if (num_c==strlen(str)/2)
			flag=1;
		if (str[0] == str [strlen(str)-1]) 	
			
			flag=0;
		if (flag==1)
			printf("YES\n");
		else
			printf("NO\n");
	}
	return 0;	
}