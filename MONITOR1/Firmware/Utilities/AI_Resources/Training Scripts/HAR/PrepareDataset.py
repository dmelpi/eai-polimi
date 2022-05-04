#!/usr/bin/env python
# coding: utf-8 

#   This software component is licensed by ST under BSD 3-Clause license,
#   the "License"; You may not use this file except in compliance with the
#   License. You may obtain a copy of the License at:
#                        https://opensource.org/licenses/BSD-3-Clause
  

'''
Data preparations utilities for the Human Activity Recognition (HAR) demonstration
 
'''

# Import required dependencies and fix seeds for random generator for tensorflow and numpy for reproducibility
import numpy as np
np.random.seed(611)

# pandas to read and process the data tables
import pandas as pd 

# scipy for labeling the segments
from scipy import stats

# to one hot code the labels
from tensorflow.keras.utils import to_categorical

# library to generate filter for gravity rotation and preprocessing
from scipy.signal import butter, resample 

# gravity rotation dependency
import sys
sys.path.append('../../signal_processing_lib/')
from gravity_filter import gravity_rotation

# library to show plots
from matplotlib import pyplot as plt

# libraries for file handling
from os.path import join

# to use a function to check if running from jupyter notebook or not
from Utilities import *

sys.path.append('../../../Datalog/Python_SDK/')
from HSD.HSDatalog import HSDatalog

# to show the progress bar during segmenting the data
if is_notebook():
	from tqdm import tqdm_notebook as tqdm
else:
	from tqdm import tqdm

from sklearn.utils import shuffle

