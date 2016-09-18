/*
Nathan Guenther
n3197119
Program 2 Hill CRC
7-15-2014
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

// Function prototypes
char *hexToBin(char *input, int length, char *bin);
char *bintoHex(char *input, int length, char *hex);
int validate(char *input, int length);
char *exclusiveOR(char *input, char *poly, int length, int counter, int j);
char *calculateCRC(char *converted, char *poly, int length, int polylength, int polyPadLength, char *savedCRC);
char *verifyCRC(char *finalBinCRC, char *finalHexCRC, int length, char *hexfile);


int main ()
{
    // Initlizise Variables
    int i = 0, j, option, check, length, polylength, polyPadLength, templength, hexlength = 0, binlength = 0;
    char temp;

    // Character arrays (dynamically allocated as needed)
    char filename[30], polytemp[17] = "10000010110001001";
    char *poly =  malloc(100 * sizeof(char));
    char *hexfile =  malloc(100 * sizeof(char));
    char *bin =  malloc(100 * sizeof(char));
    char *bin2 =  malloc(100 * sizeof(char));
    char *hex = malloc(4 * sizeof(char));
    char *hex2 = malloc(4 * sizeof(char));
    char *savedCRC = malloc(16 * sizeof(char));
    char *savedCRC2 = malloc(16 * sizeof(char));
    char *updatedfile =  hexfile;
    char *finalCRC[4];

    // Pointers to reslts returned from functions
    char *converted = NULL;
    char *finalBinCRC = NULL;
    char *finalHexCRC = NULL;

    // Initalize file pointers
    FILE *input = NULL;
    FILE *output = NULL;

    // Verify File Exists or reprompt
    while(input == NULL)
    {
        printf("Enter the name of the file you want to check: ");
        scanf("%s", filename);
        input = fopen(filename, "r");
    }

    // Scan in input file
    while((fscanf(input, "%c", &temp)) != EOF)
    {
        hexfile[i] = temp;
        i++;
    }

    // Save length of input
    length = i;

    // Set up polynomial array
    for (i = 0; i < 17; i++)
    {
        poly[i] = polytemp[i];
    }

    // Fixed poly length
    polylength = 17;
    polyPadLength = polylength;

    // Pad zeroes to poly for simple calculation later
    for (i = polyPadLength; i < (length * 4 + 16); i++)
    {
        poly[i] = '0';
        polyPadLength++;
    }


    // Validate File
    check = validate(hexfile, length);
    while(check == 1)
    {
        // Invalid input -> Reprompt
        printf("Input file format is invalid, please try again.\n\n");
        printf("Enter the name of the file you want to check: ");
        scanf("%s", filename);
        input = fopen(filename, "r");
        check = validate(hexfile, length);
    }


    //----------------------------------------------//
    //   ****         MAIN MENU LOOP        ****   //
    // Allows options to be selected multiple times //
    //----------------------------------------------//
    printf("\n-------------Menu-------------\n");
    printf("1. Calculate CRC\n2. Verify CRC\n3. Exit\n");
    printf("Choose from the above menu: ");
    scanf("%d", &option);
    while (option != 3)
    {
        // Calculate CRC
        if(option == 1)
        {
            // Print out file in hex
            printf("The input file (hex): ");
            for (i = 0; i < length; i++)
            {
                printf("%c", hexfile[i]);
            }

            // Convert hex input to binary
            converted = hexToBin(hexfile, length, bin);

            // Find new length and save original
            hexlength = length;
            length = length *4;

            // Reset check to use as counter
            check = 0;

            // Print out file in binary
            printf("\nThe input file (bin): ");
            for (i = 0; i < length; i++)
            {
                // Add space for each 4 bits
                if (check == 4)
                {
                    printf(" ");
                    check = 0;
                }
                printf("%c", converted[i]);
                check++;
            }

            // Reset check to use as counter
            check = 0;

            // Print polynomial code
            printf("\nThe polynomial that was used (binary bit string): ");
            for(i = 0; i < polylength; i++)
            {
                if(check == 4)
                {
                    printf(" ");
                    check = 0;
                }
                printf("%c", poly[i]);
                check++;
            }


            // Temp saved
            templength = length;

            // Pad 16 zeros to binary
            printf("\nWe will append sixteen zeros at the end of the binary input.\n\n");
            for (j = templength; j < (templength + 16); j++)
            {
                converted[j] = '0';
                length++;
            }

            // Calculate CRC function
            printf("The binary string answer at each XOR step of CRC calculation: \n");
            finalBinCRC = calculateCRC(converted, poly, length, polylength, polyPadLength, savedCRC);

            // Print out final CRC
            check = 0;
            printf("\nThus, the CRC is: ");
            for((i = 0); i < 16; i++)
            {
                if(check == 4)
                {
                    printf(" ");
                    check = 0;
                }
                check++;
                printf("%c", finalBinCRC[i]);
            }

            // Convert final CRC to hex
            finalHexCRC = bintoHex(finalBinCRC, length, hex);


            printf(" (bin) = ");
            for(i = 0; i < 4; i++)
                printf("%c", finalHexCRC[i]);
            printf(" (hex)\n");

            // Prepare string to write
            finalHexCRC[4] = '\0';

            // Cleanup file
            fclose(input);

            // Open file before append
            output = fopen(filename, "a");

            // Aphend CRC to file
            fprintf(output, "%s", finalHexCRC);

            // Consider CRC legth
            hexlength = hexlength + 4;
            printf("CRC has been appended to the end of the input file.\n");

            // Cleanup
            fclose(output);

            // Reopen to read in updated file
            output = fopen(filename, "r");

            i = 0;
            // Read in updated input file
            while(i < hexlength)
            {
                fscanf(output, "%c", &temp);
                updatedfile[i] = temp;
                i++;
            }

            // Find length and save old
            templength = length;
            length = i;

            // Print out final file output.
            printf("Reading input file again: ");
            for (i = 0; i < hexlength; i++)
            {
                printf("%c", updatedfile[i]);
            }

            // Cleanup file
            printf("\nClosing input file.\n");
            fclose(output);
        }

        // Verify CRC
        else if(option == 2)
        {
            // Re inialize pointers before verify
            converted = NULL;
            bin = NULL;
            finalBinCRC = NULL;
            finalHexCRC = NULL;
            updatedfile =  hexfile;

            // Decide neccessary length
            if (hexlength != 0)
                length = hexlength;

            // Print out file in hex
            printf("The input file (hex): ");
            for (i = 0; i < length; i++)
            {
                printf("%c", hexfile[i]);
            }

            // Convert hex imput to binary
            converted = hexToBin(hexfile, length, bin2);

            // Find new length and save original
            hexlength = length;
            length = length * 4;
            binlength = length;

            // Reset to use as counter
            check = 0;

            // Print out file in binary
            printf("\nThe input file (bin): ");
            for (i = 0; i < length; i++)
            {
                // Add space for each 4 bits
                if (check == 4)
                {
                    printf(" ");
                    check = 0;
                }


                printf("%c", converted[i]);
                check++;
            }

            printf("\nThe polynomial that was used (bin): ");
            for(i = 0; i < polylength; i++)
            {
                if(check == 4)
                {
                    printf(" ");
                    check = 0;
                }
                printf("%c", poly[i]);
                check++;
            }
            printf("\n");





            check = 0;
            printf("The 16-bit CRC at the end of the file: ");
            // Print out final CRC BIN
            for (i = (binlength - 16); i < binlength; i ++)
            {
                if(check == 4)
                {
                    printf(" ");
                    check = 0;
                }
                check++;
                printf("%c", converted[i]);
            }
            printf(" (bin) = ");

            // Print out final CRC HEX
            for(i = (hexlength - 4); i < hexlength; i++)
                printf("%c", hexfile[i]);
            printf(" (hex)\n");




            // Remove previous CRC before passing to recalculate
            length = length - 16;

            // Reset to use as counter
            check = 0;

            // Print polynomial code


            // Temp save
            templength = length;

            // Pad 16 zeros to binary
            for (j = templength; j < (templength + 16); j++)
            {
                converted[j] = '0';
                length++;
            }


            // Seperate length for poly array with 0's padded
            polyPadLength = polylength;

            // Pad zeroes to poly
            for (i = polyPadLength; i < length; i++)
            {
                poly[i] = '0';
                polyPadLength++;
            }






            // Calculate CRC
            printf("\nThe binary string answer at each XOR step of CRC verification: \n");
            finalBinCRC = calculateCRC(converted, poly, length, polylength, polyPadLength, savedCRC2);

            // Convert final CRC to hex
            finalHexCRC = bintoHex(finalBinCRC, length, hex2);

            // Cleanup
            fclose(input);

            // Re scan in original file before copare
            input = fopen(filename, "r");

            i = 0;
            // Scan in input file
            while((fscanf(input, "%c", &temp)) != EOF)
            {
                hexfile[i] = temp;
                i++;
            }

             // Prepare string to verify
            finalHexCRC[4] = '\0';

            // Reset to previous length before verifying
            length = i;

            // Run verification check on CRC
            verifyCRC(finalBinCRC, finalHexCRC, length, hexfile);

            // Cleanup file
            fclose(input);
        }

        // Invalid menu options
        else if (option != 3)
        {
            printf("\nSorry that is not a valid menu option, please try again.\n");
        }

        // Loop to clean char array
        for (i = 0; i < templength; i++)
        {
            hexfile[i] = '\0';
        }

        // Reprompt file input
        printf("Enter the name of the file you want to check: ");
        scanf("%s", filename);
        input = fopen(filename, "r");

        // Verify File Exists or reprompt
        while(input == NULL)
        {
            printf("Enter the name of the file you want to check: ");
            scanf("%s", filename);
            input = fopen(filename, "r");
        }

        i = 0;
        // Scan in input file
        while((fscanf(input, "%c", &temp)) != EOF)
        {
            hexfile[i] = temp;
            i++;
        }
        // Terminate end of string
        hexfile[i + 1] = '\0';

        // Find length of input
        length = i;


        // Validate File
        check = validate(hexfile, length);
        while(check == 1)
        {
            // Invalid input -> Reprompt
            printf("Input file format is invalid, please try again.\n\n");
            printf("Enter the name of the file you want to check: ");
            scanf("%s", filename);
            input = fopen(filename, "r");
            check = validate(hexfile, length);
        }

        // Set up polynomial array
        for (i = 0; i < 17; i++)
        {
            poly[i] = polytemp[i];
        }

        // Fixed poly length
        polylength = 17;

        // Seperate length for poly array with 0's padded
        polyPadLength = polylength;

        // Pad zeroes to poly for easier XOR calculation
        for (i = polyPadLength; i < (length * 4 + 16); i++)
        {
            poly[i] = '0';
            polyPadLength++;
        }

        // Re inialize variables
        i = 0;
        check = 0;

        // Re-display Menu
        //----------INPUT---------//
        printf("\n-------------Menu-------------\n");
        printf("1. Calculate CRC\n2. Verify CRC\n3. Exit\n");
        printf("Choose from the above menu: ");
        scanf("%d", &option);

    } // End of Main Menu While Loop

    // Exit
    return 0;
}

// Convert hex input to binary
char *hexToBin(char *input, int length, char *bin)
{
    int i = 0;

    // Update length
    length = length * 4;

    // Fixed converstions
    char *binValues[16] =
	{
		"0000", "0001", "0010", "0011",
		"0100", "0101", "0110", "0111",
		"1000", "1001", "1010", "1011",
		"1100", "1101", "1110", "1111",
	};

    // Loop to convert each hex value
    while (i < (length / 4))
    {
        // Cases for all possible input
        switch (input[i])
        {
            case '0': strcat(bin, binValues[0]); break;
            case '1': strcat(bin, binValues[1]); break;
            case '2': strcat(bin, binValues[2]); break;
            case '3': strcat(bin, binValues[3]); break;
            case '4': strcat(bin, binValues[4]); break;
            case '5': strcat(bin, binValues[5]); break;
            case '6': strcat(bin, binValues[6]); break;
            case '7': strcat(bin, binValues[7]); break;
            case '8': strcat(bin, binValues[8]); break;
            case '9': strcat(bin, binValues[9]); break;
            case 'A': strcat(bin, binValues[10]); break;
            case 'B': strcat(bin, binValues[11]); break;
            case 'C': strcat(bin, binValues[12]); break;
            case 'D': strcat(bin, binValues[13]); break;
            case 'E': strcat(bin, binValues[14]); break;
            case 'F': strcat(bin, binValues[15]); break;
            default: break;
        }
        i++;
    }

    return bin;
}

// Convert binary input to hex
char *bintoHex(char *input, int length, char *hex)
{
    int i = 0, j = 0, k = 0;

    // Fixed converstions
    char *binValues[16] =
	{
		"0000", "0001", "0010", "0011",
		"0100", "0101", "0110", "0111",
		"1000", "1001", "1010", "1011",
		"1100", "1101", "1110", "1111",
	};
    char hexValues[16] = "0123456789ABCDEF";

    // Nested loops to find match between hex and binary digits
    for(j = 0; j < 4; j++)
    {
        for(i = 0; i < 16; i++)
        {
            if(input[k + 0] == binValues[i][0])
                if(input[k + 1] == binValues[i][1])
                    if(input[k + 2] == binValues[i][2])
                        if(input[k + 3] == binValues[i][3])
                        {
                            hex[j] = hexValues[i];
                            k = k + 4;
                            break;
                        }
        }
    }

    // Add null terminator for string
    hex[4] = '\0';

    return hex;
}

// Checks input validity
int validate(char *input, int length) // Always returning 1 ????
{
    int i, flag = 0;

    // Loops through each characer of input
    for (i = 0; i < length; i++)
    {
        // Ensures hexadecimal values only
        if (isxdigit(input[i]) == 0)
        {
            flag = 1;
            break;
        }
    }

    // If invalid contentds
    if (flag == 1)
        return 1;

    // Success
    return 0;
}

// XOR operation function
char *exclusiveOR(char *input, char *poly, int length, int counter, int j)
{
    int i, check = 0;
    char *XOR = input;

    // Loop to computer XOF for each collum
    for(i = 0; i < (length - counter); i++)
	{
        // Same = 0 | Difference = 1
		if(input[i + j] == poly[i])
			XOR[i] = '0';
		else
			XOR[i] = '1';

        // Format spcing
        if(check == 4)
        {
            printf(" ");
            check = 0;
        }
        check++;
        printf("%c", XOR[i]);
	}
	printf("\n");

    // Return pointer to XOR'ed value
    return XOR;
}

// CRC calculation function
char *calculateCRC(char *converted, char *poly, int length, int polylength, int polyPadLength, char *savedCRC)
{
    int i, counter = 0, flag = 0, check = 0, lastcounter = 0, j = 0;
    char *temp[16];
    char *OXOR = NULL;
    char *NXOR = NULL;
    char *TempXOR = NULL;

    // Main loop to continue operation CRC until complete
    while((length - counter) >= polylength)
    {
        // First run case
        if (flag == 0)
        {
            // For each column calculate XOR result from input and poly
            TempXOR = exclusiveOR(converted, poly, length, counter, j);
            OXOR = TempXOR;
        }

        // Each subsequent run case
        else
        {
            // For each column calculate XOR result from previous XOR and poly
            TempXOR = exclusiveOR(OXOR, poly, length, counter, j);
            NXOR = TempXOR;
        }

        // Backup counter incase last run
        lastcounter = counter;
        j = 0;

        // First run case
        if (flag == 0)
        {
            // Add zeros to left of poly (shift over -->)
            while(OXOR[j] != '1')
            {
                counter++;
                j++;
            }
            flag = 1;
        }

        // Each subsequent case
        else
        {
            // Add zeros to left of poly (shift over -->)
            while(NXOR[j] != '1')
            {
                counter++;
                j++;
            }
        }

        check = 0;

        // Exit if about to go out of bounds
        if (counter > (length - polylength))
        {
            check = length - lastcounter;
            check++;
            check = check - polylength;
            break;
        }
    }


    j = 0;
    savedCRC = NXOR;
     // Extract CRC from final result
    for (i = check; i < (16 + check); i++)
    {
        savedCRC[j] = NXOR[i];
        j++;
    }

    return savedCRC;
}

char *verifyCRC(char *finalBinCRC, char *finalHexCRC, int length, char *hexfile)
{
    int i, check = 0, otherlength = 16;

    // Prepare string to write
    finalHexCRC[4] = '\0';

    // Reset check
    check = 0;

    // Loop to compare calculated hash to file hash
    for(i = 0; i < 4; i++)
    {
        // Matches so continue
        if (finalHexCRC[i] == hexfile[length - 4 + i ])
        {
            ;
        }

        // Incorrect CRC abort
        else
        {
            check = 1;
            break;
        }
    }

    printf("\nDid the CRC check pass? (Yes or No): ");
    if(check == 0)
        printf("Yes\n");
    else
        printf("No\n");
}
