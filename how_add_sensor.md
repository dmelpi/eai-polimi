# SENSORS INITIALIZATION 
*A summary of lessons learned during the initialization of the MEMS microphone sensor IMP34DT05.*

---
\
The following steps reports some critical points encountered during the initialization of the **IMP34DT05** sensor on the 'Stwin.box' board. The starting firmware was **PnPL_AI_DPU_STEVAL-STWINBX1** based on ThreadX, SensorsManager and DPUs.

### Brief to-do list:  


1.   Use cubeMX to correctly configure the pins and drivers relative to 
     the sensors you want to initialize[^1]


2.   Import[^2]   the sensor task from the SensorManager folder in the project


3.   Try to build the project and import missing includes


4.   Use keyboard shortcut  *'CTRL+H'*  and check for 'HAL_XXX_MODULE_ENABLE' where 'XXX' it's the driver's name used by the sensor (ex. 'HAL_MDF_MODULE_ENABLE') 


5.   Use *'CTRL+H'* and check for 'USE_HAL_XXX_REGISTER_CALLBACKS' and set equal to  1U


6.    If you import a sensor task be sure to enable system debug callbacks
   *'CTRL+H'* write "SYSDEBUG_xxx", where 'sensorName' 
   and set  definition equal to 1U otherwise you will not see any 'sysdebug' 


7. To check if everything is working uncomment the:
   '//SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("IMP34DT05: new data.\r\n"));'
   if the sensor is active it will continuously print this message


### Notes:

[^1] Don't take the MX configuration from other projects where the sensor is 
already initialized (e.g. project Datalog2) 

[^2] How to import tasks & files: check if the file is already in the project 
directories (Ex. the sensor tasks such "IMP34DT05task.c"), if present 
link it from the file explorer (drag and drop the file and choose 'link' 
option in the dialog window) else copy the file in the correct project's folder


\

\

\
 
*Author*: giovanni6.silva@mail.polimi.it
\
*Date last update*: 02-Aug-2022