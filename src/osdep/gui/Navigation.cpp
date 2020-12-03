#include <stdbool.h>

#include <guisan.hpp>
#include <guisan/sdl.hpp>
#include <guisan/sdl/sdltruetypefont.hpp>
#include "SelectorEntry.hpp"
#include "sysdeps.h"
#include "gui_handling.h"

typedef struct
{
	std::string activeWidget;
	std::string leftWidget;
	std::string rightWidget;
	std::string upWidget;
	std::string downWidget;
} NavigationMap;


static NavigationMap navMap[] =
{
	//  active              move left         move right        move up             move down
	// main_window
	{"About", "", "", "Shutdown", "Paths"},
	{"Paths", "SystemROMs", "SystemROMs", "About", "Quickstart"},
	{"Quickstart", "qsNTSC", "cboAModel", "Paths", "Configurations"},
	{"Configurations", "ConfigList", "ConfigList", "Quickstart", "CPU and FPU"},
	{"CPU and FPU", "7 Mhz", "68000", "Configurations", "Chipset"},
	{"Chipset", "Fast copper", "OCS", "CPU and FPU", "ROM"},
	{"ROM", "MainROM", "cboMainROM", "Chipset", "RAM"},
	{"RAM", "Chipmem", "Chipmem", "ROM", "Floppy drives"},
	{"Floppy drives", "cmdSel0", "DF0:", "RAM", "Hard drives/CD"},
	{"Hard drives/CD", "cmdCreateHDF", "cmdAddDir", "Floppy drives", "Display"},
	{"Display", "cboScreenmode", "cboScreenmode", "Hard drives/CD", "Sound"},
	{"Sound", "sndDisable", "sndDisable", "Display", "Input"},
	{"Input", "cboPort0mode", "cboPort0", "Sound", "Custom controls"},
	{"Custom controls", "Right Trigger", "0: Mouse", "Input", "Miscellaneous"},
	{"Miscellaneous", "chkStatusLineNative", "chkStatusLineNative", "Custom controls", "Priority"},
#ifdef ANDROID
	{ "Savestates",       "State0",         "State0",         "Miscellaneous",  "OnScreen" },
	{ "OnScreen",         "OnScrButton3",   "OnScrCtrl",      "Savestates",     "Shutdown" },
	{ "Quit",             "Start",          "Help",           "OnScreen",       "Paths" },
	{ "Help",             "Quit",           "Start",          "OnScreen",       "Paths" },
	{ "Start",            "Help",           "Quit",           "OnScreen",       "Paths" },
#else
	{"Priority", "cboInactiveRunAtPrio", "cboActiveRunAtPrio", "Miscellaneous", "Savestates" },
	{"Savestates", "State0", "State0", "Priority", "Shutdown"},
	{"Shutdown", "Start", "Quit", "Savestates", "Paths"},
	{"Quit", "Shutdown", "Restart", "Savestates", "Paths"},
	{"Restart", "Quit", "Help", "Savestates", "Paths"},
	{"Help", "Restart", "Reset", "Savestates", "Paths"},
	{"Reset", "Help", "Start", "Savestates", "Paths"},
	{"Start", "Reset", "Shutdown", "Savestates", "Paths"},
#endif

	// PanelPaths
	{"SystemROMs", "Paths", "Paths", "RescanROMs", "ConfigPath"},
	{"ConfigPath", "Paths", "Paths", "SystemROMs", "ControllersPath"},
	{"ControllersPath", "Paths", "Paths", "ConfigPath", "RetroArchFile"},
	{"RetroArchFile", "Paths", "Paths", "ControllersPath", "chkEnableLogging"},
	{"chkEnableLogging", "Paths", "Paths", "RetroArchFile", "cmdLogfilePath"},
	{"cmdLogfilePath", "Paths", "Paths", "chkEnableLogging", "RescanROMs"},
	{"RescanROMs", "Paths", "DownloadXML", "cmdLogfilePath", "SystemROMs"},
	{"DownloadXML", "RescanROMs", "Paths", "cmdLogfilePath", "SystemROMs"},

	//  active            move left         move right        move up           move down
	// PanelQuickstart
	{"cboAModel", "Quickstart", "qsNTSC", "cboWhdload", "cboAConfig"},
	{"qsNTSC", "cboAModel", "Quickstart", "qsMode", "cboAConfig"},
	{"cboAConfig", "Quickstart", "Quickstart", "cboAModel", "qscmdSel0"},
	{"qsDF0", "Quickstart", "qsWP0", "cboAConfig", "cboDisk0"},
	{"qsWP0", "qsDF0", "qscmdEject0", "cboAConfig", "cboDisk0"},
	//  { "qsInfo0",        "Quickstart",     "",     "",               "" },
	{"qscmdEject0", "qsWP0", "qscmdSel0", "cboAConfig", "cboDisk0"},
	{"qscmdSel0", "qscmdEject0", "Quickstart", "cboAConfig", "cboDisk0"},
	{"cboDisk0", "Quickstart", "Quickstart", "qscmdSel0", "qscmdSel1"},
	{"qsDF1", "Quickstart", "qsWP1", "cboDisk0", "cboDisk1"},
	{"qsWP1", "qsDF1", "qscmdEject1", "cboDisk0", "cboDisk1"},
	//  { "qsInfo1",        "Quickstart",     "",     "",               "" },
	{"qscmdEject1", "qsWP1", "qscmdSel1", "cboDisk0", "cboDisk1"},
	{"qscmdSel1", "qscmdEject1", "Quickstart", "cboDisk0", "cboDisk1"},
	{"cboDisk1", "Quickstart", "Quickstart", "qsDF1", "qsCDSelect"},
	{"qsCD drive", "Quickstart", "qscdEject", "cboDisk1", "cboCD"},
	{"qscdEject", "qsCD drive", "qsCDSelect", "cboDisk1", "cboCD"},
	{"qsCDSelect", "qscdEject", "Quickstart", "cboDisk1", "cboCD"},
	{"cboCD", "Quickstart", "Quickstart", "qsCDSelect", "qsMode"},
	{"qsMode", "Quickstart", "Quickstart", "cboCD", "cmdSetConfig"},
	{"cmdSetConfig", "Quickstart", "Quickstart", "qsMode", "cmdWhdloadEject"},
	{"cmdWhdloadEject", "Quickstart", "cmdWhdloadSelect", "cmdSetConfig", "cboWhdload"},
	{"cmdWhdloadSelect", "cmdWhdloadEject", "Quickstart", "cmdSetConfig", "cboWhdload"},
	{"cboWhdload", "Quickstart", "Quickstart", "cmdWhdloadEject", "cboAModel"},

	// PanelConfig
	{"ConfigList", "Configurations", "ConfigLoad", "", ""},
	{"ConfigLoad", "Configurations", "ConfigSave", "ConfigList", "ConfigList"},
	{"ConfigSave", "ConfigLoad", "CfgDelete", "ConfigList", "ConfigList"},
	{"CfgDelete", "ConfigSave", "Configurations", "ConfigList", "ConfigList"},

	//  active            move left         move right        move up           move down
	// PanelCPU
	{"68000", "CPU and FPU", "FPUnone", "JIT", "68010"},
	{"68010", "CPU and FPU", "68881", "68000", "68020"},
	{"68020", "CPU and FPU", "68882", "68010", "68030"},
	{"68030", "CPU and FPU", "CPU internal", "68020", "68040"},
	{"68040", "CPU and FPU", "FPUstrict", "68030", "CPU24Bit"},
	{"CPU24Bit", "CPU and FPU", "FPUJIT", "68040", "CPUComp"},
	{"CPUComp", "CPU and FPU", "FPUJIT", "CPU24Bit", "JIT"},
	{"JIT", "CPU and FPU", "FPUJIT", "CPUComp", "68000"},
	{"FPUnone", "68000", "7 Mhz", "FPUJIT", "68881"},
	{"68881", "68010", "14 Mhz", "FPUnone", "68882"},
	{"68882", "68020", "25 Mhz", "68881", "CPU internal"},
	{"CPU internal", "68030", "Fastest", "68882", "FPUstrict"},
	{"FPUstrict", "68040", "Fastest", "CPU internal", "FPUJIT"},
	{"FPUJIT", "CPU24Bit", "Fastest", "FPUstrict", "FPUnone"},
	{"7 Mhz", "FPUnone", "CPU and FPU", "sldCpuIdle", "14 Mhz"},
	{"14 Mhz", "FPUnone", "CPU and FPU", "7 Mhz", "25 Mhz"},
	{"25 Mhz", "FPUnone", "CPU and FPU", "14 Mhz", "Fastest"},
	{"Fastest", "FPUnone", "CPU and FPU", "25 Mhz", "sldCpuIdle"},
	{"sldCpuIdle", "", "", "Fastest", "7 Mhz"},

	// PanelChipset
	{ "optOCS", "Chipset", "optAGA", "CollFull", "optECSAgnus" },
	{ "optECSAgnus", "Chipset", "optECSDenise", "optOCS", "optFullECS" },
	{ "optFullECS", "Chipset", "chkNTSC", "optECSAgnus", "chkCycleExact" },
	{ "chkCycleExact", "Chipset", "BlitNormal", "optFullECS", "chkMemoryCycleExact" },
	{ "chkMemoryCycleExact", "Chipset", "Fast copper", "chkCycleExact", "ChipsetExtra" },
	{ "optAGA", "optOCS", "BlitNormal", "CollFull", "optECSDenise" },
	{ "optECSDenise", "optECSAgnus", "Immediate", "optAGA", "chkNTSC" },
	{ "chkNTSC", "Chipset", "BlitWait", "optECSDenise", "chkCycleExact" },
	{ "ChipsetExtra", "Chipset", "Fast copper", "", "" },
	{ "BlitNormal", "optAGA", "Chipset", "Fast copper", "Immediate" },
	{ "Immediate", "optECSDenise", "Chipset", "BlitNormal", "BlitWait" },
	{ "BlitWait", "chkNTSC", "Chipset", "Immediate", "Fast copper" },
	{ "Fast copper", "chkMemoryCycleExact", "Chipset", "BlitWait", "BlitNormal" },
	{ "CollNone", "Chipset", "Chipset", "ChipsetExtra", "Sprites only" },
	{ "Sprites only", "Chipset", "Chipset", "CollNone", "CollPlay" },
	{ "CollPlay", "Chipset", "Chipset", "Sprites only", "CollFull" },
	{ "CollFull", "Chipset", "Chipset", "CollPlay", "optOCS" },

	//  active            move left         move right        move up           move down
	// PanelROM
	{"cboMainROM", "ROM", "MainROM", "cboCartROM", "cboExtROM"},
	{"MainROM", "cboMainROM", "ROM", "CartROM", "ExtROM"},
	{"cboExtROM", "ROM", "ExtROM", "cboMainROM", "cboCartROM"},
	{"ExtROM", "cboExtROM", "ROM", "MainROM", "CartROM"},
	{"cboCartROM", "ROM", "CartROM", "cboExtROM", "cboUAEROM"},
	{"CartROM", "cboCartROM", "ROM", "ExtROM", "cboUAEROM"},
	{"cboUAEROM", "ROM", "ROM", "cboCartROM", "chkShapeShifter"},
	{"chkShapeShifter", "ROM", "ROM", "cboUAEROM", "cboMainROM"},

	//PanelRAM
	{"Chipmem", "", "", "RAM", "Slowmem"},
	{"Slowmem", "", "", "Chipmem", "Fastmem"},
	{"Fastmem", "", "", "Slowmem", "Z3mem"},
	{"Z3mem", "", "", "Fastmem", "Gfxmem"},
	{"Gfxmem", "", "", "Z3mem", "A3000Low"},
	{"A3000Low", "", "", "Gfxmem", "A3000High"},
	{"A3000High", "", "", "A3000Low", "Chipmem"},

	//PanelFloppy
	{"DF0:", "Floppy drives", "cboType0", "SaveForDisk", "cboDisk0"},
	{"cboType0", "DF0:", "cmdEject0", "SaveForDisk", "cboDisk0"},
	{"cmdEject0", "cboType0", "cmdSel0", "CreateHD", "cboDisk0"},
	{"cmdSel0", "cmdEject0", "Floppy drives", "CreateHD", "cboDisk0"},
	{"cboDisk0", "Floppy drives", "Floppy drives", "DF0:", "LoadDiskCfg"},
	{"LoadDiskCfg", "Floppy drives", "Floppy drives", "cboDisk0", "DF1:"},
	{"DF1:", "Floppy drives", "cboType1", "LoadDiskCfg", "cboDisk1"},
	{"cboType1", "DF1:", "cmdEject1", "LoadDiskCfg", "cboDisk1"},
	{"cmdEject1", "cboType1", "cmdSel1", "LoadDiskCfg", "cboDisk1"},
	{"cmdSel1", "cmdEject1", "Floppy drives", "LoadDiskCfg", "cboDisk1"},
	{"cboDisk1", "Floppy drives", "Floppy drives", "DF1:", "DF2:"},
	{"DF2:", "Floppy drives", "cboType2", "cboDisk1", "cboDisk2"},
	{"cboType2", "DF2:", "cmdEject2", "cboDisk1", "cboDisk2"},
	{"cmdEject2", "cboType2", "cmdSel2", "cboDisk1", "cboDisk2"},
	{"cmdSel2", "cmdEject2", "Floppy drives", "cboDisk1", "cboDisk2"},
	{"cboDisk2", "Floppy drives", "Floppy drives", "DF2:", "DF3:"},
	{"DF3:", "Floppy drives", "cboType3", "cboDisk2", "cboDisk3"},
	{"cboType3", "DF3:", "cmdEject3", "cboDisk2", "cboDisk3"},
	{"cmdEject3", "cboType3", "cmdSel3", "cboDisk2", "cboDisk3"},
	{"cmdSel3", "cmdEject3", "Floppy drives", "cboDisk2", "cboDisk3"},
	{"cboDisk3", "Floppy drives", "Floppy drives", "DF3:", "DriveSpeed"},
	{"DriveSpeed", "", "", "cboDisk3", "CreateDD"},
	{"SaveForDisk", "Floppy drives", "CreateDD", "DriveSpeed", "DF0:"},
	{"CreateDD", "SaveForDisk", "CreateHD", "DriveSpeed", "cboType0"},
	{"CreateHD", "CreateDD", "Floppy drives", "DriveSpeed", "cmdEject0"},

	//  active            move left           move right          move up           move down
	// PanelHD
	{"cmdProp0", "Hard drives/CD", "cmdDel0", "chkCDTurbo", "cmdProp1"},
	{"cmdDel0", "cmdProp0", "Hard drives/CD", "chkCDTurbo", "cmdDel1"},
	{"cmdProp1", "Hard drives/CD", "cmdDel1", "cmdProp0", "cmdProp2"},
	{"cmdDel1", "cmdProp1", "Hard drives/CD", "cmdDel0", "cmdDel2"},
	{"cmdProp2", "Hard drives/CD", "cmdDel2", "cmdProp1", "cmdProp3"},
	{"cmdDel2", "cmdProp2", "Hard drives/CD", "cmdDel1", "cmdDel3"},
	{"cmdProp3", "Hard drives/CD", "cmdDel3", "cmdProp2", "cmdProp4"},
	{"cmdDel3", "cmdProp3", "Hard drives/CD", "cmdDel2", "cmdDel4"},
	{"cmdProp4", "Hard drives/CD", "cmdDel4", "cmdProp3", "cmdAddDir"},
	{"cmdDel4", "cmdProp4", "Hard drives/CD", "cmdDel3", "cmdAddHDF"},
	{"cmdAddDir", "Hard drives/CD", "cmdAddHDF", "cmdProp4", "chkHDRO"},
	{"cmdAddHDF", "cmdAddDir", "cmdCreateHDF", "cmdDel4", "chkHDRO"},
	{"cmdCreateHDF", "cmdAddHDF", "Hard drives / CD", "cmdDel4", "chkSCSI" },
	{"chkHDRO", "Hard drives / CD", "chkSCSI", "cmdAddDir", "chkCD" },
	{"chkSCSI", "chkHDRO", "Hard drives / CD", "cmdCreateHDF", "cdEject" },
	{"chkCD", "Hard drives/CD", "cdEject", "chkHDRO", "cboCD"},
	{"cdEject", "chkCD", "CDSelect", "chkSCSI", "cboCD" },
	{"CDSelect", "cdEject", "Hard drives/CD", "cmdCreateHDF", "cboCD"},
	{"cboCD", "Hard drives/CD", "Hard drives/CD", "chkCD", "chkCDTurbo"},
	{ "chkCDTurbo", "Hard drives/CD", "Hard drives/CD", "cboCD", "cmdProp0" },

	//  active            move left           move right          move up           move down
	// PanelDisplay
	{ "cboFullscreen", "Display", "chkHorizontal", "chkFrameskip", "cboScreenmode"},
	{ "cboScreenmode", "Display", "chkVertical", "cboFullscreen", "sldWidth" },
	{ "sldWidth", "", "", "cboScreenmode", "sldHeight" },
	{ "sldHeight", "", "", "sldWidth", "chkAutoHeight" },
	{ "chkAutoHeight", "Display", "optSingle", "sldHeight", "cboScalingMethod" },
	{ "cboScalingMethod", "Display", "optScanlines", "chkAutoHeight", "cboResolution" },
	{ "cboResolution", "Display", "optDouble2", "cboScalingMethod", "chkFilterLowRes" },
	{ "chkFilterLowRes", "Display", "optDouble3", "cboResolution", "chkBlackerThanBlack" },
	{ "chkBlackerThanBlack", "Display", "optISingle", "chkFilterLowRes", "chkAspect" },
	{ "chkAspect", "Display", "optISingle", "chkBlackerThanBlack", "chkFlickerFixer" },
	{ "chkFlickerFixer", "Display", "optIDouble", "chkAspect", "chkFrameskip" },
	{ "chkFrameskip", "Display", "optIDouble2", "chkFlickerFixer", "cboFullscreen" },
	{ "sldRefresh", "", "", "chkFlickerFixer", "cboFullscreen" },

	{ "chkHorizontal", "cboScreenmode", "", "optIDouble3", "chkVertical" },
	{ "chkVertical", "cboScreenmode", "", "chkHorizontal", "optSingle" },
	{ "optSingle", "chkAutoHeight", "", "chkVertical", "optDouble" },
	{ "optDouble", "chkAutoHeight", "", "optSingle", "optScanlines" },
	{ "optScanlines", "cboScalingMethod", "", "optDouble", "optDouble2" },
	{ "optDouble2", "cboResolution", "", "optScanlines", "optDouble3" },
	{ "optDouble3", "chkFilterLowRes", "", "optDouble2", "optISingle" },
	{ "optISingle", "chkBlackerThanBlack", "", "optDouble3", "optIDouble" },
	{ "optIDouble", "chkFlickerFixer", "", "optISingle", "optIDouble2" },
	{ "optIDouble2", "chkFrameskip", "", "optIDouble", "optIDouble3" },
	{ "optIDouble3", "chkFrameskip", "", "optIDouble2", "chkHorizontal" },

	//  active            move left           move right          move up           move down
	//PanelSound
	{ "sndDisable", "Sound", "sldPaulaVol", "sldFloppySoundDisk", "sndDisEmu" },
	{ "sndDisEmu", "Sound", "sldPaulaVol", "sndDisable", "sndEmulate" },
	{ "sndEmulate", "Sound", "sldPaulaVol", "sndDisEmu", "sndEmuBest" },
	{ "sndEmuBest", "Sound", "sldPaulaVol", "sndEmulate", "cboChannelMode" },
	{ "cboChannelMode", "Sound", "cboSeparation", "sndEmuBest", "cboFrequency" },
	{ "cboFrequency", "Sound", "cboStereoDelay", "cboChannelMode", "chkFloppySound" },
	{ "cboInterpol", "cboSeparation", "Sound", "sldCDVol", "cboFilter" },
	{ "cboFilter", "cboStereoDelay", "Sound", "cboInterpol", "sldSoundBufferSize" },
	{ "cboSeparation", "cboChannelMode", "cboInterpol", "sndEmuBest", "cboStereoDelay" },
	{ "cboStereoDelay", "cboFrequency", "cboFilter", "cboSeparation", "chkFloppySound" },
	{ "sldPaulaVol", "", "", "sldSoundBufferSize", "sldCDVol" },
	{ "sldCDVol", "", "", "sldPaulaVol", "cboInterpol" },
	{ "chkFloppySound", "Sound", "sldSoundBufferSize", "cboFrequency", "sldFloppySoundEmpty"},
	{ "sldFloppySoundEmpty", "", "", "chkFloppySound", "sldFloppySoundDisk"},
	{ "sldFloppySoundDisk", "", "", "sldFloppySoundEmpty", "sndDisable"},
	{ "sldSoundBufferSize", "", "", "cboFilter", "sldPaulaVol"},

	//  active            move left           move right          move up           move down
	// PanelInput
	{"cboPort0", "Input", "Input", "optBoth", "cboPort0Autofire"},
	{"cboPort0Autofire", "Input", "cboPort0mode", "cboPort0", "cboPort1"},
	{"cboPort0mode", "cboPort0Autofire", "Input", "cboPort0", "cboPort1"},
	{"cboPort1", "Input", "Input", "cboPort0Autofire", "cboPort1Autofire"},
	{"cboPort1Autofire", "Input", "cboPort1mode", "cboPort1", "cmdSwapPorts"},
	{"cboPort1mode", "cboPort1Autofire", "Input", "cboPort1", "chkInputAutoswitch"},
	{"cmdSwapPorts", "Input", "chkInputAutoswitch", "cboPort1Autofire", "cboPort2"},
	{"chkInputAutoswitch", "Input", "", "cboPort1mode", "cboPort2"},
	{"cboPort2", "Input", "Input", "cmdSwapPorts", "cboPort2Autofire"},
	{"cboPort2Autofire", "Input", "Input", "cboPort2", "cboPort3"},
	{"cboPort3", "Input", "Input", "cboPort2Autofire", "cboPort3Autofire"},
	{"cboPort3Autofire", "Input", "Input", "cboPort3", "cboPort0mousemode"},
	{"cboPort0mousemode", "Input", "sldDigitalJoyMouseSpeed", "cboPort3Autofire", "cboPort1mousemode"},
	{"cboPort1mousemode", "Input", "sldAnalogJoyMouseSpeed", "cboPort0mousemode", "cboAutofireRate"},
	{"cboAutofireRate", "Input", "sldMouseSpeed", "cboPort1mousemode", "chkMouseHack" },
	{"sldDigitalJoyMouseSpeed", "", "", "cboPort3Autofire", "sldAnalogJoyMouseSpeed"},
	{"sldAnalogJoyMouseSpeed", "", "", "sldDigitalJoyMouseSpeed", "sldMouseSpeed"},
	{"sldMouseSpeed", "", "", "sldAnalogJoyMouseSpeed", "chkMagicMouseUntrap"},
	{"chkMagicMouseUntrap", "chkMouseHack", "", "sldMouseSpeed", "optHost"},
	{"chkMouseHack", "Input", "chkMagicMouseUntrap", "cboAutofireRate", "optBoth"},
	{"optBoth", "Input", "optNative", "chkMouseHack", "cboPort0"},
	{"optNative", "optBoth", "optHost", "chkMouseHack", "cboPort0"},
	{"optHost", "optNative", "", "chkMouseHack", "cboPort0"},

	// PanelCustom
	{ "0: Mouse", "Custom controls", "1: Joystick", "chkAnalogRemap", "None" },
	{ "1: Joystick", "0: Mouse", "2: Parallel 1", "chkAnalogRemap", "HotKey" },
	{ "2: Parallel 1", "1: Joystick", "3: Parallel 2", "chkAnalogRemap", "Left Trigger" },
	{ "3: Parallel 2", "2: Parallel 1", "Custom controls", "chkAnalogRemap", "Right Trigger" },

	{ "None", "Custom controls", "HotKey", "0: Mouse", "cboCustomAction0" },
	{ "HotKey", "None", "Left Trigger", "1: Joystick", "cboCustomAction0" },
	{ "Left Trigger", "HotKey", "Right Trigger", "2: Parallel 1", "cboCustomAction0" },
	{ "Right Trigger", "Left Trigger", "Custom controls", "3: Parallel 2", "cboCustomAction0" },

	{ "cboCustomAction0", "Custom controls", "cboCustomAction7", "None", "cboCustomAction1" },
	{ "cboCustomAction1", "Custom controls", "cboCustomAction8", "cboCustomAction0", "cboCustomAction2" },
	{ "cboCustomAction2", "Custom controls", "cboCustomAction9", "cboCustomAction1", "cboCustomAction3" },
	{ "cboCustomAction3", "Custom controls", "cboCustomAction10", "cboCustomAction2", "cboCustomAction4" },
	{ "cboCustomAction4", "Custom controls", "cboCustomAction11", "cboCustomAction3", "cboCustomAction5" },
	{ "cboCustomAction5", "Custom controls", "cboCustomAction12", "cboCustomAction4", "cboCustomAction6" },
	{ "cboCustomAction6", "Custom controls", "cboCustomAction13", "cboCustomAction5", "cboCustomAction7" },

	{ "cboCustomAction7", "cboCustomAction0", "Custom controls", "cboCustomAction6", "cboCustomAction8" },
	{ "cboCustomAction8", "cboCustomAction1", "Custom controls", "cboCustomAction7", "cboCustomAction9" },
	{ "cboCustomAction9", "cboCustomAction2", "Custom controls", "cboCustomAction8", "cboCustomAction10" },
	{ "cboCustomAction10", "cboCustomAction3", "Custom controls", "cboCustomAction9", "cboCustomAction11" },
	{ "cboCustomAction11", "cboCustomAction4", "Custom controls", "cboCustomAction10", "cboCustomAction12" },
	{ "cboCustomAction12", "cboCustomAction5", "Custom controls", "cboCustomAction11", "cboCustomAction13" },
	{ "cboCustomAction13", "cboCustomAction6", "Custom controls", "cboCustomAction12", "cboCustomAction14" },
	{ "cboCustomAction14", "cboCustomAction6", "Custom controls", "cboCustomAction13", "chkAnalogRemap" },

	{ "chkAnalogRemap", "Custom controls", "Custom controls", "cboCustomAction13", "0: Mouse" },

	// PanelMisc
	//  active            move left           move right          move up           move down

	{ "chkStatusLineNative", "Miscellaneous", "chkRetroArchQuit", "chkRTSCTS", "chkStatusLineRtg" },
	{ "chkStatusLineRtg", "Miscellaneous", "chkRetroArchMenu", "chkStatusLineNative", "chkShowGUI" },
	{ "chkShowGUI", "Miscellaneous", "chkRetroArchReset", "chkStatusLineRtg", "chkMouseUntrap" },
	{ "chkMouseUntrap", "Miscellaneous", "chkClipboardSharing", "chkShowGUI", "chkBSDSocket" },
	{ "chkBSDSocket", "Miscellaneous", "chkAllowHostRun", "chkMouseUntrap", "chkMasterWP" },
	{ "chkMasterWP", "Miscellaneous", "chkRCtrlIsRAmiga", "chkBSDSocket", "cboNumlock" },

	{ "chkRetroArchQuit", "chkStatusLineNative", "Miscellaneous", "chkUaeSerial", "chkRetroArchMenu" },
	{ "chkRetroArchMenu", "chkStatusLineRtg", "Miscellaneous", "chkRetroArchQuit", "chkRetroArchReset" },
	{ "chkRetroArchReset", "chkShowGUI", "Miscellaneous", "chkRetroArchMenu", "chkClipboardSharing" },
	{ "chkClipboardSharing", "chkMouseUntrap", "Miscellaneous", "chkRetroArchReset", "chkAllowHostRun" },
	{ "chkAllowHostRun", "chkBSDSocket", "Miscellaneous", "chkClipboardSharing", "chkRCtrlIsRAmiga" },
	{ "chkRCtrlIsRAmiga", "chkMasterWP", "Miscellaneous", "chkAllowHostRun", "cboScrolllock"},
	{ "cboNumlock", "Miscellaneous", "cboScrolllock", "chkMasterWP", "OpenGUI" },
	{ "cboScrolllock", "cboNumlock", "Miscellaneous", "chkRCtrlIsRAmiga", "KeyForQuit" },
	{ "OpenGUI", "Miscellaneous", "KeyForQuit", "cboNumlock", "KeyActionReplay" },
	{ "KeyForQuit", "OpenGUI", "Miscellaneous", "cboScrolllock", "KeyFullScreen" },
	{ "KeyActionReplay", "Miscellaneous", "KeyFullScreen", "OpenGUI", "txtSerialDevice" },
	{ "KeyFullScreen", "KeyActionReplay", "KeyActionReplay", "KeyForQuit", "txtSerialDevice" },
	{ "txtSerialDevice", "", "", "KeyActionReplay", "chkRTSCTS" },
	{ "chkRTSCTS", "Miscellaneous", "chkSerialDirect", "txtSerialDevice", "chkStatusLineNative" },
	{ "chkSerialDirect", "chkRTSCTS", "chkUaeSerial", "txtSerialDevice", "chkStatusLineNative" },
	{ "chkUaeSerial", "chkSerialDirect", "Miscellaneous", "txtSerialDevice", "chkRetroArchQuit" },

	// PanelPrio
	{ "cboActiveRunAtPrio", "Priority", "cboInactiveRunAtPrio", "chkActiveDisableSound", "chkActivePauseEmulation" },
	{ "chkActivePauseEmulation", "Priority", "chkInactivePauseEmulation", "cboActiveRunAtPrio", "chkActiveDisableSound" },
	{ "chkActiveDisableSound", "Priority", "chkInactiveDisableSound", "chkActivePauseEmulation", "cboActiveRunAtPrio" },
	{ "cboInactiveRunAtPrio", "cboActiveRunAtPrio", "cboMinimizedRunAtPrio", "chkInactiveDisableControllers", "chkInactivePauseEmulation" },
	{ "chkInactivePauseEmulation", "chkActivePauseEmulation", "chkMinimizedPauseEmulation", "cboInactiveRunAtPrio", "chkInactiveDisableSound" },
	{ "chkInactiveDisableSound", "chkActiveDisableSound", "chkMinimizedDisableSound", "chkInactivePauseEmulation", "chkInactiveDisableControllers" },
	{ "chkInactiveDisableControllers", "chkActiveDisableSound", "chkMinimizedDisableControllers", "chkInactiveDisableSound", "cboInactiveRunAtPrio" },
	{ "cboMinimizedRunAtPrio", "cboInactiveRunAtPrio", "Priority", "chkMinimizedDisableControllers", "chkMinimizedPauseEmulation" },
	{ "chkMinimizedPauseEmulation", "chkInactivePauseEmulation", "Priority", "cboMinimizedRunAtPrio", "chkMinimizedDisableSound" },
	{ "chkMinimizedDisableSound", "chkInactiveDisableSound", "Priority", "chkMinimizedPauseEmulation", "chkMinimizedDisableControllers" },
	{ "chkMinimizedDisableControllers", "chkInactiveDisableControllers", "Priority", "chkMinimizedDisableSound", "cboMinimizedRunAtPrio" },
	
	// PanelSavestate
	{ "State0", "Savestates", "Savestates", "LoadState", "State1" },
	{ "State1", "Savestates", "Savestates", "State0", "State2" },
	{ "State2", "Savestates", "Savestates", "State1", "State3" },
	{ "State3", "Savestates", "Savestates", "State2", "State4" },
	{ "State4", "Savestates", "Savestates", "State3", "State5" },
	{ "State5", "Savestates", "Savestates", "State4", "State6" },
	{ "State6", "Savestates", "Savestates", "State5", "State7" },
	{ "State7", "Savestates", "Savestates", "State6", "State8" },
	{ "State8", "Savestates", "Savestates", "State7", "State9" },
	{ "State9", "Savestates", "Savestates", "State8", "LoadState" },
	{ "LoadState", "Savestates", "SaveState", "State9", "State0" },
	{ "SaveState", "LoadState", "Savestates", "State9", "State0" },

#ifdef ANDROID
// PanelOnScreen
{ "OnScrCtrl",      "OnScreen",       "OnScrButton3", "DisableMenuVKeyb", "OnScrTextInput" },
{ "OnScrButton3",   "OnScrCtrl",      "OnScreen",     "CustomPos",     "OnScrButton4" },
{ "OnScrTextInput", "OnScreen",       "OnScrButton4", "OnScrCtrl",      "OnScrDpad" },
{ "OnScrButton4",   "OnScrTextInput", "OnScreen",     "OnScrButton3",   "OnScrButton5" },
{ "OnScrDpad",      "OnScreen",       "OnScrButton5", "OnScrTextInput", "OnScrButton1" },
{ "OnScrButton5",   "OnScrDpad",      "OnScreen",     "OnScrButton4",   "OnScrButton6" },
{ "OnScrButton1",   "OnScreen",       "OnScrButton6", "OnScrDpad",      "OnScrButton2" },
{ "OnScrButton6",   "OnScrButton1",   "OnScreen",     "OnScrButton5",   "CustomPos" },
{ "OnScrButton2",   "OnScreen",       "CustomPos",    "OnScrButton1",   "FloatJoy" },
{ "CustomPos",      "OnScrButton2",   "OnScreen",     "OnScrButton6",   "Reset" },
{ "FloatJoy",       "OnScreen",       "CustomPos",    "OnScrButton2",   "DisableMenuVKeyb" },
{ "DisableMenuVKeyb","OnScreen",       "CustomPos",    "FloatJoy",       "Shutdown" },
#endif

	//  active            move left         move right        move up           move down
	// EditFilesysVirtual
	{"virtPath", "", "", "txtVirtBootPri", "virtCancel"},
	{"virtRW", "txtVirtDev", "txtVirtDev", "virtOK", "virtAutoboot"},
	{"virtAutoboot", "txtVirtVol", "txtVirtBootPri", "virtRW", "virtPath"},
	{"virtOK", "virtCancel", "virtCancel", "virtPath", "virtRW"},
	{"virtCancel", "virtOK", "virtOK", "virtPath", "virtRW"},

	// EditFilesysHardfile
	{"hdfRW", "txtHdfDev", "hdfAutoboot", "hdfOK", "hdfPath"},
	{"hdfAutoboot", "hdfRW", "txtHdfBootPri", "hdfOK", "hdfPath"},
	{"hdfPath", "txtHdfPath", "txtHdfPath", "txtHdfBootPri", "txtHdfReserved"},
	{"hdfController", "hdfUnit", "hdfUnit", "txtHdfSectors", "hdfOK"},
	{"hdfUnit", "hdfController", "hdfController", "hdfBlocksize", "hdfOK"},
	{"hdfOK", "hdfCancel", "hdfCancel", "hdfUnit", "txtHdfBootPri"},
	{"hdfCancel", "hdfOK", "hdfOK", "hdfUnit", "txtHdfBootPri"},

	// CreateFilesysHardfile
	{"createHdfAutoboot", "createHdfPath", "createHdfPath", "createHdfOK", "createHdfOK"},
	{"createHdfPath", "createHdfAutoboot", "createHdfOK", "createHdfAutoboot", "createHdfOK"},
	{"createHdfOK", "createHdfCancel", "createHdfCancel", "createHdfPath", "createHdfAutoboot"},
	{"createHdfCancel", "createHdfOK", "createHdfOK", "createHdfPath", "createHdfAutoboot"},

	{"END", "", "", "", ""}
};


