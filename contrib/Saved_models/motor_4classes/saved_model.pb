??
??
8
Const
output"dtype"
valuetensor"
dtypetype

NoOp
C
Placeholder
output"dtype"
dtypetype"
shapeshape:
@
ReadVariableOp
resource
value"dtype"
dtypetype?
?
StatefulPartitionedCall
args2Tin
output2Tout"
Tin
list(type)("
Tout
list(type)("	
ffunc"
configstring "
config_protostring "
executor_typestring ?
q
VarHandleOp
resource"
	containerstring "
shared_namestring "
dtypetype"
shapeshape?"serve*2.0.02unknown8??
f
	Adam/iterVarHandleOp*
shape: *
shared_name	Adam/iter*
_output_shapes
: *
dtype0	
_
Adam/iter/Read/ReadVariableOpReadVariableOp	Adam/iter*
_output_shapes
: *
dtype0	
j
Adam/beta_1VarHandleOp*
shape: *
dtype0*
shared_nameAdam/beta_1*
_output_shapes
: 
c
Adam/beta_1/Read/ReadVariableOpReadVariableOpAdam/beta_1*
dtype0*
_output_shapes
: 
j
Adam/beta_2VarHandleOp*
shared_nameAdam/beta_2*
shape: *
_output_shapes
: *
dtype0
c
Adam/beta_2/Read/ReadVariableOpReadVariableOpAdam/beta_2*
dtype0*
_output_shapes
: 
h

Adam/decayVarHandleOp*
dtype0*
_output_shapes
: *
shape: *
shared_name
Adam/decay
a
Adam/decay/Read/ReadVariableOpReadVariableOp
Adam/decay*
_output_shapes
: *
dtype0
x
Adam/learning_rateVarHandleOp*
_output_shapes
: *
dtype0*
shape: *#
shared_nameAdam/learning_rate
q
&Adam/learning_rate/Read/ReadVariableOpReadVariableOpAdam/learning_rate*
dtype0*
_output_shapes
: 
~
conv2d/kernelVarHandleOp*
dtype0*
shape:d*
shared_nameconv2d/kernel*
_output_shapes
: 
w
!conv2d/kernel/Read/ReadVariableOpReadVariableOpconv2d/kernel*
dtype0*&
_output_shapes
:d
n
conv2d/biasVarHandleOp*
shape:d*
_output_shapes
: *
shared_nameconv2d/bias*
dtype0
g
conv2d/bias/Read/ReadVariableOpReadVariableOpconv2d/bias*
_output_shapes
:d*
dtype0
u
dense/kernelVarHandleOp*
shape:	?K*
_output_shapes
: *
shared_namedense/kernel*
dtype0
n
 dense/kernel/Read/ReadVariableOpReadVariableOpdense/kernel*
_output_shapes
:	?K*
dtype0
l

dense/biasVarHandleOp*
_output_shapes
: *
shared_name
dense/bias*
shape:*
dtype0
e
dense/bias/Read/ReadVariableOpReadVariableOp
dense/bias*
dtype0*
_output_shapes
:
x
dense_1/kernelVarHandleOp*
shared_namedense_1/kernel*
shape
:*
dtype0*
_output_shapes
: 
q
"dense_1/kernel/Read/ReadVariableOpReadVariableOpdense_1/kernel*
_output_shapes

:*
dtype0
p
dense_1/biasVarHandleOp*
_output_shapes
: *
shape:*
shared_namedense_1/bias*
dtype0
i
 dense_1/bias/Read/ReadVariableOpReadVariableOpdense_1/bias*
_output_shapes
:*
dtype0
x
dense_2/kernelVarHandleOp*
shape
:*
dtype0*
shared_namedense_2/kernel*
_output_shapes
: 
q
"dense_2/kernel/Read/ReadVariableOpReadVariableOpdense_2/kernel*
dtype0*
_output_shapes

:
p
dense_2/biasVarHandleOp*
shape:*
dtype0*
_output_shapes
: *
shared_namedense_2/bias
i
 dense_2/bias/Read/ReadVariableOpReadVariableOpdense_2/bias*
_output_shapes
:*
dtype0
^
totalVarHandleOp*
dtype0*
shape: *
_output_shapes
: *
shared_nametotal
W
total/Read/ReadVariableOpReadVariableOptotal*
_output_shapes
: *
dtype0
^
countVarHandleOp*
dtype0*
_output_shapes
: *
shape: *
shared_namecount
W
count/Read/ReadVariableOpReadVariableOpcount*
dtype0*
_output_shapes
: 
?
Adam/dense_1/kernel/mVarHandleOp*
_output_shapes
: *&
shared_nameAdam/dense_1/kernel/m*
dtype0*
shape
:

)Adam/dense_1/kernel/m/Read/ReadVariableOpReadVariableOpAdam/dense_1/kernel/m*
_output_shapes

:*
dtype0
~
Adam/dense_1/bias/mVarHandleOp*
dtype0*
shape:*
_output_shapes
: *$
shared_nameAdam/dense_1/bias/m
w
'Adam/dense_1/bias/m/Read/ReadVariableOpReadVariableOpAdam/dense_1/bias/m*
dtype0*
_output_shapes
:
?
Adam/dense_2/kernel/mVarHandleOp*
_output_shapes
: *
dtype0*&
shared_nameAdam/dense_2/kernel/m*
shape
:

)Adam/dense_2/kernel/m/Read/ReadVariableOpReadVariableOpAdam/dense_2/kernel/m*
_output_shapes

:*
dtype0
~
Adam/dense_2/bias/mVarHandleOp*
dtype0*$
shared_nameAdam/dense_2/bias/m*
shape:*
_output_shapes
: 
w
'Adam/dense_2/bias/m/Read/ReadVariableOpReadVariableOpAdam/dense_2/bias/m*
dtype0*
_output_shapes
:
?
Adam/dense_1/kernel/vVarHandleOp*&
shared_nameAdam/dense_1/kernel/v*
_output_shapes
: *
dtype0*
shape
:

)Adam/dense_1/kernel/v/Read/ReadVariableOpReadVariableOpAdam/dense_1/kernel/v*
dtype0*
_output_shapes

:
~
Adam/dense_1/bias/vVarHandleOp*
_output_shapes
: *
shape:*
dtype0*$
shared_nameAdam/dense_1/bias/v
w
'Adam/dense_1/bias/v/Read/ReadVariableOpReadVariableOpAdam/dense_1/bias/v*
_output_shapes
:*
dtype0
?
Adam/dense_2/kernel/vVarHandleOp*
_output_shapes
: *
shape
:*&
shared_nameAdam/dense_2/kernel/v*
dtype0

)Adam/dense_2/kernel/v/Read/ReadVariableOpReadVariableOpAdam/dense_2/kernel/v*
_output_shapes

:*
dtype0
~
Adam/dense_2/bias/vVarHandleOp*$
shared_nameAdam/dense_2/bias/v*
shape:*
dtype0*
_output_shapes
: 
w
'Adam/dense_2/bias/v/Read/ReadVariableOpReadVariableOpAdam/dense_2/bias/v*
dtype0*
_output_shapes
:

NoOpNoOp
?7
ConstConst"/device:CPU:0*
_output_shapes
: *
dtype0*?7
value?6B?6 B?6
?
layer-0
layer_with_weights-0
layer-1
layer_with_weights-1
layer-2
layer_with_weights-2
layer-3
layer_with_weights-3
layer-4
	optimizer
regularization_losses
	variables
	trainable_variables

	keras_api

signatures
R
regularization_losses
	variables
trainable_variables
	keras_api
?
layer-0
layer_with_weights-0
layer-1
regularization_losses
	variables
trainable_variables
	keras_api
?
layer-0
layer-1
layer_with_weights-0
layer-2
regularization_losses
	variables
trainable_variables
	keras_api
?
layer_with_weights-0
layer-0
layer-1
regularization_losses
 	variables
!trainable_variables
"	keras_api
y
#layer_with_weights-0
#layer-0
$regularization_losses
%	variables
&trainable_variables
'	keras_api
?
(iter

)beta_1

*beta_2
	+decay
,learning_rate1m?2m?3m?4m?1v?2v?3v?4v?
 
8
-0
.1
/2
03
14
25
36
47

10
21
32
43
?
regularization_losses
5metrics
6non_trainable_variables
	variables
7layer_regularization_losses
	trainable_variables

8layers
 
 
 
 
?
regularization_losses
9metrics
:non_trainable_variables
	variables
;layer_regularization_losses
trainable_variables

<layers
R
=regularization_losses
>	variables
?trainable_variables
@	keras_api
h

-kernel
.bias
Aregularization_losses
B	variables
Ctrainable_variables
D	keras_api
 

-0
.1
 
?
regularization_losses
Emetrics
Fnon_trainable_variables
	variables
Glayer_regularization_losses
trainable_variables

Hlayers
R
Iregularization_losses
J	variables
Ktrainable_variables
L	keras_api
R
Mregularization_losses
N	variables
Otrainable_variables
P	keras_api
h

/kernel
0bias
Qregularization_losses
R	variables
Strainable_variables
T	keras_api
 

/0
01
 
?
regularization_losses
Umetrics
Vnon_trainable_variables
	variables
Wlayer_regularization_losses
trainable_variables

Xlayers
h

1kernel
2bias
Yregularization_losses
Z	variables
[trainable_variables
\	keras_api
R
]regularization_losses
^	variables
_trainable_variables
`	keras_api
 

10
21

10
21
?
regularization_losses
ametrics
bnon_trainable_variables
 	variables
clayer_regularization_losses
!trainable_variables

dlayers
h

3kernel
4bias
eregularization_losses
f	variables
gtrainable_variables
h	keras_api
 

30
41

30
41
?
$regularization_losses
imetrics
jnon_trainable_variables
%	variables
klayer_regularization_losses
&trainable_variables

llayers
HF
VARIABLE_VALUE	Adam/iter)optimizer/iter/.ATTRIBUTES/VARIABLE_VALUE
LJ
VARIABLE_VALUEAdam/beta_1+optimizer/beta_1/.ATTRIBUTES/VARIABLE_VALUE
LJ
VARIABLE_VALUEAdam/beta_2+optimizer/beta_2/.ATTRIBUTES/VARIABLE_VALUE
JH
VARIABLE_VALUE
Adam/decay*optimizer/decay/.ATTRIBUTES/VARIABLE_VALUE
ZX
VARIABLE_VALUEAdam/learning_rate2optimizer/learning_rate/.ATTRIBUTES/VARIABLE_VALUE
IG
VARIABLE_VALUEconv2d/kernel&variables/0/.ATTRIBUTES/VARIABLE_VALUE
GE
VARIABLE_VALUEconv2d/bias&variables/1/.ATTRIBUTES/VARIABLE_VALUE
HF
VARIABLE_VALUEdense/kernel&variables/2/.ATTRIBUTES/VARIABLE_VALUE
FD
VARIABLE_VALUE
dense/bias&variables/3/.ATTRIBUTES/VARIABLE_VALUE
JH
VARIABLE_VALUEdense_1/kernel&variables/4/.ATTRIBUTES/VARIABLE_VALUE
HF
VARIABLE_VALUEdense_1/bias&variables/5/.ATTRIBUTES/VARIABLE_VALUE
JH
VARIABLE_VALUEdense_2/kernel&variables/6/.ATTRIBUTES/VARIABLE_VALUE
HF
VARIABLE_VALUEdense_2/bias&variables/7/.ATTRIBUTES/VARIABLE_VALUE

m0

-0
.1
/2
03
 

0
1
2
3
 
 
 
 
 
 
 
?
=regularization_losses
nmetrics
onon_trainable_variables
>	variables
player_regularization_losses
?trainable_variables

qlayers
 

-0
.1
 
?
Aregularization_losses
rmetrics
snon_trainable_variables
B	variables
tlayer_regularization_losses
Ctrainable_variables

ulayers
 

-0
.1
 

0
 
 
 
?
Iregularization_losses
vmetrics
wnon_trainable_variables
J	variables
xlayer_regularization_losses
Ktrainable_variables

ylayers
 
 
 
?
Mregularization_losses
zmetrics
{non_trainable_variables
N	variables
|layer_regularization_losses
Otrainable_variables

}layers
 

/0
01
 
?
Qregularization_losses
~metrics
non_trainable_variables
R	variables
 ?layer_regularization_losses
Strainable_variables
?layers
 

/0
01
 

0
1
2
 

10
21

10
21
?
Yregularization_losses
?metrics
?non_trainable_variables
Z	variables
 ?layer_regularization_losses
[trainable_variables
?layers
 
 
 
?
]regularization_losses
?metrics
?non_trainable_variables
^	variables
 ?layer_regularization_losses
_trainable_variables
?layers
 
 
 

0
1
 

30
41

30
41
?
eregularization_losses
?metrics
?non_trainable_variables
f	variables
 ?layer_regularization_losses
gtrainable_variables
?layers
 
 
 

#0


?total

?count
?
_fn_kwargs
?regularization_losses
?	variables
?trainable_variables
?	keras_api
 
 
 
 
 

-0
.1
 
 
 
 
 
 
 
 
 
 
 

/0
01
 
 
 
 
 
 
 
 
 
 
 
 
 
 
OM
VARIABLE_VALUEtotal4keras_api/metrics/0/total/.ATTRIBUTES/VARIABLE_VALUE
OM
VARIABLE_VALUEcount4keras_api/metrics/0/count/.ATTRIBUTES/VARIABLE_VALUE
 
 

?0
?1
 
?
?regularization_losses
?metrics
?non_trainable_variables
?	variables
 ?layer_regularization_losses
?trainable_variables
?layers
 

?0
?1
 
 
mk
VARIABLE_VALUEAdam/dense_1/kernel/mBvariables/4/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUE
ki
VARIABLE_VALUEAdam/dense_1/bias/mBvariables/5/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUE
mk
VARIABLE_VALUEAdam/dense_2/kernel/mBvariables/6/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUE
ki
VARIABLE_VALUEAdam/dense_2/bias/mBvariables/7/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUE
mk
VARIABLE_VALUEAdam/dense_1/kernel/vBvariables/4/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE
ki
VARIABLE_VALUEAdam/dense_1/bias/vBvariables/5/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE
mk
VARIABLE_VALUEAdam/dense_2/kernel/vBvariables/6/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE
ki
VARIABLE_VALUEAdam/dense_2/bias/vBvariables/7/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE
?
"serving_default_sequential_2_inputPlaceholder*$
shape:?????????d*
dtype0*/
_output_shapes
:?????????d
?
StatefulPartitionedCallStatefulPartitionedCall"serving_default_sequential_2_inputconv2d/kernelconv2d/biasdense/kernel
dense/biasdense_1/kerneldense_1/biasdense_2/kerneldense_2/bias*,
f'R%
#__inference_signature_wrapper_10850*
Tin
2	*'
_output_shapes
:?????????*,
_gradient_op_typePartitionedCall-11084**
config_proto

GPU 

CPU2J 8*
Tout
2
O
saver_filenamePlaceholder*
_output_shapes
: *
dtype0*
shape: 
?
StatefulPartitionedCall_1StatefulPartitionedCallsaver_filenameAdam/iter/Read/ReadVariableOpAdam/beta_1/Read/ReadVariableOpAdam/beta_2/Read/ReadVariableOpAdam/decay/Read/ReadVariableOp&Adam/learning_rate/Read/ReadVariableOp!conv2d/kernel/Read/ReadVariableOpconv2d/bias/Read/ReadVariableOp dense/kernel/Read/ReadVariableOpdense/bias/Read/ReadVariableOp"dense_1/kernel/Read/ReadVariableOp dense_1/bias/Read/ReadVariableOp"dense_2/kernel/Read/ReadVariableOp dense_2/bias/Read/ReadVariableOptotal/Read/ReadVariableOpcount/Read/ReadVariableOp)Adam/dense_1/kernel/m/Read/ReadVariableOp'Adam/dense_1/bias/m/Read/ReadVariableOp)Adam/dense_2/kernel/m/Read/ReadVariableOp'Adam/dense_2/bias/m/Read/ReadVariableOp)Adam/dense_1/kernel/v/Read/ReadVariableOp'Adam/dense_1/bias/v/Read/ReadVariableOp)Adam/dense_2/kernel/v/Read/ReadVariableOp'Adam/dense_2/bias/v/Read/ReadVariableOpConst**
config_proto

GPU 

CPU2J 8*$
Tin
2	*
_output_shapes
: *
Tout
2*'
f"R 
__inference__traced_save_11128*,
_gradient_op_typePartitionedCall-11129
?
StatefulPartitionedCall_2StatefulPartitionedCallsaver_filename	Adam/iterAdam/beta_1Adam/beta_2
Adam/decayAdam/learning_rateconv2d/kernelconv2d/biasdense/kernel
dense/biasdense_1/kerneldense_1/biasdense_2/kerneldense_2/biastotalcountAdam/dense_1/kernel/mAdam/dense_1/bias/mAdam/dense_2/kernel/mAdam/dense_2/bias/mAdam/dense_1/kernel/vAdam/dense_1/bias/vAdam/dense_2/kernel/vAdam/dense_2/bias/v*#
Tin
2*,
_gradient_op_typePartitionedCall-11211**
f%R#
!__inference__traced_restore_11210*
Tout
2**
config_proto

GPU 

CPU2J 8*
_output_shapes
: ??
?
?
,__inference_sequential_2_layer_call_fn_10937

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*,
_gradient_op_typePartitionedCall-10493*P
fKRI
G__inference_sequential_2_layer_call_and_return_conditional_losses_10492*
Tout
2*
Tin
2**
config_proto

GPU 

CPU2J 8*/
_output_shapes
:?????????`d?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*/
_output_shapes
:?????????`d*
T0"
identityIdentity:output:0*6
_input_shapes%
#:?????????d::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
?
G__inference_sequential_2_layer_call_and_return_conditional_losses_10923

inputs)
%conv2d_statefulpartitionedcall_args_1)
%conv2d_statefulpartitionedcall_args_2
identity??conv2d/StatefulPartitionedCall?
conv2d/StatefulPartitionedCallStatefulPartitionedCallinputs%conv2d_statefulpartitionedcall_args_1%conv2d_statefulpartitionedcall_args_2*/
_output_shapes
:?????????`d*+
_gradient_op_typePartitionedCall-5838**
config_proto

GPU 

