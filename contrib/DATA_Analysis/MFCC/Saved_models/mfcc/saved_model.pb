??
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
shapeshape?"serve*2.0.02unknown8??
f
	Adam/iterVarHandleOp*
shape: *
_output_shapes
: *
shared_name	Adam/iter*
dtype0	
_
Adam/iter/Read/ReadVariableOpReadVariableOp	Adam/iter*
dtype0	*
_output_shapes
: 
j
Adam/beta_1VarHandleOp*
dtype0*
_output_shapes
: *
shape: *
shared_nameAdam/beta_1
c
Adam/beta_1/Read/ReadVariableOpReadVariableOpAdam/beta_1*
_output_shapes
: *
dtype0
j
Adam/beta_2VarHandleOp*
shape: *
dtype0*
shared_nameAdam/beta_2*
_output_shapes
: 
c
Adam/beta_2/Read/ReadVariableOpReadVariableOpAdam/beta_2*
dtype0*
_output_shapes
: 
h

Adam/decayVarHandleOp*
dtype0*
shared_name
Adam/decay*
shape: *
_output_shapes
: 
a
Adam/decay/Read/ReadVariableOpReadVariableOp
Adam/decay*
dtype0*
_output_shapes
: 
x
Adam/learning_rateVarHandleOp*
_output_shapes
: *
dtype0*
shape: *#
shared_nameAdam/learning_rate
q
&Adam/learning_rate/Read/ReadVariableOpReadVariableOpAdam/learning_rate*
_output_shapes
: *
dtype0
z
dense_8/kernelVarHandleOp*
_output_shapes
: *
shared_namedense_8/kernel*
dtype0*
shape:
??
s
"dense_8/kernel/Read/ReadVariableOpReadVariableOpdense_8/kernel*
dtype0* 
_output_shapes
:
??
q
dense_8/biasVarHandleOp*
_output_shapes
: *
dtype0*
shape:?*
shared_namedense_8/bias
j
 dense_8/bias/Read/ReadVariableOpReadVariableOpdense_8/bias*
dtype0*
_output_shapes	
:?
~
dense_1_2/kernelVarHandleOp*
dtype0*!
shared_namedense_1_2/kernel*
_output_shapes
: *
shape:
??
w
$dense_1_2/kernel/Read/ReadVariableOpReadVariableOpdense_1_2/kernel*
dtype0* 
_output_shapes
:
??
u
dense_1_2/biasVarHandleOp*
shape:?*
dtype0*
_output_shapes
: *
shared_namedense_1_2/bias
n
"dense_1_2/bias/Read/ReadVariableOpReadVariableOpdense_1_2/bias*
_output_shapes	
:?*
dtype0
}
dense_2_2/kernelVarHandleOp*
shape:	?8*!
shared_namedense_2_2/kernel*
dtype0*
_output_shapes
: 
v
$dense_2_2/kernel/Read/ReadVariableOpReadVariableOpdense_2_2/kernel*
dtype0*
_output_shapes
:	?8
t
dense_2_2/biasVarHandleOp*
_output_shapes
: *
dtype0*
shared_namedense_2_2/bias*
shape:8
m
"dense_2_2/bias/Read/ReadVariableOpReadVariableOpdense_2_2/bias*
_output_shapes
:8*
dtype0
|
dense_3_2/kernelVarHandleOp*
shape
:88*
dtype0*
_output_shapes
: *!
shared_namedense_3_2/kernel
u
$dense_3_2/kernel/Read/ReadVariableOpReadVariableOpdense_3_2/kernel*
_output_shapes

:88*
dtype0
t
dense_3_2/biasVarHandleOp*
shared_namedense_3_2/bias*
dtype0*
shape:8*
_output_shapes
: 
m
"dense_3_2/bias/Read/ReadVariableOpReadVariableOpdense_3_2/bias*
_output_shapes
:8*
dtype0
|
dense_4_2/kernelVarHandleOp*
_output_shapes
: *
shape
:88*!
shared_namedense_4_2/kernel*
dtype0
u
$dense_4_2/kernel/Read/ReadVariableOpReadVariableOpdense_4_2/kernel*
dtype0*
_output_shapes

:88
t
dense_4_2/biasVarHandleOp*
shape:8*
dtype0*
shared_namedense_4_2/bias*
_output_shapes
: 
m
"dense_4_2/bias/Read/ReadVariableOpReadVariableOpdense_4_2/bias*
dtype0*
_output_shapes
:8
|
dense_5_2/kernelVarHandleOp*
_output_shapes
: *
shape
:88*
dtype0*!
shared_namedense_5_2/kernel
u
$dense_5_2/kernel/Read/ReadVariableOpReadVariableOpdense_5_2/kernel*
_output_shapes

:88*
dtype0
t
dense_5_2/biasVarHandleOp*
dtype0*
shared_namedense_5_2/bias*
_output_shapes
: *
shape:8
m
"dense_5_2/bias/Read/ReadVariableOpReadVariableOpdense_5_2/bias*
_output_shapes
:8*
dtype0
|
dense_6_3/kernelVarHandleOp*
shape
:8*
_output_shapes
: *
dtype0*!
shared_namedense_6_3/kernel
u
$dense_6_3/kernel/Read/ReadVariableOpReadVariableOpdense_6_3/kernel*
dtype0*
_output_shapes

:8
t
dense_6_3/biasVarHandleOp*
shared_namedense_6_3/bias*
dtype0*
shape:*
_output_shapes
: 
m
"dense_6_3/bias/Read/ReadVariableOpReadVariableOpdense_6_3/bias*
_output_shapes
:*
dtype0
^
totalVarHandleOp*
_output_shapes
: *
dtype0*
shape: *
shared_nametotal
W
total/Read/ReadVariableOpReadVariableOptotal*
_output_shapes
: *
dtype0
^
countVarHandleOp*
_output_shapes
: *
shape: *
shared_namecount*
dtype0
W
count/Read/ReadVariableOpReadVariableOpcount*
dtype0*
_output_shapes
: 
?
Adam/dense_8/kernel/mVarHandleOp*&
shared_nameAdam/dense_8/kernel/m*
_output_shapes
: *
shape:
??*
dtype0
?
)Adam/dense_8/kernel/m/Read/ReadVariableOpReadVariableOpAdam/dense_8/kernel/m* 
_output_shapes
:
??*
dtype0

