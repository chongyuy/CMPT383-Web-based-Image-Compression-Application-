import ctypes
import numpy as np
from struct import unpack

ll = ctypes.cdll.LoadLibrary
lib = ll("./name.so")


def get_image():
    file = open('../server/image/nature.bmp', 'rb')

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
    print(biheight)
    # Swap the positions of the red and blue channels
    for height in range(biheight):
        for width in range(biwidth):
            temp = bmp_array[height][width][0]
            bmp_array[height][width][0] = bmp_array[height][width][2]
            bmp_array[height][width][2] = temp
            bmp_array = np.asarray(bmp_array)


    return biheight, biwidth, bmp_array

height,width,arr = get_image()
arr = arr.flatten()
arr = arr.tolist()
# print(arr)
# print(arr)
# pyarray = [1., 2., 3., 4., 5.1]
# print(type(pyarray))
carray = (ctypes.c_int * len(arr))(*arr)
print(len(arr))
lib.change_array(carray, height,width)
print(np.array(carray))