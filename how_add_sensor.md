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




# MEET: 2022_09_09 - notes on eLoom framework 

## HW- task (button)
Il task dell'application CONTEXT viene inizializzato in un file che 
si trova  in _"Middlewares/ST/eLoom/scr/services/sysinit.c"_   l'oggetto viene definito alla riga 438
e inizializzato subito dopo:

```
/* Allocate the global application context*/
  ApplicationContext xContext; "
```

all'interno di App.c viene chiamata la funzione 
```
sys_error_code_t SysLoadApplicationContext(ApplicationContext *pAppContext);
```
che come dice la relative docstring:
```
/**
 * This function is used by the system in order to add all the managed tasks to the application context.
 * It is defined as weak in order to allow the user application to redefine it.
 *
 * @param pAppContext [IN] specifies a pointer to the application context to load with the application specific managed tasks.
 * @return SYS_NO_ERROR_CODE if success, an error code otherwise. """
``` 

Il task che gestice il pulsante si chiama: 
```
/**
 * Application managed task.
 */
static AManagedTask *sAppTaskObj = NULL;"
```

Il file di riferimento per la task che gestice il pulsante è: _"PnPL_AI_Dpu\user\Core\Src\AppTask.c"_
in questo file puoi trovare alla riga 274 il messaggio di debug: 
```
"SYS_DEBUGF(SYS_DBG_LEVEL_VERBOSE, ("HW: start.\r\n"));"
```
che è appunto quello che si vede anche nel terminale di tera term. 

In fondo a questo file c'è la "cube MX integration" e la funzione:
``` 
App_PB_EXTI_Callback(uint16_t pin)" 
```
che provoca un passaggio di stato della macchina virtuale che a sua volta permette l'attivazione del microfono



## DPUs   
 
Per quanto riguarda le DPU ( da Pag 20 del file che ti ha mandato Alberto sull' _FP-MOITOR-1_ sono spiegate abbastanza bene) aggiungerei le seguenti cose: 

### Esempio- attaccare una task come listener di una DPU: 

Quello che fa questa funzione è registrare la task "AppTask" ( che come ti dicevo prima è quella del pulsante)
come listener per eventi mandati dalla specifica DPU definita all'interno della task "ProcessTask". (questa funzione faceva parte dell'architettura iniziale del firmware - attualemente non usata nel mio progetto- alla "AppTask" nell'architettura precedente arrivavano i risultati della calssificazione dell'AI, dopodichè l'AppTask si premurava di stamparli a video!) 

(file: App.c -riga 169)
```
/* connect the App task object to the Process task.*/
  ProcessTaskAddDPUListener((ProcessTask_t*)sProcessTaskObj, ...  
  ...AppTaskGetProcEvtListenerIF((AppTask*)sAppTaskObj));
```



### Esempio: attaccare un sensore a una DPU.
Se la DPU deve essere connessa a un sensore o comunuque un oggetto che trasferisce dati, quell'oggetto deve implementare la classe _"ISourceObservable"_. 

Dato che la DPU è un oggetto che fa parte del task _"ProcessTask"_ la funzione che attacca il sensore
si chiama "ProcessTaskAttachToSensor" e viene chiamata NON da puntatore/virtual tables/messaggio ma direttamente all'interno della funzione _"ProcessTask_vtblOnEnterTaskControlLoop"_ (tramite virtual table). 

Riassumendo il flow:  
""ProcessTask_vtblOnEnterTaskControlLoop"&rarr;"ProcessTaskAttachToSensor"&rarr;"IDPU_AttachToSensor"&rarr;"ADPU_AttachToSensor_vtbl"

## COMUNICAZIONE EVENTI IMP34TASK &rarr; aiDPU

All'interno della task del microfono l'oggetto _"SensorEvent"_ viene definito non appena arrivano dei nuovi dati ed è composto da un "pointer to event source".  

1. La funzione _"SensorEventinit"_ si occupa di passare i valori dei dati che in questo momento sono nello scope della task all'oggetto _"SensorEvent"_. Adesso sensor event può essere usato come interfaccia per portare i dati all'esterno della task del microfono.  

2. Segue _"IEventSrcSendEvent"_, allo stesso modo di quanto detto per la DPU quando un oggetto vuole mandare un messaggio a un altro oggetto, secondo il framework deve implementare al suo interno l'oggetto _"IEventSrc"_, in questo caso all linea 974 del IMP34 task si una funzione che ha come input appunto questo oggetto e l'oggetto evento (_"SensorEvent"_).

IEventSrcSendEvent: chiama la funzione (tramite virtual table):  
"SensorEventSrc_vtblSendEvent" &rarr; "ISensorEventListenerOnNewDataReady" &rarr; "OnNewDataReady"(virtual)&rarr;"ADP_OnNewDataReady_vtbl" (ADPU.c)
 



\

\

\
 
*Author*: giovanni6.silva@mail.polimi.it
\
*Date last update*: 12-Set-2022