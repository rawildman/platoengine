add_executable(GeometryCylinderTest Test_Geometry_Cylinder.cpp UnitTest.cpp)
target_link_libraries(GeometryCylinderTest Cogent)
target_link_libraries(GeometryCylinderTest GTest::GTest ${Trilinos_LIBRARIES} ${Trilinos_TPL_LIBRARIES})
add_test(NAME GeometryCylinderTest COMMAND GeometryCylinderTest)
