#include "record.h"

void record_copy(struct record *dst, const struct record *src)
{
    dst->pos = src->pos;
    dst->type = src->type;
    dst->data = src->data;
    dst->is_deleted = src->is_deleted;
}
