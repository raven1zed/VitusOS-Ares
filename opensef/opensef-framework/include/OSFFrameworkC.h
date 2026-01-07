#ifndef OSF_FRAMEWORK_C_H
#define OSF_FRAMEWORK_C_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * C Wrapper for openSEF Framework
 *
 * Allows pure C code (compositor) to use the C++ framework.
 */

// Opaque types
typedef struct OSFDesktopC OSFDesktopC;
typedef struct OSFWindowC OSFWindowC;

// Framework lifecycle
void osf_framework_init(void);
void osf_framework_terminate(void);

// Window management
OSFWindowC *osf_window_create(const char *id, const char *title,
                              const char *app_id);
void osf_window_register(OSFWindowC *window);
void osf_window_unregister(const char *id);
void osf_window_update_title(const char *id, const char *title);
void osf_window_destroy(OSFWindowC *window);

// Window actions
void osf_window_focus(const char *id);
void osf_window_minimize(const char *id);
void osf_window_maximize(const char *id);
void osf_window_close(const char *id);

// Event publishing
void osf_event_publish(const char *event_type, const char *data);

#ifdef __cplusplus
}
#endif

#endif // OSF_FRAMEWORK_C_H
