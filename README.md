

[ HOW TO EXECUTE ]


0. Check downloaded program "MinGW Installer".
    If not, MUST need to download it. => https://sourceforge.net/projects/mingw/





1. Chck if "C:/MinGW/bin/" is registered in Windows Environment Variables.
    If not, follow here
        Kor => https://codedragon.tistory.com/3260
        Eng => https://stackoverflow.com/questions/5733220/how-do-i-add-the-mingw-bin-directory-to-my-system-path





2. Oepn PowerShell. (Not CommandPrompt, some texts could not work well.)





3. Now we are in a PowerShell window. 
   Copy&Paste following command, and check tool version.
        3-1. GCC version '6.3.0-1'.     (GCC == Clang compiler)
            command) gcc --version 
        3-2. mingw32-make version is 'GNU Make 3.82.90'.
            command) mingw32-make --version

    If not, MUST upgrade. Copy&Paste following commands in order.
        a) mingw-get update 
        b) mingw-get upgrade 
        c) gcc --version 





4. Now we are ready to play Undertale.
   Copy&Paste following command.
        command) Undertale.exe