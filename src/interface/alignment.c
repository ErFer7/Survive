#include "interface/alignment.h"

VectorU aligned_position(Vector position, VectorU size, VectorU parent_size, enum Alignment alignment) {
    Vector aligned_position = position;
    uint32_t width = size.x;
    uint32_t height = size.y;

    switch (alignment) {
        case TOP:
            aligned_position.x += (parent_size.x - width) / 2;
            break;
        case TOP_RIGHT:
            aligned_position.x += (parent_size.x + width / 2) - 1;
            break;
        case LEFT:
            aligned_position.y += (parent_size.y - height) / 2;
            break;
        case CENTER:
            aligned_position.x += (parent_size.x - width) / 2;
            aligned_position.y += (parent_size.y - height) / 2;
            break;
        case RIGHT:
            aligned_position.x += (parent_size.x + width / 2) - 1;
            aligned_position.y += (parent_size.y - height) / 2;
            break;
        case BOTTOM_LEFT:
            aligned_position.y += (parent_size.y + height / 2) - 1;
            break;
        case BOTTOM:
            aligned_position.x += (parent_size.x - width) / 2;
            aligned_position.y += (parent_size.y + height / 2) - 1;
            break;
        case BOTTOM_RIGHT:
            aligned_position.x += (parent_size.x + width / 2) - 1;
            aligned_position.y += (parent_size.y + height / 2) - 1;
            break;
        default:
            aligned_position.x = 0;
            aligned_position.y = 0;
            break;
    }

    return vector_to_vector_u(aligned_position);
}
