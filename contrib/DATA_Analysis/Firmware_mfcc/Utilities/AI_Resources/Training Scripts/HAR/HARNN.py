#!/usr/bin/env python
# coding: utf-8 

#   This software component is licensed by ST under BSD 3-Clause license,
#   the "License"; You may not use this file except in compliance with the
#   License. You may obtain a copy of the License at:
#                        https://opensource.org/licenses/BSD-3-Clause
  

'''
Training script of human activity recognition system (HAR), based on two different Convolutional Neural Network (CNN) architectures 


'''

# import required dependencies and fixing seeds to random number generator for numpy and tensorflow for reproducibility
import numpy as np
np.random.seed(611)

import tensorflow as tf
tf.compat.v1.set_random_seed(611)

# libraries to create and show plots
from matplotlib import pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable

# scikit learn metrics library to generate confusion matrix
from sklearn import metrics

# libraries for file handling
from os.path import isfile, join

from Utilities import *

# library to show nice progress bar for training in jupyter notebook
if is_notebook():
	from keras_tqdm import TQDMNotebookCallback

# creating a helper class for CNN building, training and evaluation
class ANNModelHandler():
	def __init__( self, modelName, classes, resultDir, inputShape, outputShape, learningRate, decayRate, nEpochs, batchSize, modelFileName, verbosity ):
		# constructor function
		self.modelName = modelName
		self.classes = classes
		self.resultDir = resultDir
		self.inputShape = inputShape
		self.outputShape = outputShape
		self.learningRate = learningRate
		self.decayRate = decayRate
		self.nEpochs = nEpochs
		self.batchSize = batchSize
		self.modelFileName = modelFileName
		self.Verbosity = verbosity
	# 
	def build_model( self ):
		# function to build the chosen CNN model
		if self.modelName == 'IGN':

			model = tf.keras.models.Sequential([ 
			tf.keras.layers.Conv2D( 24, ( 16, 1 ), input_shape = ( self.inputShape[ 1 ], self.inputShape[ 2 ], self.inputShape[ 3 ] ), activation = 'relu' ),
			tf.keras.layers.MaxPooling2D( pool_size = ( 3, 1 ) ),
			tf.keras.layers.Flatten( ),
			tf.keras.layers.Dense( 12 ),
			tf.keras.layers.Dropout( 0.5 ),
			tf.keras.layers.Dense( self.outputShape[ 1 ], activation = 'softmax' )] )
			adam = tf.keras.optimizers.Adam( lr = self.learningRate, decay = self.decayRate )
			model.compile( loss = 'categorical_crossentropy', optimizer = adam, metrics = [ 'acc' ] )
			return model

		elif self.modelName == 'GMP':

			model = tf.keras.models.Sequential([ 
			tf.keras.layers.BatchNormalization( input_shape = ( self.inputShape[ 1 ], self.inputShape[ 2 ], self.inputShape[ 3 ] ) ),
			tf.keras.layers.Conv2D( 16, ( 5, 1 ), kernel_initializer = 'glorot_uniform' ),
			tf.keras.layers.BatchNormalization( ),
			tf.keras.layers.Conv2D( 16, ( 5, 1 ), kernel_initializer = 'glorot_uniform' ),
			tf.keras.layers.GlobalMaxPooling2D( ), 
			tf.keras.layers.Dropout( 0.5 ),
			tf.keras.layers.Dense( self.outputShape[ 1 ], activation = 'softmax' ) ] )
			adam = tf.keras.optimizers.Adam( lr = self.learningRate, decay = self.decayRate )
			model.compile( loss ='categorical_crossentropy', optimizer = adam, metrics = [ 'acc' ] )
			return model

		else:
			print('Only GMP or IGN are supported as model types')
			return None

	def train_model( self, model, trainX, trainY, validationX, validationY, modelCheckPoint ):
		
		# function to train created model using provided train and validation data
		modelCheckPoint = [ modelCheckPoint ]
		vbs = self.Verbosity
		# fit the model to optimize the weights
		hist = model.fit( trainX, trainY, epochs = self.nEpochs, validation_data = ( validationX, validationY ), batch_size = self.batchSize
			, callbacks = modelCheckPoint, verbose = vbs )

		# plot accuracy evolution vs epochs
		fig = plt.figure( )
		ax = fig.add_subplot( 111 )
		ax.plot( hist.history[ 'acc' ], linewidth = 3. )
		ax.plot( hist.history[ 'val_acc' ], linewidth = 3. )
		plt.title( ' Accuracy' )
		plt.ylabel( 'Accuracy' )
		plt.xlabel( 'Epoch' )
		plt.legend( [ 'Train', 'Validation' ], loc = 'upper left' )
		plt.savefig( join( self.resultDir, self.modelFileName + '_accuracy_function.png' ) )
		plt.show( )
		
		# plot loss evolution vs epochs
		fig = plt.figure( )
		ax = fig.add_subplot( 111 )
		ax.plot( hist.history[ 'loss' ] )
		ax.plot( hist.history[ 'val_loss' ] )
		plt.title( ' Model Loss Function' )
		plt.ylabel( 'Loss function' )
		plt.xlabel( 'Epoch' )
		plt.legend( [ 'Train', 'Validation' ], loc = 'upper left' )
		plt.savefig( join( self.resultDir, self.modelFileName + '_loss_function.png' ) )
		plt.show( )

		return model

	def make_confusion_matrix( self, model, dataX, dataY ):
		'''
		This funtion creates an absolute and normalized confusion matrix for the dataX using trained model
		provided as 'model' variable and save the plotted figure as png images in result directory 
		(path specified in in resultDir ).

		'''

		# run the inferences on the test data
		predictions = model.predict( dataX, verbose = self.Verbosity )

		# getting class codes for prediction and ground truth one hot maps
		predictedClassCode = np.argmax( predictions, axis = 1 )
		groundTruthClassCode = np.argmax( dataY, axis = 1 )

		# generating confusion matrix
		cM = metrics.confusion_matrix( groundTruthClassCode, predictedClassCode )

		# normalizing the confusionMatrix for showing the probabilities instead of number of occurences
		cmNormalized = np.around( ( cM / cM.sum( axis = 1 )[ :, None ] ) * 100, 2 )
		title = 'Confusion matrix \n Nr. of Occurences and \n %age confidence'
		
		# creating a figure object
		fig, ax = plt.subplots( figsize = ( 7, 7 ) )
		
		# plotting the confusion matrix
		im = ax.imshow( cmNormalized, interpolation = 'nearest', cmap = plt.cm.Blues )

		# assiging the title, x and y labels
		plt.xlabel( 'Predicted Values' )
		plt.ylabel( 'Ground Truth' )
		plt.title( title )

		# defining the ticks for the x and y axis
		plt.xticks( range( len( self.classes ) ), self.classes, rotation = 60 )
		plt.yticks( range( len( self.classes ) ), self.classes )
		plt.ylim( bottom = len( self.classes ) - .5 )
		plt.ylim( top = -0.5 )
		
		# annotating the confusion matrix with values and printing in shall the accuracy for each class
		width, height = cM.shape 
		
		print( 'Accuracy for each class is given below.' )
		for predicted in range( width ):
			for real in range( height ):
				if cmNormalized[ predicted,real ] > 45:
					color = 'white'
				else:
				# background will be two light to have a white colored text
					color = 'black'
				if( predicted == real ):
					print( self.classes[ predicted ].ljust( 12 )+ ':', cmNormalized[ predicted,real ], '%' )
				plt.gca( ).annotate( '{}\n{}%'.format( cM[ predicted, real ], cmNormalized[ predicted, real ] ), xy = ( real, predicted ),
					horizontalalignment = 'center', verticalalignment = 'center', color = color )
		
		# showing colorbar for normalized confusion matrix case
		# creating a color bar and setting the limits
		divider = make_axes_locatable( plt.gca( ) )
		cax = divider.append_axes( "right", "5%", pad="3%" )
		plt.colorbar( im, cax = cax )

		# making sure that the figure is not clipped
		plt.tight_layout( )

		# showing plot
		plt.savefig( join( self.resultDir, self.modelFileName + '_confusion_matrix.png' ) )
		plt.show( )