FILE ( GLOB_RECURSE GRAVIZ_SOURCES
   "main.cpp"
   "Controller/*.h"
   "Controller/*.cpp"
   "View/*.h"
   "View/*.cpp"
   "gui/*.h"
   "gui/*.cpp"
   "gui/*.ui"
   "gui/*.qml"
   "graviz/*.h"
   "graviz/*.cpp"
)

SET ( GRAVIZ_APP_NAME "graviz" )

ADD_EXECUTABLE ( ${GRAVIZ_APP_NAME} ${GRAVIZ_SOURCES} )

QT5_USE_MODULES ( graviz 
   Widgets 
)
SET ( COMMANDS_LIB_NAME "commands" )

TARGET_LINK_LIBRARIES ( ${GRAVIZ_APP_NAME}
   ${COMMANDS_LIB_NAME}
   ${Boost_LIBRARIES}
)

ADD_SUBDIRECTORY ( framework )

INSTALL (
   TARGETS
      ${GRAVIZ_APP_NAME}
   RUNTIME DESTINATION ${INSTALL_BIN_DIR}
)






