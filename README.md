# Dump KCM

POC to dump Kerberos tickets from KCM using its socket. See related blog post: [https://lvruibr.github.io/kcmdump](https://lvruibr.github.io/kcmdump)

## Compilation

Standard C libraries are used along with libkrb5 which may have to be installed manually depending on the distribution.

```
gcc dumpkcm.c -o dumpkcm -lkrb5
```

## Usage

Running the binary without argument will dump Kerberos tickets from KCM to the /tmp directory. 

`./dumpkcm`

Running it as a standard user only allows extracting one's own tickets, while running it as root allows dumping all tickets from KCM.