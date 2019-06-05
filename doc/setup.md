- git (https://git-scm.com/download/)

- Visual Studio Code (https://code.visualstudio.com/)  
  Plugins:
    - CMake Tools (vector-of-bool.cmake-tools)
    - Python (ms-python.python)
    - C++ (ms-vscode.cpptools)
    - Bookmarks (alefragnani.bookmarks)

- CMake (https://cmake.org/)  
  Install command-line tools:
  ```
    sudo /Applications/CMake.app/Contents/bin/cmake-gui --install
  ```

- Python 3 (https://www.python.org/downloads/)

- Clang (https://clang.llvm.org/get_started.html)  
  Before generating build scripts, do not forget to set the CMAKE_BUILD_TYPE to Release.  
  Once compiled run ...
  ```
  sudo make install
  ```

- Vulkan (https://vulkan.lunarg.com/sdk/home)  
  Add Vulkan dierectories to environment variables (~/.bash_profile) ...
  ```
  export VULKAN_ROOT="/Users/ebeau/Documents/dev/thirdparty/vulkan"
  export VULKAN_SDK_VERSION="1.1.82"
  export VULKAN_SDK="${VULKAN_ROOT}/macOS"
  export VK_ICD_FILENAMES="${VULKAN_SDK}/etc/vulkan/icd.d/MoltenVK_icd.json"
  export VK_LAYER_PATH="${VULKAN_SDK}/etc/vulkan/explicit_layer.d"
  export PATH="/Library/Frameworks/Python.framework/Versions/3.7/bin:${VULKAN_SDK}/bin:${PATH}"
  ```
  Reload user's environment variables ...
  ```
  > source ~/.bash_profile
  ```
  Set the same environment variables in VSCode launch.json files because otherwise you may be missing them ...
  ```json
  {
      "name": "sb_vk_triangle",
      "type": "cppdbg",
      "request": "launch",
      "program": "/Users/ebeau/Documents/dev/build/macos-vscode/perso/sb_vk_triangle/sb_vk_triangle",
      "args": [],
      "stopAtEntry": false,
      "cwd": "/Users/ebeau/Documents/dev/build/macos-vscode/perso/sb_vk_triangle",
      "environment": 
      [
          {
              "name" : "VULKAN_SDK",
              "value" : "/Users/ebeau/Documents/dev/thirdparty/vulkan/macOS"
          },
          {
              "name" : "VK_ICD_FILENAMES",
              "value" : "/Users/ebeau/Documents/dev/thirdparty/vulkan/macOS/etc/vulkan/icd.d/MoltenVK_icd.json"
          },
          {
              "name" : "VK_LAYER_PATH",
              "value" : "/Users/ebeau/Documents/dev/thirdparty/vulkan/macOS/etc/vulkan/explicit_layer.d"
          }
      ],
      "externalConsole": false,
      "MIMode": "lldb"
  }
  ```