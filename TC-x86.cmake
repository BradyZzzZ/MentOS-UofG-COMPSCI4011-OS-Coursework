set (CMAKE_HOST_SYSTEM_NAME Darwin)

# the name of the target operating system
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR i686)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# where is the target environment located
set(CMAKE_FIND_ROOT_PATH "$ENV{HOMEBREW_CELLAR}/i686-elf-gcc"
                         "$ENV{HOMEBREW_CELLAR}/i686-elf-binutils"
)

set(CMAKE_AR           "$ENV{HOMEBREW_PREFIX}/bin/i686-elf-ar")
set(CMAKE_ASM_COMPILER "$ENV{HOMEBREW_PREFIX}/bin/i686-elf-gcc")
set(CMAKE_C_COMPILER   "$ENV{HOMEBREW_PREFIX}/bin/i686-elf-gcc")
set(CMAKE_CXX_COMPILER "$ENV{HOMEBREW_PREFIX}/bin/i686-elf-g++")
set(CMAKE_LINKER       "$ENV{HOMEBREW_PREFIX}/bin/i686-elf-ld")
set(CMAKE_OBJCOPY      "$ENV{HOMEBREW_PREFIX}/bin/i686-elf-objcopy")
set(CMAKE_RANLIB       "$ENV{HOMEBREW_PREFIX}/bin/i686-elf-ranlib")
set(CMAKE_SIZE         "$ENV{HOMEBREW_PREFIX}/bin/i686-elf-size")
set(CMAKE_STRIP        "$ENV{HOMEBREW_PREFIX}/bin/i686-elf-strip")



# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)