#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **keys;
    void **values;
    int size;
    int capacity;
} Map;

typedef struct {
    int width;
    int height;
    char **keys;
    Map *data;
} Map2D;

void map_init(Map* map) {
    map->size = 0;
    map->capacity = 4;

    map->keys = malloc(sizeof(char*) * map->capacity);
    map->values = malloc(sizeof(void*) * map->capacity);

    if(!map->keys || !map->values) {
        printf("Memory allocation failed\n");
        exit(1);
    }
}

void map_init_capacity(Map* map, int capacity) {
    map->size = 0;
    map->capacity = capacity;

    map->keys = malloc(sizeof(char*) * map->capacity);
    map->values = malloc(sizeof(void*) * map->capacity);

    if(!map->keys || !map->values) {
        printf("Memory allocation failed\n");
        exit(1);
    }
}

void map_resize(Map* map) {
    map->capacity *= 2;

    map->keys = realloc(map->keys, sizeof(char*) * map->capacity);
    map->values = realloc(map->values, sizeof(void*) * map->capacity);

    if(!map->keys || !map->values) {
        printf("Memory reallocation failed\n");
        exit(1);
    }
}

void map_put(Map* map, char* key, void* value) {
    for (int i =0; i < map->size; i++) {
        if(strcmp(map->keys[i], key) == 0) {
            map->values[i] = value;
            return;
        }
    }

    if (map->size == map->capacity) {
        map_resize(map);
    }

    map->keys[map->size] = strdup(key);
    map->values[map->size] = value;

    map->size++;
}

void* map_get(Map* map, char* key) {
    for (int i = 0; i < map->size; i++) {
        if (strcmp(map->keys[i], key) == 0) {
            return map->values[i];
        }
    }
    return NULL;
}

void print_map(Map* map) {
    for (int i = 0; i < map->size; i++) {
        printf("%s : %f\n", map->keys[i], *(float*)map->values[i]);
    }
}

void map_free(Map* map) {
    if (!map) return;

    for(int i = 0; i < map->size; i++) {
        free(map->keys[i]);
    }

    free(map->keys);
    free(map->values);

    map->keys = NULL;
    map->values = NULL;
    map->size = 0;
    map->capacity = 0;
}


// -- Map 2D Functions -- //
void map2d_init(Map2D *map2d, int width, int height, int map2d_capacity) {
    map2d->height = height;
    map2d->width = width;
    map2d->data = malloc(sizeof(Map) * height);
    map2d->keys = malloc(sizeof(char*) * height);
    if (!map2d->data || !map2d->keys) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    for (int i = 0; i < height; i++) {
        map_init_capacity(&map2d->data[i], map2d_capacity);
    }
}

void map2d_put(Map2D *map2d, int row, char* key2d, const char* key, void* value) {
    if (row < 0 || row >= map2d->height) {
        printf("Row out of bounds!\n");
        return;
    }
    map2d->keys[row] = key2d;
    map_put(&map2d->data[row], key, value);
}

void map2d_put_map(Map2D *map2d, int row, char* key, Map *input_map) {
    if (row < 0 || row >= map2d->height) {
        printf("Row out of bounds!\n");
        return;
    }
    map2d->keys[row] = key;
    for (int i = 0; i < input_map->size; i++) {
        map_put(&map2d->data[row], input_map->keys[i], input_map->values[i]);
    }
}

void* map2d_get(Map2D* map2d, const char* key) {
    void* res;
    for (int i = 0; i < map2d->height; i++) {
        res = map_get(&map2d->data[i], key);
    }
    return res;
}

void print_map2d(Map2D* map2d) {
    for (int i = 0; i < map2d->height; i++) {
        printf("[%s]\n", map2d->keys[i]);
        print_map(&map2d->data[i]);
    }
}

void map2d_free(Map2D* map2d) {
    if(!map2d || !map2d->data) return;

    for (int i = 0; i < map2d->height; i++) {
        Map *row = &map2d->data[i];
        for (int j = 0; j < row->size; j++) {
            free(row->values[j]);
        }
        map_free(row);
    }

    free(map2d->data);
    free(map2d->keys);
    map2d->data = NULL;
    map2d->keys = NULL;
    map2d->height = 0;
    map2d->width = 0;
}

void set_static_matrix() {
    // -- Paper vs Others Map -- //
    Map paper_map;
    map_init(&paper_map);
    float *paper_p = malloc(sizeof(float));
    *paper_p = 2.0f / 3.0f;
    float *paper_r = malloc(sizeof(float));
    *paper_r = 1.0f / 3.0f;
    float *paper_s = malloc(sizeof(float));
    *paper_s = 0.0f / 3.0f;

    map_put(&paper_map, "Paper", paper_p);
    map_put(&paper_map, "Rock",  paper_r);
    map_put(&paper_map, "Scissors", paper_s);

    // -- Rock vs Others Map -- //
    Map rock_map;
    map_init(&rock_map);
    float *rock_p = malloc(sizeof(float));
    *rock_p = 0.0f / 3.0f;
    float *rock_r = malloc(sizeof(float));
    *rock_r = 2.0f / 3.0f;
    float *rock_s = malloc(sizeof(float));
    *rock_s = 1.0f / 3.0f;

    map_put(&rock_map, "Paper", rock_p);
    map_put(&rock_map, "Rock",  rock_r);
    map_put(&rock_map, "Scissors", rock_s);

    // -- Scissors vs Others Map -- //
    Map scissors_map;
    map_init(&scissors_map);
    float *scissors_p = malloc(sizeof(float));
    *scissors_p = 2.0f / 3.0f;
    float *scissors_r = malloc(sizeof(float));
    *scissors_r = 0.0f / 3.0f;
    float *scissors_s = malloc(sizeof(float));
    *scissors_s = 1.0f / 3.0f;

    map_put(&scissors_map, "Paper", scissors_p);
    map_put(&scissors_map, "Rock",  scissors_r);
    map_put(&scissors_map, "Scissors", scissors_s);

    // -- Initializing 2D Map -- //
    Map2D st_matrix;
    map2d_init(&st_matrix, 3, 3, 9);
    map2d_put_map(&st_matrix, 0, "Paper", &paper_map);
    map2d_put_map(&st_matrix, 1, "Rock", &rock_map);
    map2d_put_map(&st_matrix, 2, "Scissors", &scissors_map);

    print_map2d(&st_matrix);

    map2d_free(&st_matrix);
    map_free(&paper_map);
    map_free(&rock_map);
    map_free(&scissors_map);
}

int main()
{
    set_static_matrix();
    return 0;
}
