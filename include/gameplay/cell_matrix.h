#pragma once

#include "gameplay/cell.h"
#include "utils/vector.h"

struct CellMatrix {
    Cell *matrix;
    VectorU size;
};

void init_cell_matrix(CellMatrix *cell_matrix);
void allocate_cell_matrix(CellMatrix *cell_matrix, VectorU size);
void free_cell_matrix(CellMatrix *cell_matrix);

static inline void fill_cell_matrix(CellMatrix *cell_matrix, const Cell cell) {
    for (uint32_t i = 0; i < cell_matrix->size.x * cell_matrix->size.y; i++) {
        cell_matrix->matrix[i] = cell;
    }
}

static inline Cell *get_cell_ref_xy(CellMatrix *cell_matrix, int32_t row, int32_t column) {
    return &cell_matrix->matrix[cell_matrix->size.x * row + column];
}

static inline Cell *get_cell_ref_xy_wrapped(CellMatrix *cell_matrix, int32_t row, int32_t column) {
    int32_t safe_row = row % (int32_t)cell_matrix->size.y;
    safe_row = (uint32_t)(safe_row < 0 ? safe_row + (int32_t)cell_matrix->size.y : safe_row);

    int32_t safe_column = column % (int32_t)cell_matrix->size.x;
    safe_column = (uint32_t)(safe_column < 0 ? safe_column + (int32_t)cell_matrix->size.x : safe_column);

    return &cell_matrix->matrix[cell_matrix->size.x * safe_row + safe_column];
}

static inline void set_cell_xy(CellMatrix *cell_matrix, int32_t row, int32_t column, Cell cell) {
    cell_matrix->matrix[cell_matrix->size.x * row + column] = cell;
}

static inline Cell *get_cell_ref_vec(CellMatrix *cell_matrix, Vector position) {
    return &cell_matrix->matrix[cell_matrix->size.x * position.y + position.x];

    return nullptr;
}

static inline Cell *get_cell_ref_vec_wrapped(CellMatrix *cell_matrix, Vector position) {
    int32_t safe_row = position.y % (int32_t)cell_matrix->size.y;
    safe_row = (uint32_t)(safe_row < 0 ? safe_row + (int32_t)cell_matrix->size.y : safe_row);

    int32_t safe_column = position.x % (int32_t)cell_matrix->size.x;
    safe_column = (uint32_t)(safe_column < 0 ? safe_column + (int32_t)cell_matrix->size.x : safe_column);

    return &cell_matrix->matrix[cell_matrix->size.x * safe_row + safe_column];
}

static inline void set_cell_vec(CellMatrix *cell_matrix, Vector position, Cell cell) {
    cell_matrix->matrix[cell_matrix->size.x * position.y + position.x] = cell;
}

static inline Cell *get_cell_ref_i(CellMatrix *cell_matrix, uint32_t index) { return &cell_matrix->matrix[index]; }

static inline void set_cell_i(CellMatrix *cell_matrix, uint32_t index, Cell cell) { cell_matrix->matrix[index] = cell; }