class DataHelper():
	def __init__( self, dataset, reducedClasses, seqLength, seqStep, preprocessing, trainTestSplit, trainValidSplit, resultDir ):
		self.dataset = dataset
		self.reducedClasses = reducedClasses
		self.seqLength = seqLength
		self.seqStep = seqStep
		self.preprocessing = preprocessing
		self.trainTestSplit = trainTestSplit
		self.trainValidSplit = trainValidSplit
		self.resultDir = resultDir
		# creating a list of activities available in two datasets
		# in case a new dataset has to be included with different classes thi
		if self.dataset == 'WISDM':
			if self.reducedClasses:
				self.classes = classes = [ 'Jogging', 'Stationary', 'Stairs', 'Walking' ]
				# self.classes = classes = [ 'Jogging', 'Stationary', 'Walking' ]
			else:
				self.classes = [ 'Downstairs', 'Jogging', 'Sitting', 'Standing', 'Upstairs', 'Walking' ]
		elif self.dataset == 'AST':
			self.classes = [ 'Stationary', 'Walking', 'Jogging', 'Biking', 'Driving' ]
			if self.reducedClasses:
				self.classes = [ 'Stationary', 'Walking', 'Jogging', 'Biking' ]
		else:
			# this will generate error as only two datasets are supported for now.
			self.classes = None


	def split_train_test_data( self, dataset ):
		# split train and test dataset with trainTestSplit proportion. (trainTestSplit out of 1 stay as train)
		datasetTrain = dataset[ 0 : int( dataset.shape[ 0 ] * self.trainTestSplit ) ]
		datasetTest = dataset[ int( dataset.shape[ 0 ] * self.trainTestSplit ) : int( dataset.shape[ 0 ] ) ]
		return datasetTrain, datasetTest

	def read_pkl_return_df( self, pklFilePath ):
		
		# initialize the script
		class_id = 0
		fileNr = 0
		myArray = [ ]

		# read pkl dataset
		Dataset = pd.read_pickle( pklFilePath )

		# list with nr files for every activity
		nrFilesPerClass = [ ]
		
		# we know there are only five activities in the dataset with labels from 0->4 so let us count nr of files for every activity
		for lbl in range( len(self.classes) ):
			nrFilesPerClass.append( Dataset[ 'act' ].count( lbl ) )
		
		# acceleration data in the dataset
		arrayData = np.array( Dataset[ 'acc' ] )

		# activity labels in dataset 
		arrayActivity = np.array( Dataset[ 'act' ] )
		
		# now let us get data for every activity one by one
		for nrFiles in nrFilesPerClass:
			# for every occurance of the label
			for i in range( fileNr, fileNr + nrFiles ):
				myArray.append( [ ] )
				for j in range( arrayData[ i ].shape[ 0 ] ):
					# for every sample in the file
					myArray[ i ].append( [ ] )
					myArray[ i ][ j ].extend( arrayData[ i ][ j ] )
					myArray[ i ][ j ].append( class_id )
			fileNr += nrFiles
			class_id += 1

		# preparing a vertical stack for the dataset 
		myArray = np.vstack( myArray[ : ] )

		# creating a dataframe to be consistent with WISDM data
		columns = [ 'x', 'y', 'z', 'Activity_Label' ]
		myDataset = pd.DataFrame( myArray, columns = columns )

		# replace activity code with activity labels to be consistent with WISDM dataset
		myDataset[ 'Activity_Label' ] = [ str( num ).replace( str( num ), self.classes[ int( num ) ] ) for num in myDataset[ 'Activity_Label' ] ]
		return myDataset		
	
	def read_dataset( self ):
		
		if self.dataset == 'WISDM':
			dataFilePath = '../../Datasets/WISDM/WISDM_raw.txt'
			# read all the data in csv 'WISDM_ar_v1.1_raw.txt' into a dataframe called dataset
			columns = ['User', 'Activity_Label', 'Arrival_Time', 'x', 'y', 'z'] # headers for the columns
			dataset = pd.read_csv( dataFilePath, header=None, names=columns )


			# removing the ; at the end of each line and casting the last variable to datatype float from string
			dataset['z'] = [float(str(char).replace(";", "")) for char in dataset['z']]

			# remove the user column as we do not need it
			dataset = dataset.drop('User', axis=1)

			# as we are workign with numbers, let us replace all the empty columns entries with NaN (not a number)
			dataset.replace(to_replace='null', value = np.NaN)

			# remove any data entry which contains NaN as a member
			dataset = dataset.dropna( axis = 0, how = 'any' )
			if self.reducedClasses:
				dataset[ 'Activity_Label' ] = [ 'Stationary' if activity == 'Standing' or activity == 'Sitting' else activity for activity in dataset[ 'Activity_Label' ] ]
				dataset[ 'Activity_Label' ] = [ 'Stairs' if activity == 'Upstairs' or activity == 'Downstairs' else activity for activity in dataset[ 'Activity_Label' ] ]

			# removing the columns for time stamp and rearranging remaining columns to match AST dataset 
			dataset = dataset[ [ 'x', 'y', 'z', 'Activity_Label' ] ]
			# dataset = dataset[dataset.Activity_Label != 'Stairs']
			print( np.unique( dataset['Activity_Label'] ) )
			datasetTrain, datasetTest = self.split_train_test_data( dataset )
			return datasetTrain, datasetTest
		
		elif self.dataset == 'AST':
			# note that the train and test data for AST are already seperate so we do not need to use split_train_test_data function
			trainDataFilePath = '../../Datasets/AST/train.pkl'
			testDataFilePath =  '../../Datasets/AST/test.pkl'
			trainDataset = self.read_pkl_return_df( trainDataFilePath )
			testDataset = self.read_pkl_return_df( testDataFilePath )
			# self.classes = [ 'Stationary', 'Walking', 'Jogging', 'Biking' ]
			# trainDataset = trainDataset[trainDataset.Activity_Label != 'Driving']
			# testDataset = testDataset[testDataset.Activity_Label != 'Driving']
			testDataset[testDataset.columns[:3]] = testDataset[testDataset.columns[:3]] * 9.8
			trainDataset[trainDataset.columns[:3]] = trainDataset[trainDataset.columns[:3]] * 9.8

			return trainDataset, testDataset 
		else:
			raise NameError('Error: Please only choose one of the two datasets, WISDM or AST')
			return null

	def preprocess_data( self, data ):
		
		# choose a sample frequency
		# if self.dataset == 'WISDM':
		# 	fSample = 20
		# 	# fSample = 26
		# else:
		# 	fSample = 26
		fSample = 26 # hardcoding to have code equivalency with C-implementation

		if self.preprocessing:
			# create a copy of data to avoid overwriting the passed dataframe
			datacopy = data.copy()
			# create highpass filter to remove dc components
			fCut = 0.4
			# fSample = 20
			filterType = 'highpass'
			fNorm = fCut / fSample
			num,den = butter( 4, fNorm, btype = filterType )
			# print('num', num)
			# print('denom', den)

			# preprocess the dataset by finding and rotating the gravity axis
			data_x = datacopy[ datacopy.columns[ : 3 ] ]
			# print( data_x.head() )
			# print('going in gravity_rotation filter ')
			datacopy[ datacopy.columns[ : 3 ] ] = gravity_rotation( np.array( data_x, dtype = float ), A_COEFF = den, B_COEFF = num )
			return datacopy
		else:
			return data

	def get_segment_indices( self, dataColumn ):

		# get segment indices to window the data into overlapping frames
		init = 0
		while init < len( dataColumn ):
			yield int( init ), int( init + self.seqLength )
			init = init + self.seqStep

	def get_data_segments( self, dataset ):

		# segmentng the data into overlaping frames
		dataIndices = dataset.index.tolist() 
		nSamples = len( dataIndices )
		segments = [ ]
		labels = np.empty( ( 0 ) )

		# need the following variable for tqdm to show the progress bar
		numSegments = int( np.floor( ( nSamples - self.seqLength ) / self.seqStep )  ) + 1 
		# print(numSegments)

		# creating segments until the get_segment_indices keep on yielding the start and end of the segments
		for (init, end) in tqdm( self.get_segment_indices( dataIndices ), unit = ' segments', desc = 'Segments built ', total = numSegments ):
			
			# check if the nr of remaing samples are enough to create a frame
			if( end < nSamples ):
				segments.append( np.transpose( [ dataset[ 'x' ].values[ init : end ],
												 dataset[ 'y' ].values[ init : end ], 
												 dataset[ 'z' ].values[ init : end ] ] ) )
				
				# use the label which occured the most in the frame
				labels = np.append( labels, stats.mode( dataset[ 'Activity_Label' ][ init : end ] )[ 0 ][ 0 ] )
		
		# converting the segments from list to numpy array
		segments = np.asarray( segments, dtype = np.float )
		return segments, labels

	def reshape_sequences( self, sequences, labels, split = False ):
		# this function reshapes the sequences into a shape which is acceptable by tensorflow
		# i.e. channel_last
		# it also enocdes the class labels into one_hot_code
		# finally it splits the data into train and validation parts using trainValidSplit 
		# proportions suplied by the user

		
		# reshaping the sequences to generate tensors with channel last configuration
		reshaped_sequences = sequences.reshape( sequences.shape[ 0 ], sequences.shape[ 1 ], sequences.shape[ 2 ], 1 )
		
		# one hot code the labels
		labels = to_categorical( [ self.classes.index( label ) for label in labels ], num_classes = len(self.classes) )
		if split:
			# splitting train and validation data
			train_test = np.random.rand( len( reshaped_sequences ) ) < self.trainValidSplit
			train_x = reshaped_sequences[ train_test ]
			train_y = labels[ train_test ]
			validation_x = reshaped_sequences[ ~train_test ]
			validation_y = labels[ ~train_test ]
			return train_x, train_y, validation_x, validation_y
		else:
			return reshaped_sequences, labels


	def prepare_data( self ):

		# this function prepares the dataset for training, validation and testing by generating
		# six tensors namely train_x, train_y, validation_x, validation_y, test_x, test_y

		# read public datasets
		datasetTrain, datasetTest = self.read_dataset( ) 
		# print(datasetTest.head())
		# print(datasetTrain.head())
		

		# preprocess train and test datasets
		datasetTrainPreprocessed = self.preprocess_data( datasetTrain )
		datasetTestPreprocessed = self.preprocess_data( datasetTest )

		# segment train and test datasets
		print( 'Segmenting Train data' )
		( sequencesTrain, labelsTrain ) = self.get_data_segments( datasetTrainPreprocessed )

		print( 'Segmenting Test data' )
		( sequencesTest, labelsTest ) = self.get_data_segments( datasetTestPreprocessed )

		print( 'Segmentation finished!' )

		# reshape sequences
		train_x, train_y, validation_x, validation_y = self.reshape_sequences( sequencesTrain, labelsTrain, split = True )
		test_x, test_y = self.reshape_sequences( sequencesTest, labelsTest )

		# shuffle the data 
		train_x, train_y = shuffle( train_x, train_y, random_state = 611 )
		test_x, test_y = shuffle( test_x, test_y, random_state = 611)
		validation_x, validation_y = shuffle( validation_x, validation_y, random_state = 611 )

		# return data tensors
		return train_x, train_y, validation_x, validation_y, test_x, test_y
	
	def prepare_dataset_for_SVM( self, magnitudes = False ):
		# this function prepares the dataset for training, validation and testing by generating
		# four tensors namely train_x, train_y, test_x, test_y

		trainX, trainy, validX, validy, testX, testy = self.prepare_data()
	
		if magnitudes:
			trainX = np.linalg.norm( trainX, axis = 2 )
			testX = np.linalg.norm( testX, axis = 2 )
			validX = np.linalg.norm( validX, axis = 2 )
		
		# vectroizing the inputs
		trainX = trainX.reshape( ( trainX.shape[ 0 ], -1 ) )
		testX = testX.reshape( ( testX.shape[ 0 ], -1 ) )
		validX = validX.reshape( ( validX.shape[ 0 ], -1 ) )

		# decoding to the class id from one-hot code
		trainy = np.argmax( trainy, axis = 1 ).astype(np.int16)
		testy = np.argmax( testy, axis = 1 ).astype(np.int16)
		validy = np.argmax( validy, axis = 1 ).astype(np.int16)
		
		testX = np.concatenate( ( testX, validX ), axis = 0 )
		testy = np.concatenate( ( testy, validy ), axis = 0 )


		return trainX, trainy, testX, testy   

	def hsdatalog_to_nn_segments( self, acqFolder, activityName, sensorName = 'ISM330DHCX', 
								sensor_type = 'ACC', offset = 100, preproc = True ):
		
		# creating a high-speed datalog object for the given acquisition folder
		hsd = HSDatalog( acquisition_folder = acqFolder )

		# reading the data as dataframe
		df = hsd.get_dataframe( sensorName, 'ACC', sample_start = offset, raw_flag = False )
		
		# dropping the 'Time' axis to exclude the time stamp
		if 'Time' in df.columns:
			df = df.drop(['Time'], axis = 1 )
		
		# converting the values from milli-gravity to m/[s^2]
		df[df.columns[:3]] = df[df.columns[:3]]  * 9.8
		# print( df.head())


		names = df.columns
		df = df.rename( columns = { names[ 0 ] : 'x', names[ 1 ] : 'y', names[ 2 ] : 'z' } )

		df['Activity_Label'] = activityName
	
		if preproc:
			df = self.preprocess_data( df )
		
		segments, labels = self.get_data_segments(df)
		segments, labels = self.reshape_sequences(segments, labels)
	
		return segments, labels

	def hsdatalog_to_ml_segments( self, acqFolder, activityName, sensorName = 'ISM330DHCX', 
								sensor_type = 'ACC', offset = 100, preproc = True ):
		
		# creating a high-speed datalog object for the given acquisition folder
		hsd = HSDatalog( acquisition_folder = acqFolder )

		# reading the data as dataframe
		df = hsd.get_dataframe( sensorName, 'ACC', sample_start = offset, raw_flag = False )
		
		# dropping the 'Time' axis to exclude the time stamp
		if 'Time' in df.columns:
			df = df.drop(['Time'], axis = 1 )
		
		# converting the values from milli-gravity to m/[s^2]
		df[df.columns[:3]] = df[df.columns[:3]]  * 9.8


		names = df.columns
		df = df.rename( columns = { names[ 0 ] : 'x', names[ 1 ] : 'y', names[ 2 ] : 'z' } )

		df['Activity_Label'] = activityName
	
		if preproc:
			df = self.preprocess_data( df )
		
		segments, labels = self.get_data_segments(df)
		segments, labels = self.reshape_sequences(segments, labels)

		segments = segments.reshape( ( segments.shape[ 0 ], -1 ) )
		labels = np.argmax( labels, axis = 1 ).astype(np.int16)
		return segments, labels

	def dump_data_for_post_validation( self, testX, testY, nrSamples ):
		# this function create data dumps from the prepared test data to later use for 
		# validating the converted code from CubeAI. This randomise the data provided and
		# save nrSamples for each of the classes with corresponding outputs into post_validation_data.npz file
		# with two variables x_test and y_test which is standard format for post validation data.
		# randomizing the order of the sequences
		testX, testY = shuffle( testX, testY )
		

		if len( testY.shape ) > 1:
			x_test = np.empty( ( 0, testX.shape[ 1 ], testX.shape[ 2 ], testX.shape[ 3 ]  ) )
			y_test = np.empty( ( 0, len( self.classes ) ) )

			# run a loop for all classes
			for i in range( testY.shape[ 1 ] ):
				# find two indices which for the current class
				inds = np.where( [ np.argmax( testY, axis = 1 ) == i ] )[ 1 ][ 0 : nrSamples ]
				# put the data into x_test and y_test variables
				x_test = np.concatenate( ( x_test, testX[ inds,: ] ), axis = 0  )
				y_test =  np.concatenate( ( y_test, testY[ inds,: ] ), axis = 0  )
		else :
			x_test = np.empty( ( 0, testX.shape[ 1 ]  ) )
			y_test = np.empty( ( 0, ) )
			# run a loop for all classes
			for i in list( np.unique( testY ) ):
				# find two indices which for the current class
				inds = np.where( [ testY == i ] )[ 1 ][ 0 : nrSamples ]
				# put the data into x_test and y_test variables
				x_test = np.concatenate( ( x_test, testX[ inds ] ), axis = 0  )
				y_test =  np.concatenate( ( y_test, testY[ inds ] ), axis = 0  )
        np.savez(self.resultDir + '/post_validation_data.npz', x_test = x_test, y_test = y_test )
	def create_neai_segments( self, trainX, trainY, nrSamplesPerClass = 50 ):
		# this function create data dumps from the prepared test data to later use for 
		# validating the converted code from CubeAI. This randomise the data provided and
		# save nrSamples for each of the classes with corresponding outputs into post_validation_data.npz file
		# with two variables x_test and y_test which is standard format for post validation data.
		# randomizing the order of the sequences
		trainX, trainY = shuffle( trainX, trainY )
		

		# run a loop for all classes
		for i in range( trainY.shape[ 1 ] ) :
			# find two indices which for the current class
			inds = np.where( [ np.argmax( trainY, axis = 1 ) == i ] )[ 1 ][ 0 : nrSamplesPerClass ]
			# put the data into x_test and y_test variables
			x =  trainX[ inds,: ]
			x = x.reshape( x.shape[ 0 ], -1 )
			y =  self.classes[ i ]
			np.savetxt( '{}/{}.csv'.format( self.resultDir, y ), x, delimiter = ',', comments = '' )