#include "sf.h"

bool parseTemplate(char* string, char* templ, int* tend, int *srange, int* frange)
{
	char* ts, *opbr, *clbr, *min;

	if(!(ts=strstr(string, templ))) return false;
	if(!(opbr=strstr(ts, "("))) return false;
	if(!(clbr=strstr(opbr, ")"))) return false;
	if(!(min=strstr(opbr, "-"))) return false;

	int start=atoi(opbr+1);
	int fin=atoi(min+1);

	if(!start||!fin) return false;
	if(start>fin)
	{
		int t;
		t=fin, fin=start, start=t;
	}

	*srange=start;
	*frange=fin;
	*tend=(clbr-string)+1;

	ts[0]=0;

	return true;
}

void zeroStarts(char* buffer, int n, int max)
{
	buffer[0]=0;
	int mlen=(int)(log(max)/log(10));
	int dig=(int)floor(pow(10,mlen));
	for(int j=1;j<=mlen;j++,dig/=10)
		if(n<dig)strcat(buffer,"0");
}