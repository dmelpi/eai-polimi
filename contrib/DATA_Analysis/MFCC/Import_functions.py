import pandas as pd
import numpy as np
from scipy import signal
from tqdm import tqdm
import matplotlib.pyplot as plt
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import confusion_matrix
from sklearn.metrics import ConfusionMatrixDisplay
import tensorflow as tf
from HSDatalog_libs import HSDatalog as hsd
from scipy.fft import fft
import os
import shutil



#######################################################################################################
#####################################      SUPPORT FUNCTIONS      ##################################### 
#######################################################################################################

# Hi-pass IIR filter to separate the low-varying signal
# 26 Hz
A_COEFF = [1.0, -3.868656635, 5.614526749, -3.622760773, 0.8768966198]
B_COEFF = [0.9364275932, -3.745710373, 5.618565559, -3.745710373, 0.9364275932]
#
# 20 Hz
# A_COEFF = [1.0, -3.83582554, 5.52081914, -3.53353522, 0.848556]
# B_COEFF = [0.92117099, -3.68468397, 5.52702596, -3.68468397, 0.92117099]
#
def hipass_filter(data, A_COEFF = A_COEFF, B_COEFF = B_COEFF):
    """Filter signal on all axis with an high-pass filter."""
    if data.ndim == 2:
        # print('internal if')
        return np.vstack([hipass_filter(d, A_COEFF, B_COEFF) for d in data.T]).T

    # initial_state = signal.lfilter_zi(B_COEFF, A_COEFF) * data[0]  # padding
    if data[0] == 0:
        initial_state = [-0.936528250873, 2.809571532101, -2.809559172096, 0.936515859573]
    else:
        initial_state = [-0.936528250873 * data[0], 2.809571532101 * data[0], -2.809559172096 * data[0], 0.936515859573 * data[0]] 
    # print('initial state', initial_state)
    data_dyn, _ = signal.lfilter(B_COEFF, A_COEFF, data, zi=initial_state)
    # print('data_dyn', data_dyn)
    return data_dyn


def decompose_dyn(data, A_COEFF = A_COEFF, B_COEFF = B_COEFF):
    """ separate acceleration in low-varying and dynamic component """
    data_dyn = hipass_filter(data, A_COEFF, B_COEFF)
    return data - data_dyn, data_dyn


def colwise_dot(lhs, rhs):
    """ compute the dot product column by column"""
    return np.sum(lhs * rhs, axis=1)


def singal_resampling(DataFrame, resampling_f):
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
#########################################      LOAD DATA      ######################################### 
#######################################################################################################

def load_data(resampling_active=False, Resampling_frequency = 6667):
    # load acquired data
    Accelerometer = 'ISM330DHCX'
    classes = np.array(['Slow','Fast','Slow_dist','Fast_dist'])

    dl = hsd.Datalog('New_acquisitions/'+classes[0])
    df_ACC_S = dl.get_sensorDataFrame(Accelerometer)
    df_ACC_S = df_ACC_S[['Time','A_x [g]','A_y [g]','A_z [g]']]

    dl = hsd.Datalog('New_acquisitions/'+classes[1])
    df_ACC_F = dl.get_sensorDataFrame(Accelerometer)
    df_ACC_F = df_ACC_F[['Time','A_x [g]','A_y [g]','A_z [g]']]

    dl = hsd.Datalog('New_acquisitions/'+classes[2])
    df_ACC_SD = dl.get_sensorDataFrame(Accelerometer)
    df_ACC_SD = df_ACC_SD[['Time','A_x [g]','A_y [g]','A_z [g]']]

    dl = hsd.Datalog('New_acquisitions/'+classes[3])
    df_ACC_FD = dl.get_sensorDataFrame(Accelerometer)
    df_ACC_FD = df_ACC_FD[['Time','A_x [g]','A_y [g]','A_z [g]']]

    # SIGNAL RESAMPLING IF NEEDED  
                                
    Available_res_freq = np.array([12.5 , 26 , 52 , 104, 208, 416, 833, 1667, 3332, 6667])
    
    print('Measured ODR:', 1/(df_ACC_S.iloc[5,0]- df_ACC_S.iloc[4,0]))
    print('')

    if resampling_active == True:
        if Resampling_frequency not in Available_res_freq:
            print('########  NEW SAMPLING FREQUENCY CHOSEN NOT AVAILABLE THEN NO RESAPLING HAS BEEN APPLIED  ########')
            print('')
        else:
            df_ACC_S = singal_resampling(df_ACC_S, Resampling_frequency)
            df_ACC_F = singal_resampling(df_ACC_F, Resampling_frequency)
            df_ACC_SD = singal_resampling(df_ACC_SD, Resampling_frequency)
            df_ACC_FD = singal_resampling(df_ACC_FD, Resampling_frequency)
            

    print('ODR after resampling:', 1/(df_ACC_S.iloc[5,0]- df_ACC_S.iloc[4,0]))

    return df_ACC_S, df_ACC_F, df_ACC_SD, df_ACC_FD





#######################################################################################################
#####################################        GRAVITY FILTER       ##################################### 
#######################################################################################################

