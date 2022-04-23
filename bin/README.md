This folder contains all necessary binaries:

* datalogger firmware to be flashed to STWIN

* python HSD SDK?

* HSD CLI and DLL?

* Vespucci HSD extension for VSCode

## HSD CLI
The command line interface (CLI) has been extracted from FP-SNS_DATALOG1 v.1.3.0 utilities.
It is meant to run on Linux x86 only. For other platforms, refer to the original package.

Issues
* scripts contain ^M characters (need dos2unix)
* scripts permissions not set (need chmod u+x)
* .so must be copied to /usr/lib
* cannot specify path for captured files
* datalog folder owner is root