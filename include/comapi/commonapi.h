#ifndef COMMON_API_H_
#define COMMON_API_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void* comApi_handle;

comApi_handle comApi_create();

void comApi_destroy();

int comApi_fd(comApi_handle handle);

void comApi_handle_event(comApi_handle handle);

#ifdef __cplusplus
}
#endif


#endif
