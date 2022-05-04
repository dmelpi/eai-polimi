#!/usr/bin/env python
# coding: utf-8 

#   This software component is licensed by ST under BSD 3-Clause license,
#   the "License"; You may not use this file except in compliance with the
#   License. You may obtain a copy of the License at:
#                        https://opensource.org/licenses/BSD-3-Clause
  

'''
Demonstration of the Human Activity Recognition (HAR) system, based on two different Convolutional Neural Networks (CNN)
 
'''
# importing all required dependencies and fixing seeds for random number generator for numpy and tensorflow for reproducibility
import numpy as np
np.random.seed(611)

import argparse, os, logging, warnings
from os.path import isfile, join
from datetime import datetime

# private libraries
from PrepareDataset import DataHelper
from HARNN import ANNModelHandler

# for using callbacks to save the model during training and comparing the results at every epoch
from keras.callbacks import ModelCheckpoint

# disabling annoying warnings originating from Tensorflow
logging.getLogger('tensorflow').disabled = True

import tensorflow as tf
tf.compat.v1.set_random_seed(611)

# disabling annoying warnings originating from python
warnings.simplefilter("ignore")


# parsing inputs and assigning default values to the parameters which are not provided by the user
parser = argparse.ArgumentParser(description='Human Activity Recognition (HAR) in Keras with Tensorflow as backend on WISDM and WISDM + self logged datasets')
parser.add_argument('--model', default = 'IGN', type = str,
					help = 'choose one of the two availavle choices, IGN or GMP, ( default = IGN )' )
parser.add_argument('--dataset', default = 'WISDM', type = str,
					help = 'choose a dataset to use out of two choices, WISDM or AST, ( default = WISDM )')
parser.add_argument('--dataDir', default = "", type= str,
                    help='path to new data collected using STM32 IoT board recorded at 26Hz as sampling rate, (default = '')')
parser.add_argument('--seqLength', type=int, default=24, help='input sequence lenght (default:24)')
parser.add_argument('--stepSize', type=int, default=24, help='step size while creating segments (default:24, equal to seqLen)')
parser.add_argument('-m', '--merge', default=True, type=bool,      # To fix later 9 classes instead of 10 (class=0 not used)
                    help='if to merge activities (default: True)')
parser.add_argument('--preprocessing', default=True, type= bool,
                    help='gravity rotation filter application (default = True)')
parser.add_argument('--trainSplit', default=0.6, type=float,
                    help='train and test split (default = 0.6 (60 precent for train and 40 precent for test))')
parser.add_argument('--validSplit', default=0.7, type=float,
                    help='train and validation data split (default = 0.7 (70 percent for train and 30 precent for validation))')
parser.add_argument('--epochs', default=20, type=int, metavar='N',
                    help='number of total epochs to run (default: 20)')
parser.add_argument('--lr', default=0.0005, type=float,
                    help='initial learning rate')
parser.add_argument('--decay', default=1e-6 , type=float,
                    help='decay in learning rate, (default = 1e-6)' )
parser.add_argument('--batchSize', default=64, type=int,
                    metavar='N', help='mini-batch size (default: 64)')
parser.add_argument('--verbose', default=1, type=int,
                    metavar='N', help='verbosity of training and test functions in keras, 0, 1, or 2. Verbosity mode. 0 = silent, 1 = progress bar, 2 = one line per epoch (default: 1)')
parser.add_argument('--nrSamplesPostValid', default=2, type= int,
                    help='Number of samples to save from every class for post training and CubeAI conversion validation. (default = 2)')

# entry point of the script 
args = parser.parse_args()
dataset = args.dataset
merge = args.merge
modelName = args.model
segmentLength = args.seqLength
stepSize = args.stepSize
preprocessing = args.preprocessing
trainTestSplit = args.trainSplit
trainValidationSplit = args.validSplit
nEpochs = args.epochs
learningRate = args.lr
decay = args.decay
batchSize = args.batchSize
verbosity = args.verbose
dataDir = args.dataDir
nrSamplesPostValid = args.nrSamplesPostValid
if dataDir == '':
     dataDirMsg = 'Not Specified'
