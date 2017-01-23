FILE ( GLOB_RECURSE GRAVIZ_SOURCES
   "Controller/*.h"
   "Controller/*.cpp"
   "graviz/*.h"
   "graviz/*.cpp"
   "gui/*.h"
   "gui/*.cpp"
   "gui/*.ui"
   "View/*.h"
   "View/*.cpp"
   "main.cpp"
)

SET ( GRAVIZ_APP_NAME "graviz" )

ADD_EXECUTABLE ( ${GRAVIZ_APP_NAME} ${GRAVIZ_SOURCES} )

QT5_USE_MODULES ( graviz 
   Widgets
)

SET ( COMMANDS_LIB_NAME "commands" )
SET ( GRAVIZSETTINGS_LIB_NAME "settings" )
SET ( SOLVER_APPS_LIB_NAME "solverApps" )

TARGET_LINK_LIBRARIES ( ${GRAVIZ_APP_NAME}
   ${COMMANDS_LIB_NAME}
   ${GRAVIZSETTINGS_LIB_NAME}
   ${Boost_LIBRARIES}
)

ADD_SUBDIRECTORY ( framework )

INSTALL (
   TARGETS
      ${GRAVIZ_APP_NAME}
   RUNTIME DESTINATION ${INSTALL_BIN_DIR}
)

INSTALL (
   DIRECTORY "configs"
   DESTINATION ${INSTALL_BIN_DIR}
   FILES_MATCHING PATTERN "*.xml"
)
