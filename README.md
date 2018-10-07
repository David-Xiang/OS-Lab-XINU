# OS-Lab-XINU
My Operating System Lab course's project -- XINU embedded system by Purdue.
Three subdir are added:

1. x86 source code
2. emulator source code
3. galileo source code

### Lab 1

##### Exercise 1

Design a process to display all process and their attributes:

- Pid
- Name
- State
- Prio
- Ppid
- StackBase
- StackPtr
- StackSize
- SemWait
- MsgRecv
- Descriptor

##### Solution 1

(last line's stuffs are hard to display...)
I modified xsh_ps.c to add a new built-in commmand -- psx.
psx means it's enhanced version of command ps.