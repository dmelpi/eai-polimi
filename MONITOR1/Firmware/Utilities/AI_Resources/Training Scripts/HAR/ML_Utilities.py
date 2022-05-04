import onnxruntime as rt
import numpy
import json
from onnx import ModelProto
from numpy import loadtxt
from numpy import savetxt
from skl2onnx import to_onnx
from google.protobuf.json_format import MessageToJson


eps = 10^(-9)

def model_conversion_to_onnx(clf, X_train, onx_out_file, onx_json_file):
    #from skl2onnx import convert_sklearn
    #from skl2onnx.common.data_types import FloatTensorType
    #initial_type = [('float_input', FloatTensorType([None, 4]))]
    #onx = convert_sklearn(clf, initial_types=initial_type)

    onx = to_onnx(clf, X_train[:1].astype(numpy.float32), options = {'zipmap' : False})
    with open(onx_out_file, "wb") as f:
        f.write(onx.SerializeToString())
    s = MessageToJson(onx)
    onx_json = json.loads(s)
    onx_str = json.dumps(onx_json)
    with open(onx_json_file, 'w') as outfile:
        json.dump(onx_json, outfile, indent=4, ensure_ascii=False)
    # print(onx)


class Flags(object):
    """Store configuration flags."""
    def __init__(self, confdict):
        for key, value in confdict.items():
            setattr(self, key, value)


def diff(p1, p2):
    p1 = p1.ravel()
    p2 = p2.ravel()
    d = numpy.abs(p2 - p1)
    return d.max(), (d / (numpy.abs(p1) + eps)).max()


def get_model(onnx_file):
    onx = ModelProto()
    with open(onnx_file, mode='rb') as f:
        content = f.read()
        onx.ParseFromString(content)
    #    print(onx)
    sess = rt.InferenceSession(onnx_file)
    return onx, sess


def predict_onnx(sess, input_file):
    input_name = sess.get_inputs()[0].name
    label_name = sess.get_outputs()[0].name
    score_name = sess.get_outputs()[1].name
    X_train = loadtxt(input_file, delimiter=',')
    onx_pred = sess.run([label_name], {input_name: X_train.astype(numpy.float32)})[0]
    onx_score = sess.run([score_name], {input_name: X_train.astype(numpy.float32)})[0]
    return onx_pred, onx_score


def onnx_gen_output_files(onnx_pred_file, onnx_score_file, onx_pred, onx_score):
    savetxt(onnx_pred_file, onx_pred, fmt='%.0f', delimiter=',')
    savetxt(onnx_score_file, onx_score, fmt='%.6f', delimiter=',')

