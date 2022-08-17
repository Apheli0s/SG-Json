#ifndef SG_TYPE_H
#define SG_TYPE_H

#include "map"     //map
#include "memory"  //unique_ptr,make_unique()
#include "array"   //array
#include "vector"  //vector
#include "variant" //variant
#include "string"  //stod()

#include "Json.h"
#include "JsonValue.h"

using sg_type = enum { SG_NULL,
                       SG_BOOL,
                       SG_NUMBER,
                       SG_STRING,
                       SG_OBJECT,
                       SG_VECBOOL,
                       SG_VECNUMBER,
                       SG_VECSTRING,
                       SG_VECOBJECT
};
#endif
