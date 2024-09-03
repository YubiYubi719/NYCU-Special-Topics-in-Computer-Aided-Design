## Probelm Description
You can see it in *spec* directory

## Execution

```makefile
$ make
```

* Change CASENAME in line 15 to run different cases

```makefile
$ make run
```

## Auto_Checker
* You can also use the auto-checker to check the functionality

```makefile
$ ./01_autoCheck
```

## Optimization
* Since the comment in netlist files only have following 2 types:
1. single line comment // ... , and it will not be put in the middle of the command.
2. multi-line comment /* ... */, and it will not be put in the middle of the command.

* User can try not to use *removeComment* function and read netlist file during dealing with comments, it will be more faster than current version.