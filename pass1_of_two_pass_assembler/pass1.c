#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int searchf(FILE *fp , char str[]); //function to search opcode and symbol in optab and symtab
int main() 
{
    char label[10] , opcode[10], operand[10];
    int locctr = 0, start=0;
    FILE *input, *optab, *symtab, *output;

    input = fopen("input.txt", "r");
    optab = fopen("optab.txt", "r");
    symtab = fopen("symtab.txt", "w");
    output = fopen("intermediate.txt", "w");
    
    if (!input || !optab || !symtab || !output) 
    {
        printf("Error accessing files."); //check files are opened
        exit(0);
    }
    fscanf(input, "%s\t%s\t%s", label, opcode, operand);
    if ( (strcmp(opcode, "START")==0) )//if first opcode is start
    {
        //converting hexadecimal address to integer
        locctr = start = (int) strtol(operand , NULL , 16); 
        
        fprintf(output, "\t%s\t%s\t%s\n", label, opcode, operand);
        fscanf(input, "%s\t%s\t%s", label, opcode, operand);
    } 
    while (strcmp(opcode, "END") != 0) 
    {
        fprintf(output, "%X\t", locctr); //printing address as hexadecimal
        if ((strcmp(label, "**"))) //if there is symbol
        {
            fclose(symtab);
            if(!(searchf(fopen("symtab.txt","a+"), label)))
                fprintf(symtab, "%s\t%X\n", label, locctr);
                //adding symbol to symtab if it is not found in symtab
            else
            {
            	printf("Error: Duplicate Symbol");
            	exit(0);
            }
        }
        if(searchf(optab,opcode))
            locctr+=3; //default length of instruction
        else if (strcmp(opcode, "WORD") == 0)
            locctr += 3; //default length of WORD
        else if (strcmp(opcode, "RESW") == 0)
            locctr += (3 * atoi(operand)); //reserving 3bytes for each word
        else if (strcmp(opcode, "RESB") == 0)
            locctr += atoi(operand);
        else if (strcmp(opcode, "BYTE") == 0)
        {
            if(operand[0]=='C')
                  locctr += (strlen(operand) - 3); //subtract 3 to remove the count of C''
            else if(operand[0]=='X')
                  locctr+=(strlen(operand)-3)/2;  
                  //sub 3 to remove count of X'' and divide by 2 as two hexa digit is 1 byte
        }
        else
        {
        	printf("Invalid Opcode");
        	exit(0);
        }
        fprintf(output, "%s\t%s\t%s\n", label, opcode, operand);
        fscanf(input, "%s\t%s\t%s", label, opcode, operand);
    }
    fprintf(output, "%X\t%s\t%s\t%s\n", locctr,label, opcode, operand); //insert last line with opcode 'END'
    fclose(symtab) , fclose(output) , fclose(optab) , fclose(input);
    printf("\nThe length of program: %X", (locctr-start)); //length of the program in hexadecimal
    return 0;
}

int searchf(FILE *fp, char str[]) 
{
    char str1[10], mne[10];
    rewind(fp); //move file pointer to start to search from begining
    while (fscanf(fp, "%s %s", str1 , mne) != EOF) 
        if (strcmp(str1, str) == 0) 
            return 1; //return 1 if opcode or symbol found
    fseek(fp, 0, SEEK_END); //move file pointer to end to continue appending symbols in symtab
    return 0; //return 0 if opcode or symbol not found in the file
}