Adam/dense_8/bias/mVarHandleOp*
_output_shapes
: *
shape:?*$
shared_nameAdam/dense_8/bias/m*
dtype0
x
'Adam/dense_8/bias/m/Read/ReadVariableOpReadVariableOpAdam/dense_8/bias/m*
_output_shapes	
:?*
dtype0
?
Adam/dense_1_2/kernel/mVarHandleOp*
shape:
??*(
shared_nameAdam/dense_1_2/kernel/m*
_output_shapes
: *
dtype0
?
+Adam/dense_1_2/kernel/m/Read/ReadVariableOpReadVariableOpAdam/dense_1_2/kernel/m* 
_output_shapes
:
??*
dtype0
?
Adam/dense_1_2/bias/mVarHandleOp*
shape:?*
_output_shapes
: *
dtype0*&
shared_nameAdam/dense_1_2/bias/m
|
)Adam/dense_1_2/bias/m/Read/ReadVariableOpReadVariableOpAdam/dense_1_2/bias/m*
_output_shapes	
:?*
dtype0
?
Adam/dense_2_2/kernel/mVarHandleOp*(
shared_nameAdam/dense_2_2/kernel/m*
dtype0*
_output_shapes
: *
shape:	?8
?
+Adam/dense_2_2/kernel/m/Read/ReadVariableOpReadVariableOpAdam/dense_2_2/kernel/m*
_output_shapes
:	?8*
dtype0
?
Adam/dense_2_2/bias/mVarHandleOp*
dtype0*
shape:8*&
shared_nameAdam/dense_2_2/bias/m*
_output_shapes
: 
{
)Adam/dense_2_2/bias/m/Read/ReadVariableOpReadVariableOpAdam/dense_2_2/bias/m*
_output_shapes
:8*
dtype0
?
Adam/dense_3_2/kernel/mVarHandleOp*(
shared_nameAdam/dense_3_2/kernel/m*
shape
:88*
_output_shapes
: *
dtype0
?
+Adam/dense_3_2/kernel/m/Read/ReadVariableOpReadVariableOpAdam/dense_3_2/kernel/m*
_output_shapes

:88*
dtype0
?
Adam/dense_3_2/bias/mVarHandleOp*&
shared_nameAdam/dense_3_2/bias/m*
_output_shapes
: *
shape:8*
dtype0
{
)Adam/dense_3_2/bias/m/Read/ReadVariableOpReadVariableOpAdam/dense_3_2/bias/m*
dtype0*
_output_shapes
:8
?
Adam/dense_4_2/kernel/mVarHandleOp*
dtype0*
_output_shapes
: *
shape
:88*(
shared_nameAdam/dense_4_2/kernel/m
?
+Adam/dense_4_2/kernel/m/Read/ReadVariableOpReadVariableOpAdam/dense_4_2/kernel/m*
_output_shapes

:88*
dtype0
?
Adam/dense_4_2/bias/mVarHandleOp*&
shared_nameAdam/dense_4_2/bias/m*
dtype0*
_output_shapes
: *
shape:8
{
)Adam/dense_4_2/bias/m/Read/ReadVariableOpReadVariableOpAdam/dense_4_2/bias/m*
dtype0*
_output_shapes
:8
?
Adam/dense_5_2/kernel/mVarHandleOp*(
shared_nameAdam/dense_5_2/kernel/m*
dtype0*
shape
:88*
_output_shapes
: 
?
+Adam/dense_5_2/kernel/m/Read/ReadVariableOpReadVariableOpAdam/dense_5_2/kernel/m*
_output_shapes

:88*
dtype0
?
Adam/dense_5_2/bias/mVarHandleOp*
shape:8*&
shared_nameAdam/dense_5_2/bias/m*
_output_shapes
: *
dtype0
{
)Adam/dense_5_2/bias/m/Read/ReadVariableOpReadVariableOpAdam/dense_5_2/bias/m*
_output_shapes
:8*
dtype0
?
Adam/dense_6_3/kernel/mVarHandleOp*
shape
:8*
_output_shapes
: *(
shared_nameAdam/dense_6_3/kernel/m*
dtype0
?
+Adam/dense_6_3/kernel/m/Read/ReadVariableOpReadVariableOpAdam/dense_6_3/kernel/m*
_output_shapes

:8*
dtype0
?
Adam/dense_6_3/bias/mVarHandleOp*
dtype0*
_output_shapes
: *
shape:*&
shared_nameAdam/dense_6_3/bias/m
{
)Adam/dense_6_3/bias/m/Read/ReadVariableOpReadVariableOpAdam/dense_6_3/bias/m*
_output_shapes
:*
dtype0
?
Adam/dense_8/kernel/vVarHandleOp*
dtype0*
_output_shapes
: *
shape:
??*&
shared_nameAdam/dense_8/kernel/v
?
)Adam/dense_8/kernel/v/Read/ReadVariableOpReadVariableOpAdam/dense_8/kernel/v*
dtype0* 
_output_shapes
:
??

Adam/dense_8/bias/vVarHandleOp*$
shared_nameAdam/dense_8/bias/v*
_output_shapes
: *
shape:?*
dtype0
x
'Adam/dense_8/bias/v/Read/ReadVariableOpReadVariableOpAdam/dense_8/bias/v*
_output_shapes	
:?*
dtype0
?
Adam/dense_1_2/kernel/vVarHandleOp*
shape:
??*
dtype0*
_output_shapes
: *(
shared_nameAdam/dense_1_2/kernel/v
?
+Adam/dense_1_2/kernel/v/Read/ReadVariableOpReadVariableOpAdam/dense_1_2/kernel/v* 
_output_shapes
:
??*
dtype0
?
Adam/dense_1_2/bias/vVarHandleOp*
shape:?*
dtype0*&
shared_nameAdam/dense_1_2/bias/v*
_output_shapes
: 
|
)Adam/dense_1_2/bias/v/Read/ReadVariableOpReadVariableOpAdam/dense_1_2/bias/v*
dtype0*
_output_shapes	
:?
?
Adam/dense_2_2/kernel/vVarHandleOp*
dtype0*
_output_shapes
: *(
shared_nameAdam/dense_2_2/kernel/v*
shape:	?8
?
+Adam/dense_2_2/kernel/v/Read/ReadVariableOpReadVariableOpAdam/dense_2_2/kernel/v*
dtype0*
_output_shapes
:	?8
?
Adam/dense_2_2/bias/vVarHandleOp*&
shared_nameAdam/dense_2_2/bias/v*
shape:8*
dtype0*
_output_shapes
: 
{
)Adam/dense_2_2/bias/v/Read/ReadVariableOpReadVariableOpAdam/dense_2_2/bias/v*
_output_shapes
:8*
dtype0
?
Adam/dense_3_2/kernel/vVarHandleOp*
shape
:88*
_output_shapes
: *
dtype0*(
shared_nameAdam/dense_3_2/kernel/v
?
+Adam/dense_3_2/kernel/v/Read/ReadVariableOpReadVariableOpAdam/dense_3_2/kernel/v*
_output_shapes

:88*
dtype0
?
Adam/dense_3_2/bias/vVarHandleOp*
shape:8*
_output_shapes
: *&
shared_nameAdam/dense_3_2/bias/v*
dtype0
{
)Adam/dense_3_2/bias/v/Read/ReadVariableOpReadVariableOpAdam/dense_3_2/bias/v*
dtype0*
_output_shapes
:8
?
Adam/dense_4_2/kernel/vVarHandleOp*
shape
:88*
dtype0*
_output_shapes
: *(
shared_nameAdam/dense_4_2/kernel/v
?
+Adam/dense_4_2/kernel/v/Read/ReadVariableOpReadVariableOpAdam/dense_4_2/kernel/v*
_output_shapes

:88*
dtype0
?
Adam/dense_4_2/bias/vVarHandleOp*
dtype0*&
shared_nameAdam/dense_4_2/bias/v*
shape:8*
_output_shapes
: 
{
)Adam/dense_4_2/bias/v/Read/ReadVariableOpReadVariableOpAdam/dense_4_2/bias/v*
_output_shapes
:8*
dtype0
?
Adam/dense_5_2/kernel/vVarHandleOp*
dtype0*(
shared_nameAdam/dense_5_2/kernel/v*
shape
:88*
_output_shapes
: 
?
+Adam/dense_5_2/kernel/v/Read/ReadVariableOpReadVariableOpAdam/dense_5_2/kernel/v*
dtype0*
_output_shapes

:88
?
Adam/dense_5_2/bias/vVarHandleOp*
shape:8*&
shared_nameAdam/dense_5_2/bias/v*
dtype0*
_output_shapes
: 
{
)Adam/dense_5_2/bias/v/Read/ReadVariableOpReadVariableOpAdam/dense_5_2/bias/v*
dtype0*
_output_shapes
:8
?
Adam/dense_6_3/kernel/vVarHandleOp*(
shared_nameAdam/dense_6_3/kernel/v*
_output_shapes
: *
dtype0*
shape
:8
?
+Adam/dense_6_3/kernel/v/Read/ReadVariableOpReadVariableOpAdam/dense_6_3/kernel/v*
dtype0*
_output_shapes

:8
?
Adam/dense_6_3/bias/vVarHandleOp*
dtype0*
_output_shapes
: *&
shared_nameAdam/dense_6_3/bias/v*
shape:
{
)Adam/dense_6_3/bias/v/Read/ReadVariableOpReadVariableOpAdam/dense_6_3/bias/v*
_output_shapes
:*
dtype0

NoOpNoOp
?U
ConstConst"/device:CPU:0*
dtype0*?U
value?UB?U B?U
?
layer-0
layer-1
layer-2
layer_with_weights-0
layer-3
layer_with_weights-1
layer-4
	optimizer
	variables
trainable_variables
	regularization_losses

	keras_api

signatures
R
	variables
trainable_variables
regularization_losses
	keras_api
l
layer-0
layer-1
	variables
trainable_variables
regularization_losses
	keras_api
R
	variables
trainable_variables
regularization_losses
	keras_api
?
layer_with_weights-0
layer-0
layer_with_weights-1
layer-1
layer_with_weights-2
layer-2
layer_with_weights-3
layer-3
layer_with_weights-4
layer-4
layer_with_weights-5
layer-5
 layer-6
!	variables
"trainable_variables
#regularization_losses
$	keras_api
y
%layer_with_weights-0
%layer-0
&	variables
'trainable_variables
(regularization_losses
)	keras_api
?
*iter

+beta_1

,beta_2
	-decay
.learning_rate/m?0m?1m?2m?3m?4m?5m?6m?7m?8m?9m?:m?;m?<m?/v?0v?1v?2v?3v?4v?5v?6v?7v?8v?9v?:v?;v?<v?
f
/0
01
12
23
34
45
56
67
78
89
910
:11
;12
<13
f
/0
01
12
23
34
45
56
67
78
89
910
:11
;12
<13
 
?
=non_trainable_variables
>layer_regularization_losses
	variables
?metrics

@layers
trainable_variables
	regularization_losses
 
 
 
 
?
Anon_trainable_variables
Blayer_regularization_losses
	variables
Cmetrics

Dlayers
trainable_variables
regularization_losses
R
E	variables
Ftrainable_variables
Gregularization_losses
H	keras_api
R
I	variables
Jtrainable_variables
Kregularization_losses
L	keras_api
 
 
 
?
Mnon_trainable_variables
Nlayer_regularization_losses
	variables
Ometrics

Players
trainable_variables
regularization_losses
 
 
 
?
Qnon_trainable_variables
Rlayer_regularization_losses
	variables
Smetrics

Tlayers
trainable_variables
regularization_losses
h

/kernel
0bias
U	variables
Vtrainable_variables
Wregularization_losses
X	keras_api
h

1kernel
2bias
Y	variables
Ztrainable_variables
[regularization_losses
\	keras_api
h

3kernel
4bias
]	variables
^trainable_variables
_regularization_losses
`	keras_api
h

5kernel
6bias
a	variables
btrainable_variables
cregularization_losses
d	keras_api
h

7kernel
8bias
e	variables
ftrainable_variables
gregularization_losses
h	keras_api
h

9kernel
:bias
i	variables
jtrainable_variables
kregularization_losses
l	keras_api
R
m	variables
ntrainable_variables
oregularization_losses
p	keras_api
V
/0
01
12
23
34
45
56
67
78
89
910
:11
V
/0
01
12
23
34
45
56
67
78
89
910
:11
 
?
qnon_trainable_variables
rlayer_regularization_losses
!	variables
smetrics

tlayers
"trainable_variables
#regularization_losses
h

;kernel
<bias
u	variables
vtrainable_variables
wregularization_losses
x	keras_api

;0
<1

;0
<1
 
?
ynon_trainable_variables
zlayer_regularization_losses
&	variables
{metrics

|layers
'trainable_variables
(regularization_losses
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
JH
VARIABLE_VALUEdense_8/kernel&variables/0/.ATTRIBUTES/VARIABLE_VALUE
HF
VARIABLE_VALUEdense_8/bias&variables/1/.ATTRIBUTES/VARIABLE_VALUE
LJ
VARIABLE_VALUEdense_1_2/kernel&variables/2/.ATTRIBUTES/VARIABLE_VALUE
JH
VARIABLE_VALUEdense_1_2/bias&variables/3/.ATTRIBUTES/VARIABLE_VALUE
LJ
VARIABLE_VALUEdense_2_2/kernel&variables/4/.ATTRIBUTES/VARIABLE_VALUE
JH
VARIABLE_VALUEdense_2_2/bias&variables/5/.ATTRIBUTES/VARIABLE_VALUE
LJ
VARIABLE_VALUEdense_3_2/kernel&variables/6/.ATTRIBUTES/VARIABLE_VALUE
JH
VARIABLE_VALUEdense_3_2/bias&variables/7/.ATTRIBUTES/VARIABLE_VALUE
LJ
VARIABLE_VALUEdense_4_2/kernel&variables/8/.ATTRIBUTES/VARIABLE_VALUE
JH
VARIABLE_VALUEdense_4_2/bias&variables/9/.ATTRIBUTES/VARIABLE_VALUE
MK
VARIABLE_VALUEdense_5_2/kernel'variables/10/.ATTRIBUTES/VARIABLE_VALUE
KI
VARIABLE_VALUEdense_5_2/bias'variables/11/.ATTRIBUTES/VARIABLE_VALUE
MK
VARIABLE_VALUEdense_6_3/kernel'variables/12/.ATTRIBUTES/VARIABLE_VALUE
KI
VARIABLE_VALUEdense_6_3/bias'variables/13/.ATTRIBUTES/VARIABLE_VALUE
 
 

}0

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
~non_trainable_variables
layer_regularization_losses
E	variables
?metrics
?layers
Ftrainable_variables
Gregularization_losses
 
 
 
?
?non_trainable_variables
 ?layer_regularization_losses
I	variables
?metrics
?layers
Jtrainable_variables
Kregularization_losses
 
 
 

0
 
 
 
 

/0
01

/0
01
 
?
?non_trainable_variables
 ?layer_regularization_losses
U	variables
?metrics
?layers
Vtrainable_variables
Wregularization_losses

10
21

10
21
 
?
?non_trainable_variables
 ?layer_regularization_losses
Y	variables
?metrics
?layers
Ztrainable_variables
[regularization_losses

30
41

30
41
 
?
?non_trainable_variables
 ?layer_regularization_losses
]	variables
?metrics
?layers
^trainable_variables
_regularization_losses

50
61

50
61
 
?
?non_trainable_variables
 ?layer_regularization_losses
a	variables
?metrics
?layers
btrainable_variables
cregularization_losses

70
81

70
81
 
?
?non_trainable_variables
 ?layer_regularization_losses
e	variables
?metrics
?layers
ftrainable_variables
gregularization_losses

90
:1

90
:1
 
?
?non_trainable_variables
 ?layer_regularization_losses
i	variables
?metrics
?layers
jtrainable_variables
kregularization_losses
 
 
 
?
?non_trainable_variables
 ?layer_regularization_losses
m	variables
?metrics
?layers
ntrainable_variables
oregularization_losses
 
 
 
1
0
1
2
3
4
5
 6

;0
<1

;0
<1
 
?
?non_trainable_variables
 ?layer_regularization_losses
u	variables
?metrics
?layers
vtrainable_variables
wregularization_losses
 
 
 

%0


?total

?count
?
_fn_kwargs
?	variables
?trainable_variables
?regularization_losses
?	keras_api
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

?0
?1
 
 
?
?non_trainable_variables
 ?layer_regularization_losses
?	variables
?metrics
?layers
?trainable_variables
?regularization_losses

?0
?1
 
 
 
mk
VARIABLE_VALUEAdam/dense_8/kernel/mBvariables/0/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUE
ki
VARIABLE_VALUEAdam/dense_8/bias/mBvariables/1/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUE
om
VARIABLE_VALUEAdam/dense_1_2/kernel/mBvariables/2/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUE
mk
VARIABLE_VALUEAdam/dense_1_2/bias/mBvariables/3/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUE
om
VARIABLE_VALUEAdam/dense_2_2/kernel/mBvariables/4/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUE
mk
VARIABLE_VALUEAdam/dense_2_2/bias/mBvariables/5/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUE
om
VARIABLE_VALUEAdam/dense_3_2/kernel/mBvariables/6/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUE
mk
VARIABLE_VALUEAdam/dense_3_2/bias/mBvariables/7/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUE
om
VARIABLE_VALUEAdam/dense_4_2/kernel/mBvariables/8/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUE
mk
VARIABLE_VALUEAdam/dense_4_2/bias/mBvariables/9/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUE
pn
VARIABLE_VALUEAdam/dense_5_2/kernel/mCvariables/10/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUE
nl
VARIABLE_VALUEAdam/dense_5_2/bias/mCvariables/11/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUE
pn
VARIABLE_VALUEAdam/dense_6_3/kernel/mCvariables/12/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUE
nl
VARIABLE_VALUEAdam/dense_6_3/bias/mCvariables/13/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUE
mk
VARIABLE_VALUEAdam/dense_8/kernel/vBvariables/0/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE
ki
VARIABLE_VALUEAdam/dense_8/bias/vBvariables/1/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE
om
VARIABLE_VALUEAdam/dense_1_2/kernel/vBvariables/2/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE
mk
VARIABLE_VALUEAdam/dense_1_2/bias/vBvariables/3/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE
om
VARIABLE_VALUEAdam/dense_2_2/kernel/vBvariables/4/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE
mk
VARIABLE_VALUEAdam/dense_2_2/bias/vBvariables/5/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE
om
VARIABLE_VALUEAdam/dense_3_2/kernel/vBvariables/6/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE
mk
VARIABLE_VALUEAdam/dense_3_2/bias/vBvariables/7/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE
om
VARIABLE_VALUEAdam/dense_4_2/kernel/vBvariables/8/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE
mk
VARIABLE_VALUEAdam/dense_4_2/bias/vBvariables/9/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE
pn
VARIABLE_VALUEAdam/dense_5_2/kernel/vCvariables/10/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE
nl
VARIABLE_VALUEAdam/dense_5_2/bias/vCvariables/11/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE
pn
VARIABLE_VALUEAdam/dense_6_3/kernel/vCvariables/12/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE
nl
VARIABLE_VALUEAdam/dense_6_3/bias/vCvariables/13/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE*
_output_shapes
: 
?
"serving_default_sequential_1_inputPlaceholder*
shape:??????????*
dtype0*(
_output_shapes
:??????????
?
StatefulPartitionedCallStatefulPartitionedCall"serving_default_sequential_1_inputdense_8/kerneldense_8/biasdense_1_2/kerneldense_1_2/biasdense_2_2/kerneldense_2_2/biasdense_3_2/kerneldense_3_2/biasdense_4_2/kerneldense_4_2/biasdense_5_2/kerneldense_5_2/biasdense_6_3/kerneldense_6_3/bias*
Tin
2**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-64387*,
f'R%
#__inference_signature_wrapper_63727*
Tout
2*'
_output_shapes
:?????????
O
saver_filenamePlaceholder*
dtype0*
_output_shapes
: *
shape: 
?
StatefulPartitionedCall_1StatefulPartitionedCallsaver_filenameAdam/iter/Read/ReadVariableOpAdam/beta_1/Read/ReadVariableOpAdam/beta_2/Read/ReadVariableOpAdam/decay/Read/ReadVariableOp&Adam/learning_rate/Read/ReadVariableOp"dense_8/kernel/Read/ReadVariableOp dense_8/bias/Read/ReadVariableOp$dense_1_2/kernel/Read/ReadVariableOp"dense_1_2/bias/Read/ReadVariableOp$dense_2_2/kernel/Read/ReadVariableOp"dense_2_2/bias/Read/ReadVariableOp$dense_3_2/kernel/Read/ReadVariableOp"dense_3_2/bias/Read/ReadVariableOp$dense_4_2/kernel/Read/ReadVariableOp"dense_4_2/bias/Read/ReadVariableOp$dense_5_2/kernel/Read/ReadVariableOp"dense_5_2/bias/Read/ReadVariableOp$dense_6_3/kernel/Read/ReadVariableOp"dense_6_3/bias/Read/ReadVariableOptotal/Read/ReadVariableOpcount/Read/ReadVariableOp)Adam/dense_8/kernel/m/Read/ReadVariableOp'Adam/dense_8/bias/m/Read/ReadVariableOp+Adam/dense_1_2/kernel/m/Read/ReadVariableOp)Adam/dense_1_2/bias/m/Read/ReadVariableOp+Adam/dense_2_2/kernel/m/Read/ReadVariableOp)Adam/dense_2_2/bias/m/Read/ReadVariableOp+Adam/dense_3_2/kernel/m/Read/ReadVariableOp)Adam/dense_3_2/bias/m/Read/ReadVariableOp+Adam/dense_4_2/kernel/m/Read/ReadVariableOp)Adam/dense_4_2/bias/m/Read/ReadVariableOp+Adam/dense_5_2/kernel/m/Read/ReadVariableOp)Adam/dense_5_2/bias/m/Read/ReadVariableOp+Adam/dense_6_3/kernel/m/Read/ReadVariableOp)Adam/dense_6_3/bias/m/Read/ReadVariableOp)Adam/dense_8/kernel/v/Read/ReadVariableOp'Adam/dense_8/bias/v/Read/ReadVariableOp+Adam/dense_1_2/kernel/v/Read/ReadVariableOp)Adam/dense_1_2/bias/v/Read/ReadVariableOp+Adam/dense_2_2/kernel/v/Read/ReadVariableOp)Adam/dense_2_2/bias/v/Read/ReadVariableOp+Adam/dense_3_2/kernel/v/Read/ReadVariableOp)Adam/dense_3_2/bias/v/Read/ReadVariableOp+Adam/dense_4_2/kernel/v/Read/ReadVariableOp)Adam/dense_4_2/bias/v/Read/ReadVariableOp+Adam/dense_5_2/kernel/v/Read/ReadVariableOp)Adam/dense_5_2/bias/v/Read/ReadVariableOp+Adam/dense_6_3/kernel/v/Read/ReadVariableOp)Adam/dense_6_3/bias/v/Read/ReadVariableOpConst*>
Tin7
523	**
config_proto

GPU 

CPU2J 8*
Tout
2*'
f"R 
__inference__traced_save_64457*,
_gradient_op_typePartitionedCall-64458*
_output_shapes
: 
?

StatefulPartitionedCall_2StatefulPartitionedCallsaver_filename	Adam/iterAdam/beta_1Adam/beta_2
Adam/decayAdam/learning_ratedense_8/kerneldense_8/biasdense_1_2/kerneldense_1_2/biasdense_2_2/kerneldense_2_2/biasdense_3_2/kerneldense_3_2/biasdense_4_2/kerneldense_4_2/biasdense_5_2/kerneldense_5_2/biasdense_6_3/kerneldense_6_3/biastotalcountAdam/dense_8/kernel/mAdam/dense_8/bias/mAdam/dense_1_2/kernel/mAdam/dense_1_2/bias/mAdam/dense_2_2/kernel/mAdam/dense_2_2/bias/mAdam/dense_3_2/kernel/mAdam/dense_3_2/bias/mAdam/dense_4_2/kernel/mAdam/dense_4_2/bias/mAdam/dense_5_2/kernel/mAdam/dense_5_2/bias/mAdam/dense_6_3/kernel/mAdam/dense_6_3/bias/mAdam/dense_8/kernel/vAdam/dense_8/bias/vAdam/dense_1_2/kernel/vAdam/dense_1_2/bias/vAdam/dense_2_2/kernel/vAdam/dense_2_2/bias/vAdam/dense_3_2/kernel/vAdam/dense_3_2/bias/vAdam/dense_4_2/kernel/vAdam/dense_4_2/bias/vAdam/dense_5_2/kernel/vAdam/dense_5_2/bias/vAdam/dense_6_3/kernel/vAdam/dense_6_3/bias/v**
f%R#
!__inference__traced_restore_64617*,
_gradient_op_typePartitionedCall-64618*
_output_shapes
: *=
Tin6
422**
config_proto

GPU 

CPU2J 8*
Tout
2??
?
c
G__inference_sequential_1_layer_call_and_return_conditional_losses_63060

inputs
identity?
flatten/PartitionedCallPartitionedCallinputs*
Tin
2*
Tout
2*,
_gradient_op_typePartitionedCall-63024*(
_output_shapes
:??????????**
config_proto

GPU 

CPU2J 8*K
fFRD
B__inference_flatten_layer_call_and_return_conditional_losses_63018i
IdentityIdentity flatten/PartitionedCall:output:0*(
_output_shapes
:??????????*
T0"
identityIdentity:output:0*'
_input_shapes
:??????????:& "
 
_user_specified_nameinputs
?
c
G__inference_sequential_1_layer_call_and_return_conditional_losses_63906

inputs
identityf
flatten/Reshape/shapeConst*
dtype0*
valueB"?????   *
_output_shapes
:u
flatten/ReshapeReshapeinputsflatten/Reshape/shape:output:0*(
_output_shapes
:??????????*
T0a
IdentityIdentityflatten/Reshape:output:0*(
_output_shapes
:??????????*
T0"
identityIdentity:output:0*'
_input_shapes
:??????????:& "
 
_user_specified_nameinputs
?
?
,__inference_sequential_4_layer_call_fn_63658
sequential_1_input"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2"
statefulpartitionedcall_args_3"
statefulpartitionedcall_args_4"
statefulpartitionedcall_args_5"
statefulpartitionedcall_args_6"
statefulpartitionedcall_args_7"
statefulpartitionedcall_args_8"
statefulpartitionedcall_args_9#
statefulpartitionedcall_args_10#
statefulpartitionedcall_args_11#
statefulpartitionedcall_args_12#
statefulpartitionedcall_args_13#
statefulpartitionedcall_args_14
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallsequential_1_inputstatefulpartitionedcall_args_1statefulpartitionedcall_args_2statefulpartitionedcall_args_3statefulpartitionedcall_args_4statefulpartitionedcall_args_5statefulpartitionedcall_args_6statefulpartitionedcall_args_7statefulpartitionedcall_args_8statefulpartitionedcall_args_9statefulpartitionedcall_args_10statefulpartitionedcall_args_11statefulpartitionedcall_args_12statefulpartitionedcall_args_13statefulpartitionedcall_args_14*'
_output_shapes
:?????????*
Tin
2*P
fKRI
G__inference_sequential_4_layer_call_and_return_conditional_losses_63640*,
_gradient_op_typePartitionedCall-63641**
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
:?????????"
identityIdentity:output:0*_
_input_shapesN
L:??????????::::::::::::::22
StatefulPartitionedCallStatefulPartitionedCall:2 .
,
_user_specified_namesequential_1_input: : : : : : : : :	 :
 : : : : 
ŵ
?
!__inference__traced_restore_64617
file_prefix
assignvariableop_adam_iter"
assignvariableop_1_adam_beta_1"
assignvariableop_2_adam_beta_2!
assignvariableop_3_adam_decay)
%assignvariableop_4_adam_learning_rate%
!assignvariableop_5_dense_8_kernel#
assignvariableop_6_dense_8_bias'
#assignvariableop_7_dense_1_2_kernel%
!assignvariableop_8_dense_1_2_bias'
#assignvariableop_9_dense_2_2_kernel&
"assignvariableop_10_dense_2_2_bias(
$assignvariableop_11_dense_3_2_kernel&
"assignvariableop_12_dense_3_2_bias(
$assignvariableop_13_dense_4_2_kernel&
"assignvariableop_14_dense_4_2_bias(
$assignvariableop_15_dense_5_2_kernel&
"assignvariableop_16_dense_5_2_bias(
$assignvariableop_17_dense_6_3_kernel&
"assignvariableop_18_dense_6_3_bias
assignvariableop_19_total
assignvariableop_20_count-
)assignvariableop_21_adam_dense_8_kernel_m+
'assignvariableop_22_adam_dense_8_bias_m/
+assignvariableop_23_adam_dense_1_2_kernel_m-
)assignvariableop_24_adam_dense_1_2_bias_m/
+assignvariableop_25_adam_dense_2_2_kernel_m-
)assignvariableop_26_adam_dense_2_2_bias_m/
+assignvariableop_27_adam_dense_3_2_kernel_m-
)assignvariableop_28_adam_dense_3_2_bias_m/
+assignvariableop_29_adam_dense_4_2_kernel_m-
)assignvariableop_30_adam_dense_4_2_bias_m/
+assignvariableop_31_adam_dense_5_2_kernel_m-
)assignvariableop_32_adam_dense_5_2_bias_m/
+assignvariableop_33_adam_dense_6_3_kernel_m-
)assignvariableop_34_adam_dense_6_3_bias_m-
)assignvariableop_35_adam_dense_8_kernel_v+
'assignvariableop_36_adam_dense_8_bias_v/
+assignvariableop_37_adam_dense_1_2_kernel_v-
)assignvariableop_38_adam_dense_1_2_bias_v/
+assignvariableop_39_adam_dense_2_2_kernel_v-
)assignvariableop_40_adam_dense_2_2_bias_v/
+assignvariableop_41_adam_dense_3_2_kernel_v-
)assignvariableop_42_adam_dense_3_2_bias_v/
+assignvariableop_43_adam_dense_4_2_kernel_v-
)assignvariableop_44_adam_dense_4_2_bias_v/
+assignvariableop_45_adam_dense_5_2_kernel_v-
)assignvariableop_46_adam_dense_5_2_bias_v/
+assignvariableop_47_adam_dense_6_3_kernel_v-
)assignvariableop_48_adam_dense_6_3_bias_v
identity_50??AssignVariableOp?AssignVariableOp_1?AssignVariableOp_10?AssignVariableOp_11?AssignVariableOp_12?AssignVariableOp_13?AssignVariableOp_14?AssignVariableOp_15?AssignVariableOp_16?AssignVariableOp_17?AssignVariableOp_18?AssignVariableOp_19?AssignVariableOp_2?AssignVariableOp_20?AssignVariableOp_21?AssignVariableOp_22?AssignVariableOp_23?AssignVariableOp_24?AssignVariableOp_25?AssignVariableOp_26?AssignVariableOp_27?AssignVariableOp_28?AssignVariableOp_29?AssignVariableOp_3?AssignVariableOp_30?AssignVariableOp_31?AssignVariableOp_32?AssignVariableOp_33?AssignVariableOp_34?AssignVariableOp_35?AssignVariableOp_36?AssignVariableOp_37?AssignVariableOp_38?AssignVariableOp_39?AssignVariableOp_4?AssignVariableOp_40?AssignVariableOp_41?AssignVariableOp_42?AssignVariableOp_43?AssignVariableOp_44?AssignVariableOp_45?AssignVariableOp_46?AssignVariableOp_47?AssignVariableOp_48?AssignVariableOp_5?AssignVariableOp_6?AssignVariableOp_7?AssignVariableOp_8?AssignVariableOp_9?	RestoreV2?RestoreV2_1?
RestoreV2/tensor_namesConst"/device:CPU:0*
dtype0*
_output_shapes
:1*?
value?B?1B)optimizer/iter/.ATTRIBUTES/VARIABLE_VALUEB+optimizer/beta_1/.ATTRIBUTES/VARIABLE_VALUEB+optimizer/beta_2/.ATTRIBUTES/VARIABLE_VALUEB*optimizer/decay/.ATTRIBUTES/VARIABLE_VALUEB2optimizer/learning_rate/.ATTRIBUTES/VARIABLE_VALUEB&variables/0/.ATTRIBUTES/VARIABLE_VALUEB&variables/1/.ATTRIBUTES/VARIABLE_VALUEB&variables/2/.ATTRIBUTES/VARIABLE_VALUEB&variables/3/.ATTRIBUTES/VARIABLE_VALUEB&variables/4/.ATTRIBUTES/VARIABLE_VALUEB&variables/5/.ATTRIBUTES/VARIABLE_VALUEB&variables/6/.ATTRIBUTES/VARIABLE_VALUEB&variables/7/.ATTRIBUTES/VARIABLE_VALUEB&variables/8/.ATTRIBUTES/VARIABLE_VALUEB&variables/9/.ATTRIBUTES/VARIABLE_VALUEB'variables/10/.ATTRIBUTES/VARIABLE_VALUEB'variables/11/.ATTRIBUTES/VARIABLE_VALUEB'variables/12/.ATTRIBUTES/VARIABLE_VALUEB'variables/13/.ATTRIBUTES/VARIABLE_VALUEB4keras_api/metrics/0/total/.ATTRIBUTES/VARIABLE_VALUEB4keras_api/metrics/0/count/.ATTRIBUTES/VARIABLE_VALUEBBvariables/0/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/1/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/2/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/3/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/4/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/5/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/6/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/7/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/8/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/9/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBCvariables/10/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBCvariables/11/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBCvariables/12/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBCvariables/13/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/0/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/1/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/2/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/3/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/4/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/5/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/6/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/7/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/8/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/9/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBCvariables/10/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBCvariables/11/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBCvariables/12/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBCvariables/13/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE?
RestoreV2/shape_and_slicesConst"/device:CPU:0*
dtype0*u
valuelBj1B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B *
_output_shapes
:1?
	RestoreV2	RestoreV2file_prefixRestoreV2/tensor_names:output:0#RestoreV2/shape_and_slices:output:0"/device:CPU:0*?
dtypes5
321	*?
_output_shapes?
?:::::::::::::::::::::::::::::::::::::::::::::::::L
IdentityIdentityRestoreV2:tensors:0*
_output_shapes
:*
T0	v
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

Identity_2IdentityRestoreV2:tensors:2*
T0*
_output_shapes
:~
AssignVariableOp_2AssignVariableOpassignvariableop_2_adam_beta_2Identity_2:output:0*
dtype0*
_output_shapes
 N

Identity_3IdentityRestoreV2:tensors:3*
_output_shapes
:*
T0}
AssignVariableOp_3AssignVariableOpassignvariableop_3_adam_decayIdentity_3:output:0*
_output_shapes
 *
dtype0N

Identity_4IdentityRestoreV2:tensors:4*
T0*
_output_shapes
:?
AssignVariableOp_4AssignVariableOp%assignvariableop_4_adam_learning_rateIdentity_4:output:0*
_output_shapes
 *
dtype0N

Identity_5IdentityRestoreV2:tensors:5*
T0*
_output_shapes
:?
AssignVariableOp_5AssignVariableOp!assignvariableop_5_dense_8_kernelIdentity_5:output:0*
_output_shapes
 *
dtype0N

Identity_6IdentityRestoreV2:tensors:6*
T0*
_output_shapes
:
AssignVariableOp_6AssignVariableOpassignvariableop_6_dense_8_biasIdentity_6:output:0*
dtype0*
_output_shapes
 N

Identity_7IdentityRestoreV2:tensors:7*
T0*
_output_shapes
:?
AssignVariableOp_7AssignVariableOp#assignvariableop_7_dense_1_2_kernelIdentity_7:output:0*
_output_shapes
 *
dtype0N

Identity_8IdentityRestoreV2:tensors:8*
T0*
_output_shapes
:?
AssignVariableOp_8AssignVariableOp!assignvariableop_8_dense_1_2_biasIdentity_8:output:0*
dtype0*
_output_shapes
 N

Identity_9IdentityRestoreV2:tensors:9*
_output_shapes
:*
T0?
AssignVariableOp_9AssignVariableOp#assignvariableop_9_dense_2_2_kernelIdentity_9:output:0*
dtype0*
_output_shapes
 P
Identity_10IdentityRestoreV2:tensors:10*
_output_shapes
:*
T0?
AssignVariableOp_10AssignVariableOp"assignvariableop_10_dense_2_2_biasIdentity_10:output:0*
dtype0*
_output_shapes
 P
Identity_11IdentityRestoreV2:tensors:11*
_output_shapes
:*
T0?
AssignVariableOp_11AssignVariableOp$assignvariableop_11_dense_3_2_kernelIdentity_11:output:0*
_output_shapes
 *
dtype0P
Identity_12IdentityRestoreV2:tensors:12*
T0*
_output_shapes
:?
AssignVariableOp_12AssignVariableOp"assignvariableop_12_dense_3_2_biasIdentity_12:output:0*
dtype0*
_output_shapes
 P
Identity_13IdentityRestoreV2:tensors:13*
T0*
_output_shapes
:?
AssignVariableOp_13AssignVariableOp$assignvariableop_13_dense_4_2_kernelIdentity_13:output:0*
_output_shapes
 *
dtype0P
Identity_14IdentityRestoreV2:tensors:14*
T0*
_output_shapes
:?
AssignVariableOp_14AssignVariableOp"assignvariableop_14_dense_4_2_biasIdentity_14:output:0*
_output_shapes
 *
dtype0P
Identity_15IdentityRestoreV2:tensors:15*
T0*
_output_shapes
:?
AssignVariableOp_15AssignVariableOp$assignvariableop_15_dense_5_2_kernelIdentity_15:output:0*
_output_shapes
 *
dtype0P
Identity_16IdentityRestoreV2:tensors:16*
_output_shapes
:*
T0?
AssignVariableOp_16AssignVariableOp"assignvariableop_16_dense_5_2_biasIdentity_16:output:0*
_output_shapes
 *
dtype0P
Identity_17IdentityRestoreV2:tensors:17*
_output_shapes
:*
T0?
AssignVariableOp_17AssignVariableOp$assignvariableop_17_dense_6_3_kernelIdentity_17:output:0*
dtype0*
_output_shapes
 P
Identity_18IdentityRestoreV2:tensors:18*
T0*
_output_shapes
:?
AssignVariableOp_18AssignVariableOp"assignvariableop_18_dense_6_3_biasIdentity_18:output:0*
dtype0*
_output_shapes
 P
Identity_19IdentityRestoreV2:tensors:19*
_output_shapes
:*
T0{
AssignVariableOp_19AssignVariableOpassignvariableop_19_totalIdentity_19:output:0*
_output_shapes
 *
dtype0P
Identity_20IdentityRestoreV2:tensors:20*
T0*
_output_shapes
:{
AssignVariableOp_20AssignVariableOpassignvariableop_20_countIdentity_20:output:0*
_output_shapes
 *
dtype0P
Identity_21IdentityRestoreV2:tensors:21*
T0*
_output_shapes
:?
AssignVariableOp_21AssignVariableOp)assignvariableop_21_adam_dense_8_kernel_mIdentity_21:output:0*
dtype0*
_output_shapes
 P
Identity_22IdentityRestoreV2:tensors:22*
_output_shapes
:*
T0?
AssignVariableOp_22AssignVariableOp'assignvariableop_22_adam_dense_8_bias_mIdentity_22:output:0*
_output_shapes
 *
dtype0P
Identity_23IdentityRestoreV2:tensors:23*
_output_shapes
:*
T0?
AssignVariableOp_23AssignVariableOp+assignvariableop_23_adam_dense_1_2_kernel_mIdentity_23:output:0*
_output_shapes
 *
dtype0P
Identity_24IdentityRestoreV2:tensors:24*
T0*
_output_shapes
:?
AssignVariableOp_24AssignVariableOp)assignvariableop_24_adam_dense_1_2_bias_mIdentity_24:output:0*
dtype0*
_output_shapes
 P
Identity_25IdentityRestoreV2:tensors:25*
T0*
_output_shapes
:?
AssignVariableOp_25AssignVariableOp+assignvariableop_25_adam_dense_2_2_kernel_mIdentity_25:output:0*
_output_shapes
 *
dtype0P
Identity_26IdentityRestoreV2:tensors:26*
T0*
_output_shapes
:?
AssignVariableOp_26AssignVariableOp)assignvariableop_26_adam_dense_2_2_bias_mIdentity_26:output:0*
dtype0*
_output_shapes
 P
Identity_27IdentityRestoreV2:tensors:27*
_output_shapes
:*
T0?
AssignVariableOp_27AssignVariableOp+assignvariableop_27_adam_dense_3_2_kernel_mIdentity_27:output:0*
dtype0*
_output_shapes
 P
Identity_28IdentityRestoreV2:tensors:28*
T0*
_output_shapes
:?
AssignVariableOp_28AssignVariableOp)assignvariableop_28_adam_dense_3_2_bias_mIdentity_28:output:0*
_output_shapes
 *
dtype0P
Identity_29IdentityRestoreV2:tensors:29*
T0*
_output_shapes
:?
AssignVariableOp_29AssignVariableOp+assignvariableop_29_adam_dense_4_2_kernel_mIdentity_29:output:0*
dtype0*
_output_shapes
 P
Identity_30IdentityRestoreV2:tensors:30*
T0*
_output_shapes
:?
AssignVariableOp_30AssignVariableOp)assignvariableop_30_adam_dense_4_2_bias_mIdentity_30:output:0*
_output_shapes
 *
dtype0P
Identity_31IdentityRestoreV2:tensors:31*
T0*
_output_shapes
:?
AssignVariableOp_31AssignVariableOp+assignvariableop_31_adam_dense_5_2_kernel_mIdentity_31:output:0*
dtype0*
_output_shapes
 P
Identity_32IdentityRestoreV2:tensors:32*
T0*
_output_shapes
:?
AssignVariableOp_32AssignVariableOp)assignvariableop_32_adam_dense_5_2_bias_mIdentity_32:output:0*
dtype0*
_output_shapes
 P
Identity_33IdentityRestoreV2:tensors:33*
_output_shapes
:*
T0?
AssignVariableOp_33AssignVariableOp+assignvariableop_33_adam_dense_6_3_kernel_mIdentity_33:output:0*
_output_shapes
 *
dtype0P
Identity_34IdentityRestoreV2:tensors:34*
_output_shapes
:*
T0?
AssignVariableOp_34AssignVariableOp)assignvariableop_34_adam_dense_6_3_bias_mIdentity_34:output:0*
dtype0*
_output_shapes
 P
Identity_35IdentityRestoreV2:tensors:35*
_output_shapes
:*
T0?
AssignVariableOp_35AssignVariableOp)assignvariableop_35_adam_dense_8_kernel_vIdentity_35:output:0*
_output_shapes
 *
dtype0P
Identity_36IdentityRestoreV2:tensors:36*
_output_shapes
:*
T0?
AssignVariableOp_36AssignVariableOp'assignvariableop_36_adam_dense_8_bias_vIdentity_36:output:0*
_output_shapes
 *
dtype0P
Identity_37IdentityRestoreV2:tensors:37*
T0*
_output_shapes
:?
AssignVariableOp_37AssignVariableOp+assignvariableop_37_adam_dense_1_2_kernel_vIdentity_37:output:0*
dtype0*
_output_shapes
 P
Identity_38IdentityRestoreV2:tensors:38*
T0*
_output_shapes
:?
AssignVariableOp_38AssignVariableOp)assignvariableop_38_adam_dense_1_2_bias_vIdentity_38:output:0*
dtype0*
_output_shapes
 P
Identity_39IdentityRestoreV2:tensors:39*
T0*
_output_shapes
:?
AssignVariableOp_39AssignVariableOp+assignvariableop_39_adam_dense_2_2_kernel_vIdentity_39:output:0*
dtype0*
_output_shapes
 P
Identity_40IdentityRestoreV2:tensors:40*
T0*
_output_shapes
:?
AssignVariableOp_40AssignVariableOp)assignvariableop_40_adam_dense_2_2_bias_vIdentity_40:output:0*
_output_shapes
 *
dtype0P
Identity_41IdentityRestoreV2:tensors:41*
T0*
_output_shapes
:?
AssignVariableOp_41AssignVariableOp+assignvariableop_41_adam_dense_3_2_kernel_vIdentity_41:output:0*
dtype0*
_output_shapes
 P
Identity_42IdentityRestoreV2:tensors:42*
T0*
_output_shapes
:?
AssignVariableOp_42AssignVariableOp)assignvariableop_42_adam_dense_3_2_bias_vIdentity_42:output:0*
_output_shapes
 *
dtype0P
Identity_43IdentityRestoreV2:tensors:43*
T0*
_output_shapes
:?
AssignVariableOp_43AssignVariableOp+assignvariableop_43_adam_dense_4_2_kernel_vIdentity_43:output:0*
dtype0*
_output_shapes
 P
Identity_44IdentityRestoreV2:tensors:44*
_output_shapes
:*
T0?
AssignVariableOp_44AssignVariableOp)assignvariableop_44_adam_dense_4_2_bias_vIdentity_44:output:0*
dtype0*
_output_shapes
 P
Identity_45IdentityRestoreV2:tensors:45*
_output_shapes
:*
T0?
AssignVariableOp_45AssignVariableOp+assignvariableop_45_adam_dense_5_2_kernel_vIdentity_45:output:0*
dtype0*
_output_shapes
 P
Identity_46IdentityRestoreV2:tensors:46*
T0*
_output_shapes
:?
AssignVariableOp_46AssignVariableOp)assignvariableop_46_adam_dense_5_2_bias_vIdentity_46:output:0*
_output_shapes
 *
dtype0P
Identity_47IdentityRestoreV2:tensors:47*
T0*
_output_shapes
:?
AssignVariableOp_47AssignVariableOp+assignvariableop_47_adam_dense_6_3_kernel_vIdentity_47:output:0*
_output_shapes
 *
dtype0P
Identity_48IdentityRestoreV2:tensors:48*
_output_shapes
:*
T0?
AssignVariableOp_48AssignVariableOp)assignvariableop_48_adam_dense_6_3_bias_vIdentity_48:output:0*
dtype0*
_output_shapes
 ?
RestoreV2_1/tensor_namesConst"/device:CPU:0*
dtype0*
_output_shapes
:*1
value(B&B_CHECKPOINTABLE_OBJECT_GRAPHt
RestoreV2_1/shape_and_slicesConst"/device:CPU:0*
valueB
B *
_output_shapes
:*
dtype0?
RestoreV2_1	RestoreV2file_prefix!RestoreV2_1/tensor_names:output:0%RestoreV2_1/shape_and_slices:output:0
^RestoreV2"/device:CPU:0*
_output_shapes
:*
dtypes
21
NoOpNoOp"/device:CPU:0*
_output_shapes
 ?	
Identity_49Identityfile_prefix^AssignVariableOp^AssignVariableOp_1^AssignVariableOp_10^AssignVariableOp_11^AssignVariableOp_12^AssignVariableOp_13^AssignVariableOp_14^AssignVariableOp_15^AssignVariableOp_16^AssignVariableOp_17^AssignVariableOp_18^AssignVariableOp_19^AssignVariableOp_2^AssignVariableOp_20^AssignVariableOp_21^AssignVariableOp_22^AssignVariableOp_23^AssignVariableOp_24^AssignVariableOp_25^AssignVariableOp_26^AssignVariableOp_27^AssignVariableOp_28^AssignVariableOp_29^AssignVariableOp_3^AssignVariableOp_30^AssignVariableOp_31^AssignVariableOp_32^AssignVariableOp_33^AssignVariableOp_34^AssignVariableOp_35^AssignVariableOp_36^AssignVariableOp_37^AssignVariableOp_38^AssignVariableOp_39^AssignVariableOp_4^AssignVariableOp_40^AssignVariableOp_41^AssignVariableOp_42^AssignVariableOp_43^AssignVariableOp_44^AssignVariableOp_45^AssignVariableOp_46^AssignVariableOp_47^AssignVariableOp_48^AssignVariableOp_5^AssignVariableOp_6^AssignVariableOp_7^AssignVariableOp_8^AssignVariableOp_9^NoOp"/device:CPU:0*
_output_shapes
: *
T0?	
Identity_50IdentityIdentity_49:output:0^AssignVariableOp^AssignVariableOp_1^AssignVariableOp_10^AssignVariableOp_11^AssignVariableOp_12^AssignVariableOp_13^AssignVariableOp_14^AssignVariableOp_15^AssignVariableOp_16^AssignVariableOp_17^AssignVariableOp_18^AssignVariableOp_19^AssignVariableOp_2^AssignVariableOp_20^AssignVariableOp_21^AssignVariableOp_22^AssignVariableOp_23^AssignVariableOp_24^AssignVariableOp_25^AssignVariableOp_26^AssignVariableOp_27^AssignVariableOp_28^AssignVariableOp_29^AssignVariableOp_3^AssignVariableOp_30^AssignVariableOp_31^AssignVariableOp_32^AssignVariableOp_33^AssignVariableOp_34^AssignVariableOp_35^AssignVariableOp_36^AssignVariableOp_37^AssignVariableOp_38^AssignVariableOp_39^AssignVariableOp_4^AssignVariableOp_40^AssignVariableOp_41^AssignVariableOp_42^AssignVariableOp_43^AssignVariableOp_44^AssignVariableOp_45^AssignVariableOp_46^AssignVariableOp_47^AssignVariableOp_48^AssignVariableOp_5^AssignVariableOp_6^AssignVariableOp_7^AssignVariableOp_8^AssignVariableOp_9
^RestoreV2^RestoreV2_1*
_output_shapes
: *
T0"#
identity_50Identity_50:output:0*?
_input_shapes?
?: :::::::::::::::::::::::::::::::::::::::::::::::::2(
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
AssignVariableOp_11AssignVariableOp_112*
AssignVariableOp_12AssignVariableOp_122*
AssignVariableOp_13AssignVariableOp_132*
AssignVariableOp_14AssignVariableOp_142*
AssignVariableOp_20AssignVariableOp_202*
AssignVariableOp_15AssignVariableOp_152*
AssignVariableOp_16AssignVariableOp_162*
AssignVariableOp_21AssignVariableOp_212*
AssignVariableOp_22AssignVariableOp_222*
AssignVariableOp_17AssignVariableOp_172*
AssignVariableOp_18AssignVariableOp_182*
AssignVariableOp_23AssignVariableOp_232*
AssignVariableOp_19AssignVariableOp_192*
AssignVariableOp_24AssignVariableOp_242*
AssignVariableOp_30AssignVariableOp_302*
AssignVariableOp_25AssignVariableOp_252*
AssignVariableOp_26AssignVariableOp_262*
AssignVariableOp_31AssignVariableOp_312$
AssignVariableOpAssignVariableOp2*
AssignVariableOp_32AssignVariableOp_322*
AssignVariableOp_27AssignVariableOp_272*
AssignVariableOp_28AssignVariableOp_282*
AssignVariableOp_33AssignVariableOp_332*
AssignVariableOp_34AssignVariableOp_342*
AssignVariableOp_29AssignVariableOp_292*
AssignVariableOp_40AssignVariableOp_402*
AssignVariableOp_35AssignVariableOp_352*
AssignVariableOp_36AssignVariableOp_362*
AssignVariableOp_41AssignVariableOp_412*
AssignVariableOp_37AssignVariableOp_372*
AssignVariableOp_42AssignVariableOp_422*
AssignVariableOp_43AssignVariableOp_432*
AssignVariableOp_38AssignVariableOp_382*
AssignVariableOp_39AssignVariableOp_392*
AssignVariableOp_44AssignVariableOp_442*
AssignVariableOp_45AssignVariableOp_452*
AssignVariableOp_46AssignVariableOp_462*
AssignVariableOp_47AssignVariableOp_472*
AssignVariableOp_48AssignVariableOp_482
RestoreV2_1RestoreV2_1: : : : : : : : : : : : : : :  :! :" :# :$ :% :& :' :( :) :* :+ :, :- :. :/ :0 :1 :+ '
%
_user_specified_namefile_prefix: : : : : : : : :	 :
 : : : : : : : 
?
a
E__inference_sequential_layer_call_and_return_conditional_losses_63082

inputs
identityO
IdentityIdentityinputs*
T0*(
_output_shapes
:??????????"
identityIdentity:output:0*'
_input_shapes
:??????????:& "
 
_user_specified_nameinputs
?b
?
 __inference__wrapped_model_63006
sequential_1_inputB
>sequential_4_sequential_2_dense_matmul_readvariableop_resourceC
?sequential_4_sequential_2_dense_biasadd_readvariableop_resourceD
@sequential_4_sequential_2_dense_1_matmul_readvariableop_resourceE
Asequential_4_sequential_2_dense_1_biasadd_readvariableop_resourceD
@sequential_4_sequential_2_dense_2_matmul_readvariableop_resourceE
Asequential_4_sequential_2_dense_2_biasadd_readvariableop_resourceD
@sequential_4_sequential_2_dense_3_matmul_readvariableop_resourceE
Asequential_4_sequential_2_dense_3_biasadd_readvariableop_resourceD
@sequential_4_sequential_2_dense_4_matmul_readvariableop_resourceE
Asequential_4_sequential_2_dense_4_biasadd_readvariableop_resourceD
@sequential_4_sequential_2_dense_5_matmul_readvariableop_resourceE
Asequential_4_sequential_2_dense_5_biasadd_readvariableop_resourceD
@sequential_4_sequential_3_dense_6_matmul_readvariableop_resourceE
Asequential_4_sequential_3_dense_6_biasadd_readvariableop_resource
identity??6sequential_4/sequential_2/dense/BiasAdd/ReadVariableOp?5sequential_4/sequential_2/dense/MatMul/ReadVariableOp?8sequential_4/sequential_2/dense_1/BiasAdd/ReadVariableOp?7sequential_4/sequential_2/dense_1/MatMul/ReadVariableOp?8sequential_4/sequential_2/dense_2/BiasAdd/ReadVariableOp?7sequential_4/sequential_2/dense_2/MatMul/ReadVariableOp?8sequential_4/sequential_2/dense_3/BiasAdd/ReadVariableOp?7sequential_4/sequential_2/dense_3/MatMul/ReadVariableOp?8sequential_4/sequential_2/dense_4/BiasAdd/ReadVariableOp?7sequential_4/sequential_2/dense_4/MatMul/ReadVariableOp?8sequential_4/sequential_2/dense_5/BiasAdd/ReadVariableOp?7sequential_4/sequential_2/dense_5/MatMul/ReadVariableOp?8sequential_4/sequential_3/dense_6/BiasAdd/ReadVariableOp?7sequential_4/sequential_3/dense_6/MatMul/ReadVariableOp?
/sequential_4/sequential_1/flatten/Reshape/shapeConst*
_output_shapes
:*
dtype0*
valueB"?????   ?
)sequential_4/sequential_1/flatten/ReshapeReshapesequential_1_input8sequential_4/sequential_1/flatten/Reshape/shape:output:0*(
_output_shapes
:??????????*
T0?
5sequential_4/sequential_2/dense/MatMul/ReadVariableOpReadVariableOp>sequential_4_sequential_2_dense_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0* 
_output_shapes
:
??*
dtype0?
&sequential_4/sequential_2/dense/MatMulMatMul2sequential_4/sequential_1/flatten/Reshape:output:0=sequential_4/sequential_2/dense/MatMul/ReadVariableOp:value:0*(
_output_shapes
:??????????*
T0?
6sequential_4/sequential_2/dense/BiasAdd/ReadVariableOpReadVariableOp?sequential_4_sequential_2_dense_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes	
:??
'sequential_4/sequential_2/dense/BiasAddBiasAdd0sequential_4/sequential_2/dense/MatMul:product:0>sequential_4/sequential_2/dense/BiasAdd/ReadVariableOp:value:0*(
_output_shapes
:??????????*
T0?
$sequential_4/sequential_2/dense/ReluRelu0sequential_4/sequential_2/dense/BiasAdd:output:0*(
_output_shapes
:??????????*
T0?
7sequential_4/sequential_2/dense_1/MatMul/ReadVariableOpReadVariableOp@sequential_4_sequential_2_dense_1_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0* 
_output_shapes
:
??*
dtype0?
(sequential_4/sequential_2/dense_1/MatMulMatMul2sequential_4/sequential_2/dense/Relu:activations:0?sequential_4/sequential_2/dense_1/MatMul/ReadVariableOp:value:0*(
_output_shapes
:??????????*
T0?
8sequential_4/sequential_2/dense_1/BiasAdd/ReadVariableOpReadVariableOpAsequential_4_sequential_2_dense_1_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes	
:?*
dtype0?
)sequential_4/sequential_2/dense_1/BiasAddBiasAdd2sequential_4/sequential_2/dense_1/MatMul:product:0@sequential_4/sequential_2/dense_1/BiasAdd/ReadVariableOp:value:0*(
_output_shapes
:??????????*
T0?
&sequential_4/sequential_2/dense_1/ReluRelu2sequential_4/sequential_2/dense_1/BiasAdd:output:0*(
_output_shapes
:??????????*
T0?
7sequential_4/sequential_2/dense_2/MatMul/ReadVariableOpReadVariableOp@sequential_4_sequential_2_dense_2_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:	?8*
dtype0?
(sequential_4/sequential_2/dense_2/MatMulMatMul4sequential_4/sequential_2/dense_1/Relu:activations:0?sequential_4/sequential_2/dense_2/MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8?
8sequential_4/sequential_2/dense_2/BiasAdd/ReadVariableOpReadVariableOpAsequential_4_sequential_2_dense_2_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:8*
dtype0?
)sequential_4/sequential_2/dense_2/BiasAddBiasAdd2sequential_4/sequential_2/dense_2/MatMul:product:0@sequential_4/sequential_2/dense_2/BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8?
&sequential_4/sequential_2/dense_2/ReluRelu2sequential_4/sequential_2/dense_2/BiasAdd:output:0*
T0*'
_output_shapes
:?????????8?
7sequential_4/sequential_2/dense_3/MatMul/ReadVariableOpReadVariableOp@sequential_4_sequential_2_dense_3_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes

:88?
(sequential_4/sequential_2/dense_3/MatMulMatMul4sequential_4/sequential_2/dense_2/Relu:activations:0?sequential_4/sequential_2/dense_3/MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8?
8sequential_4/sequential_2/dense_3/BiasAdd/ReadVariableOpReadVariableOpAsequential_4_sequential_2_dense_3_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:8*
dtype0?
)sequential_4/sequential_2/dense_3/BiasAddBiasAdd2sequential_4/sequential_2/dense_3/MatMul:product:0@sequential_4/sequential_2/dense_3/BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8?
&sequential_4/sequential_2/dense_3/ReluRelu2sequential_4/sequential_2/dense_3/BiasAdd:output:0*'
_output_shapes
:?????????8*
T0?
7sequential_4/sequential_2/dense_4/MatMul/ReadVariableOpReadVariableOp@sequential_4_sequential_2_dense_4_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes

:88?
(sequential_4/sequential_2/dense_4/MatMulMatMul4sequential_4/sequential_2/dense_3/Relu:activations:0?sequential_4/sequential_2/dense_4/MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8?
8sequential_4/sequential_2/dense_4/BiasAdd/ReadVariableOpReadVariableOpAsequential_4_sequential_2_dense_4_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:8?
)sequential_4/sequential_2/dense_4/BiasAddBiasAdd2sequential_4/sequential_2/dense_4/MatMul:product:0@sequential_4/sequential_2/dense_4/BiasAdd/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0?
&sequential_4/sequential_2/dense_4/ReluRelu2sequential_4/sequential_2/dense_4/BiasAdd:output:0*'
_output_shapes
:?????????8*
T0?
7sequential_4/sequential_2/dense_5/MatMul/ReadVariableOpReadVariableOp@sequential_4_sequential_2_dense_5_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes

:88?
(sequential_4/sequential_2/dense_5/MatMulMatMul4sequential_4/sequential_2/dense_4/Relu:activations:0?sequential_4/sequential_2/dense_5/MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8?
8sequential_4/sequential_2/dense_5/BiasAdd/ReadVariableOpReadVariableOpAsequential_4_sequential_2_dense_5_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:8*
dtype0?
)sequential_4/sequential_2/dense_5/BiasAddBiasAdd2sequential_4/sequential_2/dense_5/MatMul:product:0@sequential_4/sequential_2/dense_5/BiasAdd/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0?
&sequential_4/sequential_2/dense_5/ReluRelu2sequential_4/sequential_2/dense_5/BiasAdd:output:0*
T0*'
_output_shapes
:?????????8?
*sequential_4/sequential_2/dropout/IdentityIdentity4sequential_4/sequential_2/dense_5/Relu:activations:0*'
_output_shapes
:?????????8*
T0?
7sequential_4/sequential_3/dense_6/MatMul/ReadVariableOpReadVariableOp@sequential_4_sequential_3_dense_6_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes

:8?
(sequential_4/sequential_3/dense_6/MatMulMatMul3sequential_4/sequential_2/dropout/Identity:output:0?sequential_4/sequential_3/dense_6/MatMul/ReadVariableOp:value:0*'
_output_shapes
:?????????*
T0?
8sequential_4/sequential_3/dense_6/BiasAdd/ReadVariableOpReadVariableOpAsequential_4_sequential_3_dense_6_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:?
)sequential_4/sequential_3/dense_6/BiasAddBiasAdd2sequential_4/sequential_3/dense_6/MatMul:product:0@sequential_4/sequential_3/dense_6/BiasAdd/ReadVariableOp:value:0*'
_output_shapes
:?????????*
T0?
)sequential_4/sequential_3/dense_6/SoftmaxSoftmax2sequential_4/sequential_3/dense_6/BiasAdd:output:0*
T0*'
_output_shapes
:??????????
IdentityIdentity3sequential_4/sequential_3/dense_6/Softmax:softmax:07^sequential_4/sequential_2/dense/BiasAdd/ReadVariableOp6^sequential_4/sequential_2/dense/MatMul/ReadVariableOp9^sequential_4/sequential_2/dense_1/BiasAdd/ReadVariableOp8^sequential_4/sequential_2/dense_1/MatMul/ReadVariableOp9^sequential_4/sequential_2/dense_2/BiasAdd/ReadVariableOp8^sequential_4/sequential_2/dense_2/MatMul/ReadVariableOp9^sequential_4/sequential_2/dense_3/BiasAdd/ReadVariableOp8^sequential_4/sequential_2/dense_3/MatMul/ReadVariableOp9^sequential_4/sequential_2/dense_4/BiasAdd/ReadVariableOp8^sequential_4/sequential_2/dense_4/MatMul/ReadVariableOp9^sequential_4/sequential_2/dense_5/BiasAdd/ReadVariableOp8^sequential_4/sequential_2/dense_5/MatMul/ReadVariableOp9^sequential_4/sequential_3/dense_6/BiasAdd/ReadVariableOp8^sequential_4/sequential_3/dense_6/MatMul/ReadVariableOp*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*_
_input_shapesN
L:??????????::::::::::::::2t
8sequential_4/sequential_2/dense_5/BiasAdd/ReadVariableOp8sequential_4/sequential_2/dense_5/BiasAdd/ReadVariableOp2r
7sequential_4/sequential_3/dense_6/MatMul/ReadVariableOp7sequential_4/sequential_3/dense_6/MatMul/ReadVariableOp2t
8sequential_4/sequential_2/dense_3/BiasAdd/ReadVariableOp8sequential_4/sequential_2/dense_3/BiasAdd/ReadVariableOp2r
7sequential_4/sequential_2/dense_4/MatMul/ReadVariableOp7sequential_4/sequential_2/dense_4/MatMul/ReadVariableOp2p
6sequential_4/sequential_2/dense/BiasAdd/ReadVariableOp6sequential_4/sequential_2/dense/BiasAdd/ReadVariableOp2r
7sequential_4/sequential_2/dense_1/MatMul/ReadVariableOp7sequential_4/sequential_2/dense_1/MatMul/ReadVariableOp2t
8sequential_4/sequential_2/dense_1/BiasAdd/ReadVariableOp8sequential_4/sequential_2/dense_1/BiasAdd/ReadVariableOp2r
7sequential_4/sequential_2/dense_5/MatMul/ReadVariableOp7sequential_4/sequential_2/dense_5/MatMul/ReadVariableOp2t
8sequential_4/sequential_2/dense_4/BiasAdd/ReadVariableOp8sequential_4/sequential_2/dense_4/BiasAdd/ReadVariableOp2r
7sequential_4/sequential_2/dense_2/MatMul/ReadVariableOp7sequential_4/sequential_2/dense_2/MatMul/ReadVariableOp2n
5sequential_4/sequential_2/dense/MatMul/ReadVariableOp5sequential_4/sequential_2/dense/MatMul/ReadVariableOp2t
8sequential_4/sequential_2/dense_2/BiasAdd/ReadVariableOp8sequential_4/sequential_2/dense_2/BiasAdd/ReadVariableOp2t
8sequential_4/sequential_3/dense_6/BiasAdd/ReadVariableOp8sequential_4/sequential_3/dense_6/BiasAdd/ReadVariableOp2r
7sequential_4/sequential_2/dense_3/MatMul/ReadVariableOp7sequential_4/sequential_2/dense_3/MatMul/ReadVariableOp:2 .
,
_user_specified_namesequential_1_input: : : : : : : : :	 :
 : : : : 
?%
?
G__inference_sequential_2_layer_call_and_return_conditional_losses_63366

inputs(
$dense_statefulpartitionedcall_args_1(
$dense_statefulpartitionedcall_args_2*
&dense_1_statefulpartitionedcall_args_1*
&dense_1_statefulpartitionedcall_args_2*
&dense_2_statefulpartitionedcall_args_1*
&dense_2_statefulpartitionedcall_args_2*
&dense_3_statefulpartitionedcall_args_1*
&dense_3_statefulpartitionedcall_args_2*
&dense_4_statefulpartitionedcall_args_1*
&dense_4_statefulpartitionedcall_args_2*
&dense_5_statefulpartitionedcall_args_1*
&dense_5_statefulpartitionedcall_args_2
identity??dense/StatefulPartitionedCall?dense_1/StatefulPartitionedCall?dense_2/StatefulPartitionedCall?dense_3/StatefulPartitionedCall?dense_4/StatefulPartitionedCall?dense_5/StatefulPartitionedCall?dropout/StatefulPartitionedCall?
dense/StatefulPartitionedCallStatefulPartitionedCallinputs$dense_statefulpartitionedcall_args_1$dense_statefulpartitionedcall_args_2*(
_output_shapes
:??????????*I
fDRB
@__inference_dense_layer_call_and_return_conditional_losses_63113**
config_proto

GPU 

CPU2J 8*
Tout
2*
Tin
2*,
_gradient_op_typePartitionedCall-63119?
dense_1/StatefulPartitionedCallStatefulPartitionedCall&dense/StatefulPartitionedCall:output:0&dense_1_statefulpartitionedcall_args_1&dense_1_statefulpartitionedcall_args_2*
Tin
2**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-63147*(
_output_shapes
:??????????*K
fFRD
B__inference_dense_1_layer_call_and_return_conditional_losses_63141*
Tout
2?
dense_2/StatefulPartitionedCallStatefulPartitionedCall(dense_1/StatefulPartitionedCall:output:0&dense_2_statefulpartitionedcall_args_1&dense_2_statefulpartitionedcall_args_2*
Tout
2*
Tin
2**
config_proto

GPU 

CPU2J 8*K
fFRD
B__inference_dense_2_layer_call_and_return_conditional_losses_63169*'
_output_shapes
:?????????8*,
_gradient_op_typePartitionedCall-63175?
dense_3/StatefulPartitionedCallStatefulPartitionedCall(dense_2/StatefulPartitionedCall:output:0&dense_3_statefulpartitionedcall_args_1&dense_3_statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-63203*'
_output_shapes
:?????????8*
Tin
2*
Tout
2*K
fFRD
B__inference_dense_3_layer_call_and_return_conditional_losses_63197?
dense_4/StatefulPartitionedCallStatefulPartitionedCall(dense_3/StatefulPartitionedCall:output:0&dense_4_statefulpartitionedcall_args_1&dense_4_statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*
Tin
2*
Tout
2*K
fFRD
B__inference_dense_4_layer_call_and_return_conditional_losses_63225*'
_output_shapes
:?????????8*,
_gradient_op_typePartitionedCall-63231?
dense_5/StatefulPartitionedCallStatefulPartitionedCall(dense_4/StatefulPartitionedCall:output:0&dense_5_statefulpartitionedcall_args_1&dense_5_statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*K
fFRD
B__inference_dense_5_layer_call_and_return_conditional_losses_63253*'
_output_shapes
:?????????8*
Tout
2*,
_gradient_op_typePartitionedCall-63259*
Tin
2?
dropout/StatefulPartitionedCallStatefulPartitionedCall(dense_5/StatefulPartitionedCall:output:0*'
_output_shapes
:?????????8**
config_proto

GPU 

CPU2J 8*
Tout
2*
Tin
2*,
_gradient_op_typePartitionedCall-63301*K
fFRD
B__inference_dropout_layer_call_and_return_conditional_losses_63290?
IdentityIdentity(dropout/StatefulPartitionedCall:output:0^dense/StatefulPartitionedCall ^dense_1/StatefulPartitionedCall ^dense_2/StatefulPartitionedCall ^dense_3/StatefulPartitionedCall ^dense_4/StatefulPartitionedCall ^dense_5/StatefulPartitionedCall ^dropout/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????8"
identityIdentity:output:0*W
_input_shapesF
D:??????????::::::::::::2B
dense_2/StatefulPartitionedCalldense_2/StatefulPartitionedCall2B
dense_3/StatefulPartitionedCalldense_3/StatefulPartitionedCall2B
dense_4/StatefulPartitionedCalldense_4/StatefulPartitionedCall2B
dense_5/StatefulPartitionedCalldense_5/StatefulPartitionedCall2B
dropout/StatefulPartitionedCalldropout/StatefulPartitionedCall2>
dense/StatefulPartitionedCalldense/StatefulPartitionedCall2B
dense_1/StatefulPartitionedCalldense_1/StatefulPartitionedCall:& "
 
_user_specified_nameinputs: : : : : : : : :	 :
 : : 
?
O
,__inference_sequential_1_layer_call_fn_63051
flatten_input
identity?
PartitionedCallPartitionedCallflatten_input*P
fKRI
G__inference_sequential_1_layer_call_and_return_conditional_losses_63047*,
_gradient_op_typePartitionedCall-63048*
Tout
2*(
_output_shapes
:??????????**
config_proto

GPU 

CPU2J 8*
Tin
2a
IdentityIdentityPartitionedCall:output:0*(
_output_shapes
:??????????*
T0"
identityIdentity:output:0*'
_input_shapes
:??????????:- )
'
_user_specified_nameflatten_input
?	
?
B__inference_dense_1_layer_call_and_return_conditional_losses_64153

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0* 
_output_shapes
:
??j
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*(
_output_shapes
:??????????*
T0?
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes	
:?w
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*(
_output_shapes
:??????????*
T0Q
ReluReluBiasAdd:output:0*
T0*(
_output_shapes
:???????????
IdentityIdentityRelu:activations:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*(
_output_shapes
:??????????*
T0"
identityIdentity:output:0*/
_input_shapes
:??????????::2.
MatMul/ReadVariableOpMatMul/ReadVariableOp20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp: :& "
 
_user_specified_nameinputs: 
?
b
E__inference_sequential_layer_call_and_return_conditional_losses_63069
input_1
identityP
IdentityIdentityinput_1*
T0*(
_output_shapes
:??????????"
identityIdentity:output:0*'
_input_shapes
:??????????:' #
!
_user_specified_name	input_1
?
?
'__inference_dense_5_layer_call_fn_64232

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*K
fFRD
B__inference_dense_5_layer_call_and_return_conditional_losses_63253*
Tin
2*
Tout
2**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:?????????8*,
_gradient_op_typePartitionedCall-63259?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????8"
identityIdentity:output:0*.
_input_shapes
:?????????8::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
?
,__inference_sequential_4_layer_call_fn_63894

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2"
statefulpartitionedcall_args_3"
statefulpartitionedcall_args_4"
statefulpartitionedcall_args_5"
statefulpartitionedcall_args_6"
statefulpartitionedcall_args_7"
statefulpartitionedcall_args_8"
statefulpartitionedcall_args_9#
statefulpartitionedcall_args_10#
statefulpartitionedcall_args_11#
statefulpartitionedcall_args_12#
statefulpartitionedcall_args_13#
statefulpartitionedcall_args_14
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2statefulpartitionedcall_args_3statefulpartitionedcall_args_4statefulpartitionedcall_args_5statefulpartitionedcall_args_6statefulpartitionedcall_args_7statefulpartitionedcall_args_8statefulpartitionedcall_args_9statefulpartitionedcall_args_10statefulpartitionedcall_args_11statefulpartitionedcall_args_12statefulpartitionedcall_args_13statefulpartitionedcall_args_14*
Tin
2*
Tout
2*,
_gradient_op_typePartitionedCall-63685*'
_output_shapes
:?????????**
config_proto

GPU 

CPU2J 8*P
fKRI
G__inference_sequential_4_layer_call_and_return_conditional_losses_63684?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*_
_input_shapesN
L:??????????::::::::::::::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : : : : : : : :	 :
 : : : : 
?	
?
B__inference_dense_6_layer_call_and_return_conditional_losses_64278

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes

:8i
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
IdentityIdentitySoftmax:softmax:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*.
_input_shapes
:?????????8::20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp: :& "
 
_user_specified_nameinputs: 
?
G
*__inference_sequential_layer_call_fn_63098
input_1
identity?
PartitionedCallPartitionedCallinput_1**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-63095*(
_output_shapes
:??????????*
Tout
2*N
fIRG
E__inference_sequential_layer_call_and_return_conditional_losses_63094*
Tin
2a
IdentityIdentityPartitionedCall:output:0*
T0*(
_output_shapes
:??????????"
identityIdentity:output:0*'
_input_shapes
:??????????:' #
!
_user_specified_name	input_1
?
F
*__inference_sequential_layer_call_fn_63929

inputs
identity?
PartitionedCallPartitionedCallinputs*
Tin
2*N
fIRG
E__inference_sequential_layer_call_and_return_conditional_losses_63082*,
_gradient_op_typePartitionedCall-63083*(
_output_shapes
:??????????*
Tout
2**
config_proto

GPU 

CPU2J 8a
IdentityIdentityPartitionedCall:output:0*
T0*(
_output_shapes
:??????????"
identityIdentity:output:0*'
_input_shapes
:??????????:& "
 
_user_specified_nameinputs
?
?
,__inference_sequential_3_layer_call_fn_64113

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*
Tin
2**
config_proto

GPU 

CPU2J 8*P
fKRI
G__inference_sequential_3_layer_call_and_return_conditional_losses_63494*,
_gradient_op_typePartitionedCall-63495*'
_output_shapes
:?????????*
Tout
2?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*.
_input_shapes
:?????????8::22
StatefulPartitionedCallStatefulPartitionedCall: : :& "
 
_user_specified_nameinputs
?#
?
G__inference_sequential_2_layer_call_and_return_conditional_losses_63409

inputs(
$dense_statefulpartitionedcall_args_1(
$dense_statefulpartitionedcall_args_2*
&dense_1_statefulpartitionedcall_args_1*
&dense_1_statefulpartitionedcall_args_2*
&dense_2_statefulpartitionedcall_args_1*
&dense_2_statefulpartitionedcall_args_2*
&dense_3_statefulpartitionedcall_args_1*
&dense_3_statefulpartitionedcall_args_2*
&dense_4_statefulpartitionedcall_args_1*
&dense_4_statefulpartitionedcall_args_2*
&dense_5_statefulpartitionedcall_args_1*
&dense_5_statefulpartitionedcall_args_2
identity??dense/StatefulPartitionedCall?dense_1/StatefulPartitionedCall?dense_2/StatefulPartitionedCall?dense_3/StatefulPartitionedCall?dense_4/StatefulPartitionedCall?dense_5/StatefulPartitionedCall?
dense/StatefulPartitionedCallStatefulPartitionedCallinputs$dense_statefulpartitionedcall_args_1$dense_statefulpartitionedcall_args_2*I
fDRB
@__inference_dense_layer_call_and_return_conditional_losses_63113*(
_output_shapes
:??????????**
config_proto

GPU 

CPU2J 8*
Tin
2*
Tout
2*,
_gradient_op_typePartitionedCall-63119?
dense_1/StatefulPartitionedCallStatefulPartitionedCall&dense/StatefulPartitionedCall:output:0&dense_1_statefulpartitionedcall_args_1&dense_1_statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*(
_output_shapes
:??????????*K
fFRD
B__inference_dense_1_layer_call_and_return_conditional_losses_63141*,
_gradient_op_typePartitionedCall-63147*
Tout
2*
Tin
2?
dense_2/StatefulPartitionedCallStatefulPartitionedCall(dense_1/StatefulPartitionedCall:output:0&dense_2_statefulpartitionedcall_args_1&dense_2_statefulpartitionedcall_args_2*,
_gradient_op_typePartitionedCall-63175*'
_output_shapes
:?????????8*
Tout
2*K
fFRD
B__inference_dense_2_layer_call_and_return_conditional_losses_63169*
Tin
2**
config_proto

GPU 

CPU2J 8?
dense_3/StatefulPartitionedCallStatefulPartitionedCall(dense_2/StatefulPartitionedCall:output:0&dense_3_statefulpartitionedcall_args_1&dense_3_statefulpartitionedcall_args_2*'
_output_shapes
:?????????8*,
_gradient_op_typePartitionedCall-63203*
Tin
2**
config_proto

GPU 

CPU2J 8*
Tout
2*K
fFRD
B__inference_dense_3_layer_call_and_return_conditional_losses_63197?
dense_4/StatefulPartitionedCallStatefulPartitionedCall(dense_3/StatefulPartitionedCall:output:0&dense_4_statefulpartitionedcall_args_1&dense_4_statefulpartitionedcall_args_2*K
fFRD
B__inference_dense_4_layer_call_and_return_conditional_losses_63225**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-63231*'
_output_shapes
:?????????8*
Tin
2*
Tout
2?
dense_5/StatefulPartitionedCallStatefulPartitionedCall(dense_4/StatefulPartitionedCall:output:0&dense_5_statefulpartitionedcall_args_1&dense_5_statefulpartitionedcall_args_2*K
fFRD
B__inference_dense_5_layer_call_and_return_conditional_losses_63253*,
_gradient_op_typePartitionedCall-63259*
Tin
2*'
_output_shapes
:?????????8**
config_proto

GPU 

CPU2J 8*
Tout
2?
dropout/PartitionedCallPartitionedCall(dense_5/StatefulPartitionedCall:output:0**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-63309*
Tout
2*
Tin
2*'
_output_shapes
:?????????8*K
fFRD
B__inference_dropout_layer_call_and_return_conditional_losses_63297?
IdentityIdentity dropout/PartitionedCall:output:0^dense/StatefulPartitionedCall ^dense_1/StatefulPartitionedCall ^dense_2/StatefulPartitionedCall ^dense_3/StatefulPartitionedCall ^dense_4/StatefulPartitionedCall ^dense_5/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????8"
identityIdentity:output:0*W
_input_shapesF
D:??????????::::::::::::2B
dense_4/StatefulPartitionedCalldense_4/StatefulPartitionedCall2B
dense_5/StatefulPartitionedCalldense_5/StatefulPartitionedCall2>
dense/StatefulPartitionedCalldense/StatefulPartitionedCall2B
dense_1/StatefulPartitionedCalldense_1/StatefulPartitionedCall2B
dense_2/StatefulPartitionedCalldense_2/StatefulPartitionedCall2B
dense_3/StatefulPartitionedCalldense_3/StatefulPartitionedCall:& "
 
_user_specified_nameinputs: : : : : : : : :	 :
 : : 
?
a
B__inference_dropout_layer_call_and_return_conditional_losses_63290

inputs
identity?Q
dropout/rateConst*
_output_shapes
: *
valueB
 *??L>*
dtype0C
dropout/ShapeShapeinputs*
_output_shapes
:*
T0_
dropout/random_uniform/minConst*
_output_shapes
: *
valueB
 *    *
dtype0_
dropout/random_uniform/maxConst*
valueB
 *  ??*
_output_shapes
: *
dtype0?
$dropout/random_uniform/RandomUniformRandomUniformdropout/Shape:output:0*'
_output_shapes
:?????????8*
T0*
dtype0?
dropout/random_uniform/subSub#dropout/random_uniform/max:output:0#dropout/random_uniform/min:output:0*
_output_shapes
: *
T0?
dropout/random_uniform/mulMul-dropout/random_uniform/RandomUniform:output:0dropout/random_uniform/sub:z:0*
T0*'
_output_shapes
:?????????8?
dropout/random_uniformAdddropout/random_uniform/mul:z:0#dropout/random_uniform/min:output:0*
T0*'
_output_shapes
:?????????8R
dropout/sub/xConst*
dtype0*
_output_shapes
: *
valueB
 *  ??b
dropout/subSubdropout/sub/x:output:0dropout/rate:output:0*
_output_shapes
: *
T0V
dropout/truediv/xConst*
_output_shapes
: *
valueB
 *  ??*
dtype0h
dropout/truedivRealDivdropout/truediv/x:output:0dropout/sub:z:0*
_output_shapes
: *
T0?
dropout/GreaterEqualGreaterEqualdropout/random_uniform:z:0dropout/rate:output:0*'
_output_shapes
:?????????8*
T0a
dropout/mulMulinputsdropout/truediv:z:0*'
_output_shapes
:?????????8*
T0o
dropout/CastCastdropout/GreaterEqual:z:0*'
_output_shapes
:?????????8*

SrcT0
*

DstT0i
dropout/mul_1Muldropout/mul:z:0dropout/Cast:y:0*
T0*'
_output_shapes
:?????????8Y
IdentityIdentitydropout/mul_1:z:0*'
_output_shapes
:?????????8*
T0"
identityIdentity:output:0*&
_input_shapes
:?????????8:& "
 
_user_specified_nameinputs
?
?
'__inference_dense_4_layer_call_fn_64214

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*,
_gradient_op_typePartitionedCall-63231*
Tin
2**
config_proto

GPU 

CPU2J 8*K
fFRD
B__inference_dense_4_layer_call_and_return_conditional_losses_63225*'
_output_shapes
:?????????8*
Tout
2?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????8"
identityIdentity:output:0*.
_input_shapes
:?????????8::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
?
G__inference_sequential_3_layer_call_and_return_conditional_losses_63458
input_1*
&dense_6_statefulpartitionedcall_args_1*
&dense_6_statefulpartitionedcall_args_2
identity??dense_6/StatefulPartitionedCall?
dense_6/StatefulPartitionedCallStatefulPartitionedCallinput_1&dense_6_statefulpartitionedcall_args_1&dense_6_statefulpartitionedcall_args_2*
Tout
2**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-63446*K
fFRD
B__inference_dense_6_layer_call_and_return_conditional_losses_63440*'
_output_shapes
:?????????*
Tin
2?
IdentityIdentity(dense_6/StatefulPartitionedCall:output:0 ^dense_6/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????8::2B
dense_6/StatefulPartitionedCalldense_6/StatefulPartitionedCall:' #
!
_user_specified_name	input_1: : 
?V
?
__inference__traced_save_64457
file_prefix(
$savev2_adam_iter_read_readvariableop	*
&savev2_adam_beta_1_read_readvariableop*
&savev2_adam_beta_2_read_readvariableop)
%savev2_adam_decay_read_readvariableop1
-savev2_adam_learning_rate_read_readvariableop-
)savev2_dense_8_kernel_read_readvariableop+
'savev2_dense_8_bias_read_readvariableop/
+savev2_dense_1_2_kernel_read_readvariableop-
)savev2_dense_1_2_bias_read_readvariableop/
+savev2_dense_2_2_kernel_read_readvariableop-
)savev2_dense_2_2_bias_read_readvariableop/
+savev2_dense_3_2_kernel_read_readvariableop-
)savev2_dense_3_2_bias_read_readvariableop/
+savev2_dense_4_2_kernel_read_readvariableop-
)savev2_dense_4_2_bias_read_readvariableop/
+savev2_dense_5_2_kernel_read_readvariableop-
)savev2_dense_5_2_bias_read_readvariableop/
+savev2_dense_6_3_kernel_read_readvariableop-
)savev2_dense_6_3_bias_read_readvariableop$
 savev2_total_read_readvariableop$
 savev2_count_read_readvariableop4
0savev2_adam_dense_8_kernel_m_read_readvariableop2
.savev2_adam_dense_8_bias_m_read_readvariableop6
2savev2_adam_dense_1_2_kernel_m_read_readvariableop4
0savev2_adam_dense_1_2_bias_m_read_readvariableop6
2savev2_adam_dense_2_2_kernel_m_read_readvariableop4
0savev2_adam_dense_2_2_bias_m_read_readvariableop6
2savev2_adam_dense_3_2_kernel_m_read_readvariableop4
0savev2_adam_dense_3_2_bias_m_read_readvariableop6
2savev2_adam_dense_4_2_kernel_m_read_readvariableop4
0savev2_adam_dense_4_2_bias_m_read_readvariableop6
2savev2_adam_dense_5_2_kernel_m_read_readvariableop4
0savev2_adam_dense_5_2_bias_m_read_readvariableop6
2savev2_adam_dense_6_3_kernel_m_read_readvariableop4
0savev2_adam_dense_6_3_bias_m_read_readvariableop4
0savev2_adam_dense_8_kernel_v_read_readvariableop2
.savev2_adam_dense_8_bias_v_read_readvariableop6
2savev2_adam_dense_1_2_kernel_v_read_readvariableop4
0savev2_adam_dense_1_2_bias_v_read_readvariableop6
2savev2_adam_dense_2_2_kernel_v_read_readvariableop4
0savev2_adam_dense_2_2_bias_v_read_readvariableop6
2savev2_adam_dense_3_2_kernel_v_read_readvariableop4
0savev2_adam_dense_3_2_bias_v_read_readvariableop6
2savev2_adam_dense_4_2_kernel_v_read_readvariableop4
0savev2_adam_dense_4_2_bias_v_read_readvariableop6
2savev2_adam_dense_5_2_kernel_v_read_readvariableop4
0savev2_adam_dense_5_2_bias_v_read_readvariableop6
2savev2_adam_dense_6_3_kernel_v_read_readvariableop4
0savev2_adam_dense_6_3_bias_v_read_readvariableop
savev2_1_const

identity_1??MergeV2Checkpoints?SaveV2?SaveV2_1?
StringJoin/inputs_1Const"/device:CPU:0*<
value3B1 B+_temp_f6e06501bfd445babf997a019f924756/part*
_output_shapes
: *
dtype0s

StringJoin
StringJoinfile_prefixStringJoin/inputs_1:output:0"/device:CPU:0*
_output_shapes
: *
NL

num_shardsConst*
_output_shapes
: *
dtype0*
value	B :f
ShardedFilename/shardConst"/device:CPU:0*
dtype0*
_output_shapes
: *
value	B : ?
ShardedFilenameShardedFilenameStringJoin:output:0ShardedFilename/shard:output:0num_shards:output:0"/device:CPU:0*
_output_shapes
: ?
SaveV2/tensor_namesConst"/device:CPU:0*
_output_shapes
:1*
dtype0*?
value?B?1B)optimizer/iter/.ATTRIBUTES/VARIABLE_VALUEB+optimizer/beta_1/.ATTRIBUTES/VARIABLE_VALUEB+optimizer/beta_2/.ATTRIBUTES/VARIABLE_VALUEB*optimizer/decay/.ATTRIBUTES/VARIABLE_VALUEB2optimizer/learning_rate/.ATTRIBUTES/VARIABLE_VALUEB&variables/0/.ATTRIBUTES/VARIABLE_VALUEB&variables/1/.ATTRIBUTES/VARIABLE_VALUEB&variables/2/.ATTRIBUTES/VARIABLE_VALUEB&variables/3/.ATTRIBUTES/VARIABLE_VALUEB&variables/4/.ATTRIBUTES/VARIABLE_VALUEB&variables/5/.ATTRIBUTES/VARIABLE_VALUEB&variables/6/.ATTRIBUTES/VARIABLE_VALUEB&variables/7/.ATTRIBUTES/VARIABLE_VALUEB&variables/8/.ATTRIBUTES/VARIABLE_VALUEB&variables/9/.ATTRIBUTES/VARIABLE_VALUEB'variables/10/.ATTRIBUTES/VARIABLE_VALUEB'variables/11/.ATTRIBUTES/VARIABLE_VALUEB'variables/12/.ATTRIBUTES/VARIABLE_VALUEB'variables/13/.ATTRIBUTES/VARIABLE_VALUEB4keras_api/metrics/0/total/.ATTRIBUTES/VARIABLE_VALUEB4keras_api/metrics/0/count/.ATTRIBUTES/VARIABLE_VALUEBBvariables/0/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/1/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/2/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/3/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/4/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/5/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/6/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/7/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/8/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/9/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBCvariables/10/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBCvariables/11/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBCvariables/12/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBCvariables/13/.OPTIMIZER_SLOT/optimizer/m/.ATTRIBUTES/VARIABLE_VALUEBBvariables/0/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/1/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/2/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/3/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/4/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/5/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/6/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/7/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/8/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBBvariables/9/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBCvariables/10/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBCvariables/11/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBCvariables/12/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUEBCvariables/13/.OPTIMIZER_SLOT/optimizer/v/.ATTRIBUTES/VARIABLE_VALUE?
SaveV2/shape_and_slicesConst"/device:CPU:0*
dtype0*
_output_shapes
:1*u
valuelBj1B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B B ?
SaveV2SaveV2ShardedFilename:filename:0SaveV2/tensor_names:output:0 SaveV2/shape_and_slices:output:0$savev2_adam_iter_read_readvariableop&savev2_adam_beta_1_read_readvariableop&savev2_adam_beta_2_read_readvariableop%savev2_adam_decay_read_readvariableop-savev2_adam_learning_rate_read_readvariableop)savev2_dense_8_kernel_read_readvariableop'savev2_dense_8_bias_read_readvariableop+savev2_dense_1_2_kernel_read_readvariableop)savev2_dense_1_2_bias_read_readvariableop+savev2_dense_2_2_kernel_read_readvariableop)savev2_dense_2_2_bias_read_readvariableop+savev2_dense_3_2_kernel_read_readvariableop)savev2_dense_3_2_bias_read_readvariableop+savev2_dense_4_2_kernel_read_readvariableop)savev2_dense_4_2_bias_read_readvariableop+savev2_dense_5_2_kernel_read_readvariableop)savev2_dense_5_2_bias_read_readvariableop+savev2_dense_6_3_kernel_read_readvariableop)savev2_dense_6_3_bias_read_readvariableop savev2_total_read_readvariableop savev2_count_read_readvariableop0savev2_adam_dense_8_kernel_m_read_readvariableop.savev2_adam_dense_8_bias_m_read_readvariableop2savev2_adam_dense_1_2_kernel_m_read_readvariableop0savev2_adam_dense_1_2_bias_m_read_readvariableop2savev2_adam_dense_2_2_kernel_m_read_readvariableop0savev2_adam_dense_2_2_bias_m_read_readvariableop2savev2_adam_dense_3_2_kernel_m_read_readvariableop0savev2_adam_dense_3_2_bias_m_read_readvariableop2savev2_adam_dense_4_2_kernel_m_read_readvariableop0savev2_adam_dense_4_2_bias_m_read_readvariableop2savev2_adam_dense_5_2_kernel_m_read_readvariableop0savev2_adam_dense_5_2_bias_m_read_readvariableop2savev2_adam_dense_6_3_kernel_m_read_readvariableop0savev2_adam_dense_6_3_bias_m_read_readvariableop0savev2_adam_dense_8_kernel_v_read_readvariableop.savev2_adam_dense_8_bias_v_read_readvariableop2savev2_adam_dense_1_2_kernel_v_read_readvariableop0savev2_adam_dense_1_2_bias_v_read_readvariableop2savev2_adam_dense_2_2_kernel_v_read_readvariableop0savev2_adam_dense_2_2_bias_v_read_readvariableop2savev2_adam_dense_3_2_kernel_v_read_readvariableop0savev2_adam_dense_3_2_bias_v_read_readvariableop2savev2_adam_dense_4_2_kernel_v_read_readvariableop0savev2_adam_dense_4_2_bias_v_read_readvariableop2savev2_adam_dense_5_2_kernel_v_read_readvariableop0savev2_adam_dense_5_2_bias_v_read_readvariableop2savev2_adam_dense_6_3_kernel_v_read_readvariableop0savev2_adam_dense_6_3_bias_v_read_readvariableop"/device:CPU:0*?
dtypes5
321	*
_output_shapes
 h
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
SaveV2_1/shape_and_slicesConst"/device:CPU:0*
_output_shapes
:*
dtype0*
valueB
B ?
SaveV2_1SaveV2ShardedFilename_1:filename:0SaveV2_1/tensor_names:output:0"SaveV2_1/shape_and_slices:output:0savev2_1_const^SaveV2"/device:CPU:0*
dtypes
2*
_output_shapes
 ?
&MergeV2Checkpoints/checkpoint_prefixesPackShardedFilename:filename:0ShardedFilename_1:filename:0^SaveV2	^SaveV2_1"/device:CPU:0*
N*
T0*
_output_shapes
:?
MergeV2CheckpointsMergeV2Checkpoints/MergeV2Checkpoints/checkpoint_prefixes:output:0file_prefix	^SaveV2_1"/device:CPU:0*
_output_shapes
 f
IdentityIdentityfile_prefix^MergeV2Checkpoints"/device:CPU:0*
T0*
_output_shapes
: s

Identity_1IdentityIdentity:output:0^MergeV2Checkpoints^SaveV2	^SaveV2_1*
_output_shapes
: *
T0"!

identity_1Identity_1:output:0*?
_input_shapes?
?: : : : : : :
??:?:
??:?:	?8:8:88:8:88:8:88:8:8:: : :
??:?:
??:?:	?8:8:88:8:88:8:88:8:8::
??:?:
??:?:	?8:8:88:8:88:8:88:8:8:: 2
SaveV2_1SaveV2_12(
MergeV2CheckpointsMergeV2Checkpoints2
SaveV2SaveV2:+ '
%
_user_specified_namefile_prefix: : : : : : : : :	 :
 : : : : : : : : : : : : : : : : : : : : : :  :! :" :# :$ :% :& :' :( :) :* :+ :, :- :. :/ :0 :1 :2 
?
^
B__inference_flatten_layer_call_and_return_conditional_losses_63018

inputs
identity^
Reshape/shapeConst*
dtype0*
valueB"?????   *
_output_shapes
:e
ReshapeReshapeinputsReshape/shape:output:0*(
_output_shapes
:??????????*
T0Y
IdentityIdentityReshape:output:0*
T0*(
_output_shapes
:??????????"
identityIdentity:output:0*'
_input_shapes
:??????????:& "
 
_user_specified_nameinputs
?
?
,__inference_sequential_2_layer_call_fn_64077

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2"
statefulpartitionedcall_args_3"
statefulpartitionedcall_args_4"
statefulpartitionedcall_args_5"
statefulpartitionedcall_args_6"
statefulpartitionedcall_args_7"
statefulpartitionedcall_args_8"
statefulpartitionedcall_args_9#
statefulpartitionedcall_args_10#
statefulpartitionedcall_args_11#
statefulpartitionedcall_args_12
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2statefulpartitionedcall_args_3statefulpartitionedcall_args_4statefulpartitionedcall_args_5statefulpartitionedcall_args_6statefulpartitionedcall_args_7statefulpartitionedcall_args_8statefulpartitionedcall_args_9statefulpartitionedcall_args_10statefulpartitionedcall_args_11statefulpartitionedcall_args_12*,
_gradient_op_typePartitionedCall-63410**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:?????????8*
Tout
2*P
fKRI
G__inference_sequential_2_layer_call_and_return_conditional_losses_63409*
Tin
2?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????8"
identityIdentity:output:0*W
_input_shapesF
D:??????????::::::::::::22
StatefulPartitionedCallStatefulPartitionedCall: : : :	 :
 : : :& "
 
_user_specified_nameinputs: : : : : 
?	
?
B__inference_dense_1_layer_call_and_return_conditional_losses_63141

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0* 
_output_shapes
:
??j
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*
T0*(
_output_shapes
:???????????
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes	
:?w
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*(
_output_shapes
:??????????*
T0Q
ReluReluBiasAdd:output:0*
T0*(
_output_shapes
:???????????
IdentityIdentityRelu:activations:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*(
_output_shapes
:??????????*
T0"
identityIdentity:output:0*/
_input_shapes
:??????????::20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp:& "
 
_user_specified_nameinputs: : 
?
?
G__inference_sequential_4_layer_call_and_return_conditional_losses_63684

inputs/
+sequential_2_statefulpartitionedcall_args_1/
+sequential_2_statefulpartitionedcall_args_2/
+sequential_2_statefulpartitionedcall_args_3/
+sequential_2_statefulpartitionedcall_args_4/
+sequential_2_statefulpartitionedcall_args_5/
+sequential_2_statefulpartitionedcall_args_6/
+sequential_2_statefulpartitionedcall_args_7/
+sequential_2_statefulpartitionedcall_args_8/
+sequential_2_statefulpartitionedcall_args_90
,sequential_2_statefulpartitionedcall_args_100
,sequential_2_statefulpartitionedcall_args_110
,sequential_2_statefulpartitionedcall_args_12/
+sequential_3_statefulpartitionedcall_args_1/
+sequential_3_statefulpartitionedcall_args_2
identity??$sequential_2/StatefulPartitionedCall?$sequential_3/StatefulPartitionedCall?
sequential_1/PartitionedCallPartitionedCallinputs*P
fKRI
G__inference_sequential_1_layer_call_and_return_conditional_losses_63060*
Tin
2*
Tout
2**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-63061*(
_output_shapes
:???????????
sequential/PartitionedCallPartitionedCall%sequential_1/PartitionedCall:output:0*
Tout
2*
Tin
2*(
_output_shapes
:??????????**
config_proto

GPU 

CPU2J 8*N
fIRG
E__inference_sequential_layer_call_and_return_conditional_losses_63094*,
_gradient_op_typePartitionedCall-63095?
$sequential_2/StatefulPartitionedCallStatefulPartitionedCall#sequential/PartitionedCall:output:0+sequential_2_statefulpartitionedcall_args_1+sequential_2_statefulpartitionedcall_args_2+sequential_2_statefulpartitionedcall_args_3+sequential_2_statefulpartitionedcall_args_4+sequential_2_statefulpartitionedcall_args_5+sequential_2_statefulpartitionedcall_args_6+sequential_2_statefulpartitionedcall_args_7+sequential_2_statefulpartitionedcall_args_8+sequential_2_statefulpartitionedcall_args_9,sequential_2_statefulpartitionedcall_args_10,sequential_2_statefulpartitionedcall_args_11,sequential_2_statefulpartitionedcall_args_12*,
_gradient_op_typePartitionedCall-63410*
Tin
2**
config_proto

GPU 

CPU2J 8*
Tout
2*'
_output_shapes
:?????????8*P
fKRI
G__inference_sequential_2_layer_call_and_return_conditional_losses_63409?
$sequential_3/StatefulPartitionedCallStatefulPartitionedCall-sequential_2/StatefulPartitionedCall:output:0+sequential_3_statefulpartitionedcall_args_1+sequential_3_statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*P
fKRI
G__inference_sequential_3_layer_call_and_return_conditional_losses_63494*
Tin
2*,
_gradient_op_typePartitionedCall-63495*'
_output_shapes
:?????????*
Tout
2?
IdentityIdentity-sequential_3/StatefulPartitionedCall:output:0%^sequential_2/StatefulPartitionedCall%^sequential_3/StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*_
_input_shapesN
L:??????????::::::::::::::2L
$sequential_2/StatefulPartitionedCall$sequential_2/StatefulPartitionedCall2L
$sequential_3/StatefulPartitionedCall$sequential_3/StatefulPartitionedCall:& "
 
_user_specified_nameinputs: : : : : : : : :	 :
 : : : : 
?7
?
G__inference_sequential_2_layer_call_and_return_conditional_losses_64043

inputs(
$dense_matmul_readvariableop_resource)
%dense_biasadd_readvariableop_resource*
&dense_1_matmul_readvariableop_resource+
'dense_1_biasadd_readvariableop_resource*
&dense_2_matmul_readvariableop_resource+
'dense_2_biasadd_readvariableop_resource*
&dense_3_matmul_readvariableop_resource+
'dense_3_biasadd_readvariableop_resource*
&dense_4_matmul_readvariableop_resource+
'dense_4_biasadd_readvariableop_resource*
&dense_5_matmul_readvariableop_resource+
'dense_5_biasadd_readvariableop_resource
identity??dense/BiasAdd/ReadVariableOp?dense/MatMul/ReadVariableOp?dense_1/BiasAdd/ReadVariableOp?dense_1/MatMul/ReadVariableOp?dense_2/BiasAdd/ReadVariableOp?dense_2/MatMul/ReadVariableOp?dense_3/BiasAdd/ReadVariableOp?dense_3/MatMul/ReadVariableOp?dense_4/BiasAdd/ReadVariableOp?dense_4/MatMul/ReadVariableOp?dense_5/BiasAdd/ReadVariableOp?dense_5/MatMul/ReadVariableOp?
dense/MatMul/ReadVariableOpReadVariableOp$dense_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0* 
_output_shapes
:
??*
dtype0v
dense/MatMulMatMulinputs#dense/MatMul/ReadVariableOp:value:0*(
_output_shapes
:??????????*
T0?
dense/BiasAdd/ReadVariableOpReadVariableOp%dense_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes	
:??
dense/BiasAddBiasAdddense/MatMul:product:0$dense/BiasAdd/ReadVariableOp:value:0*
T0*(
_output_shapes
:??????????]

dense/ReluReludense/BiasAdd:output:0*
T0*(
_output_shapes
:???????????
dense_1/MatMul/ReadVariableOpReadVariableOp&dense_1_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0* 
_output_shapes
:
??*
dtype0?
dense_1/MatMulMatMuldense/Relu:activations:0%dense_1/MatMul/ReadVariableOp:value:0*
T0*(
_output_shapes
:???????????
dense_1/BiasAdd/ReadVariableOpReadVariableOp'dense_1_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes	
:??
dense_1/BiasAddBiasAdddense_1/MatMul:product:0&dense_1/BiasAdd/ReadVariableOp:value:0*
T0*(
_output_shapes
:??????????a
dense_1/ReluReludense_1/BiasAdd:output:0*(
_output_shapes
:??????????*
T0?
dense_2/MatMul/ReadVariableOpReadVariableOp&dense_2_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:	?8*
dtype0?
dense_2/MatMulMatMuldense_1/Relu:activations:0%dense_2/MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8?
dense_2/BiasAdd/ReadVariableOpReadVariableOp'dense_2_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:8?
dense_2/BiasAddBiasAdddense_2/MatMul:product:0&dense_2/BiasAdd/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0`
dense_2/ReluReludense_2/BiasAdd:output:0*
T0*'
_output_shapes
:?????????8?
dense_3/MatMul/ReadVariableOpReadVariableOp&dense_3_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes

:88*
dtype0?
dense_3/MatMulMatMuldense_2/Relu:activations:0%dense_3/MatMul/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0?
dense_3/BiasAdd/ReadVariableOpReadVariableOp'dense_3_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:8*
dtype0?
dense_3/BiasAddBiasAdddense_3/MatMul:product:0&dense_3/BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8`
dense_3/ReluReludense_3/BiasAdd:output:0*
T0*'
_output_shapes
:?????????8?
dense_4/MatMul/ReadVariableOpReadVariableOp&dense_4_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes

:88*
dtype0?
dense_4/MatMulMatMuldense_3/Relu:activations:0%dense_4/MatMul/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0?
dense_4/BiasAdd/ReadVariableOpReadVariableOp'dense_4_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:8?
dense_4/BiasAddBiasAdddense_4/MatMul:product:0&dense_4/BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8`
dense_4/ReluReludense_4/BiasAdd:output:0*'
_output_shapes
:?????????8*
T0?
dense_5/MatMul/ReadVariableOpReadVariableOp&dense_5_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes

:88*
dtype0?
dense_5/MatMulMatMuldense_4/Relu:activations:0%dense_5/MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8?
dense_5/BiasAdd/ReadVariableOpReadVariableOp'dense_5_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:8*
dtype0?
dense_5/BiasAddBiasAdddense_5/MatMul:product:0&dense_5/BiasAdd/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0`
dense_5/ReluReludense_5/BiasAdd:output:0*
T0*'
_output_shapes
:?????????8j
dropout/IdentityIdentitydense_5/Relu:activations:0*'
_output_shapes
:?????????8*
T0?
IdentityIdentitydropout/Identity:output:0^dense/BiasAdd/ReadVariableOp^dense/MatMul/ReadVariableOp^dense_1/BiasAdd/ReadVariableOp^dense_1/MatMul/ReadVariableOp^dense_2/BiasAdd/ReadVariableOp^dense_2/MatMul/ReadVariableOp^dense_3/BiasAdd/ReadVariableOp^dense_3/MatMul/ReadVariableOp^dense_4/BiasAdd/ReadVariableOp^dense_4/MatMul/ReadVariableOp^dense_5/BiasAdd/ReadVariableOp^dense_5/MatMul/ReadVariableOp*'
_output_shapes
:?????????8*
T0"
identityIdentity:output:0*W
_input_shapesF
D:??????????::::::::::::2@
dense_2/BiasAdd/ReadVariableOpdense_2/BiasAdd/ReadVariableOp2>
dense_1/MatMul/ReadVariableOpdense_1/MatMul/ReadVariableOp2@
dense_5/BiasAdd/ReadVariableOpdense_5/BiasAdd/ReadVariableOp2>
dense_5/MatMul/ReadVariableOpdense_5/MatMul/ReadVariableOp2>
dense_2/MatMul/ReadVariableOpdense_2/MatMul/ReadVariableOp2@
dense_3/BiasAdd/ReadVariableOpdense_3/BiasAdd/ReadVariableOp2:
dense/MatMul/ReadVariableOpdense/MatMul/ReadVariableOp2<
dense/BiasAdd/ReadVariableOpdense/BiasAdd/ReadVariableOp2@
dense_1/BiasAdd/ReadVariableOpdense_1/BiasAdd/ReadVariableOp2>
dense_3/MatMul/ReadVariableOpdense_3/MatMul/ReadVariableOp2@
dense_4/BiasAdd/ReadVariableOpdense_4/BiasAdd/ReadVariableOp2>
dense_4/MatMul/ReadVariableOpdense_4/MatMul/ReadVariableOp: : : : : : : :	 :
 : : :& "
 
_user_specified_nameinputs: 
?
?
'__inference_dense_2_layer_call_fn_64178

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*'
_output_shapes
:?????????8*
Tout
2*
Tin
2*K
fFRD
B__inference_dense_2_layer_call_and_return_conditional_losses_63169**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-63175?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????8"
identityIdentity:output:0*/
_input_shapes
:??????????::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?	
?
B__inference_dense_3_layer_call_and_return_conditional_losses_63197

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes

:88*
dtype0i
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8?
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:8*
dtype0v
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0P
ReluReluBiasAdd:output:0*
T0*'
_output_shapes
:?????????8?
IdentityIdentityRelu:activations:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*
T0*'
_output_shapes
:?????????8"
identityIdentity:output:0*.
_input_shapes
:?????????8::20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp:& "
 
_user_specified_nameinputs: : 
?
?
G__inference_sequential_3_layer_call_and_return_conditional_losses_64088

inputs*
&dense_6_matmul_readvariableop_resource+
'dense_6_biasadd_readvariableop_resource
identity??dense_6/BiasAdd/ReadVariableOp?dense_6/MatMul/ReadVariableOp?
dense_6/MatMul/ReadVariableOpReadVariableOp&dense_6_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes

:8y
dense_6/MatMulMatMulinputs%dense_6/MatMul/ReadVariableOp:value:0*'
_output_shapes
:?????????*
T0?
dense_6/BiasAdd/ReadVariableOpReadVariableOp'dense_6_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:*
dtype0?
dense_6/BiasAddBiasAdddense_6/MatMul:product:0&dense_6/BiasAdd/ReadVariableOp:value:0*'
_output_shapes
:?????????*
T0f
dense_6/SoftmaxSoftmaxdense_6/BiasAdd:output:0*
T0*'
_output_shapes
:??????????
IdentityIdentitydense_6/Softmax:softmax:0^dense_6/BiasAdd/ReadVariableOp^dense_6/MatMul/ReadVariableOp*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*.
_input_shapes
:?????????8::2@
dense_6/BiasAdd/ReadVariableOpdense_6/BiasAdd/ReadVariableOp2>
dense_6/MatMul/ReadVariableOpdense_6/MatMul/ReadVariableOp:& "
 
_user_specified_nameinputs: : 
?
`
'__inference_dropout_layer_call_fn_64262

inputs
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputs**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-63301*
Tout
2*K
fFRD
B__inference_dropout_layer_call_and_return_conditional_losses_63290*
Tin
2*'
_output_shapes
:?????????8?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*'
_output_shapes
:?????????8*
T0"
identityIdentity:output:0*&
_input_shapes
:?????????822
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs
?e
?
G__inference_sequential_4_layer_call_and_return_conditional_losses_63800

inputs5
1sequential_2_dense_matmul_readvariableop_resource6
2sequential_2_dense_biasadd_readvariableop_resource7
3sequential_2_dense_1_matmul_readvariableop_resource8
4sequential_2_dense_1_biasadd_readvariableop_resource7
3sequential_2_dense_2_matmul_readvariableop_resource8
4sequential_2_dense_2_biasadd_readvariableop_resource7
3sequential_2_dense_3_matmul_readvariableop_resource8
4sequential_2_dense_3_biasadd_readvariableop_resource7
3sequential_2_dense_4_matmul_readvariableop_resource8
4sequential_2_dense_4_biasadd_readvariableop_resource7
3sequential_2_dense_5_matmul_readvariableop_resource8
4sequential_2_dense_5_biasadd_readvariableop_resource7
3sequential_3_dense_6_matmul_readvariableop_resource8
4sequential_3_dense_6_biasadd_readvariableop_resource
identity??)sequential_2/dense/BiasAdd/ReadVariableOp?(sequential_2/dense/MatMul/ReadVariableOp?+sequential_2/dense_1/BiasAdd/ReadVariableOp?*sequential_2/dense_1/MatMul/ReadVariableOp?+sequential_2/dense_2/BiasAdd/ReadVariableOp?*sequential_2/dense_2/MatMul/ReadVariableOp?+sequential_2/dense_3/BiasAdd/ReadVariableOp?*sequential_2/dense_3/MatMul/ReadVariableOp?+sequential_2/dense_4/BiasAdd/ReadVariableOp?*sequential_2/dense_4/MatMul/ReadVariableOp?+sequential_2/dense_5/BiasAdd/ReadVariableOp?*sequential_2/dense_5/MatMul/ReadVariableOp?+sequential_3/dense_6/BiasAdd/ReadVariableOp?*sequential_3/dense_6/MatMul/ReadVariableOps
"sequential_1/flatten/Reshape/shapeConst*
valueB"?????   *
dtype0*
_output_shapes
:?
sequential_1/flatten/ReshapeReshapeinputs+sequential_1/flatten/Reshape/shape:output:0*(
_output_shapes
:??????????*
T0?
(sequential_2/dense/MatMul/ReadVariableOpReadVariableOp1sequential_2_dense_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0* 
_output_shapes
:
??*
dtype0?
sequential_2/dense/MatMulMatMul%sequential_1/flatten/Reshape:output:00sequential_2/dense/MatMul/ReadVariableOp:value:0*
T0*(
_output_shapes
:???????????
)sequential_2/dense/BiasAdd/ReadVariableOpReadVariableOp2sequential_2_dense_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes	
:?*
dtype0?
sequential_2/dense/BiasAddBiasAdd#sequential_2/dense/MatMul:product:01sequential_2/dense/BiasAdd/ReadVariableOp:value:0*
T0*(
_output_shapes
:??????????w
sequential_2/dense/ReluRelu#sequential_2/dense/BiasAdd:output:0*(
_output_shapes
:??????????*
T0?
*sequential_2/dense_1/MatMul/ReadVariableOpReadVariableOp3sequential_2_dense_1_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0* 
_output_shapes
:
???
sequential_2/dense_1/MatMulMatMul%sequential_2/dense/Relu:activations:02sequential_2/dense_1/MatMul/ReadVariableOp:value:0*(
_output_shapes
:??????????*
T0?
+sequential_2/dense_1/BiasAdd/ReadVariableOpReadVariableOp4sequential_2_dense_1_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes	
:??
sequential_2/dense_1/BiasAddBiasAdd%sequential_2/dense_1/MatMul:product:03sequential_2/dense_1/BiasAdd/ReadVariableOp:value:0*
T0*(
_output_shapes
:??????????{
sequential_2/dense_1/ReluRelu%sequential_2/dense_1/BiasAdd:output:0*
T0*(
_output_shapes
:???????????
*sequential_2/dense_2/MatMul/ReadVariableOpReadVariableOp3sequential_2_dense_2_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:	?8?
sequential_2/dense_2/MatMulMatMul'sequential_2/dense_1/Relu:activations:02sequential_2/dense_2/MatMul/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0?
+sequential_2/dense_2/BiasAdd/ReadVariableOpReadVariableOp4sequential_2_dense_2_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:8*
dtype0?
sequential_2/dense_2/BiasAddBiasAdd%sequential_2/dense_2/MatMul:product:03sequential_2/dense_2/BiasAdd/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0z
sequential_2/dense_2/ReluRelu%sequential_2/dense_2/BiasAdd:output:0*
T0*'
_output_shapes
:?????????8?
*sequential_2/dense_3/MatMul/ReadVariableOpReadVariableOp3sequential_2_dense_3_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes

:88?
sequential_2/dense_3/MatMulMatMul'sequential_2/dense_2/Relu:activations:02sequential_2/dense_3/MatMul/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0?
+sequential_2/dense_3/BiasAdd/ReadVariableOpReadVariableOp4sequential_2_dense_3_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:8*
dtype0?
sequential_2/dense_3/BiasAddBiasAdd%sequential_2/dense_3/MatMul:product:03sequential_2/dense_3/BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8z
sequential_2/dense_3/ReluRelu%sequential_2/dense_3/BiasAdd:output:0*
T0*'
_output_shapes
:?????????8?
*sequential_2/dense_4/MatMul/ReadVariableOpReadVariableOp3sequential_2_dense_4_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes

:88?
sequential_2/dense_4/MatMulMatMul'sequential_2/dense_3/Relu:activations:02sequential_2/dense_4/MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8?
+sequential_2/dense_4/BiasAdd/ReadVariableOpReadVariableOp4sequential_2_dense_4_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:8?
sequential_2/dense_4/BiasAddBiasAdd%sequential_2/dense_4/MatMul:product:03sequential_2/dense_4/BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8z
sequential_2/dense_4/ReluRelu%sequential_2/dense_4/BiasAdd:output:0*'
_output_shapes
:?????????8*
T0?
*sequential_2/dense_5/MatMul/ReadVariableOpReadVariableOp3sequential_2_dense_5_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes

:88*
dtype0?
sequential_2/dense_5/MatMulMatMul'sequential_2/dense_4/Relu:activations:02sequential_2/dense_5/MatMul/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0?
+sequential_2/dense_5/BiasAdd/ReadVariableOpReadVariableOp4sequential_2_dense_5_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:8?
sequential_2/dense_5/BiasAddBiasAdd%sequential_2/dense_5/MatMul:product:03sequential_2/dense_5/BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8z
sequential_2/dense_5/ReluRelu%sequential_2/dense_5/BiasAdd:output:0*
T0*'
_output_shapes
:?????????8f
!sequential_2/dropout/dropout/rateConst*
dtype0*
_output_shapes
: *
valueB
 *??L>y
"sequential_2/dropout/dropout/ShapeShape'sequential_2/dense_5/Relu:activations:0*
T0*
_output_shapes
:t
/sequential_2/dropout/dropout/random_uniform/minConst*
valueB
 *    *
dtype0*
_output_shapes
: t
/sequential_2/dropout/dropout/random_uniform/maxConst*
_output_shapes
: *
valueB
 *  ??*
dtype0?
9sequential_2/dropout/dropout/random_uniform/RandomUniformRandomUniform+sequential_2/dropout/dropout/Shape:output:0*'
_output_shapes
:?????????8*
T0*
dtype0?
/sequential_2/dropout/dropout/random_uniform/subSub8sequential_2/dropout/dropout/random_uniform/max:output:08sequential_2/dropout/dropout/random_uniform/min:output:0*
T0*
_output_shapes
: ?
/sequential_2/dropout/dropout/random_uniform/mulMulBsequential_2/dropout/dropout/random_uniform/RandomUniform:output:03sequential_2/dropout/dropout/random_uniform/sub:z:0*'
_output_shapes
:?????????8*
T0?
+sequential_2/dropout/dropout/random_uniformAdd3sequential_2/dropout/dropout/random_uniform/mul:z:08sequential_2/dropout/dropout/random_uniform/min:output:0*'
_output_shapes
:?????????8*
T0g
"sequential_2/dropout/dropout/sub/xConst*
valueB
 *  ??*
_output_shapes
: *
dtype0?
 sequential_2/dropout/dropout/subSub+sequential_2/dropout/dropout/sub/x:output:0*sequential_2/dropout/dropout/rate:output:0*
_output_shapes
: *
T0k
&sequential_2/dropout/dropout/truediv/xConst*
_output_shapes
: *
dtype0*
valueB
 *  ???
$sequential_2/dropout/dropout/truedivRealDiv/sequential_2/dropout/dropout/truediv/x:output:0$sequential_2/dropout/dropout/sub:z:0*
T0*
_output_shapes
: ?
)sequential_2/dropout/dropout/GreaterEqualGreaterEqual/sequential_2/dropout/dropout/random_uniform:z:0*sequential_2/dropout/dropout/rate:output:0*
T0*'
_output_shapes
:?????????8?
 sequential_2/dropout/dropout/mulMul'sequential_2/dense_5/Relu:activations:0(sequential_2/dropout/dropout/truediv:z:0*'
_output_shapes
:?????????8*
T0?
!sequential_2/dropout/dropout/CastCast-sequential_2/dropout/dropout/GreaterEqual:z:0*

SrcT0
*'
_output_shapes
:?????????8*

DstT0?
"sequential_2/dropout/dropout/mul_1Mul$sequential_2/dropout/dropout/mul:z:0%sequential_2/dropout/dropout/Cast:y:0*'
_output_shapes
:?????????8*
T0?
*sequential_3/dense_6/MatMul/ReadVariableOpReadVariableOp3sequential_3_dense_6_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes

:8?
sequential_3/dense_6/MatMulMatMul&sequential_2/dropout/dropout/mul_1:z:02sequential_3/dense_6/MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:??????????
+sequential_3/dense_6/BiasAdd/ReadVariableOpReadVariableOp4sequential_3_dense_6_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:?
sequential_3/dense_6/BiasAddBiasAdd%sequential_3/dense_6/MatMul:product:03sequential_3/dense_6/BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:??????????
sequential_3/dense_6/SoftmaxSoftmax%sequential_3/dense_6/BiasAdd:output:0*
T0*'
_output_shapes
:??????????
IdentityIdentity&sequential_3/dense_6/Softmax:softmax:0*^sequential_2/dense/BiasAdd/ReadVariableOp)^sequential_2/dense/MatMul/ReadVariableOp,^sequential_2/dense_1/BiasAdd/ReadVariableOp+^sequential_2/dense_1/MatMul/ReadVariableOp,^sequential_2/dense_2/BiasAdd/ReadVariableOp+^sequential_2/dense_2/MatMul/ReadVariableOp,^sequential_2/dense_3/BiasAdd/ReadVariableOp+^sequential_2/dense_3/MatMul/ReadVariableOp,^sequential_2/dense_4/BiasAdd/ReadVariableOp+^sequential_2/dense_4/MatMul/ReadVariableOp,^sequential_2/dense_5/BiasAdd/ReadVariableOp+^sequential_2/dense_5/MatMul/ReadVariableOp,^sequential_3/dense_6/BiasAdd/ReadVariableOp+^sequential_3/dense_6/MatMul/ReadVariableOp*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*_
_input_shapesN
L:??????????::::::::::::::2X
*sequential_2/dense_1/MatMul/ReadVariableOp*sequential_2/dense_1/MatMul/ReadVariableOp2Z
+sequential_2/dense_4/BiasAdd/ReadVariableOp+sequential_2/dense_4/BiasAdd/ReadVariableOp2T
(sequential_2/dense/MatMul/ReadVariableOp(sequential_2/dense/MatMul/ReadVariableOp2X
*sequential_2/dense_5/MatMul/ReadVariableOp*sequential_2/dense_5/MatMul/ReadVariableOp2Z
+sequential_2/dense_2/BiasAdd/ReadVariableOp+sequential_2/dense_2/BiasAdd/ReadVariableOp2X
*sequential_2/dense_2/MatMul/ReadVariableOp*sequential_2/dense_2/MatMul/ReadVariableOp2Z
+sequential_3/dense_6/BiasAdd/ReadVariableOp+sequential_3/dense_6/BiasAdd/ReadVariableOp2V
)sequential_2/dense/BiasAdd/ReadVariableOp)sequential_2/dense/BiasAdd/ReadVariableOp2Z
+sequential_2/dense_5/BiasAdd/ReadVariableOp+sequential_2/dense_5/BiasAdd/ReadVariableOp2X
*sequential_2/dense_3/MatMul/ReadVariableOp*sequential_2/dense_3/MatMul/ReadVariableOp2X
*sequential_3/dense_6/MatMul/ReadVariableOp*sequential_3/dense_6/MatMul/ReadVariableOp2Z
+sequential_2/dense_3/BiasAdd/ReadVariableOp+sequential_2/dense_3/BiasAdd/ReadVariableOp2X
*sequential_2/dense_4/MatMul/ReadVariableOp*sequential_2/dense_4/MatMul/ReadVariableOp2Z
+sequential_2/dense_1/BiasAdd/ReadVariableOp+sequential_2/dense_1/BiasAdd/ReadVariableOp:& "
 
_user_specified_nameinputs: : : : : : : : :	 :
 : : : : 
?
a
E__inference_sequential_layer_call_and_return_conditional_losses_63094

inputs
identityO
IdentityIdentityinputs*
T0*(
_output_shapes
:??????????"
identityIdentity:output:0*'
_input_shapes
:??????????:& "
 
_user_specified_nameinputs
?R
?
G__inference_sequential_4_layer_call_and_return_conditional_losses_63856

inputs5
1sequential_2_dense_matmul_readvariableop_resource6
2sequential_2_dense_biasadd_readvariableop_resource7
3sequential_2_dense_1_matmul_readvariableop_resource8
4sequential_2_dense_1_biasadd_readvariableop_resource7
3sequential_2_dense_2_matmul_readvariableop_resource8
4sequential_2_dense_2_biasadd_readvariableop_resource7
3sequential_2_dense_3_matmul_readvariableop_resource8
4sequential_2_dense_3_biasadd_readvariableop_resource7
3sequential_2_dense_4_matmul_readvariableop_resource8
4sequential_2_dense_4_biasadd_readvariableop_resource7
3sequential_2_dense_5_matmul_readvariableop_resource8
4sequential_2_dense_5_biasadd_readvariableop_resource7
3sequential_3_dense_6_matmul_readvariableop_resource8
4sequential_3_dense_6_biasadd_readvariableop_resource
identity??)sequential_2/dense/BiasAdd/ReadVariableOp?(sequential_2/dense/MatMul/ReadVariableOp?+sequential_2/dense_1/BiasAdd/ReadVariableOp?*sequential_2/dense_1/MatMul/ReadVariableOp?+sequential_2/dense_2/BiasAdd/ReadVariableOp?*sequential_2/dense_2/MatMul/ReadVariableOp?+sequential_2/dense_3/BiasAdd/ReadVariableOp?*sequential_2/dense_3/MatMul/ReadVariableOp?+sequential_2/dense_4/BiasAdd/ReadVariableOp?*sequential_2/dense_4/MatMul/ReadVariableOp?+sequential_2/dense_5/BiasAdd/ReadVariableOp?*sequential_2/dense_5/MatMul/ReadVariableOp?+sequential_3/dense_6/BiasAdd/ReadVariableOp?*sequential_3/dense_6/MatMul/ReadVariableOps
"sequential_1/flatten/Reshape/shapeConst*
_output_shapes
:*
dtype0*
valueB"?????   ?
sequential_1/flatten/ReshapeReshapeinputs+sequential_1/flatten/Reshape/shape:output:0*(
_output_shapes
:??????????*
T0?
(sequential_2/dense/MatMul/ReadVariableOpReadVariableOp1sequential_2_dense_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0* 
_output_shapes
:
??*
dtype0?
sequential_2/dense/MatMulMatMul%sequential_1/flatten/Reshape:output:00sequential_2/dense/MatMul/ReadVariableOp:value:0*
T0*(
_output_shapes
:???????????
)sequential_2/dense/BiasAdd/ReadVariableOpReadVariableOp2sequential_2_dense_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes	
:?*
dtype0?
sequential_2/dense/BiasAddBiasAdd#sequential_2/dense/MatMul:product:01sequential_2/dense/BiasAdd/ReadVariableOp:value:0*(
_output_shapes
:??????????*
T0w
sequential_2/dense/ReluRelu#sequential_2/dense/BiasAdd:output:0*
T0*(
_output_shapes
:???????????
*sequential_2/dense_1/MatMul/ReadVariableOpReadVariableOp3sequential_2_dense_1_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0* 
_output_shapes
:
???
sequential_2/dense_1/MatMulMatMul%sequential_2/dense/Relu:activations:02sequential_2/dense_1/MatMul/ReadVariableOp:value:0*
T0*(
_output_shapes
:???????????
+sequential_2/dense_1/BiasAdd/ReadVariableOpReadVariableOp4sequential_2_dense_1_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes	
:??
sequential_2/dense_1/BiasAddBiasAdd%sequential_2/dense_1/MatMul:product:03sequential_2/dense_1/BiasAdd/ReadVariableOp:value:0*(
_output_shapes
:??????????*
T0{
sequential_2/dense_1/ReluRelu%sequential_2/dense_1/BiasAdd:output:0*(
_output_shapes
:??????????*
T0?
*sequential_2/dense_2/MatMul/ReadVariableOpReadVariableOp3sequential_2_dense_2_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:	?8*
dtype0?
sequential_2/dense_2/MatMulMatMul'sequential_2/dense_1/Relu:activations:02sequential_2/dense_2/MatMul/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0?
+sequential_2/dense_2/BiasAdd/ReadVariableOpReadVariableOp4sequential_2_dense_2_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:8?
sequential_2/dense_2/BiasAddBiasAdd%sequential_2/dense_2/MatMul:product:03sequential_2/dense_2/BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8z
sequential_2/dense_2/ReluRelu%sequential_2/dense_2/BiasAdd:output:0*'
_output_shapes
:?????????8*
T0?
*sequential_2/dense_3/MatMul/ReadVariableOpReadVariableOp3sequential_2_dense_3_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes

:88*
dtype0?
sequential_2/dense_3/MatMulMatMul'sequential_2/dense_2/Relu:activations:02sequential_2/dense_3/MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8?
+sequential_2/dense_3/BiasAdd/ReadVariableOpReadVariableOp4sequential_2_dense_3_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:8?
sequential_2/dense_3/BiasAddBiasAdd%sequential_2/dense_3/MatMul:product:03sequential_2/dense_3/BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8z
sequential_2/dense_3/ReluRelu%sequential_2/dense_3/BiasAdd:output:0*
T0*'
_output_shapes
:?????????8?
*sequential_2/dense_4/MatMul/ReadVariableOpReadVariableOp3sequential_2_dense_4_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes

:88*
dtype0?
sequential_2/dense_4/MatMulMatMul'sequential_2/dense_3/Relu:activations:02sequential_2/dense_4/MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8?
+sequential_2/dense_4/BiasAdd/ReadVariableOpReadVariableOp4sequential_2_dense_4_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:8*
dtype0?
sequential_2/dense_4/BiasAddBiasAdd%sequential_2/dense_4/MatMul:product:03sequential_2/dense_4/BiasAdd/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0z
sequential_2/dense_4/ReluRelu%sequential_2/dense_4/BiasAdd:output:0*'
_output_shapes
:?????????8*
T0?
*sequential_2/dense_5/MatMul/ReadVariableOpReadVariableOp3sequential_2_dense_5_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes

:88?
sequential_2/dense_5/MatMulMatMul'sequential_2/dense_4/Relu:activations:02sequential_2/dense_5/MatMul/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0?
+sequential_2/dense_5/BiasAdd/ReadVariableOpReadVariableOp4sequential_2_dense_5_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:8?
sequential_2/dense_5/BiasAddBiasAdd%sequential_2/dense_5/MatMul:product:03sequential_2/dense_5/BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8z
sequential_2/dense_5/ReluRelu%sequential_2/dense_5/BiasAdd:output:0*'
_output_shapes
:?????????8*
T0?
sequential_2/dropout/IdentityIdentity'sequential_2/dense_5/Relu:activations:0*'
_output_shapes
:?????????8*
T0?
*sequential_3/dense_6/MatMul/ReadVariableOpReadVariableOp3sequential_3_dense_6_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes

:8?
sequential_3/dense_6/MatMulMatMul&sequential_2/dropout/Identity:output:02sequential_3/dense_6/MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:??????????
+sequential_3/dense_6/BiasAdd/ReadVariableOpReadVariableOp4sequential_3_dense_6_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:?
sequential_3/dense_6/BiasAddBiasAdd%sequential_3/dense_6/MatMul:product:03sequential_3/dense_6/BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:??????????
sequential_3/dense_6/SoftmaxSoftmax%sequential_3/dense_6/BiasAdd:output:0*'
_output_shapes
:?????????*
T0?
IdentityIdentity&sequential_3/dense_6/Softmax:softmax:0*^sequential_2/dense/BiasAdd/ReadVariableOp)^sequential_2/dense/MatMul/ReadVariableOp,^sequential_2/dense_1/BiasAdd/ReadVariableOp+^sequential_2/dense_1/MatMul/ReadVariableOp,^sequential_2/dense_2/BiasAdd/ReadVariableOp+^sequential_2/dense_2/MatMul/ReadVariableOp,^sequential_2/dense_3/BiasAdd/ReadVariableOp+^sequential_2/dense_3/MatMul/ReadVariableOp,^sequential_2/dense_4/BiasAdd/ReadVariableOp+^sequential_2/dense_4/MatMul/ReadVariableOp,^sequential_2/dense_5/BiasAdd/ReadVariableOp+^sequential_2/dense_5/MatMul/ReadVariableOp,^sequential_3/dense_6/BiasAdd/ReadVariableOp+^sequential_3/dense_6/MatMul/ReadVariableOp*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*_
_input_shapesN
L:??????????::::::::::::::2Z
+sequential_2/dense_5/BiasAdd/ReadVariableOp+sequential_2/dense_5/BiasAdd/ReadVariableOp2V
)sequential_2/dense/BiasAdd/ReadVariableOp)sequential_2/dense/BiasAdd/ReadVariableOp2X
*sequential_2/dense_3/MatMul/ReadVariableOp*sequential_2/dense_3/MatMul/ReadVariableOp2X
*sequential_3/dense_6/MatMul/ReadVariableOp*sequential_3/dense_6/MatMul/ReadVariableOp2Z
+sequential_2/dense_3/BiasAdd/ReadVariableOp+sequential_2/dense_3/BiasAdd/ReadVariableOp2X
*sequential_2/dense_4/MatMul/ReadVariableOp*sequential_2/dense_4/MatMul/ReadVariableOp2Z
+sequential_2/dense_1/BiasAdd/ReadVariableOp+sequential_2/dense_1/BiasAdd/ReadVariableOp2X
*sequential_2/dense_1/MatMul/ReadVariableOp*sequential_2/dense_1/MatMul/ReadVariableOp2Z
+sequential_2/dense_4/BiasAdd/ReadVariableOp+sequential_2/dense_4/BiasAdd/ReadVariableOp2X
*sequential_2/dense_5/MatMul/ReadVariableOp*sequential_2/dense_5/MatMul/ReadVariableOp2T
(sequential_2/dense/MatMul/ReadVariableOp(sequential_2/dense/MatMul/ReadVariableOp2X
*sequential_2/dense_2/MatMul/ReadVariableOp*sequential_2/dense_2/MatMul/ReadVariableOp2Z
+sequential_2/dense_2/BiasAdd/ReadVariableOp+sequential_2/dense_2/BiasAdd/ReadVariableOp2Z
+sequential_3/dense_6/BiasAdd/ReadVariableOp+sequential_3/dense_6/BiasAdd/ReadVariableOp:& "
 
_user_specified_nameinputs: : : : : : : : :	 :
 : : : : 
?
?
,__inference_sequential_3_layer_call_fn_63483
input_1"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinput_1statefulpartitionedcall_args_1statefulpartitionedcall_args_2*
Tout
2*
Tin
2*,
_gradient_op_typePartitionedCall-63478*'
_output_shapes
:?????????**
config_proto

GPU 

CPU2J 8*P
fKRI
G__inference_sequential_3_layer_call_and_return_conditional_losses_63477?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????8::22
StatefulPartitionedCallStatefulPartitionedCall:' #
!
_user_specified_name	input_1: : 
?
j
G__inference_sequential_1_layer_call_and_return_conditional_losses_63032
flatten_input
identity?
flatten/PartitionedCallPartitionedCallflatten_input**
config_proto

GPU 

CPU2J 8*
Tin
2*(
_output_shapes
:??????????*
Tout
2*,
_gradient_op_typePartitionedCall-63024*K
fFRD
B__inference_flatten_layer_call_and_return_conditional_losses_63018i
IdentityIdentity flatten/PartitionedCall:output:0*(
_output_shapes
:??????????*
T0"
identityIdentity:output:0*'
_input_shapes
:??????????:- )
'
_user_specified_nameflatten_input
?
a
E__inference_sequential_layer_call_and_return_conditional_losses_63924

inputs
identityO
IdentityIdentityinputs*(
_output_shapes
:??????????*
T0"
identityIdentity:output:0*'
_input_shapes
:??????????:& "
 
_user_specified_nameinputs
?
?
'__inference_dense_6_layer_call_fn_64285

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*'
_output_shapes
:?????????*
Tout
2*,
_gradient_op_typePartitionedCall-63446**
config_proto

GPU 

CPU2J 8*
Tin
2*K
fFRD
B__inference_dense_6_layer_call_and_return_conditional_losses_63440?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*.
_input_shapes
:?????????8::22
StatefulPartitionedCallStatefulPartitionedCall: : :& "
 
_user_specified_nameinputs
?
?
G__inference_sequential_3_layer_call_and_return_conditional_losses_63477

inputs*
&dense_6_statefulpartitionedcall_args_1*
&dense_6_statefulpartitionedcall_args_2
identity??dense_6/StatefulPartitionedCall?
dense_6/StatefulPartitionedCallStatefulPartitionedCallinputs&dense_6_statefulpartitionedcall_args_1&dense_6_statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:?????????*
Tout
2*K
fFRD
B__inference_dense_6_layer_call_and_return_conditional_losses_63440*
Tin
2*,
_gradient_op_typePartitionedCall-63446?
IdentityIdentity(dense_6/StatefulPartitionedCall:output:0 ^dense_6/StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*.
_input_shapes
:?????????8::2B
dense_6/StatefulPartitionedCalldense_6/StatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
c
G__inference_sequential_1_layer_call_and_return_conditional_losses_63047

inputs
identity?
flatten/PartitionedCallPartitionedCallinputs*K
fFRD
B__inference_flatten_layer_call_and_return_conditional_losses_63018**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-63024*
Tout
2*
Tin
2*(
_output_shapes
:??????????i
IdentityIdentity flatten/PartitionedCall:output:0*
T0*(
_output_shapes
:??????????"
identityIdentity:output:0*'
_input_shapes
:??????????:& "
 
_user_specified_nameinputs
?	
?
B__inference_dense_6_layer_call_and_return_conditional_losses_63440

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes

:8i
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:??????????
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
IdentityIdentitySoftmax:softmax:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*.
_input_shapes
:?????????8::2.
MatMul/ReadVariableOpMatMul/ReadVariableOp20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp: :& "
 
_user_specified_nameinputs: 
?
G
*__inference_sequential_layer_call_fn_63086
input_1
identity?
PartitionedCallPartitionedCallinput_1*,
_gradient_op_typePartitionedCall-63083*(
_output_shapes
:??????????*N
fIRG
E__inference_sequential_layer_call_and_return_conditional_losses_63082*
Tin
2*
Tout
2**
config_proto

GPU 

CPU2J 8a
IdentityIdentityPartitionedCall:output:0*
T0*(
_output_shapes
:??????????"
identityIdentity:output:0*'
_input_shapes
:??????????:' #
!
_user_specified_name	input_1
?
j
G__inference_sequential_1_layer_call_and_return_conditional_losses_63039
flatten_input
identity?
flatten/PartitionedCallPartitionedCallflatten_input*(
_output_shapes
:??????????*,
_gradient_op_typePartitionedCall-63024*K
fFRD
B__inference_flatten_layer_call_and_return_conditional_losses_63018**
config_proto

GPU 

CPU2J 8*
Tin
2*
Tout
2i
IdentityIdentity flatten/PartitionedCall:output:0*(
_output_shapes
:??????????*
T0"
identityIdentity:output:0*'
_input_shapes
:??????????:- )
'
_user_specified_nameflatten_input
?
?
G__inference_sequential_4_layer_call_and_return_conditional_losses_63640

inputs/
+sequential_2_statefulpartitionedcall_args_1/
+sequential_2_statefulpartitionedcall_args_2/
+sequential_2_statefulpartitionedcall_args_3/
+sequential_2_statefulpartitionedcall_args_4/
+sequential_2_statefulpartitionedcall_args_5/
+sequential_2_statefulpartitionedcall_args_6/
+sequential_2_statefulpartitionedcall_args_7/
+sequential_2_statefulpartitionedcall_args_8/
+sequential_2_statefulpartitionedcall_args_90
,sequential_2_statefulpartitionedcall_args_100
,sequential_2_statefulpartitionedcall_args_110
,sequential_2_statefulpartitionedcall_args_12/
+sequential_3_statefulpartitionedcall_args_1/
+sequential_3_statefulpartitionedcall_args_2
identity??$sequential_2/StatefulPartitionedCall?$sequential_3/StatefulPartitionedCall?
sequential_1/PartitionedCallPartitionedCallinputs*(
_output_shapes
:??????????*
Tout
2*P
fKRI
G__inference_sequential_1_layer_call_and_return_conditional_losses_63047*,
_gradient_op_typePartitionedCall-63048*
Tin
2**
config_proto

GPU 

CPU2J 8?
sequential/PartitionedCallPartitionedCall%sequential_1/PartitionedCall:output:0*,
_gradient_op_typePartitionedCall-63083*
Tin
2*(
_output_shapes
:??????????**
config_proto

GPU 

CPU2J 8*N
fIRG
E__inference_sequential_layer_call_and_return_conditional_losses_63082*
Tout
2?
$sequential_2/StatefulPartitionedCallStatefulPartitionedCall#sequential/PartitionedCall:output:0+sequential_2_statefulpartitionedcall_args_1+sequential_2_statefulpartitionedcall_args_2+sequential_2_statefulpartitionedcall_args_3+sequential_2_statefulpartitionedcall_args_4+sequential_2_statefulpartitionedcall_args_5+sequential_2_statefulpartitionedcall_args_6+sequential_2_statefulpartitionedcall_args_7+sequential_2_statefulpartitionedcall_args_8+sequential_2_statefulpartitionedcall_args_9,sequential_2_statefulpartitionedcall_args_10,sequential_2_statefulpartitionedcall_args_11,sequential_2_statefulpartitionedcall_args_12*P
fKRI
G__inference_sequential_2_layer_call_and_return_conditional_losses_63366*,
_gradient_op_typePartitionedCall-63367*
Tout
2*
Tin
2*'
_output_shapes
:?????????8**
config_proto

GPU 

CPU2J 8?
$sequential_3/StatefulPartitionedCallStatefulPartitionedCall-sequential_2/StatefulPartitionedCall:output:0+sequential_3_statefulpartitionedcall_args_1+sequential_3_statefulpartitionedcall_args_2*,
_gradient_op_typePartitionedCall-63478*'
_output_shapes
:?????????**
config_proto

GPU 

CPU2J 8*
Tout
2*P
fKRI
G__inference_sequential_3_layer_call_and_return_conditional_losses_63477*
Tin
2?
IdentityIdentity-sequential_3/StatefulPartitionedCall:output:0%^sequential_2/StatefulPartitionedCall%^sequential_3/StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*_
_input_shapesN
L:??????????::::::::::::::2L
$sequential_2/StatefulPartitionedCall$sequential_2/StatefulPartitionedCall2L
$sequential_3/StatefulPartitionedCall$sequential_3/StatefulPartitionedCall: : :	 :
 : : : : :& "
 
_user_specified_nameinputs: : : : : : 
?	
?
B__inference_dense_4_layer_call_and_return_conditional_losses_63225

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes

:88*
dtype0i
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0?
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:8v
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8P
ReluReluBiasAdd:output:0*'
_output_shapes
:?????????8*
T0?
IdentityIdentityRelu:activations:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*
T0*'
_output_shapes
:?????????8"
identityIdentity:output:0*.
_input_shapes
:?????????8::20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp:& "
 
_user_specified_nameinputs: : 
?F
?
G__inference_sequential_2_layer_call_and_return_conditional_losses_63996

inputs(
$dense_matmul_readvariableop_resource)
%dense_biasadd_readvariableop_resource*
&dense_1_matmul_readvariableop_resource+
'dense_1_biasadd_readvariableop_resource*
&dense_2_matmul_readvariableop_resource+
'dense_2_biasadd_readvariableop_resource*
&dense_3_matmul_readvariableop_resource+
'dense_3_biasadd_readvariableop_resource*
&dense_4_matmul_readvariableop_resource+
'dense_4_biasadd_readvariableop_resource*
&dense_5_matmul_readvariableop_resource+
'dense_5_biasadd_readvariableop_resource
identity??dense/BiasAdd/ReadVariableOp?dense/MatMul/ReadVariableOp?dense_1/BiasAdd/ReadVariableOp?dense_1/MatMul/ReadVariableOp?dense_2/BiasAdd/ReadVariableOp?dense_2/MatMul/ReadVariableOp?dense_3/BiasAdd/ReadVariableOp?dense_3/MatMul/ReadVariableOp?dense_4/BiasAdd/ReadVariableOp?dense_4/MatMul/ReadVariableOp?dense_5/BiasAdd/ReadVariableOp?dense_5/MatMul/ReadVariableOp?
dense/MatMul/ReadVariableOpReadVariableOp$dense_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0* 
_output_shapes
:
??*
dtype0v
dense/MatMulMatMulinputs#dense/MatMul/ReadVariableOp:value:0*
T0*(
_output_shapes
:???????????
dense/BiasAdd/ReadVariableOpReadVariableOp%dense_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes	
:?*
dtype0?
dense/BiasAddBiasAdddense/MatMul:product:0$dense/BiasAdd/ReadVariableOp:value:0*(
_output_shapes
:??????????*
T0]

dense/ReluReludense/BiasAdd:output:0*(
_output_shapes
:??????????*
T0?
dense_1/MatMul/ReadVariableOpReadVariableOp&dense_1_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0* 
_output_shapes
:
??*
dtype0?
dense_1/MatMulMatMuldense/Relu:activations:0%dense_1/MatMul/ReadVariableOp:value:0*
T0*(
_output_shapes
:???????????
dense_1/BiasAdd/ReadVariableOpReadVariableOp'dense_1_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes	
:?*
dtype0?
dense_1/BiasAddBiasAdddense_1/MatMul:product:0&dense_1/BiasAdd/ReadVariableOp:value:0*
T0*(
_output_shapes
:??????????a
dense_1/ReluReludense_1/BiasAdd:output:0*(
_output_shapes
:??????????*
T0?
dense_2/MatMul/ReadVariableOpReadVariableOp&dense_2_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:	?8*
dtype0?
dense_2/MatMulMatMuldense_1/Relu:activations:0%dense_2/MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8?
dense_2/BiasAdd/ReadVariableOpReadVariableOp'dense_2_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:8?
dense_2/BiasAddBiasAdddense_2/MatMul:product:0&dense_2/BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8`
dense_2/ReluReludense_2/BiasAdd:output:0*
T0*'
_output_shapes
:?????????8?
dense_3/MatMul/ReadVariableOpReadVariableOp&dense_3_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes

:88*
dtype0?
dense_3/MatMulMatMuldense_2/Relu:activations:0%dense_3/MatMul/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0?
dense_3/BiasAdd/ReadVariableOpReadVariableOp'dense_3_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:8?
dense_3/BiasAddBiasAdddense_3/MatMul:product:0&dense_3/BiasAdd/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0`
dense_3/ReluReludense_3/BiasAdd:output:0*'
_output_shapes
:?????????8*
T0?
dense_4/MatMul/ReadVariableOpReadVariableOp&dense_4_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes

:88?
dense_4/MatMulMatMuldense_3/Relu:activations:0%dense_4/MatMul/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0?
dense_4/BiasAdd/ReadVariableOpReadVariableOp'dense_4_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:8?
dense_4/BiasAddBiasAdddense_4/MatMul:product:0&dense_4/BiasAdd/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0`
dense_4/ReluReludense_4/BiasAdd:output:0*'
_output_shapes
:?????????8*
T0?
dense_5/MatMul/ReadVariableOpReadVariableOp&dense_5_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes

:88?
dense_5/MatMulMatMuldense_4/Relu:activations:0%dense_5/MatMul/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0?
dense_5/BiasAdd/ReadVariableOpReadVariableOp'dense_5_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:8?
dense_5/BiasAddBiasAdddense_5/MatMul:product:0&dense_5/BiasAdd/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0`
dense_5/ReluReludense_5/BiasAdd:output:0*
T0*'
_output_shapes
:?????????8Y
dropout/dropout/rateConst*
dtype0*
_output_shapes
: *
valueB
 *??L>_
dropout/dropout/ShapeShapedense_5/Relu:activations:0*
_output_shapes
:*
T0g
"dropout/dropout/random_uniform/minConst*
_output_shapes
: *
valueB
 *    *
dtype0g
"dropout/dropout/random_uniform/maxConst*
dtype0*
_output_shapes
: *
valueB
 *  ???
,dropout/dropout/random_uniform/RandomUniformRandomUniformdropout/dropout/Shape:output:0*
dtype0*
T0*'
_output_shapes
:?????????8?
"dropout/dropout/random_uniform/subSub+dropout/dropout/random_uniform/max:output:0+dropout/dropout/random_uniform/min:output:0*
_output_shapes
: *
T0?
"dropout/dropout/random_uniform/mulMul5dropout/dropout/random_uniform/RandomUniform:output:0&dropout/dropout/random_uniform/sub:z:0*
T0*'
_output_shapes
:?????????8?
dropout/dropout/random_uniformAdd&dropout/dropout/random_uniform/mul:z:0+dropout/dropout/random_uniform/min:output:0*
T0*'
_output_shapes
:?????????8Z
dropout/dropout/sub/xConst*
_output_shapes
: *
valueB
 *  ??*
dtype0z
dropout/dropout/subSubdropout/dropout/sub/x:output:0dropout/dropout/rate:output:0*
_output_shapes
: *
T0^
dropout/dropout/truediv/xConst*
dtype0*
_output_shapes
: *
valueB
 *  ???
dropout/dropout/truedivRealDiv"dropout/dropout/truediv/x:output:0dropout/dropout/sub:z:0*
_output_shapes
: *
T0?
dropout/dropout/GreaterEqualGreaterEqual"dropout/dropout/random_uniform:z:0dropout/dropout/rate:output:0*
T0*'
_output_shapes
:?????????8?
dropout/dropout/mulMuldense_5/Relu:activations:0dropout/dropout/truediv:z:0*'
_output_shapes
:?????????8*
T0
dropout/dropout/CastCast dropout/dropout/GreaterEqual:z:0*

SrcT0
*

DstT0*'
_output_shapes
:?????????8?
dropout/dropout/mul_1Muldropout/dropout/mul:z:0dropout/dropout/Cast:y:0*
T0*'
_output_shapes
:?????????8?
IdentityIdentitydropout/dropout/mul_1:z:0^dense/BiasAdd/ReadVariableOp^dense/MatMul/ReadVariableOp^dense_1/BiasAdd/ReadVariableOp^dense_1/MatMul/ReadVariableOp^dense_2/BiasAdd/ReadVariableOp^dense_2/MatMul/ReadVariableOp^dense_3/BiasAdd/ReadVariableOp^dense_3/MatMul/ReadVariableOp^dense_4/BiasAdd/ReadVariableOp^dense_4/MatMul/ReadVariableOp^dense_5/BiasAdd/ReadVariableOp^dense_5/MatMul/ReadVariableOp*'
_output_shapes
:?????????8*
T0"
identityIdentity:output:0*W
_input_shapesF
D:??????????::::::::::::2@
dense_5/BiasAdd/ReadVariableOpdense_5/BiasAdd/ReadVariableOp2>
dense_5/MatMul/ReadVariableOpdense_5/MatMul/ReadVariableOp2>
dense_2/MatMul/ReadVariableOpdense_2/MatMul/ReadVariableOp2@
dense_3/BiasAdd/ReadVariableOpdense_3/BiasAdd/ReadVariableOp2<
dense/BiasAdd/ReadVariableOpdense/BiasAdd/ReadVariableOp2:
dense/MatMul/ReadVariableOpdense/MatMul/ReadVariableOp2@
dense_1/BiasAdd/ReadVariableOpdense_1/BiasAdd/ReadVariableOp2>
dense_3/MatMul/ReadVariableOpdense_3/MatMul/ReadVariableOp2@
dense_4/BiasAdd/ReadVariableOpdense_4/BiasAdd/ReadVariableOp2>
dense_4/MatMul/ReadVariableOpdense_4/MatMul/ReadVariableOp2@
dense_2/BiasAdd/ReadVariableOpdense_2/BiasAdd/ReadVariableOp2>
dense_1/MatMul/ReadVariableOpdense_1/MatMul/ReadVariableOp: :	 :
 : : :& "
 
_user_specified_nameinputs: : : : : : : 
?
c
G__inference_sequential_1_layer_call_and_return_conditional_losses_63900

inputs
identityf
flatten/Reshape/shapeConst*
valueB"?????   *
_output_shapes
:*
dtype0u
flatten/ReshapeReshapeinputsflatten/Reshape/shape:output:0*
T0*(
_output_shapes
:??????????a
IdentityIdentityflatten/Reshape:output:0*(
_output_shapes
:??????????*
T0"
identityIdentity:output:0*'
_input_shapes
:??????????:& "
 
_user_specified_nameinputs
?
?
'__inference_dense_1_layer_call_fn_64160

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*
Tin
2**
config_proto

GPU 

CPU2J 8*(
_output_shapes
:??????????*,
_gradient_op_typePartitionedCall-63147*
Tout
2*K
fFRD
B__inference_dense_1_layer_call_and_return_conditional_losses_63141?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*(
_output_shapes
:??????????*
T0"
identityIdentity:output:0*/
_input_shapes
:??????????::22
StatefulPartitionedCallStatefulPartitionedCall: : :& "
 
_user_specified_nameinputs
?
F
*__inference_sequential_layer_call_fn_63934

inputs
identity?
PartitionedCallPartitionedCallinputs*,
_gradient_op_typePartitionedCall-63095*
Tout
2*
Tin
2**
config_proto

GPU 

CPU2J 8*(
_output_shapes
:??????????*N
fIRG
E__inference_sequential_layer_call_and_return_conditional_losses_63094a
IdentityIdentityPartitionedCall:output:0*
T0*(
_output_shapes
:??????????"
identityIdentity:output:0*'
_input_shapes
:??????????:& "
 
_user_specified_nameinputs
?#
?
G__inference_sequential_2_layer_call_and_return_conditional_losses_63340
input_1(
$dense_statefulpartitionedcall_args_1(
$dense_statefulpartitionedcall_args_2*
&dense_1_statefulpartitionedcall_args_1*
&dense_1_statefulpartitionedcall_args_2*
&dense_2_statefulpartitionedcall_args_1*
&dense_2_statefulpartitionedcall_args_2*
&dense_3_statefulpartitionedcall_args_1*
&dense_3_statefulpartitionedcall_args_2*
&dense_4_statefulpartitionedcall_args_1*
&dense_4_statefulpartitionedcall_args_2*
&dense_5_statefulpartitionedcall_args_1*
&dense_5_statefulpartitionedcall_args_2
identity??dense/StatefulPartitionedCall?dense_1/StatefulPartitionedCall?dense_2/StatefulPartitionedCall?dense_3/StatefulPartitionedCall?dense_4/StatefulPartitionedCall?dense_5/StatefulPartitionedCall?
dense/StatefulPartitionedCallStatefulPartitionedCallinput_1$dense_statefulpartitionedcall_args_1$dense_statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*
Tin
2*I
fDRB
@__inference_dense_layer_call_and_return_conditional_losses_63113*(
_output_shapes
:??????????*,
_gradient_op_typePartitionedCall-63119*
Tout
2?
dense_1/StatefulPartitionedCallStatefulPartitionedCall&dense/StatefulPartitionedCall:output:0&dense_1_statefulpartitionedcall_args_1&dense_1_statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*
Tin
2*
Tout
2*,
_gradient_op_typePartitionedCall-63147*K
fFRD
B__inference_dense_1_layer_call_and_return_conditional_losses_63141*(
_output_shapes
:???????????
dense_2/StatefulPartitionedCallStatefulPartitionedCall(dense_1/StatefulPartitionedCall:output:0&dense_2_statefulpartitionedcall_args_1&dense_2_statefulpartitionedcall_args_2*,
_gradient_op_typePartitionedCall-63175*'
_output_shapes
:?????????8**
config_proto

GPU 

CPU2J 8*
Tout
2*K
fFRD
B__inference_dense_2_layer_call_and_return_conditional_losses_63169*
Tin
2?
dense_3/StatefulPartitionedCallStatefulPartitionedCall(dense_2/StatefulPartitionedCall:output:0&dense_3_statefulpartitionedcall_args_1&dense_3_statefulpartitionedcall_args_2*'
_output_shapes
:?????????8*,
_gradient_op_typePartitionedCall-63203*
Tout
2*
Tin
2*K
fFRD
B__inference_dense_3_layer_call_and_return_conditional_losses_63197**
config_proto

GPU 

CPU2J 8?
dense_4/StatefulPartitionedCallStatefulPartitionedCall(dense_3/StatefulPartitionedCall:output:0&dense_4_statefulpartitionedcall_args_1&dense_4_statefulpartitionedcall_args_2*K
fFRD
B__inference_dense_4_layer_call_and_return_conditional_losses_63225**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-63231*
Tin
2*'
_output_shapes
:?????????8*
Tout
2?
dense_5/StatefulPartitionedCallStatefulPartitionedCall(dense_4/StatefulPartitionedCall:output:0&dense_5_statefulpartitionedcall_args_1&dense_5_statefulpartitionedcall_args_2*
Tout
2*'
_output_shapes
:?????????8**
config_proto

GPU 

CPU2J 8*
Tin
2*K
fFRD
B__inference_dense_5_layer_call_and_return_conditional_losses_63253*,
_gradient_op_typePartitionedCall-63259?
dropout/PartitionedCallPartitionedCall(dense_5/StatefulPartitionedCall:output:0*,
_gradient_op_typePartitionedCall-63309*
Tout
2*'
_output_shapes
:?????????8*K
fFRD
B__inference_dropout_layer_call_and_return_conditional_losses_63297**
config_proto

GPU 

CPU2J 8*
Tin
2?
IdentityIdentity dropout/PartitionedCall:output:0^dense/StatefulPartitionedCall ^dense_1/StatefulPartitionedCall ^dense_2/StatefulPartitionedCall ^dense_3/StatefulPartitionedCall ^dense_4/StatefulPartitionedCall ^dense_5/StatefulPartitionedCall*'
_output_shapes
:?????????8*
T0"
identityIdentity:output:0*W
_input_shapesF
D:??????????::::::::::::2>
dense/StatefulPartitionedCalldense/StatefulPartitionedCall2B
dense_1/StatefulPartitionedCalldense_1/StatefulPartitionedCall2B
dense_2/StatefulPartitionedCalldense_2/StatefulPartitionedCall2B
dense_3/StatefulPartitionedCalldense_3/StatefulPartitionedCall2B
dense_4/StatefulPartitionedCalldense_4/StatefulPartitionedCall2B
dense_5/StatefulPartitionedCalldense_5/StatefulPartitionedCall:' #
!
_user_specified_name	input_1: : : : : : : : :	 :
 : : 
?
C
'__inference_dropout_layer_call_fn_64267

inputs
identity?
PartitionedCallPartitionedCallinputs*'
_output_shapes
:?????????8*K
fFRD
B__inference_dropout_layer_call_and_return_conditional_losses_63297*
Tout
2*,
_gradient_op_typePartitionedCall-63309*
Tin
2**
config_proto

GPU 

CPU2J 8`
IdentityIdentityPartitionedCall:output:0*
T0*'
_output_shapes
:?????????8"
identityIdentity:output:0*&
_input_shapes
:?????????8:& "
 
_user_specified_nameinputs
?
?
,__inference_sequential_4_layer_call_fn_63702
sequential_1_input"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2"
statefulpartitionedcall_args_3"
statefulpartitionedcall_args_4"
statefulpartitionedcall_args_5"
statefulpartitionedcall_args_6"
statefulpartitionedcall_args_7"
statefulpartitionedcall_args_8"
statefulpartitionedcall_args_9#
statefulpartitionedcall_args_10#
statefulpartitionedcall_args_11#
statefulpartitionedcall_args_12#
statefulpartitionedcall_args_13#
statefulpartitionedcall_args_14
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallsequential_1_inputstatefulpartitionedcall_args_1statefulpartitionedcall_args_2statefulpartitionedcall_args_3statefulpartitionedcall_args_4statefulpartitionedcall_args_5statefulpartitionedcall_args_6statefulpartitionedcall_args_7statefulpartitionedcall_args_8statefulpartitionedcall_args_9statefulpartitionedcall_args_10statefulpartitionedcall_args_11statefulpartitionedcall_args_12statefulpartitionedcall_args_13statefulpartitionedcall_args_14*
Tout
2*,
_gradient_op_typePartitionedCall-63685*'
_output_shapes
:?????????**
config_proto

GPU 

CPU2J 8*P
fKRI
G__inference_sequential_4_layer_call_and_return_conditional_losses_63684*
Tin
2?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*_
_input_shapesN
L:??????????::::::::::::::22
StatefulPartitionedCallStatefulPartitionedCall:2 .
,
_user_specified_namesequential_1_input: : : : : : : : :	 :
 : : : : 
?
?
,__inference_sequential_2_layer_call_fn_63382
input_1"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2"
statefulpartitionedcall_args_3"
statefulpartitionedcall_args_4"
statefulpartitionedcall_args_5"
statefulpartitionedcall_args_6"
statefulpartitionedcall_args_7"
statefulpartitionedcall_args_8"
statefulpartitionedcall_args_9#
statefulpartitionedcall_args_10#
statefulpartitionedcall_args_11#
statefulpartitionedcall_args_12
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinput_1statefulpartitionedcall_args_1statefulpartitionedcall_args_2statefulpartitionedcall_args_3statefulpartitionedcall_args_4statefulpartitionedcall_args_5statefulpartitionedcall_args_6statefulpartitionedcall_args_7statefulpartitionedcall_args_8statefulpartitionedcall_args_9statefulpartitionedcall_args_10statefulpartitionedcall_args_11statefulpartitionedcall_args_12*P
fKRI
G__inference_sequential_2_layer_call_and_return_conditional_losses_63366*
Tout
2*,
_gradient_op_typePartitionedCall-63367*
Tin
2*'
_output_shapes
:?????????8**
config_proto

GPU 

CPU2J 8?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????8"
identityIdentity:output:0*W
_input_shapesF
D:??????????::::::::::::22
StatefulPartitionedCallStatefulPartitionedCall:' #
!
_user_specified_name	input_1: : : : : : : : :	 :
 : : 
?	
?
B__inference_dense_4_layer_call_and_return_conditional_losses_64207

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes

:88i
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0?
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:8v
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0P
ReluReluBiasAdd:output:0*
T0*'
_output_shapes
:?????????8?
IdentityIdentityRelu:activations:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*
T0*'
_output_shapes
:?????????8"
identityIdentity:output:0*.
_input_shapes
:?????????8::20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp: :& "
 
_user_specified_nameinputs: 
?
?
,__inference_sequential_4_layer_call_fn_63875

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2"
statefulpartitionedcall_args_3"
statefulpartitionedcall_args_4"
statefulpartitionedcall_args_5"
statefulpartitionedcall_args_6"
statefulpartitionedcall_args_7"
statefulpartitionedcall_args_8"
statefulpartitionedcall_args_9#
statefulpartitionedcall_args_10#
statefulpartitionedcall_args_11#
statefulpartitionedcall_args_12#
statefulpartitionedcall_args_13#
statefulpartitionedcall_args_14
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2statefulpartitionedcall_args_3statefulpartitionedcall_args_4statefulpartitionedcall_args_5statefulpartitionedcall_args_6statefulpartitionedcall_args_7statefulpartitionedcall_args_8statefulpartitionedcall_args_9statefulpartitionedcall_args_10statefulpartitionedcall_args_11statefulpartitionedcall_args_12statefulpartitionedcall_args_13statefulpartitionedcall_args_14**
config_proto

GPU 

CPU2J 8*
Tout
2*P
fKRI
G__inference_sequential_4_layer_call_and_return_conditional_losses_63640*'
_output_shapes
:?????????*,
_gradient_op_typePartitionedCall-63641*
Tin
2?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*_
_input_shapesN
L:??????????::::::::::::::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : : : : : : : :	 :
 : : : : 
?
^
B__inference_flatten_layer_call_and_return_conditional_losses_64119

inputs
identity^
Reshape/shapeConst*
dtype0*
_output_shapes
:*
valueB"?????   e
ReshapeReshapeinputsReshape/shape:output:0*
T0*(
_output_shapes
:??????????Y
IdentityIdentityReshape:output:0*
T0*(
_output_shapes
:??????????"
identityIdentity:output:0*'
_input_shapes
:??????????:& "
 
_user_specified_nameinputs
?
?
'__inference_dense_3_layer_call_fn_64196

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*,
_gradient_op_typePartitionedCall-63203*
Tout
2*
Tin
2*'
_output_shapes
:?????????8*K
fFRD
B__inference_dense_3_layer_call_and_return_conditional_losses_63197**
config_proto

GPU 

CPU2J 8?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????8"
identityIdentity:output:0*.
_input_shapes
:?????????8::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
?
G__inference_sequential_4_layer_call_and_return_conditional_losses_63615
sequential_1_input/
+sequential_2_statefulpartitionedcall_args_1/
+sequential_2_statefulpartitionedcall_args_2/
+sequential_2_statefulpartitionedcall_args_3/
+sequential_2_statefulpartitionedcall_args_4/
+sequential_2_statefulpartitionedcall_args_5/
+sequential_2_statefulpartitionedcall_args_6/
+sequential_2_statefulpartitionedcall_args_7/
+sequential_2_statefulpartitionedcall_args_8/
+sequential_2_statefulpartitionedcall_args_90
,sequential_2_statefulpartitionedcall_args_100
,sequential_2_statefulpartitionedcall_args_110
,sequential_2_statefulpartitionedcall_args_12/
+sequential_3_statefulpartitionedcall_args_1/
+sequential_3_statefulpartitionedcall_args_2
identity??$sequential_2/StatefulPartitionedCall?$sequential_3/StatefulPartitionedCall?
sequential_1/PartitionedCallPartitionedCallsequential_1_input**
config_proto

GPU 

CPU2J 8*P
fKRI
G__inference_sequential_1_layer_call_and_return_conditional_losses_63060*(
_output_shapes
:??????????*
Tout
2*,
_gradient_op_typePartitionedCall-63061*
Tin
2?
sequential/PartitionedCallPartitionedCall%sequential_1/PartitionedCall:output:0*N
fIRG
E__inference_sequential_layer_call_and_return_conditional_losses_63094*
Tout
2*(
_output_shapes
:??????????*
Tin
2*,
_gradient_op_typePartitionedCall-63095**
config_proto

GPU 

CPU2J 8?
$sequential_2/StatefulPartitionedCallStatefulPartitionedCall#sequential/PartitionedCall:output:0+sequential_2_statefulpartitionedcall_args_1+sequential_2_statefulpartitionedcall_args_2+sequential_2_statefulpartitionedcall_args_3+sequential_2_statefulpartitionedcall_args_4+sequential_2_statefulpartitionedcall_args_5+sequential_2_statefulpartitionedcall_args_6+sequential_2_statefulpartitionedcall_args_7+sequential_2_statefulpartitionedcall_args_8+sequential_2_statefulpartitionedcall_args_9,sequential_2_statefulpartitionedcall_args_10,sequential_2_statefulpartitionedcall_args_11,sequential_2_statefulpartitionedcall_args_12*P
fKRI
G__inference_sequential_2_layer_call_and_return_conditional_losses_63409*
Tin
2*
Tout
2*,
_gradient_op_typePartitionedCall-63410**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:?????????8?
$sequential_3/StatefulPartitionedCallStatefulPartitionedCall-sequential_2/StatefulPartitionedCall:output:0+sequential_3_statefulpartitionedcall_args_1+sequential_3_statefulpartitionedcall_args_2*
Tin
2*,
_gradient_op_typePartitionedCall-63495*P
fKRI
G__inference_sequential_3_layer_call_and_return_conditional_losses_63494**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:?????????*
Tout
2?
IdentityIdentity-sequential_3/StatefulPartitionedCall:output:0%^sequential_2/StatefulPartitionedCall%^sequential_3/StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*_
_input_shapesN
L:??????????::::::::::::::2L
$sequential_2/StatefulPartitionedCall$sequential_2/StatefulPartitionedCall2L
$sequential_3/StatefulPartitionedCall$sequential_3/StatefulPartitionedCall: : :2 .
,
_user_specified_namesequential_1_input: : : : : : : : :	 :
 : : 
?
?
,__inference_sequential_3_layer_call_fn_63500
input_1"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinput_1statefulpartitionedcall_args_1statefulpartitionedcall_args_2*
Tin
2**
config_proto

GPU 

CPU2J 8*'
_output_shapes
:?????????*P
fKRI
G__inference_sequential_3_layer_call_and_return_conditional_losses_63494*,
_gradient_op_typePartitionedCall-63495*
Tout
2?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*.
_input_shapes
:?????????8::22
StatefulPartitionedCallStatefulPartitionedCall: :' #
!
_user_specified_name	input_1: 
?	
?
B__inference_dense_5_layer_call_and_return_conditional_losses_64225

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes

:88i
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0?
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:8*
dtype0v
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8P
ReluReluBiasAdd:output:0*
T0*'
_output_shapes
:?????????8?
IdentityIdentityRelu:activations:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*
T0*'
_output_shapes
:?????????8"
identityIdentity:output:0*.
_input_shapes
:?????????8::2.
MatMul/ReadVariableOpMatMul/ReadVariableOp20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp: : :& "
 
_user_specified_nameinputs
?	
?
B__inference_dense_2_layer_call_and_return_conditional_losses_63169

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:	?8i
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8?
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:8v
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8P
ReluReluBiasAdd:output:0*
T0*'
_output_shapes
:?????????8?
IdentityIdentityRelu:activations:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*'
_output_shapes
:?????????8*
T0"
identityIdentity:output:0*/
_input_shapes
:??????????::20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp: : :& "
 
_user_specified_nameinputs
?
H
,__inference_sequential_1_layer_call_fn_63916

inputs
identity?
PartitionedCallPartitionedCallinputs*(
_output_shapes
:??????????**
config_proto

GPU 

CPU2J 8*
Tout
2*
Tin
2*P
fKRI
G__inference_sequential_1_layer_call_and_return_conditional_losses_63060*,
_gradient_op_typePartitionedCall-63061a
IdentityIdentityPartitionedCall:output:0*
T0*(
_output_shapes
:??????????"
identityIdentity:output:0*'
_input_shapes
:??????????:& "
 
_user_specified_nameinputs
?
a
B__inference_dropout_layer_call_and_return_conditional_losses_64252

inputs
identity?Q
dropout/rateConst*
valueB
 *??L>*
_output_shapes
: *
dtype0C
dropout/ShapeShapeinputs*
T0*
_output_shapes
:_
dropout/random_uniform/minConst*
dtype0*
valueB
 *    *
_output_shapes
: _
dropout/random_uniform/maxConst*
valueB
 *  ??*
_output_shapes
: *
dtype0?
$dropout/random_uniform/RandomUniformRandomUniformdropout/Shape:output:0*
T0*
dtype0*'
_output_shapes
:?????????8?
dropout/random_uniform/subSub#dropout/random_uniform/max:output:0#dropout/random_uniform/min:output:0*
T0*
_output_shapes
: ?
dropout/random_uniform/mulMul-dropout/random_uniform/RandomUniform:output:0dropout/random_uniform/sub:z:0*'
_output_shapes
:?????????8*
T0?
dropout/random_uniformAdddropout/random_uniform/mul:z:0#dropout/random_uniform/min:output:0*'
_output_shapes
:?????????8*
T0R
dropout/sub/xConst*
_output_shapes
: *
dtype0*
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
dropout/truedivRealDivdropout/truediv/x:output:0dropout/sub:z:0*
_output_shapes
: *
T0?
dropout/GreaterEqualGreaterEqualdropout/random_uniform:z:0dropout/rate:output:0*
T0*'
_output_shapes
:?????????8a
dropout/mulMulinputsdropout/truediv:z:0*'
_output_shapes
:?????????8*
T0o
dropout/CastCastdropout/GreaterEqual:z:0*

DstT0*'
_output_shapes
:?????????8*

SrcT0
i
dropout/mul_1Muldropout/mul:z:0dropout/Cast:y:0*'
_output_shapes
:?????????8*
T0Y
IdentityIdentitydropout/mul_1:z:0*'
_output_shapes
:?????????8*
T0"
identityIdentity:output:0*&
_input_shapes
:?????????8:& "
 
_user_specified_nameinputs
?
`
B__inference_dropout_layer_call_and_return_conditional_losses_63297

inputs

identity_1N
IdentityIdentityinputs*
T0*'
_output_shapes
:?????????8[

Identity_1IdentityIdentity:output:0*'
_output_shapes
:?????????8*
T0"!

identity_1Identity_1:output:0*&
_input_shapes
:?????????8:& "
 
_user_specified_nameinputs
?	
?
B__inference_dense_5_layer_call_and_return_conditional_losses_63253

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes

:88*
dtype0i
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8?
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:8*
dtype0v
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0P
ReluReluBiasAdd:output:0*
T0*'
_output_shapes
:?????????8?
IdentityIdentityRelu:activations:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*
T0*'
_output_shapes
:?????????8"
identityIdentity:output:0*.
_input_shapes
:?????????8::2.
MatMul/ReadVariableOpMatMul/ReadVariableOp20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp:& "
 
_user_specified_nameinputs: : 
?
?
G__inference_sequential_3_layer_call_and_return_conditional_losses_63467
input_1*
&dense_6_statefulpartitionedcall_args_1*
&dense_6_statefulpartitionedcall_args_2
identity??dense_6/StatefulPartitionedCall?
dense_6/StatefulPartitionedCallStatefulPartitionedCallinput_1&dense_6_statefulpartitionedcall_args_1&dense_6_statefulpartitionedcall_args_2*K
fFRD
B__inference_dense_6_layer_call_and_return_conditional_losses_63440**
config_proto

GPU 

CPU2J 8*
Tin
2*,
_gradient_op_typePartitionedCall-63446*'
_output_shapes
:?????????*
Tout
2?
IdentityIdentity(dense_6/StatefulPartitionedCall:output:0 ^dense_6/StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*.
_input_shapes
:?????????8::2B
dense_6/StatefulPartitionedCalldense_6/StatefulPartitionedCall:' #
!
_user_specified_name	input_1: : 
?
?
G__inference_sequential_3_layer_call_and_return_conditional_losses_63494

inputs*
&dense_6_statefulpartitionedcall_args_1*
&dense_6_statefulpartitionedcall_args_2
identity??dense_6/StatefulPartitionedCall?
dense_6/StatefulPartitionedCallStatefulPartitionedCallinputs&dense_6_statefulpartitionedcall_args_1&dense_6_statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-63446*
Tin
2*'
_output_shapes
:?????????*K
fFRD
B__inference_dense_6_layer_call_and_return_conditional_losses_63440*
Tout
2?
IdentityIdentity(dense_6/StatefulPartitionedCall:output:0 ^dense_6/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????8::2B
dense_6/StatefulPartitionedCalldense_6/StatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?	
?
B__inference_dense_3_layer_call_and_return_conditional_losses_64189

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes

:88i
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8?
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:8v
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0P
ReluReluBiasAdd:output:0*'
_output_shapes
:?????????8*
T0?
IdentityIdentityRelu:activations:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*
T0*'
_output_shapes
:?????????8"
identityIdentity:output:0*.
_input_shapes
:?????????8::20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp2.
MatMul/ReadVariableOpMatMul/ReadVariableOp:& "
 
_user_specified_nameinputs: : 
?	
?
@__inference_dense_layer_call_and_return_conditional_losses_63113

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0* 
_output_shapes
:
??*
dtype0j
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*
T0*(
_output_shapes
:???????????
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes	
:?*
dtype0w
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*
T0*(
_output_shapes
:??????????Q
ReluReluBiasAdd:output:0*(
_output_shapes
:??????????*
T0?
IdentityIdentityRelu:activations:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*(
_output_shapes
:??????????*
T0"
identityIdentity:output:0*/
_input_shapes
:??????????::2.
MatMul/ReadVariableOpMatMul/ReadVariableOp20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp:& "
 
_user_specified_nameinputs: : 
?%
?
G__inference_sequential_2_layer_call_and_return_conditional_losses_63315
input_1(
$dense_statefulpartitionedcall_args_1(
$dense_statefulpartitionedcall_args_2*
&dense_1_statefulpartitionedcall_args_1*
&dense_1_statefulpartitionedcall_args_2*
&dense_2_statefulpartitionedcall_args_1*
&dense_2_statefulpartitionedcall_args_2*
&dense_3_statefulpartitionedcall_args_1*
&dense_3_statefulpartitionedcall_args_2*
&dense_4_statefulpartitionedcall_args_1*
&dense_4_statefulpartitionedcall_args_2*
&dense_5_statefulpartitionedcall_args_1*
&dense_5_statefulpartitionedcall_args_2
identity??dense/StatefulPartitionedCall?dense_1/StatefulPartitionedCall?dense_2/StatefulPartitionedCall?dense_3/StatefulPartitionedCall?dense_4/StatefulPartitionedCall?dense_5/StatefulPartitionedCall?dropout/StatefulPartitionedCall?
dense/StatefulPartitionedCallStatefulPartitionedCallinput_1$dense_statefulpartitionedcall_args_1$dense_statefulpartitionedcall_args_2*,
_gradient_op_typePartitionedCall-63119**
config_proto

GPU 

CPU2J 8*(
_output_shapes
:??????????*I
fDRB
@__inference_dense_layer_call_and_return_conditional_losses_63113*
Tout
2*
Tin
2?
dense_1/StatefulPartitionedCallStatefulPartitionedCall&dense/StatefulPartitionedCall:output:0&dense_1_statefulpartitionedcall_args_1&dense_1_statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*
Tin
2*(
_output_shapes
:??????????*,
_gradient_op_typePartitionedCall-63147*K
fFRD
B__inference_dense_1_layer_call_and_return_conditional_losses_63141*
Tout
2?
dense_2/StatefulPartitionedCallStatefulPartitionedCall(dense_1/StatefulPartitionedCall:output:0&dense_2_statefulpartitionedcall_args_1&dense_2_statefulpartitionedcall_args_2*
Tin
2*K
fFRD
B__inference_dense_2_layer_call_and_return_conditional_losses_63169**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-63175*
Tout
2*'
_output_shapes
:?????????8?
dense_3/StatefulPartitionedCallStatefulPartitionedCall(dense_2/StatefulPartitionedCall:output:0&dense_3_statefulpartitionedcall_args_1&dense_3_statefulpartitionedcall_args_2*,
_gradient_op_typePartitionedCall-63203*
Tin
2*K
fFRD
B__inference_dense_3_layer_call_and_return_conditional_losses_63197*'
_output_shapes
:?????????8*
Tout
2**
config_proto

GPU 

CPU2J 8?
dense_4/StatefulPartitionedCallStatefulPartitionedCall(dense_3/StatefulPartitionedCall:output:0&dense_4_statefulpartitionedcall_args_1&dense_4_statefulpartitionedcall_args_2*
Tin
2*'
_output_shapes
:?????????8*K
fFRD
B__inference_dense_4_layer_call_and_return_conditional_losses_63225**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-63231*
Tout
2?
dense_5/StatefulPartitionedCallStatefulPartitionedCall(dense_4/StatefulPartitionedCall:output:0&dense_5_statefulpartitionedcall_args_1&dense_5_statefulpartitionedcall_args_2*K
fFRD
B__inference_dense_5_layer_call_and_return_conditional_losses_63253*
Tin
2*
Tout
2*'
_output_shapes
:?????????8*,
_gradient_op_typePartitionedCall-63259**
config_proto

GPU 

CPU2J 8?
dropout/StatefulPartitionedCallStatefulPartitionedCall(dense_5/StatefulPartitionedCall:output:0*K
fFRD
B__inference_dropout_layer_call_and_return_conditional_losses_63290*
Tout
2*,
_gradient_op_typePartitionedCall-63301*'
_output_shapes
:?????????8*
Tin
2**
config_proto

GPU 

CPU2J 8?
IdentityIdentity(dropout/StatefulPartitionedCall:output:0^dense/StatefulPartitionedCall ^dense_1/StatefulPartitionedCall ^dense_2/StatefulPartitionedCall ^dense_3/StatefulPartitionedCall ^dense_4/StatefulPartitionedCall ^dense_5/StatefulPartitionedCall ^dropout/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????8"
identityIdentity:output:0*W
_input_shapesF
D:??????????::::::::::::2>
dense/StatefulPartitionedCalldense/StatefulPartitionedCall2B
dense_1/StatefulPartitionedCalldense_1/StatefulPartitionedCall2B
dense_2/StatefulPartitionedCalldense_2/StatefulPartitionedCall2B
dense_3/StatefulPartitionedCalldense_3/StatefulPartitionedCall2B
dense_4/StatefulPartitionedCalldense_4/StatefulPartitionedCall2B
dense_5/StatefulPartitionedCalldense_5/StatefulPartitionedCall2B
dropout/StatefulPartitionedCalldropout/StatefulPartitionedCall: : : : : : :	 :
 : : :' #
!
_user_specified_name	input_1: : 
?
?
G__inference_sequential_4_layer_call_and_return_conditional_losses_63591
sequential_1_input/
+sequential_2_statefulpartitionedcall_args_1/
+sequential_2_statefulpartitionedcall_args_2/
+sequential_2_statefulpartitionedcall_args_3/
+sequential_2_statefulpartitionedcall_args_4/
+sequential_2_statefulpartitionedcall_args_5/
+sequential_2_statefulpartitionedcall_args_6/
+sequential_2_statefulpartitionedcall_args_7/
+sequential_2_statefulpartitionedcall_args_8/
+sequential_2_statefulpartitionedcall_args_90
,sequential_2_statefulpartitionedcall_args_100
,sequential_2_statefulpartitionedcall_args_110
,sequential_2_statefulpartitionedcall_args_12/
+sequential_3_statefulpartitionedcall_args_1/
+sequential_3_statefulpartitionedcall_args_2
identity??$sequential_2/StatefulPartitionedCall?$sequential_3/StatefulPartitionedCall?
sequential_1/PartitionedCallPartitionedCallsequential_1_input*
Tin
2**
config_proto

GPU 

CPU2J 8*P
fKRI
G__inference_sequential_1_layer_call_and_return_conditional_losses_63047*(
_output_shapes
:??????????*,
_gradient_op_typePartitionedCall-63048*
Tout
2?
sequential/PartitionedCallPartitionedCall%sequential_1/PartitionedCall:output:0*
Tout
2*(
_output_shapes
:??????????*,
_gradient_op_typePartitionedCall-63083*N
fIRG
E__inference_sequential_layer_call_and_return_conditional_losses_63082**
config_proto

GPU 

CPU2J 8*
Tin
2?
$sequential_2/StatefulPartitionedCallStatefulPartitionedCall#sequential/PartitionedCall:output:0+sequential_2_statefulpartitionedcall_args_1+sequential_2_statefulpartitionedcall_args_2+sequential_2_statefulpartitionedcall_args_3+sequential_2_statefulpartitionedcall_args_4+sequential_2_statefulpartitionedcall_args_5+sequential_2_statefulpartitionedcall_args_6+sequential_2_statefulpartitionedcall_args_7+sequential_2_statefulpartitionedcall_args_8+sequential_2_statefulpartitionedcall_args_9,sequential_2_statefulpartitionedcall_args_10,sequential_2_statefulpartitionedcall_args_11,sequential_2_statefulpartitionedcall_args_12*,
_gradient_op_typePartitionedCall-63367*
Tin
2*'
_output_shapes
:?????????8*P
fKRI
G__inference_sequential_2_layer_call_and_return_conditional_losses_63366**
config_proto

GPU 

CPU2J 8*
Tout
2?
$sequential_3/StatefulPartitionedCallStatefulPartitionedCall-sequential_2/StatefulPartitionedCall:output:0+sequential_3_statefulpartitionedcall_args_1+sequential_3_statefulpartitionedcall_args_2*,
_gradient_op_typePartitionedCall-63478*
Tout
2**
config_proto

GPU 

CPU2J 8*P
fKRI
G__inference_sequential_3_layer_call_and_return_conditional_losses_63477*'
_output_shapes
:?????????*
Tin
2?
IdentityIdentity-sequential_3/StatefulPartitionedCall:output:0%^sequential_2/StatefulPartitionedCall%^sequential_3/StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*_
_input_shapesN
L:??????????::::::::::::::2L
$sequential_2/StatefulPartitionedCall$sequential_2/StatefulPartitionedCall2L
$sequential_3/StatefulPartitionedCall$sequential_3/StatefulPartitionedCall: : : : : : : : :	 :
 : : : : :2 .
,
_user_specified_namesequential_1_input
?
?
#__inference_signature_wrapper_63727
sequential_1_input"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2"
statefulpartitionedcall_args_3"
statefulpartitionedcall_args_4"
statefulpartitionedcall_args_5"
statefulpartitionedcall_args_6"
statefulpartitionedcall_args_7"
statefulpartitionedcall_args_8"
statefulpartitionedcall_args_9#
statefulpartitionedcall_args_10#
statefulpartitionedcall_args_11#
statefulpartitionedcall_args_12#
statefulpartitionedcall_args_13#
statefulpartitionedcall_args_14
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallsequential_1_inputstatefulpartitionedcall_args_1statefulpartitionedcall_args_2statefulpartitionedcall_args_3statefulpartitionedcall_args_4statefulpartitionedcall_args_5statefulpartitionedcall_args_6statefulpartitionedcall_args_7statefulpartitionedcall_args_8statefulpartitionedcall_args_9statefulpartitionedcall_args_10statefulpartitionedcall_args_11statefulpartitionedcall_args_12statefulpartitionedcall_args_13statefulpartitionedcall_args_14*
Tin
2**
config_proto

GPU 

CPU2J 8*)
f$R"
 __inference__wrapped_model_63006*,
_gradient_op_typePartitionedCall-63710*'
_output_shapes
:?????????*
Tout
2?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*_
_input_shapesN
L:??????????::::::::::::::22
StatefulPartitionedCallStatefulPartitionedCall:2 .
,
_user_specified_namesequential_1_input: : : : : : : : :	 :
 : : : : 
?
C
'__inference_flatten_layer_call_fn_64124

inputs
identity?
PartitionedCallPartitionedCallinputs*
Tin
2*K
fFRD
B__inference_flatten_layer_call_and_return_conditional_losses_63018*,
_gradient_op_typePartitionedCall-63024**
config_proto

GPU 

CPU2J 8*(
_output_shapes
:??????????*
Tout
2a
IdentityIdentityPartitionedCall:output:0*
T0*(
_output_shapes
:??????????"
identityIdentity:output:0*'
_input_shapes
:??????????:& "
 
_user_specified_nameinputs
?
?
%__inference_dense_layer_call_fn_64142

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2*(
_output_shapes
:??????????*I
fDRB
@__inference_dense_layer_call_and_return_conditional_losses_63113*
Tout
2*,
_gradient_op_typePartitionedCall-63119**
config_proto

GPU 

CPU2J 8*
Tin
2?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*(
_output_shapes
:??????????*
T0"
identityIdentity:output:0*/
_input_shapes
:??????????::22
StatefulPartitionedCallStatefulPartitionedCall:& "
 
_user_specified_nameinputs: : 
?
b
E__inference_sequential_layer_call_and_return_conditional_losses_63075
input_1
identityP
IdentityIdentityinput_1*(
_output_shapes
:??????????*
T0"
identityIdentity:output:0*'
_input_shapes
:??????????:' #
!
_user_specified_name	input_1
?
`
B__inference_dropout_layer_call_and_return_conditional_losses_64257

inputs

identity_1N
IdentityIdentityinputs*'
_output_shapes
:?????????8*
T0[

Identity_1IdentityIdentity:output:0*'
_output_shapes
:?????????8*
T0"!

identity_1Identity_1:output:0*&
_input_shapes
:?????????8:& "
 
_user_specified_nameinputs
?	
?
@__inference_dense_layer_call_and_return_conditional_losses_64135

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0* 
_output_shapes
:
??*
dtype0j
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*(
_output_shapes
:??????????*
T0?
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes	
:?w
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*(
_output_shapes
:??????????*
T0Q
ReluReluBiasAdd:output:0*(
_output_shapes
:??????????*
T0?
IdentityIdentityRelu:activations:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*
T0*(
_output_shapes
:??????????"
identityIdentity:output:0*/
_input_shapes
:??????????::2.
MatMul/ReadVariableOpMatMul/ReadVariableOp20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp:& "
 
_user_specified_nameinputs: : 
?
H
,__inference_sequential_1_layer_call_fn_63911

inputs
identity?
PartitionedCallPartitionedCallinputs**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-63048*P
fKRI
G__inference_sequential_1_layer_call_and_return_conditional_losses_63047*(
_output_shapes
:??????????*
Tin
2*
Tout
2a
IdentityIdentityPartitionedCall:output:0*
T0*(
_output_shapes
:??????????"
identityIdentity:output:0*'
_input_shapes
:??????????:& "
 
_user_specified_nameinputs
?
O
,__inference_sequential_1_layer_call_fn_63064
flatten_input
identity?
PartitionedCallPartitionedCallflatten_input**
config_proto

GPU 

CPU2J 8*,
_gradient_op_typePartitionedCall-63061*
Tout
2*P
fKRI
G__inference_sequential_1_layer_call_and_return_conditional_losses_63060*(
_output_shapes
:??????????*
Tin
2a
IdentityIdentityPartitionedCall:output:0*(
_output_shapes
:??????????*
T0"
identityIdentity:output:0*'
_input_shapes
:??????????:- )
'
_user_specified_nameflatten_input
?
?
G__inference_sequential_3_layer_call_and_return_conditional_losses_64099

inputs*
&dense_6_matmul_readvariableop_resource+
'dense_6_biasadd_readvariableop_resource
identity??dense_6/BiasAdd/ReadVariableOp?dense_6/MatMul/ReadVariableOp?
dense_6/MatMul/ReadVariableOpReadVariableOp&dense_6_matmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes

:8*
dtype0y
dense_6/MatMulMatMulinputs%dense_6/MatMul/ReadVariableOp:value:0*
T0*'
_output_shapes
:??????????
dense_6/BiasAdd/ReadVariableOpReadVariableOp'dense_6_biasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:*
dtype0?
dense_6/BiasAddBiasAdddense_6/MatMul:product:0&dense_6/BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????f
dense_6/SoftmaxSoftmaxdense_6/BiasAdd:output:0*'
_output_shapes
:?????????*
T0?
IdentityIdentitydense_6/Softmax:softmax:0^dense_6/BiasAdd/ReadVariableOp^dense_6/MatMul/ReadVariableOp*'
_output_shapes
:?????????*
T0"
identityIdentity:output:0*.
_input_shapes
:?????????8::2@
dense_6/BiasAdd/ReadVariableOpdense_6/BiasAdd/ReadVariableOp2>
dense_6/MatMul/ReadVariableOpdense_6/MatMul/ReadVariableOp:& "
 
_user_specified_nameinputs: : 
?
a
E__inference_sequential_layer_call_and_return_conditional_losses_63920

inputs
identityO
IdentityIdentityinputs*(
_output_shapes
:??????????*
T0"
identityIdentity:output:0*'
_input_shapes
:??????????:& "
 
_user_specified_nameinputs
?
?
,__inference_sequential_2_layer_call_fn_64060

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2"
statefulpartitionedcall_args_3"
statefulpartitionedcall_args_4"
statefulpartitionedcall_args_5"
statefulpartitionedcall_args_6"
statefulpartitionedcall_args_7"
statefulpartitionedcall_args_8"
statefulpartitionedcall_args_9#
statefulpartitionedcall_args_10#
statefulpartitionedcall_args_11#
statefulpartitionedcall_args_12
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2statefulpartitionedcall_args_3statefulpartitionedcall_args_4statefulpartitionedcall_args_5statefulpartitionedcall_args_6statefulpartitionedcall_args_7statefulpartitionedcall_args_8statefulpartitionedcall_args_9statefulpartitionedcall_args_10statefulpartitionedcall_args_11statefulpartitionedcall_args_12*
Tin
2**
config_proto

GPU 

CPU2J 8*P
fKRI
G__inference_sequential_2_layer_call_and_return_conditional_losses_63366*,
_gradient_op_typePartitionedCall-63367*'
_output_shapes
:?????????8*
Tout
2?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????8"
identityIdentity:output:0*W
_input_shapesF
D:??????????::::::::::::22
StatefulPartitionedCallStatefulPartitionedCall: : : : : : : : :	 :
 : : :& "
 
_user_specified_nameinputs
?	
?
B__inference_dense_2_layer_call_and_return_conditional_losses_64171

inputs"
matmul_readvariableop_resource#
biasadd_readvariableop_resource
identity??BiasAdd/ReadVariableOp?MatMul/ReadVariableOp?
MatMul/ReadVariableOpReadVariableOpmatmul_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
dtype0*
_output_shapes
:	?8i
MatMulMatMulinputsMatMul/ReadVariableOp:value:0*'
_output_shapes
:?????????8*
T0?
BiasAdd/ReadVariableOpReadVariableOpbiasadd_readvariableop_resource",/job:localhost/replica:0/task:0/device:CPU:0*
_output_shapes
:8*
dtype0v
BiasAddBiasAddMatMul:product:0BiasAdd/ReadVariableOp:value:0*
T0*'
_output_shapes
:?????????8P
ReluReluBiasAdd:output:0*
T0*'
_output_shapes
:?????????8?
IdentityIdentityRelu:activations:0^BiasAdd/ReadVariableOp^MatMul/ReadVariableOp*
T0*'
_output_shapes
:?????????8"
identityIdentity:output:0*/
_input_shapes
:??????????::2.
MatMul/ReadVariableOpMatMul/ReadVariableOp20
BiasAdd/ReadVariableOpBiasAdd/ReadVariableOp:& "
 
_user_specified_nameinputs: : 
?
?
,__inference_sequential_3_layer_call_fn_64106

inputs"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinputsstatefulpartitionedcall_args_1statefulpartitionedcall_args_2**
config_proto

GPU 

CPU2J 8*
Tout
2*'
_output_shapes
:?????????*
Tin
2*,
_gradient_op_typePartitionedCall-63478*P
fKRI
G__inference_sequential_3_layer_call_and_return_conditional_losses_63477?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*
T0*'
_output_shapes
:?????????"
identityIdentity:output:0*.
_input_shapes
:?????????8::22
StatefulPartitionedCallStatefulPartitionedCall: : :& "
 
_user_specified_nameinputs
?
?
,__inference_sequential_2_layer_call_fn_63425
input_1"
statefulpartitionedcall_args_1"
statefulpartitionedcall_args_2"
statefulpartitionedcall_args_3"
statefulpartitionedcall_args_4"
statefulpartitionedcall_args_5"
statefulpartitionedcall_args_6"
statefulpartitionedcall_args_7"
statefulpartitionedcall_args_8"
statefulpartitionedcall_args_9#
statefulpartitionedcall_args_10#
statefulpartitionedcall_args_11#
statefulpartitionedcall_args_12
identity??StatefulPartitionedCall?
StatefulPartitionedCallStatefulPartitionedCallinput_1statefulpartitionedcall_args_1statefulpartitionedcall_args_2statefulpartitionedcall_args_3statefulpartitionedcall_args_4statefulpartitionedcall_args_5statefulpartitionedcall_args_6statefulpartitionedcall_args_7statefulpartitionedcall_args_8statefulpartitionedcall_args_9statefulpartitionedcall_args_10statefulpartitionedcall_args_11statefulpartitionedcall_args_12**
config_proto

GPU 

CPU2J 8*
Tout
2*
Tin
2*,
_gradient_op_typePartitionedCall-63410*P
fKRI
G__inference_sequential_2_layer_call_and_return_conditional_losses_63409*'
_output_shapes
:?????????8?
IdentityIdentity StatefulPartitionedCall:output:0^StatefulPartitionedCall*'
_output_shapes
:?????????8*
T0"
identityIdentity:output:0*W
_input_shapesF
D:??????????::::::::::::22
StatefulPartitionedCallStatefulPartitionedCall: : : : : : : :	 :
 : : :' #
!
_user_specified_name	input_1: "wL
saver_filename:0StatefulPartitionedCall_1:0StatefulPartitionedCall_28"
saved_model_main_op

NoOp*>
__saved_model_init_op%#
__saved_model_init_op

NoOp*?
serving_default?
R
sequential_1_input<
$serving_default_sequential_1_input:0??????????@
sequential_30
StatefulPartitionedCall:0?????????tensorflow/serving/predict:??
?B
layer-0
layer-1
layer-2
layer_with_weights-0
layer-3
layer_with_weights-1
layer-4
	optimizer
	variables
trainable_variables
	regularization_losses

	keras_api

signatures
?__call__
?_default_save_signature
+?&call_and_return_all_conditional_losses"??
_tf_keras_sequential??{"class_name": "Sequential", "name": "sequential_4", "trainable": true, "expects_training_arg": true, "dtype": "float32", "batch_input_shape": null, "config": {"name": "sequential_4", "layers": [{"class_name": "Sequential", "config": {"name": "sequential_1", "layers": [{"class_name": "Flatten", "config": {"name": "flatten", "trainable": false, "batch_input_shape": [null, 128], "dtype": "float32", "data_format": "channels_last"}}], "batch_input_shape": [null, 128]}}, {"class_name": "Sequential", "config": {"name": "sequential", "layers": []}}, {"class_name": "Sequential", "config": {"name": "sequential_2", "layers": [{"class_name": "Dense", "config": {"name": "dense", "trainable": true, "dtype": "float32", "units": 256, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_1", "trainable": true, "dtype": "float32", "units": 128, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_2", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_3", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_4", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_5", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dropout", "config": {"name": "dropout", "trainable": true, "dtype": "float32", "rate": 0.2, "noise_shape": null, "seed": null}}]}}, {"class_name": "Sequential", "config": {"name": "sequential_3", "layers": [{"class_name": "Dense", "config": {"name": "dense_6", "trainable": true, "dtype": "float32", "units": 4, "activation": "softmax", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}]}}]}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": null, "max_ndim": null, "min_ndim": 1, "axes": {}}}, "keras_version": "2.2.4-tf", "backend": "tensorflow", "model_config": {"class_name": "Sequential", "config": {"name": "sequential_4", "layers": [{"class_name": "Sequential", "config": {"name": "sequential_1", "layers": [{"class_name": "Flatten", "config": {"name": "flatten", "trainable": false, "batch_input_shape": [null, 128], "dtype": "float32", "data_format": "channels_last"}}], "batch_input_shape": [null, 128]}}, {"class_name": "Sequential", "config": {"name": "sequential", "layers": []}}, {"class_name": "Sequential", "config": {"name": "sequential_2", "layers": [{"class_name": "Dense", "config": {"name": "dense", "trainable": true, "dtype": "float32", "units": 256, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_1", "trainable": true, "dtype": "float32", "units": 128, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_2", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_3", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_4", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_5", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dropout", "config": {"name": "dropout", "trainable": true, "dtype": "float32", "rate": 0.2, "noise_shape": null, "seed": null}}]}}, {"class_name": "Sequential", "config": {"name": "sequential_3", "layers": [{"class_name": "Dense", "config": {"name": "dense_6", "trainable": true, "dtype": "float32", "units": 4, "activation": "softmax", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}]}}]}}, "training_config": {"loss": "categorical_crossentropy", "metrics": ["acc"], "weighted_metrics": null, "sample_weight_mode": null, "loss_weights": null, "optimizer_config": {"class_name": "Adam", "config": {"name": "Adam", "learning_rate": 9.999999747378752e-06, "decay": 9.999999974752427e-07, "beta_1": 0.8999999761581421, "beta_2": 0.9990000128746033, "epsilon": 1e-07, "amsgrad": false}}}}
?
	variables
trainable_variables
regularization_losses
	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "InputLayer", "name": "sequential_1_input", "trainable": true, "expects_training_arg": true, "dtype": "float32", "batch_input_shape": [null, 128], "config": {"batch_input_shape": [null, 128], "dtype": "float32", "sparse": false, "name": "sequential_1_input"}}
?
layer-0
layer-1
	variables
trainable_variables
regularization_losses
	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_sequential?{"class_name": "Sequential", "name": "sequential_1", "trainable": false, "expects_training_arg": true, "dtype": "float32", "batch_input_shape": null, "config": {"name": "sequential_1", "layers": [{"class_name": "Flatten", "config": {"name": "flatten", "trainable": false, "batch_input_shape": [null, 128], "dtype": "float32", "data_format": "channels_last"}}]}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": null, "max_ndim": null, "min_ndim": 1, "axes": {}}}, "keras_version": "2.2.4-tf", "backend": "tensorflow", "model_config": {"class_name": "Sequential", "config": {"name": "sequential_1", "layers": [{"class_name": "Flatten", "config": {"name": "flatten", "trainable": false, "batch_input_shape": [null, 128], "dtype": "float32", "data_format": "channels_last"}}]}}}
?
	variables
trainable_variables
regularization_losses
	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_sequential?{"class_name": "Sequential", "name": "sequential", "trainable": false, "expects_training_arg": true, "dtype": "float32", "batch_input_shape": null, "config": {"name": "sequential", "layers": []}, "keras_version": "2.2.4-tf", "backend": "tensorflow", "model_config": {"class_name": "Sequential", "config": {"name": "sequential", "layers": []}}}
?1
layer_with_weights-0
layer-0
layer_with_weights-1
layer-1
layer_with_weights-2
layer-2
layer_with_weights-3
layer-3
layer_with_weights-4
layer-4
layer_with_weights-5
layer-5
 layer-6
!	variables
"trainable_variables
#regularization_losses
$	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?.
_tf_keras_sequential?-{"class_name": "Sequential", "name": "sequential_2", "trainable": true, "expects_training_arg": true, "dtype": "float32", "batch_input_shape": null, "config": {"name": "sequential_2", "layers": [{"class_name": "Dense", "config": {"name": "dense", "trainable": true, "dtype": "float32", "units": 256, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_1", "trainable": true, "dtype": "float32", "units": 128, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_2", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_3", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_4", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_5", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dropout", "config": {"name": "dropout", "trainable": true, "dtype": "float32", "rate": 0.2, "noise_shape": null, "seed": null}}]}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": null, "max_ndim": null, "min_ndim": 2, "axes": {"-1": 128}}}, "keras_version": "2.2.4-tf", "backend": "tensorflow", "model_config": {"class_name": "Sequential", "config": {"name": "sequential_2", "layers": [{"class_name": "Dense", "config": {"name": "dense", "trainable": true, "dtype": "float32", "units": 256, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_1", "trainable": true, "dtype": "float32", "units": 128, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_2", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_3", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_4", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dense", "config": {"name": "dense_5", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}, {"class_name": "Dropout", "config": {"name": "dropout", "trainable": true, "dtype": "float32", "rate": 0.2, "noise_shape": null, "seed": null}}]}}}
?
%layer_with_weights-0
%layer-0
&	variables
'trainable_variables
(regularization_losses
)	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?

_tf_keras_sequential?
{"class_name": "Sequential", "name": "sequential_3", "trainable": true, "expects_training_arg": true, "dtype": "float32", "batch_input_shape": null, "config": {"name": "sequential_3", "layers": [{"class_name": "Dense", "config": {"name": "dense_6", "trainable": true, "dtype": "float32", "units": 4, "activation": "softmax", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}]}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": null, "max_ndim": null, "min_ndim": 2, "axes": {"-1": 56}}}, "keras_version": "2.2.4-tf", "backend": "tensorflow", "model_config": {"class_name": "Sequential", "config": {"name": "sequential_3", "layers": [{"class_name": "Dense", "config": {"name": "dense_6", "trainable": true, "dtype": "float32", "units": 4, "activation": "softmax", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}}]}}}
?
*iter

+beta_1

,beta_2
	-decay
.learning_rate/m?0m?1m?2m?3m?4m?5m?6m?7m?8m?9m?:m?;m?<m?/v?0v?1v?2v?3v?4v?5v?6v?7v?8v?9v?:v?;v?<v?"
	optimizer
?
/0
01
12
23
34
45
56
67
78
89
910
:11
;12
<13"
trackable_list_wrapper
?
/0
01
12
23
34
45
56
67
78
89
910
:11
;12
<13"
trackable_list_wrapper
 "
trackable_list_wrapper
?
=non_trainable_variables
>layer_regularization_losses
	variables
?metrics

@layers
trainable_variables
	regularization_losses
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
Anon_trainable_variables
Blayer_regularization_losses
	variables
Cmetrics

Dlayers
trainable_variables
regularization_losses
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
?
E	variables
Ftrainable_variables
Gregularization_losses
H	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "InputLayer", "name": "flatten_input", "trainable": false, "expects_training_arg": true, "dtype": "float32", "batch_input_shape": [null, 128], "config": {"batch_input_shape": [null, 128], "dtype": "float32", "sparse": false, "name": "flatten_input"}}
?
I	variables
Jtrainable_variables
Kregularization_losses
L	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "Flatten", "name": "flatten", "trainable": false, "expects_training_arg": false, "dtype": "float32", "batch_input_shape": [null, 128], "config": {"name": "flatten", "trainable": false, "batch_input_shape": [null, 128], "dtype": "float32", "data_format": "channels_last"}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": null, "max_ndim": null, "min_ndim": 1, "axes": {}}}}
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
?
Mnon_trainable_variables
Nlayer_regularization_losses
	variables
Ometrics

Players
trainable_variables
regularization_losses
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
Qnon_trainable_variables
Rlayer_regularization_losses
	variables
Smetrics

Tlayers
trainable_variables
regularization_losses
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
?

/kernel
0bias
U	variables
Vtrainable_variables
Wregularization_losses
X	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "Dense", "name": "dense", "trainable": true, "expects_training_arg": false, "dtype": "float32", "batch_input_shape": null, "config": {"name": "dense", "trainable": true, "dtype": "float32", "units": 256, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": null, "max_ndim": null, "min_ndim": 2, "axes": {"-1": 128}}}}
?

1kernel
2bias
Y	variables
Ztrainable_variables
[regularization_losses
\	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "Dense", "name": "dense_1", "trainable": true, "expects_training_arg": false, "dtype": "float32", "batch_input_shape": null, "config": {"name": "dense_1", "trainable": true, "dtype": "float32", "units": 128, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": null, "max_ndim": null, "min_ndim": 2, "axes": {"-1": 256}}}}
?

3kernel
4bias
]	variables
^trainable_variables
_regularization_losses
`	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "Dense", "name": "dense_2", "trainable": true, "expects_training_arg": false, "dtype": "float32", "batch_input_shape": null, "config": {"name": "dense_2", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": null, "max_ndim": null, "min_ndim": 2, "axes": {"-1": 128}}}}
?

5kernel
6bias
a	variables
btrainable_variables
cregularization_losses
d	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "Dense", "name": "dense_3", "trainable": true, "expects_training_arg": false, "dtype": "float32", "batch_input_shape": null, "config": {"name": "dense_3", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": null, "max_ndim": null, "min_ndim": 2, "axes": {"-1": 56}}}}
?

7kernel
8bias
e	variables
ftrainable_variables
gregularization_losses
h	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "Dense", "name": "dense_4", "trainable": true, "expects_training_arg": false, "dtype": "float32", "batch_input_shape": null, "config": {"name": "dense_4", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": null, "max_ndim": null, "min_ndim": 2, "axes": {"-1": 56}}}}
?

9kernel
:bias
i	variables
jtrainable_variables
kregularization_losses
l	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "Dense", "name": "dense_5", "trainable": true, "expects_training_arg": false, "dtype": "float32", "batch_input_shape": null, "config": {"name": "dense_5", "trainable": true, "dtype": "float32", "units": 56, "activation": "relu", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": null, "max_ndim": null, "min_ndim": 2, "axes": {"-1": 56}}}}
?
m	variables
ntrainable_variables
oregularization_losses
p	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "Dropout", "name": "dropout", "trainable": true, "expects_training_arg": true, "dtype": "float32", "batch_input_shape": null, "config": {"name": "dropout", "trainable": true, "dtype": "float32", "rate": 0.2, "noise_shape": null, "seed": null}}
v
/0
01
12
23
34
45
56
67
78
89
910
:11"
trackable_list_wrapper
v
/0
01
12
23
34
45
56
67
78
89
910
:11"
trackable_list_wrapper
 "
trackable_list_wrapper
?
qnon_trainable_variables
rlayer_regularization_losses
!	variables
smetrics

tlayers
"trainable_variables
#regularization_losses
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
?

;kernel
<bias
u	variables
vtrainable_variables
wregularization_losses
x	keras_api
?__call__
+?&call_and_return_all_conditional_losses"?
_tf_keras_layer?{"class_name": "Dense", "name": "dense_6", "trainable": true, "expects_training_arg": false, "dtype": "float32", "batch_input_shape": null, "config": {"name": "dense_6", "trainable": true, "dtype": "float32", "units": 4, "activation": "softmax", "use_bias": true, "kernel_initializer": {"class_name": "GlorotUniform", "config": {"seed": null}}, "bias_initializer": {"class_name": "Zeros", "config": {}}, "kernel_regularizer": null, "bias_regularizer": null, "activity_regularizer": null, "kernel_constraint": null, "bias_constraint": null}, "input_spec": {"class_name": "InputSpec", "config": {"dtype": null, "shape": null, "ndim": null, "max_ndim": null, "min_ndim": 2, "axes": {"-1": 56}}}}
.
;0
<1"
trackable_list_wrapper
.
;0
<1"
trackable_list_wrapper
 "
trackable_list_wrapper
?
ynon_trainable_variables
zlayer_regularization_losses
&	variables
{metrics

|layers
'trainable_variables
(regularization_losses
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
": 
??2dense_8/kernel
:?2dense_8/bias
$:"
??2dense_1_2/kernel
:?2dense_1_2/bias
#:!	?82dense_2_2/kernel
:82dense_2_2/bias
": 882dense_3_2/kernel
:82dense_3_2/bias
": 882dense_4_2/kernel
:82dense_4_2/bias
": 882dense_5_2/kernel
:82dense_5_2/bias
": 82dense_6_3/kernel
:2dense_6_3/bias
 "
trackable_list_wrapper
 "
trackable_list_wrapper
'
}0"
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
~non_trainable_variables
layer_regularization_losses
E	variables
?metrics
?layers
Ftrainable_variables
Gregularization_losses
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
?non_trainable_variables
 ?layer_regularization_losses
I	variables
?metrics
?layers
Jtrainable_variables
Kregularization_losses
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
0"
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
.
/0
01"
trackable_list_wrapper
 "
trackable_list_wrapper
?
?non_trainable_variables
 ?layer_regularization_losses
U	variables
?metrics
?layers
Vtrainable_variables
Wregularization_losses
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
.
10
21"
trackable_list_wrapper
.
10
21"
trackable_list_wrapper
 "
trackable_list_wrapper
?
?non_trainable_variables
 ?layer_regularization_losses
Y	variables
?metrics
?layers
Ztrainable_variables
[regularization_losses
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
.
30
41"
trackable_list_wrapper
.
30
41"
trackable_list_wrapper
 "
trackable_list_wrapper
?
?non_trainable_variables
 ?layer_regularization_losses
]	variables
?metrics
?layers
^trainable_variables
_regularization_losses
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
.
50
61"
trackable_list_wrapper
.
50
61"
trackable_list_wrapper
 "
trackable_list_wrapper
?
?non_trainable_variables
 ?layer_regularization_losses
a	variables
?metrics
?layers
btrainable_variables
cregularization_losses
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
.
70
81"
trackable_list_wrapper
.
70
81"
trackable_list_wrapper
 "
trackable_list_wrapper
?
?non_trainable_variables
 ?layer_regularization_losses
e	variables
?metrics
?layers
ftrainable_variables
gregularization_losses
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
.
90
:1"
trackable_list_wrapper
.
90
:1"
trackable_list_wrapper
 "
trackable_list_wrapper
?
?non_trainable_variables
 ?layer_regularization_losses
i	variables
?metrics
?layers
jtrainable_variables
kregularization_losses
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
?non_trainable_variables
 ?layer_regularization_losses
m	variables
?metrics
?layers
ntrainable_variables
oregularization_losses
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
Q
0
1
2
3
4
5
 6"
trackable_list_wrapper
.
;0
<1"
trackable_list_wrapper
.
;0
<1"
trackable_list_wrapper
 "
trackable_list_wrapper
?
?non_trainable_variables
 ?layer_regularization_losses
u	variables
?metrics
?layers
vtrainable_variables
wregularization_losses
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
%0"
trackable_list_wrapper
?

?total

?count
?
_fn_kwargs
?	variables
?trainable_variables
?regularization_losses
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
0
?0
?1"
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
?
?non_trainable_variables
 ?layer_regularization_losses
?	variables
?metrics
?layers
?trainable_variables
?regularization_losses
?__call__
+?&call_and_return_all_conditional_losses
'?"call_and_return_conditional_losses"
_generic_user_object
0
?0
?1"
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
 "
trackable_list_wrapper
':%
??2Adam/dense_8/kernel/m
 :?2Adam/dense_8/bias/m
):'
??2Adam/dense_1_2/kernel/m
": ?2Adam/dense_1_2/bias/m
(:&	?82Adam/dense_2_2/kernel/m
!:82Adam/dense_2_2/bias/m
':%882Adam/dense_3_2/kernel/m
!:82Adam/dense_3_2/bias/m
':%882Adam/dense_4_2/kernel/m
!:82Adam/dense_4_2/bias/m
':%882Adam/dense_5_2/kernel/m
!:82Adam/dense_5_2/bias/m
':%82Adam/dense_6_3/kernel/m
!:2Adam/dense_6_3/bias/m
':%
??2Adam/dense_8/kernel/v
 :?2Adam/dense_8/bias/v
):'
??2Adam/dense_1_2/kernel/v
": ?2Adam/dense_1_2/bias/v
(:&	?82Adam/dense_2_2/kernel/v
!:82Adam/dense_2_2/bias/v
':%882Adam/dense_3_2/kernel/v
!:82Adam/dense_3_2/bias/v
':%882Adam/dense_4_2/kernel/v
!:82Adam/dense_4_2/bias/v
':%882Adam/dense_5_2/kernel/v
!:82Adam/dense_5_2/bias/v
':%82Adam/dense_6_3/kernel/v
!:2Adam/dense_6_3/bias/v
?2?
,__inference_sequential_4_layer_call_fn_63658
,__inference_sequential_4_layer_call_fn_63875
,__inference_sequential_4_layer_call_fn_63702
,__inference_sequential_4_layer_call_fn_63894?
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
?2?
 __inference__wrapped_model_63006?
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
annotations? *2?/
-?*
sequential_1_input??????????
?2?
G__inference_sequential_4_layer_call_and_return_conditional_losses_63800
G__inference_sequential_4_layer_call_and_return_conditional_losses_63591
G__inference_sequential_4_layer_call_and_return_conditional_losses_63856
G__inference_sequential_4_layer_call_and_return_conditional_losses_63615?
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
,__inference_sequential_1_layer_call_fn_63916
,__inference_sequential_1_layer_call_fn_63911
,__inference_sequential_1_layer_call_fn_63064
,__inference_sequential_1_layer_call_fn_63051?
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
G__inference_sequential_1_layer_call_and_return_conditional_losses_63039
G__inference_sequential_1_layer_call_and_return_conditional_losses_63900
G__inference_sequential_1_layer_call_and_return_conditional_losses_63032
G__inference_sequential_1_layer_call_and_return_conditional_losses_63906?
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
*__inference_sequential_layer_call_fn_63086
*__inference_sequential_layer_call_fn_63098
*__inference_sequential_layer_call_fn_63929
*__inference_sequential_layer_call_fn_63934?
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
E__inference_sequential_layer_call_and_return_conditional_losses_63920
E__inference_sequential_layer_call_and_return_conditional_losses_63924
E__inference_sequential_layer_call_and_return_conditional_losses_63069
E__inference_sequential_layer_call_and_return_conditional_losses_63075?
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
,__inference_sequential_2_layer_call_fn_63382
,__inference_sequential_2_layer_call_fn_64077
,__inference_sequential_2_layer_call_fn_63425
,__inference_sequential_2_layer_call_fn_64060?
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
G__inference_sequential_2_layer_call_and_return_conditional_losses_63340
G__inference_sequential_2_layer_call_and_return_conditional_losses_63996
G__inference_sequential_2_layer_call_and_return_conditional_losses_64043
G__inference_sequential_2_layer_call_and_return_conditional_losses_63315?
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
,__inference_sequential_3_layer_call_fn_63483
,__inference_sequential_3_layer_call_fn_64106
,__inference_sequential_3_layer_call_fn_64113
,__inference_sequential_3_layer_call_fn_63500?
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
G__inference_sequential_3_layer_call_and_return_conditional_losses_64088
G__inference_sequential_3_layer_call_and_return_conditional_losses_64099
G__inference_sequential_3_layer_call_and_return_conditional_losses_63458
G__inference_sequential_3_layer_call_and_return_conditional_losses_63467?
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
#__inference_signature_wrapper_63727sequential_1_input
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
'__inference_flatten_layer_call_fn_64124?
???
FullArgSpec
args?
jself
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
annotations? *
 
?2?
B__inference_flatten_layer_call_and_return_conditional_losses_64119?
???
FullArgSpec
args?
jself
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
annotations? *
 
?2?
%__inference_dense_layer_call_fn_64142?
???
FullArgSpec
args?
jself
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
annotations? *
 
?2?
@__inference_dense_layer_call_and_return_conditional_losses_64135?
???
FullArgSpec
args?
jself
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
annotations? *
 
?2?
'__inference_dense_1_layer_call_fn_64160?
???
FullArgSpec
args?
jself
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
annotations? *
 
?2?
B__inference_dense_1_layer_call_and_return_conditional_losses_64153?
???
FullArgSpec
args?
jself
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
annotations? *
 
?2?
'__inference_dense_2_layer_call_fn_64178?
???
FullArgSpec
args?
jself
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
annotations? *
 
?2?
B__inference_dense_2_layer_call_and_return_conditional_losses_64171?
???
FullArgSpec
args?
jself
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
annotations? *
 
?2?
'__inference_dense_3_layer_call_fn_64196?
???
FullArgSpec
args?
jself
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
annotations? *
 
?2?
B__inference_dense_3_layer_call_and_return_conditional_losses_64189?
???
FullArgSpec
args?
jself
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
annotations? *
 
?2?
'__inference_dense_4_layer_call_fn_64214?
???
FullArgSpec
args?
jself
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
annotations? *
 
?2?
B__inference_dense_4_layer_call_and_return_conditional_losses_64207?
???
FullArgSpec
args?
jself
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
annotations? *
 
?2?
'__inference_dense_5_layer_call_fn_64232?
???
FullArgSpec
args?
jself
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
annotations? *
 
?2?
B__inference_dense_5_layer_call_and_return_conditional_losses_64225?
???
FullArgSpec
args?
jself
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
annotations? *
 
?2?
'__inference_dropout_layer_call_fn_64262
'__inference_dropout_layer_call_fn_64267?
???
FullArgSpec)
args!?
jself
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
annotations? *
 
?2?
B__inference_dropout_layer_call_and_return_conditional_losses_64252
B__inference_dropout_layer_call_and_return_conditional_losses_64257?
???
FullArgSpec)
args!?
jself
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
annotations? *
 
?2?
'__inference_dense_6_layer_call_fn_64285?
???
FullArgSpec
args?
jself
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
annotations? *
 
?2?
B__inference_dense_6_layer_call_and_return_conditional_losses_64278?
???
FullArgSpec
args?
jself
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
 ?
G__inference_sequential_3_layer_call_and_return_conditional_losses_63458e;<8?5
.?+
!?
input_1?????????8
p

 
? "%?"
?
0?????????
? ?
,__inference_sequential_4_layer_call_fn_63894d/0123456789:;<8?5
.?+
!?
inputs??????????
p 

 
? "???????????
G__inference_sequential_2_layer_call_and_return_conditional_losses_64043o/0123456789:8?5
.?+
!?
inputs??????????
p 

 
? "%?"
?
0?????????8
? ?
G__inference_sequential_4_layer_call_and_return_conditional_losses_63800q/0123456789:;<8?5
.?+
!?
inputs??????????
p

 
? "%?"
?
0?????????
? z
'__inference_dense_6_layer_call_fn_64285O;</?,
%?"
 ?
inputs?????????8
? "??????????{
'__inference_dense_2_layer_call_fn_64178P340?-
&?#
!?
inputs??????????
? "??????????8?
B__inference_dense_1_layer_call_and_return_conditional_losses_64153^120?-
&?#
!?
inputs??????????
? "&?#
?
0??????????
? z
'__inference_dense_3_layer_call_fn_64196O56/?,
%?"
 ?
inputs?????????8
? "??????????8?
B__inference_flatten_layer_call_and_return_conditional_losses_64119Z0?-
&?#
!?
inputs??????????
? "&?#
?
0??????????
? ?
,__inference_sequential_2_layer_call_fn_64060b/0123456789:8?5
.?+
!?
inputs??????????
p

 
? "??????????8?
B__inference_dropout_layer_call_and_return_conditional_losses_64257\3?0
)?&
 ?
inputs?????????8
p 
? "%?"
?
0?????????8
? ?
,__inference_sequential_4_layer_call_fn_63702p/0123456789:;<D?A
:?7
-?*
sequential_1_input??????????
p 

 
? "???????????
G__inference_sequential_3_layer_call_and_return_conditional_losses_64099d;<7?4
-?*
 ?
inputs?????????8
p 

 
? "%?"
?
0?????????
? ?
G__inference_sequential_4_layer_call_and_return_conditional_losses_63591}/0123456789:;<D?A
:?7
-?*
sequential_1_input??????????
p

 
? "%?"
?
0?????????
? z
%__inference_dense_layer_call_fn_64142Q/00?-
&?#
!?
inputs??????????
? "????????????
G__inference_sequential_3_layer_call_and_return_conditional_losses_64088d;<7?4
-?*
 ?
inputs?????????8
p

 
? "%?"
?
0?????????
? ?
*__inference_sequential_layer_call_fn_63098V9?6
/?,
"?
input_1??????????
p 

 
? "????????????
*__inference_sequential_layer_call_fn_63934U8?5
.?+
!?
inputs??????????
p 

 
? "????????????
,__inference_sequential_1_layer_call_fn_63916U8?5
.?+
!?
inputs??????????
p 

 
? "????????????
B__inference_dense_3_layer_call_and_return_conditional_losses_64189\56/?,
%?"
 ?
inputs?????????8
? "%?"
?
0?????????8
? ?
,__inference_sequential_3_layer_call_fn_64106W;<7?4
-?*
 ?
inputs?????????8
p

 
? "???????????
B__inference_dropout_layer_call_and_return_conditional_losses_64252\3?0
)?&
 ?
inputs?????????8
p
? "%?"
?
0?????????8
? ?
,__inference_sequential_2_layer_call_fn_63425c/0123456789:9?6
/?,
"?
input_1??????????
p 

 
? "??????????8?
,__inference_sequential_2_layer_call_fn_63382c/0123456789:9?6
/?,
"?
input_1??????????
p

 
? "??????????8?
G__inference_sequential_3_layer_call_and_return_conditional_losses_63467e;<8?5
.?+
!?
input_1?????????8
p 

 
? "%?"
?
0?????????
? ?
B__inference_dense_4_layer_call_and_return_conditional_losses_64207\78/?,
%?"
 ?
inputs?????????8
? "%?"
?
0?????????8
? ?
*__inference_sequential_layer_call_fn_63086V9?6
/?,
"?
input_1??????????
p

 
? "????????????
B__inference_dense_5_layer_call_and_return_conditional_losses_64225\9:/?,
%?"
 ?
inputs?????????8
? "%?"
?
0?????????8
? ?
,__inference_sequential_4_layer_call_fn_63658p/0123456789:;<D?A
:?7
-?*
sequential_1_input??????????
p

 
? "???????????
#__inference_signature_wrapper_63727?/0123456789:;<R?O
? 
H?E
C
sequential_1_input-?*
sequential_1_input??????????";?8
6
sequential_3&?#
sequential_3??????????
G__inference_sequential_1_layer_call_and_return_conditional_losses_63032i??<
5?2
(?%
flatten_input??????????
p

 
? "&?#
?
0??????????
? ?
G__inference_sequential_1_layer_call_and_return_conditional_losses_63039i??<
5?2
(?%
flatten_input??????????
p 

 
? "&?#
?
0??????????
? ?
,__inference_sequential_3_layer_call_fn_63500X;<8?5
.?+
!?
input_1?????????8
p 

 
? "???????????
,__inference_sequential_1_layer_call_fn_63911U8?5
.?+
!?
inputs??????????
p

 
? "????????????
 __inference__wrapped_model_63006?/0123456789:;<<?9
2?/
-?*
sequential_1_input??????????
? ";?8
6
sequential_3&?#
sequential_3??????????
*__inference_sequential_layer_call_fn_63929U8?5
.?+
!?
inputs??????????
p

 
? "???????????x
'__inference_flatten_layer_call_fn_64124M0?-
&?#
!?
inputs??????????
? "????????????
G__inference_sequential_2_layer_call_and_return_conditional_losses_63340p/0123456789:9?6
/?,
"?
input_1??????????
p 

 
? "%?"
?
0?????????8
? ?
,__inference_sequential_1_layer_call_fn_63064\??<
5?2
(?%
flatten_input??????????
p 

 
? "????????????
E__inference_sequential_layer_call_and_return_conditional_losses_63924b8?5
.?+
!?
inputs??????????
p 

 
? "&?#
?
0??????????
? ?
G__inference_sequential_4_layer_call_and_return_conditional_losses_63615}/0123456789:;<D?A
:?7
-?*
sequential_1_input??????????
p 

 
? "%?"
?
0?????????
? ?
,__inference_sequential_4_layer_call_fn_63875d/0123456789:;<8?5
.?+
!?
inputs??????????
p

 
? "??????????|
'__inference_dense_1_layer_call_fn_64160Q120?-
&?#
!?
inputs??????????
? "????????????
,__inference_sequential_3_layer_call_fn_64113W;<7?4
-?*
 ?
inputs?????????8
p 

 
? "???????????
E__inference_sequential_layer_call_and_return_conditional_losses_63069c9?6
/?,
"?
input_1??????????
p

 
? "&?#
?
0??????????
? z
'__inference_dropout_layer_call_fn_64267O3?0
)?&
 ?
inputs?????????8
p 
? "??????????8?
,__inference_sequential_2_layer_call_fn_64077b/0123456789:8?5
.?+
!?
inputs??????????
p 

 
? "??????????8z
'__inference_dense_4_layer_call_fn_64214O78/?,
%?"
 ?
inputs?????????8
? "??????????8?
,__inference_sequential_3_layer_call_fn_63483X;<8?5
.?+
!?
input_1?????????8
p

 
? "???????????
G__inference_sequential_4_layer_call_and_return_conditional_losses_63856q/0123456789:;<8?5
.?+
!?
inputs??????????
p 

 
? "%?"
?
0?????????
? ?
B__inference_dense_6_layer_call_and_return_conditional_losses_64278\;</?,
%?"
 ?
inputs?????????8
? "%?"
?
0?????????
? z
'__inference_dense_5_layer_call_fn_64232O9:/?,
%?"
 ?
inputs?????????8
? "??????????8z
'__inference_dropout_layer_call_fn_64262O3?0
)?&
 ?
inputs?????????8
p
? "??????????8?
G__inference_sequential_2_layer_call_and_return_conditional_losses_63315p/0123456789:9?6
/?,
"?
input_1??????????
p

 
? "%?"
?
0?????????8
? ?
G__inference_sequential_1_layer_call_and_return_conditional_losses_63900b8?5
.?+
!?
inputs??????????
p

 
? "&?#
?
0??????????
? ?
G__inference_sequential_1_layer_call_and_return_conditional_losses_63906b8?5
.?+
!?
inputs??????????
p 

 
? "&?#
?
0??????????
? ?
E__inference_sequential_layer_call_and_return_conditional_losses_63920b8?5
.?+
!?
inputs??????????
p

 
? "&?#
?
0??????????
? ?
,__inference_sequential_1_layer_call_fn_63051\??<
5?2
(?%
flatten_input??????????
p

 
? "????????????
B__inference_dense_2_layer_call_and_return_conditional_losses_64171]340?-
&?#
!?
inputs??????????
? "%?"
?
0?????????8
? ?
@__inference_dense_layer_call_and_return_conditional_losses_64135^/00?-
&?#
!?
inputs??????????
? "&?#
?
0??????????
? ?
G__inference_sequential_2_layer_call_and_return_conditional_losses_63996o/0123456789:8?5
.?+
!?
inputs??????????
p

 
? "%?"
?
0?????????8
? ?
E__inference_sequential_layer_call_and_return_conditional_losses_63075c9?6
/?,
"?
input_1??????????
p 

 
? "&?#
?
0??????????
? 