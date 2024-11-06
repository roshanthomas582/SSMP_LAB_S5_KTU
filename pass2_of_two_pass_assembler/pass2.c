//implementation of pass2 of two pass assembler in C.
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char opcode[10], operand[10], label[10], locctr[10], textrecbuff[150] , objcode[10]; 
int trstart = 0, length, trlength = 0, startaddr = 0;
FILE *intermediate, *optab, *symtab, *listing, *objectcode;

int searchfile(FILE *fp, char str[])  //function to return the address of opcode and symbols to generate object code
{
    char str1[10], addr[10];
    rewind(fp); //move file pointer to the beginning to search the file from start
    while (fscanf(fp, "%s %s", str1, addr) != EOF) //read (symbol or opcode) and its corresponding address line by line from file untill End of File
        if ( (strcmp(str, str1)) ==0)  //compare opcode or symbol in file that is str1 to the opcode or symbol read from the source program
            return (int)strtol(addr, NULL, 16); // if (opcode or symbol) is found return its corresponding address as integer
    return 0; //if not found return address as 0
}

void writeTextRec(int size) 
{
    if(strlen(textrecbuff)) //if there is unwritten textrecord in buffer write it to object code
        fprintf(objectcode,"T^%06X^%02X%s\n",trstart, trlength, textrecbuff);
    textrecbuff[0]='\0'; //after writing text record initialize the buffer as empty
    trlength = size; //change the length of buffer to the size of new record recieved
    strcat(textrecbuff, "^"), strcat(textrecbuff, objcode); //add nrw object code to buffer
    fscanf(intermediate, "%s\t%s\t%s\t%s", locctr, label, opcode, operand); //read next input line
    trstart = (int)strtol(locctr, NULL, 16); //set the location of new input line as the starting address of next text record
}

void updateTextRec(int size) 
{
    trlength += size; //update text record size. maximum text record size is 30
    strcat(textrecbuff, "^"), strcat(textrecbuff, objcode);
    fscanf(intermediate, "%s\t%s\t%s\t%s", locctr, label, opcode, operand);
}

int main() 
{
    optab = fopen("optab.txt", "r");
    symtab = fopen("symtab.txt", "r");
    intermediate = fopen("intermediate.txt", "r");
    listing = fopen("listing.txt", "w");
    objectcode = fopen("objectcode.txt", "w");
    
    fscanf(intermediate, "\t%s\t%s\t%s", label, opcode, operand); //reads the first line of intermediate fiile
    startaddr = trstart = (int)strtol(operand, NULL, 16);
    
    while (strcmp(opcode, "END")) 
        fscanf(intermediate, "%s\t%s\t%s\t%s", locctr, label, opcode, operand);
        //iterate till the end of intermediate file to find the length of program (end -start)
    length = ((int)strtol(locctr, NULL, 16)) - trstart;
    
    rewind(intermediate); //move file pointer to beginning to start from the beginning
    fscanf(intermediate, "\t%s\t%s\t%s", label, opcode, operand);
    
    if ( (strcmp(opcode, "START")) == 0 ) 
    {
        fprintf(objectcode, "H^%-6s^%06X^%06X\n", label, trstart, length); //insert the header record to object code
        fprintf(listing, "\t%s\t%s\t%s\t\n", label, opcode, operand); //insert the first line to assembly listing
        fscanf(intermediate, "%s\t%s\t%s\t%s", locctr, label, opcode, operand);//read second input line
    }
    else 
    {
        printf("Intermediate file error");
        exit(0);
    } 
    
    textrecbuff[0]='\0'; //initialize text record empty
    while (strcmp(opcode, "END")) 
    { 

        if( (strcmp(opcode, "RESW")) ==0 || (strcmp(opcode, "RESB")) ==0) 
        {
            fprintf(listing, "%s\t%s\t%s\t%s\t\n", locctr, label, opcode, operand); //write the line to assembly listing. 
            fscanf(intermediate, "%s\t%s\t%s\t%s", locctr, label, opcode, operand); //Read the next input from intermediate file as RESW and RESB don't have object code
        }
        else if( (strcmp(opcode, "WORD")) ==0) 
        {
            sprintf(objcode, "%06X", atoi(operand)); //set the operand as object code
            objcode[6] = '\0';
            fprintf(listing, "%s\t%s\t%s\t%s\t%s\n", locctr, label, opcode, operand, objcode);
            if ( (trlength + 3) <= 30 )
                updateTextRec(3);
            else
                writeTextRec(3); //if size is less than 30 bytes add text record to buffer else write the record to file
        }
        else if( (strcmp(opcode, "BYTE")) ==0) 
        {
            int j = 0;
            if (operand[0] == 'C')
                for (int i = 2; operand[i] != '\''; i++) //eg: C'EOF'. index 2 is used to start from E the actual content and discarding C''
                {
                    sprintf(objcode + j, "%02X", (unsigned char)operand[i]); //hexadecimal of ASCII each character is stored in array
                    j += 2; 
                }
            else if (operand[0] == 'X')
                for (int i = 2; operand[i] != '\''; i++) //eg: X'F1'. index 2 is used to start from F the actual content
                    objcode[j++] = operand[i];
            objcode[j] = '\0';
            fprintf(listing, "%s\t%s\t%s\t%s\t%s\n", locctr, label, opcode, operand, objcode);
            if( (trlength + (j / 2)) <= 30 )
                updateTextRec(j / 2);
            else
                writeTextRec(j / 2); //add to buffer if size less than 30 else write to object code
        }
        else 
        {
            int opcodeAddr = searchfile(optab, opcode);
            int symbolAddr = searchfile(symtab, operand);
            sprintf(objcode, "%02X%04X", opcodeAddr, symbolAddr); //generate object code as string concatination of opcode address and symbol address
            objcode[6] = '\0';
            fprintf(listing, "%s\t%s\t%s\t%s\t%s\n", locctr, label, opcode, operand, objcode); //write object code to assembly listing
            if( (trlength + 3) <= 30 )
                updateTextRec(3); 
            else    
                writeTextRec(3);
        }
        if( (strcmp(opcode, "END") ==0) && strlen(textrecbuff)) //if opcode reached END write the buffer to text record if not written
            fprintf(objectcode, "T^%06X^%02X%s\n", trstart, trlength, textrecbuff);
        
    }
    
    fprintf(listing, "%s\t%s\t%s\t%s\t\n", locctr, label, opcode, operand);
    fprintf(objectcode, "E^%06X", startaddr); //write the end record
    fclose(optab), fclose(symtab), fclose(listing), fclose(objectcode), fclose(intermediate);
}