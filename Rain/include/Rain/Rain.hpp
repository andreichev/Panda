#pragma once

#include "GlobalData.hpp"

// --------------------------------
// ---------- MACROS --------------
// --------------------------------

#define _RAIN_CONCAT_(a, b) a##b

#define RAIN_FIELDS_BEGIN(T) RAIN_FIELDS_BEGIN_INTERNAL(T, __COUNTER__)
#define RAIN_FIELDS_BEGIN_INTERNAL(T, ID)                                                          \
    namespace Rain {                                                                               \
    struct _RAIN_CONCAT_(RainAutoRegisterClass_, ID) {                                             \
        _RAIN_CONCAT_(RainAutoRegisterClass_, ID)() {                                              \
            using ClassType = T;                                                                   \
            TypeInfo &typeInfo = findOrCreateType<ClassType>();

#define RAIN_FIELD(name)                                                                           \
    registerField<StripType<decltype(ClassType::name)>>(                                           \
        typeInfo, #name, offsetof(ClassType, name), alignof(decltype(ClassType::name))             \
    );

#define RAIN_FIELDS_END RAIN_FIELDS_END_INTERNAL(__COUNTER__)
#define RAIN_FIELDS_END_INTERNAL(ID)                                                               \
    }                                                                                              \
    }                                                                                              \
    _RAIN_CONCAT_(rainAutoRegister, ID);                                                           \
    }
// ---------------------------------------------------
// ---------- MACRO REPLACEMENT EXAMPLE --------------
// ---------------------------------------------------

#if false

class Person {
public:
    int age;
    std::string name;

    Person(int age, std::string name)
        : age(age)
        , name(std::move(name)) {}
};

struct AutoRegisterClass_Example {
    AutoRegisterClass_Example() {
        using ClassType = Person;
        TypeInfo &typeInfo = findOrCreateType<ClassType>();
        registerField<StripType<decltype(ClassType::age)>>(
            typeInfo, "age", offsetof(ClassType, age),
            alignof(decltype(ClassType::age))
        );
        registerField<StripType<decltype(ClassType::name)>>(
            typeInfo, "name", offsetof(ClassType, name),
            alignof(decltype(ClassType::name))
        );
    }
} mExample;

#endif