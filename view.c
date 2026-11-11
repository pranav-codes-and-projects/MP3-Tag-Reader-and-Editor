#include <stdio.h>
#include <string.h>
#include "view.h"

static int convert_size(unsigned char size[])
{
    int value;

    value = (size[0] << 24) |
            (size[1] << 16) |
            (size[2] << 8)  |
             size[3];

    return value;
}

void view_tags(char filename[])
{
    FILE *source;
    char tag[5];
    unsigned char size[4];
    char data[1000];
    int frame_size;
    int i;

    char tags[6][2][20] =
    {
        {"TIT2", "Title"},
        {"TPE1", "Artist"},
        {"TALB", "Album"},
        {"TYER", "Year"},
        {"TCON", "Content type"},
        {"COMM", "Comment"}
    };

    source = fopen(filename, "rb");

    if (source == NULL)
    {
        printf("Unable to open file\n");
        return;
    }

    // Skip 10 byte ID3v2 header 
    fseek(source, 10, SEEK_SET);

    printf("\n--------------------------------\n");
    printf("          MP3 TAG DETAILS\n");
    printf("--------------------------------\n");

    while (1)
    {
        // Read 4 bytes of tag 
        if (fread(tag, 1, 4, source) != 4)
            break;

        tag[4] = '\0';

        // ID3 padding means tag section is over 
        if (tag[0] == '\0')
            break;

        // Read 4 bytes of size 
        if (fread(size, 1, 4, source) != 4)
            break;

        frame_size = convert_size(size);

        if (frame_size <= 0 || frame_size >= 1000)
            break;

        // 2 bytes flags + 1 byte encoding 
        fseek(source, 3, SEEK_CUR);

        // Size includes the encoding byte 
        if (fread(data, 1, frame_size - 1, source) != frame_size - 1)
            break;

        data[frame_size - 1] = '\0';

        for (i = 0; i < 6; i++)
        {
            if (strcmp(tag, tags[i][0]) == 0)
            {
                printf("%-15s : %s\n", tags[i][1], data);
                break;
            }
        }
    }

    printf("--------------------------------\n");

    fclose(source);
}
