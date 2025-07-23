#include "entity.h"
#include "globals.h"
#include "linkedlist.h"
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

// ENTITY UTILITY

BoundingBox entity_bounding_box(struct entity *item) {
  Vector3 position = item->position;
  float height = item->height;
  float width = item->width;
  float length = item->length;
  return (BoundingBox){
      (Vector3){position.x - height / 2, position.y - width / 2,
                position.z - length / 2},
      (Vector3){position.x + height / 2, position.y + width / 2,
                position.z + length / 2}};
}

void entity_collision_check(entity *p, struct list *entities) {
  struct list *ncol = entities;
  BoundingBox pbox = entity_bounding_box(p);
  while (ncol != NULL) {
    if (ncol->data == p) {
      ncol = ncol->next;
      continue;
    }
    BoundingBox nbox = entity_bounding_box(ncol->data);
    if (CheckCollisionBoxes(pbox, nbox)) {
      p->onCollide(p);
    }
    ncol = ncol->next;
  }
}

void entity_process(struct entity *proj, eevents event) {
  if (event == RENDER && proj->render != NULL)
    proj->render(proj);
  else if (event == UPDATE && proj->update != NULL)
    proj->update(proj);
  else if (event == COLLID && proj->onCollide != NULL)
    entity_collision_check(proj, proj->owner->entity_list);
  return;
}

void entity_free(struct entity *proj) {
  entities_free_entity(proj->owner, proj);
}
// WALL
void wall_render(struct entity *proj) {
  DrawCube(proj->position, proj->width, proj->height, proj->length,
           proj->color);
}

entity *wall_create(Vector3 position, float height, float width, float length,
                    Color color) {
  entity *p = MemAlloc(sizeof(entity));
  *p = (entity){.position = position,
                .destination = (Vector3){0, 0, 0},
                .height = height,
                .width = width,
                .length = length,
                .color = color,
                .speed = 0.0,
                .origin = position,
                .render = wall_render,
                .update = NULL,
                .onCollide = NULL};
  return p;
}
// END WALL
// PROJECTILE

bool projectile_finished(struct entity *proj) {
  return false;
  //return Vector3Equals(proj->position, proj->destination);
}

void projectile_update(struct entity *proj) {
  proj->position =
      Vector3MoveTowards(proj->position, proj->destination, proj->speed);
  if (projectile_finished(proj)) {
    entity_free(proj);
  }
}

void projectile_onCollide(struct entity *proj) {
  Vector3 temp = proj->destination;
  proj->destination = proj->origin;
  proj->origin = temp;
  return;
}

void projectile_render(struct entity *proj) {
  DrawCube(proj->position, proj->width, proj->height, proj->length,
           proj->color);
}

entity *projectile_create(Vector3 position, Vector3 destination) {
  entity *p = MemAlloc(sizeof(entity));

  *p = (entity){.position = position,
                .destination = destination,
                .origin = position,
                .height = PHEIGHT,
                .width = PWIDTH,
                .length = PLENGTH,
                .speed = PSPEED,
                .type = PROJECTILE,
                .color = (Color)RED,
                .update = projectile_update,
                .render = projectile_render,
                .onCollide = projectile_onCollide};
  return p;
}

// END PROJECTILE

// ENTITIES UTILITY

struct entities *entities_create() {
  struct entities *p = MemAlloc(sizeof(struct entities));
  p->entity_list = NULL;
  return p;
}

void entities_process(struct entities *e, eevents event) {
  struct list *l = e->entity_list;
  while (l != NULL) {
    struct list *ln = l->next;
    entity_process(l->data, event);
    l = ln;
  }
};

void entities_destroy(struct entities *e) {
  if (e->entity_list == NULL)
    return;
  struct list *l = e->entity_list;
  while (l != NULL) {
    l = list_remove(l, l->data);
  }
  MemFree(e);
};

void entities_free_entity(struct entities *e, struct entity *ent) {
  e->entity_list = list_remove(e->entity_list, ent);
}

void entities_add(struct entities *e, entity *ent) {
  struct list *l = e->entity_list;
  ent->owner = e;
  e->entity_list = list_add(l, ent);
}