else:
     dataDirMsg = dataDir
print( 'Running HAR on {} dataset, with following variables\nmerge = {}\nmodelName = {}, \nsegmentLength = {}\nstepSize = {}\npreprocessing = {}\ntrainTestSplit = {}\ntrainValidationSplit = {}\nnEpochs = {}\nlearningRate = {}\ndecay ={}\nbatchSize = {}\nverbosity = {}\ndataDir = {}\nnrSamplesPostValid = {}'.format(
     dataset, merge, modelName, segmentLength, stepSize, preprocessing, trainTestSplit, trainValidationSplit, nEpochs, learningRate, decay, batchSize, verbosity, dataDirMsg, nrSamplesPostValid ) )

''' creating a directory to save results with name formated as
# Mmm_dd_yyyy_hh_mm_ss_dataset_model_seqLen_stepSize_epochs_results
# example = Oct_24_2019_14_31_20_WISDM_IGN_24_16_20_results
# with name equal to the date and time formate dd/mm/YY H:M:S
'''

# if it is the first run even the results parent directory will not exist so we need to create it.
if not os.path.exists( './results/'):
	os.mkdir( './results/' )

# creating result directory for current run.
# if not already exist create a parent directory for results.
resultDirName = 'results/{}/'.format(datetime.now().strftime( "%Y_%b_%d_%H_%M_%S" ) )
os.mkdir( resultDirName )
infoString = 'runTime : {}\nDatabase : {}\nNetwork : {}\nSeqLength : {}\nStepSize : {}\nEpochs : {}\n'.format( datetime.now().strftime("%Y-%b-%d at %H:%M:%S"), dataset, modelName, segmentLength, stepSize, nEpochs )
with open( resultDirName + 'info.txt', 'w' ) as text_file:
    text_file.write( infoString )

# Create a DataHelper object

myDataHelper = DataHelper( dataset = dataset, loggedDataDir = dataDir, merge = merge,
							 modelName = modelName, seqLength = segmentLength, seqStep = stepSize,
							 preprocessing = preprocessing, trainTestSplit = trainTestSplit, 
							 trainValidSplit = trainValidationSplit, resultDir = resultDirName )


'''
Following section prepares the dataset and create six tensors namely 
TrainX, TrainY, ValidationX, ValidationY, TestX, TestY. 
Each of the variables with trailing X are the inputs with shape 
[_, segmentLength, 3, 1 ]and each of the variables with trailing Y 
are corresponding outputs with shape [ _, NrClasses ]. 
NrClasses for WISDM can be 4 or 6 and for AST is 5.
'''
TrainX, TrainY, ValidationX, ValidationY, TestX, TestY = myDataHelper.prepare_data()

# Create a ANNModelHandler object with provided variables
myHarHandler = ANNModelHandler( modelName = modelName, classes = myDataHelper.classes, resultDir = resultDirName,
								inputShape = TrainX.shape, outputShape = TrainY.shape, learningRate = learningRate,
								decayRate = decay, nEpochs = nEpochs, batchSize = batchSize,
								modelFileName = 'har_' + modelName, verbosity = verbosity )

# Create a ANN model
harModel = myHarHandler.build_model()
harModel.summary()

# Create a Checkpoint for ANN training
harModelCheckPoint = ModelCheckpoint( filepath = join( resultDirName, 'har_' + modelName + '.h5' ),
									monitor = 'val_acc', verbose = 0, save_best_only = True, mode = 'max' )

# Train the created neural network
harModel = myHarHandler.train_model( harModel, TrainX, TrainY, ValidationX, ValidationY, harModelCheckPoint )

# Validating the trained neural network and creating a confusion matrix to have a detailed understanding
# of errors and their kinds
myHarHandler.make_confusion_matrix(  harModel, TestX, TestY )

# Create an npz file for validation after conversion from CubeAI.
myDataHelper.dump_data_for_post_validation( TestX, TestY, nrSamplesPostValid )