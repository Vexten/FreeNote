#pragma once

namespace Freenote {
#define FN_ERRORS \
X(APP_SDL_VIDEO_INIT_ERR, "APP_SDL_VIDEO_INIT_ERR") \
X(APP_SDL_WINDOW_CREATE_ERR, "APP_SDL_WINDOW_CREATE_ERR") \
X(APP_GL_CONTEXT_CREATE_ERR, "APP_GL_CONTEXT_CREATE_ERR") \
X(APP_SDL_RENDERER_CREATE_ERR, "APP_SDL_RENDERER_CREATE_ERR") \
X(APP_SDL_BASE_FOLDER_NOT_RETURNED, "APP_SDL_BASE_FOLDER_NOT_RETURNED") \
X(APP_SDL_ERRORS, "This is used to check for SDL errors, you shouldn't see this text.") \
X(SHAPE_OBJ_VERTEX_PARSE_ERROR, "OBJ file empty or malformed") \
X(RESOURCE_STORAGE_FOLDER_DOES_NOT_EXIST, "The 'resource' folder was not found.")


#define X(name, str) name,
	const enum APP_ERR : size_t {
		FN_ERRORS
	};
#undef X

#define X(name, str) str,
	constexpr const char* APP_ERR_STR[] = {
		FN_ERRORS
	};
#undef X

#undef FN_ERRORS
}