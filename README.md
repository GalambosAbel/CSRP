# Project Structure
 ## cpp folder
  - code for ordering distance matrices based on different orderings and with the help of the NEOS Concorde TSP Server
 ## python folder
  - code for extracting and processing word embedding data from the BERT machine learning model
  - scripts for automatically submitting multiple jobs to NEOS by running the cpp program described above (`ordering_automation.py`) and getting some insights from the resulting output logs (`data_analysis.py`)
# CPP Setup
 *Note:* Due to the `xmlrpc` library not being Windows-native, it is suggested that the program be compiled on Linux for the following steps to have the highest likelyhood of success:
 - Make sure you have `curl` and `cmake` installed (can be installed through MSYS2 on Windows).
 - Initialize the submodules in `extern` with `git submodule update --init`
 - (WINDOWS ONLY) Navigate to `cpp/extern/xmlrpc-c/Windows/` and run `ConfigureWin32.bat` (make sure you run it from that folder since the script uses relative paths).
 - Build with `cmake`.
