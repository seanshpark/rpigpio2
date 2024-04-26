#ifndef __GPIOLIB_UTIL_H__
#define __GPIOLIB_UTIL_H__

#include <stddef.h>
#include <stdint.h>

#define INVALID_ADDRESS ((uint64_t)~0)

#define ARRAY_SIZE(_a) (sizeof(_a) / sizeof(_a[0]))

namespace rpigpio2
{

typedef struct dt_subnode_iter *DT_SUBNODE_HANDLE;

char *read_text_file(const char *fname, size_t *plen);
void *read_file(const char *fname, size_t *plen);

void dt_set_path(const char *path);
char *dt_read_prop(const char *node, const char *prop, size_t *len);
uint32_t *dt_read_cells(const char *node, const char *prop, unsigned *num_cells);
uint64_t dt_extract_num(const uint32_t *cells, int size);
uint64_t dt_read_num(const char *node, const char *prop, size_t size);
uint32_t dt_read_u32(const char *node, const char *prop);
uint64_t dt_parse_addr(const char *node);
void dt_free(void *value);

DT_SUBNODE_HANDLE dt_open_subnodes(const char *node);
const char *dt_next_subnode(DT_SUBNODE_HANDLE handle);
void dt_close_subnodes(DT_SUBNODE_HANDLE handle);

} // namespace rpigpio2

#endif // __GPIOLIB_UTIL_H__
