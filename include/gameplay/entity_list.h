#pragma once

#include "gameplay/entity.h"
#include "types.h"

#define ENTITY_BLOCK_LENGTH 1024

struct EntityBlock {
    Entity entities[ENTITY_BLOCK_LENGTH];
    uint32_t size;
    EntityBlock *previous;
    EntityBlock *next;
};

struct EntityList {
    EntityBlock *head;
    EntityBlock *tail;
    uint32_t size;
};

struct EntityListIterator {
    EntityBlock *block;
    uint32_t block_index;
    Entity *entity;
};

static inline void init_entity_list_iterator(EntityListIterator *iterator, EntityList *entities) {
    iterator->block = entities->head;
    iterator->block_index = 0;

    if (entities->head != nullptr && entities->head->size > 0) {
        iterator->entity = &entities->head->entities[0];
    } else {
        iterator->entity = nullptr;
    }
}

static inline void init_entity_block(EntityBlock *block, EntityBlock *previous) {
    block->size = 0;
    block->previous = previous;
    block->next = nullptr;

    if (previous != nullptr) {
        previous->next = block;
    }
}

static inline Entity *allocate_entity_on_block(EntityBlock *block) { return &block->entities[block->size++]; }

static inline void init_entity_list(EntityList *entities) {
    entities->head = nullptr;
    entities->tail = nullptr;
    entities->size = 0;
}

static inline EntityBlock *allocate_entity_block(EntityList *entities) {
    EntityBlock *block = malloc(sizeof(EntityBlock));
    init_entity_block(block, entities->tail);

    if (entities->head == nullptr) {
        entities->head = block;
    }

    entities->tail = block;

    return block;
}

static inline Entity *allocate_entity(EntityList *entities) {
    entities->size++;

    if (entities->tail != nullptr && entities->tail->size < ENTITY_BLOCK_LENGTH) {
        return allocate_entity_on_block(entities->tail);
    }

    EntityBlock *block = allocate_entity_block(entities);

    return allocate_entity_on_block(block);
}

static inline EntityListIterator *iterate_on_entity_list(EntityListIterator *iterator) {
    iterator->block_index++;

    if (iterator->block_index < iterator->block->size) {
        iterator->entity = &iterator->block->entities[iterator->block_index];
        return iterator;
    }

    iterator->block = iterator->block->next;
    iterator->block_index = 0;

    if (iterator->block == nullptr || iterator->block->size == 0) {
        return nullptr;
    }

    iterator->entity = &iterator->block->entities[iterator->block_index];

    return iterator;
}

static inline void free_entity_list(EntityList *entities) {
    EntityBlock *block = entities->head;

    while (block != nullptr) {
        EntityBlock *next = block->next;

        free(block);

        block = next;
    }

    entities->head = nullptr;
    entities->tail = nullptr;
    entities->size = 0;
}
