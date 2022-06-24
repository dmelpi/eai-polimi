import pandas as pd
import numpy as np
from scipy import signal
from tqdm import tqdm
import matplotlib.pyplot as plt
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import confusion_matrix
from sklearn.metrics import ConfusionMatrixDisplay
import tensorflow as tf
import HSD_utils.logger as logger
from HSD.HSDatalog import HSDatalog
from scipy.fft import fft
import os
import shutil


#######################################################################################################
#########################################      LOAD DATA      ######################################### 
#######################################################################################################

def load_data(resampling_active=False, Resampling_frequency = 6667, folder_name = 'datasets'):
    # load acquired data
    Accelerometer = 'ISM330DHCX'
    classes = np.array(['Slow','Fast','Slow_dist','Fast_dist'])

    hsd = HSDatalog(folder_name+'/'+classes[0])
    df_ACC_S = hsd.get_dataframe(Accelerometer,'ACC',sample_start=0,sample_end=-1)
    df_ACC_S = df_ACC_S[['Time','A_x [g]','A_y [g]','A_z [g]']]

    hsd = HSDatalog(folder_name+'/'+classes[1])
    df_ACC_F = hsd.get_dataframe(Accelerometer,'ACC',sample_start=0,sample_end=-1)
    df_ACC_F = df_ACC_F[['Time','A_x [g]','A_y [g]','A_z [g]']]

    hsd = HSDatalog(folder_name+'/'+classes[2])
    df_ACC_SD = hsd.get_dataframe(Accelerometer,'ACC',sample_start=0,sample_end=-1)
    df_ACC_SD = df_ACC_SD[['Time','A_x [g]','A_y [g]','A_z [g]']]

    hsd = HSDatalog(folder_name+'/'+classes[3])
    df_ACC_FD = hsd.get_dataframe(Accelerometer,'ACC',sample_start=0,sample_end=-1)
    df_ACC_FD = df_ACC_FD[['Time','A_x [g]','A_y [g]','A_z [g]']]

    # SIGNAL RESAMPLING IF NEEDED  
                                
    Available_res_freq = np.array([12.5 , 26 , 52 , 104, 208, 416, 833, 1666, 3332, 6667])
    
    print('Measured ODR:', 1/(df_ACC_S.iloc[5,0]- df_ACC_S.iloc[4,0]))
    print('')

    if resampling_active == True:
        if Resampling_frequency not in Available_res_freq:
            print('########  NEW SAMPLING FREQUENCY CHOSEN NOT AVAILABLE THEN NO RESAPLING HAS BEEN APPLIED  ########')
            print('')
        else:
            df_ACC_S = signal_resampling(df_ACC_S, Resampling_frequency)
            df_ACC_F = signal_resampling(df_ACC_F, Resampling_frequency)
            df_ACC_SD = signal_resampling(df_ACC_SD, Resampling_frequency)
            df_ACC_FD = signal_resampling(df_ACC_FD, Resampling_frequency)
            

    print('ODR after resampling:', 1/(df_ACC_S.iloc[5,0]- df_ACC_S.iloc[4,0]))

    return df_ACC_S, df_ACC_F, df_ACC_SD, df_ACC_FD



#######################################################################################################
#########################################      RESAMPLING      ########################################
#######################################################################################################

def signal_resampling(DataFrame, resampling_f):
    Data=np.array(DataFrame.iloc[:,0:4])
    Number_samples = (Data[-1,0]-Data[0,0])*resampling_f
    dt = 1/resampling_f
    new_time = np.arange(0,dt*Number_samples-dt,dt)
    f_x = signal.resample(Data[:,1], int(Number_samples))
    f_y = signal.resample(Data[:,2], int(Number_samples))
    f_z = signal.resample(Data[:,3], int(Number_samples))
    
    New_Data = np.zeros((int(Number_samples),4))
    New_Data[:,0]=new_time
    New_Data[:,1]=f_x
    New_Data[:,2]=f_y
    New_Data[:,3]=f_z
    
    New_Data_df = pd.DataFrame(New_Data, columns = DataFrame.columns)

    return New_Data_df



#######################################################################################################
##############################        PLOT SIGNAL DISTRIBUTION       ##################################
#######################################################################################################

