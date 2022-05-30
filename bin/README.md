This folder contains all necessary binaries:

* datalogger firmware to be flashed to STWIN

* python HSD SDK?

* HSD CLI and DLL?

* Vespucci HSD extension for VSCode

## HSD CLI
The command line interface (CLI) has been extracted from FP-SNS_DATALOG1 v.1.3.0 utilities.
It is meant to run on Linux x86 only. For other platforms, refer to the original package.

In order to run the CLI under Linux as a non-root user, the file 30-hsdatalog.rules should be copied into /etc/udev/rules.d/ folder and the user should be a member of the 'hsdatalog' group.

Issues
* scripts contain ^M characters (need dos2unix)
* scripts permissions not set (need chmod u+x)
* lib_hsdatalog.so must be copied to /usr/lib as root (sudo cp..)
* specify path for captured files ==> use -o
* datalog folder owner is root ==> solved with udev rules