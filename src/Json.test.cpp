#include "Json.h"

#include <stdio.h>

#define CHUNK_SIZE 1024
int main(int argc, char** argv)
{
    FILE* file = stdin;
    if (argc == 2)
    {
        file = fopen(argv[1], "r");
        if (file == nullptr)
        {
            printf("Error: Cannot open input file '%s'.", argv[1]);
            return EXIT_FAILURE;
        }
    }

    std::string buffer;
    while (true)
    {
        char chunk[CHUNK_SIZE + 1];
        if (fgets(chunk, CHUNK_SIZE, file) == nullptr)
            break;

        buffer += chunk;
    }

    JsonValue root;
    if (!root.Parse(buffer.c_str()))
        return EXIT_FAILURE;

    printf("%s", root.ToString().c_str());

    if (file != stdin)
        fclose(file);

    return EXIT_SUCCESS;
}
