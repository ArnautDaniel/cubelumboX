#include "raylib.h"

struct list {
  struct entity*data;
  struct list *next;
};

struct list *list_create(void *item);
struct list *list_add(struct list *list, void *item);
struct list *list_remove(struct list *list, void *item);
