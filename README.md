# Low-level Data Management Lab 1

Name: <add your name here>

Source and all credits goes to [Yuichi Sugiyama - simple strace](https://github.com/mmxsrup/simple-strace).

This is a simple version of the official [strace](https://www.man7.org/linux/man-pages/man1/strace.1.html) tool.
The purpose of this lab is to improve the tool, so it has more features.

## Compile

Build the program locally with the following command:

```shell
make
```

This might fail, when trying to generate the `syscall_table.h`, this is your first assigment, to fix it.

## Assignments

### Exercise 1

The code in its current form, it's not compiling successfully. Analyze the code and make sure to fix it properly, so it compiles.
Try to compile it 1 time and from there you can start debugging the code.

### Exercise 2

The actual output, it's very simple. We need to also get the result from the syscall, in the following format:
```text
[syscall_number] syscall_name = result
```
To achieve this, you will need to understand how the function `ptrace` is working.
Then make sure that you properly run the syscall and get the result value.
You can find more information in this [link](http://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/).
In the end, the output should resemble more to the `strace` output.

### Exercise 3

Get the data argument for the "write" syscall.
You can use once again this [link](http://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/) as reference.
There is already a function to get the string and some reference code, so you can focus on the syscall part.

### Exercise 4

Add a flag `--trace <syscall_number>`, where you can filter the output to only one syscall (using its number).
This should "mimic" the `strace` functionality when using the same flag.
The `<syscall>` can be `0` (for read), `1` (for write), and so on.

## Usage

```shell
./my_strace PROG [ARGS]
```

Once fixed, you can trace a command like `echo hello` like this:

```shell
./my_strace echo hello
```

And the output should be very similar to running the official strace command:
```shell
strace echo hello
```

## Docker

To use it with Docker, you first need to build the docker image:

```shell
docker build -t lab1 .
```

Then you can run it like this:
```shell
docker run --rm -ti -v $(pwd)/:/root/lab -w /root/lab lab1 /bin/bash -c "make; ./my_strace echo hello"
```

Or you can start a shell inside the container, and then run the commands manually:

```shell
docker run --rm -ti -v $(pwd)/:/root/lab -w /root/lab lab1 /bin/bash
# inside the container
make
./my_strace echo hello
```
