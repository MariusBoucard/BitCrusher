# This file was generated by FRUT's Jucer2CMake from "GainPlugin.jucer"

cmake_minimum_required(VERSION 3.5)

project("BitCrusher")


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../FRUT/prefix/FRUT/cmake")
include(Reprojucer)


set(BitCrusher_jucer_FILE
  "${CMAKE_CURRENT_LIST_DIR}/GainPlugin.jucer"
)


set(JUCE_MODULES_GLOBAL_PATH "C:/Users/Marius/Desktop/JUCE/modules")


jucer_project_begin(
  JUCER_FORMAT_VERSION "1"
  PROJECT_FILE "${BitCrusher_jucer_FILE}"
  PROJECT_ID "e0DFdb"
)

jucer_project_settings(
  PROJECT_NAME "BitCrusher"
  PROJECT_VERSION "1.0.0"
  COMPANY_NAME "JUCE"
  COMPANY_WEBSITE "http://juce.com"
  USE_GLOBAL_APPCONFIG_HEADER OFF
  ADD_USING_NAMESPACE_JUCE_TO_JUCE_HEADER ON
  DISPLAY_THE_JUCE_SPLASH_SCREEN ON # Required for closed source applications without an Indie or Pro JUCE license
  PROJECT_TYPE "Audio Plug-in"
  BUNDLE_IDENTIFIER "com.JUCE.BitCrusher"
  CXX_LANGUAGE_STANDARD "C++17"
  PREPROCESSOR_DEFINITIONS
    "PIP_JUCE_EXAMPLES_DIRECTORY=QzpcVXNlcnNcTWFyaXVzXERlc2t0b3BcQUlQbHVnaW5cSnVjZVxleGFtcGxlcw=="
  # NOTES
  #   Gain audio plugin.
)

jucer_audio_plugin_settings(
  PLUGIN_FORMATS
    "VST3"
    "AU"
    "Standalone"
  # PLUGIN_CHARACTERISTICS
  PLUGIN_NAME "BitCrusher"
  PLUGIN_DESCRIPTION "BitCrusher"
  PLUGIN_MANUFACTURER "BALL"
  PLUGIN_MANUFACTURER_CODE "Manu"
  PLUGIN_CODE "E0df"
  # PLUGIN_CHANNEL_CONFIGURATIONS
  PLUGIN_AAX_IDENTIFIER "com.JUCE.BitCrusher"
  PLUGIN_AU_EXPORT_PREFIX "BitCrusherAU"
  PLUGIN_AU_MAIN_TYPE "kAudioUnitType_Effect"
  PLUGIN_AU_IS_SANDBOX_SAFE ON
  PLUGIN_VST3_CATEGORY
    "Fx"
  PLUGIN_RTAS_CATEGORY
    "ePlugInCategory_None"
  PLUGIN_AAX_CATEGORY
    "AAX_ePlugInCategory_None"
  PLUGIN_VST_LEGACY_CATEGORY "kPlugCategEffect"
)

jucer_project_files("GainPlugin/Source"
# Compile   Xcode     Binary    File
#           Resource  Resource
  x         .         .         "Source/Main.cpp"
  x         .         .         "Source/GainPluginDemo.h"
  x         .         .         "Source/AmpAudioProcessor.cpp"
  x         .         .         "Source/AmpAudioProcessor.h"
  x         .         .         "Source/AmpAudioProcessor.hpp"
)

jucer_project_files("GainPlugin/Source/dsp"
# Compile   Xcode     Binary    File
#           Resource  Resource
  x         .         .         "Source/dsp/Mappers.cpp"
  x         .         .         "Source/dsp/Mappers.h"
  x         .         .         "Source/dsp/ParameterSetup.h"
  x         .         .         "Source/dsp/Processor.cpp"
  x         .         .         "Source/dsp/Processor.h"
  x         .         .         "Source/dsp/Processor.hpp"
)

jucer_project_files("GainPlugin/Source/dsp/Bones"
# Compile   Xcode     Binary    File
#           Resource  Resource
  x         .         .         "Source/dsp/Bones/ToneStack.cpp"
  x         .         .         "Source/dsp/Bones/ToneStack.h"
)

jucer_project_files("GainPlugin/Source/looknfeel"
# Compile   Xcode     Binary    File
#           Resource  Resource
  x         .         .         "Source/looknfeel/GeneralLookAndFeel.h"
  x         .         .         "Source/looknfeel/StripKnob_look_n_feel.h"
  x         .         .         "Source/looknfeel/ToogleButtonLookAndFeel.h"
  x         .         .         "Source/looknfeel/TopBarLookAndFeel.h"
)

