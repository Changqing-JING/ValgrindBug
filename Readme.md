create a debug build of valgrind
```shell
git clone https://sourceware.org/git/valgrind.git
```
Open Makefile.all.am, change -O2 to -Og

```shell
./autogen.sh
./configure
make -j 12
ln -s ./coregrind/vgpreload_core-amd64-linux.so ./memcheck/vgpreload_core-amd64-linux.so
```

example launch.json
```json
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) Launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/coregrind/valgrind",
            "args": [
                "--tool=memcheck",
                "--default-suppressions=no",
                "/mnt/hgfs/code/workspace/LearningProjects/ValgrindBug/build/ValgrindBug"
            ],
            "stopAtEntry": false,
            "cwd": "${fileDirname}",
            "environment": [{
                "name": "VALGRIND_LIB",
                "value": "${workspaceFolder}/memcheck"
            }],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description":  "Set Disassembly Flavor to Intel",
                    "text": "-gdb-set disassembly-flavor intel",
                    "ignoreFailures": true
                }
            ],
        }
        

    ]
}
```