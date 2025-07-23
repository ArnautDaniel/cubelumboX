#include "linkedlist.h"
#include "entity.h"
#include <raylib.h>
#include <stdlib.h>

struct list *list_create(void *item)
{
  struct list *p = MemAlloc(sizeof(struct list));
  p->next = NULL;
  p->data = item;
  return p;
}

struct list *list_add(struct list *list, void *item)
{
  if (list == NULL)
  {
    return list_create(item);
  }

  struct list *p = list_create(item);
  p->next = list;
  return p;
}

void list_free(struct list *list)
{
  MemFree(list->data);
  MemFree(list);
}

struct list *list_remove(struct list *list, void *item)
{

  if (list == NULL)
  {
    return NULL;
  }

  if (list->data == item)
  {
    struct list *p = list->next;
    list_free(list);
    return p;
  }

  struct list *list_prev = list;
  struct list *list_cur = list->next;

  while (list_cur != NULL)
  {
    if (list_cur->data == item)
    {
      struct list *p = list_cur->next;
      list_free(list_cur);
      list_prev->next = p;
      return list;
    }
    list_prev = list_cur;
    list_cur = list_cur->next;
  }

  return list;
}