jucer_project_files("GainPlugin/Source/ui"
# Compile   Xcode     Binary    File
#           Resource  Resource
  x         .         .         "Source/ui/gui.cpp"
  x         .         .         "Source/ui/gui.h"
  x         .         .         "Source/ui/gui.hpp"
)

jucer_project_files("GainPlugin/Source/ui/components"
# Compile   Xcode     Binary    File
#           Resource  Resource
  x         .         .         "Source/ui/components/TopBarComponent.cpp"
  x         .         .         "Source/ui/components/TopBarComponent.h"
)

jucer_project_files("GainPlugin/Source/ui/components/leftSection"
# Compile   Xcode     Binary    File
#           Resource  Resource
  x         .         .         "Source/ui/components/leftSection/leftComponent.cpp"
  x         .         .         "Source/ui/components/leftSection/leftComponent.h"
)

jucer_project_files("GainPlugin/Source/ui/components/midSection"
# Compile   Xcode     Binary    File
#           Resource  Resource
  x         .         .         "Source/ui/components/midSection/midComponent.cpp"
  x         .         .         "Source/ui/components/midSection/midComponent.h"
)

jucer_project_files("GainPlugin/Source/ui/components/rightSection"
# Compile   Xcode     Binary    File
#           Resource  Resource
  x         .         .         "Source/ui/components/rightSection/rightComponent.cpp"
  x         .         .         "Source/ui/components/rightSection/rightComponent.h"
)



jucer_project_files("GainPlugin/ressources/knob/Geiger"
# Compile   Xcode     Binary    File
#           Resource  Resource
  .         x         x         "Source/ressources/knob/Geiger/g2.png"
  .         x         x         "Source/ressources/knob/Geiger/g1.png"
  .         x         x         "Source/ressources/knob/Geiger/gRad.png"
)

jucer_project_files("GainPlugin/ressources/knob/knob"
# Compile   Xcode     Binary    File
#           Resource  Resource
  .         x         x         "Source/ressources/knob/knob/bit.png"
  .         x         x         "Source/ressources/knob/knob/sr.png"
  .         x         x         "Source/ressources/knob/knob/RAD.png"
)

jucer_project_files("GainPlugin/ressources/plate"
# Compile   Xcode     Binary    File
#           Resource  Resource
  .         x         x         "Source/ressources/plate/plate.png"
  # Add more files here if needed
)

jucer_project_files("GainPlugin/ressources/knob/sat"
# Compile   Xcode     Binary    File
#           Resource  Resource
  .         x         x         "Source/ressources/knob/sat/Mix.png"
  .         x         x         "Source/ressources/knob/sat/SatBF.png"
  .         x         x         "Source/ressources/knob/sat/SatBG.png"
  .         x         x         "Source/ressources/knob/sat/SatBQ.png"

  .         x         x         "Source/ressources/knob/sat/SatG.png"
  .         x         x         "Source/ressources/knob/sat/SatM.png"
)

jucer_project_module(
  juce_audio_basics
  PATH "${JUCE_MODULES_GLOBAL_PATH}"
)

jucer_project_module(
  juce_audio_devices
  PATH "${JUCE_MODULES_GLOBAL_PATH}"
  # JUCE_USE_WINRT_MIDI
  # JUCE_ASIO
  # JUCE_WASAPI
  # JUCE_DIRECTSOUND
  # JUCE_ALSA
  # JUCE_JACK
  # JUCE_BELA
  # JUCE_USE_ANDROID_OBOE
  # JUCE_USE_OBOE_STABILIZED_CALLBACK
  # JUCE_USE_ANDROID_OPENSLES
  # JUCE_DISABLE_AUDIO_MIXING_WITH_OTHER_APPS
)

jucer_project_module(
  juce_audio_formats
  PATH "${JUCE_MODULES_GLOBAL_PATH}"
  # JUCE_USE_FLAC
  # JUCE_USE_OGGVORBIS
  # JUCE_USE_MP3AUDIOFORMAT
  # JUCE_USE_LAME_AUDIO_FORMAT
  # JUCE_USE_WINDOWS_MEDIA_FORMAT
)

jucer_project_module(
  juce_audio_plugin_client
  PATH "${JUCE_MODULES_GLOBAL_PATH}"
  JUCE_VST3_CAN_REPLACE_VST2 OFF
  # JUCE_FORCE_USE_LEGACY_PARAM_IDS
  # JUCE_FORCE_LEGACY_PARAMETER_AUTOMATION_TYPE
  # JUCE_USE_STUDIO_ONE_COMPATIBLE_PARAMETERS
  # JUCE_AU_WRAPPERS_SAVE_PROGRAM_STATES
  # JUCE_STANDALONE_FILTER_WINDOW_USE_KIOSK_MODE
)