def plot_variable_distributions(trainX, title):
    
    cut= int(trainX.shape[1] )
    longX = trainX[:, -cut:, :]
    
    longX = longX.reshape((longX.shape[0] * longX.shape[1], longX.shape[2]))
    plt.figure()
    xaxis = None
    
    for i in range(longX.shape[1]):
        ax = plt.subplot(longX.shape[1], 1, i+1, sharex=xaxis)
        ax.set_xlim(-1, 1)
        if i==0:
            xaxis = ax
            ax.set_title(title)
        
        plt.hist(longX[:, i], bins=100)
    plt.show()
    


#######################################################################################################
##########################        SUMMARIZE THE EXPERIMENT RESULT       ###############################
#######################################################################################################

def summarize_results(scores, params):
 	# summarize mean and standard deviation
    for i in range(len(scores)):
        m, s = np.mean(scores[i]), np.std(scores[i])
        print('Param=%s: %.3f%% (+/-%.3f)' % (params[i], m, s))
 	# boxplot of scores
    plt.boxplot(scores, labels=params)
    plt.ylabel('Overall test accuracy')


#######################################################################################################
##################################        WELCH SPECTRUM         ######################################
#######################################################################################################

def signal_welch(DataFrame, sensor_Type, n_avg):
    #Insert the entire Dataframe from where take data
    #Insert the sensor Type named in the Dataframe (ex. 'MIC')
    Data=np.array(DataFrame[sensor_Type])
    T=DataFrame.iloc[-1,0]-DataFrame.iloc[0,0]
    sampling_time=DataFrame.iloc[3,0]-DataFrame.iloc[2,0]
    sampling_f=1/sampling_time
    delta_f=1/T
    f, Pxx_den = signal.welch(Data, sampling_f,window='hann', nperseg=len(Data)/n_avg)
    return f,Pxx_den



#######################################################################################################
###################################      EVALUATE THE AI MODEL      ###################################
#######################################################################################################

def evaluate_model(trainX, trainy, validationX, validationy, testX, testy , model, learningRate, nEpochs = 5000 ,batchSize=128, verbosity=0, learning_curves=False, ConfusionMatrix=False ):

    decayRate = 1e-6
    nrSamplesPostValid = 2

    # early stopping callback
    callback = tf.keras.callbacks.EarlyStopping(monitor='val_loss', patience=15, restore_best_weights=True)


    adam = tf.keras.optimizers.Adam( lr = learningRate, decay = decayRate )
    model.compile( loss ='categorical_crossentropy', optimizer = adam, metrics = [ 'acc' ] )
         
    hist=model.fit( trainX, trainy, epochs = nEpochs, validation_data = ( validationX, validationy ), batch_size = batchSize
     			, verbose = verbosity , callbacks=[callback])  

    # evaluate model
    _, accuracy = model.evaluate(testX, testy, batch_size=batchSize, verbose=0)
    
    if learning_curves == True:

        fig = plt.figure( )
        ax = fig.add_subplot( 121 )
        ax.plot( hist.history[ 'acc' ], linewidth = 2. )
        ax.plot( hist.history[ 'val_acc' ], linewidth = 2. )
        plt.title( ' Accuracy' )
        plt.ylabel( 'Accuracy' )
        plt.xlabel( 'Epoch' )
        plt.legend( [ 'Train', 'Validation' ], loc = 'upper left' )


        ax = fig.add_subplot( 122 )
        ax.plot( hist.history[ 'loss' ], linewidth = 2. )
        ax.plot( hist.history[ 'val_loss' ], linewidth = 2. )
        plt.title( ' Loss' )
        plt.ylabel( 'Loss' )
        plt.xlabel( 'Epoch' )
        plt.legend( [ 'Train', 'Validation' ], loc = 'upper left' )
    
    if ConfusionMatrix == True:

        predictions=model.predict(testX)
        for ii in range(predictions.shape[0]):
            for jj in range(predictions.shape[1]):
                   if predictions[ii,jj]>0.8:
                        predictions[ii,jj]=1
                   else:
                        predictions[ii,jj]=0

        cm=confusion_matrix(testy.argmax(axis=1), predictions.argmax(axis=1))

        labels=['Slow','Fast','Slow_disturbance','Fast_disturbance']
        disp = ConfusionMatrixDisplay(confusion_matrix=cm, display_labels=labels)
        disp.plot(cmap=plt.cm.Blues)
        plt.show()

        precision=np.empty(4)
        recall=np.empty(4)

        for ii in range(len(cm)):
            recall[ii]=cm[ii,ii]/sum(cm[ii,:])
            
        for ii in range(len(cm)):
            precision[ii]=cm[ii,ii]/sum(cm[:,ii])    
            
        print('precision: ', precision)
        print('recall: ',recall)
        
    return model, accuracy