CPU2J 8*
Tout
2*0
f+R)
'__inference_restored_function_body_5837*
Tin
2?
IdentityIdentity'conv2d/StatefulPartitionedCall:output:0^conv2d/StatefulPartitionedCall*
T0*/
_output_shapes
:?????????`d"
identityIdentity:output:0*6
_input_shapes%
#:?????????d::2@
conv2d/StatefulPartitionedCallconv2d/StatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
?
G__inference_sequential_4_layer_call_and_return_conditional_losses_11020

inputs*
&dense_2_statefulpartitionedcall_args_1*
&dense_2_statefulpartitionedcall_args_2
identity??dense_2/StatefulPartitionedCall?
dense_2/StatefulPartitionedCallStatefulPartitionedCallinputs&dense_2_statefulpartitionedcall_args_1&dense_2_statefulpartitionedcall_args_2*0
f+R)
'__inference_restored_function_body_8497*
Tin
2*'
_output_shapes
:?????????**
config_proto

GPU 

CPU2J 8*+
_gradient_op_typePartitionedCall-8498*
Tout
2?
IdentityIdentity(dense_2/StatefulPartitionedCall:output:0 ^dense_2/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::2B
dense_2/StatefulPartitionedCalldense_2/StatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
?
%__inference_conv2d_layer_call_fn_4861

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*
Tin
2*
Tout
2*I
fDRB
@__inference_conv2d_layer_call_and_return_conditional_losses_4853**
config_proto

GPU 

CPU2J 8*A
_output_shapes/
-:+???????????????????????????d*+
_gradient_op_typePartitionedCall-4854?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*A
_output_shapes/
-:+???????????????????????????d*
T0"
identityIdentity:output:0*H
_input_shapes7
5:+???????????????????????????::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
]
A__inference_flatten_layer_call_and_return_conditional_losses_3460

inputs
identity^
Reshape/shapeConst*
valueB"?????%  *
dtype0*
_output_shapes
:e
ReshapeReshapeinputsReshape/shape:output:0*
T0*(
_output_shapes
:??????????KY
IdentityIdentityReshape:output:0*
T0*(
_output_shapes
:??????????K"
identityIdentity:output:0*.
_input_shapes
:????????? d:& "
 
_user_specified_nameinputs
?	
?
A__inference_dense_2_layer_call_and_return_conditional_losses_7298

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes

:*
dtype0i
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*'
_output_shapes
:?????????*
T0?
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:*
dtype0v
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????V
SoftmaxSoftmaxBiasAdd:output:0*'
_output_shapes
:?????????*
T0?
IdentityIdentitySoftmax:softmax:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp:& "
 
_user_specified_nameinputs: : 
?
?
G__inference_sequential_4_layer_call_and_return_conditional_losses_10644
input_1*
&dense_2_statefulpartitionedcall_args_1*
&dense_2_statefulpartitionedcall_args_2
identity??dense_2/StatefulPartitionedCall?
dense_2/StatefulPartitionedCallStatefulPartitionedCallinput_1&dense_2_statefulpartitionedcall_args_1&dense_2_statefulpartitionedcall_args_2*
Tout
2*
Tin
2*+
_gradient_op_typePartitionedCall-8498**
config_proto

GPU 

CPU2J 8*0
f+R)
'__inference_restored_function_body_8497*'
_output_shapes
:??????????
IdentityIdentity(dense_2/StatefulPartitionedCall:output:0 ^dense_2/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::2B
dense_2/StatefulPartitionedCalldense_2/StatefulPartitionedCall:' #
!
_user_specified_name	input_1: : 
?
i
?__forward_flatten_layer_call_and_return_conditional_losses_9121
inputs_0
identity

inputs^
Reshape/shapeConst*
dtype0*
_output_shapes
:*
valueB"?????%  g
ReshapeReshapeinputs_0Reshape/shape:output:0*(
_output_shapes
:??????????K*
T0Y
IdentityIdentityReshape:output:0*
T0*(
_output_shapes
:??????????K"
inputsinputs_0"
identityIdentity:output:0*m
backward_function_nameSQ__inference___backward_flatten_layer_call_and_return_conditional_losses_9113_9122*.
_input_shapes
:????????? d:& "
 
_user_specified_nameinputs
?
H
,__inference_max_pooling2d_layer_call_fn_3725

inputs
identity?
PartitionedCallPartitionedCallinputs*
Tin
2*
Tout
2**
config_proto

GPU 

CPU2J 8*J
_output_shapes8
6:4????????????????????????????????????*+
_gradient_op_typePartitionedCall-3720*P
fKRI
G__inference_max_pooling2d_layer_call_and_return_conditional_losses_3719?
IdentityIdentityPartitionedCall:output:0*J
_output_shapes8
6:4????????????????????????????????????*
T0"
identityIdentity:output:0*I
_input_shapes8
6:4????????????????????????????????????:& "
 
_user_specified_nameinputs
?W
?
!__inference__traced_restore_11210
file_prefix
assignvariableop_adam_iter"
assignvariableop_1_adam_beta_1"
assignvariableop_2_adam_beta_2!
assignvariableop_3_adam_decay)
%assignvariableop_4_adam_learning_rate$
 assignvariableop_5_conv2d_kernel"
assignvariableop_6_conv2d_bias#
assignvariableop_7_dense_kernel!
assignvariableop_8_dense_bias%
!assignvariableop_9_dense_1_kernel$
 assignvariableop_10_dense_1_bias&
"assignvariableop_11_dense_2_kernel$
 assignvariableop_12_dense_2_bias
assignvariableop_13_total
assignvariableop_14_count-
)assignvariableop_15_adam_dense_1_kernel_m+
'assignvariableop_16_adam_dense_1_bias_m-
)assignvariableop_17_adam_dense_2_kernel_m+
'assignvariableop_18_adam_dense_2_bias_m-
)assignvariableop_19_adam_dense_1_kernel_v+
'assignvariableop_20_adam_dense_1_bias_v-
)assignvariableop_21_adam_dense_2_kernel_v+
'assignvariableop_22_adam_dense_2_bias_v
identity_24??AssignVariableOp?AssignVariableOp_1?AssignVariableOp_10?AssignVariableOp_11?AssignVariableOp_12?AssignVariableOp_13?AssignVariableOp_14?AssignVariableOp_15?AssignVariableOp_16?AssignVariableOp_17?AssignVariableOp_18?AssignVariableOp_19?AssignVariableOp_2?AssignVariableOp_20?AssignVariableOp_21?AssignVariableOp_22?AssignVariableOp_3?AssignVariableOp_4?AssignVariableOp_5?AssignVariableOp_6?AssignVariableOp_7?AssignVariableOp_8?AssignVariableOp_9?	RestoreV2?RestoreV2_1?

RestoreV2/tensor_namesConst"/device:CPU:0*
_output_shapes
:*?	
value?	B?	B)optimizer/iter/.ATTRIBUTES/VARIABLE_VALUEB+optimizer/beta_1/.ATTRIBUTES/VARIABLE_VALUEB+optimizer/beta_2/.ATTRIBUTES/VARIABLE_VALUEB*optimizer/decay/.ATTRIBUTES/VARIABLE_VALUEB2optimizer/learning_rate/.ATTRIBUTES/VARIABLE_VALUEB&variables/0/.ATTRIBUTES/VARIABLE_VALUEB&variables/1/.ATTRIBUTES/VARIABLE_VALUEB&variables/2/.ATTRIBUTES/VARIABLE_VALUEB&variables/3/.ATTRIBUTES/VARIABLE_VALUEB&variables/4/.ATTRIBUTES/VARIABLE_VALUEB&variables/5/.ATTRIBUTES/VARIABLE_VALUEB&variables/6/.ATTRIBUTES/VARIABLE_VALUEB&variables/7/.ATTRIBUTES/VARIABLE_VALUEB4keras_api/metrics/0/total/.ATTRIBUTES/VARIABLE_VALUEB4keras_api/metrics/0/count/.ATTRIBUTES/VARIABLE_VALUEBBvariables/4/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/5/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/6/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/7/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/4/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/5/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/6/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/7/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE*
dtype0?
RestoreV2/shape_and_slicesConst"/device:CPU:0*A
value8B6B B B B B B B B B B B B B B B B B B B B B B B *
dtype0*
_output_shapes
:?
	RestoreV2	RestoreV2file_prefixRestoreV2/tensor_names:output:0#RestoreV2/shape_and_slices:output:0"/device:CPU:0*%
dtypes
2	*p
_output_shapes^
\:::::::::::::::::::::::L
IdentityIdentityRestoreV2:tensors:0*
T0	*
_output_shapes
:v
AssignVariableOpAssignVariableOpassignvariableop_adam_iterIdentity:output:0*
_output_shapes
 *
dtype0	N

Identity_1IdentityRestoreV2:tensors:1*
_output_shapes
:*
T0~
AssignVariableOp_1AssignVariableOpassignvariableop_1_adam_beta_1Identity_1:output:0*
dtype0*
_output_shapes
 N

Identity_2IdentityRestoreV2:tensors:2*
_output_shapes
:*
T0~
AssignVariableOp_2AssignVariableOpassignvariableop_2_adam_beta_2Identity_2:output:0*
dtype0*
_output_shapes
 N

Identity_3IdentityRestoreV2:tensors:3*
T0*
_output_shapes
:}
AssignVariableOp_3AssignVariableOpassignvariableop_3_adam_decayIdentity_3:output:0*
dtype0*
_output_shapes
 N

Identity_4IdentityRestoreV2:tensors:4*
_output_shapes
:*
T0?
AssignVariableOp_4AssignVariableOp%assignvariableop_4_adam_learning_rateIdentity_4:output:0*
_output_shapes
 *
dtype0N

Identity_5IdentityRestoreV2:tensors:5*
_output_shapes
:*
T0?
AssignVariableOp_5AssignVariableOp assignvariableop_5_conv2d_kernelIdentity_5:output:0*
dtype0*
_output_shapes
 N

Identity_6IdentityRestoreV2:tensors:6*
T0*
_output_shapes
:~
AssignVariableOp_6AssignVariableOpassignvariableop_6_conv2d_biasIdentity_6:output:0*
_output_shapes
 *
dtype0N

Identity_7IdentityRestoreV2:tensors:7*
T0*
_output_shapes
:
AssignVariableOp_7AssignVariableOpassignvariableop_7_dense_kernelIdentity_7:output:0*
dtype0*
_output_shapes
 N

Identity_8IdentityRestoreV2:tensors:8*
_output_shapes
:*
T0}
AssignVariableOp_8AssignVariableOpassignvariableop_8_dense_biasIdentity_8:output:0*
dtype0*
_output_shapes
 N

Identity_9IdentityRestoreV2:tensors:9*
_output_shapes
:*
T0?
AssignVariableOp_9AssignVariableOp!assignvariableop_9_dense_1_kernelIdentity_9:output:0*
dtype0*
_output_shapes
 P
Identity_10IdentityRestoreV2:tensors:10*
_output_shapes
:*
T0?
AssignVariableOp_10AssignVariableOp assignvariableop_10_dense_1_biasIdentity_10:output:0*
dtype0*
_output_shapes
 P
Identity_11IdentityRestoreV2:tensors:11*
T0*
_output_shapes
:?
AssignVariableOp_11AssignVariableOp"assignvariableop_11_dense_2_kernelIdentity_11:output:0*
dtype0*
_output_shapes
 P
Identity_12IdentityRestoreV2:tensors:12*
T0*
_output_shapes
:?
AssignVariableOp_12AssignVariableOp assignvariableop_12_dense_2_biasIdentity_12:output:0*
dtype0*
_output_shapes
 P
Identity_13IdentityRestoreV2:tensors:13*
_output_shapes
:*
T0{
AssignVariableOp_13AssignVariableOpassignvariableop_13_totalIdentity_13:output:0*
dtype0*
_output_shapes
 P
Identity_14IdentityRestoreV2:tensors:14*
T0*
_output_shapes
:{
AssignVariableOp_14AssignVariableOpassignvariableop_14_countIdentity_14:output:0*
_output_shapes
 *
dtype0P
Identity_15IdentityRestoreV2:tensors:15*
_output_shapes
:*
T0?
AssignVariableOp_15AssignVariableOp)assignvariableop_15_adam_dense_1_kernel_mIdentity_15:output:0*
dtype0*
_output_shapes
 P
Identity_16IdentityRestoreV2:tensors:16*
_output_shapes
:*
T0?
AssignVariableOp_16AssignVariableOp'assignvariableop_16_adam_dense_1_bias_mIdentity_16:output:0*
_output_shapes
 *
dtype0P
Identity_17IdentityRestoreV2:tensors:17*
T0*
_output_shapes
:?
AssignVariableOp_17AssignVariableOp)assignvariableop_17_adam_dense_2_kernel_mIdentity_17:output:0*
dtype0*
_output_shapes
 P
Identity_18IdentityRestoreV2:tensors:18*
T0*
_output_shapes
:?
AssignVariableOp_18AssignVariableOp'assignvariableop_18_adam_dense_2_bias_mIdentity_18:output:0*
_output_shapes
 *
dtype0P
Identity_19IdentityRestoreV2:tensors:19*
T0*
_output_shapes
:?
AssignVariableOp_19AssignVariableOp)assignvariableop_19_adam_dense_1_kernel_vIdentity_19:output:0*
dtype0*
_output_shapes
 P
Identity_20IdentityRestoreV2:tensors:20*
_output_shapes
:*
T0?
AssignVariableOp_20AssignVariableOp'assignvariableop_20_adam_dense_1_bias_vIdentity_20:output:0*
_output_shapes
 *
dtype0P
Identity_21IdentityRestoreV2:tensors:21*
T0*
_output_shapes
:?
AssignVariableOp_21AssignVariableOp)assignvariableop_21_adam_dense_2_kernel_vIdentity_21:output:0*
_output_shapes
 *
dtype0P
Identity_22IdentityRestoreV2:tensors:22*
T0*
_output_shapes
:?
AssignVariableOp_22AssignVariableOp'assignvariableop_22_adam_dense_2_bias_vIdentity_22:output:0*
dtype0*
_output_shapes
 ?
RestoreV2_1/tensor_namesConst"/device:CPU:0*
dtype0*
_output_shapes
:*1
value(B&B_CHECKPOINTABLE_OBJECT_GRAPHt
RestoreV2_1/shape_and_slicesConst"/device:CPU:0*
_output_shapes
:*
valueB
B *
dtype0?
RestoreV2_1	RestoreV2file_prefix!RestoreV2_1/tensor_names:output:0%RestoreV2_1/shape_and_slices:output:0
^RestoreV2"/device:CPU:0*
dtypes
2*
_output_shapes
:1
NoOpNoOp"/device:CPU:0*
_output_shapes
 ?
Identity_23Identityfile_prefix^AssignVariableOp^AssignVariableOp_1^AssignVariableOp_10^AssignVariableOp_11^AssignVariableOp_12^AssignVariableOp_13^AssignVariableOp_14^AssignVariableOp_15^AssignVariableOp_16^AssignVariableOp_17^AssignVariableOp_18^AssignVariableOp_19^AssignVariableOp_2^AssignVariableOp_20^AssignVariableOp_21^AssignVariableOp_22^AssignVariableOp_3^AssignVariableOp_4^AssignVariableOp_5^AssignVariableOp_6^AssignVariableOp_7^AssignVariableOp_8^AssignVariableOp_9^NoOp"/device:CPU:0*
T0*
_output_shapes
: ?
Identity_24IdentityIdentity_23:output:0^AssignVariableOp^AssignVariableOp_1^AssignVariableOp_10^AssignVariableOp_11^AssignVariableOp_12^AssignVariableOp_13^AssignVariableOp_14^AssignVariableOp_15^AssignVariableOp_16^AssignVariableOp_17^AssignVariableOp_18^AssignVariableOp_19^AssignVariableOp_2^AssignVariableOp_20^AssignVariableOp_21^AssignVariableOp_22^AssignVariableOp_3^AssignVariableOp_4^AssignVariableOp_5^AssignVariableOp_6^AssignVariableOp_7^AssignVariableOp_8^AssignVariableOp_9
^RestoreV2^RestoreV2_1*
_output_shapes
: *
T0"#
identity_24Identity_24:output:0*q
_input_shapes`
^: :::::::::::::::::::::::2$
AssignVariableOpAssignVariableOp2(
AssignVariableOp_1AssignVariableOp_12(
AssignVariableOp_2AssignVariableOp_22(
AssignVariableOp_3AssignVariableOp_32(
AssignVariableOp_4AssignVariableOp_42(
AssignVariableOp_5AssignVariableOp_52(
AssignVariableOp_6AssignVariableOp_62(
AssignVariableOp_7AssignVariableOp_72(
AssignVariableOp_8AssignVariableOp_82(
AssignVariableOp_9AssignVariableOp_92
	RestoreV2	RestoreV22*
AssignVariableOp_10AssignVariableOp_102*
AssignVariableOp_11AssignVariableOp_112
RestoreV2_1RestoreV2_12*
AssignVariableOp_12AssignVariableOp_122*
AssignVariableOp_13AssignVariableOp_132*
AssignVariableOp_14AssignVariableOp_142*
AssignVariableOp_20AssignVariableOp_202*
AssignVariableOp_15AssignVariableOp_152*
AssignVariableOp_21AssignVariableOp_212*
AssignVariableOp_16AssignVariableOp_162*
AssignVariableOp_17AssignVariableOp_172*
AssignVariableOp_22AssignVariableOp_222*
AssignVariableOp_18AssignVariableOp_182*
AssignVariableOp_19AssignVariableOp_19: : : : : : : :+ '
%
_user_specified_namefile_prefix: : : : : : : : :	 :
 : : : : : : 
?

?
,__inference_sequential_5_layer_call_fn_10916

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2"
statefulpartitionedcall_args_3"
statefulpartitionedcall_args_4"
statefulpartitionedcall_args_5"
statefulpartitionedcall_args_6"
statefulpartitionedcall_args_7"
statefulpartitionedcall_args_8
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2statefulpartitionedcall_args_3statefulpartitionedcall_args_4statefulpartitionedcall_args_5statefulpartitionedcall_args_6statefulpartitionedcall_args_7statefulpartitionedcall_args_8*'
_output_shapes
:?????????*
Tout
2*P
fKRI
G__inference_sequential_5_layer_call_and_return_conditional_losses_10819*,
_gradient_op_typePartitionedCall-10820**
config_proto

GPU 

CPU2J 8*
Tin
2	?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*N
_input_shapes=
;:?????????d::::::::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : : : : : : : 
?
?
,__inference_sequential_4_layer_call_fn_10660
input_1"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinput_1statefulpartitionedcall_args_1statefulpartitionedcall_args_2*,
_gradient_op_typePartitionedCall-10655*P
fKRI
G__inference_sequential_4_layer_call_and_return_conditional_losses_10654**
config_proto

GPU 

CPU2J 8*
Tout
2*
Tin
2*'
_output_shapes
:??????????
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*.
_input_shapes
:?????????::22
StatefulPartitionedCallStatefulPartitionedCall: : :' #
!
_user_specified_name	input_1
?
?
G__inference_sequential_1_layer_call_and_return_conditional_losses_10536
input_1(
$dense_statefulpartitionedcall_args_1(
$dense_statefulpartitionedcall_args_2
identity??dense/StatefulPartitionedCall?
max_pooling2d/PartitionedCallPartitionedCallinput_1*+
_gradient_op_typePartitionedCall-4562*
Tout
2**
config_proto

GPU 

CPU2J 8*0
f+R)
'__inference_restored_function_body_4561*/
_output_shapes
:????????? d*
Tin
2?
flatten/PartitionedCallPartitionedCall&max_pooling2d/PartitionedCall:output:0*0
f+R)
'__inference_restored_function_body_4570*
Tin
2**
config_proto

GPU 

CPU2J 8*+
_gradient_op_typePartitionedCall-4571*(
_output_shapes
:??????????K*
Tout
2?
dense/StatefulPartitionedCallStatefulPartitionedCall flatten/PartitionedCall:output:0$dense_statefulpartitionedcall_args_1$dense_statefulpartitionedcall_args_2*
Tin
2*'
_output_shapes
:?????????*+
_gradient_op_typePartitionedCall-4582*0
f+R)
'__inference_restored_function_body_4581**
config_proto

GPU 

CPU2J 8*
Tout
2?
IdentityIdentity&dense/StatefulPartitionedCall:output:0^dense/StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*6
_input_shapes%
#:?????????`d::2>
dense/StatefulPartitionedCalldense/StatefulPartitionedCall:' #
!
_user_specified_name	input_1: : 
?

?
,__inference_sequential_5_layer_call_fn_10903

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2"
statefulpartitionedcall_args_3"
statefulpartitionedcall_args_4"
statefulpartitionedcall_args_5"
statefulpartitionedcall_args_6"
statefulpartitionedcall_args_7"
statefulpartitionedcall_args_8
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2statefulpartitionedcall_args_3statefulpartitionedcall_args_4statefulpartitionedcall_args_5statefulpartitionedcall_args_6statefulpartitionedcall_args_7statefulpartitionedcall_args_8*
Tin
2	**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:?????????*P
fKRI
G__inference_sequential_5_layer_call_and_return_conditional_losses_10787*
Tout
2*,
_gradient_op_typePartitionedCall-10788?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*N
_input_shapes=
;:?????????d::::::::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : : : : : : : 
?
?
,__inference_sequential_4_layer_call_fn_11027

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*,
_gradient_op_typePartitionedCall-10655*
Tin
2*P
fKRI
G__inference_sequential_4_layer_call_and_return_conditional_losses_10654*'
_output_shapes
:?????????*
Tout
2**
config_proto

GPU 

CPU2J 8?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
?
'__inference_restored_function_body_4581

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*
Tin
2*+
_gradient_op_typePartitionedCall-3826*H
fCRA
?__inference_dense_layer_call_and_return_conditional_losses_3825*
Tout
2*F
_output_shapes4
2:?????????:	?K:??????????K**
config_proto

GPU 

CPU2J 8?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*/
_input_shapes
:??????????K::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
?
,__inference_sequential_1_layer_call_fn_10554
input_1"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinput_1statefulpartitionedcall_args_1statefulpartitionedcall_args_2*
Tout
2*
Tin
2*,
_gradient_op_typePartitionedCall-10549**
config_proto

GPU 

CPU2J 8*P
fKRI
G__inference_sequential_1_layer_call_and_return_conditional_losses_10548*'
_output_shapes
:??????????
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*6
_input_shapes%
#:?????????`d::22
StatefulPartitionedCallStatefulPartitionedCall:' #
!
_user_specified_name	input_1: : 
?
?
,__inference_sequential_3_layer_call_fn_10627
input_1"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinput_1statefulpartitionedcall_args_1statefulpartitionedcall_args_2*
Tin
2*,
_gradient_op_typePartitionedCall-10622*'
_output_shapes
:?????????*
Tout
2*P
fKRI
G__inference_sequential_3_layer_call_and_return_conditional_losses_10621**
config_proto

GPU 

CPU2J 8?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*.
_input_shapes
:?????????::22
StatefulPartitionedCallStatefulPartitionedCall: :' #
!
_user_specified_name	input_1: 
?
?
>__forward_conv2d_layer_call_and_return_conditional_losses_9180
inputs_0"
conv2d_readvariableop_resource#
biasadd_readvariableop_resource
identity
relu

inputs
conv2d_readvariableop??BiasAdd/ReadVariableOp?Conv2D/ReadVariableOp?
Conv2D/ReadVariableOpReadVariableOpconv2d_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*&
_output_shapes
:d?
Conv2DConv2Dinputs_0Conv2D/ReadVariableOp:value:0*A
_output_shapes/
-:+???????????????????????????d*
T0*
strides
*
paddingVALID?
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:d?
BiasAddBiasAddConv2D:output:0BiasAdd/ReadVariableOp:value:0*
T0*A
_output_shapes/
-:+???????????????????????????dj
ReluReluBiasAdd:output:0*A
_output_shapes/
-:+???????????????????????????d*
T0?
IdentityIdentityRelu:activations:0^BiasAdd/ReadVariableOp^Conv2D/ReadVariableOp*A
_output_shapes/
-:+???????????????????????????d*
T0"
identityIdentity:output:0"6
conv2d_readvariableopConv2D/ReadVariableOp:value:0"
inputsinputs_0"
reluRelu:activations:0*H
_input_shapes7
5:+???????????????????????????::*l
backward_function_nameRP__inference___backward_conv2d_layer_call_and_return_conditional_losses_9164_918120
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
Conv2D/ReadVariableOpConv2D/ReadVariableOp:& "
 
_user_specified_nameinputs: : 
?

?
,__inference_sequential_5_layer_call_fn_10831
sequential_2_input"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2"
statefulpartitionedcall_args_3"
statefulpartitionedcall_args_4"
statefulpartitionedcall_args_5"
statefulpartitionedcall_args_6"
statefulpartitionedcall_args_7"
statefulpartitionedcall_args_8
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallsequential_2_inputstatefulpartitionedcall_args_1statefulpartitionedcall_args_2statefulpartitionedcall_args_3statefulpartitionedcall_args_4statefulpartitionedcall_args_5statefulpartitionedcall_args_6statefulpartitionedcall_args_7statefulpartitionedcall_args_8**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:?????????*
Tin
2	*P
fKRI
G__inference_sequential_5_layer_call_and_return_conditional_losses_10819*,
_gradient_op_typePartitionedCall-10820*
Tout
2?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*N
_input_shapes=
;:?????????d::::::::22
StatefulPartitionedCallStatefulPartitionedCall:2 .
,
_user_specified_namesequential_2_input: : : : : : : : 
?
?
A__inference_dense_1_layer_call_and_return_conditional_losses_6030

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes

:*
dtype0i
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:??????????
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:v
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:??????????
IdentityIdentityBiasAdd:output:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp: : :& "
 
_user_specified_nameinputs
?
?
,__inference_sequential_4_layer_call_fn_11034

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*P
fKRI
G__inference_sequential_4_layer_call_and_return_conditional_losses_10671*,
_gradient_op_typePartitionedCall-10672**
config_proto

GPU 

CPU2J 8*
Tout
2*
Tin
2*'
_output_shapes
:??????????
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::22
StatefulPartitionedCallStatefulPartitionedCall: : :& "
 
_user_specified_nameinputs
?
?
,__inference_sequential_2_layer_call_fn_10515
conv2d_input"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallconv2d_inputstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*,
_gradient_op_typePartitionedCall-10510*
Tin
2**
config_proto

GPU 

CPU2J 8*P
fKRI
G__inference_sequential_2_layer_call_and_return_conditional_losses_10509*
Tout
2*/
_output_shapes
:?????????`d?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*/
_output_shapes
:?????????`d*
T0"
identityIdentity:output:0*6
_input_shapes%
#:?????????d::22
StatefulPartitionedCallStatefulPartitionedCall:, (
&
_user_specified_nameconv2d_input: : 
?#
?
G__inference_sequential_5_layer_call_and_return_conditional_losses_10871

inputs6
2sequential_2_conv2d_statefulpartitionedcall_args_16
2sequential_2_conv2d_statefulpartitionedcall_args_25
1sequential_1_dense_statefulpartitionedcall_args_15
1sequential_1_dense_statefulpartitionedcall_args_27
3sequential_3_dense_1_statefulpartitionedcall_args_17
3sequential_3_dense_1_statefulpartitionedcall_args_27
3sequential_4_dense_2_statefulpartitionedcall_args_17
3sequential_4_dense_2_statefulpartitionedcall_args_2
identity??*sequential_1/dense/StatefulPartitionedCall?+sequential_2/conv2d/StatefulPartitionedCall?,sequential_3/dense_1/StatefulPartitionedCall?,sequential_3/dropout/StatefulPartitionedCall?,sequential_4/dense_2/StatefulPartitionedCall?
+sequential_2/conv2d/StatefulPartitionedCallStatefulPartitionedCallinputs2sequential_2_conv2d_statefulpartitionedcall_args_12sequential_2_conv2d_statefulpartitionedcall_args_2*
Tout
2*+
_gradient_op_typePartitionedCall-5838*0
f+R)
'__inference_restored_function_body_5837*
Tin
2**
config_proto

GPU 

CPU2J 8*/
_output_shapes
:?????????`d?
*sequential_1/max_pooling2d/PartitionedCallPartitionedCall4sequential_2/conv2d/StatefulPartitionedCall:output:0*+
_gradient_op_typePartitionedCall-4562*
Tin
2*
Tout
2*/
_output_shapes
:????????? d*0
f+R)
'__inference_restored_function_body_4561**
config_proto

GPU 

CPU2J 8?
$sequential_1/flatten/PartitionedCallPartitionedCall3sequential_1/max_pooling2d/PartitionedCall:output:0**
config_proto

GPU 

CPU2J 8*(
_output_shapes
:??????????K*+
_gradient_op_typePartitionedCall-4571*
Tin
2*0
f+R)
'__inference_restored_function_body_4570*
Tout
2?
*sequential_1/dense/StatefulPartitionedCallStatefulPartitionedCall-sequential_1/flatten/PartitionedCall:output:01sequential_1_dense_statefulpartitionedcall_args_11sequential_1_dense_statefulpartitionedcall_args_2*'
_output_shapes
:?????????*0
f+R)
'__inference_restored_function_body_4581*
Tin
2*+
_gradient_op_typePartitionedCall-4582*
Tout
2**
config_proto

GPU 

CPU2J 8?
,sequential_3/dense_1/StatefulPartitionedCallStatefulPartitionedCall3sequential_1/dense/StatefulPartitionedCall:output:03sequential_3_dense_1_statefulpartitionedcall_args_13sequential_3_dense_1_statefulpartitionedcall_args_2*
Tin
2*'
_output_shapes
:?????????*0
f+R)
'__inference_restored_function_body_7171**
config_proto

GPU 

CPU2J 8*+
_gradient_op_typePartitionedCall-7172*
Tout
2?
,sequential_3/dropout/StatefulPartitionedCallStatefulPartitionedCall5sequential_3/dense_1/StatefulPartitionedCall:output:0*'
_output_shapes
:?????????*
Tout
2**
config_proto

GPU 

CPU2J 8*+
_gradient_op_typePartitionedCall-7186*
Tin
2*0
f+R)
'__inference_restored_function_body_7185?
,sequential_4/dense_2/StatefulPartitionedCallStatefulPartitionedCall5sequential_3/dropout/StatefulPartitionedCall:output:03sequential_4_dense_2_statefulpartitionedcall_args_13sequential_4_dense_2_statefulpartitionedcall_args_2*'
_output_shapes
:?????????*
Tout
2*
Tin
2*0
f+R)
'__inference_restored_function_body_8497**
config_proto

GPU 

CPU2J 8*+
_gradient_op_typePartitionedCall-8498?
IdentityIdentity5sequential_4/dense_2/StatefulPartitionedCall:output:0+^sequential_1/dense/StatefulPartitionedCall,^sequential_2/conv2d/StatefulPartitionedCall-^sequential_3/dense_1/StatefulPartitionedCall-^sequential_3/dropout/StatefulPartitionedCall-^sequential_4/dense_2/StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*N
_input_shapes=
;:?????????d::::::::2\
,sequential_3/dropout/StatefulPartitionedCall,sequential_3/dropout/StatefulPartitionedCall2\
,sequential_3/dense_1/StatefulPartitionedCall,sequential_3/dense_1/StatefulPartitionedCall2Z
+sequential_2/conv2d/StatefulPartitionedCall+sequential_2/conv2d/StatefulPartitionedCall2\
,sequential_4/dense_2/StatefulPartitionedCall,sequential_4/dense_2/StatefulPartitionedCall2X
*sequential_1/dense/StatefulPartitionedCall*sequential_1/dense/StatefulPartitionedCall: :& "
 
_user_specified_nameinputs: : : : : : : 
?
C
'__inference_restored_function_body_7195

inputs
identity?
PartitionedCallPartitionedCallinputs**
config_proto

GPU 

CPU2J 8*J
fERC
A__inference_dropout_layer_call_and_return_conditional_losses_6320*+
_gradient_op_typePartitionedCall-6321*
Tin
2*'
_output_shapes
:?????????*
Tout
2`
IdentityIdentityPartitionedCall:output:0*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*&
_input_shapes
:?????????:& "
 
_user_specified_nameinputs
?!
?
G__inference_sequential_5_layer_call_and_return_conditional_losses_10890

inputs6
2sequential_2_conv2d_statefulpartitionedcall_args_16
2sequential_2_conv2d_statefulpartitionedcall_args_25
1sequential_1_dense_statefulpartitionedcall_args_15
1sequential_1_dense_statefulpartitionedcall_args_27
3sequential_3_dense_1_statefulpartitionedcall_args_17
3sequential_3_dense_1_statefulpartitionedcall_args_27
3sequential_4_dense_2_statefulpartitionedcall_args_17
3sequential_4_dense_2_statefulpartitionedcall_args_2
identity??*sequential_1/dense/StatefulPartitionedCall?+sequential_2/conv2d/StatefulPartitionedCall?,sequential_3/dense_1/StatefulPartitionedCall?,sequential_4/dense_2/StatefulPartitionedCall?
+sequential_2/conv2d/StatefulPartitionedCallStatefulPartitionedCallinputs2sequential_2_conv2d_statefulpartitionedcall_args_12sequential_2_conv2d_statefulpartitionedcall_args_2*0
f+R)
'__inference_restored_function_body_5837*
Tin
2*/
_output_shapes
:?????????`d*+
_gradient_op_typePartitionedCall-5838**
config_proto

GPU 

CPU2J 8*
Tout
2?
*sequential_1/max_pooling2d/PartitionedCallPartitionedCall4sequential_2/conv2d/StatefulPartitionedCall:output:0*
Tin
2*0
f+R)
'__inference_restored_function_body_4561*/
_output_shapes
:????????? d*
Tout
2*+
_gradient_op_typePartitionedCall-4562**
config_proto

GPU 

CPU2J 8?
$sequential_1/flatten/PartitionedCallPartitionedCall3sequential_1/max_pooling2d/PartitionedCall:output:0*
Tin
2**
config_proto

GPU 

CPU2J 8*(
_output_shapes
:??????????K*+
_gradient_op_typePartitionedCall-4571*0
f+R)
'__inference_restored_function_body_4570*
Tout
2?
*sequential_1/dense/StatefulPartitionedCallStatefulPartitionedCall-sequential_1/flatten/PartitionedCall:output:01sequential_1_dense_statefulpartitionedcall_args_11sequential_1_dense_statefulpartitionedcall_args_2*+
_gradient_op_typePartitionedCall-4582*0
f+R)
'__inference_restored_function_body_4581**
config_proto

GPU 

CPU2J 8*
Tout
2*'
_output_shapes
:?????????*
Tin
2?
,sequential_3/dense_1/StatefulPartitionedCallStatefulPartitionedCall3sequential_1/dense/StatefulPartitionedCall:output:03sequential_3_dense_1_statefulpartitionedcall_args_13sequential_3_dense_1_statefulpartitionedcall_args_2*
Tin
2*+
_gradient_op_typePartitionedCall-7172*0
f+R)
'__inference_restored_function_body_7171*'
_output_shapes
:?????????*
Tout
2**
config_proto

GPU 

CPU2J 8?
$sequential_3/dropout/PartitionedCallPartitionedCall5sequential_3/dense_1/StatefulPartitionedCall:output:0*'
_output_shapes
:?????????*0
f+R)
'__inference_restored_function_body_7195*
Tout
2*
Tin
2*+
_gradient_op_typePartitionedCall-7196**
config_proto

GPU 

CPU2J 8?
,sequential_4/dense_2/StatefulPartitionedCallStatefulPartitionedCall-sequential_3/dropout/PartitionedCall:output:03sequential_4_dense_2_statefulpartitionedcall_args_13sequential_4_dense_2_statefulpartitionedcall_args_2*
Tout
2*+
_gradient_op_typePartitionedCall-8498*
Tin
2*'
_output_shapes
:?????????*0
f+R)
'__inference_restored_function_body_8497**
config_proto

GPU 

CPU2J 8?
IdentityIdentity5sequential_4/dense_2/StatefulPartitionedCall:output:0+^sequential_1/dense/StatefulPartitionedCall,^sequential_2/conv2d/StatefulPartitionedCall-^sequential_3/dense_1/StatefulPartitionedCall-^sequential_4/dense_2/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*N
_input_shapes=
;:?????????d::::::::2\
,sequential_3/dense_1/StatefulPartitionedCall,sequential_3/dense_1/StatefulPartitionedCall2Z
+sequential_2/conv2d/StatefulPartitionedCall+sequential_2/conv2d/StatefulPartitionedCall2\
,sequential_4/dense_2/StatefulPartitionedCall,sequential_4/dense_2/StatefulPartitionedCall2X
*sequential_1/dense/StatefulPartitionedCall*sequential_1/dense/StatefulPartitionedCall:& "
 
_user_specified_nameinputs: : : : : : : : 
?
?
G__inference_sequential_2_layer_call_and_return_conditional_losses_10492

inputs)
%conv2d_statefulpartitionedcall_args_1)
%conv2d_statefulpartitionedcall_args_2
identity??conv2d/StatefulPartitionedCall?
conv2d/StatefulPartitionedCallStatefulPartitionedCallinputs%conv2d_statefulpartitionedcall_args_1%conv2d_statefulpartitionedcall_args_2*+
_gradient_op_typePartitionedCall-5838*/
_output_shapes
:?????????`d*
Tout
2*
Tin
2*0
f+R)
'__inference_restored_function_body_5837**
config_proto

GPU 

CPU2J 8?
IdentityIdentity'conv2d/StatefulPartitionedCall:output:0^conv2d/StatefulPartitionedCall*
T0*/
_output_shapes
:?????????`d"
identityIdentity:output:0*6
_input_shapes%
#:?????????d::2@
conv2d/StatefulPartitionedCallconv2d/StatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
?
,__inference_sequential_3_layer_call_fn_10999

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*
Tout
2**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:?????????*,
_gradient_op_typePartitionedCall-10604*P
fKRI
G__inference_sequential_3_layer_call_and_return_conditional_losses_10603*
Tin
2?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
_
A__inference_dropout_layer_call_and_return_conditional_losses_6320

inputs

identity_1N
IdentityIdentityinputs*'
_output_shapes
:?????????*
T0[

Identity_1IdentityIdentity:output:0*
T0*'
_output_shapes
:?????????"!

identity_1Identity_1:output:0*&
_input_shapes
:?????????:& "
 
_user_specified_nameinputs
?	
?
G__inference_sequential_3_layer_call_and_return_conditional_losses_10992

inputs*
&dense_1_statefulpartitionedcall_args_1*
&dense_1_statefulpartitionedcall_args_2
identity??dense_1/StatefulPartitionedCall?
dense_1/StatefulPartitionedCallStatefulPartitionedCallinputs&dense_1_statefulpartitionedcall_args_1&dense_1_statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*
Tin
2*0
f+R)
'__inference_restored_function_body_7171*+
_gradient_op_typePartitionedCall-7172*
Tout
2*'
_output_shapes
:??????????
dropout/PartitionedCallPartitionedCall(dense_1/StatefulPartitionedCall:output:0*'
_output_shapes
:?????????*
Tin
2*0
f+R)
'__inference_restored_function_body_7195*
Tout
2**
config_proto

GPU 

CPU2J 8*+
_gradient_op_typePartitionedCall-7196?
IdentityIdentity dropout/PartitionedCall:output:0 ^dense_1/StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*.
_input_shapes
:?????????::2B
dense_1/StatefulPartitionedCalldense_1/StatefulPartitionedCall: : :& "
 
_user_specified_nameinputs
?

?
G__inference_sequential_3_layer_call_and_return_conditional_losses_10603

inputs*
&dense_1_statefulpartitionedcall_args_1*
&dense_1_statefulpartitionedcall_args_2
identity??dense_1/StatefulPartitionedCall?dropout/StatefulPartitionedCall?
dense_1/StatefulPartitionedCallStatefulPartitionedCallinputs&dense_1_statefulpartitionedcall_args_1&dense_1_statefulpartitionedcall_args_2*0
f+R)
'__inference_restored_function_body_7171**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:?????????*+
_gradient_op_typePartitionedCall-7172*
Tout
2*
Tin
2?
dropout/StatefulPartitionedCallStatefulPartitionedCall(dense_1/StatefulPartitionedCall:output:0**
config_proto

GPU 

CPU2J 8*
Tout
2*+
_gradient_op_typePartitionedCall-7186*0
f+R)
'__inference_restored_function_body_7185*
Tin
2*'
_output_shapes
:??????????
IdentityIdentity(dropout/StatefulPartitionedCall:output:0 ^dense_1/StatefulPartitionedCall ^dropout/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::2B
dense_1/StatefulPartitionedCalldense_1/StatefulPartitionedCall2B
dropout/StatefulPartitionedCalldropout/StatefulPartitionedCall: :& "
 
_user_specified_nameinputs: 
?
B
&__inference_flatten_layer_call_fn_3466

inputs
identity?
PartitionedCallPartitionedCallinputs*(
_output_shapes
:??????????K*
Tout
2*J
fERC
A__inference_flatten_layer_call_and_return_conditional_losses_3460*+
_gradient_op_typePartitionedCall-3461*
Tin
2**
config_proto

GPU 

CPU2J 8a
IdentityIdentityPartitionedCall:output:0*
T0*(
_output_shapes
:??????????K"
identityIdentity:output:0*.
_input_shapes
:????????? d:& "
 
_user_specified_nameinputs
?
_
&__inference_dropout_layer_call_fn_6509

inputs
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputs*
Tin
2*+
_gradient_op_typePartitionedCall-6504*J
fERC
A__inference_dropout_layer_call_and_return_conditional_losses_6503*'
_output_shapes
:?????????**
config_proto

GPU 

CPU2J 8*
Tout
2?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*&
_input_shapes
:?????????22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs
?
?
?__forward_dropout_layer_call_and_return_conditional_losses_9014
inputs_0
identity
dropout_mul
dropout_cast

inputs
dropout_truediv?Q
dropout/rateConst*
_output_shapes
: *
valueB
 *   ?*
dtype0E
dropout/ShapeShapeinputs_0*
_output_shapes
:*
T0_
dropout/random_uniform/minConst*
valueB
 *    *
_output_shapes
: *
dtype0_
dropout/random_uniform/maxConst*
dtype0*
_output_shapes
: *
valueB
 *  ???
$dropout/random_uniform/RandomUniformRandomUniformdropout/Shape:output:0*
dtype0*
T0*'
_output_shapes
:??????????
dropout/random_uniform/subSub#dropout/random_uniform/max:output:0#dropout/random_uniform/min:output:0*
T0*
_output_shapes
: ?
dropout/random_uniform/mulMul-dropout/random_uniform/RandomUniform:output:0dropout/random_uniform/sub:z:0*
T0*'
_output_shapes
:??????????
dropout/random_uniformAdddropout/random_uniform/mul:z:0#dropout/random_uniform/min:output:0*
T0*'
_output_shapes
:?????????R
dropout/sub/xConst*
dtype0*
valueB
 *  ??*
_output_shapes
: b
dropout/subSubdropout/sub/x:output:0dropout/rate:output:0*
T0*
_output_shapes
: V
dropout/truediv/xConst*
valueB
 *  ??*
_output_shapes
: *
dtype0h
dropout/truedivRealDivdropout/truediv/x:output:0dropout/sub:z:0*
T0*
_output_shapes
: ?
dropout/GreaterEqualGreaterEqualdropout/random_uniform:z:0dropout/rate:output:0*
T0*'
_output_shapes
:?????????c
dropout/mulMulinputs_0dropout/truediv:z:0*
T0*'
_output_shapes
:?????????o
dropout/CastCastdropout/GreaterEqual:z:0*

SrcT0
*'
_output_shapes
:?????????*

DstT0i
dropout/mul_1Muldropout/mul:z:0dropout/Cast:y:0*
T0*'
_output_shapes
:?????????Y
IdentityIdentitydropout/mul_1:z:0*
T0*'
_output_shapes
:?????????"
dropout_muldropout/mul:z:0" 
dropout_castdropout/Cast:y:0"
inputsinputs_0"&
dropout_truedivdropout/truediv:z:0"
identityIdentity:output:0*m
backward_function_nameSQ__inference___backward_dropout_layer_call_and_return_conditional_losses_8985_9015*&
_input_shapes
:?????????:& "
 
_user_specified_nameinputs
?
?
G__inference_sequential_1_layer_call_and_return_conditional_losses_10567

inputs(
$dense_statefulpartitionedcall_args_1(
$dense_statefulpartitionedcall_args_2
identity??dense/StatefulPartitionedCall?
max_pooling2d/PartitionedCallPartitionedCallinputs*0
f+R)
'__inference_restored_function_body_4561*+
_gradient_op_typePartitionedCall-4562*/
_output_shapes
:????????? d*
Tout
2**
config_proto

GPU 

CPU2J 8*
Tin
2?
flatten/PartitionedCallPartitionedCall&max_pooling2d/PartitionedCall:output:0*(
_output_shapes
:??????????K*+
_gradient_op_typePartitionedCall-4571*
Tout
2**
config_proto

GPU 

CPU2J 8*
Tin
2*0
f+R)
'__inference_restored_function_body_4570?
dense/StatefulPartitionedCallStatefulPartitionedCall flatten/PartitionedCall:output:0$dense_statefulpartitionedcall_args_1$dense_statefulpartitionedcall_args_2*
Tout
2*0
f+R)
'__inference_restored_function_body_4581*
Tin
2**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:?????????*+
_gradient_op_typePartitionedCall-4582?
IdentityIdentity&dense/StatefulPartitionedCall:output:0^dense/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*6
_input_shapes%
#:?????????`d::2>
dense/StatefulPartitionedCalldense/StatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
?
'__inference_restored_function_body_8497

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*
Tin
2*+
_gradient_op_typePartitionedCall-7602*J
fERC
A__inference_dense_2_layer_call_and_return_conditional_losses_7601*
Tout
2**
config_proto

GPU 

CPU2J 8*W
_output_shapesE
C:?????????:?????????::??????????
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?

?
,__inference_sequential_5_layer_call_fn_10799
sequential_2_input"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2"
statefulpartitionedcall_args_3"
statefulpartitionedcall_args_4"
statefulpartitionedcall_args_5"
statefulpartitionedcall_args_6"
statefulpartitionedcall_args_7"
statefulpartitionedcall_args_8
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallsequential_2_inputstatefulpartitionedcall_args_1statefulpartitionedcall_args_2statefulpartitionedcall_args_3statefulpartitionedcall_args_4statefulpartitionedcall_args_5statefulpartitionedcall_args_6statefulpartitionedcall_args_7statefulpartitionedcall_args_8**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-10788*
Tin
2	*'
_output_shapes
:?????????*P
fKRI
G__inference_sequential_5_layer_call_and_return_conditional_losses_10787*
Tout
2?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*N
_input_shapes=
;:?????????d::::::::22
StatefulPartitionedCallStatefulPartitionedCall:2 .
,
_user_specified_namesequential_2_input: : : : : : : : 
?
?
G__inference_sequential_5_layer_call_and_return_conditional_losses_10750
sequential_2_input/
+sequential_2_statefulpartitionedcall_args_1/
+sequential_2_statefulpartitionedcall_args_2/
+sequential_1_statefulpartitionedcall_args_1/
+sequential_1_statefulpartitionedcall_args_2/
+sequential_3_statefulpartitionedcall_args_1/
+sequential_3_statefulpartitionedcall_args_2/
+sequential_4_statefulpartitionedcall_args_1/
+sequential_4_statefulpartitionedcall_args_2
identity??$sequential_1/StatefulPartitionedCall?$sequential_2/StatefulPartitionedCall?$sequential_3/StatefulPartitionedCall?$sequential_4/StatefulPartitionedCall?
$sequential_2/StatefulPartitionedCallStatefulPartitionedCallsequential_2_input+sequential_2_statefulpartitionedcall_args_1+sequential_2_statefulpartitionedcall_args_2*
Tout
2*/
_output_shapes
:?????????`d**
config_proto

GPU 

CPU2J 8*P
fKRI
G__inference_sequential_2_layer_call_and_return_conditional_losses_10492*,
_gradient_op_typePartitionedCall-10493*
Tin
2?
$sequential_1/StatefulPartitionedCallStatefulPartitionedCall-sequential_2/StatefulPartitionedCall:output:0+sequential_1_statefulpartitionedcall_args_1+sequential_1_statefulpartitionedcall_args_2*
Tout
2*,
_gradient_op_typePartitionedCall-10549**
config_proto

GPU 

CPU2J 8*
Tin
2*P
fKRI
G__inference_sequential_1_layer_call_and_return_conditional_losses_10548*'
_output_shapes
:??????????
$sequential_3/StatefulPartitionedCallStatefulPartitionedCall-sequential_1/StatefulPartitionedCall:output:0+sequential_3_statefulpartitionedcall_args_1+sequential_3_statefulpartitionedcall_args_2*,
_gradient_op_typePartitionedCall-10604**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:?????????*
Tout
2*P
fKRI
G__inference_sequential_3_layer_call_and_return_conditional_losses_10603*
Tin
2?
$sequential_4/StatefulPartitionedCallStatefulPartitionedCall-sequential_3/StatefulPartitionedCall:output:0+sequential_4_statefulpartitionedcall_args_1+sequential_4_statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*P
fKRI
G__inference_sequential_4_layer_call_and_return_conditional_losses_10654*
Tin
2*'
_output_shapes
:?????????*
Tout
2*,
_gradient_op_typePartitionedCall-10655?
IdentityIdentity-sequential_4/StatefulPartitionedCall:output:0%^sequential_1/StatefulPartitionedCall%^sequential_2/StatefulPartitionedCall%^sequential_3/StatefulPartitionedCall%^sequential_4/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*N
_input_shapes=
;:?????????d::::::::2L
$sequential_2/StatefulPartitionedCall$sequential_2/StatefulPartitionedCall2L
$sequential_3/StatefulPartitionedCall$sequential_3/StatefulPartitionedCall2L
$sequential_4/StatefulPartitionedCall$sequential_4/StatefulPartitionedCall2L
$sequential_1/StatefulPartitionedCall$sequential_1/StatefulPartitionedCall:2 .
,
_user_specified_namesequential_2_input: : : : : : : : 
?0
?	
__inference__traced_save_11128
file_prefix(
$savev2_adam_iter_read_readvariableop	*
&savev2_adam_beta_1_read_readvariableop*
&savev2_adam_beta_2_read_readvariableop)
%savev2_adam_decay_read_readvariableop1
-savev2_adam_learning_rate_read_readvariableop,
(savev2_conv2d_kernel_read_readvariableop*
&savev2_conv2d_bias_read_readvariableop+
'savev2_dense_kernel_read_readvariableop)
%savev2_dense_bias_read_readvariableop-
)savev2_dense_1_kernel_read_readvariableop+
'savev2_dense_1_bias_read_readvariableop-
)savev2_dense_2_kernel_read_readvariableop+
'savev2_dense_2_bias_read_readvariableop$
 savev2_total_read_readvariableop$
 savev2_count_read_readvariableop4
0savev2_adam_dense_1_kernel_m_read_readvariableop2
.savev2_adam_dense_1_bias_m_read_readvariableop4
0savev2_adam_dense_2_kernel_m_read_readvariableop2
.savev2_adam_dense_2_bias_m_read_readvariableop4
0savev2_adam_dense_1_kernel_v_read_readvariableop2
.savev2_adam_dense_1_bias_v_read_readvariableop4
0savev2_adam_dense_2_kernel_v_read_readvariableop2
.savev2_adam_dense_2_bias_v_read_readvariableop
savev2_1_const

identity_1??MergeV2Checkpoints?SaveV2?SaveV2_1?
StringJoin/inputs_1Const"/device:CPU:0*<
value3B1 B+_temp_f236f4dd1fbe4a5f9f69df5205eb5404/part*
dtype0*
_output_shapes
: s

StringJoin
StringJoinfile_prefixStringJoin/inputs_1:output:0"/device:CPU:0*
N*
_output_shapes
: L

num_shardsConst*
_output_shapes
: *
value	B :*
dtype0f
ShardedFilename/shardConst"/device:CPU:0*
dtype0*
value	B : *
_output_shapes
: ?
ShardedFilenameShardedFilenameStringJoin:output:0ShardedFilename/shard:output:0num_shards:output:0"/device:CPU:0*
_output_shapes
: ?

SaveV2/tensor_namesConst"/device:CPU:0*
dtype0*
_output_shapes
:*?	
value?	B?	B)optimizer/iter/.ATTRIBUTES/VARIABLE_VALUEB+optimizer/beta_1/.ATTRIBUTES/VARIABLE_VALUEB+optimizer/beta_2/.ATTRIBUTES/VARIABLE_VALUEB*optimizer/decay/.ATTRIBUTES/VARIABLE_VALUEB2optimizer/learning_rate/.ATTRIBUTES/VARIABLE_VALUEB&variables/0/.ATTRIBUTES/VARIABLE_VALUEB&variables/1/.ATTRIBUTES/VARIABLE_VALUEB&variables/2/.ATTRIBUTES/VARIABLE_VALUEB&variables/3/.ATTRIBUTES/VARIABLE_VALUEB&variables/4/.ATTRIBUTES/VARIABLE_VALUEB&variables/5/.ATTRIBUTES/VARIABLE_VALUEB&variables/6/.ATTRIBUTES/VARIABLE_VALUEB&variables/7/.ATTRIBUTES/VARIABLE_VALUEB4keras_api/metrics/0/total/.ATTRIBUTES/VARIABLE_VALUEB4keras_api/metrics/0/count/.ATTRIBUTES/VARIABLE_VALUEBBvariables/4/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/5/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/6/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/7/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/4/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/5/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/6/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/7/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE?
SaveV2/shape_and_slicesConst"/device:CPU:0*
_output_shapes
:*A
value8B6B B B B B B B B B B B B B B B B B B B B B B B *
dtype0?	
SaveV2SaveV2ShardedFilename:filename:0SaveV2/tensor_names:output:0 SaveV2/shape_and_slices:output:0$savev2_adam_iter_read_readvariableop&savev2_adam_beta_1_read_readvariableop&savev2_adam_beta_2_read_readvariableop%savev2_adam_decay_read_readvariableop-savev2_adam_learning_rate_read_readvariableop(savev2_conv2d_kernel_read_readvariableop&savev2_conv2d_bias_read_readvariableop'savev2_dense_kernel_read_readvariableop%savev2_dense_bias_read_readvariableop)savev2_dense_1_kernel_read_readvariableop'savev2_dense_1_bias_read_readvariableop)savev2_dense_2_kernel_read_readvariableop'savev2_dense_2_bias_read_readvariableop savev2_total_read_readvariableop savev2_count_read_readvariableop0savev2_adam_dense_1_kernel_m_read_readvariableop.savev2_adam_dense_1_bias_m_read_readvariableop0savev2_adam_dense_2_kernel_m_read_readvariableop.savev2_adam_dense_2_bias_m_read_readvariableop0savev2_adam_dense_1_kernel_v_read_readvariableop.savev2_adam_dense_1_bias_v_read_readvariableop0savev2_adam_dense_2_kernel_v_read_readvariableop.savev2_adam_dense_2_bias_v_read_readvariableop"/device:CPU:0*
_output_shapes
 *%
dtypes
2	h
ShardedFilename_1/shardConst"/device:CPU:0*
value	B :*
_output_shapes
: *
dtype0?
ShardedFilename_1ShardedFilenameStringJoin:output:0 ShardedFilename_1/shard:output:0num_shards:output:0"/device:CPU:0*
_output_shapes
: ?
SaveV2_1/tensor_namesConst"/device:CPU:0*
_output_shapes
:*1
value(B&B_CHECKPOINTABLE_OBJECT_GRAPH*
dtype0q
SaveV2_1/shape_and_slicesConst"/device:CPU:0*
dtype0*
valueB
B *
_output_shapes
:?
SaveV2_1SaveV2ShardedFilename_1:filename:0SaveV2_1/tensor_names:output:0"SaveV2_1/shape_and_slices:output:0savev2_1_const^SaveV2"/device:CPU:0*
_output_shapes
 *
dtypes
2?
&MergeV2Checkpoints/checkpoint_prefixesPackShardedFilename:filename:0ShardedFilename_1:filename:0^SaveV2	^SaveV2_1"/device:CPU:0*
T0*
N*
_output_shapes
:?
MergeV2CheckpointsMergeV2Checkpoints/MergeV2Checkpoints/checkpoint_prefixes:output:0file_prefix	^SaveV2_1"/device:CPU:0*
_output_shapes
 f
IdentityIdentityfile_prefix^MergeV2Checkpoints"/device:CPU:0*
_output_shapes
: *
T0s

Identity_1IdentityIdentity:output:0^MergeV2Checkpoints^SaveV2	^SaveV2_1*
_output_shapes
: *
T0"!

identity_1Identity_1:output:0*?
_input_shapes?
?: : : : : : :d:d:	?K:::::: : ::::::::: 2(
MergeV2CheckpointsMergeV2Checkpoints2
SaveV2SaveV22
SaveV2_1SaveV2_1:+ '
%
_user_specified_namefile_prefix: : : : : : : : :	 :
 : : : : : : : : : : : : : : 
?
?
@__inference_conv2d_layer_call_and_return_conditional_losses_4853

inputs"
conv2d_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?Conv2D/ReadVariableOp?
Conv2D/ReadVariableOpReadVariableOpconv2d_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*&
_output_shapes
:d*
dtype0?
Conv2DConv2DinputsConv2D/ReadVariableOp:value:0*
paddingVALID*A
_output_shapes/
-:+???????????????????????????d*
T0*
strides
?
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:d*
dtype0?
BiasAddBiasAddConv2D:output:0BiasAdd/ReadVariableOp:value:0*
T0*A
_output_shapes/
-:+???????????????????????????dj
ReluReluBiasAdd:output:0*A
_output_shapes/
-:+???????????????????????????d*
T0?
IdentityIdentityRelu:activations:0^BiasAdd/ReadVariableOp^Conv2D/ReadVariableOp*A
_output_shapes/
-:+???????????????????????????d*
T0"
identityIdentity:output:0*H
_input_shapes7
5:+???????????????????????????::20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
Conv2D/ReadVariableOpConv2D/ReadVariableOp:& "
 
_user_specified_nameinputs: : 
?
?
,__inference_sequential_2_layer_call_fn_10498
conv2d_input"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallconv2d_inputstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*
Tin
2*P
fKRI
G__inference_sequential_2_layer_call_and_return_conditional_losses_10492*/
_output_shapes
:?????????`d**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-10493*
Tout
2?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*/
_output_shapes
:?????????`d"
identityIdentity:output:0*6
_input_shapes%
#:?????????d::22
StatefulPartitionedCallStatefulPartitionedCall:, (
&
_user_specified_nameconv2d_input: : 
?
?
G__inference_sequential_1_layer_call_and_return_conditional_losses_10962

inputs(
$dense_statefulpartitionedcall_args_1(
$dense_statefulpartitionedcall_args_2
identity??dense/StatefulPartitionedCall?
max_pooling2d/PartitionedCallPartitionedCallinputs*
Tin
2**
config_proto

GPU 

CPU2J 8*
Tout
2*+
_gradient_op_typePartitionedCall-4562*0
f+R)
'__inference_restored_function_body_4561*/
_output_shapes
:????????? d?
flatten/PartitionedCallPartitionedCall&max_pooling2d/PartitionedCall:output:0*+
_gradient_op_typePartitionedCall-4571*
Tin
2*0
f+R)
'__inference_restored_function_body_4570**
config_proto

GPU 

CPU2J 8*(
_output_shapes
:??????????K*
Tout
2?
dense/StatefulPartitionedCallStatefulPartitionedCall flatten/PartitionedCall:output:0$dense_statefulpartitionedcall_args_1$dense_statefulpartitionedcall_args_2*
Tout
2*
Tin
2*0
f+R)
'__inference_restored_function_body_4581*+
_gradient_op_typePartitionedCall-4582**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:??????????
IdentityIdentity&dense/StatefulPartitionedCall:output:0^dense/StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*6
_input_shapes%
#:?????????`d::2>
dense/StatefulPartitionedCalldense/StatefulPartitionedCall: :& "
 
_user_specified_nameinputs: 
?
?
G__inference_sequential_2_layer_call_and_return_conditional_losses_10930

inputs)
%conv2d_statefulpartitionedcall_args_1)
%conv2d_statefulpartitionedcall_args_2
identity??conv2d/StatefulPartitionedCall?
conv2d/StatefulPartitionedCallStatefulPartitionedCallinputs%conv2d_statefulpartitionedcall_args_1%conv2d_statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*/
_output_shapes
:?????????`d*
Tout
2*0
f+R)
'__inference_restored_function_body_5837*
Tin
2*+
_gradient_op_typePartitionedCall-5838?
IdentityIdentity'conv2d/StatefulPartitionedCall:output:0^conv2d/StatefulPartitionedCall*/
_output_shapes
:?????????`d*
T0"
identityIdentity:output:0*6
_input_shapes%
#:?????????d::2@
conv2d/StatefulPartitionedCallconv2d/StatefulPartitionedCall: : :& "
 
_user_specified_nameinputs
?
?
G__inference_sequential_2_layer_call_and_return_conditional_losses_10473
conv2d_input)
%conv2d_statefulpartitionedcall_args_1)
%conv2d_statefulpartitionedcall_args_2
identity??conv2d/StatefulPartitionedCall?
conv2d/StatefulPartitionedCallStatefulPartitionedCallconv2d_input%conv2d_statefulpartitionedcall_args_1%conv2d_statefulpartitionedcall_args_2*
Tout
2*/
_output_shapes
:?????????`d*+
_gradient_op_typePartitionedCall-5838*0
f+R)
'__inference_restored_function_body_5837**
config_proto

GPU 

CPU2J 8*
Tin
2?
IdentityIdentity'conv2d/StatefulPartitionedCall:output:0^conv2d/StatefulPartitionedCall*/
_output_shapes
:?????????`d*
T0"
identityIdentity:output:0*6
_input_shapes%
#:?????????d::2@
conv2d/StatefulPartitionedCallconv2d/StatefulPartitionedCall:, (
&
_user_specified_nameconv2d_input: : 
?
`
A__inference_dropout_layer_call_and_return_conditional_losses_6391

inputs
identity?Q
dropout/rateConst*
dtype0*
_output_shapes
: *
valueB
 *   ?C
dropout/ShapeShapeinputs*
_output_shapes
:*
T0_
dropout/random_uniform/minConst*
valueB
 *    *
dtype0*
_output_shapes
: _
dropout/random_uniform/maxConst*
_output_shapes
: *
valueB
 *  ??*
dtype0?
$dropout/random_uniform/RandomUniformRandomUniformdropout/Shape:output:0*
T0*
dtype0*'
_output_shapes
:??????????
dropout/random_uniform/subSub#dropout/random_uniform/max:output:0#dropout/random_uniform/min:output:0*
_output_shapes
: *
T0?
dropout/random_uniform/mulMul-dropout/random_uniform/RandomUniform:output:0dropout/random_uniform/sub:z:0*
T0*'
_output_shapes
:??????????
dropout/random_uniformAdddropout/random_uniform/mul:z:0#dropout/random_uniform/min:output:0*'
_output_shapes
:?????????*
T0R
dropout/sub/xConst*
dtype0*
_output_shapes
: *
valueB
 *  ??b
dropout/subSubdropout/sub/x:output:0dropout/rate:output:0*
T0*
_output_shapes
: V
dropout/truediv/xConst*
_output_shapes
: *
valueB
 *  ??*
dtype0h
dropout/truedivRealDivdropout/truediv/x:output:0dropout/sub:z:0*
T0*
_output_shapes
: ?
dropout/GreaterEqualGreaterEqualdropout/random_uniform:z:0dropout/rate:output:0*'
_output_shapes
:?????????*
T0a
dropout/mulMulinputsdropout/truediv:z:0*'
_output_shapes
:?????????*
T0o
dropout/CastCastdropout/GreaterEqual:z:0*'
_output_shapes
:?????????*

SrcT0
*

DstT0i
dropout/mul_1Muldropout/mul:z:0dropout/Cast:y:0*'
_output_shapes
:?????????*
T0Y
IdentityIdentitydropout/mul_1:z:0*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*&
_input_shapes
:?????????:& "
 
_user_specified_nameinputs
?
C
'__inference_restored_function_body_4561

inputs
identity?
PartitionedCallPartitionedCallinputs**
config_proto

GPU 

CPU2J 8*+
_gradient_op_typePartitionedCall-3720*
Tout
2*
Tin
2*?
_output_shapes?
?:4????????????????????????????????????:4????????????????????????????????????:4????????????????????????????????????*P
fKRI
G__inference_max_pooling2d_layer_call_and_return_conditional_losses_3719?
IdentityIdentityPartitionedCall:output:0*
T0*J
_output_shapes8
6:4????????????????????????????????????"
identityIdentity:output:0*I
_input_shapes8
6:4????????????????????????????????????:& "
 
_user_specified_nameinputs
?
?
,__inference_sequential_3_layer_call_fn_10609
input_1"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinput_1statefulpartitionedcall_args_1statefulpartitionedcall_args_2*'
_output_shapes
:?????????**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-10604*
Tout
2*P
fKRI
G__inference_sequential_3_layer_call_and_return_conditional_losses_10603*
Tin
2?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*.
_input_shapes
:?????????::22
StatefulPartitionedCallStatefulPartitionedCall: : :' #
!
_user_specified_name	input_1
?
?
G__inference_sequential_1_layer_call_and_return_conditional_losses_10525
input_1(
$dense_statefulpartitionedcall_args_1(
$dense_statefulpartitionedcall_args_2
identity??dense/StatefulPartitionedCall?
max_pooling2d/PartitionedCallPartitionedCallinput_1*0
f+R)
'__inference_restored_function_body_4561**
config_proto

GPU 

CPU2J 8*
Tout
2*+
_gradient_op_typePartitionedCall-4562*
Tin
2*/
_output_shapes
:????????? d?
flatten/PartitionedCallPartitionedCall&max_pooling2d/PartitionedCall:output:0*0
f+R)
'__inference_restored_function_body_4570*
Tin
2*
Tout
2*(
_output_shapes
:??????????K*+
_gradient_op_typePartitionedCall-4571**
config_proto

GPU 

CPU2J 8?
dense/StatefulPartitionedCallStatefulPartitionedCall flatten/PartitionedCall:output:0$dense_statefulpartitionedcall_args_1$dense_statefulpartitionedcall_args_2*
Tin
2**
config_proto

GPU 

CPU2J 8*0
f+R)
'__inference_restored_function_body_4581*
Tout
2*'
_output_shapes
:?????????*+
_gradient_op_typePartitionedCall-4582?
IdentityIdentity&dense/StatefulPartitionedCall:output:0^dense/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*6
_input_shapes%
#:?????????`d::2>
dense/StatefulPartitionedCalldense/StatefulPartitionedCall: : :' #
!
_user_specified_name	input_1
?
?
,__inference_sequential_3_layer_call_fn_11006

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*
Tin
2*P
fKRI
G__inference_sequential_3_layer_call_and_return_conditional_losses_10621*
Tout
2*'
_output_shapes
:?????????**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-10622?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
?
'__inference_restored_function_body_5837

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*I
fDRB
@__inference_conv2d_layer_call_and_return_conditional_losses_4853*
Tin
2*?
_output_shapes?
?:+???????????????????????????d:+???????????????????????????d:+???????????????????????????:d*
Tout
2*+
_gradient_op_typePartitionedCall-4854**
config_proto

GPU 

CPU2J 8?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*A
_output_shapes/
-:+???????????????????????????d*
T0"
identityIdentity:output:0*H
_input_shapes7
5:+???????????????????????????::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
?
G__inference_sequential_4_layer_call_and_return_conditional_losses_10671

inputs*
&dense_2_statefulpartitionedcall_args_1*
&dense_2_statefulpartitionedcall_args_2
identity??dense_2/StatefulPartitionedCall?
dense_2/StatefulPartitionedCallStatefulPartitionedCallinputs&dense_2_statefulpartitionedcall_args_1&dense_2_statefulpartitionedcall_args_2*+
_gradient_op_typePartitionedCall-8498**
config_proto

GPU 

CPU2J 8*
Tout
2*0
f+R)
'__inference_restored_function_body_8497*
Tin
2*'
_output_shapes
:??????????
IdentityIdentity(dense_2/StatefulPartitionedCall:output:0 ^dense_2/StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*.
_input_shapes
:?????????::2B
dense_2/StatefulPartitionedCalldense_2/StatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?	
?
G__inference_sequential_3_layer_call_and_return_conditional_losses_10592
input_1*
&dense_1_statefulpartitionedcall_args_1*
&dense_1_statefulpartitionedcall_args_2
identity??dense_1/StatefulPartitionedCall?
dense_1/StatefulPartitionedCallStatefulPartitionedCallinput_1&dense_1_statefulpartitionedcall_args_1&dense_1_statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:?????????*0
f+R)
'__inference_restored_function_body_7171*
Tout
2*+
_gradient_op_typePartitionedCall-7172*
Tin
2?
dropout/PartitionedCallPartitionedCall(dense_1/StatefulPartitionedCall:output:0*+
_gradient_op_typePartitionedCall-7196*
Tin
2*0
f+R)
'__inference_restored_function_body_7195*'
_output_shapes
:?????????*
Tout
2**
config_proto

GPU 

CPU2J 8?
IdentityIdentity dropout/PartitionedCall:output:0 ^dense_1/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::2B
dense_1/StatefulPartitionedCalldense_1/StatefulPartitionedCall:' #
!
_user_specified_name	input_1: : 
?

?
=__forward_dense_layer_call_and_return_conditional_losses_9088
inputs_0"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity
matmul_readvariableop

inputs??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:	?K*
dtype0k
MatMulMatMulinputs_0MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:??????????
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:v
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:??????????
IdentityIdentityBiasAdd:output:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*
T0*'
_output_shapes
:?????????"6
matmul_readvariableopMatMul/ReadVariableOp:value:0"
inputsinputs_0"
identityIdentity:output:0*k
backward_function_nameQO__inference___backward_dense_layer_call_and_return_conditional_losses_9076_9089*/
_input_shapes
:??????????K::20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp: : :& "
 
_user_specified_nameinputs
?
?
?__inference_dense_layer_call_and_return_conditional_losses_3825

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:	?K*
dtype0i
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:??????????
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:v
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:??????????
IdentityIdentityBiasAdd:output:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*/
_input_shapes
:??????????K::20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp:& "
 
_user_specified_nameinputs: : 
?
?
G__inference_sequential_4_layer_call_and_return_conditional_losses_10654

inputs*
&dense_2_statefulpartitionedcall_args_1*
&dense_2_statefulpartitionedcall_args_2
identity??dense_2/StatefulPartitionedCall?
dense_2/StatefulPartitionedCallStatefulPartitionedCallinputs&dense_2_statefulpartitionedcall_args_1&dense_2_statefulpartitionedcall_args_2*
Tin
2*'
_output_shapes
:?????????*+
_gradient_op_typePartitionedCall-8498*0
f+R)
'__inference_restored_function_body_8497*
Tout
2**
config_proto

GPU 

CPU2J 8?
IdentityIdentity(dense_2/StatefulPartitionedCall:output:0 ^dense_2/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::2B
dense_2/StatefulPartitionedCalldense_2/StatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?&
?
 __inference__wrapped_model_10463
sequential_2_inputC
?sequential_5_sequential_2_conv2d_statefulpartitionedcall_args_1C
?sequential_5_sequential_2_conv2d_statefulpartitionedcall_args_2B
>sequential_5_sequential_1_dense_statefulpartitionedcall_args_1B
>sequential_5_sequential_1_dense_statefulpartitionedcall_args_2D
@sequential_5_sequential_3_dense_1_statefulpartitionedcall_args_1D
@sequential_5_sequential_3_dense_1_statefulpartitionedcall_args_2D
@sequential_5_sequential_4_dense_2_statefulpartitionedcall_args_1D
@sequential_5_sequential_4_dense_2_statefulpartitionedcall_args_2
identity??7sequential_5/sequential_1/dense/StatefulPartitionedCall?8sequential_5/sequential_2/conv2d/StatefulPartitionedCall?9sequential_5/sequential_3/dense_1/StatefulPartitionedCall?9sequential_5/sequential_4/dense_2/StatefulPartitionedCall?
8sequential_5/sequential_2/conv2d/StatefulPartitionedCallStatefulPartitionedCallsequential_2_input?sequential_5_sequential_2_conv2d_statefulpartitionedcall_args_1?sequential_5_sequential_2_conv2d_statefulpartitionedcall_args_2*/
_output_shapes
:?????????`d**
config_proto

GPU 

CPU2J 8*+
_gradient_op_typePartitionedCall-5838*
Tout
2*0
f+R)
'__inference_restored_function_body_5837*
Tin
2?
7sequential_5/sequential_1/max_pooling2d/PartitionedCallPartitionedCallAsequential_5/sequential_2/conv2d/StatefulPartitionedCall:output:0**
config_proto

GPU 

CPU2J 8*+
_gradient_op_typePartitionedCall-4562*
Tout
2*
Tin
2*/
_output_shapes
:????????? d*0
f+R)
'__inference_restored_function_body_4561?
1sequential_5/sequential_1/flatten/PartitionedCallPartitionedCall@sequential_5/sequential_1/max_pooling2d/PartitionedCall:output:0**
config_proto

GPU 

CPU2J 8*+
_gradient_op_typePartitionedCall-4571*
Tin
2*0
f+R)
'__inference_restored_function_body_4570*
Tout
2*(
_output_shapes
:??????????K?
7sequential_5/sequential_1/dense/StatefulPartitionedCallStatefulPartitionedCall:sequential_5/sequential_1/flatten/PartitionedCall:output:0>sequential_5_sequential_1_dense_statefulpartitionedcall_args_1>sequential_5_sequential_1_dense_statefulpartitionedcall_args_2*
Tout
2**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:?????????*
Tin
2*+
_gradient_op_typePartitionedCall-4582*0
f+R)
'__inference_restored_function_body_4581?
9sequential_5/sequential_3/dense_1/StatefulPartitionedCallStatefulPartitionedCall@sequential_5/sequential_1/dense/StatefulPartitionedCall:output:0@sequential_5_sequential_3_dense_1_statefulpartitionedcall_args_1@sequential_5_sequential_3_dense_1_statefulpartitionedcall_args_2*0
f+R)
'__inference_restored_function_body_7171*'
_output_shapes
:?????????*
Tin
2**
config_proto

GPU 

CPU2J 8*+
_gradient_op_typePartitionedCall-7172*
Tout
2?
1sequential_5/sequential_3/dropout/PartitionedCallPartitionedCallBsequential_5/sequential_3/dense_1/StatefulPartitionedCall:output:0*
Tin
2*+
_gradient_op_typePartitionedCall-7196**
config_proto

GPU 

CPU2J 8*
Tout
2*'
_output_shapes
:?????????*0
f+R)
'__inference_restored_function_body_7195?
9sequential_5/sequential_4/dense_2/StatefulPartitionedCallStatefulPartitionedCall:sequential_5/sequential_3/dropout/PartitionedCall:output:0@sequential_5_sequential_4_dense_2_statefulpartitionedcall_args_1@sequential_5_sequential_4_dense_2_statefulpartitionedcall_args_2*'
_output_shapes
:?????????*
Tin
2**
config_proto

GPU 

CPU2J 8*+
_gradient_op_typePartitionedCall-8498*
Tout
2*0
f+R)
'__inference_restored_function_body_8497?
IdentityIdentityBsequential_5/sequential_4/dense_2/StatefulPartitionedCall:output:08^sequential_5/sequential_1/dense/StatefulPartitionedCall9^sequential_5/sequential_2/conv2d/StatefulPartitionedCall:^sequential_5/sequential_3/dense_1/StatefulPartitionedCall:^sequential_5/sequential_4/dense_2/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*N
_input_shapes=
;:?????????d::::::::2v
9sequential_5/sequential_3/dense_1/StatefulPartitionedCall9sequential_5/sequential_3/dense_1/StatefulPartitionedCall2t
8sequential_5/sequential_2/conv2d/StatefulPartitionedCall8sequential_5/sequential_2/conv2d/StatefulPartitionedCall2v
9sequential_5/sequential_4/dense_2/StatefulPartitionedCall9sequential_5/sequential_4/dense_2/StatefulPartitionedCall2r
7sequential_5/sequential_1/dense/StatefulPartitionedCall7sequential_5/sequential_1/dense/StatefulPartitionedCall:2 .
,
_user_specified_namesequential_2_input: : : : : : : : 
?
?
,__inference_sequential_4_layer_call_fn_10677
input_1"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinput_1statefulpartitionedcall_args_1statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*
Tout
2*P
fKRI
G__inference_sequential_4_layer_call_and_return_conditional_losses_10671*'
_output_shapes
:?????????*
Tin
2*,
_gradient_op_typePartitionedCall-10672?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::22
StatefulPartitionedCallStatefulPartitionedCall:' #
!
_user_specified_name	input_1: : 
?
?
G__inference_sequential_1_layer_call_and_return_conditional_losses_10953

inputs(
$dense_statefulpartitionedcall_args_1(
$dense_statefulpartitionedcall_args_2
identity??dense/StatefulPartitionedCall?
max_pooling2d/PartitionedCallPartitionedCallinputs*/
_output_shapes
:????????? d*+
_gradient_op_typePartitionedCall-4562*
Tout
2*
Tin
2*0
f+R)
'__inference_restored_function_body_4561**
config_proto

GPU 

CPU2J 8?
flatten/PartitionedCallPartitionedCall&max_pooling2d/PartitionedCall:output:0*0
f+R)
'__inference_restored_function_body_4570**
config_proto

GPU 

CPU2J 8*(
_output_shapes
:??????????K*+
_gradient_op_typePartitionedCall-4571*
Tin
2*
Tout
2?
dense/StatefulPartitionedCallStatefulPartitionedCall flatten/PartitionedCall:output:0$dense_statefulpartitionedcall_args_1$dense_statefulpartitionedcall_args_2*+
_gradient_op_typePartitionedCall-4582*'
_output_shapes
:?????????*0
f+R)
'__inference_restored_function_body_4581**
config_proto

GPU 

CPU2J 8*
Tout
2*
Tin
2?
IdentityIdentity&dense/StatefulPartitionedCall:output:0^dense/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*6
_input_shapes%
#:?????????`d::2>
dense/StatefulPartitionedCalldense/StatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
_
A__inference_dropout_layer_call_and_return_conditional_losses_6408

inputs

identity_1N
IdentityIdentityinputs*
T0*'
_output_shapes
:?????????[

Identity_1IdentityIdentity:output:0*'
_output_shapes
:?????????*
T0"!

identity_1Identity_1:output:0*&
_input_shapes
:?????????:& "
 
_user_specified_nameinputs
?
?
$__inference_dense_layer_call_fn_3485

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*+
_gradient_op_typePartitionedCall-3478**
config_proto

GPU 

CPU2J 8*
Tout
2*
Tin
2*H
fCRA
?__inference_dense_layer_call_and_return_conditional_losses_3477*'
_output_shapes
:??????????
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*/
_input_shapes
:??????????K::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
B
&__inference_dropout_layer_call_fn_6482

inputs
identity?
PartitionedCallPartitionedCallinputs*+
_gradient_op_typePartitionedCall-6409*
Tout
2**
config_proto

GPU 

CPU2J 8*
Tin
2*'
_output_shapes
:?????????*J
fERC
A__inference_dropout_layer_call_and_return_conditional_losses_6408`
IdentityIdentityPartitionedCall:output:0*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*&
_input_shapes
:?????????:& "
 
_user_specified_nameinputs
?
?
G__inference_sequential_5_layer_call_and_return_conditional_losses_10768
sequential_2_input/
+sequential_2_statefulpartitionedcall_args_1/
+sequential_2_statefulpartitionedcall_args_2/
+sequential_1_statefulpartitionedcall_args_1/
+sequential_1_statefulpartitionedcall_args_2/
+sequential_3_statefulpartitionedcall_args_1/
+sequential_3_statefulpartitionedcall_args_2/
+sequential_4_statefulpartitionedcall_args_1/
+sequential_4_statefulpartitionedcall_args_2
identity??$sequential_1/StatefulPartitionedCall?$sequential_2/StatefulPartitionedCall?$sequential_3/StatefulPartitionedCall?$sequential_4/StatefulPartitionedCall?
$sequential_2/StatefulPartitionedCallStatefulPartitionedCallsequential_2_input+sequential_2_statefulpartitionedcall_args_1+sequential_2_statefulpartitionedcall_args_2*P
fKRI
G__inference_sequential_2_layer_call_and_return_conditional_losses_10509*/
_output_shapes
:?????????`d**
config_proto

GPU 

CPU2J 8*
Tout
2*,
_gradient_op_typePartitionedCall-10510*
Tin
2?
$sequential_1/StatefulPartitionedCallStatefulPartitionedCall-sequential_2/StatefulPartitionedCall:output:0+sequential_1_statefulpartitionedcall_args_1+sequential_1_statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:?????????*
Tout
2*P
fKRI
G__inference_sequential_1_layer_call_and_return_conditional_losses_10567*
Tin
2*,
_gradient_op_typePartitionedCall-10568?
$sequential_3/StatefulPartitionedCallStatefulPartitionedCall-sequential_1/StatefulPartitionedCall:output:0+sequential_3_statefulpartitionedcall_args_1+sequential_3_statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*P
fKRI
G__inference_sequential_3_layer_call_and_return_conditional_losses_10621*,
_gradient_op_typePartitionedCall-10622*
Tout
2*'
_output_shapes
:?????????*
Tin
2?
$sequential_4/StatefulPartitionedCallStatefulPartitionedCall-sequential_3/StatefulPartitionedCall:output:0+sequential_4_statefulpartitionedcall_args_1+sequential_4_statefulpartitionedcall_args_2*'
_output_shapes
:?????????*,
_gradient_op_typePartitionedCall-10672*P
fKRI
G__inference_sequential_4_layer_call_and_return_conditional_losses_10671*
Tout
2*
Tin
2**
config_proto

GPU 

CPU2J 8?
IdentityIdentity-sequential_4/StatefulPartitionedCall:output:0%^sequential_1/StatefulPartitionedCall%^sequential_2/StatefulPartitionedCall%^sequential_3/StatefulPartitionedCall%^sequential_4/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*N
_input_shapes=
;:?????????d::::::::2L
$sequential_4/StatefulPartitionedCall$sequential_4/StatefulPartitionedCall2L
$sequential_1/StatefulPartitionedCall$sequential_1/StatefulPartitionedCall2L
$sequential_2/StatefulPartitionedCall$sequential_2/StatefulPartitionedCall2L
$sequential_3/StatefulPartitionedCall$sequential_3/StatefulPartitionedCall:2 .
,
_user_specified_namesequential_2_input: : : : : : : : 
?
?
&__inference_dense_2_layer_call_fn_7306

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*'
_output_shapes
:?????????*+
_gradient_op_typePartitionedCall-7299*
Tout
2*J
fERC
A__inference_dense_2_layer_call_and_return_conditional_losses_7298*
Tin
2**
config_proto

GPU 

CPU2J 8?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?

?
#__inference_signature_wrapper_10850
sequential_2_input"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2"
statefulpartitionedcall_args_3"
statefulpartitionedcall_args_4"
statefulpartitionedcall_args_5"
statefulpartitionedcall_args_6"
statefulpartitionedcall_args_7"
statefulpartitionedcall_args_8
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallsequential_2_inputstatefulpartitionedcall_args_1statefulpartitionedcall_args_2statefulpartitionedcall_args_3statefulpartitionedcall_args_4statefulpartitionedcall_args_5statefulpartitionedcall_args_6statefulpartitionedcall_args_7statefulpartitionedcall_args_8**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-10839*)
f$R"
 __inference__wrapped_model_10463*
Tout
2*'
_output_shapes
:?????????*
Tin
2	?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*N
_input_shapes=
;:?????????d::::::::22
StatefulPartitionedCallStatefulPartitionedCall: : : :2 .
,
_user_specified_namesequential_2_input: : : : : 
?
|
E__forward_max_pooling2d_layer_call_and_return_conditional_losses_9145
inputs_0
identity

inputs
maxpool?
MaxPoolMaxPoolinputs_0*J
_output_shapes8
6:4????????????????????????????????????*
strides
*
paddingVALID*
ksize
{
IdentityIdentityMaxPool:output:0*J
_output_shapes8
6:4????????????????????????????????????*
T0"
inputsinputs_0"
maxpoolMaxPool:output:0"
identityIdentity:output:0*I
_input_shapes8
6:4????????????????????????????????????*s
backward_function_nameYW__inference___backward_max_pooling2d_layer_call_and_return_conditional_losses_9137_9146:& "
 
_user_specified_nameinputs
?
?
&__inference_dense_1_layer_call_fn_6131

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*+
_gradient_op_typePartitionedCall-6124*
Tout
2**
config_proto

GPU 

CPU2J 8*J
fERC
A__inference_dense_1_layer_call_and_return_conditional_losses_6123*
Tin
2*'
_output_shapes
:??????????
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
c
G__inference_max_pooling2d_layer_call_and_return_conditional_losses_3719

inputs
identity?
MaxPoolMaxPoolinputs*
ksize
*
paddingVALID*J
_output_shapes8
6:4????????????????????????????????????*
strides
{
IdentityIdentityMaxPool:output:0*
T0*J
_output_shapes8
6:4????????????????????????????????????"
identityIdentity:output:0*I
_input_shapes8
6:4????????????????????????????????????:& "
 
_user_specified_nameinputs
?
?
G__inference_sequential_1_layer_call_and_return_conditional_losses_10548

inputs(
$dense_statefulpartitionedcall_args_1(
$dense_statefulpartitionedcall_args_2
identity??dense/StatefulPartitionedCall?
max_pooling2d/PartitionedCallPartitionedCallinputs**
config_proto

GPU 

CPU2J 8*0
f+R)
'__inference_restored_function_body_4561*/
_output_shapes
:????????? d*+
_gradient_op_typePartitionedCall-4562*
Tout
2*
Tin
2?
flatten/PartitionedCallPartitionedCall&max_pooling2d/PartitionedCall:output:0*(
_output_shapes
:??????????K*+
_gradient_op_typePartitionedCall-4571*
Tin
2**
config_proto

GPU 

CPU2J 8*
Tout
2*0
f+R)
'__inference_restored_function_body_4570?
dense/StatefulPartitionedCallStatefulPartitionedCall flatten/PartitionedCall:output:0$dense_statefulpartitionedcall_args_1$dense_statefulpartitionedcall_args_2*'
_output_shapes
:?????????*
Tin
2*
Tout
2*0
f+R)
'__inference_restored_function_body_4581**
config_proto

GPU 

CPU2J 8*+
_gradient_op_typePartitionedCall-4582?
IdentityIdentity&dense/StatefulPartitionedCall:output:0^dense/StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*6
_input_shapes%
#:?????????`d::2>
dense/StatefulPartitionedCalldense/StatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?	
?
A__inference_dense_2_layer_call_and_return_conditional_losses_7601

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes

:*
dtype0i
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:??????????
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:v
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*'
_output_shapes
:?????????*
T0V
SoftmaxSoftmaxBiasAdd:output:0*
T0*'
_output_shapes
:??????????
IdentityIdentitySoftmax:softmax:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp:& "
 
_user_specified_nameinputs: : 
?
`
'__inference_restored_function_body_7185

inputs
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputs**
config_proto

GPU 

CPU2J 8*b
_output_shapesP
N:?????????:?????????:?????????:?????????: *
Tin
2*+
_gradient_op_typePartitionedCall-6392*J
fERC
A__inference_dropout_layer_call_and_return_conditional_losses_6391*
Tout
2?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*&
_input_shapes
:?????????22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs
?
]
A__inference_flatten_layer_call_and_return_conditional_losses_3422

inputs
identity^
Reshape/shapeConst*
dtype0*
valueB"?????%  *
_output_shapes
:e
ReshapeReshapeinputsReshape/shape:output:0*(
_output_shapes
:??????????K*
T0Y
IdentityIdentityReshape:output:0*
T0*(
_output_shapes
:??????????K"
identityIdentity:output:0*.
_input_shapes
:????????? d:& "
 
_user_specified_nameinputs
?
?
A__inference_dense_1_layer_call_and_return_conditional_losses_6123

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes

:*
dtype0i
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:??????????
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:v
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*'
_output_shapes
:?????????*
T0?
IdentityIdentityBiasAdd:output:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*.
_input_shapes
:?????????::20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp: :& "
 
_user_specified_nameinputs: 
?
?
,__inference_sequential_1_layer_call_fn_10976

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*P
fKRI
G__inference_sequential_1_layer_call_and_return_conditional_losses_10567*,
_gradient_op_typePartitionedCall-10568*'
_output_shapes
:?????????*
Tin
2*
Tout
2**
config_proto

GPU 

CPU2J 8?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*6
_input_shapes%
#:?????????`d::22
StatefulPartitionedCallStatefulPartitionedCall: :& "
 
_user_specified_nameinputs: 
?
?
G__inference_sequential_5_layer_call_and_return_conditional_losses_10819

inputs/
+sequential_2_statefulpartitionedcall_args_1/
+sequential_2_statefulpartitionedcall_args_2/
+sequential_1_statefulpartitionedcall_args_1/
+sequential_1_statefulpartitionedcall_args_2/
+sequential_3_statefulpartitionedcall_args_1/
+sequential_3_statefulpartitionedcall_args_2/
+sequential_4_statefulpartitionedcall_args_1/
+sequential_4_statefulpartitionedcall_args_2
identity??$sequential_1/StatefulPartitionedCall?$sequential_2/StatefulPartitionedCall?$sequential_3/StatefulPartitionedCall?$sequential_4/StatefulPartitionedCall?
$sequential_2/StatefulPartitionedCallStatefulPartitionedCallinputs+sequential_2_statefulpartitionedcall_args_1+sequential_2_statefulpartitionedcall_args_2*/
_output_shapes
:?????????`d*P
fKRI
G__inference_sequential_2_layer_call_and_return_conditional_losses_10509*
Tout
2*,
_gradient_op_typePartitionedCall-10510**
config_proto

GPU 

CPU2J 8*
Tin
2?
$sequential_1/StatefulPartitionedCallStatefulPartitionedCall-sequential_2/StatefulPartitionedCall:output:0+sequential_1_statefulpartitionedcall_args_1+sequential_1_statefulpartitionedcall_args_2*P
fKRI
G__inference_sequential_1_layer_call_and_return_conditional_losses_10567*
Tout
2**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:?????????*
Tin
2*,
_gradient_op_typePartitionedCall-10568?
$sequential_3/StatefulPartitionedCallStatefulPartitionedCall-sequential_1/StatefulPartitionedCall:output:0+sequential_3_statefulpartitionedcall_args_1+sequential_3_statefulpartitionedcall_args_2*
Tout
2*P
fKRI
G__inference_sequential_3_layer_call_and_return_conditional_losses_10621*'
_output_shapes
:?????????*
Tin
2**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-10622?
$sequential_4/StatefulPartitionedCallStatefulPartitionedCall-sequential_3/StatefulPartitionedCall:output:0+sequential_4_statefulpartitionedcall_args_1+sequential_4_statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-10672*
Tin
2*'
_output_shapes
:?????????*
Tout
2*P
fKRI
G__inference_sequential_4_layer_call_and_return_conditional_losses_10671?
IdentityIdentity-sequential_4/StatefulPartitionedCall:output:0%^sequential_1/StatefulPartitionedCall%^sequential_2/StatefulPartitionedCall%^sequential_3/StatefulPartitionedCall%^sequential_4/StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*N
_input_shapes=
;:?????????d::::::::2L
$sequential_1/StatefulPartitionedCall$sequential_1/StatefulPartitionedCall2L
$sequential_2/StatefulPartitionedCall$sequential_2/StatefulPartitionedCall2L
$sequential_3/StatefulPartitionedCall$sequential_3/StatefulPartitionedCall2L
$sequential_4/StatefulPartitionedCall$sequential_4/StatefulPartitionedCall: : : : : : :& "
 
_user_specified_nameinputs: : 
?
`
A__inference_dropout_layer_call_and_return_conditional_losses_6503

inputs
identity?Q
dropout/rateConst*
_output_shapes
: *
valueB
 *   ?*
dtype0C
dropout/ShapeShapeinputs*
_output_shapes
:*
T0_
dropout/random_uniform/minConst*
valueB
 *    *
dtype0*
_output_shapes
: _
dropout/random_uniform/maxConst*
_output_shapes
: *
dtype0*
valueB
 *  ???
$dropout/random_uniform/RandomUniformRandomUniformdropout/Shape:output:0*
dtype0*'
_output_shapes
:?????????*
T0?
dropout/random_uniform/subSub#dropout/random_uniform/max:output:0#dropout/random_uniform/min:output:0*
T0*
_output_shapes
: ?
dropout/random_uniform/mulMul-dropout/random_uniform/RandomUniform:output:0dropout/random_uniform/sub:z:0*
T0*'
_output_shapes
:??????????
dropout/random_uniformAdddropout/random_uniform/mul:z:0#dropout/random_uniform/min:output:0*'
_output_shapes
:?????????*
T0R
dropout/sub/xConst*
dtype0*
_output_shapes
: *
valueB
 *  ??b
dropout/subSubdropout/sub/x:output:0dropout/rate:output:0*
T0*
_output_shapes
: V
dropout/truediv/xConst*
_output_shapes
: *
dtype0*
valueB
 *  ??h
dropout/truedivRealDivdropout/truediv/x:output:0dropout/sub:z:0*
T0*
_output_shapes
: ?
dropout/GreaterEqualGreaterEqualdropout/random_uniform:z:0dropout/rate:output:0*
T0*'
_output_shapes
:?????????a
dropout/mulMulinputsdropout/truediv:z:0*
T0*'
_output_shapes
:?????????o
dropout/CastCastdropout/GreaterEqual:z:0*'
_output_shapes
:?????????*

DstT0*

SrcT0
i
dropout/mul_1Muldropout/mul:z:0dropout/Cast:y:0*'
_output_shapes
:?????????*
T0Y
IdentityIdentitydropout/mul_1:z:0*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*&
_input_shapes
:?????????:& "
 
_user_specified_nameinputs
?
?
G__inference_sequential_2_layer_call_and_return_conditional_losses_10482
conv2d_input)
%conv2d_statefulpartitionedcall_args_1)
%conv2d_statefulpartitionedcall_args_2
identity??conv2d/StatefulPartitionedCall?
conv2d/StatefulPartitionedCallStatefulPartitionedCallconv2d_input%conv2d_statefulpartitionedcall_args_1%conv2d_statefulpartitionedcall_args_2*+
_gradient_op_typePartitionedCall-5838*
Tout
2*0
f+R)
'__inference_restored_function_body_5837*
Tin
2*/
_output_shapes
:?????????`d**
config_proto

GPU 

CPU2J 8?
IdentityIdentity'conv2d/StatefulPartitionedCall:output:0^conv2d/StatefulPartitionedCall*
T0*/
_output_shapes
:?????????`d"
identityIdentity:output:0*6
_input_shapes%
#:?????????d::2@
conv2d/StatefulPartitionedCallconv2d/StatefulPartitionedCall:, (
&
_user_specified_nameconv2d_input: : 
?
?
G__inference_sequential_2_layer_call_and_return_conditional_losses_10509

inputs)
%conv2d_statefulpartitionedcall_args_1)
%conv2d_statefulpartitionedcall_args_2
identity??conv2d/StatefulPartitionedCall?
conv2d/StatefulPartitionedCallStatefulPartitionedCallinputs%conv2d_statefulpartitionedcall_args_1%conv2d_statefulpartitionedcall_args_2*/
_output_shapes
:?????????`d*
Tout
2*
Tin
2**
config_proto

GPU 

CPU2J 8*0
f+R)
'__inference_restored_function_body_5837*+
_gradient_op_typePartitionedCall-5838?
IdentityIdentity'conv2d/StatefulPartitionedCall:output:0^conv2d/StatefulPartitionedCall*/
_output_shapes
:?????????`d*
T0"
identityIdentity:output:0*6
_input_shapes%
#:?????????d::2@
conv2d/StatefulPartitionedCallconv2d/StatefulPartitionedCall: :& "
 
_user_specified_nameinputs: 
?
?
?__inference_dense_layer_call_and_return_conditional_losses_3477

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:	?K*
dtype0i
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:??????????
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:*
dtype0v
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:??????????
IdentityIdentityBiasAdd:output:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*/
_input_shapes
:??????????K::20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp:& "
 
_user_specified_nameinputs: : 
?
?
,__inference_sequential_2_layer_call_fn_10944

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*P
fKRI
G__inference_sequential_2_layer_call_and_return_conditional_losses_10509*
Tout
2*
Tin
2**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-10510*/
_output_shapes
:?????????`d?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*/
_output_shapes
:?????????`d"
identityIdentity:output:0*6
_input_shapes%
#:?????????d::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
?
G__inference_sequential_5_layer_call_and_return_conditional_losses_10787

inputs/
+sequential_2_statefulpartitionedcall_args_1/
+sequential_2_statefulpartitionedcall_args_2/
+sequential_1_statefulpartitionedcall_args_1/
+sequential_1_statefulpartitionedcall_args_2/
+sequential_3_statefulpartitionedcall_args_1/
+sequential_3_statefulpartitionedcall_args_2/
+sequential_4_statefulpartitionedcall_args_1/
+sequential_4_statefulpartitionedcall_args_2
identity??$sequential_1/StatefulPartitionedCall?$sequential_2/StatefulPartitionedCall?$sequential_3/StatefulPartitionedCall?$sequential_4/StatefulPartitionedCall?
$sequential_2/StatefulPartitionedCallStatefulPartitionedCallinputs+sequential_2_statefulpartitionedcall_args_1+sequential_2_statefulpartitionedcall_args_2*P
fKRI
G__inference_sequential_2_layer_call_and_return_conditional_losses_10492**
config_proto

GPU 

CPU2J 8*
Tin
2*/
_output_shapes
:?????????`d*,
_gradient_op_typePartitionedCall-10493*
Tout
2?
$sequential_1/StatefulPartitionedCallStatefulPartitionedCall-sequential_2/StatefulPartitionedCall:output:0+sequential_1_statefulpartitionedcall_args_1+sequential_1_statefulpartitionedcall_args_2*
Tin
2*,
_gradient_op_typePartitionedCall-10549*P
fKRI
G__inference_sequential_1_layer_call_and_return_conditional_losses_10548*
Tout
2**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:??????????
$sequential_3/StatefulPartitionedCallStatefulPartitionedCall-sequential_1/StatefulPartitionedCall:output:0+sequential_3_statefulpartitionedcall_args_1+sequential_3_statefulpartitionedcall_args_2*
Tout
2*P
fKRI
G__inference_sequential_3_layer_call_and_return_conditional_losses_10603*,
_gradient_op_typePartitionedCall-10604*
Tin
2**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:??????????
$sequential_4/StatefulPartitionedCallStatefulPartitionedCall-sequential_3/StatefulPartitionedCall:output:0+sequential_4_statefulpartitionedcall_args_1+sequential_4_statefulpartitionedcall_args_2*
Tin
2*P
fKRI
G__inference_sequential_4_layer_call_and_return_conditional_losses_10654*,
_gradient_op_typePartitionedCall-10655**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:?????????*
Tout
2?
IdentityIdentity-sequential_4/StatefulPartitionedCall:output:0%^sequential_1/StatefulPartitionedCall%^sequential_2/StatefulPartitionedCall%^sequential_3/StatefulPartitionedCall%^sequential_4/StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*N
_input_shapes=
;:?????????d::::::::2L
$sequential_1/StatefulPartitionedCall$sequential_1/StatefulPartitionedCall2L
$sequential_2/StatefulPartitionedCall$sequential_2/StatefulPartitionedCall2L
$sequential_3/StatefulPartitionedCall$sequential_3/StatefulPartitionedCall2L
$sequential_4/StatefulPartitionedCall$sequential_4/StatefulPartitionedCall:& "
 
_user_specified_nameinputs: : : : : : : : 
?

?
G__inference_sequential_3_layer_call_and_return_conditional_losses_10984

inputs*
&dense_1_statefulpartitionedcall_args_1*
&dense_1_statefulpartitionedcall_args_2
identity??dense_1/StatefulPartitionedCall?dropout/StatefulPartitionedCall?
dense_1/StatefulPartitionedCallStatefulPartitionedCallinputs&dense_1_statefulpartitionedcall_args_1&dense_1_statefulpartitionedcall_args_2*
Tout
2**
config_proto

GPU 

CPU2J 8*0
f+R)
'__inference_restored_function_body_7171*'
_output_shapes
:?????????*+
_gradient_op_typePartitionedCall-7172*
Tin
2?
dropout/StatefulPartitionedCallStatefulPartitionedCall(dense_1/StatefulPartitionedCall:output:0*'
_output_shapes
:?????????*
Tout
2*0
f+R)
'__inference_restored_function_body_7185*+
_gradient_op_typePartitionedCall-7186*
Tin
2**
config_proto

GPU 

CPU2J 8?
IdentityIdentity(dropout/StatefulPartitionedCall:output:0 ^dense_1/StatefulPartitionedCall ^dropout/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::2B
dropout/StatefulPartitionedCalldropout/StatefulPartitionedCall2B
dense_1/StatefulPartitionedCalldense_1/StatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
?
G__inference_sequential_4_layer_call_and_return_conditional_losses_10635
input_1*
&dense_2_statefulpartitionedcall_args_1*
&dense_2_statefulpartitionedcall_args_2
identity??dense_2/StatefulPartitionedCall?
dense_2/StatefulPartitionedCallStatefulPartitionedCallinput_1&dense_2_statefulpartitionedcall_args_1&dense_2_statefulpartitionedcall_args_2*
Tout
2*+
_gradient_op_typePartitionedCall-8498*'
_output_shapes
:?????????*
Tin
2**
config_proto

GPU 

CPU2J 8*0
f+R)
'__inference_restored_function_body_8497?
IdentityIdentity(dense_2/StatefulPartitionedCall:output:0 ^dense_2/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::2B
dense_2/StatefulPartitionedCalldense_2/StatefulPartitionedCall:' #
!
_user_specified_name	input_1: : 
?	
?
G__inference_sequential_3_layer_call_and_return_conditional_losses_10621

inputs*
&dense_1_statefulpartitionedcall_args_1*
&dense_1_statefulpartitionedcall_args_2
identity??dense_1/StatefulPartitionedCall?
dense_1/StatefulPartitionedCallStatefulPartitionedCallinputs&dense_1_statefulpartitionedcall_args_1&dense_1_statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*
Tin
2*'
_output_shapes
:?????????*+
_gradient_op_typePartitionedCall-7172*0
f+R)
'__inference_restored_function_body_7171*
Tout
2?
dropout/PartitionedCallPartitionedCall(dense_1/StatefulPartitionedCall:output:0*0
f+R)
'__inference_restored_function_body_7195**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:?????????*+
_gradient_op_typePartitionedCall-7196*
Tout
2*
Tin
2?
IdentityIdentity dropout/PartitionedCall:output:0 ^dense_1/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::2B
dense_1/StatefulPartitionedCalldense_1/StatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
?
,__inference_sequential_1_layer_call_fn_10573
input_1"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinput_1statefulpartitionedcall_args_1statefulpartitionedcall_args_2*,
_gradient_op_typePartitionedCall-10568*'
_output_shapes
:?????????*
Tout
2*P
fKRI
G__inference_sequential_1_layer_call_and_return_conditional_losses_10567*
Tin
2**
config_proto

GPU 

CPU2J 8?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*6
_input_shapes%
#:?????????`d::22
StatefulPartitionedCallStatefulPartitionedCall:' #
!
_user_specified_name	input_1: : 
?
?
G__inference_sequential_4_layer_call_and_return_conditional_losses_11013

inputs*
&dense_2_statefulpartitionedcall_args_1*
&dense_2_statefulpartitionedcall_args_2
identity??dense_2/StatefulPartitionedCall?
dense_2/StatefulPartitionedCallStatefulPartitionedCallinputs&dense_2_statefulpartitionedcall_args_1&dense_2_statefulpartitionedcall_args_2*0
f+R)
'__inference_restored_function_body_8497**
config_proto

