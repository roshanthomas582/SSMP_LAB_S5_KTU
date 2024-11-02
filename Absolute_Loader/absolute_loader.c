//Implementation of simple Absolute Loader in C
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main()
{
    FILE *fp;
    int i, j, length;
    char line[80], arr[10];

    fp = fopen("objectcode.txt", "r");

    printf("The Program name is: ");
    if (fgets(line, sizeof(line), fp)) 
    {
        for (i = 2; line[i]!='^'; i++)
            printf("%c", line[i]);
        printf("\n");
    }

    printf("Memory Address\tObject Code\n");
    do
    {
        fscanf(fp, "%s", line);
        if (line[0] == 'T')
        {
            for (i = 0; i < 6; i++)
                arr[i] = line[i + 2];
            arr[i] = '\0';
            length = strtol(arr, NULL, 16);

            i = 12;
            while (line[i] != '\0')
            {
                if (line[i] != '^')
                {
                    printf("  %X : \t  %c%c\n", length, line[i], line[i + 1]);
                    length++;
                    i = i + 2;
                }
                else
                    i++;
            }
        }
        else if (line[0] == 'E')
            break;
    } while (!feof(fp));

    fclose(fp);
}