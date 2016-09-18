/*
Nathan Guenther
n3197119
Program 1 Hill Cipher
6-16-2014
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


int main()
{

    // Initilaize variables
    int i = 0, j = 0, k = 0;
    int size, flag, length = 0, counter = 0;
    int temp1, temp2;
    char alphabet[27] = "abcdefghijklmnopqrstuvwxyz";

    int scan[10000];
    char filename[30];

    int key [10][10], plaintemp[10000], ciphertemp[10000];
    char plain[10000], cipher[10000];

    FILE *keyfile;
    FILE *plainfile;
    FILE *cipherfile;

    // Recieve input from user
    printf("Please enter the name of the file storing the key.\n");
    scanf("%s", filename);

     // Create key file
    keyfile = fopen(filename, "r");

    printf("Please enter the name of the file to encrypt.\n");
    scanf("%s", filename);

    // Create Input File
    plainfile = fopen(filename, "r");

    printf("Please enter the name of the file to store the ciphertext.\n");
    scanf("%s", filename);

    // Scan in key file to temporary location
    while((fscanf(keyfile, "%d", &scan[i])) != EOF)
    {
        // Find key size special first case
        if (i == 0)
        {
            size = scan[0];
        }

        // Increment until end of file
        i++;
    }

    // Reset index to start at matrix (size already recorded)
    i = 1;

    // Loop through marix rows
    for (j = 0; j < size; j++)
    {
        // Loop through columns
        for (k = 0; k < size; k++)
        {
            // Create encrpyion key matrix
            key[j][k] = scan[i];
            i++;
        }
    }

    // Reset index before plaintext part
    i = 0;


    // Scan in plaintext file to encrpyt
    while((fscanf(plainfile, "%c", &plain[i])) != EOF)
    {
        // Reset flag check
        flag = 0;

        // Check for valid character
        if( isalpha(plain[i]) )
        {
            flag = 1;

            // Eliminate Case Sensitive issues -> Force Lowercase
            plain[i] = tolower(plain[i]);
        }

        // If valid character, increment up
        if (flag == 1)
            i++;
    }

    // Save length of plaintext
    length = i - 1;

    // Check if leftover characters
    if (((length + 1) % size) != 0)
    {
        // Calculate Remainder
        temp1 = (length + 1) % size;

        // Calculate extra padding
        temp2 = size - temp1;

        // Insert neccessary padding as x's
        for (i = 1; i <= temp2; i++)
        {
            plain[length + i] = 'x';
            counter++;
        }

        // Update length with padding
        length = length + counter;
    }

    // Reset counter
    counter = 0;


    // Loop to prepare plaintext
    for(i = 0; i <= length; i++)
    {
        // Covert to numerical equivalent
        plaintemp[i] = (plain[i] - 'a');
    }

    // Create ciphertext
    for (counter = 0; counter <= length;)
    {
        // Loop through Rows
        for (j = 0; j < size; j++)
        {
            // Reset math temp variables for each section
            temp2 = 0;
            temp1 = 0;

            // Loop through columns
            for (k = 0; k < size; k++)
            {
                // Matrix multiplication
                 temp1 = (key[j][k] * plaintemp[counter + k]);
                 temp2 = temp2 + temp1;
            }

            // Mod 26 to fit in range of alphavet
            ciphertemp[counter + j] = (temp2 % 26);

            // Convert back to characters
            cipher[counter + j] = (alphabet[ciphertemp[counter + j]]);
        }

        // Increment to next section
        counter = counter + size;
    }

    // Reset counter
    counter = 0;

    // Create output file
    cipherfile = fopen(filename, "w");

    // Loop through cipher
    for (i = 0; i <= length; i++)
    {
        // Check for end of line (80 chars)
        if (counter >= 80)
        {
            // New line and reset counter
            fprintf(cipherfile, "\n");
            counter = 0;
        }

        // Print to file each letter
        fprintf(cipherfile, "%c", cipher[i]);
        counter++;
    }

    // Cleanup
    fclose(keyfile);
    fclose(plainfile);
    fclose(cipherfile);

    // Exit
    return 0;
}
