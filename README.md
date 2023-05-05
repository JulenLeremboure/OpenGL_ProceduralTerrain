# How to install

#### CMake

Make sure to get the latest version of CMake. The project works with 3.24 and above.

#### vcpkg

Get vcpkg from here : https://vcpkg.io/en/getting-started.html

Or run those commands, preferably at the root of your user folder (Windows) :

```bash
git clone https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.bat
./vcpkg/vcpkg.exe integrate install
```

#### Visual Studio

- Open the projet folder into Visual Studio

- If nothing happens, try to save CMakeLists.txt with CTRL + S. All dependencies should build (this take a while)

- Next to the Play mode icon, change the build target into the generated .exe.

- Enjoie.

If an error occurs concerning your CMake version, maybe your CMake has not been detected. Into CMakeSettings.txt, check for the "cmakeexecutable" path.
