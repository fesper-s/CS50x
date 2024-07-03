#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
 
int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    // Create a buffer for a block of data
    uint8_t buffer[512];

    // While there's still data left to read from the memory card
    int counter = 0;
    FILE *new_file = NULL;
    char filename[8];
    while (fread(buffer, 1, 512, card) == 512)
    {
        // Create JPEGs from the data
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // Close the previous file
            if (new_file != NULL)
            {
                fclose(new_file);
            }

            // Create a new file
            sprintf(filename, "%03i.jpg", counter);
            new_file = fopen(filename, "w");
            if (new_file == NULL)
            {
                return 1;
            }

            // Write the block of data to the new file
            fwrite(buffer, 1, 512, new_file);
            counter++;
        }
        else if (new_file != NULL)
        {
            // Continue writing to the current file
            fwrite(buffer, 1, 512, new_file);
        }
    }
    fclose(new_file);
    fclose(card);
    return 0;
}