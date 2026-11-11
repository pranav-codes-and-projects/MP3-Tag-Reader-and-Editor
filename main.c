#include <stdio.h>
#include <string.h>
#include "view.h"
#include "edit.h"

void print_help(void)
{
    printf("\nMP3 TAG READER AND EDITOR\n");
    printf("-------------------------\n");
    printf("View : ./a.out -v sample.mp3\n");
    printf("Edit : ./a.out -e -t <new_data> sample.mp3\n");
    printf("       ./a.out -e -a <new_data> sample.mp3\n");
    printf("       ./a.out -e -A <new_data> sample.mp3\n");
    printf("       ./a.out -e -y <new_data> sample.mp3\n");
    printf("       ./a.out -e -m <new_data> sample.mp3\n");
    printf("       ./a.out -e -c <new_data> sample.mp3\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        print_help();
        return 0;
    }

    if (strcmp(argv[1], "--help") == 0)
    {
        print_help();
    }
    else if (strcmp(argv[1], "-v") == 0)
    {
        if (argc != 3)
        {
            printf("Usage: ./a.out -v sample.mp3\n");
            return 0;
        }

        view_tags(argv[2]);
    }
    else if (strcmp(argv[1], "-e") == 0)
    {
        if (argc != 5)
        {
            printf("Usage: ./a.out -e <option> <new_data> sample.mp3\n");
            return 0;
        }

        edit_tag(argv[2], argv[3], argv[4]);
    }
    else
    {
        printf("Invalid option\n");
        print_help();
    }

    return 0;
}