GPU 

CPU2J 8*
Tin
2*+
_gradient_op_typePartitionedCall-8498*'
_output_shapes
:?????????*
Tout
2?
IdentityIdentity(dense_2/StatefulPartitionedCall:output:0 ^dense_2/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::2B
dense_2/StatefulPartitionedCalldense_2/StatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
?
'__inference_restored_function_body_7171

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*D
_output_shapes2
0:?????????::?????????*
Tout
2*
Tin
2**
config_proto

GPU 

CPU2J 8*+
_gradient_op_typePartitionedCall-6031*J
fERC
A__inference_dense_1_layer_call_and_return_conditional_losses_6030?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?

?
G__inference_sequential_3_layer_call_and_return_conditional_losses_10582
input_1*
&dense_1_statefulpartitionedcall_args_1*
&dense_1_statefulpartitionedcall_args_2
identity??dense_1/StatefulPartitionedCall?dropout/StatefulPartitionedCall?
dense_1/StatefulPartitionedCallStatefulPartitionedCallinput_1&dense_1_statefulpartitionedcall_args_1&dense_1_statefulpartitionedcall_args_2*0
f+R)
'__inference_restored_function_body_7171*
Tout
2*
Tin
2**
config_proto

GPU 

CPU2J 8*+
_gradient_op_typePartitionedCall-7172*'
_output_shapes
:??????????
dropout/StatefulPartitionedCallStatefulPartitionedCall(dense_1/StatefulPartitionedCall:output:0*0
f+R)
'__inference_restored_function_body_7185*
Tin
2**
config_proto

