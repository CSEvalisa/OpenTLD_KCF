
include_directories(
    tld
    kcf
    ../3rdparty/cvblobs
    ${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})

add_library(libopentld
    kcf/fhog.cpp
    kcf/tracker.h
    kcf/kcftracker.cpp
    tld/Clustering.cpp
    tld/DetectionResult.cpp
    tld/DetectorCascade.cpp
    tld/EnsembleClassifier.cpp
    tld/ForegroundDetector.cpp
    tld/kcf_tracking.cpp
    tld/NNClassifier.cpp
    tld/TLD.cpp
    tld/TLDUtil.cpp
    tld/VarianceFilter.cpp

    kcf/ffttools.hpp
    kcf/fhog.hpp
    kcf/labdata.hpp
    kcf/recttools.hpp
    kcf/kcftracker.hpp
    tld/Clustering.h
    tld/DetectionResult.h
    tld/DetectorCascade.h
    tld/EnsembleClassifier.h
    tld/ForegroundDetector.h
    tld/IntegralImage.h
    tld/kcf_tracking.h
    tld/NNClassifier.h
    tld/NormalizedPatch.h
    tld/TLD.h
    tld/TLDUtil.h
    tld/VarianceFilter.h
)

target_link_libraries(libopentld ${OpenCV_LIBS})

set_target_properties(libopentld PROPERTIES OUTPUT_NAME opentld)

