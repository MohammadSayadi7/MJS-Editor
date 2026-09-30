#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t size;
    size_t capacity;
    char *data;
} TextBuffer;

void addCharToBuffer(TextBuffer *tb, char item) {
    if (tb->size == tb->capacity) {
        size_t newCapacity = tb->capacity * 2;

        char *tmp = realloc(tb->data, newCapacity * sizeof(char));

        if (tmp == NULL) {
            printf("Failed to add capacity!");
            return;
        }

        printf("Added more capacity to buffer\nCurrent size: %dBytes\n", newCapacity);

        tb->data = tmp;
        tb->capacity = newCapacity;
    }

    tb->data[tb->size] = item;
    tb->size++;
}

void readFileByCharacter(FILE *filePointer, TextBuffer *tb) {
    do {
        char c = fgetc(filePointer);

        if (feof(filePointer)) {
            break;
        }

        addCharToBuffer(tb, c);
    } while(1);
}

int main(int argc, char *argv[]) {

    if (argc == 1) {
        printf("Usage: mjs <filename>");
        return 1;
    }

    FILE *fp;
    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        printf("Could not find file '%s'", argv[1]);
        return 1;
    }

    printf("File has opened successfuly!\n\n");
    
    TextBuffer tb;

    tb.size = 0;
    tb.capacity = 16;
    tb.data = malloc(tb.capacity * sizeof(char));

    if (tb.data == NULL) {
        printf("Failed to allocate memory for buffer!");
        return 1;
    }

    // load characters to buffer
    readFileByCharacter(fp, &tb);
    
    // close file
    fclose(fp);

    // print characters
    printf("\nFile content:\n");
    int index;
    for (index = 0; index < tb.size; index++) {
        printf("%c", tb.data[index]);
    }

    // free allocated memory
    printf("\n\nFree buffer memory");
    free(tb.data);
    tb.data = NULL;
    
    return 0;
}