#######################################################################################################
##################################      LOAD EXISTING MODEL      ######################################
#######################################################################################################


def load_existing_model(model_name, n_last_trainable_layers = 3, out_layer = True):
     
    exist  = os.path.exists('Saved_models/'+ model_name)
    if exist == True:
        ##model_1 = tf.keras.models.load_model('Saved_models/mfcc.h5')
        if out_layer == True:
            base_model_non_trainable = tf.keras.models.Sequential((tf.keras.models.load_model('Saved_models/'+ model_name+'.h5')).layers[1:-n_last_trainable_layers])
            input_layer = tf.keras.models.Sequential((tf.keras.models.load_model('Saved_models/'+ model_name+'.h5')).layers[0])
            base_model_trainable = tf.keras.models.Sequential((tf.keras.models.load_model('Saved_models/'+ model_name+'.h5')).layers[-n_last_trainable_layers:-1])
            output_layer = tf.keras.models.Sequential((tf.keras.models.load_model('Saved_models/'+ model_name+'.h5')).layers[-1])
            
            input_layer.trainable = False
            base_model_non_trainable.trainable=False
            
            model = tf.keras.models.Sequential([input_layer, base_model_non_trainable, base_model_trainable, output_layer])
        else:
            
            base_model_non_trainable = tf.keras.models.Sequential((tf.keras.models.load_model('Saved_models/'+ model_name+'.h5')).layers[1:-n_last_trainable_layers])
            input_layer = tf.keras.models.Sequential((tf.keras.models.load_model('Saved_models/'+ model_name+'.h5')).layers[0])
            base_model_trainable = tf.keras.models.Sequential((tf.keras.models.load_model('Saved_models/'+ model_name+'.h5')).layers[-n_last_trainable_layers:-1])
            
            input_layer.trainable = False
            base_model_non_trainable.trainable=False
            
            model = tf.keras.models.Sequential([input_layer, base_model_non_trainable, base_model_trainable])
        
        model.summary()

        return model
        
    
    else:
        print()
        raise SystemExit('The model named '+ model_name + ' does not exist in the folder Saved_models/')

    


#######################################################################################################
####################################         SAVE MODEL       #########################################
#######################################################################################################

def save_model(model, model_name):

    save_model_answer = input("Do you want to save this model? yes/no: ")
    exist  = os.path.exists('Saved_models/'+ model_name)
    if exist == True:
        if save_model_answer == 'yes':
            overwrite_model = input("Do you want to overwrite this model? yes/no: ")
            if overwrite_model == 'yes':
                shutil.rmtree('Saved_models/'+ model_name)
                os.remove('Saved_models/'+ model_name+'.h5')
                model.save('Saved_models/'+ model_name+'.h5')
                model.save('Saved_models/'+ model_name)
            else:
                model_name = input("Insert a different model name: ")
                model.save('Saved_models/'+ model_name+'.h5')
                model.save('Saved_models/'+ model_name)
    
    else:
                model.save('Saved_models/'+ model_name+'.h5')
                model.save('Saved_models/'+ model_name)
         
        
    
#######################################################################################################
###############################        Spectrogram Matrix Plot        #################################
#######################################################################################################

def show_spectrogram(X,ratio,limits,cmin=None,cmax=None, title='Insert a title', x_label ="Time [s]", y_label = "Frequency [Hz]"):

  w, h=plt.figaspect(ratio)
  fig = plt.figure(figsize=(w*2,h*2))
  ax = fig.add_axes([0.1,0.1,0.8,0.8])
  im = ax.imshow(X, extent = limits, cmap ='viridis',interpolation='none')
  cmin = np.min(X)
  xleft, xright = ax.get_xlim()
  ybottom, ytop = ax.get_ylim()
  ax.set_aspect(abs((xright-xleft)/(ybottom-ytop))*ratio)
  ax.invert_yaxis()
  ax.set_xlabel(x_label)
  ax.set_ylabel(y_label)
  ax.set_title(title)

  if cmin != None:
    cbar = plt.colorbar(im);
    cbar.mappable.set_clim(cmin,cmax);



