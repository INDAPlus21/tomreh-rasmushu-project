workspace "tomreh-rasmushu-project"
   configurations { "Debug", "Release" }
   platforms { "Win32", "MacOS", "x64"}

   filter "platforms:Win32"
      system "windows"
      architecture "x86"

   filter { "platforms:x64" }
      system "windows"
      architecture "x64"

   filter { "platforms:MacOS" }
      system "macosx"
      architecture "x64"

   filter { }


project "Rayman"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}"

   files { "./src/**.h", "./src/**.cpp" }

   includedirs {
      "dep/glfw-3.3.6/include",
      "dep/glew-2.2.0_1/include",
   }

      filter { "system:windows" }
         links {
            "glfw3",
            "glew32s",
            "user32",
            "gdi32",
            "kernel32",
            "opengl32"
         }

      filter { "system:macosx" }
         links {
            "glfw",
            "glew",
            "OpenGL.framework"
         }

      filter { "system:windows", "architecture:x86" } 
         libdirs {
            "dep/glfw-3.3.6/win32",
            "dep/glew-2.2.0_1/win32"
         }
   
      filter { "system:windows", "architecture:x64" }
         libdirs {
            "./dep/glfw-3.3.6/x64",
            "./dep/glew-2.2.0_1/x64"
         }

      filter { "system:macosx", "architecture:x64" }
         libdirs {
            "dep/glfw-3.3.6/macos",
            "dep/glew-2.2.0_1/macos",
         }

         buildoptions {"-F /Library/Frameworks"}
         linkoptions {"-F /Library/Frameworks"}

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
   
   filter { }