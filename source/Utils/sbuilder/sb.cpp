#include <stdio.h>
#include "sf.h"

void showHelp()
{
	printf("\t%%d(start-end) - Generates strings, replacing elements with numbers in selected diapason.\n");
	printf("\t%%D(start-end) - Generates strings, replacing elements with numbers in selected diapason, adding left zeros to make string length the same.\n");
}

void main(int argc, char* argv[])
{
	printf("\n\nString bulider v1.0e\n\tUsing:\n\t\tsb[.exe] string output_file\n\n\ttype sb -? for elements help\n\n");

	if(argc>1)
		if(strstr(argv[1],"-?"))
		{
			showHelp();
			return;
		}

	if (argc!=3)
	{
		printf("\tBad syntax!");
		return ;
	}

	int e,s,f,i;
	bool parsed=false;
	FILE *out=fopen(argv[2], "w");

	if(out)
	{
		if(parseTemplate(argv[1],"%d",&e, &s, &f))
		{
			parsed=true;
			for(i=s;i<=f;i++)
				fprintf(out,"%s%d%s\n", argv[1],i,argv[1]+e);
			printf("\tElement %%d successfully parsed\n");
		}

		if(parseTemplate(argv[1],"%D",&e, &s, &f))
		{
			parsed=true;
			char intro[256];
			for(i=s;i<=f;i++)
			{
				zeroStarts(intro, i, f);
				fprintf(out,"%s%s%d%s\n", argv[1],intro, i,argv[1]+e);
			}
			printf("\tElement %%D successfully parsed\n");
		}

		if(!parsed)
			printf("\tParsing elements not found, or bad syntax");

		fclose(out);
	}
	else
		printf("\tCannot open file '%s' for writing!\n", argv[2]);
}

