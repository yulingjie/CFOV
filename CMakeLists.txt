cmake_minimum_required (VERSION 2.6)
project (CFOV)

include_directories(inc Pdcurses)

add_executable (CFOV src/main.cpp
					src/Size.cpp
					src/Common.cpp
					src/RayCastVisibility.cpp
					src/Rectangle.cpp
					src/TileMap.cpp
					src/ShadowCastVisibility.cpp
					src/DiamondWallsVisibility.cpp
					inc/Common.h
					inc/LevelPoint.h
					inc/RayCastVisibility.h
					inc/Rectangle.h
					inc/Size.h
					inc/Tile.h
					inc/TileMap.h
					inc/Visibility.h
					inc/ShadowCastVisibility.h
					inc/DiamondWallsVisibility.h)

find_library(PDCURSES_LIBRARY pdcurses HINTS ./Pdcurses)
target_link_libraries(CFOV PUBLIC ${PDCURSES_LIBRARY})