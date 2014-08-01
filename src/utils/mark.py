import numpy as np
import cv2
import math

def draw_machine_mark(size, location, image):
    '''
    This function draws a machine mark on the image
    '''
    original = cv2.imread("assets/machine_black_red.png", -1)

    osize = math.sqrt(original.size)/2
    ratio = size / osize
    height, width, depth = image.shape
    timg = cv2.resize(original, (0,0), fx=ratio, fy=ratio)
    x, y = location

    image[:] *= 0.8

    x_start = max(0, x - size/2)
    x_end = min(x_start + size, width)
    x_start = min(x_end - size, x_start)
    y_start = max(0, y - size/2)
    y_end = min(y_start + size, height)
    y_start = min(y_end - size, y_start)

    roi = image[y_start : y_end, x_start : x_end]
    for c in range(0,3):
        roi[:,:,c] = timg[:,:,c] * (timg[:,:,3] / 255.0) + roi[:,:,c] * (1 - timg[:,:,3] / 255.0)
    image[y_start : y_end, x_start : x_end] = roi
