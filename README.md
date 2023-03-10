### MIT 6.1810 Fall 2022 Lab-Files Backup

* Course Website: [MIT 6.1810/6.S081 Fall 2022](https://pdos.csail.mit.edu/6.S081/2022/)

* If you want to start from scratch, use the [original git-repo for labs](https://pdos.csail.mit.edu/6.S081/2022/labs/util.html).

* This is a backup and read-only repo for my solutions on learning concepts in 6.1810 course, completed lab as below list:
    * util    (100/100, 20hrs)
    * syscall (40/40  , 5hrs )
    * pgtbl   (46/46  , 6hrs )
    * traps   (95/95  , 10hrs)
    * cow     (110/110, 10hrs)
    * thread  (60/60  , 5hrs )
    * net     (100/100, 9hrs )
    * lock    (80/80  , 8hrs )
    * fs      (100/100, 10hrs)
    * mmap    (140/140, 10hrs)

------

* Extra hints for lab
    * util-primes
        1. Observe when pipe would be actual removed while child exit(), add debug printf() in pipeclose() and upper function.
        2. Try to execute files from user/pptst0.c and user/pptst1.c, figure out how these code work?
    * cow-cowtest
        1. May need atomic-lock for fork-childs.
    * net-nettests
        1. Use the given lock is sufficient.
        2. If multiple operation stucks, think about which register could check the commited packet has been send.
    * fs-bigfile
        1. Delete `fs.img` before `make qemu`.
    * mmap-mmaptests
        1. Only implement the necessary functionality is fine.
        2. Think about how you have done the pgtbl lab.
        3. May need to clean valid page manually.

------

* Note: Operation environment is based on Win11/WSL2, but VirtoIO Disk Operation is relative slow to primitive system, so bigfile in fs-lab elasped 488s to complete.