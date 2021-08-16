#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	int len,n,i,index;
	char str[101],str_2[101],str_out[101];

	scanf ("%d",&n);
	for (index=0;index<n;index++)
	{
		scanf ("%s",str);
		len=strlen(str);
		if (len > 10)
		{
			len=len-2;
			sprintf(str_2,"%d",len);
			str_out[0]=str[0];
			for (i=1;i<=strlen(str_2);i++)
			{
				str_out[i]=str_2[i-1];
			}
			str_out[i]=str[len+1];
			str_out[i+1]='\0';
			printf ("%s\n",str_out);
		}
		else
			printf ("%s\n",str);

		
	}
	return 0;
}