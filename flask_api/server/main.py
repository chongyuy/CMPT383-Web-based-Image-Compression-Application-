from flask import Flask
from struct import unpack
import numpy as np
from flask import Flask, jsonify
from flask_cors import CORS
import ctypes

app = Flask(__name__)
DEBUG = True
app = Flask(__name__)
app.config.from_object(__name__)
CORS(app, resources={r'/*': {'origins': '*'}})



@app.route('/get/<int:x>', methods=['GET'])
def get_image(x):
    if x == 1:
        file = open('image/earth.bmp', 'rb')
    elif x == 2:
        file = open('image/nature.bmp', 'rb')
    elif x == 3:
        file = open('image/nature_2.bmp', 'rb')
    elif x == 4:
        file = open('image/Fall.bmp', 'rb')

    head_info = file.read(18)
    biwidth = unpack("<i", file.read(4))[0]
    biheight = unpack("<i", file.read(4))[0]
    tail_info = file.read(28)
    bmp_array = []

    for height in range(biheight):
        bmp_array_row = []
        count = 0
        for width in range(biwidth):
            bmp_array_row.append(
                [unpack("<B", file.read(1))[0], unpack("<B", file.read(1))[0], unpack("<B", file.read(1))[0]])
            count = count + 3
        while count % 4 != 0:
            file.read(1)
            count = count + 1
        bmp_array.append(bmp_array_row)
    bmp_array.reverse()
    file.close()
    # Swap the positions of the red and blue channels
    for height in range(biheight):
        for width in range(biwidth):
            temp = bmp_array[height][width][0]
            bmp_array[height][width][0] = bmp_array[height][width][2]
            bmp_array[height][width][2] = temp
            bmp_array = np.asarray(bmp_array)
    s1 = np.ndarray.tolist(bmp_array)
    data = {
        "image_array": s1,
        "height": height,
        "width": width
    }
    return jsonify(data)


if __name__ == '__main__':
    app.run()
