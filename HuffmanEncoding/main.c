#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256
#define INVALID_BIT_READ -1
#define INVALID_BIT_WRITE -1
#define FAILURE -1
#define SUCCESS 0
#define END_OF_FILE -1

typedef struct {
    int index;
    unsigned int weight;
} node_t;
typedef node_t * node_ptr;
node_ptr nodes = NULL;

int num_chars = 256;
int num_active = 0;
int *frequency = NULL;
unsigned int original_size = 0;
int num_nodes = 0;
int *leaf_index = NULL;
int *parent_index = NULL;
int free_index = 1;
int *stack;
int stack_head;
unsigned char buffer[MAX_BUFFER_SIZE];
int bits_in_buffer = 0;
int current_bit = 0;
int eof_input = 0;

int f_read_head(FILE *f);
int f_write_head(FILE *f);
int f_read_bit(FILE *f);
int f_write_bit(FILE *f, int bit);
int f_flush_buff(FILE *f);
void f_decode_bits(FILE *fin, FILE *fout);
void f_encode_alpha(FILE *fout, int c);
void tree_build();
void tree_add_leaves();
int tree_add_node(int index, int weight);
void init();
int encode(const char* ifile, const char *ofile);
int decode(const char* ifile, const char *ofile);
void destroy();


int main(int argc, char **argv) {
    if (argc != 4) {
        puts("Please enter the correct number of arguments");
        // USAGE: ./huffman [encode | decode]
        // example: gcc main.c -o main; ./main encode test.txt encode.txt
        return FAILURE;
    }

    init();
    
    if (strcmp(argv[1], "encode") == 0)
        encode(argv[2], argv[3]);
    else if (strcmp(argv[1], "decode") == 0)
        decode(argv[2], argv[3]);

    destroy();
    return SUCCESS;
}

void determine_frequency(FILE *f) {
    int c;
    while ((c = fgetc(f)) != EOF) {
        ++frequency[c];
        ++original_size;
    }
    for (c = 0; c < num_chars; ++c)
    {
        if (frequency[c] > 0)
            ++num_active;
    }
}

void init() {
    frequency = calloc(2 * num_chars, sizeof(int));
    leaf_index = frequency + num_chars - 1;
}

void allocate_tree() {
    nodes = (node_ptr) calloc(2 * num_active, sizeof(node_t));
    parent_index = (int *) calloc(num_active, sizeof(int));
}

void destroy() {
    free(parent_index);
    free(frequency);
    free(nodes);
}

int tree_add_node(int index, int weight) {
    int i = num_nodes++;
    while (i > 0 && nodes[i].weight > weight) {
        memcpy(&nodes[i + 1], &nodes[i], sizeof(node_t));
        if (nodes[i].index < 0)
            ++leaf_index[-nodes[i].index];
        else
            ++parent_index[nodes[i].index];
        --i;
    }

    ++i;
    nodes[i].index = index;
    nodes[i].weight = weight;
    if (index < 0)
        leaf_index[-index] = i;
    else
        parent_index[index] = i;

    return i;
}

void tree_add_leaves() {
    int i, freq;
    for (i = 0; i < num_chars; ++i) {
        freq = frequency[i];
        if (freq > 0)
            tree_add_node(-(i + 1), freq);
    }
}

void tree_build() {
    int a, b, index;
    while (free_index < num_nodes) {
        a = free_index++;
        b = free_index++;
        index = tree_add_node(b/2,
            nodes[a].weight + nodes[b].weight);
        parent_index[b/2] = index;
    }
}


int encode(const char* ifile, const char *ofile) {
    FILE *fin, *fout;
    if ((fin = fopen(ifile, "rb")) == NULL) {
        perror("Failed to open input file");
        return FAILURE;
    }
    if ((fout = fopen(ofile, "wb")) == NULL) {
        perror("Failed to open output file");
        fclose(fin);
        return FAILURE;
    }

    determine_frequency(fin);
    stack = (int *) calloc(num_active - 1, sizeof(int));
    allocate_tree();

    tree_add_leaves();
    f_write_head(fout);
    tree_build();
    fseek(fin, 0, SEEK_SET);
    int c;
    while ((c = fgetc(fin)) != EOF)
        f_encode_alpha(fout, c);
    f_flush_buff(fout);
    free(stack);
    fclose(fin);
    fclose(fout);

    return 0;
}

void f_encode_alpha(FILE *fout, int c) {
    int node_index;
    stack_head = 0;
    node_index = leaf_index[c + 1];
    while (node_index < num_nodes) {
        stack[stack_head++] = node_index % 2;
        node_index = parent_index[(node_index + 1) / 2];
    }
    while (--stack_head > -1)
        f_write_bit(fout, stack[stack_head]);
}