def gravity_rotation(data):
    
    A_COEFF = [1.0, -3.868656635, 5.614526749, -3.622760773, 0.8768966198]
    B_COEFF = [0.9364275932, -3.745710373, 5.618565559, -3.745710373, 0.9364275932]
    # Rotate the coordinate system in order to have z pointing in the gravity direction

    data_g, data_dyn = decompose_dyn(data, A_COEFF, B_COEFF)
    
    # Normalize gravity
    data_g = data_g / np.sqrt(colwise_dot(data_g, data_g))[:, np.newaxis]
    # print('data_g', data_g)
    # Cross product between z and g versors
    axis = np.concatenate(
        (-data_g[:, 1:2], data_g[:, 0:1], np.zeros((data.shape[0], 1))), axis=1)
    sin, cos = np.sqrt(colwise_dot(axis, axis))[:, np.newaxis], -data_g[:, 2:3]
    # print('sin', sin)
    # print('cos', cos)

    # Normalize rotation axis and handle degenerate configurations
    with np.errstate(divide='ignore', invalid='ignore'):
        axis = np.true_divide(axis, sin)
        # Set rotation to 0 if gravity aligned to z
        axis[axis == np.inf] = 0.0
        axis = np.nan_to_num(axis)

    # Rodrigues formula for rotations

    data_dyn = data_dyn * cos + np.cross(axis, data_dyn) * sin + \
          axis * colwise_dot(axis, data_dyn)[:, np.newaxis] * (1.0 - cos)
    # print( data_dyn[0,:])
    return data_dyn


#######################################################################################################
##################################        LOW PASS FILTER        ######################################
#######################################################################################################

def signal_filtering(DataFrame, stop_frequency):
    #Insert the entire Dataframe from where take data
    #Insert the sensor Type named in the Dataframe (ex. 'MIC')
    T=DataFrame.iloc[-1,0]-DataFrame.iloc[0,0]
    sampling_time=DataFrame.iloc[3,0]-DataFrame.iloc[2,0]
    sampling_f=1/sampling_time
    sos = signal.butter(3, stop_frequency,btype='low',fs=sampling_f,output='sos')
    
    DataFrame.iloc[:,1]=signal.sosfilt(sos, DataFrame.iloc[:,1])
    DataFrame.iloc[:,2]=signal.sosfilt(sos, DataFrame.iloc[:,2])
    DataFrame.iloc[:,3]=signal.sosfilt(sos, DataFrame.iloc[:,3])
    
    return DataFrame




#######################################################################################################
#################################        STANDARDIZE DATA         #####################################
#######################################################################################################

def scale_data(X, standardize):
	# remove overlap
	cut = int(X.shape[1])
	longX = X[:, -cut:, :]
	# flatten windows
	longX = longX.reshape((longX.shape[0] * longX.shape[1], longX.shape[2]))
	# flatten train and test
	flatX = X.reshape((X.shape[0] * X.shape[1], X.shape[2]))

	# standardize
	if standardize:
		s = StandardScaler()
		# fit on training data
		s.fit(longX)
		# apply to training and test data
		longX = s.transform(longX)
		flatX = s.transform(flatX)
	# reshape
	flatX = flatX.reshape((X.shape))
	return flatX



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
##########################      CNN TIME SERIES SORTING INPUT MATRIX      #############################
#######################################################################################################

def CNN_time_series_sorting_input_matrix(DataFrame,sliding_samples,n_samples):
    
    N=0
    while (len(DataFrame)-(N*sliding_samples+n_samples)) > 0:
        N=N+1
    
    X_class=np.ones((N,int(n_samples),3))
    for ii in tqdm(range(N)):
        df_acc=DataFrame.iloc[ii*sliding_samples:ii*sliding_samples+n_samples]
        
        X_support=np.array(df_acc.iloc[:,1:4])
        X_support=gravity_rotation(X_support)
        
        X_class[ii,:,0]=X_support[:,0]
        X_class[ii,:,1]=X_support[:,1]
        X_class[ii,:,2]=X_support[:,2]
        
    X_class = X_class.reshape((X_class.shape[0], X_class.shape[1],X_class.shape[2],1))

    return X_class



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

        if out_layer == True:
            base_model_non_trainable = tf.keras.models.Sequential((tf.keras.models.load_model('Saved_models/'+ model_name)).layers[1:-n_last_trainable_layers])
            input_layer = tf.keras.models.Sequential((tf.keras.models.load_model('Saved_models/'+ model_name)).layers[0])
            base_model_trainable = tf.keras.models.Sequential((tf.keras.models.load_model('Saved_models/'+ model_name)).layers[-n_last_trainable_layers:-1])
            output_layer = tf.keras.models.Sequential((tf.keras.models.load_model('Saved_models/'+ model_name)).layers[-1])
            
            input_layer.trainable = False
            base_model_non_trainable.trainable=False
            
            model = tf.keras.models.Sequential([input_layer, base_model_non_trainable, base_model_trainable, output_layer])
            model.summary()
        else:
            
            base_model_non_trainable = tf.keras.models.Sequential((tf.keras.models.load_model('Saved_models/'+ model_name)).layers[1:-n_last_trainable_layers])
            input_layer = tf.keras.models.Sequential((tf.keras.models.load_model('Saved_models/'+ model_name)).layers[0])
            base_model_trainable = tf.keras.models.Sequential((tf.keras.models.load_model('Saved_models/'+ model_name)).layers[-n_last_trainable_layers:-1])
            
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

def modelSave(model, model_name):

    save_model = input("Do you want to save this model? yes/no: ")
    
    exist  = os.path.exists('Saved_models/'+ model_name)
    if exist == True:
        if save_model == 'yes':
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
         
        
    


