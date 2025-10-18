#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"

Main* create_dummy_data() {
    Main *main_obj = malloc(sizeof(Main));
    if (!main_obj) return NULL;

    main_obj->id = 1;
    main_obj->title = strdup("Dummy Main Object");

    main_obj-> section_count = 2;
    main_obj->sections = malloc(sizeof(Section) * main_obj->section_count);
    if (!main_obj->sections) {
        free(main_obj);
        return NULL;
    }

    for (int i = 0; i < main_obj->section_count; i++) {
        Section *sec = &main_obj->sections[i];
        char sec_name[32];
        snprintf(sec_name, sizeof(sec_name), "Section_%d", i);
        sec->name = strdup(sec_name);
        sec->element_count = 3;
        sec->elements = malloc(sizeof(Element) * sec->element_count);
        if (!sec->elements) {
            continue;
        }
        for (int j = 0; j < sec->element_count; j++) {
            Element *elem = &sec->elements[j];
            if ((j % 2) == 0) {
                elem->type = ATTR_TYPE_A;
                elem->data.a.name = strdup("attr a name");
                elem->data.a.value = strdup("attr a value");
            } else {
                elem->type = ATTR_TYPE_A;
                elem->data.b.name = strdup("attr b name");
                elem->data.b.value = strdup("attr b value");
            }
        }
    }
    return main_obj;
}


#include "test.h"
#include <stdlib.h>

void free_main(Main *main_obj) {
    if (!main_obj) return;
    // Free each section
    for (int i = 0; i < main_obj->section_count; i++) {
        Section *sec = &main_obj->sections[i];
        // Free section name
        free(sec->name);
        // Free each element in the section
        for (int j = 0; j < sec->element_count; j++) {
            Element *elem = &sec->elements[j];
            switch (elem->type) {
                case ATTR_TYPE_A:
                    free(elem->data.a.name);
                    free(elem->data.a.value);
                    break;
                case ATTR_TYPE_B:
                    free(elem->data.b.name);
                    free(elem->data.b.value);
                    break;
            }
        }
        // Free the element array
        free(sec->elements);
    }
    // Free the sections array
    free(main_obj->sections);
    // Free the main title
    free(main_obj->title);
    // Finally, free the main object itself
    free(main_obj);
}

