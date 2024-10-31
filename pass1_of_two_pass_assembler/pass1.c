#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void printfile(FILE* fp , char str[]);
int searchf(FILE *fp , char str[]);
int main() 
{
    char opcode[10], operand[10], label[10];
    int locctr, start, length;
    FILE *input, *optab, *symtab, *output;
    input = fopen("input.txt", "r");
    optab = fopen("optab.txt", "r");
    symtab = fopen("symtab.txt", "w");
    output = fopen("intermediate.txt", "w");
    fscanf(input, "%s\t%s\t%s", label, opcode, operand);
    if (!(strcmp(opcode, "START") )) 
    {
        start = (int) strtol(operand , NULL , 16);
        locctr = start;
        fprintf(output, "\t%s\t%s\t%s\n", label, opcode, operand);
        fscanf(input, "%s\t%s\t%s", label, opcode, operand);
    } 
    else
        locctr = 0;
    while (strcmp(opcode, "END") != 0) 
    {
        fprintf(output, "%X\t", locctr);
        if ((strcmp(label, "**")))
        {
            fclose(symtab);
            if(!(searchf(fopen("symtab.txt","a+"), label)))
                fprintf(symtab, "%s\t%X\n", label, locctr);
            else
            {
            	printf("Error: Duplicate Symbol");
            	exit(0);
            }
        }
        if(searchf(optab,opcode))
            locctr+=3;
        else if (strcmp(opcode, "WORD") == 0)
            locctr += 3;
        else if (strcmp(opcode, "RESW") == 0)
            locctr += (3 * atoi(operand));
        else if (strcmp(opcode, "RESB") == 0)
            locctr += atoi(operand);
        else if (strcmp(opcode, "BYTE") == 0)
        {
            if(operand[0]=='C')
                  locctr += (strlen(operand) - 3);
            else if(operand[0]=='X')
                  locctr+=(strlen(operand)-3)/2;
        }
        else
        {
        	printf("Invalid Opcode");
        	exit(0);
        }
        fprintf(output, "%s\t%s\t%s\n", label, opcode, operand);
        fscanf(input, "%s\t%s\t%s", label, opcode, operand);
    }
    fprintf(output, "%X\t", locctr);
    fprintf(output, "%s\t%s\t%s\n", label, opcode, operand);
    length = locctr - start;
    printfile(input , "input");
    printfile(optab , "optab");
    fclose(symtab);
    fclose(output);
    printfile(fopen("intermediate.txt", "r") , "intermediate");
    printf("\nThe length of program: %X", length);
    printfile(fopen("symtab.txt", "r") , "symtab");
    return 0;
}
void printfile(FILE* fp , char str[])
{
    rewind(fp);
    printf("\n\nThe contents of %s file is: \n",str);
    char ch = fgetc(fp);
    while (ch != EOF) 
    {
        
        printf("%c", ch);
        ch = fgetc(fp);
    }
    fclose(fp);
}
int searchf(FILE *fp, char str[]) 
{
    char str1[10], mne[10];
    rewind(fp);
    while (fscanf(fp, "%s %s", str1 , mne) != EOF) 
        if (strcmp(str1, str) == 0) 
            return 1; 
    fseek(fp, 0, SEEK_END); 
    return 0;
}