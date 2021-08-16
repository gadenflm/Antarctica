#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	int t,index,flag,i;
	char str[52];
	char str_a[52];
	char str_b[52];
	char str_ab[52];

	scanf("%d",&t);
	for (index=0;index<t;index++)
	{
		scanf("%s",str);
		flag=0;

	 	for (i=0;i<strlen(str);i++)
		{
			if (str[i]=='C')
			{
				str_a[i]='A';
				str_b[i]='B';
			}
			else
			{
				str_a[i]=str[i];
				str_b[i]=str[i];
			}
			if (str[i]=='A')
				str_ab[i]='B';
			else
				str_ab[i]=str[i];
		}

		str_a[i]='\0';
		str_b[i]='\0';
		str_ab[i]='\0';

		if (Is_Regular(str_a)==1)
			flag=1;
	
		if (Is_Regular(str_b)==1)
			flag=1;

		if (Is_Regular(str_ab)==1)
			flag=1;
	
		if (flag==1)
			printf("YES\n");
		else
			printf("NO\n");
	}
	return 0;	
}

int Is_Regular (char* str)
{
	char a[52];
	int len,i,ind;
	len=strlen(str);

	if (str[0] == str [len-1]) 	return 0;
	ind=0;
	a[ind]=str[0];
	for (i=1;i<len;i++)
	{
		if (ind==-1) 
		{
			ind++;
			a[ind]=str[i];
			continue;
		}		
		if (a[ind]==str[i])
		{			
			ind++;
			a[ind]=str[i];
		}
		else
		{
			a[ind]='\0';
			ind--;
		}

	}
	if (ind==-1) return 1 ; else return 0;
}