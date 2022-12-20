#include "Json.h"

int main(int argc, char** argv)
{
    FILE* file = fopen("example.json", "rb");
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);

    char* buffer = new char[size + 1];
    fseek(file, 0, SEEK_SET);
    fread(buffer, size, 1, file);
    buffer[size] = '\0';

    JsonValue root;
    JsonParser jsonParser;
    jsonParser.Parse(buffer, root);
    printf("%s", root.ToStdString(0, "    ").c_str());

    return EXIT_SUCCESS;
}
