# ReverseShell

A simple C Reverse Shell.

## Disclaimer

Use for educational purposes only.

## Compilation

**Note: You need to set the Host and Port in <client_linux.c>**

Compile the client:
```bash
gcc -Wall -g client_linux.c -lm -o client
```

## Running the server

```bash
python3 server.py <host> <port>
```
