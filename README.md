# Lottery CPU Scheduler
Operating Systems | University of Michigan-Dearborn | Dearborn, MI | 2019 </br>
<i>All verbiage in this document (unless otherwise stated) was provided by Professor Guo at University of Michigan-Dearborn.</i>

# Description
In this project, you'll implement a simpler version of lottery CPU scheduler in xv6.

The basic idea is simple:nassign each running process a slice of the processor based in proportion to the
number of tickets it has; the more tickets a process has, the more it runs.

Each time slice, a randomized lottery determines the winner of the lottery; that winning process is the one that runs
for that time slice.

# Objectives
  1) To gain further knowledge of a real kernel, xv6.
  2) To familiarize yourself with a scheduler.
  3) To change that scheduler to a new algorithm.
  
# Main Tasks
You'll need three new system calls to implement this scheduler:

  # 1) System call, settickets
  The first is int settickets(int number), which sets the number of tickets of the calling process.
  
  By default, processes should have <b>10 tickets.
  
  You can assume that the <b>maximum number of tickets per process is 100000.
  
  The number of tickets should be inherited by children created via fork.
  
  This routine should return 0 if successful, and -1 otherwise (If, for example, the caller passes in a number less than one or more than 100000).

  # 2) System call, getpinfo
  The second is int getpinfo(struct pstat *). 
  
  This routine returns some information about all running processes, including <b>how many times each has been chosen to run and the process ID of each.
  
  You can use this system call to build a variant of the command line program ps, which can then be called to see what is going on. 
  
  The structure pstat is defined below; note, you cannot change this structure, and must use it exactly as is.
  
  This routine should return 0 if successful, and -1 otherwise(if, for example, a bad or NULL pointer is passed into the kernel).

  The structure should look like what you see here, in a file you'll have to include called pstat.h:
  
    #ifndef _PSTAH_H_
    #define _PSTAT_H_
    #include "param.h"
    struct pstat {
    int num_processes; // the total number of non-UNUSED processes in the process table
    int pid[NPROC]; // the PID of each process
    int tickets[NPROC]; // the number of tickets this process has
    int ticks[NPROC]; // the number of ticks each process has accumulated
    };
    #endif // _PSTAT_H_
        
  # 3) System call, yield
  The third system is int yield(). 
  
  This system call is just a wrapper. 
  
  It simply calls the real implementation yield() and return 0.
    
# Lottery scheduler
  Most of the code for the scheduler is quite localized and can be found in proc.c; the associated
  header file, proc.h is also quite useful to examine. 
  
  To change the scheduler, not much needs to be done; study its control flow and then try some small changes.
  
  You'll need to assign tickets to a process when it is created. 
  
  Specifically, you'll need to make sure a child process inherits the same number of tickets as its parents. 
  
  Thus, if the parent has 10 tickets, and calls fork() to create a child process, the child should also get 10 tickets.
    
# Supplied Test Programs
  # 1) ps.c
  We have supplied ps.c which is a test program which runs getpinfo and outputs the information in the struct.
     
  # 2) lotteryTest.c
  We have supplied lotteryTest.c which is a test program which:
  
  Takes as command line arguments an amount of time to run for followed by a list of numbers of tickets to assign to each subprocess. 
  
  Each subprocess runs an infinite loop and is killed after running of the designated amount of time.
  
  Outputs a table of the programs in order, with the number of tickets assigned to each and the number of ticks it ran, as reported by getpinfo.
  
  Reports an error if getpinfo
    o the num_processes field returned is negative or exceeds NPROC
    o indicated that a child process had the wrong number of tickets;
    o was missing a child process
    
  Note, there are two CPUs in xv6. 
  
  You will need to create at least three subprocesses to test the lottery scheduler. 
  
  In your report, you will should include at least the following tests:</br>
    • lotteryTest 1000 500 400 100 50 10
    • lotteryTest 2000 200 200 200 100 100 200
    • lotteryTest 2000 10 20 50 100 200 300
    
  In order to make your new ps and lotteryTest programs available to run from the xv6 shell, dd _ps and _lotteryTest to the UPROGS definition in Makefile.
  
# Hints
# 1) Reading on Lottery Scheduling
  1. For an alternate explanation to the lottery scheduler, see Chapter 9 of Arpaci-Dusseau (especially Figure 9.1).
  
# 2) Reading on xv6’s scheduler
  1. Read Chapter 5 of the xv6 book for documentation on xv6’s existing scheduler.
  
# 3) Suggested order of operations
  1. Implement settickets, but don’t actually use ticket counts for anything.
  2. Implement getpinfo. Use the ps.c to verify that it works.
  3. Add tracking of the number of ticks a process runs. Use the ps.c to verify that it works.
  4. Implement the lottery scheduling algorithm. Use the lotteryTest.c to test it.
  
# 4) Tracking the number of ticks a process has been running
  1. proc.h contains xv6’s process control block, to which you should add a member variable to track the number of ticks a process has used.
  2. You may need to modify fork (in proc.c) or related functions to make sure the tick count and the number of tickets are initialized correctly.
  
# 5) Adding settickets
  1. You can use argint to retrieve the integer argument to your system call. (Making sys_setticketstake an argument will not work).
  2. Like for tracking the number of ticks a process has been running, you will need to edit the process control block in proc.h.
  3. Follow the example of similar system calls in sysproc.c.
  
# 6) Adding getpinfo
  1. You can use the argptr to retrieve the pointer argument in your system call handler.
  2. You should iterate through the process list ptable , skipping over UNUSED processes.
  3. Look at the code for kill in proc.c for an example of how to search through the list of processes by pid.
  4. Before and after accessing the process table (ptable ), you should acquire ptable.lock and afterwards you should release it. You can see an example of this in kill in proc.c. This will keep you from running into problems if a process is removed while you are iterating through the process table.
  
# 7) Adding the lottery scheduling algorithm
  1. You will need to add a psuedorandom number generator to the kernel. We’ve supplied a
  Multiply-with-carry pseudorandom number generator (random.h). To use it, you will
  need to initialize it first with srand(seed)and then call rand() % (MAX + 1),
  which will generate an random integer number between 0 and MAX.
  
  2. It is okay if your pseudorandom number generator uses a fixed seed. But if you don’t want
  to do this xv6 has a function cmostime that reads the current time of day.
  
  3. The logic in schedule implements a round-robin scheduling algorithm, by iterating
  through all processes, scheduling each one as it iterates though them. You most likely will
  modify it to instead, iterate through almost all processes each time a new process needs to
  be scheduled to choose which one to run.
  
  4. getpinfo provides you the information you need to test that your lottery scheduler
  behaves correctly. You will not need it in the implementation of the scheduler itself.
  5. When there are no runnable processes, your scheduler should release the process table lock
  to give interrupts (like for keypresses) a chance to make programs runnable, then re-acquire
  that lock and iterate through the process table again.
  
# 8) Identifying panics
  If xv6 prints a message containing something like panic: acquire, this means that
  something called panic("acquire").
  
  The panic() function stops the OS, printing out an error message. 
  
  Generally, these panics are caused by assertions in the xv6 code, checking the current state is consistent.
  
  Most xv6 panic messages include the name of the function that called panic, so you can often search for that function name and see when it called panic.
  
  In general, you can get grep the xv6 code to find out what exactly the cause was.
  
  For example, panic("acquire"); appears in acquire() in spinlock.c.
  It is called if a thread tries to acquire a spinlock that the current thread already holds.
