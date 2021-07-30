# ReverseShell

A simple C Reverse Shell.

## Disclaimer

Use for educational purposes only.

## Compilation

**Note: You need to set the Host and Port in <client_linux.c>**

Compile the client:
```bash
gcc client_linux.c -o client
```
Compile the client optimized for a extra small binary:
```bash
gcc -Os -s -ffunction-sections -fdata-sections -Wl,--gc-sections -fno-unroll-loops -fno-ident client_linux.c -o client
```

## Running the server

```bash
python3 server.py <host> <port>
```
