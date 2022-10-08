- Conan.io website: https://conan.io
- Search for a package from conan global server
    conan search <name> --remote=conancenter
- List currently installed packages in cache
    conan search *
- List currently installed packages in cache with filter
    conan search catch2/2.*
- Get information from a specific package
    conan inspect <name>/<version
- Install packages (working directory as to be where you want packages to be installed for your current project e.g. cmake build dir)
    conan install <path folder containing conanfile.txt> -pr=<path to profile file>
- Get infoamtion about current project (working directory as to be where packages have been installed e.g. cmake build dir)
    conan info <path folder conataining conanfile.txt>
- List local profiles
    conan profile list
- View a profile content
    conan profile show <name>