jucer_project_module(
  juce_audio_processors
  PATH "${JUCE_MODULES_GLOBAL_PATH}"
  # JUCE_PLUGINHOST_VST
  # JUCE_PLUGINHOST_VST3
  # JUCE_PLUGINHOST_AU
  # JUCE_PLUGINHOST_LADSPA
  # JUCE_PLUGINHOST_LV2
  # JUCE_PLUGINHOST_ARA
  # JUCE_CUSTOM_VST3_SDK
)

jucer_project_module(
  juce_audio_utils
  PATH "${JUCE_MODULES_GLOBAL_PATH}"
  # JUCE_USE_CDREADER
  # JUCE_USE_CDBURNER
)

jucer_project_module(
  juce_core
  PATH "${JUCE_MODULES_GLOBAL_PATH}"
  # JUCE_FORCE_DEBUG
  # JUCE_LOG_ASSERTIONS
  # JUCE_CHECK_MEMORY_LEAKS
  # JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES
  # JUCE_INCLUDE_ZLIB_CODE
  # JUCE_USE_CURL
  # JUCE_LOAD_CURL_SYMBOLS_LAZILY
  # JUCE_CATCH_UNHANDLED_EXCEPTIONS
  # JUCE_ALLOW_STATIC_NULL_VARIABLES
  JUCE_STRICT_REFCOUNTEDPOINTER ON
  # JUCE_ENABLE_ALLOCATION_HOOKS
)

jucer_project_module(
  juce_data_structures
  PATH "${JUCE_MODULES_GLOBAL_PATH}"
)

#jucer_project_module(
 # juce_dsp
#  PATH "${JUCE_MODULES_GLOBAL_PATH}"
  # JUCE_ASSERTION_FIRFILTER
  # JUCE_DSP_USE_INTEL_MKL
  # JUCE_DSP_USE_SHARED_FFTW
  # JUCE_DSP_USE_STATIC_FFTW
  # JUCE_DSP_ENABLE_SNAP_TO_ZERO
#)

jucer_project_module(
  juce_events
  PATH "${JUCE_MODULES_GLOBAL_PATH}"
  # JUCE_EXECUTE_APP_SUSPEND_ON_BACKGROUND_TASK
)

jucer_project_module(
  juce_graphics
  PATH "${JUCE_MODULES_GLOBAL_PATH}"
  # JUCE_USE_COREIMAGE_LOADER
  # JUCE_USE_DIRECTWRITE
  # JUCE_DISABLE_COREGRAPHICS_FONT_SMOOTHING
)

jucer_project_module(
  juce_gui_basics
  PATH "${JUCE_MODULES_GLOBAL_PATH}"
  # JUCE_ENABLE_REPAINT_DEBUGGING
  # JUCE_USE_XRANDR
  # JUCE_USE_XINERAMA
  # JUCE_USE_XSHM
  # JUCE_USE_XRENDER
  # JUCE_USE_XCURSOR
  # JUCE_WIN_PER_MONITOR_DPI_AWARE
)

jucer_project_module(
  juce_gui_extra
  PATH "${JUCE_MODULES_GLOBAL_PATH}"
  # JUCE_WEB_BROWSER
  # JUCE_USE_WIN_WEBVIEW2
  # JUCE_ENABLE_LIVE_CONSTANT_EDITOR
)

jucer_export_target(
  "Xcode (macOS)"
  # VST3_SDK_FOLDER
)

jucer_export_target_configuration(
  "Xcode (macOS)"
  NAME "Debug"
  DEBUG_MODE ON
  BINARY_NAME "BitCrusher"
  OPTIMISATION "-O0 (no optimisation)"
)

jucer_export_target_configuration(
  "Xcode (macOS)"
  NAME "Release"
  DEBUG_MODE OFF
  BINARY_NAME "BitCrusher"
  OPTIMISATION "-O3 (fastest with safe optimisations)"
)

jucer_export_target(
  "Visual Studio 2022"
  # VST3_SDK_FOLDER
  EXTRA_COMPILER_FLAGS
    "/bigobj"
)

jucer_export_target_configuration(
  "Visual Studio 2022"
  NAME "Debug"
  DEBUG_MODE ON
  BINARY_NAME "BitCrusher"
  OPTIMISATION "No optimisation"
)

jucer_export_target_configuration(
  "Visual Studio 2022"
  NAME "Release"
  DEBUG_MODE OFF
  BINARY_NAME "BitCrusher"
  OPTIMISATION "Maximise speed"
)


jucer_project_end()
target_link_libraries(BitCrusher_Shared_Code PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
target_link_libraries(BitCrusher_Shared_Code PUBLIC AudioDSPTools)