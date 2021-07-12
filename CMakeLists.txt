cmake_minimum_required(VERSION 3.20)

project(MyProject VERSION 1.0.0 LANGUAGES C CXX)

# ROOT project directory
set(ROOT_SRC_DIR src)

# Main executable target name
set(MAIN_TARGET a)

# directories for include dependencies
set(INCLUDE_DEPENDENCIES "dependencies/include/")
set(LIB_DEPENDENCIES "dependencies/lib/")

# build ParserXML library
set(PARSER_XML ParserXML)
file(GLOB_RECURSE source_files
	"${ROOT_SRC_DIR}/*.cpp"
	"${ROOT_SRC_DIR}/*.c"
)

add_executable(${MAIN_TARGET} 
				${Sources} 
				${source_files} 
)

set_target_properties(${MAIN_TARGET} 
			PROPERTIES 
			C_STANDARD 17
			C_STANDARD_REQUIRED ON
)

target_include_directories(${MAIN_TARGET} PUBLIC "${INCLUDE_DEPENDENCIES}")

target_link_directories(${MAIN_TARGET} PUBLIC "${LIB_DEPENDENCIES}")

target_link_libraries(${MAIN_TARGET} PUBLIC gdi32 glfw3)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resourses" DESTINATION "${PROJECT_BINARY_DIR}")
