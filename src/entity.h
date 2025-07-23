#include <raylib.h>

typedef struct entity entity;

typedef void (*UpdateFunc)(entity *);
typedef void (*RenderFunc)(entity *);
typedef void (*CollisionFunc)(entity *);

typedef enum
{
  PROJECTILE,
  STATIC,
} etype;

typedef enum
{
  RENDER,
  UPDATE,
  COLLID
} eevents;

struct list;

struct entity
{
  Vector3 position;
  Vector3 destination;
  Vector3 origin;
  float height, width, length;
  float speed;
  Color color;
  etype type;
  struct entities *owner;
  UpdateFunc update;
  RenderFunc render;
  CollisionFunc onCollide;
};

struct entities
{
  struct list *entity_list;
};

struct entities *entities_create();
void entities_process(struct entities *e, eevents event);
void entities_destroy(struct entities *e);
void entities_add(struct entities *e, entity *ent);
void entities_free_entity(struct entities *e, struct entity *ent);

// PROJECTILE
entity *projectile_create(Vector3 position, Vector3 destination);

// WALL

entity *wall_create(Vector3 position, float height, float width, float length,
                    Color color);