GPU 

CPU2J 8*+
_gradient_op_typePartitionedCall-7186*'
_output_shapes
:?????????*
Tout
2?
IdentityIdentity(dropout/StatefulPartitionedCall:output:0 ^dense_1/StatefulPartitionedCall ^dropout/StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*.
_input_shapes
:?????????::2B
dropout/StatefulPartitionedCalldropout/StatefulPartitionedCall2B
dense_1/StatefulPartitionedCalldense_1/StatefulPartitionedCall:' #
!
_user_specified_name	input_1: : 
?
?
?__forward_dense_2_layer_call_and_return_conditional_losses_8957
inputs_0"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity
softmax
matmul_readvariableop

inputs??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes

:k
MatMulMatMulinputs_0MatMul/ReadVariableOp:value:0*'
_output_shapes
:?????????*
T0?
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:*
dtype0v
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*'
_output_shapes
:?????????*
T0V
SoftmaxSoftmaxBiasAdd:output:0*'
_output_shapes
:?????????*
T0?
IdentityIdentitySoftmax:softmax:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*
T0*'
_output_shapes
:?????????"6
matmul_readvariableopMatMul/ReadVariableOp:value:0"
inputsinputs_0"
identityIdentity:output:0"
softmaxSoftmax:softmax:0*.
_input_shapes
:?????????::*m
backward_function_nameSQ__inference___backward_dense_2_layer_call_and_return_conditional_losses_8939_895820
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp:& "
 
_user_specified_nameinputs: : 
?

?
?__forward_dense_1_layer_call_and_return_conditional_losses_9051
inputs_0"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity
matmul_readvariableop

inputs??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes

:k
MatMulMatMulinputs_0MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:??????????
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:v
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:??????????
IdentityIdentityBiasAdd:output:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0"6
matmul_readvariableopMatMul/ReadVariableOp:value:0"
inputsinputs_0*.
_input_shapes
:?????????::*m
backward_function_nameSQ__inference___backward_dense_1_layer_call_and_return_conditional_losses_9039_905220
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp:& "
 
_user_specified_nameinputs: : 
?
C
'__inference_restored_function_body_4570

inputs
identity?
PartitionedCallPartitionedCallinputs*
Tin
2**
config_proto

GPU 

CPU2J 8*
Tout
2*+
_gradient_op_typePartitionedCall-3423*C
_output_shapes1
/:??????????K:????????? d*J
fERC
A__inference_flatten_layer_call_and_return_conditional_losses_3422a
IdentityIdentityPartitionedCall:output:0*(
_output_shapes
:??????????K*
T0"
identityIdentity:output:0*.
_input_shapes
:????????? d:& "
 
_user_specified_nameinputs
?
?
,__inference_sequential_1_layer_call_fn_10969

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*P
fKRI
G__inference_sequential_1_layer_call_and_return_conditional_losses_10548*'
_output_shapes
:?????????**
config_proto

GPU 

CPU2J 8*
Tout
2*,
_gradient_op_typePartitionedCall-10549*
Tin
2?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*6
_input_shapes%
#:?????????`d::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : "wL
saver_filename:0StatefulPartitionedCall_1:0StatefulPartitionedCall_28"
saved_model_main_op

NoOp*?
serving_default?
Y
sequential_2_inputC
$serving_default_sequential_2_input:0?????????d@
sequential_40
StatefulPartitionedCall:0?????????tensorflow/serving/predict*>
__saved_model_init_op%#
__saved_model_init_op

NoOp:̀
?4
layer-0
layer_with_weights-0
layer-1
layer_with_weights-1
layer-2
layer_with_weights-2
layer-3
layer_with_weights-3
layer-4
	optimizer
regularization_losses
	variables
	trainable_variables

	keras_api

signatures
?_default_save_signature
?__call__
+?&call_and_return_all_conditional_losses"?1
_tf_keras_sequential?0{"class_name": "Sequential", "name": "sequential_5", "trainable": true, "expects_training_arg": true, "dtype": "float32", "batch_input_shape": null, "config": {"name": "sequential_5", "layers": [{"class_name": "Sequential", "config": {"name": "sequential_2", "layers": [{"class_name": "Conv2D", "config": {"name": "conv2d", "trainable": true, "batch_input_shape": [null, 100, 3, 1], "dtype": "float32", "filters": 100, "kernel_size": [5, 1], "strides": [1, 1], "padding": "valid", "data_format": "channels_last", "dilation_rate": [1, 1], "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}], "batch_input_shape": [null, 100, 3, 1]}}, {"class_name": "Sequential", "config": {"name": "sequential_1", "layers": [{"class_name": "MaxPooling2D", "config": {"name": "max_pooling2d", "trainable": true, "dtype": "float32", "pool_size": [3, 1], "padding": "valid", "strides": [3, 1], "data_format": "channels_last"}}, {"class_name": "Flatten", "config": {"name": "flatten", "trainable": true, "dtype": "float32", "data_format": "channels_last"}}, {"class_name": "Dense", "config": {"name": "dense", "trainable": true, "dtype": "float32", "units": 12, "activation": "linear", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}]}}, {"class_name": "Sequential", "config": {"name": "sequential_3", "layers": [{"class_name": "Dense", "config": {"name": "dense_1", "trainable": true, "dtype": "float32", "units": 12, "activation": "linear", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dropout", "config": {"name": "dropout", "trainable": true, "dtype": "float32", "rate": 0.5, "noise_shape": null, "seed": null}}]}}, {"class_name": "Sequential", "config": {"name": "sequential_4", "layers": [{"class_name": "Dense", "config": {"name": "dense_2", "trainable": true, "dtype": "float32", "units": 4, "activation": "softmax", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}]}}]}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": 4, "max_ndim": null, "min_ndim": null, "axes": {"-1": 1}}}, "keras_version": "2.2.4-tf", "backend": "tensorflow", "model_config": {"class_name": "Sequential", "config": {"name": "sequential_5", "layers": [{"class_name": "Sequential", "config": {"name": "sequential_2", "layers": [{"class_name": "Conv2D", "config": {"name": "conv2d", "trainable": true, "batch_input_shape": [null, 100, 3, 1], "dtype": "float32", "filters": 100, "kernel_size": [5, 1], "strides": [1, 1], "padding": "valid", "data_format": "channels_last", "dilation_rate": [1, 1], "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}], "batch_input_shape": [null, 100, 3, 1]}}, {"class_name": "Sequential", "config": {"name": "sequential_1", "layers": [{"class_name": "MaxPooling2D", "config": {"name": "max_pooling2d", "trainable": true, "dtype": "float32", "pool_size": [3, 1], "padding": "valid", "strides": [3, 1], "data_format": "channels_last"}}, {"class_name": "Flatten", "config": {"name": "flatten", "trainable": true, "dtype": "float32", "data_format": "channels_last"}}, {"class_name": "Dense", "config": {"name": "dense", "trainable": true, "dtype": "float32", "units": 12, "activation": "linear", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}]}}, {"class_name": "Sequential", "config": {"name": "sequential_3", "layers": [{"class_name": "Dense", "config": {"name": "dense_1", "trainable": true, "dtype": "float32", "units": 12, "activation": "linear", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dropout", "config": {"name": "dropout", "trainable": true, "dtype": "float32", "rate": 0.5, "noise_shape": null, "seed": null}}]}}, {"class_name": "Sequential", "config": {"name": "sequential_4", "layers": [{"class_name": "Dense", "config": {"name": "dense_2", "trainable": true, "dtype": "float32", "units": 4, "activation": "softmax", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}]}}]}}, "training_config": {"loss": "categorical_crossentropy", "metrics": ["acc"], "weighted_metrics": null, "sample_weight_mode": null, "loss_weights": null, "optimizer_config": {"class_name": "Adam", "config": {"name": "Adam", "learning_rate": 0.003000000026077032, "decay": 9.999999974752427e-07, "beta_1": 0.8999999761581421, "beta_2": 0.9990000128746033, "epsilon": 1e-07, "amsgrad": false}}}}
?
regularization_losses
	variables
trainable_variables
	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "InputLayer", "name": "sequential_2_input", "trainable": true, "expects_training_arg": true, "dtype": "float32", "batch_input_shape": [null, 100, 3, 1], "config": {"batch_input_shape": [null, 100, 3, 1], "dtype": "float32", "sparse": false, "name": "sequential_2_input"}}
?
layer-0
layer_with_weights-0
layer-1
regularization_losses
	variables
trainable_variables
	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_sequential?{"class_name": "Sequential", "name": "sequential_2", "trainable": false, "expects_training_arg": true, "dtype": "float32", "batch_input_shape": null, "config": {"name": "sequential_2", "layers": [{"class_name": "Conv2D", "config": {"name": "conv2d", "trainable": true, "batch_input_shape": [null, 100, 3, 1], "dtype": "float32", "filters": 100, "kernel_size": [5, 1], "strides": [1, 1], "padding": "valid", "data_format": "channels_last", "dilation_rate": [1, 1], "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}]}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": 4, "max_ndim": null, "min_ndim": null, "axes": {"-1": 1}}}, "keras_version": "2.2.4-tf", "backend": "tensorflow", "model_config": {"class_name": "Sequential", "config": {"name": "sequential_2", "layers": [{"class_name": "Conv2D", "config": {"name": "conv2d", "trainable": true, "batch_input_shape": [null, 100, 3, 1], "dtype": "float32", "filters": 100, "kernel_size": [5, 1], "strides": [1, 1], "padding": "valid", "data_format": "channels_last", "dilation_rate": [1, 1], "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}]}}}
?
layer-0
layer-1
layer_with_weights-0
layer-2
regularization_losses
	variables
trainable_variables
	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_sequential?{"class_name": "Sequential", "name": "sequential_1", "trainable": false, "expects_training_arg": true, "dtype": "float32", "batch_input_shape": null, "config": {"name": "sequential_1", "layers": [{"class_name": "MaxPooling2D", "config": {"name": "max_pooling2d", "trainable": true, "dtype": "float32", "pool_size": [3, 1], "padding": "valid", "strides": [3, 1], "data_format": "channels_last"}}, {"class_name": "Flatten", "config": {"name": "flatten", "trainable": true, "dtype": "float32", "data_format": "channels_last"}}, {"class_name": "Dense", "config": {"name": "dense", "trainable": true, "dtype": "float32", "units": 12, "activation": "linear", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}]}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": 4, "max_ndim": null, "min_ndim": null, "axes": {}}}, "keras_version": "2.2.4-tf", "backend": "tensorflow", "model_config": {"class_name": "Sequential", "config": {"name": "sequential_1", "layers": [{"class_name": "MaxPooling2D", "config": {"name": "max_pooling2d", "trainable": true, "dtype": "float32", "pool_size": [3, 1], "padding": "valid", "strides": [3, 1], "data_format": "channels_last"}}, {"class_name": "Flatten", "config": {"name": "flatten", "trainable": true, "dtype": "float32", "data_format": "channels_last"}}, {"class_name": "Dense", "config": {"name": "dense", "trainable": true, "dtype": "float32", "units": 12, "activation": "linear", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}]}}}
?
layer_with_weights-0
layer-0
layer-1
regularization_losses
 	variables
!trainable_variables
"	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_sequential?{"class_name": "Sequential", "name": "sequential_3", "trainable": true, "expects_training_arg": true, "dtype": "float32", "batch_input_shape": null, "config": {"name": "sequential_3", "layers": [{"class_name": "Dense", "config": {"name": "dense_1", "trainable": true, "dtype": "float32", "units": 12, "activation": "linear", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dropout", "config": {"name": "dropout", "trainable": true, "dtype": "float32", "rate": 0.5, "noise_shape": null, "seed": null}}]}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": null, "max_ndim": null, "min_ndim": 2, "axes": {"-1": 12}}}, "keras_version": "2.2.4-tf", "backend": "tensorflow", "model_config": {"class_name": "Sequential", "config": {"name": "sequential_3", "layers": [{"class_name": "Dense", "config": {"name": "dense_1", "trainable": true, "dtype": "float32", "units": 12, "activation": "linear", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dropout", "config": {"name": "dropout", "trainable": true, "dtype": "float32", "rate": 0.5, "noise_shape": null, "seed": null}}]}}}
?
#layer_with_weights-0
#layer-0
$regularization_losses
%	variables
&trainable_variables
'	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?

_tf_keras_sequential?
{"class_name": "Sequential", "name": "sequential_4", "trainable": true, "expects_training_arg": true, "dtype": "float32", "batch_input_shape": null, "config": {"name": "sequential_4", "layers": [{"class_name": "Dense", "config": {"name": "dense_2", "trainable": true, "dtype": "float32", "units": 4, "activation": "softmax", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}]}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": null, "max_ndim": null, "min_ndim": 2, "axes": {"-1": 12}}}, "keras_version": "2.2.4-tf", "backend": "tensorflow", "model_config": {"class_name": "Sequential", "config": {"name": "sequential_4", "layers": [{"class_name": "Dense", "config": {"name": "dense_2", "trainable": true, "dtype": "float32", "units": 4, "activation": "softmax", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}]}}}
?
(iter

)beta_1

*beta_2
	+decay
,learning_rate1m?2m?3m?4m?1v?2v?3v?4v?"
	optimizer
 "
trackable_list_wrapper
X
-0
.1
/2
03
14
25
36
47"
trackable_list_wrapper
<
10
21
32
43"
trackable_list_wrapper
?
regularization_losses
5metrics
6non_trainable_variables
	variables
7layer_regularization_losses
	trainable_variables

8layers
?__call__
?_default_save_signature
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
-
?serving_default"
signature_map
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
?
regularization_losses
9metrics
:non_trainable_variables
	variables
;layer_regularization_losses
trainable_variables

<layers
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
?
=regularization_losses
>	variables
?trainable_variables
@	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "InputLayer", "name": "conv2d_input", "trainable": false, "expects_training_arg": true, "dtype": "float32", "batch_input_shape": [null, 100, 3, 1], "config": {"batch_input_shape": [null, 100, 3, 1], "dtype": "float32", "sparse": false, "name": "conv2d_input"}}
?

-kernel
.bias
Aregularization_losses
B	variables
Ctrainable_variables
D	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "Conv2D", "name": "conv2d", "trainable": false, "expects_training_arg": false, "dtype": "float32", "batch_input_shape": [null, 100, 3, 1], "config": {"name": "conv2d", "trainable": true, "batch_input_shape": [null, 100, 3, 1], "dtype": "float32", "filters": 100, "kernel_size": [5, 1], "strides": [1, 1], "padding": "valid", "data_format": "channels_last", "dilation_rate": [1, 1], "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": 4, "max_ndim": null, "min_ndim": null, "axes": {"-1": 1}}}}
 "
trackable_list_wrapper
.
-0
.1"
trackable_list_wrapper
 "
trackable_list_wrapper
?
regularization_losses
Emetrics
Fnon_trainable_variables
	variables
Glayer_regularization_losses
trainable_variables

Hlayers
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
?
Iregularization_losses
J	variables
Ktrainable_variables
L	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "MaxPooling2D", "name": "max_pooling2d", "trainable": false, "expects_training_arg": false, "dtype": "float32", "batch_input_shape": null, "config": {"name": "max_pooling2d", "trainable": true, "dtype": "float32", "pool_size": [3, 1], "padding": "valid", "strides": [3, 1], "data_format": "channels_last"}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": 4, "max_ndim": null, "min_ndim": null, "axes": {}}}}
?
Mregularization_losses
N	variables
Otrainable_variables
P	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "Flatten", "name": "flatten", "trainable": false, "expects_training_arg": false, "dtype": "float32", "batch_input_shape": null, "config": {"name": "flatten", "trainable": true, "dtype": "float32", "data_format": "channels_last"}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": null, "max_ndim": null, "min_ndim": 1, "axes": {}}}}
?

/kernel
0bias
Qregularization_losses
R	variables
Strainable_variables
T	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "Dense", "name": "dense", "trainable": false, "expects_training_arg": false, "dtype": "float32", "batch_input_shape": null, "config": {"name": "dense", "trainable": true, "dtype": "float32", "units": 12, "activation": "linear", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": null, "max_ndim": null, "min_ndim": 2, "axes": {"-1": 9600}}}}
 "
trackable_list_wrapper
.
/0
01"
trackable_list_wrapper
 "
trackable_list_wrapper
?
regularization_losses
Umetrics
Vnon_trainable_variables
	variables
Wlayer_regularization_losses
trainable_variables

Xlayers
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
?

1kernel
2bias
Yregularization_losses
Z	variables
[trainable_variables
\	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "Dense", "name": "dense_1", "trainable": true, "expects_training_arg": false, "dtype": "float32", "batch_input_shape": null, "config": {"name": "dense_1", "trainable": true, "dtype": "float32", "units": 12, "activation": "linear", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": null, "max_ndim": null, "min_ndim": 2, "axes": {"-1": 12}}}}
?
]regularization_losses
^	variables
_trainable_variables
`	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "Dropout", "name": "dropout", "trainable": true, "expects_training_arg": true, "dtype": "float32", "batch_input_shape": null, "config": {"name": "dropout", "trainable": true, "dtype": "float32", "rate": 0.5, "noise_shape": null, "seed": null}}
 "
trackable_list_wrapper
.
10
21"
trackable_list_wrapper
.
10
21"
trackable_list_wrapper
?
regularization_losses
ametrics
bnon_trainable_variables
 	variables
clayer_regularization_losses
!trainable_variables

dlayers
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
?

3kernel
4bias
eregularization_losses
f	variables
gtrainable_variables
h	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "Dense", "name": "dense_2", "trainable": true, "expects_training_arg": false, "dtype": "float32", "batch_input_shape": null, "config": {"name": "dense_2", "trainable": true, "dtype": "float32", "units": 4, "activation": "softmax", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": null, "max_ndim": null, "min_ndim": 2, "axes": {"-1": 12}}}}
 "
trackable_list_wrapper
.
30
41"
trackable_list_wrapper
.
30
41"
trackable_list_wrapper
?
$regularization_losses
imetrics
jnon_trainable_variables
%	variables
klayer_regularization_losses
&trainable_variables

llayers
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
:	 (2	Adam/iter
: (2Adam/beta_1
: (2Adam/beta_2
: (2
Adam/decay
: (2Adam/learning_rate
':%d2conv2d/kernel
:d2conv2d/bias
:	?K2dense/kernel
:2
dense/bias
 :2dense_1/kernel
:2dense_1/bias
 :2dense_2/kernel
:2dense_2/bias
'
m0"
trackable_list_wrapper
<
-0
.1
/2
03"
trackable_list_wrapper
 "
trackable_list_wrapper
<
0
1
2
3"
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
?
=regularization_losses
nmetrics
onon_trainable_variables
>	variables
player_regularization_losses
?trainable_variables

qlayers
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
 "
trackable_list_wrapper
.
-0
.1"
trackable_list_wrapper
 "
trackable_list_wrapper
?
Aregularization_losses
rmetrics
snon_trainable_variables
B	variables
tlayer_regularization_losses
Ctrainable_variables

ulayers
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
 "
trackable_list_wrapper
.
-0
.1"
trackable_list_wrapper
 "
trackable_list_wrapper
'
0"
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
?
Iregularization_losses
vmetrics
wnon_trainable_variables
J	variables
xlayer_regularization_losses
Ktrainable_variables

ylayers
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
?
Mregularization_losses
zmetrics
{non_trainable_variables
N	variables
|layer_regularization_losses
Otrainable_variables

}layers
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
 "
trackable_list_wrapper
.
/0
01"
trackable_list_wrapper
 "
trackable_list_wrapper
?
Qregularization_losses
~metrics
non_trainable_variables
R	variables
 ?layer_regularization_losses
Strainable_variables
?layers
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
 "
trackable_list_wrapper
.
/0
01"
trackable_list_wrapper
 "
trackable_list_wrapper
5
0
1
2"
trackable_list_wrapper
 "
trackable_list_wrapper
.
10
21"
trackable_list_wrapper
.
10
21"
trackable_list_wrapper
?
Yregularization_losses
?metrics
?non_trainable_variables
Z	variables
 ?layer_regularization_losses
[trainable_variables
?layers
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
?
]regularization_losses
?metrics
?non_trainable_variables
^	variables
 ?layer_regularization_losses
_trainable_variables
?layers
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
.
0
1"
trackable_list_wrapper
 "
trackable_list_wrapper
.
30
41"
trackable_list_wrapper
.
30
41"
trackable_list_wrapper
?
eregularization_losses
?metrics
?non_trainable_variables
f	variables
 ?layer_regularization_losses
gtrainable_variables
?layers
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
'
#0"
trackable_list_wrapper
?

?total

?count
?
_fn_kwargs
?regularization_losses
?	variables
?trainable_variables
?	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "MeanMetricWrapper", "name": "acc", "trainable": true, "expects_training_arg": true, "dtype": "float32", "batch_input_shape": null, "config": {"name": "acc", "dtype": "float32"}}
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
.
-0
.1"
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
.
/0
01"
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
:  (2total
:  (2count
 "
trackable_dict_wrapper
 "
trackable_list_wrapper
0
?0
?1"
trackable_list_wrapper
 "
trackable_list_wrapper
?
?regularization_losses
?metrics
?non_trainable_variables
?	variables
 ?layer_regularization_losses
?trainable_variables
?layers
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
 "
trackable_list_wrapper
0
?0
?1"
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
%:#2Adam/dense_1/kernel/m
:2Adam/dense_1/bias/m
%:#2Adam/dense_2/kernel/m
:2Adam/dense_2/bias/m
%:#2Adam/dense_1/kernel/v
:2Adam/dense_1/bias/v
%:#2Adam/dense_2/kernel/v
:2Adam/dense_2/bias/v
?2?
 __inference__wrapped_model_10463?
???
FullArgSpec
args? 
varargsjargs
varkw
 
defaults
 

kwonlyargs? 
kwonlydefaults
 
annotations? *9?6
4?1
sequential_2_input?????????d
?2?
,__inference_sequential_5_layer_call_fn_10903
,__inference_sequential_5_layer_call_fn_10831
,__inference_sequential_5_layer_call_fn_10799
,__inference_sequential_5_layer_call_fn_10916?
???
FullArgSpec1
args)?&
jself
jinputs

jtraining
jmask
varargs
 
varkw
 
defaults?
p 

 

kwonlyargs? 
kwonlydefaults? 
annotations? *
 
?2?
G__inference_sequential_5_layer_call_and_return_conditional_losses_10890
G__inference_sequential_5_layer_call_and_return_conditional_losses_10871
G__inference_sequential_5_layer_call_and_return_conditional_losses_10768
G__inference_sequential_5_layer_call_and_return_conditional_losses_10750?
???
FullArgSpec1
args)?&
jself
jinputs

jtraining
jmask
varargs
 
varkw
 
defaults?
p 

 

kwonlyargs? 
kwonlydefaults? 
annotations? *
 
?2??
???
FullArgSpec
args?
jself
jinputs
varargs
 
varkwjkwargs
defaults? 

kwonlyargs?

jtraining%
kwonlydefaults?

trainingp 
annotations? *
 
?2??
???
FullArgSpec
args?
jself
jinputs
varargs
 
varkwjkwargs
defaults? 

kwonlyargs?

jtraining%
kwonlydefaults?

trainingp 
annotations? *
 
?2?
,__inference_sequential_2_layer_call_fn_10515
,__inference_sequential_2_layer_call_fn_10937
,__inference_sequential_2_layer_call_fn_10944
,__inference_sequential_2_layer_call_fn_10498?
???
FullArgSpec1
args)?&
jself
jinputs

jtraining
jmask
varargs
 
varkw
 
defaults?
p 

 

kwonlyargs? 
kwonlydefaults? 
annotations? *
 
?2?
G__inference_sequential_2_layer_call_and_return_conditional_losses_10923
G__inference_sequential_2_layer_call_and_return_conditional_losses_10930
G__inference_sequential_2_layer_call_and_return_conditional_losses_10473
G__inference_sequential_2_layer_call_and_return_conditional_losses_10482?
???
FullArgSpec1
args)?&
jself
jinputs

jtraining
jmask
varargs
 
varkw
 
defaults?
p 

 

kwonlyargs? 
kwonlydefaults? 
annotations? *
 
?2?
,__inference_sequential_1_layer_call_fn_10976
,__inference_sequential_1_layer_call_fn_10969
,__inference_sequential_1_layer_call_fn_10573
,__inference_sequential_1_layer_call_fn_10554?
???
FullArgSpec1
args)?&
jself
jinputs

jtraining
jmask
varargs
 
varkw
 
defaults?
p 

 

kwonlyargs? 
kwonlydefaults? 
annotations? *
 
?2?
G__inference_sequential_1_layer_call_and_return_conditional_losses_10953
G__inference_sequential_1_layer_call_and_return_conditional_losses_10962
G__inference_sequential_1_layer_call_and_return_conditional_losses_10536
G__inference_sequential_1_layer_call_and_return_conditional_losses_10525?
???
FullArgSpec1
args)?&
jself
jinputs

jtraining
jmask
varargs
 
varkw
 
defaults?
p 

 

kwonlyargs? 
kwonlydefaults? 
annotations? *
 
?2?
,__inference_sequential_3_layer_call_fn_10999
,__inference_sequential_3_layer_call_fn_11006
,__inference_sequential_3_layer_call_fn_10627
,__inference_sequential_3_layer_call_fn_10609?
???
FullArgSpec1
args)?&
jself
jinputs

jtraining
jmask
varargs
 
varkw
 
defaults?
p 

 

kwonlyargs? 
kwonlydefaults? 
annotations? *
 
?2?
G__inference_sequential_3_layer_call_and_return_conditional_losses_10992
G__inference_sequential_3_layer_call_and_return_conditional_losses_10984
G__inference_sequential_3_layer_call_and_return_conditional_losses_10592
G__inference_sequential_3_layer_call_and_return_conditional_losses_10582?
???
FullArgSpec1
args)?&
jself
jinputs

jtraining
jmask
varargs
 
varkw
 
defaults?
p 

 

kwonlyargs? 
kwonlydefaults? 
annotations? *
 
?2?
,__inference_sequential_4_layer_call_fn_11027
,__inference_sequential_4_layer_call_fn_11034
,__inference_sequential_4_layer_call_fn_10677
,__inference_sequential_4_layer_call_fn_10660?
???
FullArgSpec1
args)?&
jself
jinputs

jtraining
jmask
varargs
 
varkw
 
defaults?
p 

 

kwonlyargs? 
kwonlydefaults? 
annotations? *
 
?2?
G__inference_sequential_4_layer_call_and_return_conditional_losses_11020
G__inference_sequential_4_layer_call_and_return_conditional_losses_11013
G__inference_sequential_4_layer_call_and_return_conditional_losses_10635
G__inference_sequential_4_layer_call_and_return_conditional_losses_10644?
???
FullArgSpec1
args)?&
jself
jinputs

jtraining
jmask
varargs
 
varkw
 
defaults?
p 

 

kwonlyargs? 
kwonlydefaults? 
annotations? *
 
=B;
#__inference_signature_wrapper_10850sequential_2_input
?2??
???
FullArgSpec
args?
jself
jinputs
varargs
 
varkwjkwargs
defaults? 

kwonlyargs?

jtraining%
kwonlydefaults?

trainingp 
annotations? *
 
?2??
???
FullArgSpec
args?
jself
jinputs
varargs
 
varkwjkwargs
defaults? 

kwonlyargs?

jtraining%
kwonlydefaults?

trainingp 
annotations? *
 
?2?
%__inference_conv2d_layer_call_fn_4861?
???
FullArgSpec
args?

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs? 
kwonlydefaults
 
annotations? *7?4
2?/+???????????????????????????
?2?
@__inference_conv2d_layer_call_and_return_conditional_losses_4853?
???
FullArgSpec
args?

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs? 
kwonlydefaults
 
annotations? *7?4
2?/+???????????????????????????
?2?
,__inference_max_pooling2d_layer_call_fn_3725?
???
FullArgSpec
args?

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs? 
kwonlydefaults
 
annotations? *@?=
;?84????????????????????????????????????
?2?
G__inference_max_pooling2d_layer_call_and_return_conditional_losses_3719?
???
FullArgSpec
args?

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs? 
kwonlydefaults
 
annotations? *@?=
;?84????????????????????????????????????
?2?
&__inference_flatten_layer_call_fn_3466?
???
FullArgSpec
args?

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs? 
kwonlydefaults
 
annotations? *
 
?2?
A__inference_flatten_layer_call_and_return_conditional_losses_3422?
???
FullArgSpec
args?

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs? 
kwonlydefaults
 
annotations? *
 
?2?
$__inference_dense_layer_call_fn_3485?
???
FullArgSpec
args?

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs? 
kwonlydefaults
 
annotations? *
 
?2?
?__inference_dense_layer_call_and_return_conditional_losses_3825?
???
FullArgSpec
args?

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs? 
kwonlydefaults
 
annotations? *
 
?2?
&__inference_dense_1_layer_call_fn_6131?
???
FullArgSpec
args?

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs? 
kwonlydefaults
 
annotations? *
 
?2?
A__inference_dense_1_layer_call_and_return_conditional_losses_6030?
???
FullArgSpec
args?

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs? 
kwonlydefaults
 
annotations? *
 
?2?
&__inference_dropout_layer_call_fn_6482
&__inference_dropout_layer_call_fn_6509?
???
FullArgSpec!
args?
jinputs

jtraining
varargs
 
varkw
 
defaults?
p 

kwonlyargs? 
kwonlydefaults? 
annotations? *
 
?2?
A__inference_dropout_layer_call_and_return_conditional_losses_6320
A__inference_dropout_layer_call_and_return_conditional_losses_6391?
???
FullArgSpec!
args?
jinputs

jtraining
varargs
 
varkw
 
defaults?
p 

kwonlyargs? 
kwonlydefaults? 
annotations? *
 
?2?
&__inference_dense_2_layer_call_fn_7306?
???
FullArgSpec
args?

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs? 
kwonlydefaults
 
annotations? *
 
?2?
A__inference_dense_2_layer_call_and_return_conditional_losses_7601?
???
FullArgSpec
args?

jinputs
varargs
 
varkw
 
defaults
 

kwonlyargs? 
kwonlydefaults
 
annotations? *
 
?2??
???
FullArgSpec
args?
jself
jinputs
varargs
 
varkwjkwargs
defaults? 

kwonlyargs?

jtraining%
kwonlydefaults?

trainingp 
annotations? *
 
?2??
???
FullArgSpec
args?
jself
jinputs
varargs
 
varkwjkwargs
defaults? 

kwonlyargs?

jtraining%
kwonlydefaults?

trainingp 
annotations? *
 ?
A__inference_dropout_layer_call_and_return_conditional_losses_6320\3?0
)?&
 ?
inputs?????????
p 
? "%?"
?
0?????????
? ?
G__inference_sequential_4_layer_call_and_return_conditional_losses_10635e348?5
.?+
!?
input_1?????????
p

 
? "%?"
?
0?????????
? y
&__inference_dense_1_layer_call_fn_6131O12/?,
%?"
 ?
inputs?????????
? "???????????
G__inference_sequential_4_layer_call_and_return_conditional_losses_11020d347?4
-?*
 ?
inputs?????????
p 

 
? "%?"
?
0?????????
? ?
,__inference_sequential_3_layer_call_fn_10627X128?5
.?+
!?
input_1?????????
p 

 
? "??????????y
&__inference_dropout_layer_call_fn_6509O3?0
)?&
 ?
inputs?????????
p
? "???????????
,__inference_sequential_1_layer_call_fn_10969_/0??<
5?2
(?%
inputs?????????`d
p

 
? "???????????
,__inference_sequential_5_layer_call_fn_10799q-./01234K?H
A?>
4?1
sequential_2_input?????????d
p

 
? "???????????
A__inference_flatten_layer_call_and_return_conditional_losses_3422a7?4
-?*
(?%
inputs????????? d
? "&?#
?
0??????????K
? ?
,__inference_sequential_3_layer_call_fn_10609X128?5
.?+
!?
input_1?????????
p

 
? "???????????
,__inference_sequential_2_layer_call_fn_10944g-.??<
5?2
(?%
inputs?????????d
p 

 
? " ??????????`d?
A__inference_dense_1_layer_call_and_return_conditional_losses_6030\12/?,
%?"
 ?
inputs?????????
? "%?"
?
0?????????
? ?
%__inference_conv2d_layer_call_fn_4861?-.I?F
??<
:?7
inputs+???????????????????????????
? "2?/+???????????????????????????d?
,__inference_sequential_1_layer_call_fn_10573`/0@?=
6?3
)?&
input_1?????????`d
p 

 
? "???????????
,__inference_max_pooling2d_layer_call_fn_3725?R?O
H?E
C?@
inputs4????????????????????????????????????
? ";?84?????????????????????????????????????
G__inference_sequential_3_layer_call_and_return_conditional_losses_10992d127?4
-?*
 ?
inputs?????????
p 

 
? "%?"
?
0?????????
? ?
,__inference_sequential_4_layer_call_fn_11034W347?4
-?*
 ?
inputs?????????
p 

 
? "???????????
G__inference_sequential_2_layer_call_and_return_conditional_losses_10473z-.E?B
;?8
.?+
conv2d_input?????????d
p

 
? "-?*
#? 
0?????????`d
? ?
,__inference_sequential_5_layer_call_fn_10916e-./01234??<
5?2
(?%
inputs?????????d
p 

 
? "???????????
,__inference_sequential_2_layer_call_fn_10937g-.??<
5?2
(?%
inputs?????????d
p

 
? " ??????????`d?
G__inference_sequential_3_layer_call_and_return_conditional_losses_10984d127?4
-?*
 ?
inputs?????????
p

 
? "%?"
?
0?????????
? ?
,__inference_sequential_1_layer_call_fn_10554`/0@?=
6?3
)?&
input_1?????????`d
p

 
? "???????????
#__inference_signature_wrapper_10850?-./01234Y?V
? 
O?L
J
sequential_2_input4?1
sequential_2_input?????????d";?8
6
sequential_4&?#
sequential_4??????????
,__inference_sequential_3_layer_call_fn_10999W127?4
-?*
 ?
inputs?????????
p

 
? "???????????
G__inference_sequential_2_layer_call_and_return_conditional_losses_10923t-.??<
5?2
(?%
inputs?????????d
p

 
? "-?*
#? 
0?????????`d
? ~
&__inference_flatten_layer_call_fn_3466T7?4
-?*
(?%
inputs????????? d
? "???????????K?
G__inference_sequential_2_layer_call_and_return_conditional_losses_10482z-.E?B
;?8
.?+
conv2d_input?????????d
p 

 
? "-?*
#? 
0?????????`d
? ?
G__inference_sequential_1_layer_call_and_return_conditional_losses_10525m/0@?=
6?3
)?&
input_1?????????`d
p

 
? "%?"
?
0?????????
? ?
 __inference__wrapped_model_10463?-./01234C?@
9?6
4?1
sequential_2_input?????????d
? ";?8
6
sequential_4&?#
sequential_4??????????
G__inference_sequential_1_layer_call_and_return_conditional_losses_10962l/0??<
5?2
(?%
inputs?????????`d
p 

 
? "%?"
?
0?????????
? ?
,__inference_sequential_5_layer_call_fn_10903e-./01234??<
5?2
(?%
inputs?????????d
p

 
? "???????????
G__inference_sequential_3_layer_call_and_return_conditional_losses_10582e128?5
.?+
!?
input_1?????????
p

 
? "%?"
?
0?????????
? ?
G__inference_sequential_4_layer_call_and_return_conditional_losses_10644e348?5
.?+
!?
input_1?????????
p 

 
? "%?"
?
0?????????
? ?
,__inference_sequential_4_layer_call_fn_10660X348?5
.?+
!?
input_1?????????
p

 
? "???????????
,__inference_sequential_5_layer_call_fn_10831q-./01234K?H
A?>
4?1
sequential_2_input?????????d
p 

 
? "???????????
G__inference_sequential_5_layer_call_and_return_conditional_losses_10750~-./01234K?H
A?>
4?1
sequential_2_input?????????d
p

 
? "%?"
?
0?????????
? ?
,__inference_sequential_4_layer_call_fn_10677X348?5
.?+
!?
input_1?????????
p 

 
? "??????????y
&__inference_dropout_layer_call_fn_6482O3?0
)?&
 ?
inputs?????????
p 
? "???????????
@__inference_conv2d_layer_call_and_return_conditional_losses_4853?-.I?F
??<
:?7
inputs+???????????????????????????
? "??<
5?2
0+???????????????????????????d
? ?
,__inference_sequential_2_layer_call_fn_10498m-.E?B
;?8
.?+
conv2d_input?????????d
p

 
? " ??????????`dy
&__inference_dense_2_layer_call_fn_7306O34/?,
%?"
 ?
inputs?????????
? "???????????
G__inference_sequential_5_layer_call_and_return_conditional_losses_10871r-./01234??<
5?2
(?%
inputs?????????d
p

 
? "%?"
?
0?????????
? ?
G__inference_sequential_4_layer_call_and_return_conditional_losses_11013d347?4
-?*
 ?
inputs?????????
p

 
? "%?"
?
0?????????
? ?
G__inference_max_pooling2d_layer_call_and_return_conditional_losses_3719?R?O
H?E
C?@
inputs4????????????????????????????????????
? "H?E
>?;
04????????????????????????????????????
? ?
G__inference_sequential_3_layer_call_and_return_conditional_losses_10592e128?5
.?+
!?
input_1?????????
p 

 
? "%?"
?
0?????????
? ?
A__inference_dense_2_layer_call_and_return_conditional_losses_7601\34/?,
%?"
 ?
inputs?????????
? "%?"
?
0?????????
? ?
G__inference_sequential_1_layer_call_and_return_conditional_losses_10536m/0@?=
6?3
)?&
input_1?????????`d
p 

 
? "%?"
?
0?????????
? ?
G__inference_sequential_5_layer_call_and_return_conditional_losses_10890r-./01234??<
5?2
(?%
inputs?????????d
p 

 
? "%?"
?
0?????????
? ?
,__inference_sequential_2_layer_call_fn_10515m-.E?B
;?8
.?+
conv2d_input?????????d
p 

 
? " ??????????`d?
G__inference_sequential_2_layer_call_and_return_conditional_losses_10930t-.??<
5?2
(?%
inputs?????????d
p 

 
? "-?*
#? 
0?????????`d
? ?
,__inference_sequential_3_layer_call_fn_11006W127?4
-?*
 ?
inputs?????????
p 

 
? "???????????
A__inference_dropout_layer_call_and_return_conditional_losses_6391\3?0
)?&
 ?
inputs?????????
p
? "%?"
?
0?????????
? ?
,__inference_sequential_4_layer_call_fn_11027W347?4
-?*
 ?
inputs?????????
p

 
? "???????????
G__inference_sequential_5_layer_call_and_return_conditional_losses_10768~-./01234K?H
A?>
4?1
sequential_2_input?????????d
p 

 
? "%?"
?
0?????????
? x
$__inference_dense_layer_call_fn_3485P/00?-
&?#
!?
inputs??????????K
? "???????????
?__inference_dense_layer_call_and_return_conditional_losses_3825]/00?-
&?#
!?
inputs??????????K
? "%?"
?
0?????????
? ?
G__inference_sequential_1_layer_call_and_return_conditional_losses_10953l/0??<
5?2
(?%
inputs?????????`d
p

 
? "%?"
?
0?????????
? ?
,__inference_sequential_1_layer_call_fn_10976_/0??<
5?2
(?%
inputs?????????`d
p 

 
? "??????????