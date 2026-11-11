#include <stdio.h>
#include <string.h>
#include "edit.h"

static int convert_size(unsigned char size[])
{
    int value;

    value = (size[0] << 24) |
            (size[1] << 16) |
            (size[2] << 8)  |
             size[3];

    return value;
}

void convert_to_big_endian(int value, unsigned char size[])
{
    size[0] = (value >> 24) & 0xFF;
    size[1] = (value >> 16) & 0xFF;
    size[2] = (value >> 8) & 0xFF;
    size[3] = value & 0xFF;
}

void edit_tag(char option[], char new_data[], char filename[])
{
    FILE *source;
    FILE *temp;

    char tag[5];
    unsigned char size[4];
    unsigned char flags[3];
    char data[1000];
    char required_tag[5];
    char ch;

    int old_size;
    int new_size;
    int found = 0;
    int i;

    if (strcmp(option, "-t") == 0)
        strcpy(required_tag, "TIT2");
    else if (strcmp(option, "-a") == 0)
        strcpy(required_tag, "TPE1");
    else if (strcmp(option, "-A") == 0)
        strcpy(required_tag, "TALB");
    else if (strcmp(option, "-y") == 0)
        strcpy(required_tag, "TYER");
    else if (strcmp(option, "-m") == 0)
        strcpy(required_tag, "TCON");
    else if (strcmp(option, "-c") == 0)
        strcpy(required_tag, "COMM");
    else
    {
        printf("Invalid edit option\n");
        return;
    }

    source = fopen(filename, "rb");

    if (source == NULL)
    {
        printf("Unable to open file\n");
        return;
    }

    temp = fopen("temp.mp3", "wb");

    if (temp == NULL)
    {
        printf("Unable to create temp.mp3\n");
        fclose(source);
        return;
    }

    // Read and write 10 bytes of header */
    for (i = 0; i < 10; i++)
    {
        fread(&ch, 1, 1, source);
        fwrite(&ch, 1, 1, temp);
    }

    while (1)
    {
        // Read 4 bytes of tag */
        if (fread(tag, 1, 4, source) != 4)
            break;

        tag[4] = '\0';

        // End of ID3 frames */
        if (tag[0] == '\0')
        {
            fwrite(tag, 1, 4, temp);

            /* Copy remaining data from source to temp */
            while (fread(&ch, 1, 1, source) == 1)
                fwrite(&ch, 1, 1, temp);

            break;
        }

        // Check whether fetched tag matches required tag */
        if (strcmp(tag, required_tag) == 0)
        {
            found = 1;

            /* Write 4 bytes of tag */
            fwrite(tag, 1, 4, temp);

            /* Read old size */
            fread(size, 1, 4, source);
            old_size = convert_size(size);

            /* New size = encoding byte + new data */
            new_size = strlen(new_data) + 1;
            convert_to_big_endian(new_size, size);

            /* Write new size */
            fwrite(size, 1, 4, temp);

            /* Read and write 3 bytes: 2 flags + 1 encoding */
            fread(flags, 1, 3, source);
            fwrite(flags, 1, 3, temp);

            /* Write new data */
            fwrite(new_data, 1, strlen(new_data), temp);

            /* Skip old data. Encoding byte was already handled. */
            fseek(source, old_size - 1, SEEK_CUR);
        }
        else
        {
            /* Non-matching tag: copy it unchanged */
            fwrite(tag, 1, 4, temp);

            /* Read and write old size */
            fread(size, 1, 4, source);
            fwrite(size, 1, 4, temp);

            old_size = convert_size(size);

            /* Read and write 3 bytes */
            fread(flags, 1, 3, source);
            fwrite(flags, 1, 3, temp);

            /* Read and write old data */
            fread(data, 1, old_size - 1, source);
            fwrite(data, 1, old_size - 1, temp);
        }
    }

    fclose(source);
    fclose(temp);

    if (found == 0)
    {
        printf("Tag not found\n");
        remove("temp.mp3");
        return;
    }

    /* Remove original and rename temporary file */
    remove(filename);
    rename("temp.mp3", filename);

    printf("Tag edited successfully\n");
}
