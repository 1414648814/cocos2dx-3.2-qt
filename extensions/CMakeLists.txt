set(EXTENSIONS_SRC
  assets-manager/AssetsManager.cpp
  GUI/CCControlExtension/CCControl.cpp
  GUI/CCControlExtension/CCControlButton.cpp
  GUI/CCControlExtension/CCControlColourPicker.cpp
  GUI/CCControlExtension/CCControlHuePicker.cpp
  GUI/CCControlExtension/CCControlPotentiometer.cpp
  GUI/CCControlExtension/CCControlSaturationBrightnessPicker.cpp
  GUI/CCControlExtension/CCControlSlider.cpp
  GUI/CCControlExtension/CCControlStepper.cpp
  GUI/CCControlExtension/CCControlSwitch.cpp
  GUI/CCControlExtension/CCControlUtils.cpp
  GUI/CCControlExtension/CCInvocation.cpp
  GUI/CCControlExtension/CCScale9Sprite.cpp
  GUI/CCEditBox/CCEditBox.cpp
  GUI/CCEditBox/CCEditBoxImplAndroid.cpp
  GUI/CCEditBox/CCEditBoxImplNone.cpp
  GUI/CCEditBox/CCEditBoxImplWin.cpp
  GUI/CCScrollView/CCScrollView.cpp
  GUI/CCScrollView/CCTableView.cpp
  GUI/CCScrollView/CCTableViewCell.cpp
  GUI/CCControlExtension/RepeatNode/Repeat9Sprite.cpp
  GUI/CCControlExtension/RepeatNode/RepeatNode.cpp
  physics-nodes/CCPhysicsDebugNode.cpp
  physics-nodes/CCPhysicsSprite.cpp
  ext/CCSpriteWithHue.cpp
)

if(WIN32)
  ADD_DEFINITIONS(-DUNICODE -D_UNICODE)

  set(PLATFORM_EXTENSIONS_SRC
    proj.win32/Win32InputBox.cpp
  )

  include_directories(
    ../external/win32-specific/gles/include
    ../external/curl/include/win32
  )
elseif(APPLE)

else()

endif()

include_directories(
  ..
)

add_library(extensions STATIC
  ${EXTENSIONS_SRC}
  ${PLATFORM_EXTENSIONS_SRC}
)

set_target_properties(extensions
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

