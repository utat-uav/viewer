add_executable(gcsViewer gcsmain.cpp imgutils.cpp)
install (TARGETS gcsViewer DESTINATION /usr/local/bin)

include_directories( ../imgsrc/)

#Linked Libraries
target_link_libraries(gcsViewer imagestream)
target_link_libraries(gcsViewer ${OpenCV_LIBS})
target_link_libraries(gcsViewer ${zbar_LIBRARY})

