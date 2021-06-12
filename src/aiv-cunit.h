#ifndef AIV_CUNIT_H
#define AIV_CUNIT_H
#include <stdio.h>
#include <string.h>
#include <math.h>

const char* __cunit_rel_src(const char* path) {
    const char* subpath = strstr(path, "\\src");
    if (subpath == NULL) subpath = strstr(path, "\\tests");
    if (subpath == NULL) return path;
    return subpath+1;
}

int __failure_count = 0;

#define CUNIT_INT_EQ(exp, res) if (exp != res) {\
    __failure_count++;\
    printf("> %s: expected '%d' but was '%d' [%s@%d]\n", __FUNCTION__, exp, res, __cunit_rel_src(__FILE__), __LINE__);\
    return; \
}

#define CUNIT_IS_TRUE(exp) if (exp == 0) {\
    __failure_count++;\
    printf("> %s: expected '%s' but was '%s' [%s@%d]\n", __FUNCTION__, "true", "false", __cunit_rel_src(__FILE__), __LINE__);\
    return; \
}

#define CUNIT_FLOAT_EQ(exp, res) if (fabsf((exp) - (res)) > 0.000001f) {\
    __failure_count++;\
    printf("> %s: expected '%f' but was '%f' [%s@%d]\n", __FUNCTION__, exp, res, __cunit_rel_src(__FILE__), __LINE__);\
    return; \
}


#define CUNIT_TEST(name) void name()

#define CUNIT_RUNNER(...) int main() {\
void (*func_ptr[])() = {__VA_ARGS__}; \
int funct_ptr_size = sizeof(func_ptr);\
int ptr_size = sizeof(func_ptr[0]); \
int funct_ptr_count = funct_ptr_size / ptr_size; \
for(int i=0; i<funct_ptr_count; i++) { \
    func_ptr[i](); \
} \
int passed = funct_ptr_count - __failure_count;\
printf("Total: %d, Passed: %d, Failed: %d\n", funct_ptr_count, passed, __failure_count);\
return 0;\
}

#endif /* AIV_CUNIT_H */