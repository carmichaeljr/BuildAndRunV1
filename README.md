# BuildAndRun
A command line tool that simplifies the compilation of projects.

To run this tool, make sure you call the BuildAndRun.bat file, and **NOT** the BuildAndRun.exe file. Doing this is a patch that will temporarily change the directory to the given files directory, so that the compiler can find all relivant resources.

The possible arguments are shown below:
BuildAndRun.bat [option] [FileDir]
    **OPTION** can be one of the following: 's','c','e', or 'ce'
      - 's': Open settings for modification
      - 'c': Compile the given file
      - 'e': Execute the given file with the specified intrepreter
      - 'ce': Commpile and then execute the given file
