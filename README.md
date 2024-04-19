
Reading 'Олег Цилюрик - Расширения ядра Linux: драйверы и модули"  
Building external (out-of-tree) kernel modules  
    date bed: 2024-03-16  
    date end: -  


Setup: Raspberry Pi 4 Model B Rev 1.4
Linux distribution: Debian GNU/Linux 11 (bullseye)
Kernel version:  
    6.1.21-v8+  
    aarch64-linux-gnu-gcc-8 (Ubuntu/Linaro 8.4.0-3ubuntu1) 8.4.0  
    GNU Binutils for Ubuntu 2.34  
    #1642 SMP PREEMPT Mon Apr  3 17:24:16 BST 2023  


How to deal with it:
    - If parent directory contains README file - read it
    - If current directory contains README file - read it
    - If something is still unclear, search in 'docs' dir


Special marks. For marks to be higlighted install VSCode extension "Highlight" :
https://marketplace.visualstudio.com/items?itemName=fabiospampinato.vscode-highlight.
Highlighting rules are in the .vscode/settings.json file under the "highlight.regexes"
    TODO:    =  come back later  
    [?]      =  unclear moments or smth to be studied later  
    [!]      =  important moment  
    [see ]   =  external link
    [:  :]   =  cross reference  
    $ ...    =  shell command (sudo is always skipped)  
    >> ...   =  code line
    

Special names:  
    ROOT    = project root directory
    KDIR    = /lib/modules/`uname -r`/ directory


Techonologies to be involved later
    profiler (https://linux-kernel-labs.github.io/refs/heads/master/labs/kernel_profiling.html)  
    static and dynamic analizers (gcc, ...)  
    multithread debugging (valgrind, ...)  
    testing (gtest/gmock)  
