
file(GLOB DUNE_TELLO_FILES vendor/libraries/tello/*.cpp
                           vendor/libraries/tello/*.hpp

                          #  vendor/libraries/tello/h264decoder/*.cpp
                          #  vendor/libraries/tello/h264decoder/*.hpp
                          #  vendor/libraries/tello/h264decoder/libavcodec/*.cpp
                          #  vendor/libraries/tello/h264decoder/libavcodec/*.hpp
                          #  vendor/libraries/tello/h264decoder/libavcodec/libavutil/*.cpp
                          #  vendor/libraries/tello/h264decoder/libavcodec/libavutil/*.hpp
                          #  vendor/libraries/tello/libswscale/*.cpp
                          #  vendor/libraries/tello/libswscale/*.hpp
                          #  vendor/libraries/tello/libswscale/libavutil*.cpp

                          #  vendor/libraries/tello/libavcodec/*.cpp
                          #  vendor/libraries/tello/libavcodec/*.hpp

                          #  vendor/libraries/tello/libavutil/*.cpp
                          #  vendor/libraries/tello/libavutil/*.hpp

                          #  vendor/libraries/tello/libavformat/*.cpp
                          #  vendor)/libraries/tello/libswscale/*.cpp
                          #  vendor/libraries/tello/libswresample/*.cpp
  )


message("Marius: ======== h264 decoder ATTEMPT TO BUILD ================")


# set(SOURCES vendor/libraries/tello/h264decoder/*.cpp
#   vendor/libraries/tello/h264decoder/*.cpp

# )


find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h PATHS ${AVCODEC_INCLUDE_DIRS})
find_library(AVCODEC_LIBRARY avcodec PATHS $ {AVCODEC_LIBRARY_DIRS})

# avutil
find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h PATHS ${AVUTIL_INCLUDE_DIRS})
find_library(AVUTIL_LIBRARY avutil PATHS ${AVUTIL_LIBRARY_DIRS})

# swresample
find_path(SWRESAMPLE_INCLUDE_DIR libswresample/swresample.h PATHS ${SWRESAMPLE_INCLUDE_DIRS})
find_library(SWRESAMPLE_LIBRARY swresample PATHS ${SWRESAMPLE_LIBRARY_DIRS})

# swscale
find_path(SWSCALE_INCLUDE_DIR libswscale/swscale.h PATHS ${AVRESAMPLE_INCLUDE_DIRS})
find_library(SWSCALE_LIBRARY swscale PATHS ${SWSCALE_LIBRARY_DIRS})

set(AVCODEC_LIBS ${AVCODEC_LIBRARY} ${AVUTIL_LIBRARY} ${SWRESAMPLE_LIBRARY} ${SWSCALE_LIBRARY})

message(${AVCODEC_LIBS})
dune_add_lib(${AVCODEC_LIBRARY})
dune_add_lib(${AVUTIL_LIBRARY})
dune_add_lib(${SWRESAMPLE_LIBRARY})
dune_add_lib(${SWSCALE_LIBRARY})

# dune_add_lib(AVCODEC_INCLUDE_DIR)
# dune_add_lib(/usr/include/x86_64-linux-gnu)
# dune_add_lib(/usr/include/x86_64-linux-gnu/libavcodec)
# dune_add_lib(/usr/include/x86_64-linux-gnu/libavresample)
# dune_add_lib(/usr/include/x86_64-linux-gnu/libavswscale)
# dune_add_lib(/usr/include/x86_64-linux-gnu/libavutil)
# dune_add_lib(x86_64-linux-gnu/libavformat)
# dune_add_lib(x86_64-linux-gnu/libavutil)
# dune_add_lib(libswresample)
# dune_add_lib(libswscale)
# dune_add_lib(h264decoder)

set_source_files_properties(${DUNE_TELLO_FILES}
    PROPERTIES COMPILE_FLAGS "${DUNE_C_FLAGS}")


list(APPEND DUNE_VENDOR_FILES ${DUNE_TELLO_FILES})

set(DUNE_VENDOR_INCS_DIR ${DUNE_VENDOR_INCS_DIR}
  ${PROJECT_SOURCE_DIR}/vendor/libraries
)

# set(DUNE_VENDOR_INCS_DIR ${DUNE_VENDOR_INCS_DIR}
# ${PROJECT_SOURCE_DIR}/user/vendor/libraries/oCam)