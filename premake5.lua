workspace "Foxy"
  architecture "x64"
  startproject "Foxy"

  configurations
  {
    "Debug",
    "Release",
    "Dist"
  }

  outputdir = "%{cfg.buildcfg}"

  IncludeDir = {}
  IncludeDir["directx"] = "vendor/directx/Inc"
  IncludeDir["luac"] = "vendor/luac"
  IncludeDir["sol2"] = "vendor/sol2/include"
  IncludeDir["json"] = "vendor/json/single_include"
  IncludeDir["MinHook"] = "vendor/MinHook/include"
  IncludeDir["ImGui"] = "vendor/ImGui"
  IncludeDir["ImGuiImpl"] = "vendor/ImGui/backends"
  IncludeDir["StackWalker"] = "vendor/StackWalker/Main/StackWalker/"
  
  CppVersion = "C++20"
  MsvcToolset = "v142"
  WindowsSdkVersion = "10.0"
  
  function DeclareMSVCOptions()
    filter "system:windows"
    staticruntime "Off"
    systemversion (WindowsSdkVersion)
    toolset (MsvcToolset)
    cppdialect (CppVersion)

    defines
    {
      "_CRT_SECURE_NO_WARNINGS",
      "NOMINMAX",
      "WIN32_LEAN_AND_MEAN",
      "_WIN32_WINNT=0x601" -- Support Windows 7
    }
    
    disablewarnings
    {
      "4100", -- C4100: unreferenced formal parameter
      "4201", -- C4201: nameless struct/union
      "4307"  -- C4307: integral constant overflow
    }
  end
   
  function DeclareDebugOptions()
    filter "configurations:Debug"
      defines { "_DEBUG" }
      symbols "On"
    filter "not configurations:Debug"
      defines { "NDEBUG" }
  end
   
  project "ImGui"
    location "vendor/%{prj.name}"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/lib/" .. outputdir)
    objdir ("bin/lib/int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
      "vendor/%{prj.name}/imgui.cpp",
      "vendor/%{prj.name}/imgui_demo.cpp",
      "vendor/%{prj.name}/imgui_draw.cpp",
      "vendor/%{prj.name}/imgui_tables.cpp",
      "vendor/%{prj.name}/imgui_widgets.cpp",
      "vendor/%{prj.name}/backends/imgui_impl_dx11.cpp",
      "vendor/%{prj.name}/backends/imgui_impl_win32.cpp"
    }

    includedirs
    {
      "vendor/%{prj.name}"
    }

    DeclareMSVCOptions()
    DeclareDebugOptions()

  project "directx"
    location "vendor/%{prj.name}"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/lib/" .. outputdir)
    objdir ("bin/lib/int/" .. outputdir .. "/%{prj.name}")

    files
    {
      "vendor/%{prj.name}/Inc/**.h",
      "vendor/%{prj.name}/Src/**.cpp",
      "vendor/%{prj.name}/Src/Shaders/**.fx",
      "vendor/%{prj.name}/Src/Shaders/Compiled/**.inc"
    }

    includedirs
    {
      "vendor/%{prj.name}/Inc",
      "vendor/%{prj.name}/Src/Shaders/Compiled"
    }

    DeclareMSVCOptions()
    DeclareDebugOptions()

  project "luac"
    location "vendor/%{prj.name}"
    kind "StaticLib"
    language "C"

    targetdir ("bin/lib/" .. outputdir)
    objdir ("bin/lib/int/" .. outputdir .. "/%{prj.name}")

    files
    {
      "vendor/%{prj.name}/**.h",
      "vendor/%{prj.name}/**.c"
    }

    includedirs
    {
      "vendor/%{prj.name}"
    }

    DeclareMSVCOptions()
    DeclareDebugOptions()
    
  project "sol2"
    location "vendor/%{prj.name}"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/lib/" .. outputdir)
    objdir ("bin/lib/int/" .. outputdir .. "/%{prj.name}")

    files
    {
      "vendor/%{prj.name}/include/**.h",
      "vendor/%{prj.name}/include/**.hpp",
      "vendor/%{prj.name}/src/**.cpp"
    }

    includedirs
    {
      "vendor/%{prj.name}/include"
    }

    DeclareMSVCOptions()
    DeclareDebugOptions()

  project "StackWalker"
    location "vendor/%{prj.name}"
    kind "StaticLib"
    language "C++"
  
    targetdir ("bin/lib/" .. outputdir)
    objdir ("bin/lib/int/" .. outputdir .. "/%{prj.name}")

    files
    {
      "vendor/%{prj.name}/Main/StackWalker/StackWalker.cpp"
    }

    includedirs
    {
      "vendor/%{prj.name}/Main/StackWalker/"
    }

    DeclareMSVCOptions()
    DeclareDebugOptions()

  project "MinHook"
    location "vendor/%{prj.name}"
    kind "StaticLib"
    language "C"

    targetdir ("bin/lib/" .. outputdir)
    objdir ("bin/lib/int/" .. outputdir .. "/%{prj.name}")

    files
    {
      "vendor/%{prj.name}/include/**.h",
      "vendor/%{prj.name}/src/**.h",
      "vendor/%{prj.name}/src/**.c"
    }

    DeclareMSVCOptions()
    DeclareDebugOptions()

  project "Foxy"
    location "Foxy"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir)
    objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

    --PrecompiledHeaderInclude = "Common.hpp"
    --PrecompiledHeaderSource = "%{prj.name}/pch/src/Common.cpp"
 
    files
    {
      "%{prj.name}/src/**.hpp",
      "%{prj.name}/src/**.cpp",
      "%{prj.name}/src/**.asm"
    }

    includedirs
    {
      "%{IncludeDir.directx}",
      "%{IncludeDir.luac}",
      "%{IncludeDir.sol2}",
      "%{IncludeDir.json}",
      "%{IncludeDir.MinHook}",
      "%{IncludeDir.ImGui}",
      "%{IncludeDir.ImGuiImpl}",
      "%{IncludeDir.StackWalker}",
      "%{prj.name}/src"
    }

    libdirs
    {
      "bin/lib"
    }

    links
    {
      "DirectX",
      "luac",
      "MinHook",
      "ImGui",
      "StackWalker"
    }

    --pchheader "%{PrecompiledHeaderInclude}"
    --pchsource "%{PrecompiledHeaderSource}"

    forceincludes
    {
      "%{PrecompiledHeaderInclude}"
    }

    DeclareMSVCOptions()
    DeclareDebugOptions()

    flags { "NoImportLib", "Maps" }

    filter "configurations:Debug"
      defines { "BIGBASEV2_DEBUG" }

    filter "configurations:Release"
      defines { "BIGBASEV2_RELEASE" }
      optimize "speed"
    filter "configurations:Dist"
      flags { "LinkTimeOptimization", "FatalCompileWarnings" }
      defines { "BIGBASEV2_DIST" }
      optimize "speed"