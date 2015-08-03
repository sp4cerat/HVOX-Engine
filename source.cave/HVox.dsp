# Microsoft Developer Studio Project File - Name="HVox" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=HVox - Win32 Debug
!MESSAGE Dies ist kein gultiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und fuhren Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "HVox.mak".
!MESSAGE 
!MESSAGE Sie konnen beim Ausfuhren von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "HVox.mak" CFG="HVox - Win32 Debug"
!MESSAGE 
!MESSAGE Fur die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "HVox - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "HVox - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HVox - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /w /W0 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 dinput8.lib glut32.lib glu32.lib opengl32.lib glu32.lib glut32.lib glaux.lib kernel32.lib gdi32.lib user32.lib advapi32.lib gdi32.lib comdlg32.lib winspool.lib shell32.lib winmm.lib comctl32.lib ole32.lib oleaut32.lib dxguid.lib dsound.lib winmm.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "HVox - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dinput8.lib glut32.lib glu32.lib opengl32.lib glu32.lib glut32.lib glaux.lib kernel32.lib gdi32.lib user32.lib advapi32.lib gdi32.lib comdlg32.lib winspool.lib shell32.lib winmm.lib comctl32.lib ole32.lib oleaut32.lib dxguid.lib dsound.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "HVox - Win32 Release"
# Name "HVox - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BENCHMARK.cpp
# End Source File
# Begin Source File

SOURCE=.\BMP.cpp
# End Source File
# Begin Source File

SOURCE=.\ENGINE.cpp
# End Source File
# Begin Source File

SOURCE=.\GFX.cpp
# End Source File
# Begin Source File

SOURCE=.\HOUSE.cpp
# End Source File
# Begin Source File

SOURCE=.\IOTools.cpp
# End Source File
# Begin Source File

SOURCE=.\MC.cpp
# End Source File
# Begin Source File

SOURCE=.\OBJ.cpp
# End Source File
# Begin Source File

SOURCE=.\PBUFFER.cpp
# End Source File
# Begin Source File

SOURCE=.\Vector.cpp
# End Source File
# Begin Source File

SOURCE=.\WINDOW.cpp
# End Source File
# Begin Source File

SOURCE=.\Winmain.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BENCHMARK.h
# End Source File
# Begin Source File

SOURCE=.\BMP.h
# End Source File
# Begin Source File

SOURCE=.\ENGINE.h
# End Source File
# Begin Source File

SOURCE=.\GFX.h
# End Source File
# Begin Source File

SOURCE=.\HOUSE.h
# End Source File
# Begin Source File

SOURCE=.\IOTools.h
# End Source File
# Begin Source File

SOURCE=.\Math_Defs.h
# End Source File
# Begin Source File

SOURCE=.\MC.h
# End Source File
# Begin Source File

SOURCE=.\OBJ.h
# End Source File
# Begin Source File

SOURCE=.\pbuffer.h
# End Source File
# Begin Source File

SOURCE=.\SKY.h
# End Source File
# Begin Source File

SOURCE=.\STRUCTS.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Shader"

# PROP Default_Filter "txt"
# Begin Source File

SOURCE=..\shaders\fp_blur.txt
# End Source File
# Begin Source File

SOURCE=..\shaders\fp_classic.txt
# End Source File
# Begin Source File

SOURCE=..\shaders\fp_envbump.txt
# End Source File
# Begin Source File

SOURCE=..\shaders\fp_envbump2.txt
# End Source File
# Begin Source File

SOURCE=..\shaders\fp_glow.txt
# End Source File
# Begin Source File

SOURCE=..\shaders\fp_objtex.txt
# End Source File
# Begin Source File

SOURCE=..\shaders\fp_parallax.txt
# End Source File
# Begin Source File

SOURCE=..\shaders\fp_terraintex.txt
# End Source File
# Begin Source File

SOURCE=..\shaders\fp_threshold.txt
# End Source File
# Begin Source File

SOURCE=..\shaders\fp_uberbump.txt
# End Source File
# Begin Source File

SOURCE=..\shaders\vp_blur.txt
# End Source File
# Begin Source File

SOURCE=..\shaders\vp_envbump.txt
# End Source File
# Begin Source File

SOURCE=..\shaders\vp_envbump2.txt
# End Source File
# Begin Source File

SOURCE=..\shaders\vp_glow.txt
# End Source File
# Begin Source File

SOURCE=..\shaders\vp_objtex.txt
# End Source File
# Begin Source File

SOURCE=..\shaders\vp_terraintex.txt
# End Source File
# Begin Source File

SOURCE=..\shaders\vp_threshold.txt
# End Source File
# Begin Source File

SOURCE=..\shaders\vp_uberbump.txt
# End Source File
# End Group
# End Target
# End Project
