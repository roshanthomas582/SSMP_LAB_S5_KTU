//implementation of pass1 of two pass assembler in c
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int searchf(FILE *fp , char str[]); //function to search opcode in optab
int main() 
{
    char label[10] , opcode[10], operand[10];
    int locctr = 0, start=0;
    FILE *input, *optab, *symtab, *output;

    input = fopen("input.txt", "r");
    optab = fopen("optab.txt", "r");
    symtab = fopen("symtab.txt", "w");
    output = fopen("intermediate.txt", "w");

    fscanf(input, "%s\t%s\t%s", label, opcode, operand); //reads first line of source program 
    if ( (strcmp(opcode, "START")==0) )//if first opcode is start. strcmp returns 0 if both string are same. 
    {
        //converting hexadecimal address to integer
        locctr = start = (int) strtol(operand , NULL , 16); 
        
        fprintf(output, "\t%s\t%s\t%s\n", label, opcode, operand); //writing first line to intermediate file
        fscanf(input, "%s\t%s\t%s", label, opcode, operand); //reading second line from source program
    } 
    
    while (strcmp(opcode, "END") != 0) //repeats the following untill opcode reaches END
    {
        fprintf(output, "%X\t%s\t%s\t%s\n", locctr , label, opcode, operand); //writes the current line to intermediate file. %X to print locctr as hexadecimal
        
        if ( (strcmp(label, "**")) !=0) //if there is symbol in label field that is not **
            fprintf(symtab, "%s\t%X\n", label, locctr); //writes the symbol and it's corresponding locctr to symtab.

        if( searchf(optab,opcode) )//returns true if opcode found 
            locctr+=3; //default length of instruction
        else if (strcmp(opcode, "WORD") == 0)
            locctr += 3; //default length of WORD
        else if (strcmp(opcode, "RESW") == 0)
            locctr += (3 * atoi(operand)); //reserving 3bytes for each word
        else if (strcmp(opcode, "RESB") == 0)
            locctr += atoi(operand); //adds the operand 
        else if (strcmp(opcode, "BYTE") == 0)
        {
            if(operand[0]=='C')
                  locctr += (strlen(operand) - 3); //subtract 3 to remove the count of C''. eg: C'EOF'
            else if(operand[0]=='X')
                  locctr+=(strlen(operand)-3)/2;  
                  //sub 3 to remove count of X'' and divide by 2 as two hexa digit is 1 byte
        }
        else
        {
           	printf("Invalid Opcode");
           	exit(0);
        }
        fscanf(input, "%s\t%s\t%s", label, opcode, operand); //reads the next line. 
    }
    fprintf(output, "%X\t%s\t%s\t%s\n", locctr,label, opcode, operand); //insert last line with opcode 'END'
    fclose(symtab) , fclose(output) , fclose(optab) , fclose(input);
    printf("\nThe length of program: %X\n", (locctr-start)); //length of the program in hexadecimal
    return 0;
}

int searchf(FILE *fp, char str[]) 
{
    char str1[10], mne[10];
    rewind(fp); //move file pointer to begining to search from begining
    while (fscanf(fp, "%s %s", str1 , mne) != EOF) //reads opcode and it's address line by line from optab until file reaches End of File
        if (strcmp(str1, str) == 0) //Compares opcode read from the optab file that is str1 with opcode found in source program that is str
            return 1; //return 1 if opcode found. 
    return 0; //return 0 if opcode not found in the file
}