#include "gameplay/cell_matrix.h"

#include "utils/vector.h"

void init_cell_matrix(CellMatrix *cell_matrix) {
    cell_matrix->matrix = nullptr;
    cell_matrix->size = VECTORU_ZERO;
}

void allocate_cell_matrix(CellMatrix *cell_matrix, VectorU size) {
    cell_matrix->size = size;

    if (cell_matrix->matrix == nullptr) {
        cell_matrix->matrix = malloc(sizeof(Cell) * size.x * size.y);
    } else {
        cell_matrix->matrix = realloc(cell_matrix->matrix, sizeof(Cell) * size.x * size.y);
    }
}

void free_cell_matrix(CellMatrix *cell_matrix) {
    if (cell_matrix->matrix != nullptr) {
        free(cell_matrix->matrix);
        cell_matrix->matrix = nullptr;
    }
}
