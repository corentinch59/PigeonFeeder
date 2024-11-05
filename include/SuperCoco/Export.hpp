#pragma once

#ifdef SCE_ENGINE_STATIC
	#define SCE_ENGINE_API
#else

	#ifdef _WIN32

		#ifdef SUPER_COCO_BUILD
			#define SUPER_COCO_API __declspec(dllexport)
		#else
			#define SUPER_COCO_API __declspec(dllimport)
		#endif

	#else

		#define SUPER_COCO_API __attribute__((visibility("default")))

	#endif
#endif