cmake_minimum_required (VERSION 3.13)

add_library(dearpy3d_importer)
set_property(TARGET dearpy3d_importer PROPERTY CXX_STANDARD 17)

target_sources(dearpy3d_importer

	PRIVATE

		"src/mvImporter.cpp"

)

target_include_directories(dearpy3d_importer

	PRIVATE

		"src"
		"../Dependencies/json/"

)
