#include <circular_buffer.h>

#define BUFFER_GUARD UINT16_MAX

enum buffer_init_status
circular_buffer_init(circular_buffer *p_circular_buffer, uint16_t *array, uint16_t circular_buffer_capacity)
{
    if (circular_buffer_capacity < 2)
    {
        return INIT_SIZE_INVALID;
    }
    p_circular_buffer->buffer_capacity = circular_buffer_capacity - 1;
    p_circular_buffer->buffer_count = 0;
    p_circular_buffer->head = 0;
    p_circular_buffer->tail = 0;
    p_circular_buffer->buffer = array;
    p_circular_buffer->buffer[p_circular_buffer->buffer_capacity] = BUFFER_GUARD;
    return INIT_SUCCESS;
}

bool circular_buffer_put(circular_buffer *p_circular_buffer, uint16_t item)
{
    if (p_circular_buffer->buffer_count >= p_circular_buffer->buffer_capacity)
    {
        return false;
    }
    p_circular_buffer->buffer[p_circular_buffer->head++] = item;
    if (p_circular_buffer->head == p_circular_buffer->buffer_capacity)
    {
        p_circular_buffer->head = 0;
    }
    p_circular_buffer->buffer_count++;
    return true;
}

uint16_t circular_buffer_get(circular_buffer *p_circular_buffer)
{
    if (p_circular_buffer->buffer == NULL)
    {
        return BUFFER_ERROR_NONEXISTENT;
    }
    if (p_circular_buffer->buffer_count < 1)
    {
        return BUFFER_ERROR_EMPTY;
    }
    uint16_t value = p_circular_buffer->buffer[p_circular_buffer->tail++];
    if (p_circular_buffer->tail == p_circular_buffer->buffer_capacity)
    {
        p_circular_buffer->tail = 0;
    }
    p_circular_buffer->buffer_count--;
    return value;
}

bool circular_buffer_is_empty(circular_buffer *p_circular_buffer)
{
    return p_circular_buffer->buffer_count == 0;
}

bool circular_buffer_is_full(circular_buffer *p_circular_buffer)
{
    return p_circular_buffer->buffer_count == p_circular_buffer->buffer_capacity;
}

uint16_t circular_buffer_get_count(circular_buffer *p_circular_buffer)
{
    return p_circular_buffer->buffer_count;
}

uint16_t circular_buffer_get_capacity(circular_buffer *p_circular_buffer)
{
    return p_circular_buffer->buffer_capacity;
}

bool circular_buffer_verify_integrity(circular_buffer *p_circular_buffer)
{
    return p_circular_buffer->buffer[p_circular_buffer->buffer_capacity] == BUFFER_GUARD;
}

void circular_buffer_clear(circular_buffer *p_circular_buffer)
{
    p_circular_buffer->head = 0;
    p_circular_buffer->tail = 0;
    p_circular_buffer->buffer_count = 0;
}