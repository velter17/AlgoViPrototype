FILE ( GLOB_RECURSE ALGOVI_SOURCES
   "Controller/*.h"
   "Controller/*.cpp"
   "algovi/*.h"
   "algovi/*.cpp"
   "gui/*.h"
   "gui/*.cpp"
   "gui/*.ui"
   "View/*.h"
   "View/*.cpp"
   "main.cpp"
)

SET ( ALGOVI_APP_NAME "algovi" )

ADD_EXECUTABLE ( ${ALGOVI_APP_NAME} ${ALGOVI_SOURCES} )

QT5_USE_MODULES ( algovi 
   Widgets
)

SET ( COMMANDS_LIB_NAME "commands" )
SET ( ALGOVISETTINGS_LIB_NAME "settings" )
SET ( SOLVER_APPS_LIB_NAME "solverApps" )

TARGET_LINK_LIBRARIES ( ${ALGOVI_APP_NAME}
   ${COMMANDS_LIB_NAME}
   ${ALGOVISETTINGS_LIB_NAME}
   ${Boost_LIBRARIES}
)

ADD_SUBDIRECTORY ( framework )

INSTALL (
   TARGETS
      ${ALGOVI_APP_NAME}
   RUNTIME DESTINATION ${INSTALL_BIN_DIR}
)

INSTALL (
   DIRECTORY "configs"
   DESTINATION ${INSTALL_BIN_DIR}
   FILES_MATCHING PATTERN "*.xml"
)