int decode(const char* ifile, const char *ofile) {
    FILE *fin, *fout;
    if ((fin = fopen(ifile, "rb")) == NULL) {
        perror("Failed to open input file");
        return FAILURE;
    }
    if ((fout = fopen(ofile, "wb")) == NULL) {
        perror("Failed to open output file");
        fclose(fin);
        return FAILURE;
    }

    if (f_read_head(fin) == 0) {
        tree_build();
        f_decode_bits(fin, fout);
    }
    fclose(fin);
    fclose(fout);

    return 0;
}

void f_decode_bits(FILE *fin, FILE *fout) {
    int i = 0, bit, node_index = nodes[num_nodes].index;
    while (1) {
        bit = f_read_bit(fin);
        if (bit == -1)
            break;
        node_index = nodes[node_index * 2 - bit].index;
        if (node_index < 0) {
            char c = -node_index - 1;
            fwrite(&c, 1, 1, fout);
            if (++i == original_size)
                break;
            node_index = nodes[num_nodes].index;
        }
    }
}

int f_write_bit(FILE *f, int bit) {
    if (bits_in_buffer == MAX_BUFFER_SIZE << 3) {
        size_t bytes_written =
            fwrite(buffer, 1, MAX_BUFFER_SIZE, f);
        if (bytes_written < MAX_BUFFER_SIZE && ferror(f))
            return INVALID_BIT_WRITE;
        bits_in_buffer = 0;
        memset(buffer, 0, MAX_BUFFER_SIZE);
    }
    if (bit)
        buffer[bits_in_buffer >> 3] |=
            (0x1 << (7 - bits_in_buffer % 8));
    ++bits_in_buffer;
    return SUCCESS;
}

int f_flush_buff(FILE *f) {
    if (bits_in_buffer) {
        size_t bytes_written =
            fwrite(buffer, 1,
                (bits_in_buffer + 7) >> 3, f);
        if (bytes_written < MAX_BUFFER_SIZE && ferror(f))
            return -1;
        bits_in_buffer = 0;
    }
    return 0;
}

int f_read_bit(FILE *f) {
    if (current_bit == bits_in_buffer) {
        if (eof_input)
            return END_OF_FILE;
        else {
            size_t bytes_read =
                fread(buffer, 1, MAX_BUFFER_SIZE, f);
            if (bytes_read < MAX_BUFFER_SIZE) {
                if (feof(f))
                    eof_input = 1;
            }
            bits_in_buffer = bytes_read << 3;
            current_bit = 0;
        }
    }

    if (bits_in_buffer == 0)
        return END_OF_FILE;
    int bit = (buffer[current_bit >> 3] >>
        (7 - current_bit % 8)) & 0x1;
    ++current_bit;
    return bit;
}

int f_write_head(FILE *f) {
     int i, j, byte = 0,
         size = sizeof(unsigned int) + 1 +
              num_active * (1 + sizeof(int));
     unsigned int weight;
     char *buffer = (char *) calloc(size, 1);
     if (buffer == NULL)
         return FAILURE;

     j = sizeof(int);
     while (j--)
         buffer[byte++] =
             (original_size >> (j << 3)) & 0xff;
     buffer[byte++] = (char) num_active;
     for (i = 1; i <= num_active; ++i) {
         weight = nodes[i].weight;
         buffer[byte++] =
             (char) (-nodes[i].index - 1);
         j = sizeof(int);
         while (j--)
             buffer[byte++] =
                 (weight >> (j << 3)) & 0xff;
     }
     fwrite(buffer, 1, size, f);
     free(buffer);
     return 0;
}

int f_read_head(FILE *f) {
     int i, j, byte = 0, size;
     size_t bytes_read;
     unsigned char buff[4];

     bytes_read = fread(&buff, 1, sizeof(int), f);
     if (bytes_read < 1)
         return END_OF_FILE;
     byte = 0;
     original_size = buff[byte++];
     while (byte < sizeof(int))
         original_size =
             (original_size << (1 << 3)) | buff[byte++];

     bytes_read = fread(&num_active, 1, 1, f);
     if (bytes_read < 1)
         return END_OF_FILE;

     allocate_tree();

     size = num_active * (1 + sizeof(int));
     unsigned int weight;
     char *buffer = (char *) calloc(size, 1);
     if (buffer == NULL)
         return FAILURE;
     fread(buffer, 1, size, f);
     byte = 0;
     for (i = 1; i <= num_active; ++i) {
         nodes[i].index = -(buffer[byte++] + 1);
         j = 0;
         weight = (unsigned char) buffer[byte++];
         while (++j < sizeof(int)) {
             weight = (weight << (1 << 3)) |
                 (unsigned char) buffer[byte++];
         }
         nodes[i].weight = weight;
     }
     num_nodes = (int) num_active;
     free(buffer);
     return 0;
}

