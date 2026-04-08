#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

// -- Map Functions -- //
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

void map_put(Map* map, const char* key, void* value) {
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

void* map_get(Map* map, const char* key) {
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

void map2d_put(Map2D *map2d, const char* key2d, const char* key, void* value) {
    for (int i = 0; i < map2d->height; i++) {
        if (strcmp(map2d->keys[i], key2d) == 0) {
            map_put(&map2d->data[i], key, value);
            return;
        }
    }
    printf("Row key not found: %s\n", key2d);
}

void map2d_put_map(Map2D *map2d, int row, const char* key, Map *input_map) {
    if (row < 0 || row >= map2d->height) {
        printf("Row out of bounds!\n");
        return;
    }
    map2d->keys[row] = key;
    for (int i = 0; i < input_map->size; i++) {
        map_put(&map2d->data[row], input_map->keys[i], input_map->values[i]);
    }
}

void* map2d_get(Map2D *map2d, const char* key) {
    void* res;
    for (int i = 0; i < map2d->height; i++) {
        res = map_get(&map2d->data[i], key);
    }
    return res;
}

Map* map2d_get_row(Map2D *map2d, const char* key) {
    Map* result_map;
    for (int i = 0; i < map2d->height; i++) {
        if (strcmp(map2d->keys[i], key) == 0) {
            result_map = &map2d->data[i];
            return result_map;
        }
    }
    return NULL;
}

Map2D map2d_copy(const Map2D *map2d) {
    Map2D output_map;
    map2d_init(&output_map, map2d->width, map2d->height, (map2d->height * map2d->width));

    for (int i = 0; i < map2d->height; i++) {
        output_map.keys[i] = map2d->keys[i];

        Map *map2d_row = &map2d->data[i];
        for (int j = 0; j < map2d_row->size; j++) {
            float *new_val = malloc(sizeof(float));
            *new_val = *(float*)map2d_row->values[j];

            map_put(&output_map.data[i], map2d_row->keys[j], new_val);
        }
    }

    return output_map;
}

void print_map2d(Map2D *map2d) {
    for (int i = 0; i < map2d->height; i++) {
        printf("[%s]\n", map2d->keys[i]);
        print_map(&map2d->data[i]);
    }
}

void map2d_free(Map2D *map2d) {
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


// -- Main Game Logic -- //
void set_static_matrix(Map2D *st_matrix) {
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

    // -- Initializing 2D Map -- /
    map2d_init(st_matrix, 3, 3, 9);
    map2d_put_map(st_matrix, 0, "Paper", &paper_map);
    map2d_put_map(st_matrix, 1, "Rock", &rock_map);
    map2d_put_map(st_matrix, 2, "Scissors", &scissors_map);

    print_map2d(st_matrix);
}

int scores(const char* static_move, const char* learning_move) {
    int reward = 0;
    if (strcmp(static_move, learning_move) == 0) { reward = 0; }
    else if ((strcmp(static_move, "Paper") == 0 && strcmp(learning_move, "Rock") == 0) ||
        (strcmp(static_move, "Scissors") == 0 && strcmp(learning_move, "Paper") == 0) ||
        (strcmp(static_move, "Rock") == 0 && strcmp(learning_move, "Scissors") == 0)) { reward = 1; }
    else { reward = -1; }
    return reward;
}

const char* weighted_random_choice(Map *row) {
    float r = (float)rand() / RAND_MAX;

    float cumulative = 0.0f;
    for (int i = 0; i < row->size; i++) {
        cumulative += *(float *)row->values[i];
        if (r <= cumulative) {
            return row->keys[i];
        }
    }
    return row->keys[row->size - 1];
}

const char* static_player(const char* last_move, Map2D *static_matrix) {
    Map* row_map;
    row_map = map2d_get_row(static_matrix, last_move);
    if(!row_map) {
        printf("Something went wrong!\n");
        exit(-1);
    }
    return weighted_random_choice(row_map);
}

const char* learning_player(const char* last_move, const char* op_last_move, int reward, Map2D *transition_matrix) {
    Map* row_map = map2d_get_row(transition_matrix, op_last_move);
    if(!row_map) {
        printf("Something went wrong!\n");
        exit(-1);
    }
    float *old_value = (float*)map_get(row_map, last_move);
    *old_value = fmaxf(*old_value += (reward * *old_value * 0.9), 1e-6f);

    float sum = 0.0f;
    for (int i = 0; i < row_map->size; i++) {
        sum += *(float*)row_map->values[i];
    }
    for (int i = 0; i < row_map->size; i++) {
        *(float*)row_map->values[i] /= sum;
    }

    return weighted_random_choice(row_map);
}

int main()
{
    srand(time(NULL));
    Map2D static_matrix;
    set_static_matrix(&static_matrix);
    Map2D learning_matrix = map2d_copy(&static_matrix);
    const char* states[] = {"Paper", "Rock", "Scissors"};

    const char* static_move = states[rand() % 3];
    const char* learning_move = static_move;
    int *static_history = malloc(sizeof(int) * 1000);
    int *learning_history = malloc(sizeof(int) * 1000);
    int score = scores(static_move, learning_move);
    int static_reward = score;
    int learning_reward = -score;

    for (int i = 0; i < 1000; i++) {
        const char* last_static_move = static_move;
        const char* last_learning_move = learning_move;


        static_move = static_player(last_static_move, &static_matrix);

        learning_move = learning_player(last_learning_move, last_static_move, learning_reward, &learning_matrix);

        score = scores(static_move, learning_move);
        static_reward = score;
        learning_reward = -score;

        static_history[i] = score;
        learning_history[i] = -score;
    }
    printf("Static History:\n[");
    for (int i = 0; i < 1000; i++) {
        printf("%d", static_history[i]);
        if (i < 1000 - 1) printf(", ");
    }
    printf("]\nLearning History:\n[");
    for (int i = 0; i < 1000; i++) {
        printf("%d", learning_history[i]);
        if (i < 1000 - 1) printf(", ");
    }
    printf("]\n");

    // -- Free allocated memory -- //
    map2d_free(&static_matrix);
    free(static_history);
    free(learning_history);
    map2d_free(&learning_matrix);
    return 0;
}
