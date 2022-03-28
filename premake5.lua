workspace "tomreh-rasmushu-project"
   location "project"
   configurations { "Debug", "Release" }
   platforms { "Win32", "x64", "macosx" }

   filter { "platforms:Win32" }
      system "Windows"
      architecture "x86"

   filter { "platforms:x64" }
      system "Windows"
      architecture "x64"

   filter { "platforms:macosx" }
      system "macosx"
      architecture "x64"

   filter { }


project "Project Rayman"
   kind "ConsoleApp"
   language "C++"
   location "project/Project Rayman"
   targetdir "bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}"

   files { "**.h", "**.cpp" }

   includedirs {
      "dep/include",
   }

      links {
         "glfw"
      }

      filter "system:Windows"
         filter "architecture:x86"
            libdirs {
               "dep/win32"
            }
      
      filter "system:Windows"
         filter "architecture:x64"
            libdirs {
               "dep/x64"
            }

      filter "system:macosx"
         filter "architecture:x64"
            libdirs {
               "dep/macos"
            }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
   
   filter { }