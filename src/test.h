#ifndef TEST_H
#define TEST_H

typedef struct {
    char *name;
    char *value;
} AttrTypeA;

typedef struct {
    char *name;
    char *value;
} AttrTypeB;

typedef enum {
    ATTR_TYPE_A,
    ATTR_TYPE_B
} AttrType;

typedef struct {
    AttrType type;
    union {
        AttrTypeA a;
        AttrTypeB b;
    } data;
} Element;

typedef struct {
    char *name;
    Element *elements;
    int element_count;
} Section; 

typedef struct {
    int id;
    char *title;
    Section *sections;
    int section_count;
} Main;

#endif
