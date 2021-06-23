ThreadWorkerDemo 
Author: Erno Pakarinen
email: codesmith-fi@gmail.com
Date: 26th of June 2021
Contact: see bottom

Description
===========

Updated for the blog article! Feel free to comment and/or send me email. 

This is a simple example of an extendable multi thread worker. 

After starting the demo, you can give following commands
    quit        will stop all worker threads and exit
    [number]    will start a task doing the given [number] of steps, first Idle worker is allocated for it

Each worker runs in a separate thread. The class `Worker` can be inherited and the actual work task implemented in the `doWork()` method which can be overridden in sub classes.

To use, create an instance of a `Worker` class (or sub class), and call the `work()` method to start a task. You can pass a lambda function to the method which will be used by the Worker thread if given. If not given the overridden `doWork()` will be called when task begins.

Building
========

To build, either open the MSVC 2019 project `ThreadWorkerDemo.sln` or build it with e.g. GCC using `make`. This project has no dependencies other than standard C++ library. It should compile with C++14 and upwards. 

Contact
=======
You can reach me through my email codesmith.fi@gmail.com

I also lurk in Discord with a id of "xanaki#9472" if you want to contact and talk with me. You can also find me from several coding channels like One Lone Coder and GameFromScratch for example.

My own discord channel is "Knights who say RPG" @ https://discord.gg/P4Uu3GU7tx
My blog I just started is @ https://codesmith.hashnode.dev/
My GitHub is located at @ https://github.com/codesmith-fi
