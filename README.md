# How to install

#### CMake

Make sure to get the latest version of CMake. The project works with 3.24 and above.

#### vcpkg

Get vcpkg from here : https://vcpkg.io/en/getting-started.html

And don't forget to run those commands (Windows) :

```bash
.\vcpkg\bootstrap-vcpkg.bat
vcpkg integrate install
```

Then, install those dependencies :

```bash
.\vcpkg\vcpkg.exe install --triplet x64-windows
```

#### Visual Studio

- Open the projet folder into Visual Studio

- If nothing happens, try to save CMakeLists.txt with CTRL + S. All dependencies should build (this take a while)

- Next to the Play mode icon, change the build target into the generated .exe.

- Enjoie.
