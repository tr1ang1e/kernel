
Reading 'Олег Цилюрик - Расширения ядра Linux: драйверы и модули"  
Building external (out-of-tree) kernel modules  
&nbsp;&nbsp;&nbsp;&nbsp;date bed: 2024-03-16  
&nbsp;&nbsp;&nbsp;&nbsp;date end: -  
  
  
Setup: Raspberry Pi 4 Model B Rev 1.4
Linux distribution: Debian GNU/Linux 11 (bullseye)
Kernel version:  
&nbsp;&nbsp;&nbsp;&nbsp;6.1.21-v8+  
&nbsp;&nbsp;&nbsp;&nbsp;aarch64-linux-gnu-gcc-8 (Ubuntu/Linaro 8.4.0-3ubuntu1) 8.4.0  
&nbsp;&nbsp;&nbsp;&nbsp;GNU Binutils for Ubuntu 2.34  
&nbsp;&nbsp;&nbsp;&nbsp;#1642 SMP PREEMPT Mon Apr  3 17:24:16 BST 2023  
  
  
How to deal with it:  
&nbsp;&nbsp;&nbsp;&nbsp;- If parent directory contains README file - read it  
&nbsp;&nbsp;&nbsp;&nbsp;- If current directory contains README file - read it  
&nbsp;&nbsp;&nbsp;&nbsp;- If something is still unclear, search in 'docs' dir  
  
  
Special marks. For marks to be higlighted install VSCode extension "Highlight" :  
https://marketplace.visualstudio.com/items?itemName=fabiospampinato.vscode-highlight.  
Highlighting rules are in the .vscode/settings.json file under the "highlight.regexes"  
&nbsp;&nbsp;&nbsp;&nbsp;TODO:    =  come back later  
&nbsp;&nbsp;&nbsp;&nbsp;[?]      =  unclear moments or smth to be studied later  
&nbsp;&nbsp;&nbsp;&nbsp;[!]      =  important moment  
&nbsp;&nbsp;&nbsp;&nbsp;[see ]   =  external link  
&nbsp;&nbsp;&nbsp;&nbsp;[:  :]   =  cross reference (for full list see docs/crossrefs.note)  
&nbsp;&nbsp;&nbsp;&nbsp;$ ...    =  shell command (sudo is always skipped)  
&nbsp;&nbsp;&nbsp;&nbsp;>> ...   =  code line  
     
  
Special names:  
&nbsp;&nbsp;&nbsp;&nbsp;ROOT    = project root directory  
&nbsp;&nbsp;&nbsp;&nbsp;DOCS    = ROOT/docs/  
&nbsp;&nbsp;&nbsp;&nbsp;KDIR    = /lib/modules/`uname -r`/  directory  
  
  
Functions naming:   ootm_<module>_<name>
  
  
Module source code examples testing:    see ROOT/tests/README.note  
  
  
Techonologies to be involved later  
&nbsp;&nbsp;&nbsp;&nbsp;profiler (https://linux-kernel-labs.github.io/refs/heads/master/labs/kernel_profiling.html)  
&nbsp;&nbsp;&nbsp;&nbsp;static and dynamic analizers (gcc, ...)  
&nbsp;&nbsp;&nbsp;&nbsp;multithread debugging (valgrind, ...)  
&nbsp;&nbsp;&nbsp;&nbsp;testing (gtest/gmock)  
