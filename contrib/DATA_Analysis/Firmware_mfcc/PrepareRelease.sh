#!/bin/bash -v
Prj="Projects/STM32L4R9ZI-STWIN/Applications/FP-AI-MONITOR1"
ToRemove="../.repo
.project
Middlewares/ST/NanoEdge_AI_Library/Lib/GCC/ARM_CM33_NTZ
Middlewares/ST/NanoEdge_AI_Library/.project
Middlewares/ST/eLooM/.project
$Prj/tools
Package_license.md"

for F in $ToRemove; do
    echo deleting $F
    rm -rf $F
done

#echo deleting Release_Notes.md
#find . -name "Release_Notes.md" -exec rm -rf {} \;

echo deleting doxyfile
find . -name "*.doxyfile" -exec rm -rf {} \;
find . -name "Doxyfile"  -exec rm -rf {} \;

echo deleting git repositories
find .. -name ".git" | xargs /bin/rm -rf "{}"
find .. -name "*.gitignore" -exec rm -rf {} +

echo deleting unused CMSIS libraries
find Drivers/CMSIS/Lib/ -type f ! -name "*M4lf*" -exec rm -rf {} +

echo keeping only chm in documentation folder
find Documentation/* -maxdepth 0 ! -name '*.chm' -exec rm -rf {} +

echo removing doc repositories
rm -rf $Prj/doc
rm -rf $Prj/DPU/doc
rm -rf $Prj/SensorManager/doc
rm -rf $Prj/doc
rm -rf Middlewares/ST/eLooM/doc

echo removing validation scripts 
rm -rf $Prj/Monitor_Validation

echo cleaning IAR 
find $Prj/EWARM/* -maxdepth 0 -type d | xargs /bin/rm -rf "{}"
rm -rf $Prj/EWARM/Project.dep

echo cleaning Keil
find $Prj/MDK-ARM/* -maxdepth 0 -type d | xargs /bin/rm -rf "{}"
rm -rf $Prj/MDK-ARM/*.lst
rm -rf $Prj/MDK-ARM/Project.uvguix*

echo cleaning CubeIde 
find $Prj/STM32CubeIDE/Application/ -maxdepth 1 ! -name Startup ! -name Application -exec rm -rf {} +
find $Prj/STM32CubeIDE/ -maxdepth 1 -type d  ! -name Application ! -name STM32CubeIDE -exec rm -rf {} +

echo --- setting versions in files ----
for FileName in `ls $Prj/Src/*.[c-h] $Prj/Inc/*.[c-h] $Prj/readme.txt`; do
  echo "Changing $FileName"
  sed -i 's/\$Version\$/V1\.0\.0/' $FileName
  sed -i 's/\$Date\$/15-September-2021/' $FileName
done
 