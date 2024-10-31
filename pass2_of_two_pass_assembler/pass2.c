#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int searchf(FILE *fp , char str[]);
int searchloc(int start);
int writetr(char tr[] , int* start, int* tcount , char opcode[] , char loc[], FILE *fp , char list[],char addr[], char label[], char operand[], FILE *input , int a);
int main()
{
    char opcode[10], operand[10], label[10],loc[10] , tr[150] , addr[10], list[10];
    int start, length,tcount=0 , initial;
    FILE *input, *optab, *symtab,*listing,*objcode;
    optab = fopen("optab.txt", "r");
    symtab = fopen("symtab.txt", "r");
    input = fopen("intermediate.txt", "r");
    listing = fopen("listing.txt","w");
    objcode = fopen("objectcode.txt","w");
    fscanf(input, "\t%s\t%s\t%s",label, opcode, operand);
    start=(int)strtol(operand,NULL,16);
    initial=start;
    while (strcmp(opcode, "END") != 0)
       fscanf(input, "%s\t%s\t%s\t%s",loc, label, opcode, operand);
    length= ((int)strtol(loc,NULL,16)) - start;
    rewind(input);
    fscanf(input, "\t%s\t%s\t%s",label, opcode, operand);
    if (!(strcmp(opcode, "START") )) 
    {
       fprintf(objcode,"H^%-6s^%06X^%06X\n",label,start,length);
       fprintf(listing,"\t%s\t%s\t%s\t\n",label,opcode,operand);
       fscanf(input, "%s\t%s\t%s\t%s",loc, label, opcode, operand);
       strcpy(addr,loc);
	   start=(int)strtol(loc,NULL,16);
    }
    else
    {
       printf("Intermediate file error");
       exit(0);
    }
	tr[0]='\0';
    while (strcmp(opcode, "END") != 0)
    { 
        int count =(int)strtol(loc,NULL,16)-(int)strtol(addr,NULL,16);
    	if((addr[0]==loc[0]) && count<512)
    	{
    		if(strcmp(opcode,"RESW")==0 || strcmp(opcode,"RESB")==0)
		    {
    		   fprintf(listing,"%s\t%s\t%s\t%s\t\n",loc,label,opcode,operand);
			   strcpy(addr,loc);
			   fscanf(input, "%s\t%s\t%s\t%s",loc, label, opcode, operand);
		    }
		    else if(strcmp(opcode,"WORD")==0)
		    {
    		    sprintf(list,"%06X",atoi(operand));
    	        fprintf(listing,"%s\t%s\t%s\t%s\t%s\n",loc,label,opcode,operand,list);
				if((tcount+3)<=30)
				{
				   tcount+=3;
    	           strcat(tr,"^");
    	           strcat(tr,list);
    	           strcpy(addr,loc);
    	           fscanf(input, "%s\t%s\t%s\t%s",loc, label, opcode, operand);
				}
				else
				   writetr(tr , &start,&tcount,opcode,loc,objcode ,list,addr,label,operand,input,3);
		    }
			else if(strcmp(opcode,"BYTE")==0)
			{
               char bytes[150] = {0};
               int j = 0;
               if (operand[0] == 'C')
			    {
                  for (int i = 2; operand[i] != '\''; i++) 
                  {
                      sprintf(bytes + j, "%02X", (unsigned char)operand[i]);
                      j += 2;
                  }
                } 
               else if (operand[0] == 'X')
               {
                  for (int i = 2; operand[i] != '\''; i += 2) 
                  {
                      bytes[j++] = operand[i];
                      bytes[j++] = operand[i + 1];
                  }
               }
			   fprintf(listing, "%s\t%s\t%s\t%s\t%s\n", loc, label, opcode, operand, bytes);
			   if((tcount+(j/2))<=30)
			   {
				  tcount+=j/2;
                  strcat(tr,"^");
    	          strcat(tr,bytes);
    	          strcpy(addr,loc);
                  fscanf(input, "%s\t%s\t%s\t%s", loc, label, opcode, operand);
			   }
			   else
			       writetr(tr , &start,&tcount,opcode,loc,objcode ,bytes,addr,label,operand,input,j/2);
            }
		    else
    	    {
    	   	 int opa=searchf(optab,opcode);
    	        int sya=searchf(symtab,operand);
    	        sprintf(list,"%02X%04X",opa,sya);
    	        fprintf(listing,"%s\t%s\t%s\t%s\t%s\n",loc,label,opcode,operand,list);
				if((tcount+3)<=30)
				{
				   tcount+=3;
    	           strcat(tr,"^");
    	           strcat(tr,list);
    	           strcpy(addr,loc);
    	           fscanf(input, "%s\t%s\t%s\t%s",loc, label, opcode, operand);
				}
				else
				   writetr(tr , &start,&tcount,opcode,loc,objcode ,list,addr,label,operand,input,3);
    	    }
		    if(strcmp(opcode,"END")==0)
		    {
			   if(strlen(tr))
			   {
			      fprintf(objcode,"T^%06X^%02X%s\n",start,tcount,tr);
    		      tr[0]='\0';
				  start=(int)strtol(loc,NULL,16);
			   }
		    }
    	}
    	else
    	{
			if(strlen(tr))
			{
    		   fprintf(objcode,"T^%06X^%02X%s\n",start,tcount,tr);
			   if((strcmp(opcode,"RESW")!=0) && (strcmp(opcode,"RESB")!=0))
			       start=(int)strtol(loc,NULL,16);
			   else
			       start=searchloc((int)strtol(loc,NULL,16));
			}
    		tr[0]='\0';
			tcount=0;
			strcpy(addr,loc);
    	}
    }
    fprintf(listing,"%s\t%s\t%s\t%s\t\n",loc,label,opcode,operand);
	fprintf(objcode,"E^%06X",initial);
	fclose(optab);
	fclose(symtab);
	fclose(listing);
	fclose(objcode);
	fclose(input);
}
int searchf(FILE *fp, char str[]) 
{
    char str1[10], mne[10], str2[10];
    rewind(fp);
    while (fscanf(fp, "%s %s", str1, mne) != EOF) 
    {
        char *index = strchr(str, ',');
        if (index != NULL) 
        {
            size_t len = index - str;
            strncpy(str2, str, len);
            str2[len] = '\0';
            if (strcmp(str1, str2) == 0) 
                return (int)strtol(mne, NULL, 16) + 32768;
        }
        if (strcmp(str, str1) == 0) 
            return (int)strtol(mne, NULL, 16);
    }
    return 0;
}
int searchloc(int start)
{
	FILE *fp = fopen("intermediate.txt","r");
	char loc[10],label[10],opcode[10],operand[10],addr[10];
	fscanf(fp, "\t%s\t%s\t%s",label, opcode, operand);
	sprintf(addr,"%X",start);
	while ((strcmp(loc,addr))!=0)
		fscanf(fp, "%s\t%s\t%s\t%s",loc, label, opcode, operand);
    while(fscanf(fp, "%s\t%s\t%s\t%s",loc, label, opcode, operand)!=EOF)
		if((strcmp(opcode,"RESW")!=0) && (strcmp(opcode,"RESB"))!=0)
		{
			fclose(fp);
			return (int)strtol(loc,NULL,16);
		}
	fclose(fp);
	return start;
}
int writetr(char tr[] , int* start, int* tcount , char opcode[] , char loc[], FILE *fp , char list[],char addr[], char label[], char operand[] , FILE *input , int a)
{
    if(strlen(tr))
	{
    	fprintf(fp,"T^%06X^%02X%s\n",*start,*tcount,tr);
		if((strcmp(opcode,"RESW")!=0) && (strcmp(opcode,"RESB")!=0))
		    *start=(int)strtol(loc,NULL,16);
		else
		    *start=searchloc((int)strtol(loc,NULL,16));
	}
    tr[0]='\0';
	*tcount=a;
	strcat(tr,"^");
    strcat(tr,list);
    strcpy(addr,loc);
    fscanf(input, "%s\t%s\t%s\t%s",loc, label, opcode, operand);
}