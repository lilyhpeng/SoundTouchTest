get_filename_component(SOUND_TOUCH_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

ADD_DEFINITIONS(-Dopenmp)

set(SOUND_TOUCH_INC
		${SOUND_TOUCH_CMAKE_DIR}/
		)

set(SOUND_TOUCH_SRC
		${SOUND_TOUCH_CMAKE_DIR}/AAFilter.cpp
		${SOUND_TOUCH_CMAKE_DIR}/BPMDetect.cpp
		${SOUND_TOUCH_CMAKE_DIR}/cpu_detect_x86.cpp
		${SOUND_TOUCH_CMAKE_DIR}/FIFOSampleBuffer.cpp
		${SOUND_TOUCH_CMAKE_DIR}/FIRFilter.cpp
		${SOUND_TOUCH_CMAKE_DIR}/mmx_optimized.cpp
		${SOUND_TOUCH_CMAKE_DIR}/PeakFinder.cpp
		${SOUND_TOUCH_CMAKE_DIR}/RateTransposer.cpp
		${SOUND_TOUCH_CMAKE_DIR}/SoundTouch.cpp
		${SOUND_TOUCH_CMAKE_DIR}/sse_optimized.cpp
		${SOUND_TOUCH_CMAKE_DIR}/TDStretch.cpp
		)
