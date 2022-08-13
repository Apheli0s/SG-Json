#ifndef SG_TYPE_H
#define SG_TYPE_H

#include "map"    //map
#include "memory" //unique_ptr
#include "array"  //std::array
#include "vector" //std::vector

#include "Json.h"
#include "JsonValue.h"

using sg_type = enum { SG_NULL,
                       SG_BOOL,
                       SG_INT,
                       SG_DOUBLE,
                       SG_STRING,
                       SG_OBJECT,
                       SG_VECBOOL,
                       SG_VECINT,
                       SG_VECDOUBLE,
                       SG_VECSTRING,
                       SG_VECOBJECT
};
#endif