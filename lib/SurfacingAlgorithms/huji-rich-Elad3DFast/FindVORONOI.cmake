

cmake_minimum_required(VERSION 3.4)
message(BLOP)
#project(VoronoiLib VERSION 1.0.0 DESCRIPTION "Library used for running the voronoi algorithm.")
#Checks if the library is already present, if so, skips this part entirely.
IF(NOT TARGET VoronoiLib)

#    include_directories(${CMAKE_MODULE_PATH})
    # Create a new LIBRARY target as a shared library, note that this creates an so file in the cmake-debug output dir.
    # Adding the source code.
    add_library(VoronoiLib SHARED
                #elad3d
                #source/3D/GeometryCommon
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Voronoi3D.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Delaunay3D.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Face.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/HilbertOrder3D_Utils.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Intersections.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/predicates.cxx
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Predicates3D.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/RoundGrid3D.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tessellation3D.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/tetgen.cxx
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Tetrahedron.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon/Vector3D.cpp
                #source/3D/r3d
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/r3d/Intersection3D.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/r3d/r3d.c
                #source/misc
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/bisection.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/func_1_var.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/horner.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/int2str.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/io3D.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/mesh_generator.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/mesh_generator3D.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/serializable.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/simple_io.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/universal_error.cpp
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc/utils.cpp
                #extra
                lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/tessellation/geometry.cpp
                )


    # Telling cmake where to find headers for the code.
    target_include_directories(VoronoiLib PUBLIC lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/3D/GeometryCommon)
    target_include_directories(VoronoiLib PUBLIC lib/SurfacingAlgorithms/huji-rich-Elad3DFast/source/misc)

    MESSAGE(CREATED_VORONOI)

ENDIF()