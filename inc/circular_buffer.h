#include <common.h>
#include <limits.h>

#define BUFFER_ERROR_EMPTY UINT16_MAX
#define BUFFER_ERROR_NONEXISTENT (BUFFER_ERROR_EMPTY - 1)

enum buffer_init_status
{
    INIT_SUCCESS,
    INIT_SIZE_INVALID,
};

typedef struct
{
    uint16_t *buffer;
    uint16_t buffer_capacity;
    uint16_t buffer_count;
    uint16_t head;
    uint16_t tail;
} circular_buffer;

enum buffer_init_status circular_buffer_init(circular_buffer *p_circular_buffer, uint16_t *buffer, uint16_t buffer_capacity);
bool circular_buffer_put(circular_buffer *p_circular_buffer, uint16_t item);
uint16_t circular_buffer_get(circular_buffer *p_circular_buffer);
bool circular_buffer_is_empty(circular_buffer *p_circular_buffer);
bool circular_buffer_is_full(circular_buffer *p_circular_buffer);
uint16_t circular_buffer_get_count(circular_buffer *p_circular_buffer);
uint16_t circular_buffer_get_capacity(circular_buffer *p_circular_buffer);
bool circular_buffer_verify_integrity(circular_buffer *p_circular_buffer);
void circular_buffer_clear(circular_buffer *p_circular_buffer);