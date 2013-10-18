cmake_minimum_required(VERSION 2.8)

#set (CMAKE_C_COMPILER clang)
#set (CMAKE_CXX_COMPILER clang++)

# set project's name
  PROJECT( mycash )
  
  # with SET() command you can change variables or define new ones
  # here we define mycash_SRCS variable that contains a list of all .cpp files
  # note that we don't need \ at the end of line
  SET( mycash_SRCS
    accountcombobox.cpp
    agentcombobox.cpp
    accounttypecombobox.cpp
    currencycombobox.cpp
    currencyedit.cpp
    database.cpp
    editaccount.cpp
    editoperation.cpp
    global.cpp
    listaccounts.cpp
    listagents.cpp
    listcurrency.cpp
    listoperations.cpp
    listplanoper.cpp
    main.cpp
    mycash.cpp
    mydateedit.cpp
    opendatabase.cpp
    createdatabase.cpp
    correctbalance.cpp
  )
  
  # another list, this time it includes all header files that should be treated with moc
  SET( mycash_MOC_HDRS
    accountcombobox.h
    agentcombobox.h
    accounttypecombobox.h
    currencycombobox.h
    currencyedit.h
    database.h
    editaccount.h
    editoperation.h
    global.h
    listaccounts.h
    listagents.h
    listcurrency.h
    listoperations.h
    listplanoper.h
    mycash.h
    mydateedit.h
    opendatabase.h
    createdatabase.h
    correctbalance.h
  )
  
  # some .ui files
  SET( mycash_UIS
    editaccount.ui
    editoperation.ui
    listaccounts.ui
    listagents.ui
    listcurrency.ui
    listoperations.ui
    listplanoper.ui
    mycash.ui
    opendatabase.ui
    createdatabase.ui
    correctbalance.ui
  )
  
  # and finally an resource file
  SET( mycash_RCS
         icons.qrc
  )
  
  # enable warnings
  ADD_DEFINITIONS( -g )
  
  # by default only QtCore and QtGui modules are enabled
  # other modules must be enabled like this:
  # SET( QT_USE_QT3SUPPORT TRUE )
  SET( QT_USE_QTSQL TRUE )
  
  # this command finds Qt4 libraries and sets all required variables
  # note that it's Qt4, not QT4 or qt4
  FIND_PACKAGE( Qt4 REQUIRED )
  
  # add some useful macros and variables
  # (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
  INCLUDE( ${QT_USE_FILE} )
  
  # this command will generate rules that will run rcc on all files from mycash_RCS
  # in result mycash_RC_SRCS variable will contain paths to files produced by rcc
  QT4_ADD_RESOURCES( mycash_RC_SRCS ${mycash_RCS} )
  
  # this will run uic on .ui files:
  QT4_WRAP_UI( mycash_UI_HDRS ${mycash_UIS} )
  
  # and finally this will run moc:
  QT4_WRAP_CPP( mycash_MOC_SRCS ${mycash_MOC_HDRS} )
  
  # we need this to be able to include headers produced by uic in our code
  # (CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES() works just like INCLUDEPATH from qmake)
  INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )
  
  # here we instruct CMake to build "mycash" executable from all of the source files
  ADD_EXECUTABLE( mycash ${mycash_SRCS} ${mycash_MOC_SRCS} ${mycash_RC_SRCS} ${mycash_UI_HDRS} )
  
  # last thing we have to do is to tell CMake what libraries our executable needs,
  # luckily FIND_PACKAGE prepared QT_LIBRARIES variable for us:
  TARGET_LINK_LIBRARIES( mycash ${QT_LIBRARIES} )
