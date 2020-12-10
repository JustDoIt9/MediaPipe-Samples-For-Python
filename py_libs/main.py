import os
import time
import struct
import cv2
import json

import numpy as np

import ci_detector as CIdetector
import emotion_detector as Emotiondetector
import pulse_detector as Pulsedetector

_r_fd = int(os.getenv("PY_READ_FD"))
_w_fd = int(os.getenv("PY_WRITE_FD"))


_r_pipe = os.fdopen(_r_fd, 'rb', 0)
_w_pipe = os.fdopen(_w_fd, 'wb', 0)

_isEnded = False

class NumpyEncoder(json.JSONEncoder):
    """ Special json encoder for numpy types """

    def default(self, obj):
        if isinstance(obj, np.integer):
            return int(obj)
        elif isinstance(obj, np.floating):
            return float(obj)
        elif isinstance(obj, np.ndarray):
            return obj.tolist()
        return json.JSONEncoder.default(self, obj)


def _read_n(f, n, decode):
    buf = ''
    while n > 0:
        data = f.read(n)
        if data == '':
            raise RuntimeError('unexpected EOF')
        if (decode == True):
            buf += data.decode()
        else:
            buf = data
        n -= len(data)
    return buf


def _api_get():

    # Response comes as [resultSize][resultString]
    buf = _read_n(_r_pipe, 4, False)
    msg_size = struct.unpack('<I', buf)[0]
    # print('py - message size : ', msg_size)

    json_data = _read_n(_r_pipe, msg_size, False)
    # print('py - face_rect : ', json_data)

    buf = _read_n(_r_pipe, 4, False)
    msg_size = struct.unpack('<I', buf)[0]
    # print('py - message size : ', msg_size)

    frame = _read_n(_r_pipe, msg_size, False)
    # print('py - message read end : ', msg_size)

    if frame == "":
        raise Exception(frame)

    return [json_data, frame]


def request_frame(msg):
    # send mat request
    # print('py - sent request')
    msg_size = struct.pack('<I', len(msg))
    _w_pipe.write(msg_size)
    _w_pipe.write(msg.encode())

    # receive data
    # print('py - receiving message')
    [json_data, buf] = _api_get()

    if json_data == 'close':
        _isEnded = True
        return 

    data = json.loads(json_data)
    
    img_np = np.frombuffer(buf, dtype=np.uint8)
    img_np_adj = np.reshape(img_np, (mat_rect["rows"], mat_rect["cols"], 3))

    return img_np_adj


def main():
    print('py - Script Starting')

    result = json.dumps({'status': 'ready'})

    while not _isEnded:
        # request frame data
        img_np_adj = request_frame(result)

        try:
            # your code here ...
            result = 'from python'

            result = json.dumps(
                {'result': result, 'status': 'ready'}, cls=NumpyEncoder)

            cv2.imshow('python', img_np_adj)
            cv2.waitKey(5)

        except Exception as ex:
            print("py - exception", ex)
            continue

        time.sleep(0.05)


if __name__ == "__main__":
    main()