bool HandleNavigation(int direction)
{
	gcn::FocusHandler* focusHdl = gui_top->_getFocusHandler();
	gcn::Widget* focusTarget = nullptr;

	if (focusHdl != nullptr)
	{
		gcn::Widget* activeWidget = focusHdl->getFocused();

		if (activeWidget != nullptr 
			&& activeWidget->getId().length() > 0 
			&& activeWidget->getId().substr(0, 3) != "txt")
		{
			std::string activeName = activeWidget->getId();
			auto bFoundEnabled = false;
			auto tries = 10;

			while (!bFoundEnabled && tries > 0)
			{
				std::string searchFor;

				for (auto i = 0; navMap[i].activeWidget != "END"; ++i)
				{
					if (navMap[i].activeWidget == activeName)
					{
						switch (direction)
						{
						case DIRECTION_LEFT:
							searchFor = navMap[i].leftWidget;
							break;
						case DIRECTION_RIGHT:
							searchFor = navMap[i].rightWidget;
							break;
						case DIRECTION_UP:
							searchFor = navMap[i].upWidget;
							break;
						case DIRECTION_DOWN:
							searchFor = navMap[i].downWidget;
							break;
						default:
							break;
						}
						if (searchFor.length() > 0)
						{
							focusTarget = gui_top->findWidgetById(searchFor);
							if (focusTarget != nullptr)
							{
								if (focusTarget->isEnabled() && focusTarget->isVisible())
									bFoundEnabled = true;
								else
									activeName = searchFor;
							}
							else
							{
								bFoundEnabled = true;
								break;
							}
						}
						break;
					}
				}
				if (searchFor.empty())
					bFoundEnabled = true; // No entry to navigate to -> exit loop
				--tries;
			}

			if (focusTarget != nullptr)
			{
				if (activeWidget->getId().substr(0, 3) == "cbo")
				{
					auto* dropdown = dynamic_cast<gcn::DropDown*>(activeWidget);
					if (dropdown->isDroppedDown() && (direction == DIRECTION_UP || direction == DIRECTION_DOWN))
						focusTarget = nullptr; // Up/down navigates in list if dropped down
				}
			}
		}
	}

	if (focusTarget != nullptr)
		focusTarget->requestFocus();
	return focusTarget != nullptr;
}

void PushFakeKey(const SDL_Keycode inKey)
{
	SDL_Event event;

	event.type = SDL_KEYDOWN; // and the key up
	event.key.keysym.sym = inKey;
	gui_input->pushInput(event); // Fire key down
	event.type = SDL_KEYUP; // and the key up
	gui_input->pushInput(event); // Fire key down
}
